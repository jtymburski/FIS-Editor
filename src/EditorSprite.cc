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

/*
 * Description: Returns the sprite name
 *
 * Output: The sprites name
 */
QString EditorSprite::getName()
{
  return name;
}

/*
 * Description: Sets the frame path
 *
 * Input: The frames path
 */
void EditorSprite::setPath(QString p)
{
  frame_paths.push_back(p);
}

/*
 * Description: Sets the frame image
 *
 * Input: The frame image
 */
void EditorSprite::setImage(QImage img)
{
  frame_images.push_back(img);
}

/*
 * Description: Gets the frame path
 *
 * Input: The frames position in the sequence
 *
 * Output: The frames path
 */
QString EditorSprite::getPath(int pos)
{
  return frame_paths[pos];
}

/*
 * Description: Gets the frame image
 *
 * Input: The frames position in the sequence
 *
 * Output: The frames image
 */
QImage EditorSprite::getImage(int pos)
{
  // TODO: Set this properly after the frame framework is in place
  return frame_images.last();
}

/*
 * Description: Gets the frame count
 *
 * Output: The frame count
 */
int EditorSprite::frameCount()
{
  // TODO: Set this properly after the frame framework is in place
  return frame_images.size();
}

/*
 * Description: Sets the Editor Sprites name
 *
 * Input: Name
 */
void EditorSprite::setName(QString n)
{
  name = n;
}

/*
 * Description: Sets the Sprites brightness
 *
 * Input: Brightness
 */
void EditorSprite::setBrightness(int brightness)
{
  sprite->setBrightness(brightness/100.0);
}

/*
 * Description: Sets the sprites animation time
 *
 * Input: Time (As a string which is converted to an int)
 */
void EditorSprite::setAnimationTime(QString time)
{
  int timeint = time.toInt();
  if(timeint > 65565)
    timeint = 65565;
  else if(timeint < 0)
    timeint = 0;
  sprite->setAnimationTime(timeint);
}

/*
 * Description: Sets the sprites color mask
 *
 * Input: Color value
 */
void EditorSprite::setColorRed(int red)
{
  //Set Red
}

/*
 * Description: Sets the sprites color mask
 *
 * Input: Color value
 */
void EditorSprite::setColorBlue(int blue)
{
  //Set Red
}

/*
 * Description: Sets the sprites color mask
 *
 * Input: Color value
 */
void EditorSprite::setColorGreen(int green)
{
  //Set Red
}

/*
 * Description: Sets the sprites direction forward
 */
void EditorSprite::setDirectionForward(){}

/*
 * Description: Sets the sprites direction reversed
 */
void EditorSprite::setDirectionReverse(){}


/*
 * Description: Sets the sprites direction7
 *
 * Input: Direction int (Declared as int for signal-slot reasons)
 */
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

/*
 * Description: Sets the sprites opacity
 *
 * Input: Opacity value
 */
void EditorSprite::setOpacity(int opacity)
{
  sprite->setOpacity(opacity);
}

/*
 * Description: Sets the sprites rotation
 *
 * Input: String angle (Declared as QString or signal-slot reasons)
 */
void EditorSprite::setRotation(QString angle)
{
  sprite->setRotation(angle.toInt());
}


/*
 * Description: Sets the sprites id (backend)
 *
 * Input: ID value
 */
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
  //QPainter painter(this);
  //painter.drawRect(0,0,65,65);
}
