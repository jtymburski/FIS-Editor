/*******************************************************************************
 * Class Name: FrameView
 * Date Created: January 22, 2014
 * Inheritance: QWidget
 * Description: The frame and arrow images in the Frame Manipulator
 ******************************************************************************/
#include "Dialog/FrameView.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function
 *
 * Input: parent widget, type, frame image, current, before, after positions
 *        and frame direction
 */

FrameView::FrameView(QWidget *parent, EditorEnumDb::FrameViewType type,
                     EditorSprite* current, int position, int before, int after)
  : QWidget(parent)
{
  current_sprite = current;
  this->type = type;
  //framepath = framedir;
  position_current = position;
  position_before = before;
  position_after = after;
  //horflip = false;
  //verflip = false;

  /* If the type is a frame, the size is the frame size and the edit dialog
   * is triggered */
  if(type == EditorEnumDb::FRAME)
  {
    setFixedSize(64, 64);
    //frame = frame_image;
    connect(this, SIGNAL(editFrame(int)), parent, SLOT(editFrame(int)));
  }
  /* If the type is viewonly, set the size and do not make connections */
  else if(type == EditorEnumDb::VIEWONLY)
  {
    setFixedSize(64,64);
    //frame = frame_image;
  }
  /* If the type is anything else, the size is 32x32 and the appropriate
   * frame addition is triggered */
  else
  {
    setFixedSize(32,32);
    connect(this, SIGNAL(addHead(QString)), parent, SLOT(addHead(QString)));
    connect(this, SIGNAL(addTail(QString)), parent, SLOT(addTail(QString)));
    connect(this, SIGNAL(addMidpoint(QString,int,int)),
            parent, SLOT(addMidpoint(QString,int,int)));
    //frame = frame_image;
  }
}

/*
 * Description: Destructor function
 */
FrameView::~FrameView()
{
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: Double click event for editing and adding frames
 *
 * Input : Event
 */
void FrameView::mouseDoubleClickEvent(QMouseEvent *event)
{
  if((event->buttons() & Qt::LeftButton) && current_sprite != NULL)
  {
    QString framepath = current_sprite->getPath(0);
    QString path = "";
    switch(type)
    {
      case EditorEnumDb::FRAME:
        emit editFrame(position_current);
        break;
      case EditorEnumDb::HEAD:
        path = QFileDialog::getOpenFileName(this,
                                    tr("Select A Frame To Add To The Head"),
                                    framepath,tr("Image Files (*.png)"));

        emit addHead(path);
        break;
      case EditorEnumDb::MIDPOINT:
        path = QFileDialog::getOpenFileName(this,
                                    tr("Select A Frame To Add To A Midpoint"),
                                    framepath,tr("Image Files (*.png)"));
        emit addMidpoint(path, position_before, position_after);
        break;
      case EditorEnumDb::TAIL:
        path = QFileDialog::getOpenFileName(this,
                                    tr("Select A Frame To Add To The Tail"),
                                    framepath,tr("Image Files (*.png)"));
        emit addTail(path);
        break;
      case EditorEnumDb::VIEWONLY:
      default:
        break;
    }
  }
}

/*
 * Description: Paints the label with transformations
 */
void FrameView::paintEvent(QPaintEvent *)
{
  if(current_sprite != NULL)
  {
    QPainter painter(this);

    if(type == EditorEnumDb::FRAME)
      current_sprite->paint(position_current, &painter, 0, 0,
                            width(), height());
    else
      current_sprite->paint(&painter, 0, 0, width(), height());
  }
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Reloads the frame image
 */
void FrameView::reloadFrame()
{
  //frame = new QPixmap(currentsprite->getPath(pos));
  //framepath = currentsprite->getPath(pos);
  update();
}

/*
 * Description: Sets horizontal and vertical flips
 *
 * Input: Flip toggles
 */
//void FrameView::setFlips(bool horizontal, bool vertical)
//{
//  horflip = horizontal;
//  verflip = vertical;
//  update();
//}

/*
 * Description: Sets horizontal flip
 *
 * Input: Flip toggle
 */
//void FrameView::setHFlip(bool horizontal)
//{
//  horflip = horizontal;
//  update();
//}

/*
 * Description: Sets vertical flip
 *
 * Input: Flip toggle
 */
//void FrameView::setVFlip(bool vertical)
//{
//  verflip = vertical;
//  update();
//}
