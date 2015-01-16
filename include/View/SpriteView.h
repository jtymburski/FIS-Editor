/*******************************************************************************
 * Class Name: SpriteToolbox
 * Date Created: November 14, 2013
 * Inheritance: QWidget
 * Description: The side toolbar that gives access to sprites.
 ******************************************************************************/
#ifndef SpriteView_H
#define SpriteView_H

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
  SpriteView(QWidget* parent = 0);

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

protected:
  /* Sets up the painting of all selected files */
  void paintEvent(QPaintEvent *);

  /* Views Frames */
  void mouseDoubleClickEvent(QMouseEvent *e);

public slots:

  /* Adds an Editor Sprite to the toolbox */
  void addEditorSprite(EditorSprite* e);

  /* Opens the sprite editing dialog */
  void editSprite();

  /* Updates the sprite list */
  void updateList();

  /* Views the frame sequence */
  void viewFrameSequence();

signals:
  /* Sends up the Editor Sprite */
  void sendUpEditorSprite(EditorSprite* sprite);

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
#endif // SpriteView_H
