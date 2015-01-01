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
  top_view = new QListWidget(this);
  top_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
  QStringList items;
  items << "Raw Images" << "Sprites";
  top_view->addItems(items);
  top_view->setCurrentRow(0);
  top_view->setMaximumHeight(100);
  connect(top_view, SIGNAL(currentRowChanged(int)),
          this, SLOT(updateSelected(int)));

  /* Sets up the various views */
  raw_view = new RawImageView(this);
  sprite_view = new SpriteView(this);

  /* Connections for the views */
  connect(raw_view->getToolbox(),SIGNAL(sendUpEditorSprite(EditorSprite*)),
          sprite_view,SLOT(addEditorSprite(EditorSprite*)));

  /* Layout */
  layout = new QVBoxLayout(this);
  layout->addWidget(top_view);
  layout->addWidget(raw_view);
  layout->addWidget(sprite_view);
  updateSelected(0);

  //QHBoxLayout* button_layout = new QHBoxLayout();
  //button_layout->addWidget(new_button);
  //button_layout->addWidget(del_button);
  //button_layout->addWidget(import_button);
  //button_layout->addWidget(duplicate_button);
  //layout->addLayout(button_layout);
  //modifyBottomList(top_view->currentRow());
}

/*
 * Description: Destructor Function
 */
MapDatabase::~MapDatabase()
{
  delete raw_view;
  delete sprite_view;
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

/* Updates based on selected index */
void MapDatabase::updateSelected(int index)
{
  if(index == 0)
  {
    raw_view->show();
    sprite_view->hide();
  }
  else if(index == 1)
  {
    raw_view->hide();
    sprite_view->show();
  }
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

RawImageView* MapDatabase::getRawView()
{
  return raw_view;
}

SpriteView* MapDatabase::getSpriteView()
{
  return sprite_view;
}

