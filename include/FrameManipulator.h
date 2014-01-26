/*******************************************************************************
 * Class Name: FrameManipulator
 * Date Created: January 19, 2014
 * Inheritance: QWidget
 * Description: The frame implement in the sprite creation dialog
 ******************************************************************************/
#ifndef FRAMEMANIPULATOR_H
#define FRAMEMANIPULATOR_H

#include <QPixmap>
#include <QVector>
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QMouseEvent>
#include "FrameManipulationDialog.h"
#include "EditorSprite.h"
#include "Manipulabel.h"

class FrameManipulator : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  FrameManipulator(QWidget* parent = 0, EditorSprite* c = 0);

  /* Destructor Function */
  ~FrameManipulator();


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
  FrameManipulationDialog* manipulator;

  /* Vector of frame images */
  QVector<Manipulabel*> framelabels;

  /* Vector of midpoint arrows */
  QVector<Manipulabel*> arrowlabels;

  /* Head label */
  Manipulabel* startlabel;

  /* Tail label */
  Manipulabel* endlabel;

  /* Layout */
  QHBoxLayout* layout;
};

#endif // FRAMEMANIPULATOR_H
