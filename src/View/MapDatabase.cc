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
  view_top = new QListWidget(widget_main);
  view_top->setEditTriggers(QAbstractItemView::NoEditTriggers);
  QStringList items;
  items << "Raw Images" << "Sprites" << "Things" << "Persons" << "NPCs";
  view_top->addItems(items);
  view_top->setCurrentRow(0);
  view_top->setMaximumHeight(60);
  connect(view_top, SIGNAL(currentRowChanged(int)),
          this, SLOT(updateSelected(int)));

  /* Sets up the various views */
  view_raw = new RawImageView(widget_main);
  view_sprite = new SpriteView(widget_main);
  view_thing = new MapThingView(widget_main);
  view_person = new MapPersonView(widget_main);
  view_npc = new MapNPCView(widget_main);

  /* Connections for the views */
  connect(view_raw->getToolbox(),SIGNAL(sendUpEditorSprite(EditorSprite*)),
          view_sprite,SLOT(addEditorSprite(EditorSprite*)));
  connect(view_thing, SIGNAL(ensureVisible(QRect)),
          this, SIGNAL(ensureVisible(QRect)));
  connect(view_thing, SIGNAL(fillWithData(EditorEnumDb::MapObjectMode)),
          this, SLOT(fillWithData(EditorEnumDb::MapObjectMode)));
  connect(view_thing, SIGNAL(selectTile(EditorEnumDb::MapObjectMode)),
          this, SLOT(selectTile(EditorEnumDb::MapObjectMode)));
  connect(view_person, SIGNAL(ensureVisible(QRect)),
          this, SIGNAL(ensureVisible(QRect)));
  connect(view_person, SIGNAL(fillWithData(EditorEnumDb::MapObjectMode)),
          this, SLOT(fillWithData(EditorEnumDb::MapObjectMode)));
  connect(view_person, SIGNAL(selectTile(EditorEnumDb::MapObjectMode)),
          this, SLOT(selectTile(EditorEnumDb::MapObjectMode)));
  connect(view_npc, SIGNAL(ensureVisible(QRect)),
          this, SIGNAL(ensureVisible(QRect)));
  connect(view_npc, SIGNAL(fillWithData(EditorEnumDb::MapObjectMode)),
          this, SLOT(fillWithData(EditorEnumDb::MapObjectMode)));
  connect(view_npc, SIGNAL(selectTile(EditorEnumDb::MapObjectMode)),
          this, SLOT(selectTile(EditorEnumDb::MapObjectMode)));
  connect(view_npc, SIGNAL(pathEditStart(EditorNPCPath*)),
          this, SLOT(pathEditStart(EditorNPCPath*)));

  /* Push buttons at the bottom of the layout */
  button_delete = new QPushButton("Delete", widget_main);
  button_duplicate = new QPushButton("Duplicate", widget_main);
  button_import = new QPushButton("Import", widget_main);
  button_new = new QPushButton("New", widget_main);
  QHBoxLayout* hlayout = new QHBoxLayout();
  hlayout->addWidget(button_new);
  hlayout->addWidget(button_delete);
  hlayout->addWidget(button_import);
  hlayout->addWidget(button_duplicate);
  connect(button_delete, SIGNAL(clicked()), this, SLOT(buttonDelete()));
  connect(button_duplicate, SIGNAL(clicked()), this, SLOT(buttonDuplicate()));
  connect(button_import, SIGNAL(clicked()), this, SLOT(buttonImport()));
  connect(button_new, SIGNAL(clicked()), this, SLOT(buttonNew()));

  /* Layout */
  QVBoxLayout* layout = new QVBoxLayout(widget_main);
  layout->addWidget(view_top);
  layout->addWidget(view_raw);
  layout->addWidget(view_sprite);
  layout->addWidget(view_thing);
  layout->addWidget(view_person);
  layout->addWidget(view_npc);
  layout->addLayout(hlayout);
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
  connect(path_list, SIGNAL(currentRowChanged(int)),
          this, SLOT(pathChanged(int)));
  connect(path_list, SIGNAL(customContextMenuRequested(QPoint)),
          this, SLOT(pathClickRight(QPoint)));
  connect(path_list, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
          this, SLOT(pathClickDouble(QListWidgetItem*)));

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
  else if(view_person->isVisible())
    view_person->newPerson();
  else if(view_npc->isVisible())
    view_npc->newNPC();
}

