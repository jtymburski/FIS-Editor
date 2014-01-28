/*******************************************************************************
 * Class Name: EditorSpriteToolbox
 * Date Created: November 24, 2013
 * Inheritance: QWidget
 * Description: This class handles the toolbox on the side of the application
 ******************************************************************************/
#include "EditorSpriteToolbox.h"
#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function - Set up toolbox
 *
 * Input: parent widget
 */
EditorSpriteToolbox::EditorSpriteToolbox(QWidget *parent) : QWidget(parent)
{
  current = new EditorSprite();
  edit_sprite = new SpriteCreationDialog(this);
  editor_sprite_list = new EditorSpriteList(this);
  editor_sprite_list->show();
  editor_sprite_list->setFixedSize(288,512);
  connect(editor_sprite_list,SIGNAL(currentRowChanged(int)),
          this,SLOT(setCurrent(int)));
  connect(editor_sprite_list,SIGNAL(updateSprites()),this,SLOT(refreshList()));
  nextID = 1;
}

/*
 * Description: Destructor function
 *
 * Input: none
 */
EditorSpriteToolbox::~EditorSpriteToolbox()
{
  //qDebug()<<"Removing Editor Sprite Toolbox";
  delete edit_sprite;
  for(int i=0; i<editor_sprites.size(); i++)
  {
    delete editor_sprites[i];
    editor_sprites[i] = NULL;
  }
  //delete current;
  current = NULL;
}


/*
 * Description: Transforms the given frame from the given sprite
 *
 * Input: Editor sprite and position (Default is zero)
 *
 * Output: Returns the transformed pixmap
 */
QPixmap EditorSpriteToolbox::transformPixmap(EditorSprite* pic, int pos)
{
  QTransform transform;

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
  QTransform trans = transform;
  transform = trans.rotate(pic->getFrameAngle(pos));
  QPixmap returnimage(pic->getPath(pos));
  return returnimage.transformed(transform);
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
void EditorSpriteToolbox::paintEvent(QPaintEvent *)
{
  QPainter painter(this);
  QRect bound(12,520,64,64);
  QRect border(11,519,66,66);
  painter.setPen(QPen(QBrush(Qt::black),2));
  painter.drawPixmap(bound,transformPixmap(current));
  painter.drawRect(border);
  painter.setFont(QFont("helvetica",14,QFont::Bold));
  painter.drawText(80,536,current->getName());
  painter.setFont(QFont("arial",8));
  QString framecount = "Framecount : ";
  painter.drawText(85,554,framecount.append(QString::
                                            number(current->frameCount())));
}

/*
 * Description: The mouse double click event brings up a display that contains
 * all of the frames in the current sprite selected, right now this was just for
 * debugging reasons, however it may have a use later on
 *
 * Inputs: Mouse event
 */
void EditorSpriteToolbox::mouseDoubleClickEvent(QMouseEvent *e)
{

  if(e->button() & Qt::RightButton)
  {
      edit_sprite = new SpriteCreationDialog(this,current,
                                             current->getPath(0),
                                             0,false);

      connect(edit_sprite,SIGNAL(ok()),this,SLOT(refreshList()));
      edit_sprite->show();
  }
}


/*
 * Description: Adds an editor sprite to the toolbox
 *
 * Inputs: The editor sprite to add
 */
void EditorSpriteToolbox::addEditorSprite(EditorSprite *e)
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

/* Description: Refreshes the Editor Sprite list
 *
 */
void EditorSpriteToolbox::refreshList()
{
  for(int i=0; i<editor_sprites.size(); i++)
    editor_sprite_list->item(i)->setText(editor_sprites[i]->getName());
  update();
}


/*
 * Description: Gets the current editor sprite
 *
 * Output: The current editor sprite
 */
EditorSprite* EditorSpriteToolbox::getCurrent()
{
  return current;
}

/*
 * Description: Sets the index of the current editor sprite
 *
 * Inputs: Index
 */
void EditorSpriteToolbox::setCurrent(int index)
{
  current = editor_sprites[index];
  editor_sprite_list->setCurrentSprite(current);
  update();
}
