/*******************************************************************************
 * Class Name: FrameList
 * Date Created: January 19, 2014
 * Inheritance: QWidget
 * Description: The frame implement in the sprite creation dialog
 ******************************************************************************/
#include "Dialog/FrameList.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function
 *
 * Input: parent widget, current sprite
 */
FrameList::FrameList(QWidget *parent, EditorSprite *c)
  : QWidget(parent)
{
  currentsprite = c;
  manipulator = new FrameDialog(this,currentsprite,0);
  manipulator->setModal(true);
  manipulator->hide();
  connect(manipulator,SIGNAL(finishedSave()),this,SLOT(addFrames()));
  layout = new QHBoxLayout(this);
  addFrames();
}


/*
 * Description: Destructor Function
 */
FrameList::~FrameList()
{
}

/*
 * Description: Recreates the frame dialog with the current sequence
 */
void FrameList::addFrames()
{
  int boxwidth = 0;
  for(int i=0; i<layout->count(); i++)
  {
    layout->itemAt(i)->widget()->hide();
  }
  delete QWidget::layout();

  startlabel = new FrameView(this,EditorEnumDb::HEAD,
                               new QPixmap(":/Icons/Resources/start.png"),0,0,0,
                               currentsprite->getPath(0));
  endlabel = new FrameView(this,EditorEnumDb::TAIL,
                             new QPixmap(":/Icons/Resources/end.png"),0,0,0,
                             currentsprite->getPath(0));

  layout = new QHBoxLayout(this);
  layout->setSpacing(0);
  layout->addWidget(startlabel);
  boxwidth += 32;

  for(int i=0; i<currentsprite->frameCount(); i++)
  {

    framelabels.push_back(new FrameView(this,EditorEnumDb::FRAME,
                             new QPixmap(currentsprite->getPath(i)),
                                          i,0,0,currentsprite->getPath(0),
                                          currentsprite));
    framelabels.last()->setHFlip(currentsprite->getHorizontalFlip(i));
    framelabels.last()->setVFlip(currentsprite->getVerticalFlip(i));
    boxwidth += 64;
    layout->addWidget(framelabels.last());

    if(i != currentsprite->frameCount()-1)
    {
      arrowlabels.push_back(new FrameView(this,EditorEnumDb::MIDPOINT,
                             new QPixmap(":/Icons/Resources/midpoint.png"),
                              0,i,i+1,currentsprite->getPath(0)));
      boxwidth += 32;
      layout->addWidget(arrowlabels.last());
    }
  }
  boxwidth += 64;
  layout->addWidget(endlabel);
  setLayout(layout);
  setFixedWidth(boxwidth);
  update();
}

/*
 * Description: Edits the frame at position x
 *
 * Input: Frame position
 */
void FrameList::editFrame(int x)
{
  delete manipulator;
  manipulator = new FrameDialog(this,currentsprite,x);
  connect(manipulator,SIGNAL(finishedSave()),this,SLOT(addFrames()));
  manipulator->exec();
}

/*
 * Description: Adds a new head frame to the sequence
 *
 * Input: Frame path
 */
void FrameList::addHead(QString x)
{
  currentsprite->addHead(x);
  addFrames();
}

/*
 * Description: Adds a new tail frame to the sequence
 *
 * Input: Frame path
 */
void FrameList::addTail(QString x)
{
  currentsprite->setPath(x);
  addFrames();
}

/*
 * Description: Adds a midpoint frame
 *
 * Input: Frame path, before and after positions
 */
void FrameList::addMidpoint(QString x, int before, int after)
{
  currentsprite->addMidpoint(x,before,after);
  addFrames();
}
