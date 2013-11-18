/******************************************************************************
* Class Name: SpriteChoice
* Date Created: Nov 16, 2013
* Inheritance: QWidget
* Description: This class is a toolbox view of each sprite that the user can
*              add to the map, these are imported by the user.

******************************************************************************/
#include "SpriteChoice.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function - Requires a path
 *
 * Input: File path
 */
SpriteChoice::SpriteChoice(QWidget *parent, QString p, int id) : QWidget(parent)
{
  /* Sets up size of the boundary and loads the given image into memory */
  setGeometry(0,0,66,66);
  path = p;
  id_number = id;
  mode = STANDARD;
  pic = new QImage(path);
  pic->load(path);
  connect(this, SIGNAL(chosen(int)),parent,SLOT(deselectOthers(int)));
  connect(this,SIGNAL(pathOfImage(QString)),parent,SLOT(setSprite(QString)));
}

/*
 * Description: Destructor Function
 */
SpriteChoice::~SpriteChoice()
{
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Loads a sprite from the given file path, updates the image
 *
 * Inputs: File path
 */
void SpriteChoice::loadSprite(QString path)
{
  pic->load(path);
  update();
}


/*
 * Description: Deselects this sprite choice
 */
void SpriteChoice::deselect()
{
  mode = STANDARD;
  update();
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: The paint event for the sprite. Sets up the bounding box, then
 *              draws the current image into said box
 *
 * Inputs: Unused
 */
void SpriteChoice::paintEvent(QPaintEvent *)
{
  QPainter painter(this);
  /* If the path is valid (which it should never not be), draw the result into
   * the frame with a one pixel thick black border */
  if(path != NULL)
  {
    switch(mode)
    {
      case STANDARD:
        painter.setPen(Qt::black);
        break;
      case HOVERED:
        painter.setPen(Qt::cyan);
        break;
      case SELECTED:
        painter.setPen(Qt::green);
        break;
      default:
        painter.setPen(Qt::black);
        break;
    }

    painter.drawRect(0,0,65,65);
    QRect* bound = new QRect(1,1,64,64);
    painter.drawImage(*bound,*pic);
  }
}

/*
 * Description: The mouse entry event, changes the bounding box color
 *
 * Inputs: Unused
 */
void SpriteChoice::enterEvent(QEvent *)
{
  if(mode != SELECTED)
  {
    mode = HOVERED;
    update();
  }
}

/*
 * Description: The mouse leave event, changes the bounding box color
 *
 * Inputs: Unused
 */
void SpriteChoice::leaveEvent(QEvent *)
{
  if(mode != SELECTED)
  {
    mode = STANDARD;
    update();
  }
}

/*
 * Description: The mouse press event, changes the bounding box color
 *
 * Inputs: Unused
 */
void SpriteChoice::mousePressEvent(QMouseEvent *event)
{
  if(event->button() == Qt::LeftButton)
  {
    if(mode != SELECTED)
    {
      mode = SELECTED;
      emit chosen(id_number);
      emit pathOfImage(path);
    }
    else
      mode = STANDARD;
  }
  update();
}


