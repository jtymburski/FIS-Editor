/*******************************************************************************
 * Class Name: EditorSpriteList
 * Date Created: January 19, 2014
 * Inheritance: QListWidget
 * Description: The list of editor sprites
 ******************************************************************************/
#include "View/EditorSpriteList.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function
 *
 * Input: Parent Widget
 */
EditorSpriteList::EditorSpriteList(QWidget* parent, EditorSprite* c)
  : QListWidget(parent)
{
  currentsprite = c;
  rightclick_menu = new QMenu("Sprite Edit",this);
  QAction* edit_sprite = new QAction("Edit Sprite",rightclick_menu);
  connect(edit_sprite,SIGNAL(triggered()),this,SLOT(editSprite()));
  rightclick_menu->addAction(edit_sprite);
  QAction* view_frames = new QAction("View Frames",rightclick_menu);
  connect(view_frames,SIGNAL(triggered()),this,SLOT(viewFrameSequence()));
  rightclick_menu->addAction(view_frames);
  spriteeditordialog = NULL;
  rightclick_menu->hide();
}

/*
 * Description: Destructor function
 *
 * Input: none
 */
EditorSpriteList::~EditorSpriteList()
{
  delete rightclick_menu;
}

void EditorSpriteList::setCurrentSprite(EditorSprite *e)
{
  currentsprite = e;
}

/*
 * Description: The mouse event for the right click menu
 *
 * Inputs: Mouse event
 */
void EditorSpriteList::mousePressEvent(QMouseEvent *event)
{
  QListWidget::mousePressEvent(event);
  if(event->buttons() & Qt::RightButton)
  {
    QListWidgetItem* current = itemAt(event->pos());
    if(current != NULL)
      rightclick_menu->exec(QCursor::pos());
  }
}


/*
 * Description: The mouse event for double clicking
 *
 * Inputs: Mouse event
 */
void EditorSpriteList::mouseDoubleClickEvent(QMouseEvent *event)
{
  QListWidget::mouseDoubleClickEvent(event);
  if(event->buttons() & Qt::LeftButton)
  {
    QListWidgetItem* current = itemAt(event->pos());
    if(current != NULL)
      editSprite();
  }
}
void EditorSpriteList::viewFrameSequence()
{
  QDialog* frames = new QDialog(this);
  QHBoxLayout* framelayout = new QHBoxLayout(frames);
  QVector<QLabel*> labels;
  for(int i=0; i<currentsprite->frameCount(); i++)
  {
    labels.push_back(new QLabel());
    QPixmap temp(currentsprite->getPath(i));
    QTransform trans;
    QTransform transformation =
        trans.rotate(currentsprite->getQuickRotation());
    labels.last()->setPixmap(temp.transformed(transformation));
    framelayout->addWidget(labels.last());
  }
  frames->setLayout(framelayout);
  frames->setModal(false);
  frames->setWindowTitle(currentsprite->getName());
  frames->exec();
}

void EditorSpriteList::editSprite()
{
  spriteeditordialog = new SpriteDialog(this,currentsprite,
                                         currentsprite->getPath(0),
                                         0,false);
  connect(spriteeditordialog,SIGNAL(ok()),this,SIGNAL(updateSprites()));
  spriteeditordialog->show();
}

