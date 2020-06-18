/******************************************************************************
* Class Name: EditorSprite
* Date Created: Nov 24, 2013
* Inheritance: QWidget
* Description: This class is a toolbox view of each sprite that the user can
*              add to the map, these are imported by the user.
******************************************************************************/
#include "Database/EditorSprite.h"
#include <QDebug>

/* Constant Implementation - see header file for descriptions */
const float EditorSprite::kREF_RGB = 255.0;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function - Requires a path
 *
 * Inputs: File path
 */
EditorSprite::EditorSprite(QString img_path) : QObject()
{
  active_frame = 0;
  mode = EditorEnumDb::STANDARD;
  name = "Default";
  if(img_path != "")
    setPath(0, img_path);
}

/*
 * Description: Copy constructor.
 *
 * Inputs: none
 */
EditorSprite::EditorSprite(const EditorSprite &source) : EditorSprite()
{
  copySelf(source);
}

/*
 * Description: Destructor Function
 */
EditorSprite::~EditorSprite()
{
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: Copies all data from source editor sprite to this editor
 *              sprite.
 *
 * Inputs: EditorSprite &source - the source to copy from
 *         bool only_base - only the base data. i.e. no frame data
 * Output: none
 */
void EditorSprite::copySelf(const EditorSprite &source, bool only_base)
{
  /* Copy sprite specific data */
  if(!only_base)
  {
    mode = source.mode;
    name = source.name;
    frame_info = source.frame_info;
  }

  /* Copy base data */
  sprite = source.sprite;
}

/*
 * Description: Returns the frame mods in string form - for file handling.
 *
 * Inputs: int index - the index of the frame data
 * Output: QString - the output string
 */
QString EditorSprite::getFrameMods(int index)
{
  QString temp = "path";

  if(index >= 0 && index < frame_info.size())
  {
    if(frame_info[index].hflip)
      temp += "_hf";
    if(frame_info[index].vflip)
      temp += "_vf";
    if(frame_info[index].rotate90)
      temp += "_90";
    if(frame_info[index].rotate180)
      temp += "_180";
    if(frame_info[index].rotate270)
      temp += "_270";
  }

  return temp;
}

/*
 * Description: Returns the transformed pixmap, with all necessary sprite mods.
 *
 * Inputs: int index - the frame index
 *         int w - the width of the pixmap
 *         int h - the height of the pixmap
 *         bool shadow - render as shadow. false by default
 *         QColor shadow_color - the color to render the shadow if true
 * Output: QPixmap - the transformed pixmap
 */
QPixmap EditorSprite::transformPixmap(int index, int w, int h, bool shadow,
                                      QColor shadow_color)
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
  transform.rotate(getFrameAngle(index) + sprite.getRotationDegrees());

  /* Modify brightness and color values */
  QImage editing_image = getImage(index);
  QColor old_color;
  int r_mod = getColorRed();
  int g_mod = getColorGreen();
  int b_mod = getColorBlue();
  int r,g,b;

  for(int i = 0; i < editing_image.width(); i++)
  {
    for(int j = 0; j < editing_image.height(); j++)
    {
      if(qAlpha(editing_image.pixel(i, j)) > 0)
      {
        if(shadow)
        {
          r = shadow_color.red();
          g = shadow_color.green();
          b = shadow_color.blue();
        }
        else
        {
          old_color = QColor(editing_image.pixel(i, j));
          r = old_color.red();
          g = old_color.green();
          b = old_color.blue();

          /* Brightness value */
          float delta_mod = (getBrightness() / kREF_RGB);
          if(delta_mod < 1.0)
          {
            r *= delta_mod;
            g *= delta_mod;
            b *= delta_mod;
          }
          else if(delta_mod > 1.0)
          {
            int bright_value = getBrightness() - kREF_RGB;
            r += bright_value;
            g += bright_value;
            b += bright_value;

            /* Bound the values */
            r = qBound(0, r, (int)kREF_RGB);
            g = qBound(0, g, (int)kREF_RGB);
            b = qBound(0, b, (int)kREF_RGB);
          }

          /* Then, modify the color */
          r *= (r_mod / kREF_RGB);
          g *= (g_mod / kREF_RGB);
          b *= (b_mod / kREF_RGB);
        }

        editing_image.setPixel(i, j, qRgba(r, g, b,
                                           qAlpha(editing_image.pixel(i, j))));
      }
    }
  }

  /* Return the pixmap */
  return QPixmap::fromImage(editing_image).transformed(transform);
}

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
  return QString::number(sprite.getAnimationTimeMs());
}

