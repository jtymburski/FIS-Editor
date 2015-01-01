/*******************************************************************************
 * Class Name: SpriteView
 * Date Created: November 24, 2013
 * Inheritance: QWidget
 * Description: This class handles the toolbox on the side of the application
 ******************************************************************************/
#include "View/SpriteView.h"
#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function - Set up toolbox
 *
 * Input: parent widget
 */
SpriteView::SpriteView(QWidget *parent) : QWidget(parent)
{
  QVBoxLayout* layout = new QVBoxLayout(this);

  current = new EditorSprite();
  //QWidget* widget = new QWidget(this);
  //widget->setStyleSheet("background-color:red;");
  //edit_sprite = new SpriteDialog(this);
  bottom_view = new QWidget(this);
  bottom_view->setMinimumHeight(150);
  editor_sprite_list = new EditorSpriteList(this);
  //editor_sprite_list->show();
  //editor_sprite_list->setFixedSize(288,512);
  connect(editor_sprite_list,SIGNAL(currentRowChanged(int)),
          this,SLOT(setCurrent(int)));
  connect(editor_sprite_list,SIGNAL(updateSprites()),this,SLOT(refreshList()));
  nextID = 1;

  //layout->addWidget(widget);//editor_sprite_list);
  layout->addWidget(editor_sprite_list);
  layout->addWidget(bottom_view);
  setLayout(layout);
}

/*
 * Description: Destructor function
 *
 * Input: none
 */
