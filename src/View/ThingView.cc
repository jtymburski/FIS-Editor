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

  /* Image label */
  lbl_image = new QLabel(this);
  lbl_image->setMinimumSize(250, 250);
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
void ThingView::editThing()
{
  EditorThing* current = getSelected();

  /* Delete the old and create the new dialog */
  if(thing_dialog != NULL)
    delete thing_dialog;
  thing_dialog = new ThingDialog(current, this);
  // TODO: Connect once the thing pop-up is complete
  //connect(thing_dialog, SIGNAL(ok()), this, SLOT(updateList()));
  thing_dialog->show();
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

  thing_list->clear();
  if(editor_map != NULL)
  {
    for(int i = 0; i < editor_map->getThingCount(); i++)
      thing_list->addItem(editor_map->getThingByIndex(i)->getNameList());
    editor_map->updateAll();
  }

  thing_list->setCurrentRow(index);
  update();
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

/* The current row changes in the list widget */
void ThingView::currentRowChanged(int index)
{
  if(editor_map != NULL)
  {
    /* Blank the labels */
    lbl_id->setText("ID:");
    lbl_image->setPixmap(QPixmap());
    lbl_name->setText("Name:");
    lbl_size->setText("Size:");

    EditorThing* thing = editor_map->getThingByIndex(index);
    if(thing != NULL)
    {
      /* Set the labels */
      lbl_id->setText("ID: " + QString::number(thing->getID()));
      lbl_name->setText("Name: " + thing->getName());

      /* If matrix is valid, set the remaining info */
      if(thing->getMatrix() != NULL)
      {
        lbl_image->setPixmap(thing->getMatrix()->getSnapshot(lbl_image->width(),
                                                          lbl_image->height()));
        lbl_size->setText("Size: " +
                          QString::number(thing->getMatrix()->getWidth()) +
                          "W x " +
                          QString::number(thing->getMatrix()->getHeight()) +
                          "H");
      }
    }
  }
}

/* An item in the list was double clicked */
void ThingView::itemDoubleClicked(QListWidgetItem*)
{
  editThing();
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
  editor_map = map;
  updateList();
}
