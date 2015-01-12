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
  bottom_view = new QWidget(this);
  bottom_view->setMinimumHeight(150);
  editor_sprite_list = new EditorSpriteList(this);
  connect(editor_sprite_list,SIGNAL(currentRowChanged(int)),
          this,SLOT(setCurrent(int)));
  connect(editor_sprite_list,SIGNAL(updateSprites()),this,SLOT(refreshList()));
  nextID = 0;

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
    edit_sprite = new SpriteDialog(this, current, current->getPath(0),
                                   0, false);

    connect(edit_sprite, SIGNAL(ok()), this, SLOT(refreshList()));
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

  /* Adds the item to the visible list */
  editor_sprite_list->addItem("temp");
  editor_sprite_list->setCurrentRow(editor_sprite_list->count()-1);
  editor_sprite_list->setCurrentSprite(e);

  /* Increments the id tracker and updates view */
  nextID++;
  refreshList();
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
  {
    EditorSprite* e = editor_sprites[i];

    QString name_str = QString::number(e->getId()) + ": " + e->getName();
    if(nextID < 10)
      name_str = "0" + name_str;
    if(nextID < 100)
      name_str = "0" + name_str;
    editor_sprite_list->item(i)->setText(name_str);
  }
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

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
