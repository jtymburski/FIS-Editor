/*******************************************************************************
 * Class Name: ThingView
 * Date Created: February 13, 2015
 * Inheritance: QWidget
 * Description: The side toolbar in the map database that gives access to the
 *              map things.
 ******************************************************************************/
#include "View/ThingView.h"
#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor Function */
ThingView::ThingView(QWidget* parent) : QWidget(parent)
{
  /* Initialize variables */
  editor_map = NULL;
  thing_dialog = NULL;

  /* Create the layout */
  createLayout();
}

/* Destructor function */
ThingView::~ThingView()
{

}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* Adds an editor thing to the stack */
void ThingView::addThing(EditorThing* thing)
{
  if(editor_map != NULL)
  {
    /* Sets the id and puts the thing in the library */
    thing->setID(editor_map->getNextThingID());
    thing->setTileIcons(editor_map->getTileIcons());
    int index = editor_map->setThing(thing);

    /* Increments the id tracker and updates view */
    updateList();
    thing_list->setCurrentRow(index);
  }
}

/* Creates the layout - only called on initial construction */
void ThingView::createLayout()
{
  /* Layout */
  QVBoxLayout* layout = new QVBoxLayout(this);

  /* The list widget */
  thing_list = new QListWidget(this);
  connect(thing_list, SIGNAL(currentRowChanged(int)),
          this, SLOT(currentRowChanged(int)));
  connect(thing_list, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
          this, SLOT(itemDoubleClicked(QListWidgetItem*)));
  layout->addWidget(thing_list, 1);

  /* The instances widget */
  QLabel* lbl_instances = new QLabel("Instances:", this);
  layout->addWidget(lbl_instances, 0);
  thing_instances = new QListWidget(this);
  thing_instances->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(thing_instances, SIGNAL(customContextMenuRequested(QPoint)),
          this, SLOT(instanceMenu(QPoint)));
  connect(thing_instances, SIGNAL(currentRowChanged(int)),
          this, SLOT(instanceRowChanged(int)));
  layout->addWidget(thing_instances, 1);

  /* Right click menu control */
  rightclick_menu = new QMenu("Thing Edit", this);
  QAction* edit_thing = new QAction("Edit", rightclick_menu);
  connect(edit_thing, SIGNAL(triggered()), this, SLOT(editInstance()));
  QAction* delete_thing = new QAction("Delete", rightclick_menu);
  connect(delete_thing, SIGNAL(triggered()), this, SLOT(deleteInstance()));
  rightclick_menu->addAction(edit_thing);
  rightclick_menu->addAction(delete_thing);
  rightclick_menu->hide();

  /* Image label */
  lbl_image = new QLabel(this);
  lbl_image->setMinimumSize(200, 200);
  lbl_image->setAlignment(Qt::AlignCenter);
  lbl_image->setStyleSheet("border: 1px solid black");
  layout->addWidget(lbl_image, 0, Qt::AlignHCenter);

  /* Name label */
  lbl_name = new QLabel("Name:", this);
  layout->addWidget(lbl_name, 0, Qt::AlignHCenter);

  /* ID label */
  lbl_id = new QLabel("ID:", this);
  layout->addWidget(lbl_id, 0, Qt::AlignHCenter);

  /* Size label */
  lbl_size = new QLabel("Size:", this);
  layout->addWidget(lbl_size, 0, Qt::AlignHCenter);
}

/* Opens the thing editing dialog */
void ThingView::editThing(EditorThing* sub_thing)
{
  EditorThing* current = getSelected();
  if(sub_thing != NULL)
    current = sub_thing;

  /* Delete the old and create the new dialog */
  if(thing_dialog != NULL)
  {
    disconnect(thing_dialog, SIGNAL(ok()), this, SLOT(updateThings()));
    disconnect(thing_dialog, SIGNAL(selectTile(EditorEnumDb::MapViewMode)),
               this, SIGNAL(selectTile(EditorEnumDb::MapViewMode)));
    delete thing_dialog;
  }
  thing_dialog = new ThingDialog(current, this);
  connect(thing_dialog, SIGNAL(ok()), this, SLOT(updateThings()));
  connect(thing_dialog, SIGNAL(selectTile(EditorEnumDb::MapViewMode)),
          this, SIGNAL(selectTile(EditorEnumDb::MapViewMode)));
  thing_dialog->show();

  emit fillWithData(EditorEnumDb::THING_VIEW);
}

