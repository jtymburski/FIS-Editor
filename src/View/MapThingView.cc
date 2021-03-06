/*******************************************************************************
 * Class Name: MapThingView
 * Date Created: February 13, 2015
 * Inheritance: QWidget
 * Description: The side toolbar in the map database that gives access to the
 *              map things.
 ******************************************************************************/
#include "View/MapThingView.h"
#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function. Creates the thing view, which is the
 *              side bar within the left map database. Controls things (bases
 *              and instances).
 *
 * Inputs: QWidget* parent - the parent widget
 */
MapThingView::MapThingView(QWidget* parent) : QWidget(parent)
{
  /* Initialize variables */
  editor_map = nullptr;
  instance_dialog = nullptr;
  thing_dialog = nullptr;

  /* Create the layout */
  createLayout();
}

/*
 * Description: Destructor function
 */
MapThingView::~MapThingView()
{

}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: Adds thing to the base list. Triggered by the "new thing"
 *              press. The ID is the next available.
 *
 * Inputs: EditorMapThing* thing - the new thing to add (ID not set yet)
 * Output: bool - true if added successfully
 */
bool MapThingView::addThing(EditorMapThing* thing)
{
  if(editor_map != nullptr)
  {
    /* Try and get the ID */
    int id = editor_map->getNextThingID();
    if(id >= 0)
    {
      /* Sets the id and puts the thing in the library */
      thing->setID(id);
      thing->setTileIcons(editor_map->getTileIcons());
      int index = editor_map->setThing(thing);

      /* Increments the id tracker and updates view */
      updateList();
      thing_list->setCurrentRow(index);

      return true;
    }
    else
    {
      delete thing;
      QMessageBox::information(this, "No Space Remaining",
                      "The max limit of thing bases has been reached");
    }
  }
  return false;
}

/*
 * Description: Creates the dialog layout with QT functional widgets.
 *
 * Inputs: none
 * Output: none
 */
void MapThingView::createLayout()
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
  connect(thing_instances, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
          this, SLOT(instanceDoubleClicked(QListWidgetItem*)));
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
  lbl_image->setStyleSheet("border: 1px solid #a8a8a8");
  layout->addWidget(lbl_image, 0, Qt::AlignHCenter);

  /* Name label */
  lbl_name = new QLabel("", this);
  layout->addWidget(lbl_name, 0, Qt::AlignHCenter);

  /* ID label */
  lbl_id = new QLabel("", this);
  layout->addWidget(lbl_id, 0, Qt::AlignHCenter);

  /* Size label */
  lbl_size = new QLabel("", this);
  layout->addWidget(lbl_size, 0, Qt::AlignHCenter);
}

/*
 * Description: Deletes the base dialog, if it exists.
 *
 * Inputs: none
 * Output: none
 */
void MapThingView::deletePopBase()
{
  if(thing_dialog != nullptr)
  {
    disconnect(thing_dialog, SIGNAL(ok()), this, SLOT(updateThings()));
    disconnect(thing_dialog, SIGNAL(selectTile(EditorEnumDb::MapObjectMode)),
               this, SIGNAL(selectTile(EditorEnumDb::MapObjectMode)));
    delete thing_dialog;
    thing_dialog = nullptr;
  }
}

/*
 * Description: Deletes the instance dialog, if it exists.
 *
 * Inputs: none
 * Output: none
 */
void MapThingView::deletePopInstance()
{
  if(instance_dialog != nullptr)
  {
    disconnect(instance_dialog, SIGNAL(ok(QString)),
               this, SLOT(thingInstanceUpdate(QString)));
    disconnect(instance_dialog,
               SIGNAL(selectTile(EditorEnumDb::MapObjectMode)),
               this, SIGNAL(selectTile(EditorEnumDb::MapObjectMode)));
    disconnect(instance_dialog, SIGNAL(editBase(EditorMapThing*)),
               this, SLOT(editBaseThing(EditorMapThing*)));
    delete instance_dialog;
    instance_dialog = nullptr;
  }
}

