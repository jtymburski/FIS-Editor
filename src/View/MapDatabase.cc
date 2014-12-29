/*******************************************************************************
 * Class Name: MapDatabase
 * Date Created: December 23, 2014
 * Inheritance: QWidget
 * Description: Database on the left hand side that contains all of the sprite
 * creation/selection and editing components.
 ******************************************************************************/
#include "View/MapDatabase.h"

MapDatabase::MapDatabase(QWidget *parent) : QTabWidget(parent)
{
  /* Sets up a scroll area with the images tab */
  raw_view = new RawImageView(this);

  /* Sets up a scroll area with the sprites tab */
  sprite_view = new SpriteView(this);

  connect(raw_view->getToolbox(),SIGNAL(sendUpEditorSprite(EditorSprite*)),
          sprite_view,SLOT(addEditorSprite(EditorSprite*)));


  addTab(raw_view,"Raw Images");
  addTab(sprite_view,"Sprites");
}
/*
 * Description: Destructor Function
 */
MapDatabase::~MapDatabase()
{
  delete raw_view;
  delete sprite_view;
}

RawImageView* MapDatabase::getRawView()
{
  return raw_view;
}

SpriteView* MapDatabase::getSpriteView()
{
  return sprite_view;
}

