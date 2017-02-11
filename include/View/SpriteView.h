/*******************************************************************************
 * Class Name: SpriteView
 * Date Created: November 14, 2013
 * Inheritance: QWidget
 * Description: The side toolbar that gives access to sprites.
 ******************************************************************************/
#ifndef SPRITEVIEW_H
#define SPRITEVIEW_H

#include <QDialog>
#include <QGroupBox>
#include <QLabel>
#include <QListWidget>
#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>

#include "Database/EditorMap.h"
#include "Dialog/SpriteDialog.h"
#include "EditorHelpers.h"
#include "View/SpriteViewList.h"

class SpriteView : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  SpriteView(QWidget* parent = NULL);

  /* Destructor function */
  ~SpriteView();

private:
  /* Bottom view widget to act as a placeholder for description */
  QWidget* bottom_view;

  /* The current editing map */
  EditorMap* editor_map;

  /* The list widget that contains all the sprite names */
  SpriteViewList* editor_sprite_list;

  /* The sprite dialog for editing */
  SpriteDialog* sprite_dialog;

  /* Sprite Images */
  QVector<Sprite*> sprite_images;

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Sets up the painting of all selected files */
  void paintEvent(QPaintEvent *);

  /* Views Frames */
  void mouseDoubleClickEvent(QMouseEvent *e);

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Change layer and pen trigger */
  void changeLayer(EditorEnumDb::Layer layer);

  /* Sends up the Editor Sprite */
  void sendUpEditorSprite(EditorSprite* sprite);

  /* Trigger handles with sounds */
  void soundFillTrigger(EditorEnumDb::MapObjectMode mode);

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* Adds an Editor Sprite to the toolbox */
  void addEditorSprite(EditorSprite* e);

  /* Edit the hover sprite at the layer */
  void editHoverSprite(EditorEnumDb::Layer layer);

  /* Opens the sprite editing dialog */
  void editSprite(EditorSprite* edit = NULL);

  /* Slot fill and triggers for sound list */
  void soundFill(QList<QString> sound_list);

  /* Updates the sprite list */
  void updateList();

  /* Update selected sprite */
  void updateSelected(int current_row);

  /* Views the frame sequence */
  void viewFrameSequence();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Deletes the selected sprite */
  bool deleteSprite();

  /* Duplicates the selected sprite */
  bool duplicateSprite();

  /* Get current sprite */
  EditorSprite* getSelected();

  /* Gets the editor map */
  EditorMap* getEditorMap();

  /* Imports a sprite from another file */
  void importSprite();

  /* Creates a new blank sprite */
  void newSprite();

  /* Sets the editor map, which contains the data needed */
  void setEditorMap(EditorMap* map);
};
#endif // SPRITEVIEW_H
