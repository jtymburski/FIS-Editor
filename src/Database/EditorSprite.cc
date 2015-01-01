/******************************************************************************
* Class Name: EditorSprite
* Date Created: Nov 24, 2013
* Inheritance: QWidget
* Description: This class is a toolbox view of each sprite that the user can
*              add to the map, these are imported by the user.
******************************************************************************/
#include "Database/EditorSprite.h"
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
  mode = EditorEnumDb::STANDARD;
  name = "Default";
  sprite = new Sprite();
  setPath(0, img_path);
}

/*
 * Description: Destructor Function
 */
EditorSprite::~EditorSprite()
{
  if(sprite != NULL)
    delete sprite;
  sprite = NULL;
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* Returns a transformed image */
QPixmap EditorSprite::transformPixmap(int index, int w, int h)
{
  QTransform transform;
  qreal m11 = transform.m11();    /* Horizontal scaling */
  qreal m12 = transform.m12();    /* Vertical shearing */
  qreal m13 = transform.m13();    /* Horizontal Projection */
  qreal m21 = transform.m21();    /* Horizontal shearing */
  qreal m22 = transform.m22();    /* vertical scaling */
  qreal m23 = transform.m23();    /* Vertical Projection */
  qreal m31 = transform.m31();    /* Horizontal Position (DX) */
  qreal m32 = transform.m32();    /* Vertical Position (DY) */
  qreal m33 = transform.m33();    /* Addtional Projection Factor */

  /* Execute horizontal flip */
  if(getHorizontalFlip(index))
  {
    qreal scale = m11;

    m11 = -m11;

    /* Re-position back to origin */
    if(m31 > 0)
      m31 = 0;
    else
      m31 = (w * scale);
  }

  /* Execute vertical flip */
  if(getVerticalFlip(index))
  {
    qreal scale = m22;
    m22 = -m22;

    /* Re-position back to origin */
    if(m32 > 0)
      m32 = 0;
    else
      m32 = (h * scale);
  }

  /* Transform */
  transform.setMatrix(m11, m12, m13, m21, m22, m23, m31, m32, m33);
  transform.rotate(getFrameAngle(index) + sprite->getRotation());

  /* Modify brightness and color values */
  QImage editing_image = getImage(index);
  QColor old_color;
  int delta = getBrightness() - 255;
  int r_mod = getColorRed();
  int g_mod = getColorGreen();
  int b_mod = getColorBlue();
  int r,g,b;

  for(int i=0; i < editing_image.width(); i++)
  {
    for(int j=0; j < editing_image.height(); j++)
    {
      if(qAlpha(editing_image.pixel(i,j)) > 0)
      {
        /* Modify color first */
        old_color = QColor(editing_image.pixel(i,j));
        r = old_color.red() + r_mod - 255;
        g = old_color.green() + g_mod - 255;
        b = old_color.blue() + b_mod - 255;

        /* Then brightness value */
        r += delta;
        g += delta;
        b += delta;

        /* Bound the values */
        r = qBound(0,r,255);
        g = qBound(0,g,255);
        b = qBound(0,b,255);

        editing_image.setPixel(i,j,qRgb(r,g,b));
      }
    }
  }

  /* Return the pixmap */
  return QPixmap::fromImage(editing_image).transformed(transform);
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
//void EditorSprite::paintEvent(QPaintEvent *)
//{
//  QPainter painter(this);
//  painter.drawRect(0,0,65,65);
//}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

/*
 * Description: Sets the frame head
 *
 * Input: The path
 */
void EditorSprite::addHead(QString path)
{
  setPath(0, path);
}

/*
 * Description: Adds a frame in the given position
 *
 * Input: The path, and before and after elements
 */
void EditorSprite::addMidpoint(QString path, int after)
{
  setPath(after, path);
}

/*
 * Description : Deletes the frame from the sequence
 *
 * Input: Position
 */
void EditorSprite::deleteFrame(int pos)
{
  if(pos >= 0 && pos < frame_info.size())
    frame_info.remove(pos);
}

/*
 * Description: Returns the sprite animation time, as a string
 *
 * Inputs: none
 * Output: QString - the time as string, in milliseconds
 */
QString EditorSprite::getAnimationTime()
{
  return QString::number(sprite->getAnimationTime());
}

/*
 * Description: Returns the brightness of the sprite.
 *
 * Inputs: none
 * Output: int - the brightness as a range from 0 to 512
 */
int EditorSprite::getBrightness()
{
  return sprite->getBrightness()*255.0;
}

/*
 * Description: Returns the blue color value of the sprite, as a range from 0
 *              to 255. If 255, the blue value will be full color.
 *
 * Inputs: none
 * Output: int - the blue color value, from 0 to 255
 */
int EditorSprite::getColorBlue()
{
  return sprite->getColorBlue();
}

/*
 * Description: Returns the red color value of the sprite, as a range from 0
 *              to 255. If 255, the red value will be full color.
 *
 * Inputs: none
 * Output: int - the red color value, from 0 to 255
 */
int EditorSprite::getColorRed()
{
  return sprite->getColorRed();
}

/*
 * Description: Returns the green color value of the sprite, as a range from 0
 *              to 255. If 255, the green value will be full color.
 *
 * Inputs: none
 * Output: int - the green color value, from 0 to 255
 */
int EditorSprite::getColorGreen()
{
  return sprite->getColorGreen();
}

/*
 * Description: Returns the sprite opacity, as a value from 0 to 255.
 *
 * Inputs: none
 * Output: int - the sprite opacity value
 */
int EditorSprite::getOpacity()
{
  return sprite->getOpacity();
}

/*
 * Description: Gets the rotation value flags.
 *
 * Inputs: none
 * Output: int - returns the angle to rotate
 */
// TODO: Remove
int EditorSprite::getQuickRotation()
{
//  if(flipped90)
//    return 90;
//  else if(flipped180)
//    return 180;
//  else if(flipped270)
//    return 270;
//  else
    return 0;
}

/*
 * Description: Sets the sprites animation time
 *
 * Input: Time (As a string which is converted to an int)
 */
void EditorSprite::setAnimationTime(QString time)
{
  int timeint = time.toInt();
  if(timeint > 65535)
    timeint = 65535;
  else if(timeint < 0)
    timeint = 0;
  sprite->setAnimationTime(timeint);
  emit spriteChanged();
}

/*
 * Description: Sets the Sprites brightness
 *
 * Input: Brightness
 */
void EditorSprite::setBrightness(int brightness)
{
  sprite->setBrightness(brightness/255.0);
  emit spriteChanged();
}

/*
 * Description: Sets the sprites color mask
 *
 * Input: Color value
 */
void EditorSprite::setColorBlue(int blue)
{
  sprite->setColorBlue(blue);
  emit spriteChanged();
}

/*
 * Description: Sets the sprites color mask
 *
 * Input: Color value
 */
void EditorSprite::setColorGreen(int green)
{
  sprite->setColorGreen(green);
  emit spriteChanged();
}

/*
 * Description: Sets the sprites color mask
 *
 * Input: Color value
 */
void EditorSprite::setColorRed(int red)
{
  sprite->setColorRed(red);
  emit spriteChanged();
}

/*
 * Description: Sets the sprites direction7
 *
 * Input: Direction int (Declared as int for signal-slot reasons)
 */
void EditorSprite::setDirection(int dir)
{
  if(dir==0)
    sprite->setDirectionForward();
  else
    sprite->setDirectionReverse();
}

/*
 * Description: Sets the sprites direction forward
 */
void EditorSprite::setDirectionForward()
{
  setDirection(0);
}

/*
 * Description: Sets the sprites direction reversed
 */
void EditorSprite::setDirectionReverse()
{
  setDirection(1);
}

/*
 * Description: Sets the frame angle
 *
 * Input: Frame number and angle
 */
void EditorSprite::setFrameAngle(int frame_num, int angle)
{
  if(frame_num >= 0 && frame_num < frame_info.size())
  {
    frame_info[frame_num].rotate90 = false;
    frame_info[frame_num].rotate180 = false;
    frame_info[frame_num].rotate270 = false;

    /* Modify for the new percentage */
    if(angle == 90)
      frame_info[frame_num].rotate90 = true;
    else if(angle == 180)
      frame_info[frame_num].rotate180 = true;
    else if(angle == 270)
      frame_info[frame_num].rotate270 = true;

    emit spriteChanged();
  }
}

/*
info.path = path;
info.image = QImage(path);
*/

/*
 * Description : Sets the frame path at the given position
 *
 * Input: Frame position, path
 */
void EditorSprite::setFramePath(int frame_num, QString newpath)
{
  if(frame_num >= 0 && frame_num < frame_info.size())
  {
    frame_info[frame_num].path = newpath;
    frame_info[frame_num].image = QImage(newpath);

    emit spriteChanged();
  }
}

/*
 * Description: Sets the frame horizonal flip
 *
 * Input: Frame number and flip
 */
void EditorSprite::setHorizontalFlip(int frame_num, bool flip)
{
  if(frame_num >= 0 && frame_num < frame_info.size())
  {
    frame_info[frame_num].hflip = flip;
    emit spriteChanged();
  }
}

/*
 * Description: Sets the sprites id (backend)
 *
 * Input: ID value
 */
void EditorSprite::setId(int id)
{
  if(id >= 0 && id <= 65535)
    sprite->setId(id);
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
 * Description: Sets the sprites opacity
 *
 * Input: Opacity value
 */
void EditorSprite::setOpacity(int opacity)
{
  sprite->setOpacity(opacity);
  emit spriteChanged();
}

/*
 * Description: Sets the sprites rotation
 *
 * Input: String angle (Declared as QString or signal-slot reasons)
 */
void EditorSprite::setRotation(QString angle)
{
  sprite->setRotation(angle.toInt());
  emit spriteChanged();
}

/*
 * Description: Sets the frame vertical flip
 *
 * Input: Frame number and flip
 */
void EditorSprite::setVerticalFlip(int frame_num, bool flip)
{
  if(frame_num >= 0 && frame_num < frame_info.size())
  {
    frame_info[frame_num].vflip = flip;
    emit spriteChanged();
  }
}

/*
 * Description: Sets the sprites rotation
 */
void EditorSprite::set0()
{
  for(int i = 0; i < frameCount(); i++)
    setFrameAngle(i, 0);
}

/*
 * Description: Sets the sprites rotation
 */
void EditorSprite::set90()
{
  for(int i = 0; i < frameCount(); i++)
    setFrameAngle(i, 90);
}

/*
 * Description: Sets the sprites rotation
 */
void EditorSprite::set180()
{
  for(int i = 0; i < frameCount(); i++)
    setFrameAngle(i,180);
}

/*
 * Description: Sets the sprites rotation
 */
void EditorSprite::set270()
{
  for(int i = 0; i < frameCount(); i++)
    setFrameAngle(i,270);
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Gets the frame count
 *
 * Output: The frame count
 */
int EditorSprite::frameCount()
{
  return frame_info.size();
}

/*
 * Description: Gets the frame angle
 *
 * Output: The frames angle
 */
int EditorSprite::getFrameAngle(int frame_num)
{
  if(frame_num >= 0 && frame_num < frame_info.size())
  {
    if(frame_info[frame_num].rotate90)
      return 90;
    else if(frame_info[frame_num].rotate180)
      return 180;
    else if(frame_info[frame_num].rotate270)
      return 270;
  }
  return 0;
}

/*
 * Description: Gets the frame horizonal flip
 *
 * Output: The frame horizonal flip
 */
bool EditorSprite::getHorizontalFlip(int frame_num)
{
  if(frame_num >= 0 && frame_num < frame_info.size())
    return frame_info[frame_num].hflip;
  return false;
}

/*
 * Description: Gets the frame image
 *
 * Input: The frames position in the sequence
 *
 * Output: The frames image
 */
QImage EditorSprite::getImage(int frame_num)
{
  if(frame_num >= 0 && frame_num < frame_info.size())
    return frame_info[frame_num].image;
  return QImage();
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
 * Description: Gets the frame path
 *
 * Input: The frames position in the sequence
 *
 * Output: The frames path
 */
QString EditorSprite::getPath(int frame_num)
{
  if(frame_num >= 0 && frame_num < frame_info.size())
    return frame_info[frame_num].path;
  return "";
}

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
 * Description: Gets the frame vertical flip
 *
 * Output: The frame vertical flip
 */
bool EditorSprite::getVerticalFlip(int frame_num)
{
  if(frame_num >= 0 && frame_num < frame_info.size())
    return frame_info[frame_num].vflip;
  return false;
}

/*
 * Description: Paints the top frame on the stack at the given x, y and with
 *              the width and height.
 *
 * Inputs: QPainter* painter - the paint controller
 *         int x - x offset from top of QPainter object
 *         int y - y offset from top of QPainter object
 *         int w - width of rendering image
 *         int h - height of rendering image
 * Output: bool - did it get rendered?
 */
bool EditorSprite::paint(QPainter* painter, int x, int y, int w, int h)
{
  return paint(0, painter, x, y, w, h);

}

/*
 * Description: Paints the frame at the index on the stack at the given x, y
 *              and with the width and height.
 *
 * Inputs: QPainter* painter - the paint controller
 *         int x - x offset from top of QPainter object
 *         int y - y offset from top of QPainter object
 *         int w - width of rendering image
 *         int h - height of rendering image
 * Output: bool - did it get rendered?
 */
bool EditorSprite::paint(int index, QPainter* painter, int x, int y,
                         int w, int h)
{
  if(painter != NULL && index >= 0 && index < frame_info.size())
  {
    QRect bound(x, y, w, h);
    qreal old_opacity = painter->opacity();

    /* Paint pixmap */
    painter->setOpacity(getOpacity() / 100.0);
    painter->drawPixmap(bound, transformPixmap(index, w, h));

    /* Restore values */
    painter->setOpacity(old_opacity);

    return true;
  }
  return false;
}

/*
 * Description: Sets the frame path
 *
 * Input: The frames path
 */
void EditorSprite::setPath(int index, QString path)
{
  FrameInfo info;
  info.path = path;
  info.image = QImage(path);
  info.hflip = false;
  info.vflip = false;
  info.rotate90 = false;
  info.rotate180 = false;
  info.rotate270 = false;

  /* Place in the stack */
  if(index <= 0)
    frame_info.push_front(info);
  else if(index >= frame_info.size())
    frame_info.push_back(info);
  else
    frame_info.insert(index, info);
}
