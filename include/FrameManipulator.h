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

class FrameManipulator : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  FrameManipulator(QWidget* parent = 0, EditorSprite* c = 0);

  /* Destructor Function */
  ~FrameManipulator();

  /* Frame Addition */
  void addFrames(EditorSprite* e);

protected:
  /* Mouse right click menu */
  void mousePressEvent(QMouseEvent *);
private:
  /* Sprite whose frames are in the dialog */
  EditorSprite* currentsprite;
  FrameManipulationDialog* manipulator;

  /* Vector of frame images */
  QVector<QPixmap*> frames;
  QVector<QLabel*> framelabels;
  QVector<QPixmap*> arrows;
  QVector<QLabel*> arrowlabels;
  QPixmap* start;
  QLabel* startlabel;
  QPixmap* end;
  QLabel* endlabel;
};

#endif // FRAMEMANIPULATOR_H
