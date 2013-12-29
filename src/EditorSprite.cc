/******************************************************************************
* Class Name: EditorSprite
* Date Created: Nov 24, 2013
* Inheritance: QWidget
* Description: This class is a toolbox view of each sprite that the user can
*              add to the map, these are imported by the user.
******************************************************************************/
#include "EditorSprite.h"
#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function - Requires a path
 *
 * Input: File path
 */
EditorSprite::EditorSprite(QString img_path)
{
  setGeometry(0,0,66,66);
  name = "Default";
  sprite = new Sprite();
  mode = EditorEnumDb::STANDARD;
  setPath(img_path);
  setImage(QImage(img_path));
  update();
}

/*
 * Description: Destructor Function
 */
EditorSprite::~EditorSprite()
{
  //qDebug()<<"Removing EditorSprite - "<<name;
  delete sprite;
  sprite = NULL;
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

QString EditorSprite::getName()
{
  return name;
}

void EditorSprite::setPath(QString p)
{
  frame_paths.push_back(p);
}

void EditorSprite::setImage(QImage img)
{
  frame_images.push_back(img);
}

QString EditorSprite::getPath(int pos)
{
  return frame_paths[pos];
}

QImage EditorSprite::getImage(int pos)
{
  return frame_images.last();
}

void EditorSprite::setName(QString n)
{
  name = n;
}


void EditorSprite::setBrightness(int brightness)
{
  sprite->setBrightness(brightness/100.0);
}

/* Sets the sprites animation time */
void EditorSprite::setAnimationTime(QString time)
{
  int timeint = time.toInt();
  if(timeint > 65565)
    timeint = 65565;
  else if(timeint < 0)
    timeint = 0;
  sprite->setAnimationTime(timeint);
}

/* Sets the sprites color mask */
void EditorSprite::setColorRed(int red)
{
  //Set Red
}
void EditorSprite::setColorBlue(int blue)
{
  //Set Red
}
void EditorSprite::setColorGreen(int green)
{
  //Set Red
}

/* Sets the sprites direction */
void EditorSprite::setDirectionForward(){}
void EditorSprite::setDirectionReverse(){}

void EditorSprite::setDirection(int dir)
{
  if(dir==0)
  {
    qDebug()<<"Forward";
    sprite->setDirectionForward();
  }
  else
  {
    qDebug()<<"Reverse";
    sprite->setDirectionReverse();
  }
}

/* Sets the sprites opacity */
void EditorSprite::setOpacity(int opacity)
{
  sprite->setOpacity(opacity);
}

/* Sets the sprites rotation */
void EditorSprite::setRotation(QString angle)
{
  sprite->setRotation(angle.toInt());
}

/* Sets the sprites id (Backend) */
void EditorSprite::setId(int id){}

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
 //   switch(mode)
 //   {
 //     case EditorEnumDb::STANDARD:
 //       painter.setPen(Qt::black);
 //       break;
 //     case EditorEnumDb::HOVERED:
 //       painter.setPen(Qt::cyan);
 //       break;
 //     case EditorEnumDb::SELECTED:
 //       painter.setPen(Qt::green);
 //       break;
 //     default:
 //       painter.setPen(Qt::black);
 //       break;
 //  }

    painter.drawRect(0,0,65,65);
    //QRect* bound = new QRect(1,1,64,64);
    //painter.drawImage(*bound,*pic);
}
