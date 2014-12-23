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


public slots:
  /* Replaces frame at position x */
  void editFrame(int x);
  /* Appends a frame to the head */
  void addHead(QString x);
  /* Appends a frame to the tail */
  void addTail(QString x);
  /* Appends a frame to the sequence */
  void addMidpoint(QString x, int before, int after);
  /* Frame Addition */
  void addFrames();

protected:
private:
  /* Sprite whose frames are in the dialog */
  EditorSprite* currentsprite;
  FrameDialog* manipulator;

  /* Vector of frame images */
  QVector<FrameView*> framelabels;

  /* Vector of midpoint arrows */
  QVector<FrameView*> arrowlabels;

  /* Head label */
  FrameView* startlabel;

  /* Tail label */
  FrameView* endlabel;

  /* Layout */
  QHBoxLayout* layout;
};

#endif // FRAMELIST_H
