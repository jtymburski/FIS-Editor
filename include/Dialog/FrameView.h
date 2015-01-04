/*******************************************************************************
 * Class Name: FrameView
 * Date Created: January 22, 2014
 * Inheritance: QWidget
 * Description: The frame and arrow images in the Frame Manipulator
 ******************************************************************************/
#ifndef FrameView_H
#define FrameView_H

#include <QFileDialog>
#include <QMouseEvent>
#include <QWidget>

#include "EditorEnumDb.h"
#include "Database/EditorSprite.h"

class FrameView : public QWidget
{
  Q_OBJECT
public:
  /* Constructor function */
  FrameView(QWidget* parent = NULL,
            EditorEnumDb::FrameViewType type = EditorEnumDb::FRAME,
            EditorSprite* current = NULL, int position = 0, int before = 0,
            int after = 0);

  /* Destructor function */
  ~FrameView();

private:
  /* Label type */
  EditorEnumDb::FrameViewType type;

  /* Editor sprite pointer */
  EditorSprite* current_sprite;

  /* Position Before After and Current */
  int position_after;
  int position_before;
  int position_current;

  /* A pointer to the frame for viewing */
  //QPixmap* frame;

  /* Horizontal and Vertical flip */
  //bool horflip,verflip;

  /* Path to the frame */
  //QString framepath;

protected:
  /* Mouse press event */
  void mouseDoubleClickEvent(QMouseEvent *event);

  /* Painting */
  void paintEvent(QPaintEvent *);

signals:
  /* Calls edit frame */
  void editFrame(int);

  /* Adds a midpoint */
  void addMidpoint(QString,int,int);

  /* Adds a new head frame */
  void addHead(QString);

  /* Adds a new tail frame */
  void addTail(QString);

public:
  /* Reloads the frames after changes */
  void reloadFrame();

  /* Set angle */
  //void setAngle(int angle);

  /* Painting control */
  //QPixmap setBrightness(int value, QPixmap original);
  //QPixmap setColor(int red, int blue, int green, QPixmap original);

  /* Flip setting */
  //void setFlips(bool horizontal, bool vertical);
  //void setHFlip(bool horizontal);
  //void setVFlip(bool vertical);
};

#endif // FrameView_H