/*
 * Description: Opens the edit thing dialog, on an existing thing in the list
 *              (either base or instance).
 *
 * Inputs: EditorMapThing* sub_thing - the thing to edit
 * Output: none
 */
void MapThingView::editThing(EditorMapThing* sub_thing)
{
  EditorMapThing* current = getSelected();
  if(sub_thing != NULL)
    current = sub_thing;

  /* Delete all existing pop-ups first */
  deletePopBase();
  deletePopInstance();

  /* -- Is an instance -- */
  if(current->getBaseThing() != NULL)
  {
    /* Create the new instance dialog */
    instance_dialog = new InstanceDialog(current, this);
    connect(instance_dialog, SIGNAL(ok(QString)),
            this, SLOT(thingInstanceUpdate(QString)));
    connect(instance_dialog, SIGNAL(selectTile(EditorEnumDb::MapObjectMode)),
            this, SIGNAL(selectTile(EditorEnumDb::MapObjectMode)));
    connect(instance_dialog, SIGNAL(editBase(EditorMapThing*)),
            this, SLOT(editBaseThing(EditorMapThing*)));
    instance_dialog->show();
  }
  /* -- Is a base -- */
  else
  {
    /* Create the new thing dialog */
    thing_dialog = new ThingDialog(current, this);
    connect(thing_dialog, SIGNAL(ok()), this, SLOT(updateThings()));
    connect(thing_dialog, SIGNAL(selectTile(EditorEnumDb::MapObjectMode)),
            this, SIGNAL(selectTile(EditorEnumDb::MapObjectMode)));
    thing_dialog->show();
  }

  /* Fills the dialogs with data */
  emit fillWithData(EditorEnumDb::THING_VIEW);
}

/*
 * Description: Updates the info in the QT widgets within the view.
 *
 * Inputs: none
 * Output: none
 */
