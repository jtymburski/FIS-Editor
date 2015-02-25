/*******************************************************************************
 * Class Name: MapDatabase
 * Date Created: December 23, 2014
 * Inheritance: QWidget
 * Description: Database on the left hand side that contains all of the sprite
 * creation/selection and editing components.
 ******************************************************************************/
#include "View/MapDatabase.h"

MapDatabase::MapDatabase(QWidget *parent) : QWidget(parent)
{
  editing_map = NULL;
  mode_for_data = EditorEnumDb::RAW_VIEW;
  mode_for_tile = EditorEnumDb::RAW_VIEW;

  /* Top view */
  view_top = new QListWidget(this);
  view_top->setEditTriggers(QAbstractItemView::NoEditTriggers);
  QStringList items;
  items << "Raw Images" << "Sprites" << "Things";
  view_top->addItems(items);
  view_top->setCurrentRow(0);
  view_top->setMaximumHeight(60);
  connect(view_top, SIGNAL(currentRowChanged(int)),
          this, SLOT(updateSelected(int)));

  /* Sets up the various views */
  view_raw = new RawImageView(this);
  view_sprite = new SpriteView(this);
  view_thing = new ThingView(this);

  /* Connections for the views */
  connect(view_raw->getToolbox(),SIGNAL(sendUpEditorSprite(EditorSprite*)),
          view_sprite,SLOT(addEditorSprite(EditorSprite*)));
  connect(view_thing, SIGNAL(fillWithData(EditorEnumDb::MapViewMode)),
          this, SLOT(fillWithData(EditorEnumDb::MapViewMode)));
  connect(view_thing, SIGNAL(selectTile(EditorEnumDb::MapViewMode)),
          this, SLOT(selectTile(EditorEnumDb::MapViewMode)));

  /* Push buttons at the bottom of the layout */
  button_delete = new QPushButton("Delete", this);
  button_duplicate = new QPushButton("Duplicate", this);
  button_import = new QPushButton("Import", this);
  button_new = new QPushButton("New", this);
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
  layout = new QVBoxLayout(this);
  layout->addWidget(view_top);
  layout->addWidget(view_raw);
  layout->addWidget(view_sprite);
  layout->addWidget(view_thing);
  layout->addLayout(hlayout);
  updateSelected(EditorEnumDb::RAW_VIEW);
}

/*
 * Description: Destructor Function
 */
MapDatabase::~MapDatabase()
{
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
}

// TODO: Comment
void MapDatabase::buttonDuplicate()
{
  /* Duplicate button control to visible widget */
  if(view_sprite->isVisible())
    view_sprite->duplicateSprite();
  else if(view_thing->isVisible())
    view_thing->duplicateThing();
}

// TODO: Comment
void MapDatabase::buttonImport()
{
  /* Import button control to visible widget */
  if(view_sprite->isVisible())
    view_sprite->importSprite();
  else if(view_thing->isVisible())
    view_thing->importThing();
}

// TODO: Comment
void MapDatabase::buttonNew()
{
  /* New button control to visible widget */
  if(view_sprite->isVisible())
    view_sprite->newSprite();
  else if(view_thing->isVisible())
    view_thing->newThing();
}

/* Fills thing with data */
void MapDatabase::fillWithData(EditorEnumDb::MapViewMode view)
{
  if(editing_map != NULL)
  {
    /* Set who called the update */
    mode_for_data = view;

    /* Update things */
    if(view == EditorEnumDb::THING_VIEW)
    {
      view_thing->updateListSubmaps(editing_map->getMapList());

      // TODO: REVISE - this isn't supposed to be base things but actual map
      //                things
      QVector<QString> thing_list = editing_map->getThingList();
      thing_list.push_front("0: Player");
      view_thing->updateListThings(thing_list);
    }

    /* Emit signal to get other lists (items and maps) */
    emit updateEventObjects();
  }
}

/* Select a tile trigger - to the map render */
void MapDatabase::selectTile(EditorEnumDb::MapViewMode view)
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
}

/* Updated data from higher up in the stack */
void MapDatabase::updatedItems(QVector<QString> items)
{
  if(mode_for_data == EditorEnumDb::THING_VIEW)
  {
    view_thing->updateListItems(items);
  }
}

/* Updated data from higher up in the stack */
void MapDatabase::updatedMaps(QVector<QString> maps)
{
  if(mode_for_data == EditorEnumDb::THING_VIEW)
  {
    view_thing->updateListMaps(maps);
  }
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
  }
  else if(index == EditorEnumDb::THING_VIEW)
  {
    view_raw->hide();
    view_sprite->hide();
    view_thing->show();
  }
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

RawImageView* MapDatabase::getRawView()
{
  return view_raw;
}

SpriteView* MapDatabase::getSpriteView()
{
  return view_sprite;
}

ThingView* MapDatabase::getThingView()
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
}