/* Fills thing with data */
void MapDatabase::fillWithData(EditorEnumDb::MapObjectMode view)
{
  if(editing_map != NULL)
  {
    /* Set who called the update */
    mode_for_data = view;

    /* Compile list */
    QVector<QString> thing_list = editing_map->getThingList(0, true, true);
    thing_list.push_front("0: Player");
    thing_list << editing_map->getPersonList(0, true, true);
    thing_list << editing_map->getNPCList(0, true, true);

    /* Update things */
    if(view == EditorEnumDb::THING_VIEW)
    {
      view_thing->updateListSubmaps(editing_map->getMapList());
      view_thing->updateListThings(thing_list);
    }
    /* Update persons */
    else if(view == EditorEnumDb::PERSON_VIEW)
    {
      view_person->updateListSubmaps(editing_map->getMapList());
      view_person->updateListThings(thing_list);
    }
    else if(view == EditorEnumDb::NPC_VIEW)
    {
      view_npc->updateListSubmaps(editing_map->getMapList());
      view_npc->updateListThings(thing_list);
    }

    /* Emit signal to get other lists (items and maps) */
    emit updateEventObjects();
  }
}

/* Path chosen changed */
void MapDatabase::pathChanged(int current_row)
{
  if(path_working != NULL)
    path_working->setIndexSelect(current_row);
}

/* Click control for path edit view */
void MapDatabase::pathClickDouble(QListWidgetItem* item)
{
  // TODO
}

/* Click control for path edit view */
void MapDatabase::pathClickRight(const QPoint & pos)
{
  // TODO
}

/* Path edit start. Connected to views below */
void MapDatabase::pathEditStart(EditorNPCPath* path)
{
  if(path_working == NULL && path != NULL)
  {
    /* Set the widget mode and update node view */
    setWidgetMode(EditorEnumDb::PATH_EDIT);
    path_working = path;
    updatePathNodes();
    emit pathEditTrigger(path);

    /* Signals */
    connect(path_working, SIGNAL(pathChanged()), this, SLOT(updatePathNodes()));

    // TODO
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

    /* Restore and disconnect map view */
    path_working = NULL;
    emit pathEditTrigger(NULL);
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
  else if(mode_for_tile == EditorEnumDb::PERSON_VIEW)
  {
    view_person->updateSelectedTile(id, x, y);
  }
  else if(mode_for_tile == EditorEnumDb::NPC_VIEW)
  {
    view_npc->updateSelectedTile(id, x, y);
  }
}

/* Updated data from higher up in the stack */
void MapDatabase::updatedItems(QVector<QString> items)
{
  if(mode_for_data == EditorEnumDb::THING_VIEW)
  {
    view_thing->updateListItems(items);
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
void MapDatabase::updatedMaps(QVector<QString> maps)
{
  if(mode_for_data == EditorEnumDb::THING_VIEW)
  {
    view_thing->updateListMaps(maps);
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

/* Updates the path nodes in the list widget */
// TODO: Comment
void MapDatabase::updatePathNodes()
{
  /* Clear the existing list */
  path_list->clear();

  /* Fill the new list */
  for(int i = 0; i < path_working->getNodes().size(); i++)
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
    view_person->hide();
    view_npc->hide();

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
    view_person->hide();
    view_npc->hide();
  }
  else if(index == EditorEnumDb::THING_VIEW)
  {
    view_raw->hide();
    view_sprite->hide();
    view_thing->show();
    view_person->hide();
    view_npc->hide();
  }
  else if(index == EditorEnumDb::PERSON_VIEW)
  {
    view_raw->hide();
    view_sprite->hide();
    view_thing->hide();
    view_person->show();
    view_npc->hide();
  }
  else if(index == EditorEnumDb::NPC_VIEW)
  {
    view_raw->hide();
    view_sprite->hide();
    view_thing->hide();
    view_person->hide();
    view_npc->show();
  }
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

MapNPCView* MapDatabase::getNPCView()
{
  return view_npc;
}

MapPersonView* MapDatabase::getPersonView()
{
  return view_person;
}

RawImageView* MapDatabase::getRawView()
{
  return view_raw;
}

SpriteView* MapDatabase::getSpriteView()
{
  return view_sprite;
}

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

  /* Add to the person view */
  view_person->setEditorMap(editing_map);

  /* Add to the npc view */
  view_npc->setEditorMap(editing_map);
}
