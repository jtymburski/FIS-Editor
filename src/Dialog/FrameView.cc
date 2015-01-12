/*******************************************************************************
 * Class Name: FrameView
 * Date Created: January 22, 2014
 * Inheritance: QWidget
 * Description: The frame and arrow images in the Frame Manipulator
 ******************************************************************************/
#include "Dialog/FrameView.h"
#include <QDebug>

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
  position_current = position;
  position_before = before;
  position_after = after;

  /* If the type is a frame, the size is the frame size and the edit dialog
   * is triggered */
  if(type == EditorEnumDb::FRAME)
  {
    setFixedSize(66, 66);
    connect(this, SIGNAL(editFrame(int)), parent, SLOT(editFrame(int)));
  }
  /* If the type is viewonly, set the size and do not make connections */
  else if(type == EditorEnumDb::VIEWONLY)
  {
    setFixedSize(66,66);
  }
  /* If the type is anything else, the size is 32x32 and the appropriate
   * frame addition is triggered */
  else
  {
    setFixedSize(32,32);
    connect(this, SIGNAL(addHead(QString)), parent, SLOT(addHead(QString)));
    connect(this, SIGNAL(addTail(QString)), parent, SLOT(addTail(QString)));
    connect(this, SIGNAL(addMidpoint(QString,int)),
            parent, SLOT(addMidpoint(QString,int)));
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
    QString path = "";
    switch(type)
    {
      case EditorEnumDb::FRAME:
        emit editFrame(position_current);
        break;
      case EditorEnumDb::HEAD:
        path = QFileDialog::getOpenFileName(this,
                                    tr("Select A Frame To Add To The Head"),
                                    EditorHelpers::getSpriteDir(),
                                    tr("Image Files (*.png)"));
        if(path != "")
          emit addHead(path);
        break;
      case EditorEnumDb::MIDPOINT:
        path = QFileDialog::getOpenFileName(this,
                                    tr("Select A Frame To Add To A Midpoint"),
                                    EditorHelpers::getSpriteDir(),
                                    tr("Image Files (*.png)"));
        if(path != "")
          emit addMidpoint(path, position_after);
        break;
      case EditorEnumDb::TAIL:
        path = QFileDialog::getOpenFileName(this,
                                    tr("Select A Frame To Add To The Tail"),
                                    EditorHelpers::getSpriteDir(),
                                    tr("Image Files (*.png)"));
        if(path != "")
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

    if(type == EditorEnumDb::FRAME || type == EditorEnumDb::VIEWONLY)
    {
      current_sprite->paint(position_current, &painter, 1, 1,
                            width() - 2, height() - 2);
      QPen pen = painter.pen();
      pen.setWidth(2);
      painter.setPen(pen);
      painter.drawRect(0, 0, width(), height());
    }
    else
    {
      current_sprite->paint(&painter, 0, 0, width(), height());
    }
  }
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
