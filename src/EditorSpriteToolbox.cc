/*******************************************************************************
 * Class Name: EditorSpriteToolbox
 * Date Created: November 24, 2013
 * Inheritance: QWidget
 * Description: This class handles the toolbox on the side of the application
 ******************************************************************************/
#include "EditorSpriteToolbox.h"
#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function - Set up toolbox
 *
 * Input: parent widget
 */
EditorSpriteToolbox::EditorSpriteToolbox(QWidget *parent) : QWidget(parent)
{
  //setFixedSize(256,720);
  current = new EditorSprite();
  editor_sprite_list = new QListWidget(this);
  editor_sprite_list->show();
  editor_sprite_list->setFixedSize(288,640);
  connect(editor_sprite_list,SIGNAL(currentRowChanged(int)),
          this,SLOT(setCurrent(int)));
  nextID = 1;
}

/*
 * Description: Destructor function
 *
 * Input: none
 */
EditorSpriteToolbox::~EditorSpriteToolbox()
{
  //qDebug()<<"Removing Editor Sprite Toolbox";
  for(int i=0; i<editor_sprites.size(); i++)
  {
    delete editor_sprites[i];
    editor_sprites[i] = NULL;
  }
  //delete current;
  current = NULL;
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: The paint event for the toolbox, this mainly sets up the
 *              positions for all of the sprites that have been selected and
 *              added to the toolbar
 *
 * Inputs: Unused
 */
void EditorSpriteToolbox::paintEvent(QPaintEvent *)
{
 // QPainter painter(this);
  /* Sets up the spacing of all the sprites that will appear in the box */
  //for(int i=0; i<editor_sprites.size(); i++)
  //{
  // painter.drawText(4,32+(i*32),QString::number(editor_sprites.at(i)
  //                                               ->getSprite()->getId()));
  //  painter.drawImage(32,32+(i*32),editor_sprites.at(i)->getImage(0));
  //  painter.drawText(32,32+(i*32),editor_sprites.at(i)->getName());
  //  resize(width(),32+(i*32));
  //}

}

void EditorSpriteToolbox::addEditorSprite(EditorSprite *e)
{
  e->getSprite()->setId(nextID);
  editor_sprites.push_back(e);
  nextID++;
  editor_sprite_list->addItem(e->getName());
  update();
}

EditorSprite* EditorSpriteToolbox::getCurrent()
{
  return current;
}

void EditorSpriteToolbox::setCurrent(int index)
{
  current = editor_sprites[index];
}
