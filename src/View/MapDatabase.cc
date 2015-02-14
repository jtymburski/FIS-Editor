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
  /* Top view */
  view_top = new QListWidget(this);
  view_top->setEditTriggers(QAbstractItemView::NoEditTriggers);
  QStringList items;
  items << "Raw Images" << "Sprites" << "Things";
  view_top->addItems(items);
  view_top->setCurrentRow(0);
  view_top->setMaximumHeight(100);
  connect(view_top, SIGNAL(currentRowChanged(int)),
          this, SLOT(updateSelected(int)));

  /* Sets up the various views */
  view_raw = new RawImageView(this);
  view_sprite = new SpriteView(this);
  view_thing = new ThingView(this);

  /* Connections for the views */
  connect(view_raw->getToolbox(),SIGNAL(sendUpEditorSprite(EditorSprite*)),
          view_sprite,SLOT(addEditorSprite(EditorSprite*)));

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
  /* Add the sprites */
  view_sprite->setEditorMap(editing_map);

  /* Add to the thing view */
  view_thing->setEditorMap(editing_map);
}
