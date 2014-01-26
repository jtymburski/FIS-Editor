/*******************************************************************************
 * Class Name: Manipulabel
 * Date Created: January 22, 2014
 * Inheritance: QWidget
 * Description: The frame and arrow images in the Frame Manipulator
 ******************************************************************************/
#include "Manipulabel.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function
 *
 * Input: parent widget, type, frame image, current, before, after positions
 *        and frame direction
 */

Manipulabel::Manipulabel(QWidget *parent,EditorEnumDb::ManipulabelType type,
                         QPixmap* frame_image,
                         int position, int before, int after, QString framedir)
  : QWidget(parent)
{
  t = type;
  framepath = framedir;
  pos = position;
  bef = before;
  aft = after;
  horflip = false;
  verflip = false;
  /* If the type is a frame, the size is the frame size and the edit dialog
   * is triggered */
  if(t == EditorEnumDb::FRAME)
  {
    setFixedSize(64,64);
    frame = frame_image;
    connect(this,SIGNAL(editFrame(int)),parent,SLOT(editFrame(int)));
  }
  /* If the type is anything else, the size is 32x32 and the appropriate
   * frame addition is triggered */
  else
  {
    setFixedSize(32,32);
    connect(this,SIGNAL(addHead(QString)),parent,SLOT(addHead(QString)));
    connect(this,SIGNAL(addTail(QString)),parent,SLOT(addTail(QString)));
    connect(this,SIGNAL(addMidpoint(QString,int,int)),
            parent,SLOT(addMidpoint(QString,int,int)));
    frame = frame_image;
  }
}

/*
 * Description: Destructor function
 */
Manipulabel::~Manipulabel()
{
}


/*
 * Description: Paints the label with transformations
 */
void Manipulabel::paintEvent(QPaintEvent *)
{
  QPainter painter(this);
  QPixmap temp = *frame;
  QTransform transform;

  if(horflip)
  {
    qreal m11 = transform.m11();    // Horizontal scaling
    qreal m12 = transform.m12();    // Vertical shearing
    qreal m13 = transform.m13();    // Horizontal Projection
    qreal m21 = transform.m21();    // Horizontal shearing
    qreal m22 = transform.m22();    // vertical scaling
    qreal m23 = transform.m23();    // Vertical Projection
    qreal m31 = transform.m31();    // Horizontal Position (DX)
    qreal m32 = transform.m32();    // Vertical Position (DY)
    qreal m33 = transform.m33();    // Addtional Projection Factor

    qreal scale = m11;

    m11 = -m11;

    // Re-position back to origin
    if(m31 > 0)
      m31 = 0;
    else
      m31 = (width() * scale);

    // Write back to the matrix
    transform.setMatrix(m11, m12, m13, m21, m22, m23, m31, m32, m33);
  }
  if(verflip)
  {
    qreal m11 = transform.m11();    // Horizontal scaling
    qreal m12 = transform.m12();    // Vertical shearing
    qreal m13 = transform.m13();    // Horizontal Projection
    qreal m21 = transform.m21();    // Horizontal shearing
    qreal m22 = transform.m22();    // vertical scaling
    qreal m23 = transform.m23();    // Vertical Projection
    qreal m31 = transform.m31();    // Horizontal Position (DX)
    qreal m32 = transform.m32();    // Vertical Position (DY)
    qreal m33 = transform.m33();    // Addtional Projection Factor

    qreal scale = m22;
    m22 = -m22;

    // Re-position back to origin
    if(m32 > 0)
      m32 = 0;
    else
      m32 = (height() * scale);

    // Write back to the matrix
    transform.setMatrix(m11, m12, m13, m21, m22, m23, m31, m32, m33);
  }
  painter.drawPixmap(0,0,width(),height(),temp.transformed(transform));
}


/*
 * Description: Double click event for editing and adding frames
 *
 * Input : Event
 */
void Manipulabel::mouseDoubleClickEvent(QMouseEvent *event)
{
  if(event->buttons() & Qt::LeftButton)
  {
    QString path;
    switch(t)
    {
      case EditorEnumDb::FRAME:
        emit editFrame(pos);
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
        emit addMidpoint(path,bef,aft);
        break;
      case EditorEnumDb::TAIL:
        path = QFileDialog::getOpenFileName(this,
                                    tr("Select A Frame To Add To The Tail"),
                                    framepath,tr("Image Files (*.png)"));
        emit addTail(path);
        break;
      default:
        break;
    }
  }
}


/*
 * Description: Sets horizontal and vertical flips
 *
 * Input: Flip toggles
 */
void Manipulabel::setFlips(bool horizontal, bool vertical)
{
  horflip = horizontal;
  verflip = vertical;
  update();
}

/*
 * Description: Sets horizontal flip
 *
 * Input: Flip toggle
 */
void Manipulabel::setHFlip(bool horizontal)
{
  horflip = horizontal;
  update();
}

/*
 * Description: Sets vertical flip
 *
 * Input: Flip toggle
 */
void Manipulabel::setVFlip(bool vertical)
{
  verflip = vertical;
  update();
}
