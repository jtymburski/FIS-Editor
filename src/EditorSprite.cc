/******************************************************************************
* Class Name: EditorSprite
* Date Created: Nov 24, 2013
* Inheritance: QWidget
* Description: This class is a toolbox view of each sprite that the user can
*              add to the map, these are imported by the user.
******************************************************************************/
#include "EditorSprite.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function - Requires a path
 *
 * Input: File path
 */
EditorSprite::EditorSprite()
{
  sprite = new Sprite();
  mode = EditorEnumDb::STANDARD;
  path = "";
}

/*
 * Description: Destructor Function
 */
EditorSprite::~EditorSprite()
{
}


/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Returns the actual sprite
 *
 * Output: The Sprite for in-game
 */

Sprite* EditorSprite::getSprite()
{
  return sprite;
}

void EditorSprite::setPath(QString p)
{
  path = p;
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: The paint event for the sprite. Sets up the bounding box, then
 *              draws the current image into said box
 *
 * Inputs: Unused
 */
void EditorSprite::paintEvent(QPaintEvent *)
{
  QPainter painter(this);
  /* If the path is valid (which it should never not be), draw the result into
   * the frame with a one pixel thick black border */
    switch(mode)
    {
      case EditorEnumDb::STANDARD:
        painter.setPen(Qt::black);
        break;
      case EditorEnumDb::HOVERED:
        painter.setPen(Qt::cyan);
        break;
      case EditorEnumDb::SELECTED:
        painter.setPen(Qt::green);
        break;
      default:
        painter.setPen(Qt::black);
        break;
    }

    painter.drawRect(0,0,65,65);
    QRect* bound = new QRect(1,1,64,64);
    //painter.drawImage(*bound,*pic);
}
