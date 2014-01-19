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
  editor_sprite_list = new QListWidget(this);
  editor_sprite_list->show();
  editor_sprite_list->setFixedSize(288,512);
  connect(editor_sprite_list,SIGNAL(currentRowChanged(int)),
          this,SLOT(setCurrent(int)));
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
  QTransform temp;
  QTransform transformation = temp.rotate(current->getQuickRotation());
  painter.drawImage(bound,current->getImage(0).transformed(transformation));
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
  if(e->button() & Qt::LeftButton)
  {
    frames = new QDialog(this);
    QHBoxLayout* framelayout = new QHBoxLayout(frames);
    QVector<QLabel*> labels;
    for(int i=0; i<current->frameCount(); i++)
    {
      labels.push_back(new QLabel());
      labels.last()->setPixmap(QPixmap(current->getPath(i)));
      framelayout->addWidget(labels.last());
    }
    frames->setLayout(framelayout);
    frames->setModal(false);
    frames->setWindowTitle(current->getName());
    frames->exec();
  }
  else if(e->button() & Qt::RightButton)
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
  update();
}
