/*******************************************************************************
 * Class Name: Manipulabel
 * Date Created: January 22, 2014
 * Inheritance: QWidget
 * Description: The frame and arrow images in the Frame Manipulator
 ******************************************************************************/
#ifndef MANIPULABEL_H
#define MANIPULABEL_H

#include <QMouseEvent>
#include <QPixmap>
#include <QWidget>
#include <QPainter>
#include <QFileDialog>
#include "FrameManipulationDialog.h"
#include "EditorEnumDb.h"

class Manipulabel : public QWidget
{
  Q_OBJECT
public:
  /* Constructor function */
  Manipulabel(QWidget* parent = 0,
              EditorEnumDb::ManipulabelType type = EditorEnumDb::FRAME,
      QPixmap* frame_image = 0,int position = 0, int before = 0, int after = 0,
              QString framedir = QDir::current().absolutePath());

  /* Destructor function */
  ~Manipulabel();

  /* Flip setting */
  void setFlips(bool horizontal, bool vertical);
  void setHFlip(bool horizontal);
  void setVFlip(bool vertical);
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

private:
  /* Label type */
  EditorEnumDb::ManipulabelType t;

  /* The Manipulation Dialog */
  FrameManipulationDialog* manipulator;

  /* Position Before After and Current */
  int pos,bef,aft;

  /* A pointer to the frame for viewing */
  QPixmap* frame;

  /* Horizontal and Vertical flip */
  bool horflip,verflip;

  /* Path to the frame */
  QString framepath;
};

#endif // MANIPULABEL_H
