/*******************************************************************************
 * Class Name: MapNPCView
 * Date Created: March 31, 2015
 * Inheritance: QWidget
 * Description: The side toolbar in the map database that gives access to the
 *              map npcs.
 ******************************************************************************/
#include "View/MapNPCView.h"
#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function. Creates the npc view, which is the
 *              side bar within the left map database. Controls npcs (bases
 *              and instances).
 *
 * Inputs: QWidget* parent - the parent widget
 */
MapNPCView::MapNPCView(QWidget* parent) : QWidget(parent)
{
  /* Initialize variables */
  editor_map = NULL;
  instance_dialog = NULL;
  npc_dialog = NULL;

  /* Create the layout */
  createLayout();
}

/*
 * Description: Destructor function
 */
MapNPCView::~MapNPCView()
{

}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: Adds npc to the base list. Triggered by the "new npc"
 *              press. The ID is the next available.
 *
 * Inputs: EditorMapNPC* npc - the new npc to add (ID not set yet)
 * Output: none
 */
void MapNPCView::addNPC(EditorMapNPC* npc)
{
  if(editor_map != NULL)
  {
    /* Sets the id and puts the npc in the library */
    npc->setID(editor_map->getNextNPCID());
    npc->setTileIcons(editor_map->getTileIcons());
    int index = editor_map->setNPC(npc);

    /* Increments the id tracker and updates view */
    updateList();
    npc_list->setCurrentRow(index);
  }
}

/*
 * Description: Creates the dialog layout with QT functional widgets.
 *
 * Inputs: none
 * Output: none
 */
