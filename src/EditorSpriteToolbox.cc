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
EditorSpriteToolbox::EditorSpriteToolbox(QWidget *parent) :
  SpriteToolbox(parent)
{
}

/*
 * Description: Destructor function
 *
 * Input: none
 */
EditorSpriteToolbox::~EditorSpriteToolbox()
{
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
  /* Sets up the spacing of all the sprites that will appear in the box */
//  int spacing = 68;
//  for(int i=0, j=0, k=0; i<editor_sprites.size(); i++, k++)
//  {
//    editor_sprites.at(i)->show();
//    if(i%4 == 0)
//    {
//      j+=spacing;
//      k=0;
//    }
//    editor_sprites.at(i)->move(spacing*k,j);
//  }
}

void EditorSpriteToolbox::addEditorSprite(EditorSprite *e)
{
  qDebug()<<e->getSprite()->getBrightness();
  editor_sprites.push_back(e);
  editor_sprites.at(0)->update();
}
