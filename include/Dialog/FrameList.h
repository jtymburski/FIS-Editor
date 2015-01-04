/*******************************************************************************
 * Class Name: FrameList
 * Date Created: January 19, 2014
 * Inheritance: QWidget
 * Description: The frame implement in the sprite creation dialog
 ******************************************************************************/
#ifndef FRAMELIST_H
#define FRAMELIST_H

#include <QPixmap>
#include <QVector>
#include <QWidget>
#include <QLabel>
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
  FrameList(QWidget* parent = 0, EditorSprite* c = 0);

  /* Destructor Function */
  ~FrameList();

private:
  /* Vector of midpoint arrows */
  QVector<FrameView*> arrowlabels;

  /* Sprite whose frames are in the dialog */
  EditorSprite* currentsprite;

  /* Tail label */
  FrameView* endlabel;

  /* Vector of frame images */
  QVector<FrameView*> framelabels;

  /* Layout */
  QHBoxLayout* layout;

  /* The dialog manipulator pop-up */
  FrameDialog* manipulator;

  /* Head label */
  FrameView* startlabel;

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
  void addMidpoint(QString x, int before, int after);

  /* Appends a frame to the tail */
  void addTail(QString x);

  /* Replaces frame at position x */
  void editFrame(int x);
};

#endif // FRAMELIST_H