void MapThingView::updateInfo()
{
  if(editor_map != NULL)
  {
    /* Blank the labels */
    lbl_id->setText("");
    lbl_image->setPixmap(QPixmap());
    lbl_name->setText("");
    lbl_size->setText("");

    EditorMapThing* thing = getSelected();
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

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

/*
 * Description: Slot which triggers when the top list changes. Updates the
 *              selected thing.
 *
 * Inputs: int index - the new index in the list
 * Output: none
 */
void MapThingView::currentRowChanged(int index)
{
  if(editor_map != NULL)
  {
    editor_map->setCurrentThing(index);
    editor_map->setHoverThing(-1);
    if(editor_map->getHoverInfo()->active_layer != EditorEnumDb::THING)
      emit changeLayer(EditorEnumDb::THING);
  }

  updateInfo();
  thing_instances->clearSelection();
}

/*
 * Description: Slot which triggers when an instance is deleted, from a right
 *              click select on the instance.
 *
 * Inputs: none
 * Output: none
 */
void MapThingView::deleteInstance()
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

/*
 * Description: Triggers by instance dialog to edit base of the instant.
 *
 * Inputs: EditorMapThing* base - the base to edit
 * Output: none
 */
void MapThingView::editBaseThing(EditorMapThing* base)
{
  /* Trigger edit on base */
  if(base != NULL)
    editThing(base);
}

/*
 * Description: Triggers to edit the hover tile thing instance at the index.
 *
 * Inputs: int index - the depth index on the tile
 * Output: none
 */
void MapThingView::editHoverInstance(int index)
{
  if(editor_map != NULL)
  {
    /* Check if hover tile is valid */
    EditorTile* t = editor_map->getHoverInfo()->hover_tile;
    if(t != NULL)
    {
      /* Check if thing at index is valid */
      EditorMapThing* thing = t->getThing(index);
      if(thing != NULL)
        editThing(thing);
    }
  }
}

/*
 * Description: Slot which triggers when an instance is edited, from a right
 *              click select on the instance. Opens up edit dialog.
 *
 * Inputs: none
 * Output: none
 */
void MapThingView::editInstance()
{
  if(thing_instances->currentItem() != NULL)
  {
    int id = getInstanceID(thing_instances->currentItem()->text());

    EditorMapThing* thing = editor_map->getThing(
                                           id, editor_map->getCurrentMap()->id);
    if(thing != NULL)
      editThing(thing);
  }
}

/*
 * Description: Instance list item double clicked. Just triggers the edit
 *              instance action.
 *
 * Inputs: QListWidgetItem* - not used
 * Output: none
 */
void MapThingView::instanceDoubleClicked(QListWidgetItem*)
{
  editInstance();
}

/*
 * Description: Slot which triggers a right click menu when an instance is
 *              right clicked within the list. Offers delete and edit actions.
 *
 * Inputs: const QPoint & pos - the position right clicked in widget
 * Output: none
 */
void MapThingView::instanceMenu(const QPoint & pos)
{
  QListWidgetItem* item = thing_instances->itemAt(pos);
  if(item != NULL)
    rightclick_menu->exec(QCursor::pos());
}

/*
 * Description: Slot which triggers when a row in the instance list of things
 *              changes. This changes which thing is hovered on the map.
 *
 * Inputs: int index - the new index of the row in the list
 *         bool lock_viewport - lock the viewport from jumping to thing.
 *                              Default to false.
 * Output: none
 */
void MapThingView::instanceRowChanged(int index, bool lock_viewport)
{
  if(index >= 0 && editor_map != NULL)
  {
    /* Set the hover thing in the class */
    int thing_id = getInstanceID(thing_instances->currentItem()->text());
    if(editor_map->setHoverThing(thing_id))
    {
      EditorMapThing* thing = editor_map->getThing(thing_id,
                                              editor_map->getCurrentMap()->id);
      if(thing != NULL)
      {
        if(!lock_viewport)
          emit ensureVisible(editor_map->getCurrentMap()->tiles[thing->getX()]
                                                               [thing->getY()]);

        /* Select the base in the list */
        if(thing->getBaseThing() != NULL)
        {
          /* Get index and block signals */
          int index = editor_map->getThingIndex(thing->getBaseThing()->getID());
          thing_list->blockSignals(true);

          /* Processing */
          thing_list->setCurrentRow(index);
          editor_map->setCurrentThing(index);
          if(editor_map->getHoverInfo()->active_layer != EditorEnumDb::THING)
            emit changeLayer(EditorEnumDb::THING);

          /* Unblock and update */
          thing_list->blockSignals(false);
          updateInfo();
        }
      }
    }
  }
}

/*
 * Description: Slot which triggers when an item in the base list is double
 *              clicked. Opens a pop-up for the thing to be edited.
 *
 * Inputs: QListWidgetItem* - not used
 * Output: none
 */
void MapThingView::itemDoubleClicked(QListWidgetItem*)
{
  editThing();
}

/*
 * Description: Slot which triggers to update the instances in the view and
 *              put them in the list. Triggers on change events, such as editing
 *              and deleting instances, and on initial setup
 *
 * Inputs: QString name_list - the list of the added thing. Blank if N/A
 * Output: none
 */
void MapThingView::thingInstanceUpdate(QString name_list)
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

  /* If name list is not blank, select the name in the list */
  if(!name_list.isEmpty())
  {
    for(int i = 0; i < thing_instances->count(); i++)
    {
      if(thing_instances->item(i)->text() == name_list)
      {
        thing_instances->setCurrentRow(i);
        instanceRowChanged(i, true);
      }
    }
  }

  thing_instances->blockSignals(false);
}

/*
 * Description: Refreshes the Editor Thing list.
 *
 * Inputs: none
 * Output: none
 */
void MapThingView::updateList()
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
    thingInstanceUpdate("");
  }

  thing_list->setCurrentRow(index);
  updateInfo();
  update();
}

