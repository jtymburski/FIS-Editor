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
  EditorSprite();

  /* Destructor function */
  ~EditorSprite();

  /* Gets the sprite for alteration */
  Sprite* getSprite();

  /* Sets the sprite path */
  void setPath(QString p);

  /* Gets the user submitted name of the sprite */
  QString getName();

public slots:
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
  void setId(int id);


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

  /* Frame option storage */
  FrameOptions frame_options;


signals:

};
#endif // IMAGECHOICE_H