/* Refreshes the info in the lower half of the widget */
// TODO: Comment
void ThingView::updateInfo()
{
  if(editor_map != NULL)
  {
    /* Blank the labels */
    lbl_id->setText("ID:");
    lbl_image->setPixmap(QPixmap());
    lbl_name->setText("Name:");
    lbl_size->setText("Size:  |  Frames:");

    EditorThing* thing = getSelected();
    if(thing != NULL)
    {
      /* Set the labels */
      lbl_id->setText("ID: " + QString::number(thing->getID()));
      lbl_name->setText("Name: " + thing->getName());

      /* If matrix is valid, set the remaining info */
      if(thing->getMatrix() != NULL)
      {
        lbl_image->setPixmap(thing->getMatrix()->getSnapshot(200, 200));
        lbl_size->setText("Size: " +
                      QString::number(thing->getMatrix()->getWidth()) +
                      "W x " +
                      QString::number(thing->getMatrix()->getHeight()) +
                      "H  |  Frames: " +
                      QString::number(thing->getMatrix()->getTrimFrames() + 1));
      }
    }
  }
}

/*
 * Description: Refreshes the Editor Thing list.
 *
 * Inputs: none
 * Output: none
 */
void ThingView::updateList()
{
  int index = thing_list->currentRow();

  /* Set up the base list */
  thing_list->clear();
  if(editor_map != NULL)
  {
    /* Set up the base list */
    for(int i = 0; i < editor_map->getThingCount(); i++)
      thing_list->addItem(editor_map->getThingByIndex(i)->getNameList());
    editor_map->updateAll();

    /* Set up the instances list */
    thingInstanceUpdate();
  }

  thing_list->setCurrentRow(index);
  updateInfo();
  update();
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

/* The current row changes in the list widget */
void ThingView::currentRowChanged(int index)
{
  if(editor_map != NULL)
    editor_map->setCurrentThing(index);
  updateInfo();
}

/* Delete thing instance */
void ThingView::deleteInstance()
{
  if(thing_instances->currentItem() != NULL)
  {
    QString text = thing_instances->currentItem()->text();

    /* Create warning about deleting */
    QMessageBox msg_box;
    msg_box.setText("Deleting thing instance: " + text);
    msg_box.setInformativeText("Are you sure?");
    msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    if(msg_box.exec() == QMessageBox::Yes)
    {
      int index = getInstanceID(text);
      editor_map->unsetThing(index, true);
    }
  }
}

/* Edit thing instance */
void ThingView::editInstance()
{
  if(thing_instances->currentItem() != NULL)
  {
    int id = getInstanceID(thing_instances->currentItem()->text());

    EditorThing* thing = editor_map->getThing(
                                           id, editor_map->getCurrentMap()->id);
    if(thing != NULL)
    {
      editThing(thing);
    }
  }
}

/* Instance menu trigger */
void ThingView::instanceMenu(const QPoint & pos)
{
  QListWidgetItem* item = thing_instances->itemAt(pos);
  if(item != NULL)
    rightclick_menu->exec(QCursor::pos());
}

/* Instance of thing row changed */
void ThingView::instanceRowChanged(int index)
{
  if(index >= 0 && editor_map != NULL)
  {
    int thing_index = getInstanceID(thing_instances->currentItem()->text());
    if(editor_map->setHoverThing(thing_index))
      emit ensureVisible(editor_map->getHoverInfo()->selected_thing);
  }
}

/* An item in the list was double clicked */
void ThingView::itemDoubleClicked(QListWidgetItem*)
{
  editThing();
}

/* Update thing instances in view */
void ThingView::thingInstanceUpdate()
{
  thing_instances->blockSignals(true);
  thing_instances->clearSelection();
  thing_instances->clearFocus();
  thing_instances->clear();

  if(editor_map != NULL)
  {
    /* Set up the instances list */
    int sub_index = editor_map->getCurrentMapIndex();
    if(sub_index >= 0)
    {
      for(int i = 0; i < editor_map->getThingCount(sub_index); i++)
      {
        thing_instances->addItem(
                      editor_map->getThingByIndex(i, sub_index)->getNameList());
      }
      thing_instances->sortItems();
    }
  }

  thing_instances->clearSelection();
  thing_instances->clearFocus();
  thing_instances->blockSignals(false);
}

/* Updates the thing sidebar */
void ThingView::updateThings()
{
  /* Update the thing in the pop-up */
  if(thing_dialog != NULL)
  {
    editor_map->thingRemoveFromTiles();
    thing_dialog->updateOriginal();
    editor_map->thingAddToTiles();
  }

  /* Update the list in the view */
  updateList();
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
  
/* Deletes the selected thing */
bool ThingView::deleteThing()
{
  EditorThing* selected = getSelected();
  bool success = false;

  /* If valid, proceed deletion */
  if(selected != NULL)
  {
    int index = thing_list->currentRow();
    success = editor_map->unsetThingByIndex(index);

    /* If successfull, reset list */
    if(success)
    {
      updateList();
      if(index >= thing_list->count())
        thing_list->setCurrentRow(thing_list->count() - 1);
    }
  }

  return success;
}

/* Duplicates the selected thing */
bool ThingView::duplicateThing()
{
  EditorThing* selected = getSelected();

  /* If valid, proceed duplication */
  if(selected != NULL)
  {
    EditorThing* new_thing = new EditorThing(*selected);
    addThing(new_thing);
    return true;
  }

  return false;
}

/* Get current thing */
EditorThing* ThingView::getSelected()
{
  int index = thing_list->currentRow();
  EditorThing* thing = NULL;

  /* Check the validity */
  if(editor_map != NULL)
    thing = editor_map->getThingByIndex(index);

  return thing;
}

/* Gets the editor map */
EditorMap* ThingView::getEditorMap()
{
  return editor_map;
}

/* Imports a thing from another file */
void ThingView::importThing()
{
  // TODO: Future
  QMessageBox::information(this, "Notification",
                           "Coming soon to a production near you!");
}

/* Creates a new blank thing */
void ThingView::newThing()
{
  EditorThing* new_thing = new EditorThing();
  addThing(new_thing);
  editThing();
}

/* Sets the editor map, which contains the data needed */
void ThingView::setEditorMap(EditorMap* map)
{
  /* If existing editor map is not NULL, undo */
  if(editor_map != NULL)
  {
    disconnect(editor_map, SIGNAL(thingInstanceChanged()),
               this, SLOT(thingInstanceUpdate()));
  }

  editor_map = map;

  /* If new map is not NULL, reconnect */
  if(editor_map != NULL)
  {
    connect(editor_map, SIGNAL(thingInstanceChanged()),
            this, SLOT(thingInstanceUpdate()));
  }

  /* Finally, update list */
  updateList();
}

/* Updates list in thing dialog, needed for event control */
void ThingView::updateListItems(QVector<QString> list)
{
  if(thing_dialog != NULL)
    thing_dialog->getEventView()->setListItems(list);
}

/* Updates list in thing dialog, needed for event control */
void ThingView::updateListMaps(QVector<QString> list)
{
  if(thing_dialog != NULL)
    thing_dialog->getEventView()->setListMaps(list);
}

/* Updates list in thing dialog, needed for event control */
void ThingView::updateListThings(QVector<QString> list)
{
  if(thing_dialog != NULL)
    thing_dialog->getEventView()->setListThings(list);
}

/* Update the selected tile for the thing */
void ThingView::updateListSubmaps(QVector<QString> list)
{
  if(thing_dialog != NULL)
    thing_dialog->getEventView()->setListSubmaps(list);
}

/* Update the selected tile for the thing */
void ThingView::updateSelectedTile(int id, int x, int y)
{
  if(thing_dialog != NULL)
    thing_dialog->updateSelectedTile(id, x, y);
}

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/

/* Returns the instance ID */
int ThingView::getInstanceID(QString text)
{
  /* Split to find the number */
  QStringList front = text.split('(');
  if(front.size() == 2)
  {
    QStringList back = front.back().split(')');
    if(back.size() == 2)
    {
      return back.front().toInt();
    }
  }

  return -1;
}
