/*******************************************************************************
 * Class Name: SpriteViewList
 * Date Created: January 19, 2014
 * Inheritance: QListWidget
 * Description: The list of editor sprites
 ******************************************************************************/
#include "View/SpriteViewList.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function
 *
 * Input: Parent Widget
 */
SpriteViewList::SpriteViewList(QWidget* parent)
              : QListWidget(parent)
{
  /* Right click menu control */
  rightclick_menu = new QMenu("Sprite Edit", this);
  QAction* edit_sprite = new QAction("Edit Sprite", rightclick_menu);
  connect(edit_sprite, SIGNAL(triggered()), this, SIGNAL(editSprite()));
  rightclick_menu->addAction(edit_sprite);
  QAction* view_frames = new QAction("View Frames", rightclick_menu);
  connect(view_frames, SIGNAL(triggered()), this, SIGNAL(viewFrameSequence()));
  rightclick_menu->addAction(view_frames);
  rightclick_menu->hide();
}

/*
 * Description: Destructor function
 *
 * Input: none
 */
SpriteViewList::~SpriteViewList()
{
  delete rightclick_menu;
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: The mouse event for double clicking
 *
 * Inputs: Mouse event
 */
void SpriteViewList::mouseDoubleClickEvent(QMouseEvent *event)
{
  QListWidget::mouseDoubleClickEvent(event);
  if(event->buttons() & Qt::LeftButton)
  {
    QListWidgetItem* current = itemAt(event->pos());
    if(current != NULL)
      emit editSprite();
  }
}

/*
 * Description: The mouse event for the right click menu
 *
 * Inputs: Mouse event
 */
void SpriteViewList::mousePressEvent(QMouseEvent *event)
{
  QListWidget::mousePressEvent(event);
  if(event->buttons() & Qt::RightButton)
  {
    QListWidgetItem* current = itemAt(event->pos());
    if(current != NULL)
      rightclick_menu->exec(QCursor::pos());
  }
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
