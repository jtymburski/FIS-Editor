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
  items << "Raw Images" << "Sprites";
  view_top->addItems(items);
  view_top->setCurrentRow(0);
  view_top->setMaximumHeight(100);
  connect(view_top, SIGNAL(currentRowChanged(int)),
          this, SLOT(updateSelected(int)));

  /* Sets up the various views */
  view_raw = new RawImageView(this);
  view_sprite = new SpriteView(this);

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
  layout->addLayout(hlayout);
  updateSelected(EditorEnumDb::RAW_VIEW);
}

/*
 * Description: Destructor Function
 */
MapDatabase::~MapDatabase()
{
  delete view_raw;
  delete view_sprite;
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

// TODO: Comment
void MapDatabase::buttonDelete()
{
  qDebug() << "DELETE";
}

// TODO: Comment
void MapDatabase::buttonDuplicate()
{
  qDebug() << "DUPLICATE";
}

// TODO: Comment
void MapDatabase::buttonImport()
{
  qDebug() << "IMPORT";
}

// TODO: Comment
void MapDatabase::buttonNew()
{
  qDebug() << "NEW";
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

