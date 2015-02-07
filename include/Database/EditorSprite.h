/*******************************************************************************
 * Class Name: EditorSprite
 * Date Created: November 24, 2013
 * Inheritance: QWidget
 * Description: A sprite representation that appears in the toolbox
 ******************************************************************************/
#ifndef EDITORSPRITE_H
#define EDITORSPRITE_H

#include <QObject>
#include <QPainter>

#include "Database/EditorTemplate.h"
#include "EditorEnumDb.h"
#include "EditorHelpers.h"
#include "FileHandler.h"

/* Struct for frame option storage */
struct FrameInfo
{
  QString path;
  QImage image;

  bool hflip;
  bool vflip;
  bool rotate90;
  bool rotate180;
  bool rotate270;
};

class EditorSprite : public QObject, public EditorTemplate
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorSprite(QString img_path = "");

  /* Copy constructor */
  EditorSprite(const EditorSprite &source);

  /* Destructor function */
  virtual ~EditorSprite();

private:
  /* The active frame number */
  int active_frame;

  /* The view mode for the border */
  EditorEnumDb::Border mode; // TODO: What is this for??

  /* Name */
  QString name;

  /* The actual Sprite for in game */
  Sprite* sprite;

  /* Frame information */
  QVector<FrameInfo> frame_info;

protected:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorSprite &source);

  /* Get frame mods */
  QString getFrameMods(int index);

  /* Takes a path from file and splits it to determine if there is more than 1*/
  QList<QString> splitPath(QString base_path);

  /* Returns a transformed image */
  QPixmap transformPixmap(int index, int w, int h);

public slots:
  /* Adds a frame to the top of the sequence */
  void addHead(QString path);

  /* Adds a frame to the top of the sequence */
  void addMidpoint(QString path, int after);

  /* Deletes a frame from the sequence */
  void deleteFrame(int);

  /* Gets the sprites animation time */
  QString getAnimationTime();

  /* Gets the sprites brightness */
  int getBrightness();

  /* Gets the sprites color mask */
  int getColorBlue();
  int getColorGreen();
  int getColorRed();

  /* Gets the sprites opacity */
  int getOpacity();

  /* Reset the flips for all frames */
  void resetFlips();

  /* Sets the sprites animation time */
  void setAnimationTime(QString time);

  /* Sets the sprites brightness */
  void setBrightness(int brightness);

  /* Sets the sprites color mask */
  void setColorBlue(int blue);
  void setColorGreen(int green);
  void setColorRed(int red);

  /* Sets the sprites direction */
  void setDirection(int dir);
  void setDirectionForward();
  void setDirectionReverse();

  /* Sets the frame angle of a given frame */
  void setFrameAngle(int frame_num, int angle);

  /* Sets the frame path at the given position */
  void setFramePath(int frame_num, QString newpath);

  /* Sets the frames horizontal flip of a given frame */
  void setHorizontalFlip(int,bool);
  void setHorizontalFlips(bool flip = true);

  /* Sets the user submitted name */
  virtual void setName(QString name);

  /* Sets the sprites opacity */
  void setOpacity(int opacity);

  /* Sets the sprites rotation */
  void setRotation(QString angle);

  /* Sets the frames vertical flip of a given frame */
  void setVerticalFlip(int,bool);
  void setVerticalFlips(bool flip = true);

  /* Sets all of the sprites frames to 270 deg rotation */
  void set0();

  /* Sets all of the sprites frames to 90 deg rotation */
  void set90();

  /* Sets all of the sprites frames to 180 deg rotation */
  void set180();

  /* Sets all of the sprites frames to 270 deg rotation */
  void set270();

signals:
  void spriteChanged();

public:
  /* Adds paths to the tail end of the sprite stack. Splits on | */
  int addPath(QString path);

  /* Deletes all frames */
  void deleteAllFrames();

  /* Gets the current number of frames */
  int frameCount();

  /* Returns the active frame index */
  int getActiveFrameIndex();

  /* Sets the frame angle of a given frame */
  int getFrameAngle(int frame_num);

  /* Gets the frames horizontal flip of a given frame */
  bool getHorizontalFlip(int frame_num);

  /* Gets the ID for the sprite */
  virtual int getID();

  /* Gets the image path */
  QImage getImage(int frame_num);

  /* Gets the last valid frame index in the sprite stack. -1 if none */
  int getLastValidFrame();

  /* Gets the user submitted name of the sprite */
  virtual QString getName();

  /* Gets the proper formatted name and ID for listing */
  virtual QString getNameList();

  /* Gets the sprite path */
  QString getPath(int frame_num);

  /* Returns the modified pixmap */
  QPixmap getPixmap(int index, int w, int h);

  /* Get the number of paths that are similar, to be optimized */
  int getSmartCount();

  /* Gets the sprite for alteration */
  Sprite* getSprite();

  /* Gets the frames vertical flip of a given frame */
  bool getVerticalFlip(int);

  /* Returns true if all frames in the sprite are not valid paths (null imgs) */
  bool isAllNull();

  /* Loads the game data */
  void load(XmlData data, int index);

  /* Paint the base sprite */
  bool paint(QPainter* painter, QRect rect);
  bool paint(QPainter* painter, int x, int y, int w, int h);
  bool paint(int index, QPainter* painter, QRect rect);
  bool paint(int index, QPainter* painter, int x, int y, int w, int h);

  /* Saves the game data */
  void save(FileHandler* fh, bool game_only = false);

  /* Sets the active frame index */
  void setActiveFrame(int index);
  
  /* Sets the sprites id (Backend) */
  virtual void setID(int);

  /* Trims the number of frames to this maximum */
  void setMaximumFrames(int count);

  /* Sets the sprite path */
  void setPath(int index, QString path);

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  /* The copy operator */
  EditorSprite& operator= (const EditorSprite &source);
};
#endif // EDITORSPRITE_H
