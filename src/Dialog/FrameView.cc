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
    setFixedSize(EditorHelpers::getTileSize() + 2,
                 EditorHelpers::getTileSize() + 2);
    connect(this, SIGNAL(editFrame(int)), parent, SLOT(editFrame(int)));
  }
  /* If the type is viewonly, set the size and do not make connections */
  else if(type == EditorEnumDb::VIEWONLY)
  {
    setFixedSize(EditorHelpers::getTileSize() + 2,
                 EditorHelpers::getTileSize() + 2);
  }
  /* If the type is anything else, the size is 32x32 and the appropriate
   * frame addition is triggered */
  else
  {
    setFixedSize(EditorHelpers::getTileSize() / 2,
                 EditorHelpers::getTileSize());
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
    /* If it's a frame, edit it */
    if(type == EditorEnumDb::FRAME)
    {
      emit editFrame(position_current);
    }
    /* Otherwise, if not in view mode, its tail, mid, or head */
    else if(type != EditorEnumDb::VIEWONLY)
    {
      QString path = QFileDialog::getOpenFileName(
                                          this, tr("Select A Frame To Add"),
                                          EditorHelpers::getPreviousPath(),
                                          tr("Image Files (*.png)"));
      if(path != "")
      {
        EditorHelpers::setPreviousPath(path);

        /* Choose the appropriate signal */
        if(type == EditorEnumDb::HEAD)
          emit addHead(path);
        else if(type == EditorEnumDb::MIDPOINT)
          emit addMidpoint(path, position_after);
        else if(type == EditorEnumDb::TAIL)
          emit addTail(path);
      }
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
