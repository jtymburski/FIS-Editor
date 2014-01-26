/*******************************************************************************
 * Class Name: EditorSprite
 * Date Created: November 24, 2013
 * Inheritance: QWidget
 * Description: A sprite representation that appears in the toolbox
 ******************************************************************************/
#ifndef EDITORSPRITE_H
#define EDITORSPRITE_H

#include <QWidget>
#include <QPainter>
#include "EditorEnumDb.h"

/* Struct for frame option storage */
struct FrameOptions
{
  bool vertical_flip;
  bool horizontal_flip;
  bool ninetydeg;
  bool oneeightydeg;
  bool twoseventydeg;
};

class EditorSprite : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorSprite(QString img_path = " ");

  /* Destructor function */
  ~EditorSprite();

  /* Gets the sprite for alteration */
  Sprite* getSprite();

  /* Sets the sprite path */
  void setPath(QString path);

  /* Sets the image path */
  void setImage(QImage img);

  /* Gets the sprite path */
  QString getPath(int pos);

  /* Gets the image path */
  QImage getImage(int);

  /* Gets the user submitted name of the sprite */
  QString getName();

  /* Gets the current number of frames */
  int frameCount();

  /* Gets the frames horizontal and vertical flip of a given frame */
  bool getHorizontalFlip(int);
  bool getVerticalFlip(int);

  /* Sets the frame angle of a given frame */
  int getFrameAngle(int frame_num);

public slots:
  /* Adds a frame to the top of the sequence */
  void addHead(QString name);

  /* Adds a frame to the top of the sequence */
  void addMidpoint(QString name, int before, int after);

  /* Sets the user submitted name */
  void setName(QString name);

  /* Sets the sprites animation time */
  void setAnimationTime(QString time);

  /* Sets the sprites color mask */
  void setColorRed(int red);
  void setColorBlue(int blue);
  void setColorGreen(int green);

  /* Sets the sprites direction */
  void setDirection(int dir);
  void setDirectionForward();
  void setDirectionReverse();

  /* Sets the sprites opacity */
  void setOpacity(int opacity);

  /* Sets the sprites rotation */
  void setRotation(QString angle);

  /* Sets the sprites brightness */
  void setBrightness(int brightness);

  /* Sets the sprites id (Backend) */
  void setId(int);

  /* Sets the frames horizontal and vertical flip of a given frame */
  void setHorizontalFlip(int,bool);
  void setVerticalFlip(int,bool);

  /* Sets the frame angle of a given frame */
  void setFrameAngle(int frame_num,int angle);

  /* Sets all of the sprites frames to 90 deg rotation */
  void set90();

  /* Sets all of the sprites frames to 180 deg rotation */
  void set180();

  /* Sets all of the sprites frames to 270 deg rotation */
  void set270();

  /* Sets all of the sprites frames to 270 deg rotation */
  void set0();

  /* Gets all of the sprites frames rotations */
  int getQuickRotation();

protected:
  /* Painting event */
  void paintEvent(QPaintEvent *);

private:
  /* The view mode for the border */
  EditorEnumDb::Border mode;

  /* Name */
  QString name;

  /* The actual Sprite for in game */
  Sprite* sprite;

  /* The path to the sprite frames */
  QVector<QString> frame_paths;

  /* The sprite frame images */
  QVector<QImage> frame_images;

  /* The frame angles */
  QVector<int> frame_angles;

  /* The frame horizontal flips */
  QVector<bool> frame_hflip;

  /* The frame vertical flips */
  QVector<bool> frame_vflip;

  /* Frame option storage */
  FrameOptions frame_options;

  /* Quick flip storage */
  bool flipped0;
  bool flipped90;
  bool flipped180;
  bool flipped270;


signals:

};
#endif // IMAGECHOICE_H
