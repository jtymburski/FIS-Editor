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
  /* Make the stepper sprites */
  step_end = new EditorSprite(":/images/end.png");
  step_mid = new EditorSprite(":/images/midpoint.png");
  step_start = new EditorSprite(":/images/end.png");

  /* Set up the manipulator dialog and current sprites */
  currentsprite = c;
  manipulator = NULL;
  if(currentsprite != NULL)
    addFrames();

  setMinimumHeight(EditorHelpers::getTileSize() + 10);
}

/*
 * Description: Destructor Function
 */
FrameList::~FrameList()
{
  /* Delete step sprites */
  delete step_end;
  delete step_mid;
  delete step_start;
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

/*
 * Description: Recreates the frame dialog with the current sequence
 */
void FrameList::addFrames()
{
  int boxwidth = 0;

  /* Clear out the previous layout */
  if(layout() != NULL)
  {
    QLayoutItem* item;
    while((item = layout()->takeAt(0)) != NULL)
    {
      delete item->widget();
      delete item;
    }
    delete layout();
    label_frames.clear();
  }

  /* Start and end labels */
  label_start = new FrameView(this, EditorEnumDb::HEAD,
                              step_start, 0, 0, 0);
  label_end = new FrameView(this, EditorEnumDb::TAIL,
                            step_end, 0, 0, 0);

  /* Layout setup */
  QHBoxLayout* layout = new QHBoxLayout(this);
  layout->setSpacing(0);
  layout->addWidget(label_start);
  boxwidth += 32;

  /* Fill the layout */
  for(int i = 0; i < currentsprite->frameCount(); i++)
  {
    label_frames.push_back(new FrameView(this, EditorEnumDb::FRAME,
                                         currentsprite, i, 0, 0));
    boxwidth += 64;
    layout->addWidget(label_frames.last());

    if(i != currentsprite->frameCount() - 1)
    {
      label_frames.push_back(new FrameView(this,EditorEnumDb::MIDPOINT,
                                           step_mid, 0, i, i + 1));
      boxwidth += 32;
      layout->addWidget(label_frames.last());
    }
  }

  /* Finalize the layout */
  boxwidth += 64;
  layout->addWidget(label_end);
  setLayout(layout);
  setFixedWidth(boxwidth);
  update();
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
 * Description: Adds a midpoint frame
 *
 * Input: Frame path, before and after positions
 */
void FrameList::addMidpoint(QString x, int after)
{
  currentsprite->addMidpoint(x, after);
  addFrames();
}

/*
 * Description: Adds a new tail frame to the sequence
 *
 * Input: Frame path
 */
void FrameList::addTail(QString x)
{
  currentsprite->setPath(currentsprite->frameCount(), x);
  addFrames();
}

/*
 * Description: Edits the frame at position x
 *
 * Input: Frame position
 */
void FrameList::editFrame(int x)
{
  if(manipulator != NULL)
    delete manipulator;
  manipulator = new FrameDialog(this, currentsprite, x);
  manipulator->exec();
  addFrames();
}