/*
 * Description: Returns the brightness of the sprite.
 *
 * Inputs: none
 * Output: int - the brightness as a range from 0 to 512
 */
int EditorSprite::getBrightness()
{
  return sprite.getBrightness()*kREF_RGB;
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
  return sprite.getColorBlue();
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
  return sprite.getColorRed();
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
  return sprite.getColorGreen();
}

/*
 * Description: Returns the sprite opacity, as a value from 0 to 255.
 *
 * Inputs: none
 * Output: int - the sprite opacity value
 */
int EditorSprite::getOpacity()
{
  return sprite.getOpacity();
}

/*
 * Description: Resets the flip on all frames in the sprite
 *
 * Inputs: none
 * Output: none
 */
void EditorSprite::resetFlips()
{
  for(int i = 0; i < frame_info.size(); i++)
  {
    frame_info[i].hflip = false;
    frame_info[i].vflip = false;
  }
  emit spriteChanged();
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
  sprite.setAnimationTimeMs(timeint);
  emit spriteChanged();
}

/*
 * Description: Sets the Sprites brightness
 *
 * Input: Brightness
 */
void EditorSprite::setBrightness(int brightness)
{
  sprite.setBrightness(brightness / kREF_RGB);
  emit spriteChanged();
}

/*
 * Description: Sets the sprites color mask
 *
 * Input: Color value
 */
void EditorSprite::setColorBlue(int blue)
{
  sprite.setColorBlue(blue);
  emit spriteChanged();
}

/*
 * Description: Sets the sprites color mask
 *
 * Input: Color value
 */
void EditorSprite::setColorGreen(int green)
{
  sprite.setColorGreen(green);
  emit spriteChanged();
}

/*
 * Description: Sets the sprites color mask
 *
 * Input: Color value
 */
void EditorSprite::setColorRed(int red)
{
  sprite.setColorRed(red);
  emit spriteChanged();
}

/*
 * Description: Sets the sprites direction7
 *
 * Input: Direction int (Declared as int for signal-slot reasons)
 */
void EditorSprite::setDirection(int dir)
{
  if (dir == 0)
    setDirectionForward();
  else
    setDirectionReverse();
}

/*
 * Description: Sets the sprites direction forward
 */
void EditorSprite::setDirectionForward()
{
  sprite.setAnimationSequence(Sequence::FORWARD);
}

/*
 * Description: Sets the sprites direction reversed
 */
