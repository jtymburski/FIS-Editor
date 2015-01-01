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
#include "EditorEnumDb.h"

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

class EditorSprite : public QObject
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorSprite(QString img_path = " ");

  /* Destructor function */
  ~EditorSprite();

private:
  /* The view mode for the border */
  EditorEnumDb::Border mode; // TODO: What is this for??

  /* Name */
  QString name;

  /* The actual Sprite for in game */
  Sprite* sprite;

  /* Frame information */
  QVector<FrameInfo> frame_info;

private:
  /* Returns a transformed image */
  QPixmap transformPixmap(int index, int w, int h);

protected:
  /* Painting event */
//  void paintEvent(QPaintEvent *);

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

  /* Gets all of the sprites frames rotations */
  int getQuickRotation(); // TODO: Remove

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

  /* Sets the sprites id (Backend) */
  void setId(int);

  /* Sets the user submitted name */
  void setName(QString name);

  /* Sets the sprites opacity */
  void setOpacity(int opacity);

  /* Sets the sprites rotation */
  void setRotation(QString angle);

  /* Sets the frames vertical flip of a given frame */
  void setVerticalFlip(int,bool);

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
  /* Gets the current number of frames */
  int frameCount();

  /* Sets the frame angle of a given frame */
  int getFrameAngle(int frame_num);

  /* Gets the frames horizontal flip of a given frame */
  bool getHorizontalFlip(int frame_num);

  /* Gets the image path */
  QImage getImage(int frame_num);

  /* Gets the user submitted name of the sprite */
  QString getName();

  /* Gets the sprite path */
  QString getPath(int frame_num);

  /* Gets the sprite for alteration */
  Sprite* getSprite();

  /* Gets the frames vertical flip of a given frame */
  bool getVerticalFlip(int);

  /* Paint the base sprite */
  bool paint(QPainter* painter, int x, int y, int w, int h);
  bool paint(int index, QPainter* painter, int x, int y, int w, int h);

  /* Sets the sprite path */
  void setPath(int index, QString path);
};
#endif // EDITORSPRITE_H
