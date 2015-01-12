/*******************************************************************************
 * Class Name: FrameList
 * Date Created: January 19, 2014
 * Inheritance: QWidget
 * Description: The frame implement in the sprite creation dialog
 ******************************************************************************/
#ifndef FRAMELIST_H
#define FRAMELIST_H

#include <QVector>
#include <QWidget>
#include <QHBoxLayout>
#include <QMouseEvent>

#include "Dialog/FrameDialog.h"
#include "Database/EditorSprite.h"
#include "Dialog/FrameView.h"

class FrameList : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  FrameList(QWidget* parent = NULL, EditorSprite* c = NULL);

  /* Destructor Function */
  ~FrameList();

private:
  /* Sprite whose frames are in the dialog */
  EditorSprite* currentsprite;

  /* Labels in the list */
  FrameView* label_end;
  QVector<FrameView*> label_frames;
  FrameView* label_start;

  /* The dialog manipulator pop-up */
  FrameDialog* manipulator;

  /* Frame step sprites */
  EditorSprite* step_end;
  EditorSprite* step_mid;
  EditorSprite* step_start;

private:

protected:

public slots:
  /* Frame Addition */
  void addFrames();

  /* Appends a frame to the head */
  void addHead(QString x);

  /* Appends a frame to the sequence */
  void addMidpoint(QString x, int after);

  /* Appends a frame to the tail */
  void addTail(QString x);

  /* Replaces frame at position x */
  void editFrame(int x);
};

#endif // FRAMELIST_H
