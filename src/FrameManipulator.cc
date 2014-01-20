/*******************************************************************************
 * Class Name: FrameManipulator
 * Date Created: January 19, 2014
 * Inheritance: QWidget
 * Description: The frame implement in the sprite creation dialog
 ******************************************************************************/

#include "FrameManipulator.h"

FrameManipulator::FrameManipulator(QWidget *parent, EditorSprite *c)
  : QWidget(parent)
{
  currentsprite = c;
  manipulator = new FrameManipulationDialog(this,currentsprite,0);
  manipulator->setModal(true);
  manipulator->hide();
  addFrames(currentsprite);
}

FrameManipulator::~FrameManipulator()
{
}

void FrameManipulator::addFrames(EditorSprite *e)
{
  currentsprite = e;
  start = new QPixmap(":/Icons/Resources/start.png");
  startlabel = new QLabel(this);
  startlabel->setPixmap(start->scaled(32,32));

  end = new QPixmap(":/Icons/Resources/end.png");
  endlabel = new QLabel(this);
  endlabel->setPixmap(end->scaled(32,32));


  QHBoxLayout* layout = new QHBoxLayout(this);
  layout->setSpacing(0);
  layout->addWidget(startlabel);

  for(int i=0; i<currentsprite->frameCount(); i++)
  {
    frames.push_back(new QPixmap(currentsprite->getPath(i)));
    framelabels.push_back(new QLabel(currentsprite->getPath(i)));
    framelabels.last()->setPixmap(*frames.last());
    layout->addWidget(framelabels.last());

    if(i != currentsprite->frameCount()-1)
    {
      arrows.push_back(new QPixmap(":/Icons/Resources/midpoint.png"));
      arrowlabels.push_back(new QLabel("Mid"));
      arrowlabels.last()->setPixmap(arrows.last()->scaled(32,32));
      layout->addWidget(arrowlabels.last());
    }
  }

  layout->addWidget(endlabel);
  setLayout(layout);
}

void FrameManipulator::mousePressEvent(QMouseEvent *event)
{
  if(event->button() & Qt::RightButton)
  {
    delete manipulator;
    manipulator = new FrameManipulationDialog(this,currentsprite,0);
    manipulator->exec();
  }
}