void EditorSprite::setDirectionReverse()
{
  sprite.setAnimationSequence(Sequence::REVERSE);
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
 * Description : Sets the frame path at the given position
 *
 * Input: Frame position, path
 */
void EditorSprite::setFramePath(int frame_num, QString newpath)
{
  if(frame_num >= 0 && frame_num < frame_info.size())
  {
    frame_info[frame_num].path = QDir::toNativeSeparators(newpath);
    frame_info[frame_num].image = QImage(newpath);

    emit spriteChanged();
  }
}

/*
 * Description: Sets the frame horizonal flip of a single frame index
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
 * Description: Sets the frame horizontal flips for all frames
 *
 * Input: bool flip - true if wanting to flip
 */
void EditorSprite::setHorizontalFlips(bool flip)
{
  for(int i = 0; i < frame_info.size(); i++)
    frame_info[i].hflip = flip;
  emit spriteChanged();
}

/*
 * Description: Sets the Editor Sprites name
 *
 * Input: QString - name of sprite
 *        bool - should the widget be updated? default true. Not used here
 */
void EditorSprite::setName(QString n, bool update)
{
  (void)update;

  name = n;
}

/*
 * Description: Sets the sprites opacity
 *
 * Input: Opacity value
 */
void EditorSprite::setOpacity(int opacity)
{
  sprite.setOpacity(opacity);
  emit spriteChanged();
}

/*
 * Description: Sets the sprites rotation
 *
 * Inputs: String angle (Declared as QString or signal-slot reasons)
 * Output: none
 */
void EditorSprite::setRotation(QString angle)
{
  sprite.setRotationDegrees(angle.toInt());
  emit spriteChanged();
}

/*
 * Description: Sets the frame vertical flip for a single index
 *
 * Inputs: Frame number and flip
 * Output: none
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
 * Description: Sets the frame vertical flip for all frames
 *
 * Input: bool flip - true if wanting to vertically flip the sprite
 * Output: none
 */
void EditorSprite::setVerticalFlips(bool flip)
{
  for(int i = 0; i < frame_info.size(); i++)
    frame_info[i].vflip = flip;
  emit spriteChanged();
}

/*
 * Description: Sets the sprites rotation
 *
 * Inputs: none
 * Output: none
 */
void EditorSprite::set0()
{
  for(int i = 0; i < frameCount(); i++)
    setFrameAngle(i, 0);
}

/*
 * Description: Sets the sprites rotation
 *
 * Inputs: none
 * Output: none
 */
void EditorSprite::set90()
{
  for(int i = 0; i < frameCount(); i++)
    setFrameAngle(i, 90);
}

/*
 * Description: Sets the sprites rotation
 *
 * Inputs: none
 * Output: none
 */
void EditorSprite::set180()
{
  for(int i = 0; i < frameCount(); i++)
    setFrameAngle(i,180);
}

/*
 * Description: Sets the sprites rotation
 *
 * Inputs: none
 * Output: none
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
 * Description: Adds path to the tail end of the sprite stack. This also takes
 *              the path and attempts to split it by the | delimiter. This will
 *              indicate the number of frames it will be adding.
 *
 * Inputs: QString path - the path to try and split and add in
 *         bool hflip - horizontally flip all sprites added
 *         bool vflip - vertically flip all sprites added
 * Output: int - the number of frames added to the end
 */
int EditorSprite::addPath(QString path, bool hflip, bool vflip)
{
  int count = 0;

  /* Only proceed if path is not empty */
  if(!path.isEmpty())
  {
    QList<QString> path_set = EditorHelpers::splitPath(path);
    count = path_set.size();

    /* Loop through all paths */
    for(int i = 0; i < path_set.size(); i++)
    {
      setPath(frameCount(), path_set[i]);
      frame_info.last().hflip = hflip;
      frame_info.last().vflip = vflip;
    }
  }

  return count;
}

/*
 * Description: Copies only the base data for sprite and nothing related to
 *              the frames, ID, or rendering.
 *
 * Inputs: const EditorSprite &source - the copy object
 * Output: none
 */
void EditorSprite::copyBaseSprite(const EditorSprite &source)
{
  copySelf(source, true);
}

/*
 * Description: Deletes all frames in the sprite
 *
 * Inputs: none
 * Output: none
 */
void EditorSprite::deleteAllFrames()
{
  while(frameCount() > 0)
    frame_info.removeLast();
}

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
 * Description: Returns the active frame reference index (what will be painted).
 *
 * Inputs: none
 * Output: int - the frame index
 */
int EditorSprite::getActiveFrameIndex()
{
  return active_frame;
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
 * Description: Returns the sprite ID
 *
 * Input: none
 * Output: int - the id of the sprite
 */
int EditorSprite::getID() const
{
  return sprite.getId();
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
 * Description: Returns the last frame that has a valid frame index from the
 *              end of the frame stack. -1 if none are valid.
 *
 * Inputs: none
 * Output: int - last valid frame index. -1 if none
 */
int EditorSprite::getLastValidFrame()
{
  int last_valid = -1;

  for(int i = 0; i < frame_info.size(); i++)
    if(!frame_info[i].image.isNull())
      last_valid = i;

  return last_valid;
}

/*
 * Description: Returns the sprite name
 *
 * Output: The sprites name
 */
QString EditorSprite::getName() const
{
  return name;
}

/*
 * Description: Returns the formatted name and ID for listing. It's in the
 *              format: "XXX: sssssssssssss"
 *
 * Inputs: none
 * Output: QString - the name for a list
 */
QString EditorSprite::getNameList()
{
  return EditorHelpers::getListString(getID(), getName(), -1, true);
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
 * Description: Returns the set of paths as a simplified representation of the
 *              frames within the sprite. The stack includes a string pair
 *              where the first is the frame delimiter (what simplifications it
 *              has) and the second is the actual path.
 *
 * Inputs: none
 * Output: QList<QPair<QString,QString>> - the returned path list
 */
QList<QPair<QString,QString>> EditorSprite::getPathSet()
{
  QList<QPair<QString,QString>> frame_stack;

  /* Try to execute a smart parse first */
  int index = getSmartCount();

  if(index > 0)
  {
    QString png = ".png";
    QString sep = "|";
    QString base_path = EditorHelpers::trimPath(frame_info.front().path);
    base_path = base_path.left(base_path.size() - png.size() - 2);
    base_path += sep + QString::number(index) + sep + png;
    frame_stack.push_back(QPair<QString,QString>(getFrameMods(0), base_path));
  }

  /* Finally, proceed to add all remaining tiles */
  for(int i = index; i < frame_info.size(); i++)
  {
    frame_stack.push_back(QPair<QString,QString>(getFrameMods(i),
                                EditorHelpers::trimPath(frame_info[i].path)));
  }

  return frame_stack;
}

/*
 * Description: Returns the pixmap of the given frame index scaled with the
 *              sprite settings.
 *
 * Inputs: int index - the frame index
 *         int w - the width of the pixmap
 *         int h - the height of the pixmap
 * Output: QPixmap - a paintable QPixmap of the above frame
 */
QPixmap EditorSprite::getPixmap(int index, int w, int h)
{
  if(index >= 0 && index < frame_info.size())
  {
    QPixmap original = transformPixmap(index, w, h);
    return original.scaled(w, h, Qt::KeepAspectRatio);
  }
  return QPixmap();
}

/*
 * Description: Returns the number of frames at the start of the sprite that
 *              can be lumped together.
 *
 * Inputs: none
 * Output: int - a count of number of frames to lump together. 0 if none
 */
int EditorSprite::getSmartCount()
{
  bool finished = false;
  int png_width = 4;

  /* Only proceed checks if there is more than one frame */
  if(frame_info.size() > 1)
  {
    int size = frame_info.front().path.size();
    QString no_end = frame_info.front().path.left(size - png_width);
    QString main = no_end.left(no_end.size() - 2);

    /* Only proceed if the end two numbers are digits */
    if(no_end.at(no_end.size() - 1).isDigit() &&
       no_end.at(no_end.size() - 2).isDigit())
    {
      int index = 1;
      int ref_index = no_end.right(2).toInt();

      for(int i = 1; !finished && (i < frame_info.size()); i++)
      {
        QString orig = frame_info[i].path;
        QString sub = orig.left(orig.size() - png_width);
        QString sub_t = sub.left(sub.size() - 2);

        /* Processing to check if it's valid */
        if(sub_t == main &&
           sub.at(sub.size() - 1).isDigit() &&
           sub.at(sub.size() - 2).isDigit() &&
           frame_info[i].hflip == frame_info.front().hflip &&
           frame_info[i].vflip == frame_info.front().vflip &&
           frame_info[i].rotate90 == frame_info.front().rotate90 &&
           frame_info[i].rotate180 == frame_info.front().rotate180 &&
           frame_info[i].rotate270 == frame_info.front().rotate270 &&
           sub.right(2).toInt() == (ref_index + index))
        {
          index++;
        }
        else
        {
          finished = true;
        }
      }

      if(index > 1)
        return index;
    }
  }

  return 0;
}

/*
 * Description: Returns the reference sound ID. If less than 0, not valid.
 *
 * Inputs: none
 * Output: int - the reference sound ID
 */
int EditorSprite::getSoundID()
{
  return sprite.getSoundId();
}

/*
 * Description: Returns the actual sprite
 *
 * Output: The Sprite for in-game
 */
SpriteCore& EditorSprite::getSprite()
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
 * Description: Returns true if all frames in the sprite are null images (not
 *              valid paths).
 *
 * Inputs: none
 * Output: bool - true if all are null and invalid. Otherwise false
 */
bool EditorSprite::isAllNull()
{
  bool all_null = true;

  for(int i = 0; all_null && (i < frame_info.size()); i++)
    if(!frame_info[i].image.isNull())
      all_null = false;

  return all_null;
}

/*
 * Description: Loads the game data from file.
 *
 * Inputs: XmlData data - the data struct from the XML
 *         int index - the offset index into the XML stack.
 * Output: none
 */
void EditorSprite::load(XmlData data, int index)
{
  QString element = QString::fromStdString(data.getElement(index));
  QStringList elements = element.split("_");

  if(element == "name")
  {
    setName(QString::fromStdString(data.getDataString()));
  }
  else if(element == "animation")
  {
    setAnimationTime(QString::number(data.getDataInteger()));
  }
  else if(element == "rotation")
  {
    setRotation(QString::number(data.getDataFloat()));
  }
  else if(element == "brightness")
  {
    setBrightness(static_cast<int>(data.getDataFloat() * kREF_RGB));
  }
  else if(element == "color_r")
  {
    setColorRed(data.getDataInteger());
  }
  else if(element == "color_g")
  {
    setColorGreen(data.getDataInteger());
  }
  else if(element == "color_b")
  {
    setColorBlue(data.getDataInteger());
  }
  else if(element == "opacity")
  {
    setOpacity(data.getDataInteger());
  }
  else if(element == "forward")
  {
    if(data.getDataBool())
      setDirectionForward();
    else
      setDirectionReverse();
  }
  else if(elements.front() == "path")
  {
    /* Add the path(s) */
    int start_point = frameCount();
    addPath(EditorHelpers::getProjectDir() + QDir::separator() +
            QString::fromStdString(data.getDataString()));
    int end_point = frameCount();

    /* Add adjustments */
    for(int i = 0; i < elements.size(); i++)
    {
      for(int j = start_point; j < end_point; j++)
      {
        if(elements[i] == "hf")
          frame_info[j].hflip = true;
        else if(elements[i] == "vf")
          frame_info[j].vflip = true;
        else if(elements[i] == "90")
          frame_info[j].rotate90 = true;
        else if(elements[i] == "180")
          frame_info[j].rotate180 = true;
        else if(elements[i] == "270")
          frame_info[j].rotate270 = true;
      }
    }
  }
  else if(element == "sound_id")
  {
    setSoundID(data.getDataInteger());
  }
}

/*
 * Description: Paints the active frame on the stack at the given QRect bound.
 *
 * Inputs: QPainter* painter - the paint controller
 *         QRect rect - the bounding rectangle
 *         bool shadow - true to render as shadow instead. Default false
 *         QColor shadow_color - the color to render the shadow if true
 * Output: bool - did it get rendered?
 */
bool EditorSprite::paint(QPainter* painter, QRect rect, bool shadow,
                         QColor shadow_color)
{
  return paint(painter, rect.x(), rect.y(), rect.width(), rect.height(),
               shadow, shadow_color);
}

/*
 * Description: Paints the active frame on the stack at the given x, y and with
 *              the width and height.
 *
 * Inputs: QPainter* painter - the paint controller
 *         int x - x offset from top of QPainter object
 *         int y - y offset from top of QPainter object
 *         int w - width of rendering image
 *         int h - height of rendering image
 *         bool shadow - true to render as shadow instead. Default false
 *         QColor shadow_color - the color to render the shadow if true
 * Output: bool - did it get rendered?
 */
bool EditorSprite::paint(QPainter* painter, int x, int y, int w, int h,
                         bool shadow, QColor shadow_color)
{
  return paint(active_frame, painter, x, y, w, h, shadow, shadow_color);

}

/*
 * Description: Paints the top frame at the index on the stack at the given
 *              QRect bound.
 *
 * Inputs: int index - the index in the frame stack
 *         QPainter* painter - the paint controller
 *         QRect rect - the bounding rectangle
 *         bool shadow - true to render as shadow instead. Default false
 *         QColor shadow_color - the color to render the shadow if true
 * Output: bool - did it get rendered?
 */
bool EditorSprite::paint(int index, QPainter* painter, QRect rect,
                         bool shadow, QColor shadow_color)
{
  return paint(index, painter, rect.x(), rect.y(), rect.width(), rect.height(),
               shadow, shadow_color);
}

/*
 * Description: Paints the frame at the index on the stack at the given x, y
 *              and with the width and height.
 *
 * Inputs: int index - the index in the frame stack
 *         QPainter* painter - the paint controller
 *         int x - x offset from top of QPainter object
 *         int y - y offset from top of QPainter object
 *         int w - width of rendering image
 *         int h - height of rendering image
 *         bool shadow - true to render as shadow instead. Default false
 *         QColor shadow_color - the color to render the shadow if true
 * Output: bool - did it get rendered?
 */
bool EditorSprite::paint(int index, QPainter* painter, int x, int y,
                         int w, int h, bool shadow, QColor shadow_color)
{
  if(painter != NULL && index >= 0 && index < frame_info.size())
  {
    QRect bound(x, y, w, h);
    qreal old_opacity = painter->opacity();

    /* Paint pixmap */
    painter->setOpacity(getOpacity() / kREF_RGB);
    if(shadow)
      painter->setOpacity(shadow_color.alphaF());
    painter->drawPixmap(bound, transformPixmap(index, w, h, shadow,
                                               shadow_color));

    /* Restore values */
    painter->setOpacity(old_opacity);

    return true;
  }
  return false;
}

/*
 * Description: Saves the data of this sprite to the file handler pointer.
 *              Game only toggle removes editor only data.
 *
 * Inputs: FileHandler* fh - the saving file handler
 *         bool game_only - true if you only want game relevant data, not editor
 *         bool core_only - true if only core data. No frame data, names, or
 *                          wrapper
 * Output: none
 */
void EditorSprite::save(FileHandler* fh, bool game_only, bool core_only,
                        QString element)
{
  QString base_path = EditorHelpers::getSpriteDir();
  base_path = base_path.left(base_path.indexOf("/sprites"));
  EditorSprite ref;
  bool use_name = element.isEmpty();

  if(fh != NULL)
  {
    /* Check if data is default */
    bool animate = ref.getAnimationTime() != getAnimationTime();
    bool rotate = ref.getSprite().getRotationDegrees() != sprite.getRotationDegrees();
    bool bright = ref.getSprite().getBrightness() != sprite.getBrightness();
    bool color_r = ref.getSprite().getColorRed() != sprite.getColorRed();
    bool color_g = ref.getSprite().getColorGreen() != sprite.getColorGreen();
    bool color_b = ref.getSprite().getColorBlue() != sprite.getColorBlue();
    bool opac = ref.getSprite().getOpacity() != sprite.getOpacity();
    bool dir = ref.getSprite().getAnimationSequence() != sprite.getAnimationSequence();
    bool sound_id = (getSoundID() >= 0);

    if(!core_only)
    {
      if(use_name)
        fh->writeXmlElement("sprite", "id", getID());
      else
        fh->writeXmlElement(element.toStdString());
    }
    else if(animate || rotate || bright || color_r ||
            color_g || color_b || opac || dir || sound_id)
    {
      fh->writeXmlElement("sprite");
    }

    /* Write sprite data */
    if(!game_only && !core_only && use_name)
      fh->writeXmlData("name", getName().toStdString());
    if(animate)
      fh->writeXmlData("animation", getAnimationTime().toInt());
    if(rotate)
      fh->writeXmlData("rotation", sprite.getRotationDegrees());
    if(bright)
      fh->writeXmlData("brightness", sprite.getBrightness());
    if(color_r)
      fh->writeXmlData("color_r", sprite.getColorRed());
    if(color_g)
      fh->writeXmlData("color_g", sprite.getColorGreen());
    if(color_b)
      fh->writeXmlData("color_b", sprite.getColorBlue());
    if(opac)
      fh->writeXmlData("opacity", sprite.getOpacity());
    if(dir)
      fh->writeXmlData("forward", sprite.getAnimationSequence() == Sequence::FORWARD);
    if(sound_id)
      fh->writeXmlData("sound_id", getSoundID());

    /* Write frame data */
    if(!core_only)
    {
      QList<QPair<QString,QString>> frame_set = getPathSet();
      for(int i = 0; i < frame_set.size(); i++)
        fh->writeXmlData(frame_set[i].first.toStdString(),
                         frame_set[i].second.toStdString());
    }

    /* Close element, if relevant */
    if(!core_only || animate || rotate || bright || color_r ||
            color_g || color_b || opac || dir)
    {
      fh->writeXmlElementEnd();
    }
  }
}

/*
 * Description: Sets the active frame index. If out of range, no sprite will be
 *              rendered but this call will still set it.
 *
 * Inputs: int index - the frame index num
 * Output: none
 */
void EditorSprite::setActiveFrame(int index)
{
  if(index >= 0)
    active_frame = index;
}

/*
 * Description: Sets the sprites id (backend)
 *
 * Input: ID value
 */
void EditorSprite::setID(int id)
{
  if(id >= 0 && id <= 65535)
    sprite.setId(id);
}

/*
 * Description: Sets the maximum number of frames to trim the sprite down to.
 *
 * Inputs: int count - the number of frames to keep
 * Output: none
 */
void EditorSprite::setMaximumFrames(int count)
{
  if(count >= 0)
  {
    while(frameCount() > count)
      frame_info.removeLast();
  }
}

/*
 * Description: Sets the frame path
 *
 * Input: int index - the frame location to insert
 *        QString path - the path to insert
 * Output: none
 */
void EditorSprite::setPath(int index, QString path)
{
  FrameInfo info;
  info.path = QDir::toNativeSeparators(path);
  if(path.isEmpty())
    info.image = QImage();
  else
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

/*
 * Description: Sets the reference sound ID. If less than 0, unsets it.
 *
 * Inputs: int id - the new reference ID
 * Output: none
 */
void EditorSprite::setSoundID(int id)
{
  sprite.setSoundId(id);
}

/*=============================================================================
 * OPERATOR FUNCTIONS
 *============================================================================*/

/*
 * Description: Copy operator construction. This is called when the variable
 *              already exists and equal operator used with another
 *              EditorSprite.
 *
 * Inputs: const EditorSprite &source - the source class constructor
 * Output: EditorSprite& - pointer to the copied class
 */
EditorSprite& EditorSprite::operator= (const EditorSprite &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
