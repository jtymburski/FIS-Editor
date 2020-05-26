/*******************************************************************************
 * Class Name: MapDatabase
 * Date Created: December 23, 2014
 * Inheritance: QWidget
 * Description: Database on the left hand side that contains all of the sprite
 * creation/selection and editing components.
 ******************************************************************************/
#include "View/MapDatabase.h"

MapDatabase::MapDatabase(QWidget *parent) : QStackedWidget(parent)
{
  editing_map = NULL;
  mode_for_data = EditorEnumDb::RAW_VIEW;
  mode_for_tile = EditorEnumDb::RAW_VIEW;
  path_working = NULL;
  widget_mode = EditorEnumDb::NORMAL_EDIT;

  /* Set-up widgets */
  setupMain();
  setupPathEdit();
  setupTeleportSelect();

  /* Stacked widget control */
  addWidget(widget_main);
  addWidget(widget_path);
  addWidget(widget_teleport);

  /* View control */
  updateSelected(EditorEnumDb::RAW_VIEW);
  setWidgetMode(widget_mode);
}

/*
 * Description: Destructor Function
 */
MapDatabase::~MapDatabase()
{
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* Set-up main widget */
// TODO: Comment
void MapDatabase::setupMain()
{
  widget_main = new QWidget(this);

  /* Top view */
  combo_top = new QComboBox(widget_main);
  QStringList items;
  items << "Raw Images" << "Sprites" << "Things" << "Interactive Objects"
        << "Items" << "Persons" << "NPCs" << "Music" << "Battle Scenes"
        << "Lay Overs";
  combo_top->addItems(items);
  connect(combo_top, SIGNAL(currentIndexChanged(int)),
          this, SLOT(updateSelected(int)));

  /* Sets up the various views */
  view_battlescene = new MapBattleSceneView(widget_main);
  view_io = new MapIOView(widget_main);
  view_item = new MapItemView(widget_main);
  view_lays = new MapLayView(widget_main);
  view_music = new MapMusicView(widget_main);
  view_npc = new MapNPCView(widget_main);
  view_person = new MapPersonView(widget_main);
  view_raw = new RawImageView(widget_main);
  view_sprite = new SpriteView(widget_main);
  view_thing = new MapThingView(widget_main);

  /* Sprite and Raw connections */
  connect(view_raw->getToolbox(),SIGNAL(sendUpEditorSprite(EditorSprite*)),
          view_sprite,SLOT(addEditorSprite(EditorSprite*)));
  connect(view_sprite, SIGNAL(changeLayer(EditorEnumDb::Layer)),
          this, SIGNAL(changeLayer(EditorEnumDb::Layer)));
  connect(view_sprite, SIGNAL(soundFillTrigger(EditorEnumDb::MapObjectMode)),
          this, SLOT(fillWithData(EditorEnumDb::MapObjectMode)));

  /* IO connections */
  connect(view_io, SIGNAL(changeLayer(EditorEnumDb::Layer)),
          this, SIGNAL(changeLayer(EditorEnumDb::Layer)));
  connect(view_io, SIGNAL(ensureVisible(QGraphicsItem*)),
          this, SIGNAL(ensureVisible(QGraphicsItem*)));
  connect(view_io, SIGNAL(fillWithData(EditorEnumDb::MapObjectMode)),
          this, SLOT(fillWithData(EditorEnumDb::MapObjectMode)));
  connect(view_io, SIGNAL(selectTile(EditorEnumDb::MapObjectMode)),
          this, SLOT(selectTile(EditorEnumDb::MapObjectMode)));

  /* Item connections */
  connect(view_item, SIGNAL(changeLayer(EditorEnumDb::Layer)),
          this, SIGNAL(changeLayer(EditorEnumDb::Layer)));
  connect(view_item, SIGNAL(ensureVisible(QGraphicsItem*)),
          this, SIGNAL(ensureVisible(QGraphicsItem*)));
  connect(view_item, SIGNAL(fillWithData(EditorEnumDb::MapObjectMode)),
          this, SLOT(fillWithData(EditorEnumDb::MapObjectMode)));

  /* NPC connections */
  connect(view_npc, SIGNAL(changeLayer(EditorEnumDb::Layer)),
          this, SIGNAL(changeLayer(EditorEnumDb::Layer)));
  connect(view_npc, SIGNAL(ensureVisible(QGraphicsItem*)),
          this, SIGNAL(ensureVisible(QGraphicsItem*)));
  connect(view_npc, SIGNAL(fillWithData(EditorEnumDb::MapObjectMode)),
          this, SLOT(fillWithData(EditorEnumDb::MapObjectMode)));
  connect(view_npc, SIGNAL(selectTile(EditorEnumDb::MapObjectMode)),
          this, SLOT(selectTile(EditorEnumDb::MapObjectMode)));
  connect(view_npc, SIGNAL(pathEditStart(EditorNPCPath*)),
          this, SLOT(pathEditStart(EditorNPCPath*)));

  /* Person connections */
  connect(view_person, SIGNAL(changeLayer(EditorEnumDb::Layer)),
          this, SIGNAL(changeLayer(EditorEnumDb::Layer)));
  connect(view_person, SIGNAL(ensureVisible(QGraphicsItem*)),
          this, SIGNAL(ensureVisible(QGraphicsItem*)));
  connect(view_person, SIGNAL(fillWithData(EditorEnumDb::MapObjectMode)),
          this, SLOT(fillWithData(EditorEnumDb::MapObjectMode)));
  connect(view_person, SIGNAL(selectTile(EditorEnumDb::MapObjectMode)),
          this, SLOT(selectTile(EditorEnumDb::MapObjectMode)));

  /* Thing connections */
  connect(view_thing, SIGNAL(changeLayer(EditorEnumDb::Layer)),
          this, SIGNAL(changeLayer(EditorEnumDb::Layer)));
  connect(view_thing, SIGNAL(ensureVisible(QGraphicsItem*)),
          this, SIGNAL(ensureVisible(QGraphicsItem*)));
  connect(view_thing, SIGNAL(fillWithData(EditorEnumDb::MapObjectMode)),
          this, SLOT(fillWithData(EditorEnumDb::MapObjectMode)));
  connect(view_thing, SIGNAL(selectTile(EditorEnumDb::MapObjectMode)),
          this, SLOT(selectTile(EditorEnumDb::MapObjectMode)));

  /* Battle scene connections */
  connect(view_battlescene, SIGNAL(fillWithData(EditorEnumDb::MapObjectMode)),
          this, SLOT(fillWithData(EditorEnumDb::MapObjectMode)));

  /* Push buttons at the bottom of the layout */
  button_delete = new QPushButton("Delete", widget_main);
  button_duplicate = new QPushButton("Duplicate", widget_main);
  button_import = new QPushButton("Import", widget_main);
  button_new = new QPushButton("New", widget_main);
  QHBoxLayout* hlayout = new QHBoxLayout();
  hlayout->addWidget(button_new);
  hlayout->addWidget(button_delete);
  hlayout->addWidget(button_duplicate);
  hlayout->addWidget(button_import);
  connect(button_delete, SIGNAL(clicked()), this, SLOT(buttonDelete()));
  connect(button_duplicate, SIGNAL(clicked()), this, SLOT(buttonDuplicate()));
  connect(button_import, SIGNAL(clicked()), this, SLOT(buttonImport()));
  connect(button_new, SIGNAL(clicked()), this, SLOT(buttonNew()));

  /* Layout */
  QVBoxLayout* layout = new QVBoxLayout(widget_main);
  layout->addWidget(combo_top);
  layout->addLayout(hlayout);
  layout->addWidget(view_raw);
  layout->addWidget(view_sprite);
  layout->addWidget(view_thing);
  layout->addWidget(view_io);
  layout->addWidget(view_item);
  layout->addWidget(view_person);
  layout->addWidget(view_npc);
  layout->addWidget(view_music);
  layout->addWidget(view_battlescene);
  layout->addWidget(view_lays);
}

/* Set-up path editor widget */
// TODO: Comment
void MapDatabase::setupPathEdit()
{
  widget_path = new QWidget(this);

  /* Title label */
  QLabel* lbl_title = new QLabel("NPC Path Edit", widget_path);
  QFont font = lbl_title->font();
  font.setBold(true);
  font.setPointSize(9);
  lbl_title->setFont(font);

  /* Node viewer and editor */
  path_list = new QListWidget(widget_path);
  path_list->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(path_list, SIGNAL(currentRowChanged(int)),
          this, SLOT(pathChanged(int)));
  connect(path_list, SIGNAL(customContextMenuRequested(QPoint)),
          this, SLOT(pathClickRight(QPoint)));
  connect(path_list, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
          this, SLOT(pathClickDouble(QListWidgetItem*)));

  /* Right click menu control */
  path_menu = new QMenu("Node Edit", widget_path);
  path_edit_node = new QAction("Edit", path_menu);
  connect(path_edit_node, SIGNAL(triggered()), this, SLOT(pathNodeEdit()));
  path_delete_node = new QAction("Delete", widget_path);
  connect(path_delete_node, SIGNAL(triggered()), this, SLOT(pathNodeDelete()));
  path_menu->addAction(path_edit_node);
  path_menu->addAction(path_delete_node);
  path_menu->hide();

  /* Button finish */
  QPushButton* button_path = new QPushButton("Finished", widget_path);
  button_path->setMinimumWidth(150);
  connect(button_path, SIGNAL(clicked()), this, SLOT(pathFinished()));

  /* Layout */
  QVBoxLayout* layout = new QVBoxLayout(widget_path);
  layout->addWidget(lbl_title, 0, Qt::AlignHCenter);
  layout->addWidget(path_list);
  layout->addWidget(button_path, 0, Qt::AlignHCenter);
}

/* Set-up teleport selector */
// TODO: Comment
void MapDatabase::setupTeleportSelect()
{
  widget_teleport = new QWidget(this);

  /* Label for the teleport notification */
  QLabel* lbl_teleport = new QLabel(
       "Select tile for\nwhere the teleportation\nevent will send the thing.",
       widget_teleport);
  lbl_teleport->setAlignment(Qt::AlignCenter);
  QFont font = lbl_teleport->font();
  font.setPointSize(14);
  font.setBold(true);
  lbl_teleport->setFont(font);

  /* Layout */
  QVBoxLayout* layout = new QVBoxLayout(widget_teleport);
  layout->addWidget(lbl_teleport, 0, Qt::AlignCenter);
}

/* Sets the widget mode */
// TODO: Comment
void MapDatabase::setWidgetMode(EditorEnumDb::MapEditMode mode)
{
  setCurrentIndex(mode);
  widget_mode = mode;
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

// TODO: Comment
void MapDatabase::buttonDelete()
{
  /* Delete button control to visible widget */
  if(view_sprite->isVisible())
    view_sprite->deleteSprite();
  else if(view_thing->isVisible())
    view_thing->deleteThing();
  else if(view_io->isVisible())
    view_io->deleteIO();
  else if(view_item->isVisible())
    view_item->deleteItem();
  else if(view_person->isVisible())
    view_person->deletePerson();
  else if(view_npc->isVisible())
    view_npc->deleteNPC();
}

// TODO: Comment
void MapDatabase::buttonDuplicate()
{
  /* Duplicate button control to visible widget */
  if(view_sprite->isVisible())
    view_sprite->duplicateSprite();
  else if(view_thing->isVisible())
    view_thing->duplicateThing();
  else if(view_io->isVisible())
    view_io->duplicateIO();
  else if(view_item->isVisible())
    view_item->duplicateItem();
  else if(view_person->isVisible())
    view_person->duplicatePerson();
  else if(view_npc->isVisible())
    view_npc->duplicateNPC();
}

// TODO: Comment
void MapDatabase::buttonImport()
{
  /* Import button control to visible widget */
  if(view_sprite->isVisible())
    view_sprite->importSprite();
  else if(view_thing->isVisible())
    view_thing->importThing();
  else if(view_io->isVisible())
    view_io->importIO();
  else if(view_item->isVisible())
    view_item->importItem();
  else if(view_person->isVisible())
    view_person->importPerson();
  else if(view_npc->isVisible())
    view_npc->importNPC();
}

// TODO: Comment
void MapDatabase::buttonNew()
{
  /* New button control to visible widget */
  if(view_sprite->isVisible())
    view_sprite->newSprite();
  else if(view_thing->isVisible())
    view_thing->newThing();
  else if(view_io->isVisible())
    view_io->newIO();
  else if(view_item->isVisible())
    view_item->newItem();
  else if(view_person->isVisible())
    view_person->newPerson();
  else if(view_npc->isVisible())
    view_npc->newNPC();
}

/* Fills thing with data */
void MapDatabase::fillWithData(EditorEnumDb::MapObjectMode view)
{
  if(editing_map != nullptr)
  {
    /* Set who called the update */
    mode_for_data = view;

    /* Compile thing and map lists */
    QList<QString> thing_list = editing_map->getThingList(0, true, true);
    QList<QPair<QString,QString>> io_set =
                                       editing_map->getIOListSet(0, true, true);
    QList<QString> item_list = editing_map->getItemList(0, true, true);
    QList<QString> person_list = editing_map->getPersonList(0, true, true);
    QList<QString> npc_list = editing_map->getNPCList(0, true, true);

    /* List sorting and extra elements */
    qSort(thing_list);
    qSort(io_set);
    qSort(item_list);
    qSort(person_list);
    person_list.push_front("0: Player");
    qSort(npc_list);

    /* Update things */
    if(view == EditorEnumDb::THING_VIEW)
    {
      view_thing->updateListMapThings(thing_list, io_set, item_list,
                                      person_list, npc_list);
      view_thing->updateListSubmaps(editing_map->getMapList());
    }
    /* Update IOs */
    else if(view == EditorEnumDb::IO_VIEW)
    {
      view_io->updateListMapThings(thing_list, io_set, item_list,
                                   person_list, npc_list);
      view_io->updateListSubmaps(editing_map->getMapList());
    }
    /* Update persons */
    else if(view == EditorEnumDb::PERSON_VIEW)
    {
      view_person->updateListMapThings(thing_list, io_set, item_list,
                                       person_list, npc_list);
      view_person->updateListSubmaps(editing_map->getMapList());
    }
    else if(view == EditorEnumDb::NPC_VIEW)
    {
      view_npc->updateListMapThings(thing_list, io_set, item_list,
                                    person_list, npc_list);
      view_npc->updateListSubmaps(editing_map->getMapList());
    }

    /* Emit signal to get other lists (items, maps, sound, music) */
    if(view != EditorEnumDb::MUSIC_VIEW ||
       view != EditorEnumDb::MAPBATTLESCENE_VIEW)
    {
      emit updateEventObjects();
      emit updateSoundObjects();
    }
    else if(view == EditorEnumDb::MAPBATTLESCENE_VIEW)
    {
      emit updateBattleSceneObjects();
    }
    else
    {
      emit updateMusicObjects();
    }
  }
}

/* Path chosen changed */
void MapDatabase::pathChanged(int current_row)
{
  if(path_working != NULL && editing_map != NULL && current_row >= 0)
  {
    path_working->setIndexSelect(current_row-1);
    emit ensureVisible(editing_map->getCurrentMap()->
                                  tiles[path_working->getNodeX(current_row-1)]
                                       [path_working->getNodeY(current_row-1)]);
  }
}

/* Click control for path edit view */
void MapDatabase::pathClickDouble(QListWidgetItem*)
{
  if(path_working != NULL && (path_working->getState() == MapNPC::LOOPED ||
                              path_working->getState() == MapNPC::BACKANDFORTH))
    pathNodeEdit();
}

/* Click control for path edit view */
void MapDatabase::pathClickRight(const QPoint & pos)
{
  if(path_working != NULL)
  {
    QListWidgetItem* item = path_list->itemAt(pos);
    if(item != NULL)
    {
      /* Disable delete if the node is the root node, then exec */
      path_delete_node->setEnabled(path_list->row(item) > 0);
      path_edit_node->setEnabled(path_working->getState() == MapNPC::LOOPED ||
                             path_working->getState() == MapNPC::BACKANDFORTH ||
                             (path_working->getState() == MapNPC::RANDOMRANGE &&
                              path_list->row(item) == 0));
      path_menu->exec(QCursor::pos());
    }
  }
}

/* Path edit start. Connected to views below */
void MapDatabase::pathEditStart(EditorNPCPath* path)
{
  if(path_working == NULL && path != NULL)
  {
    /* Set the widget mode and update node view */
    setWidgetMode(EditorEnumDb::PATH_EDIT);
    path_working = path;
    editing_map->setHoverPathMode(true);
    updatePathNodes();
    emit pathEditTrigger(path);

    /* Signals */
    connect(path_working, SIGNAL(pathChanged()), this, SLOT(updatePathNodes()));
  }
}

/* Click control for path edit view */
void MapDatabase::pathFinished()
{
  if(path_working != NULL)
  {
    /* Restore the widget mode */
    setWidgetMode(EditorEnumDb::NORMAL_EDIT);
    view_npc->updatePathFinished();
    disconnect(path_working, SIGNAL(pathChanged()),
               this, SLOT(updatePathNodes()));

    /* Restore and disconnect map view */
    editing_map->setHoverPathMode(false);
    path_working = NULL;
    emit pathEditTrigger(NULL);
  }
}

/* Delete path node on selection in list */
void MapDatabase::pathNodeDelete()
{
  if(path_working != NULL && path_list->currentItem() != NULL)
  {
    int index = path_list->row(path_list->currentItem())-1;

    /* Only proceed if index is greater than 0 - can't delete root node */
    if(index >= 0)
    {
      /* Create warning about deleting */
      QMessageBox msg_box;
      msg_box.setWindowTitle("Node Delete");
      msg_box.setText("Deleting path node index " + QString::number(index));
      msg_box.setInformativeText("Are you sure?");
      msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
      if(msg_box.exec() == QMessageBox::Yes)
        path_working->deleteNode(index);
    }
  }
}

/* Path node edit on selection in list */
void MapDatabase::pathNodeEdit()
{
  if(path_working != NULL && (path_working->getState() == MapNPC::LOOPED ||
                            path_working->getState() == MapNPC::BACKANDFORTH ||
                            path_working->getState() == MapNPC::RANDOMRANGE))
  {
    /* Create the dialog and execute */
    int index = path_list->currentRow();
    NodeDialog* dialog = new NodeDialog(path_working, index - 1, this);
    if(dialog->exec() == QDialog::Accepted)
      path_list->setCurrentRow(index);

    delete dialog;
    dialog = NULL;
  }
}

/* Select a tile trigger - to the map render */
void MapDatabase::selectTile(EditorEnumDb::MapObjectMode view)
{
  if(editing_map != NULL)
  {
    mode_for_tile = view;
    emit selectTile();
  }
}

/* Sends the selected tile to the appropriate thing pop-up */
void MapDatabase::sendSelectedTile(int id, int x, int y)
{
  if(mode_for_tile == EditorEnumDb::THING_VIEW)
  {
    view_thing->updateSelectedTile(id, x, y);
  }
  else if(mode_for_tile == EditorEnumDb::IO_VIEW)
  {
    view_io->updateSelectedTile(id, x, y);
  }
  else if(mode_for_tile == EditorEnumDb::PERSON_VIEW)
  {
    view_person->updateSelectedTile(id, x, y);
  }
  else if(mode_for_tile == EditorEnumDb::NPC_VIEW)
  {
    view_npc->updateSelectedTile(id, x, y);
  }
}

/* Update all lists */
void MapDatabase::updateAllLists()
{
  view_thing->updateList();
  view_io->updateList();
  view_item->updateList();
  view_person->updateList();
  view_npc->updateList();
  view_music->updateData();
  view_battlescene->updateData();
  view_lays->updateData();
}

/* Updated data from higher up in the stack */
void MapDatabase::updatedBattleScenes(QList<QPair<int,QString>> scenes)
{
  /* Made it such that whenever called, it updates the scene list, since
   * it's the only one. If it ever changes where there is two, evaluate.
   * The reason for this was the requirement of scene view in map needs to be
   * updated whenever music is changed - trigger by gamedb */
  //if(mode_for_data == EditorEnumDb::MAPBATTLESCENE_VIEW)
  view_battlescene->updateListScenes(scenes);
}

/* Updated data from higher up in the stack */
void MapDatabase::updatedItems(QList<QString> items)
{
  if(mode_for_data == EditorEnumDb::THING_VIEW)
  {
    view_thing->updateListItems(items);
  }
  else if(mode_for_data == EditorEnumDb::IO_VIEW)
  {
    view_io->updateListItems(items);
  }
  else if(mode_for_data == EditorEnumDb::ITEM_VIEW)
  {
    view_item->updateListItems(items);
  }
  else if(mode_for_data == EditorEnumDb::PERSON_VIEW)
  {
    view_person->updateListItems(items);
  }
  else if(mode_for_data == EditorEnumDb::NPC_VIEW)
  {
    view_npc->updateListItems(items);
  }
}

/* Updated data from higher up in the stack */
void MapDatabase::updatedMaps(QList<QString> maps)
{
  if(mode_for_data == EditorEnumDb::THING_VIEW)
  {
    view_thing->updateListMaps(maps);
  }
  else if(mode_for_data == EditorEnumDb::IO_VIEW)
  {
    view_io->updateListMaps(maps);
  }
  else if(mode_for_data == EditorEnumDb::PERSON_VIEW)
  {
    view_person->updateListMaps(maps);
  }
  else if(mode_for_data == EditorEnumDb::NPC_VIEW)
  {
    view_npc->updateListMaps(maps);
  }
}

/* Updated data from higher up in the stack */
void MapDatabase::updatedMusic(QList<QString> music)
{
  /* Made it such that whenever called, it updates the music list, since
   * it's the only one. If it ever changes where there is two, evaluate.
   * The reason for this was the requirement of music view in map needs to be
   * updated whenever music is changed - trigger by gamedb */
  //if(mode_for_data == EditorEnumDb::MUSIC_VIEW)
  view_music->updateListMusic(music);
}

/* Updated data from higher up in the stack */
void MapDatabase::updatedParties(QList<QString> parties)
{
  if(mode_for_data == EditorEnumDb::THING_VIEW)
  {
    view_thing->updateListParties(parties);
  }
  else if(mode_for_data == EditorEnumDb::IO_VIEW)
  {
    view_io->updateListParties(parties);
  }
  else if(mode_for_data == EditorEnumDb::PERSON_VIEW)
  {
    view_person->updateListParties(parties);
  }
  else if(mode_for_data == EditorEnumDb::NPC_VIEW)
  {
    view_npc->updateListParties(parties);
  }
}

/* Sound trigger and fill slot from various views */
// TODO: Comment
void MapDatabase::updatedSounds(QList<QString> sounds)
{
  if(mode_for_data == EditorEnumDb::SPRITE_VIEW && view_sprite != nullptr)
    view_sprite->soundFill(sounds);
  else if(mode_for_data == EditorEnumDb::THING_VIEW && view_thing != nullptr)
    view_thing->updateListSounds(sounds);
  else if(mode_for_data == EditorEnumDb::IO_VIEW && view_io != nullptr)
    view_io->updateListSounds(sounds);
  else if(mode_for_data == EditorEnumDb::ITEM_VIEW && view_item != nullptr)
    view_item->updateListSounds(sounds);
  else if(mode_for_data == EditorEnumDb::PERSON_VIEW &&
          view_person != nullptr)
    view_person->updateListSounds(sounds);
  else if(mode_for_data == EditorEnumDb::NPC_VIEW && view_npc != nullptr)
    view_npc->updateListSounds(sounds);
}

/* Updates the path nodes in the list widget */
// TODO: Comment
void MapDatabase::updatePathNodes()
{
  /* Clear the existing list */
  path_list->clear();

  /* Fill the new list */
  for(int i = -1; i < path_working->getNodes().size(); i++)
    path_list->addItem(path_working->getNodeStr(i));
}

/* Updates based on selected index */
void MapDatabase::updateSelected(int index)
{
  /* General assumption is enabled buttons */
  button_delete->setEnabled(true);
  button_duplicate->setEnabled(true);
  button_import->setEnabled(true);
  button_new->setEnabled(true);

  /* Switch the index view */
  if(index == EditorEnumDb::RAW_VIEW)
  {
    view_raw->show();
    view_sprite->hide();
    view_thing->hide();
    view_io->hide();
    view_item->hide();
    view_person->hide();
    view_npc->hide();
    view_music->hide();
    view_battlescene->hide();
    view_lays->hide();

    /* Raw view has no buttons enabled */
    button_delete->setEnabled(false);
    button_duplicate->setEnabled(false);
    button_import->setEnabled(false);
    button_new->setEnabled(false);
  }
  else if(index == EditorEnumDb::SPRITE_VIEW)
  {
    view_raw->hide();
    view_sprite->show();
    view_thing->hide();
    view_io->hide();
    view_item->hide();
    view_person->hide();
    view_npc->hide();
    view_music->hide();
    view_battlescene->hide();
    view_lays->hide();
  }
  else if(index == EditorEnumDb::THING_VIEW)
  {
    view_raw->hide();
    view_sprite->hide();
    view_thing->show();
    view_io->hide();
    view_item->hide();
    view_person->hide();
    view_npc->hide();
    view_music->hide();
    view_battlescene->hide();
    view_lays->hide();
  }
  else if(index == EditorEnumDb::IO_VIEW)
  {
    view_raw->hide();
    view_sprite->hide();
    view_thing->hide();
    view_io->show();
    view_item->hide();
    view_person->hide();
    view_npc->hide();
    view_music->hide();
    view_battlescene->hide();
    view_lays->hide();
  }
  else if(index == EditorEnumDb::ITEM_VIEW)
  {
    view_raw->hide();
    view_sprite->hide();
    view_thing->hide();
    view_io->hide();
    view_item->show();
    view_person->hide();
    view_npc->hide();
    view_music->hide();
    view_battlescene->hide();
    view_lays->hide();

    /* Item view has no buttons enabled */
    button_delete->setEnabled(false);
    button_duplicate->setEnabled(false);
    button_import->setEnabled(false);
    button_new->setEnabled(false);
  }
  else if(index == EditorEnumDb::PERSON_VIEW)
  {
    view_raw->hide();
    view_sprite->hide();
    view_thing->hide();
    view_io->hide();
    view_item->hide();
    view_person->show();
    view_npc->hide();
    view_music->hide();
    view_battlescene->hide();
    view_lays->hide();
  }
  else if(index == EditorEnumDb::NPC_VIEW)
  {
    view_raw->hide();
    view_sprite->hide();
    view_thing->hide();
    view_io->hide();
    view_item->hide();
    view_person->hide();
    view_npc->show();
    view_music->hide();
    view_battlescene->hide();
    view_lays->hide();
  }
  else if(index == EditorEnumDb::MUSIC_VIEW)
  {
    view_raw->hide();
    view_sprite->hide();
    view_thing->hide();
    view_io->hide();
    view_item->hide();
    view_person->hide();
    view_npc->hide();
    view_music->show();
    view_battlescene->hide();
    view_lays->hide();

    /* Music view has no buttons enabled */
    button_delete->setEnabled(false);
    button_duplicate->setEnabled(false);
    button_import->setEnabled(false);
    button_new->setEnabled(false);

    if(view_music->getDataMusic().size() == 0)
      emit updateMusicObjects();
  }
  else if(index == EditorEnumDb::MAPBATTLESCENE_VIEW)
  {
    view_raw->hide();
    view_sprite->hide();
    view_thing->hide();
    view_io->hide();
    view_item->hide();
    view_person->hide();
    view_npc->hide();
    view_music->hide();
    view_battlescene->show();
    view_lays->hide();

    /* Music view has no buttons enabled */
    button_delete->setEnabled(false);
    button_duplicate->setEnabled(false);
    button_import->setEnabled(false);
    button_new->setEnabled(false);
  }
  else if(index == EditorEnumDb::MAPLAY_VIEW)
  {
    view_raw->hide();
    view_sprite->hide();
    view_thing->hide();
    view_io->hide();
    view_item->hide();
    view_person->hide();
    view_npc->hide();
    view_music->hide();
    view_battlescene->hide();
    view_lays->show();

    /* Music view has no buttons enabled */
    button_delete->setEnabled(false);
    button_duplicate->setEnabled(false);
    button_import->setEnabled(false);
    button_new->setEnabled(false);
  }
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Returns the views */
MapBattleSceneView* MapDatabase::getBattleSceneView()
{
  return view_battlescene;
}

/* Returns the views */
MapIOView* MapDatabase::getIOView()
{
  return view_io;
}

/* Returns the views */
MapItemView* MapDatabase::getItemView()
{
  return view_item;
}

/* Returns the lay view reference pointer */
MapLayView* MapDatabase::getLayView()
{
  return view_lays;
}

/* Returns the views */
MapNPCView* MapDatabase::getNPCView()
{
  return view_npc;
}

/* Returns the views */
MapPersonView* MapDatabase::getPersonView()
{
  return view_person;
}

/* Returns the views */
RawImageView* MapDatabase::getRawView()
{
  return view_raw;
}

/* Returns the views */
SpriteView* MapDatabase::getSpriteView()
{
  return view_sprite;
}

/* Returns the views */
MapThingView* MapDatabase::getThingView()
{
  return view_thing;
}

/* Sets the map editor */
void MapDatabase::setMapEditor(EditorMap* editing_map)
{
  this->editing_map = editing_map;

  /* Add the sprites */
  view_sprite->setEditorMap(editing_map);

  /* Add to the thing view */
  view_thing->setEditorMap(editing_map);

  /* Add to the IO view */
  view_io->setEditorMap(editing_map);

  /* Add to the item view */
  view_item->setEditorMap(editing_map);

  /* Add to the person view */
  view_person->setEditorMap(editing_map);

  /* Add to the npc view */
  view_npc->setEditorMap(editing_map);

  /* Add to the music view */
  view_music->setEditorMap(editing_map);

  /* Add to the battle scene view */
  view_battlescene->setEditorMap(editing_map);

  /* Add to the lay managing view */
  view_lays->setEditorMap(editing_map);
}
