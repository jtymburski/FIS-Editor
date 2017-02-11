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
 * Inputs: QWidget* parent - the parent widget
 */
SpriteView::SpriteView(QWidget *parent) : QWidget(parent)
{
  QVBoxLayout* layout = new QVBoxLayout(this);

  editor_map = NULL;
  sprite_dialog = NULL;
  bottom_view = new QWidget(this);
  bottom_view->setMinimumHeight(150);
  editor_sprite_list = new SpriteViewList(this);
  connect(editor_sprite_list,SIGNAL(currentRowChanged(int)),
          this,SLOT(updateSelected(int)));
  connect(editor_sprite_list, SIGNAL(editSprite()), this, SLOT(editSprite()));
  connect(editor_sprite_list, SIGNAL(viewFrameSequence()),
          this, SLOT(viewFrameSequence()));

  layout->addWidget(editor_sprite_list);
  layout->addWidget(bottom_view);
  setLayout(layout);
}

/*
 * Description: Destructor function
 */
SpriteView::~SpriteView()
{

}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: The paint event for the toolbox, this mainly sets up the
 *              positions for all of the sprites that have been selected and
 *              added to the toolbar
 *
 * Inputs: none
 * Output: none
 */
void SpriteView::paintEvent(QPaintEvent *)
{
  int l = 64;

  if(bottom_view != NULL)
  {
    QPainter painter(this);
    painter.setBackgroundMode(Qt::TransparentMode);

    /* Set bounds */
    QRect border(bottom_view->x(), bottom_view->y(), l + 1, l + 1);

    /* Paint the sprite */
    EditorSprite* current = getSelected();
    if(current != NULL)
      current->paint(&painter, bottom_view->x() + 1, bottom_view->y() + 1, l, l);

    /* Paint the border */
    painter.setPen(QPen(QBrush(QColor(168, 168, 168)), 1));
    painter.setOpacity(1.0);
    painter.drawRect(border);

    /* Name and frame count text */
    if(current != NULL)
    {
      painter.setPen(QPen(QBrush(Qt::black), 1));
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
 *              all of the frames in the current sprite selected, right now
 *              this was just for debugging reasons, however it may have a use
 *              later on.
 *
 * Inputs: QMouseEvent* e - the mouse double click event
 * Output: none
 */
void SpriteView::mouseDoubleClickEvent(QMouseEvent *e)
{

  if(e->button() & Qt::RightButton)
  {
    editSprite();
  }
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

/*
 * Description: Adds an editor sprite to the toolbox
 *
 * Inputs: EditorSprite* e - The editor sprite to add
 * Output: none
 */
void SpriteView::addEditorSprite(EditorSprite *e)
{
  if(editor_map != NULL)
  {
    /* Sets the id and puts the sprite in the library */
    e->setID(editor_map->getNextSpriteID());
    int index = editor_map->setSprite(e);

    /* Increments the id tracker and updates view */
    updateList();
    editor_sprite_list->setCurrentRow(index);
  }
}

/*
 * Description: Edits the hover tile sprite at the designated layer.
 *
 * Inputs: EditorEnumDb::Layer layer - the sprite layer
 * Output: none
 */
void SpriteView::editHoverSprite(EditorEnumDb::Layer layer)
{
  if(editor_map != NULL)
  {
    /* Check if hover tile is valid */
    EditorTile* t = editor_map->getHoverInfo()->hover_tile;
    if(t != NULL)
    {
      /* Check if sprite at index is valid */
      EditorSprite* sprite = t->getSprite(layer);
      if(sprite != NULL)
        editSprite(sprite);
    }
  }
}

/*
 * Description: Opens the sprite editing dialog.
 *
 * Inputs: EditorSprite* edit - a sprite to edit. If NULL, use current (default)
 * Output: none
 */
void SpriteView::editSprite(EditorSprite* edit)
{
  EditorSprite* current = getSelected();
  if(edit != NULL)
    current = edit;

  /* Delete the old dialog */
  if(sprite_dialog != NULL)
    delete sprite_dialog;

  /* Create the new dialog */
  sprite_dialog = new SpriteDialog(this, current, "",0, false);
  connect(sprite_dialog, SIGNAL(ok()), this, SLOT(updateList()));
  emit soundFillTrigger(EditorEnumDb::SPRITE_VIEW);
  sprite_dialog->show();
}

/*
 * Description: Function call to add sound list to sprite dialog, if set
 *
 * Inputs: QList<QString> sound_list - list of all sound resources available
 * Output: none
 */
void SpriteView::soundFill(QList<QString> sound_list)
{
  if(sprite_dialog != nullptr)
    sprite_dialog->soundFill(sound_list);
}

/*
 * Description: Refreshes the Editor Sprite list
 *
 * Inputs: none
 * Output: none
 */
void SpriteView::updateList()
{
  int index = editor_sprite_list->currentRow();

  editor_sprite_list->clear();
  if(editor_map != NULL)
  {
    for(int i = 0; i < editor_map->getSpriteCount(); i++)
    {
      EditorSprite* sprite = editor_map->getSpriteByIndex(i);

      QListWidgetItem* item = new QListWidgetItem();

      item->setText(sprite->getNameList());
      item->setIcon(sprite->getPixmap(0, 32, 32));
      editor_sprite_list->addItem(item);
    }

    editor_map->updateAll();
  }

  editor_sprite_list->setCurrentRow(index);
  update();
}

/*
 * Description: Updates the selected sprite at the current row.
 *
 * Inputs: int current_row - the row to update
 * Output: none
 */
void SpriteView::updateSelected(int current_row)
{
  if(editor_map != NULL)
  {
    editor_map->setCurrentSprite(current_row);
    EditorEnumDb::Layer layer = editor_map->getHoverInfo()->active_layer;
    if(layer != EditorEnumDb::BASE && layer != EditorEnumDb::ENHANCER &&
       layer != EditorEnumDb::LOWER1 && layer != EditorEnumDb::LOWER2 &&
       layer != EditorEnumDb::LOWER3 && layer != EditorEnumDb::LOWER4 &&
       layer != EditorEnumDb::LOWER5 && layer != EditorEnumDb::UPPER1 &&
       layer != EditorEnumDb::UPPER2 && layer != EditorEnumDb::UPPER3 &&
       layer != EditorEnumDb::UPPER4 && layer != EditorEnumDb::UPPER5)
    {
      emit changeLayer(EditorEnumDb::LOWER1);
    }
  }

  update();
}

/*
 * Description: View the frame sequence of the current sprite.
 *
 * Inputs: none
 * Output: none
 */
void SpriteView::viewFrameSequence()
{
  EditorSprite* current_sprite = getSelected();
  int size = EditorHelpers::getTileSize();

  if(current_sprite != NULL)
  {
    QDialog* frames = new QDialog(this);
    QHBoxLayout* framelayout = new QHBoxLayout(frames);
    QVector<QLabel*> labels;

    /* Fill with the frames */
    for(int i = 0; i < current_sprite->frameCount(); i++)
    {
      labels.push_back(new QLabel());
      labels.last()->setFixedSize(size, size);
      labels.last()->setPixmap(current_sprite->getPixmap(i, size, size));
      framelayout->addWidget(labels.last());
    }

    /* Finalize dialog */
    frames->setLayout(framelayout);
    frames->setModal(false);
    frames->setWindowTitle(current_sprite->getName());
    frames->exec();
  }
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Deletes the selected sprite in the base list. Triggered by
 *              parent.
 *
 * Inputs: none
 * Output: bool - returns true if sprite deleted.
 */
bool SpriteView::deleteSprite()
{
  EditorSprite* selected = getSelected();
  bool success = false;

  /* If valid, proceed deletion */
  if(selected != NULL)
  {
    /* Create warning about deleting */
    QMessageBox msg_box;
    msg_box.setText("Deleting sprite " + selected->getNameList());
    msg_box.setInformativeText(
         "This will delete ALL instances of this sprite on map. Are you sure?");
    msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    if(msg_box.exec() == QMessageBox::Yes)
    {
      /* Proceed with deletion */
      int index = editor_sprite_list->currentRow();
      success = editor_map->unsetSpriteByIndex(index);

      /* If successfull, reset list */
      if(success)
      {
        updateList();
        if(index >= editor_sprite_list->count())
          editor_sprite_list->setCurrentRow(editor_sprite_list->count() - 1);
      }
    }
  }

  return success;
}

/*
 * Description: Duplicates the selected sprite in the base list. Triggered by
 *              parent.
 *
 * Inputs: none
 * Output: bool - returns true if sprite was duplicated.
 */
bool SpriteView::duplicateSprite()
{
  EditorSprite* selected = getSelected();

  /* If valid, proceed duplication */
  if(selected != NULL)
  {
    EditorSprite* new_sprite = new EditorSprite(*selected);
    addEditorSprite(new_sprite);
    return true;
  }

  return false;
}

/*
 * Description: Returns which sprite is selected in the base list.
 *
 * Inputs: none
 * Output: EditorThing* - selected sprite. NULL if none selected
 */
EditorSprite* SpriteView::getSelected()
{
  EditorSprite* sprite = NULL;

  /* Check the validity */
  if(editor_map != NULL)
    sprite = editor_map->getSpriteByIndex(editor_map->getCurrentSpriteIndex());

  return sprite;
}

/*
 * Description: Returns the editor map, which contains all things and instances.
 *
 * Inputs: none
 * Output: EditorMap* - pointer to the editor map. NULL if none set.
 */
EditorMap* SpriteView::getEditorMap()
{
  return editor_map;
}

/*
 * Description: Imports a new sprite into the base list. Triggered by
 *              parent.
 *
 * Inputs: none
 * Output: none
 */
void SpriteView::importSprite()
{
  // TODO: Future
  QMessageBox::information(this, "Notification",
                           "Coming soon to a production near you!");
}

/*
 * Description: Creates a new sprite into the base list. Triggered by
 *              parent. Proceeds to call edit on the new sprite and opens
 *              SpriteDialog.
 *
 * Inputs: none
 * Output: none
 */
void SpriteView::newSprite()
{
  EditorSprite* new_sprite = new EditorSprite();
  addEditorSprite(new_sprite);
  editSprite();
}

/*
 * Description: Sets the control editor map. This contains the things, sprites
 *              and all data relevant to display.
 *
 * Inputs: EditorMap* map - the map to use for control
 * Output: none
 */
void SpriteView::setEditorMap(EditorMap* map)
{
  /* If existing editor map is not NULL, undo */
  if(editor_map != NULL)
  {
    disconnect(editor_map, SIGNAL(spriteIndexChange(int)),
               this, SLOT(updateSelected(int)));
  }

  editor_map = map;

  if(editor_map != NULL)
  {
    connect(editor_map, SIGNAL(spriteIndexChange(int)),
            this, SLOT(updateSelected(int)));
  }

  /* Finally, update list */
  updateList();
}