SpriteView::~SpriteView()
{
  //qDebug()<<"Removing Editor Sprite Toolbox";
  //delete edit_sprite;
  for(int i=0; i<editor_sprites.size(); i++)
  {
    delete editor_sprites[i];
    editor_sprites[i] = NULL;
  }
  //delete current;
  current = NULL;
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: The paint event for the toolbox, this mainly sets up the
 *              positions for all of the sprites that have been selected and
 *              added to the toolbar
 *
 * Inputs: Unused
 */
void SpriteView::paintEvent(QPaintEvent *)
{
  int l = 64;

  if(bottom_view != NULL)
  {
    QPainter painter(this);
    painter.setBackgroundMode(Qt::TransparentMode);

    /* Set bounds */
    QRect border(bottom_view->x(), bottom_view->y(), l + 2, l + 2);

    /* Paint the sprite */
    if(current != NULL)
      current->paint(&painter, bottom_view->x() + 1, bottom_view->y() + 1, l, l);

    /* Paint the border */
    painter.setPen(QPen(QBrush(Qt::black), 2));
    painter.setOpacity(1.0);
    painter.drawRect(border);

    /* Name and frame count text */
    if(current != NULL)
    {
      painter.setFont(QFont("helvetica", 14, QFont::Bold));
      painter.drawText(l * 1.5, bottom_view->y() + 20, current->getName());

      painter.setFont(QFont("helvetica", 12));
      painter.drawText(l * 1.5, bottom_view->y() + 40,
                       "Framecount: " + QString::number(current->frameCount()));
    }
  }
}

/*
 * Description: The mouse double click event brings up a display that contains
 * all of the frames in the current sprite selected, right now this was just for
 * debugging reasons, however it may have a use later on
 *
 * Inputs: Mouse event
 */
void SpriteView::mouseDoubleClickEvent(QMouseEvent *e)
{

  if(e->button() & Qt::RightButton)
  {
      edit_sprite = new SpriteDialog(this,current,
                                             current->getPath(0),
                                             0,false);

      connect(edit_sprite,SIGNAL(ok()),this,SLOT(refreshList()));
      edit_sprite->show();
  }
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

/*
 * Description: Adds an editor sprite to the toolbox
 *
 * Inputs: The editor sprite to add
 */
void SpriteView::addEditorSprite(EditorSprite *e)
{
  /* Sets the id */
  e->getSprite()->setId(nextID);

  /* Puts the sprite in the library */
  editor_sprites.push_back(e);

  /* Increments the id tracker */
  nextID++;

  /* Adds the item to the visible list */
  editor_sprite_list->addItem(e->getName());
  editor_sprite_list->setCurrentRow(editor_sprite_list->count()-1);
  editor_sprite_list->setCurrentSprite(e);
  update();
}

/*
 * Description: Gets the current editor sprite
 *
 * Output: The current editor sprite
 */
EditorSprite* SpriteView::getCurrent()
{
  return current;
}

/* Description: Refreshes the Editor Sprite list
 *
 */
void SpriteView::refreshList()
{
  for(int i=0; i<editor_sprites.size(); i++)
    editor_sprite_list->item(i)->setText(editor_sprites[i]->getName());
  update();
}

/*
 * Description: Sets the index of the current editor sprite
 *
 * Inputs: Index
 */
void SpriteView::setCurrent(int index)
{
  current = editor_sprites[index];
  editor_sprite_list->setCurrentSprite(current);
  update();
}

/*
 * Description: Transforms the given frame from the given sprite
 *
 * Input: Editor sprite and position (Default is zero)
 *
 * Output: Returns the transformed pixmap
 */
QPixmap SpriteView::transformPixmap(EditorSprite* pic, int pos)
{
  QTransform transform;

  /* Execute horizontal flip */
  if(pic->getHorizontalFlip(pos))
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
      m31 = (64 * scale);

    // Write back to the matrix
    transform.setMatrix(m11, m12, m13, m21, m22, m23, m31, m32, m33);
  }

  /* Execute vertical flip */
  if(pic->getVerticalFlip(pos))
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
      m32 = (64 * scale);

    // Write back to the matrix
    transform.setMatrix(m11, m12, m13, m21, m22, m23, m31, m32, m33);
  }

  /* Transform */
  QTransform trans = transform;
  transform = trans.rotate(pic->getFrameAngle(pos));
  QPixmap returnimage(pic->getPath(pos));

  /* Modify brightness and color */
  if(current != NULL)
  {
    returnimage = setBrightness(current->getBrightness()-255,returnimage);
    returnimage = setColor(current->getColorRed(),
                           current->getColorBlue(),
                           current->getColorGreen(),
                           returnimage);
  }
  return returnimage.transformed(transform);
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

// TODO: Comment
QPixmap SpriteView::setBrightness(int delta, QPixmap original)
{
  QImage original_image = original.toImage();
  QImage editing = original.toImage();
  QColor old_color;
  int r,g,b;

  for(int i=0; i<editing.width(); i++)
  {
    for(int j=0; j<editing.height(); j++)
    {
      if(qAlpha(original_image.pixel(i,j)) > 0)
      {
        old_color = QColor(original_image.pixel(i,j));
        r = old_color.red() + delta;
        g = old_color.green() + delta;
        b = old_color.blue() + delta;

        r = qBound(0,r,255);
        g = qBound(0,g,255);
        b = qBound(0,b,255);

        editing.setPixel(i,j,qRgb(r,g,b));
      }
    }
  }
  QPixmap output;
  output = output.fromImage(editing);
  return output;
}

// TODO: Comment
QPixmap SpriteView::setColor(int deltared, int deltablue,
                             int deltagreen, QPixmap original)
{
  QImage original_image = original.toImage();
  QImage editing = original.toImage();
  QColor old_color;
  int r,g,b;

  for(int i=0; i<editing.width(); i++)
  {
    for(int j=0; j<editing.height(); j++)
    {
      if(qAlpha(original_image.pixel(i,j)) > 0)
      {
        old_color = QColor(original_image.pixel(i,j));
        r = old_color.red()+deltared-255;
        g = old_color.green()+deltablue-255;
        b = old_color.blue()+deltagreen-255;

        r = qBound(0,r,255);
        g = qBound(0,g,255);
        b = qBound(0,b,255);

        editing.setPixel(i,j,qRgb(r,g,b));
      }
    }
  }
  QPixmap output;
  output = output.fromImage(editing);
  return output;
}
