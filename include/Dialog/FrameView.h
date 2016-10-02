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
#include "EditorHelpers.h"
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
  /* Editor sprite pointer */
  EditorSprite* current_sprite;

  /* Position Before After and Current */
  int position_after;
  int position_before;
  int position_current;

  /* Label type */
  EditorEnumDb::FrameViewType type;

protected:
  /* Mouse press event */
  void mouseDoubleClickEvent(QMouseEvent *event);

  /* Painting */
  void paintEvent(QPaintEvent *);

signals:
  /* Calls edit frame */
  void editFrame(int);

  /* Adds a midpoint */
  void addMidpoint(QString,int);

  /* Adds a new head frame */
  void addHead(QString);

  /* Adds a new tail frame */
  void addTail(QString);

public:
};

#endif // FRAMEVIEW_H