void MapNPCView::createLayout()
{
  /* Layout */
  QVBoxLayout* layout = new QVBoxLayout(this);

  /* The list widget */
  npc_list = new QListWidget(this);
  connect(npc_list, SIGNAL(currentRowChanged(int)),
          this, SLOT(currentRowChanged(int)));
  connect(npc_list, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
          this, SLOT(itemDoubleClicked(QListWidgetItem*)));
  layout->addWidget(npc_list, 1);

  /* The instances widget */
  QLabel* lbl_instances = new QLabel("Instances:", this);
  layout->addWidget(lbl_instances, 0);
  npc_instances = new QListWidget(this);
  npc_instances->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(npc_instances, SIGNAL(customContextMenuRequested(QPoint)),
          this, SLOT(instanceMenu(QPoint)));
  connect(npc_instances, SIGNAL(currentRowChanged(int)),
          this, SLOT(instanceRowChanged(int)));
  connect(npc_instances, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
          this, SLOT(instanceDoubleClicked(QListWidgetItem*)));
  layout->addWidget(npc_instances, 1);

  /* Right click menu control */
  rightclick_menu = new QMenu("NPC Edit", this);
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

/*
 * Description: Deletes the base dialog, if it exists.
 *
 * Inputs: none
 * Output: none
 */
void MapNPCView::deletePopBase()
{
  if(npc_dialog != NULL)
  {
    disconnect(npc_dialog, SIGNAL(ok()), this, SLOT(updateNPCs()));
    disconnect(npc_dialog, SIGNAL(selectTile(EditorEnumDb::MapObjectMode)),
               this, SIGNAL(selectTile(EditorEnumDb::MapObjectMode)));
    delete npc_dialog;
    npc_dialog = nullptr;
  }
}

/*
 * Description: Deletes the instance dialog, if it exists.
 *
 * Inputs: none
 * Output: none
 */
void MapNPCView::deletePopInstance()
{
  if(instance_dialog != nullptr)
  {
    disconnect(instance_dialog, SIGNAL(ok(QString)),
               this, SLOT(npcInstanceUpdate(QString)));
    disconnect(instance_dialog,
               SIGNAL(selectTile(EditorEnumDb::MapObjectMode)),
               this, SIGNAL(selectTile(EditorEnumDb::MapObjectMode)));
    disconnect(instance_dialog, SIGNAL(pathEditStart(EditorNPCPath*)),
               this, SIGNAL(pathEditStart(EditorNPCPath*)));
    disconnect(instance_dialog, SIGNAL(editBase(EditorMapThing*)),
               this, SLOT(editBaseNPC(EditorMapThing*)));
    delete instance_dialog;
    instance_dialog = nullptr;
  }
}

/*
 * Description: Opens the edit npc dialog, on an existing npc in the list
 *              (either base or instance).
 *
 * Inputs: EditorMapNPC* sub_npc - the npc to edit
 * Output: none
 */
void MapNPCView::editNPC(EditorMapNPC* sub_npc)
{
  EditorMapNPC* current = getSelected();
  if(sub_npc != NULL)
    current = sub_npc;

  /* Delete all existing pop-ups first */
  deletePopBase();
  deletePopInstance();

  /* -- Is an instance -- */
  if(current->getBasePerson() != NULL)
  {
    /* Create the new instance dialog */
    instance_dialog = new InstanceDialog(current, this);
    connect(instance_dialog, SIGNAL(ok(QString)),
            this, SLOT(npcInstanceUpdate(QString)));
    connect(instance_dialog, SIGNAL(selectTile(EditorEnumDb::MapObjectMode)),
            this, SIGNAL(selectTile(EditorEnumDb::MapObjectMode)));
    connect(instance_dialog, SIGNAL(pathEditStart(EditorNPCPath*)),
            this, SIGNAL(pathEditStart(EditorNPCPath*)));
    connect(instance_dialog, SIGNAL(editBase(EditorMapThing*)),
            this, SLOT(editBaseNPC(EditorMapThing*)));
    instance_dialog->show();
  }
  /* -- Is a base -- */
  else
  {
    /* Create the new base dialog */
    npc_dialog = new PersonDialog(current, this);
    connect(npc_dialog, SIGNAL(ok()), this, SLOT(updateNPCs()));
    connect(npc_dialog, SIGNAL(selectTile(EditorEnumDb::MapObjectMode)),
            this, SIGNAL(selectTile(EditorEnumDb::MapObjectMode)));
    npc_dialog->show();
  }

  /* Fills the dialogs with data */
  emit fillWithData(EditorEnumDb::NPC_VIEW);
}

/*
 * Description: Updates the info in the QT widgets within the view.
 *
 * Inputs: none
 * Output: none
 */
void MapNPCView::updateInfo()
{
  if(editor_map != NULL)
  {
    /* Blank the labels */
    lbl_id->setText("ID:");
    lbl_image->setPixmap(QPixmap());
    lbl_name->setText("Name:");
    lbl_size->setText("Size:  |  Frames:");

    EditorMapNPC* npc = getSelected();
    if(npc != NULL)
    {
      /* Set the labels */
      lbl_id->setText("ID: " + QString::number(npc->getID()));
      lbl_name->setText("Name: " + npc->getName());

      /* If matrix is valid, set the remaining info */
      if(npc->getMatrix() != NULL)
      {
        lbl_image->setPixmap(npc->getMatrix()->getSnapshot(200, 200));
        lbl_size->setText("Size: " +
                    QString::number(npc->getMatrix()->getWidth()) +
                    "W x " +
                    QString::number(npc->getMatrix()->getHeight()) +
                    "H  |  Frames: " +
                    QString::number(npc->getMatrix()->getTrimFrames() + 1));
      }
    }
  }
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

/*
 * Description: Slot which triggers when the top list changes. Updates the
 *              selected npc.
 *
 * Inputs: int index - the new index in the list
 * Output: none
 */
void MapNPCView::currentRowChanged(int index)
{
  if(editor_map != NULL)
  {
    editor_map->setCurrentNPC(index);
    editor_map->setHoverNPC(-1);
    if(editor_map->getHoverInfo()->active_layer != EditorEnumDb::NPC)
      emit changeLayer(EditorEnumDb::NPC);
  }

  updateInfo();
  npc_instances->clearSelection();
}

/*
 * Description: Slot which triggers when an instance is deleted, from a right
 *              click select on the instance.
 *
 * Inputs: none
 * Output: none
 */
void MapNPCView::deleteInstance()
{
  if(npc_instances->currentItem() != NULL)
  {
    QString text = npc_instances->currentItem()->text();

    /* Create warning about deleting */
    QMessageBox msg_box;
    msg_box.setText("Deleting npc instance: " + text);
    msg_box.setInformativeText("Are you sure?");
    msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    if(msg_box.exec() == QMessageBox::Yes)
    {
      int index = MapThingView::getInstanceID(text);
      editor_map->unsetNPC(index, true);
    }
  }
}

/*
 * Description: Triggers by instance dialog to edit base of the instant.
 *
 * Inputs: EditorMapThing* base - the base npc to edit
 * Output: none
 */
void MapNPCView::editBaseNPC(EditorMapThing* base)
{
  /* Trigger edit on base */
  if(base != NULL)
    editNPC((EditorMapNPC*)base);
}

/*
 * Description: Triggers to edit the hover tile NPC instance at the index.
 *
 * Inputs: int index - the depth index on the tile
 * Output: none
 */
void MapNPCView::editHoverInstance(int index)
{
  if(editor_map != NULL)
  {
    /* Check if hover tile is valid */
    EditorTile* t = editor_map->getHoverInfo()->hover_tile;
    if(t != NULL)
    {
      /* Check if npc at index is valid */
      EditorMapNPC* npc = t->getNPC(index);
      if(npc != NULL)
        editNPC(npc);
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
void MapNPCView::editInstance()
{
  if(npc_instances->currentItem() != NULL)
  {
    int id = MapThingView::getInstanceID(npc_instances->currentItem()->text());

    EditorMapNPC* npc = editor_map->getNPC(id, editor_map->getCurrentMap()->id);
    if(npc != NULL)
      editNPC(npc);
  }
}

/*
 * Description: Slot which triggers a right click menu when an instance is
 *              right clicked within the list. Offers delete and edit actions.
 *
 * Inputs: const QPoint & pos - the position right clicked in widget
 * Output: none
 */
void MapNPCView::instanceMenu(const QPoint & pos)
{
  QListWidgetItem* item = npc_instances->itemAt(pos);
  if(item != NULL)
    rightclick_menu->exec(QCursor::pos());
}

/*
 * Description: Instance list item double clicked. Just triggers the edit
 *              instance action.
 *
 * Inputs: QListWidgetItem* - not used
 * Output: none
 */
void MapNPCView::instanceDoubleClicked(QListWidgetItem*)
{
  editInstance();
}

/*
 * Description: Slot which triggers when a row in the instance list of npcs
 *              changes. This changes which npc is hovered on the map.
 *
 * Inputs: int index - the new index of the row in the list
 *         bool lock_viewport - lock the viewport from jumping to NPC. Default
 *                              to false.
 * Output: none
 */
void MapNPCView::instanceRowChanged(int index, bool lock_viewport)
{
  if(index >= 0 && editor_map != NULL)
  {
    /* Set the hover NPC in the class */
    int npc_id = MapThingView::getInstanceID(
                                          npc_instances->currentItem()->text());
    if(editor_map->setHoverNPC(npc_id))
    {
      EditorMapNPC* npc = editor_map->getNPC(npc_id,
                                             editor_map->getCurrentMap()->id);
      if(npc != NULL)
      {
        if(!lock_viewport)
          emit ensureVisible(editor_map->getCurrentMap()->tiles[npc->getX()]
                                                               [npc->getY()]);

        /* Select the base in the list */
        if(npc->getBaseNPC() != NULL)
        {
          /* Get index and block signals */
          int index = editor_map->getNPCIndex(npc->getBaseNPC()->getID());
          npc_list->blockSignals(true);

          /* Processing */
          npc_list->setCurrentRow(index);
          editor_map->setCurrentNPC(index);
          if(editor_map->getHoverInfo()->active_layer != EditorEnumDb::NPC)
            emit changeLayer(EditorEnumDb::NPC);

          /* Unblock and update */
          npc_list->blockSignals(false);
          updateInfo();
        }
      }
    }
  }
}

/*
 * Description: Slot which triggers when an item in the base list is double
 *              clicked. Opens a pop-up for the npc to be edited.
 *
 * Inputs: QListWidgetItem* - not used
 * Output: none
 */
void MapNPCView::itemDoubleClicked(QListWidgetItem*)
{
  editNPC();
}

/*
 * Description: Slot which triggers to update the instances in the view and
 *              put them in the list. Triggers on change events, such as editing
 *              and deleting instances, and on initial setup
 *
 * Inputs: QString name_list - the list of the added npc. Blank if N/A
 * Output: none
 */
void MapNPCView::npcInstanceUpdate(QString name_list)
{
  npc_instances->blockSignals(true);
  npc_instances->clearSelection();
  npc_instances->clearFocus();
  npc_instances->clear();

  if(editor_map != NULL)
  {
    /* Set up the instances list */
    int sub_index = editor_map->getCurrentMapIndex();
    if(sub_index >= 0)
    {
      for(int i = 0; i < editor_map->getNPCCount(sub_index); i++)
      {
        npc_instances->addItem(
                        editor_map->getNPCByIndex(i, sub_index)->getNameList());
      }
      npc_instances->sortItems();
    }
  }

  npc_instances->clearSelection();
  npc_instances->clearFocus();

  /* If name list is not blank, select the name in the list */
  if(!name_list.isEmpty())
  {
    for(int i = 0; i < npc_instances->count(); i++)
    {
      if(npc_instances->item(i)->text() == name_list)
      {
        npc_instances->setCurrentRow(i);
        instanceRowChanged(i, true);
      }
    }
  }

  npc_instances->blockSignals(false);
}

/*
 * Description: Refreshes the Editor NPC list.
 *
 * Inputs: none
 * Output: none
 */
void MapNPCView::updateList()
{
  int index = npc_list->currentRow();

  /* Set up the base list */
  npc_list->clear();
  if(editor_map != NULL)
  {
    /* Set up the base list */
    for(int i = 0; i < editor_map->getNPCCount(); i++)
      npc_list->addItem(editor_map->getNPCByIndex(i)->getNameList());
    editor_map->updateAll();

    /* Set up the instances list */
    npcInstanceUpdate("");
  }

  npc_list->setCurrentRow(index);
  updateInfo();
  update();
}

/*
 * Description: Slot which triggers to update a npc upon completion by the
 *              person dialog (on ok()). This will also update all instances of
 *              the object, deleting as necessary.
 *
 * Inputs: none
 * Output: none
 */
void MapNPCView::updateNPCs()
{
  /* Update the npc in the pop-up */
  if(npc_dialog != NULL)
  {
    editor_map->tilesNPCRemove();
    npc_dialog->updateOriginal();
    editor_map->tilesNPCAdd();
  }

  /* Update the list in the view */
  updateList();
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Deletes the selected npc in the base list. Triggered by
 *              parent.
 *
 * Inputs: none
 * Output: bool - returns true if npc deleted.
 */
bool MapNPCView::deleteNPC()
{
  EditorMapNPC* selected = getSelected();
  bool success = false;

  /* If valid, proceed deletion */
  if(selected != NULL)
  {
    /* Create warning about deleting */
    QMessageBox msg_box;
    msg_box.setText("Deleting base npc " + selected->getNameList());
    msg_box.setInformativeText(
                  "This will delete ALL instances of this base. Are you sure?");
    msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    if(msg_box.exec() == QMessageBox::Yes)
    {
      /* Proceed with deletion */
      int index = npc_list->currentRow();
      success = editor_map->unsetNPCByIndex(index);

      /* If successfull, reset list */
      if(success)
      {
        updateList();
        if(index >= npc_list->count())
          npc_list->setCurrentRow(npc_list->count() - 1);
      }
    }
  }

  return success;
}

/*
 * Description: Duplicates the selected npc in the base list. Triggered by
 *              parent.
 *
 * Inputs: none
 * Output: bool - returns true if npc was duplicated.
 */
bool MapNPCView::duplicateNPC()
{
  EditorMapNPC* selected = getSelected();

  /* If valid, proceed duplication */
  if(selected != NULL)
  {
    EditorMapNPC* new_npc = new EditorMapNPC(*selected);
    addNPC(new_npc);
    return true;
  }

  return false;
}

/*
 * Description: Returns which npc is selected in the base list.
 *
 * Inputs: none
 * Output: EditorMapNPC* - selected npc. NULL if none selected
 */
EditorMapNPC* MapNPCView::getSelected()
{
  int index = npc_list->currentRow();
  EditorMapNPC* npc = NULL;

  /* Check the validity */
  if(editor_map != NULL)
    npc = editor_map->getNPCByIndex(index);

  return npc;
}

/*
 * Description: Returns the editor map, which contains all npcs and instances
 *
 * Inputs: none
 * Output: EditorMap* - pointer to the editor map. NULL if none set.
 */
EditorMap* MapNPCView::getEditorMap()
{
  return editor_map;
}

/*
 * Description: Imports a new npc into the base list. Triggered by
 *              parent.
 *
 * Inputs: none
 * Output: none
 */
void MapNPCView::importNPC()
{
  // TODO: Future
  QMessageBox::information(this, "Notification",
                           "Coming soon to a production near you!");
}

/*
 * Description: Creates a new npc into the base list. Triggered by
 *              parent. Proceeds to call edit on the new npc and opens
 *              PersonDialog.
 *
 * Inputs: none
 * Output: none
 */
void MapNPCView::newNPC()
{
  EditorMapNPC* new_npc = new EditorMapNPC();
  addNPC(new_npc);
  editNPC();
}

/*
 * Description: Sets the control editor map. This contains the npcs, sprites
 *              and all data relevant to display.
 *
 * Inputs: EditorMap* map - the map to use for control
 * Output: none
 */
void MapNPCView::setEditorMap(EditorMap* map)
{
  /* If existing editor map is not NULL, undo */
  if(editor_map != NULL)
  {
    disconnect(editor_map, SIGNAL(npcInstanceChanged(QString)),
               this, SLOT(npcInstanceUpdate(QString)));
  }

  editor_map = map;

  /* If new map is not NULL, reconnect */
  if(editor_map != NULL)
  {
    connect(editor_map, SIGNAL(npcInstanceChanged(QString)),
            this, SLOT(npcInstanceUpdate(QString)));
  }

  /* Finally, update list */
  updateList();
}

/*
 * Description: Sets the list of items, used for event creation
 *
 * Inputs: QVector<QString> - list of all items (for give item event)
 * Output: none
 */
void MapNPCView::updateListItems(QVector<QString> list)
{
  if(npc_dialog != NULL)
    npc_dialog->setListItems(list);
  if(instance_dialog != NULL)
    instance_dialog->setListItems(list);
}

/*
 * Description: Sets the list of maps, used for event creation
 *
 * Inputs: QVector<QString> - list of all maps (for change map event)
 * Output: none
 */
void MapNPCView::updateListMaps(QVector<QString> list)
{
  if(npc_dialog != NULL)
    npc_dialog->setListMaps(list);
  if(instance_dialog != NULL)
    instance_dialog->setListMaps(list);
}

/*
 * Description: Sets the list of things, ios, items, persons, npcs, used for
 *              event creation.
 *
 * Inputs: QVector<QString> things - list of all map things
 *         QVector<QPair<QString,QString>> ios - list of all map ios
 *         QVector<QString> items - list of all map items
 *         QVector<QString> persons - list of all map persons
 *         QVector<QString> npcs - list of all map npcs
 * Output: none
 */
void MapNPCView::updateListMapThings(QVector<QString> things,
                                     QVector<QPair<QString,QString>> ios,
                                     QVector<QString> items,
                                     QVector<QString> persons,
                                     QVector<QString> npcs)
{
  if(npc_dialog != nullptr)
    npc_dialog->setListMapThings(things, ios, items, persons, npcs);
  if(instance_dialog != nullptr)
    instance_dialog->setListMapThings(things, ios, items, persons, npcs);
}

/*
 * Description: Sets the list of maps, used for the party dropdown
 *
 * Inputs: QVector<QString> - list of all parties (for selected dropdown)
 * Output: none
 */
void MapNPCView::updateListParties(QVector<QString> list)
{
  if(instance_dialog != NULL)
    instance_dialog->setListParties(list);
}

/*
 * Description: Sets the list of sounds, used for the sound dropdown
 *
 * Inputs: QList<QString> list - list of all sound resources available
 * Output: none
 */
void MapNPCView::updateListSounds(QList<QString> list)
{
  if(npc_dialog != nullptr)
    npc_dialog->setListSounds(list);
  if(instance_dialog != nullptr)
    instance_dialog->setListSounds(list);
}

/*
 * Description: Sets the list of sub-maps, used for event creation.
 *
 * Inputs: QVector<QString> - list of all sub-maps (for teleport event)
 * Output: none
 */
void MapNPCView::updateListSubmaps(QVector<QString> list)
{
  if(npc_dialog != NULL)
    npc_dialog->setListSubmaps(list);
  if(instance_dialog != NULL)
    instance_dialog->setListSubmaps(list);
}

/*
 * Description: Updates the instance dialog and notifies that the path
 *              edit has been completed. Triggered on the finish button inside
 *              the view in the MapDatabase.
 *
 * Inputs: none
 * Output: none
 */
void MapNPCView::updatePathFinished()
{
  if(instance_dialog != NULL)
    instance_dialog->updatePathFinished();
}

/*
 * Description: Updates the event embedded in the person dialog with the
 *              selected tile. Used for event creation. Called by parent.
 *
 * Inputs: int id - the id of the sub-map
 *         int x, y - the coordinates in the map
 * Output: none
 */
void MapNPCView::updateSelectedTile(int id, int x, int y)
{
  if(npc_dialog != NULL)
    npc_dialog->updateSelectedTile(id, x, y);
  if(instance_dialog != NULL)
    instance_dialog->updateSelectedTile(id, x, y);
}