/*
 * Description: Slot which triggers to update a thing upeon completion by the
 *              thing dialog (on ok()). This will also update all instances of
 *              the object, deleting as necessary.
 *
 * Inputs: none
 * Output: none
 */
void MapThingView::updateThings()
{
  /* Update the thing in the pop-up */
  if(thing_dialog != NULL)
  {
    editor_map->tilesThingRemove();
    thing_dialog->updateOriginal();
    editor_map->tilesThingAdd();
  }

  /* Update the list in the view */
  updateList();
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Deletes the selected thing in the base list. Triggered by
 *              parent.
 *
 * Inputs: none
 * Output: bool - returns true if thing deleted.
 */
bool MapThingView::deleteThing()
{
  EditorMapThing* selected = getSelected();
  bool success = false;

  /* If valid, proceed deletion */
  if(selected != NULL)
  {
    /* Create warning about deleting */
    QMessageBox msg_box;
    msg_box.setText("Deleting base thing " + selected->getNameList());
    msg_box.setInformativeText(
                  "This will delete ALL instances of this base. Are you sure?");
    msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    if(msg_box.exec() == QMessageBox::Yes)
    {
      /* Proceed with deletion */
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
  }

  return success;
}

/*
 * Description: Duplicates the selected thing in the base list. Triggered by
 *              parent.
 *
 * Inputs: none
 * Output: bool - returns true if thing was duplicated.
 */
bool MapThingView::duplicateThing()
{
  EditorMapThing* selected = getSelected();

  /* If valid, proceed duplication */
  if(selected != NULL)
  {
    EditorMapThing* new_thing = new EditorMapThing(*selected);
    if(addThing(new_thing))
      return true;
  }

  return false;
}

/*
 * Description: Returns which thing is selected in the base list.
 *
 * Inputs: none
 * Output: EditorMapThing* - selected thing. NULL if none selected
 */
EditorMapThing* MapThingView::getSelected()
{
  int index = thing_list->currentRow();
  EditorMapThing* thing = NULL;

  /* Check the validity */
  if(editor_map != NULL)
    thing = editor_map->getThingByIndex(index);

  return thing;
}

/*
 * Description: Returns the editor map, which contains all things and instances.
 *
 * Inputs: none
 * Output: EditorMap* - pointer to the editor map. NULL if none set.
 */
EditorMap* MapThingView::getEditorMap()
{
  return editor_map;
}

/*
 * Description: Imports a new thing into the base list. Triggered by
 *              parent.
 *
 * Inputs: none
 * Output: none
 */
void MapThingView::importThing()
{
  // TODO: Future
  QMessageBox::information(this, "Notification",
                           "Coming soon to a production near you!");
}

/*
 * Description: Creates a new thing into the base list. Triggered by
 *              parent. Proceeds to call edit on the new thing and opens
 *              ThingDialog.
 *
 * Inputs: none
 * Output: none
 */
void MapThingView::newThing()
{
  EditorMapThing* new_thing = new EditorMapThing();
  if(addThing(new_thing))
    editThing();
}

/*
 * Description: Sets the control editor map. This contains the things, sprites
 *              and all data relevant to display.
 *
 * Inputs: EditorMap* map - the map to use for control
 * Output: none
 */
void MapThingView::setEditorMap(EditorMap* map)
{
  /* If existing editor map is not NULL, undo */
  if(editor_map != NULL)
  {
    disconnect(editor_map, SIGNAL(thingInstanceChanged(QString)),
               this, SLOT(thingInstanceUpdate(QString)));
  }

  editor_map = map;

  /* If new map is not NULL, reconnect */
  if(editor_map != NULL)
  {
    connect(editor_map, SIGNAL(thingInstanceChanged(QString)),
            this, SLOT(thingInstanceUpdate(QString)));
  }

  /* Finally, update list */
  updateList();
}

/*
 * Description: Sets the list of items, used for event creation
 *
 * Inputs: QList<QString> - list of all items (for give item event)
 * Output: none
 */
void MapThingView::updateListItems(QList<QString> list)
{
  if(thing_dialog != nullptr)
    thing_dialog->setListItems(list);
  if(instance_dialog != nullptr)
    instance_dialog->setListItems(list);
}

/*
 * Description: Sets the list of maps, used for event creation
 *
 * Inputs: QList<QString> - list of all maps (for change map event)
 * Output: none
 */
void MapThingView::updateListMaps(QList<QString> list)
{
  if(thing_dialog != nullptr)
    thing_dialog->setListMaps(list);
  if(instance_dialog != nullptr)
    instance_dialog->setListMaps(list);
}

/*
 * Description: Sets the list of things, ios, items, persons, npcs, used for
 *              event creation.
 *
 * Inputs: QList<QString> things - list of all map things
 *         QList<QPair<QString,QString>> ios - list of all map ios
 *         QList<QString> items - list of all map items
 *         QList<QString> persons - list of all map persons
 *         QList<QString> npcs - list of all map npcs
 * Output: none
 */
void MapThingView::updateListMapThings(QList<QString> things,
                                       QList<QPair<QString,QString>> ios,
                                       QList<QString> items,
                                       QList<QString> persons,
                                       QList<QString> npcs)
{
  if(thing_dialog != nullptr)
    thing_dialog->setListMapThings(things, ios, items, persons, npcs);
  if(instance_dialog != nullptr)
    instance_dialog->setListMapThings(things, ios, items, persons, npcs);
}

/*
 * Description: Sets the list of maps, used for the party dropdown
 *
 * Inputs: QList<QString> - list of all parties (for selected dropdown)
 * Output: none
 */
void MapThingView::updateListParties(QList<QString> list)
{
  if(instance_dialog != nullptr)
    instance_dialog->setListParties(list);
}

/*
 * Description: Sets the list of sounds, used for the sound dropdown
 *
 * Inputs: QList<QString> list - list of all sound resources available
 * Output: none
 */
void MapThingView::updateListSounds(QList<QString> list)
{
  if(thing_dialog != nullptr)
    thing_dialog->setListSounds(list);
  if(instance_dialog != nullptr)
    instance_dialog->setListSounds(list);
}

/*
 * Description: Sets the list of sub-maps, used for event creation.
 *
 * Inputs: QList<QString> - list of all sub-maps (for teleport event)
 * Output: none
 */
void MapThingView::updateListSubmaps(QList<QString> list)
{
  if(thing_dialog != nullptr)
    thing_dialog->setListSubmaps(list);
  if(instance_dialog != nullptr)
    instance_dialog->setListSubmaps(list);
}

/*
 * Description: Updates the event embedded in the thing dialog with the
 *              selected tile. Used for event creation. Called by parent.
 *
 * Inputs: int id - the id of the sub-map
 *         int x, y - the coordinates in the map
 * Output: none
 */
void MapThingView::updateSelectedTile(int id, int x, int y)
{
  if(thing_dialog != NULL)
    thing_dialog->updateSelectedTile(id, x, y);
  if(instance_dialog != NULL)
    instance_dialog->updateSelectedTile(id, x, y);
}

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Returns the instance ID from the text in the list.
 *
 * Inputs: QString text - list text for instances
 * Output: int - the id of the thing. -1 if failed.
 */
int MapThingView::getInstanceID(QString text)
{
  /* Primary split on colon */
  QStringList main_split = text.split(':');
  if(main_split.size() >= 2)
  {
    /* Split to find the number */
    QStringList front = main_split.front().split('(');
    if(front.size() == 2)
    {
      QStringList back = front.back().split(')');
      if(back.size() == 2)
      {
        return back.front().toInt();
      }
    }
  }

  return -1;
}
