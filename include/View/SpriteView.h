/*******************************************************************************
 * Class Name: SpriteToolbox
 * Date Created: November 14, 2013
 * Inheritance: QWidget
 * Description: The side toolbar that gives access to sprites.
 ******************************************************************************/
#ifndef SpriteView_H
#define SpriteView_H

#include <QDialog>
#include <QLabel>
#include <QListWidget>
#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>
#include "Database/EditorSprite.h"
#include "View/EditorSpriteList.h"
#include "Dialog/SpriteDialog.h"

class SpriteView : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  SpriteView(QWidget* parent = 0);

  /* Destructor function */
  ~SpriteView();

private:
  /* Bottom view widget to act as a placeholder */
  QWidget* bottom_view;

  /* The current Sprite selected in the toolbox */
  EditorSprite* current;

  /* The sprite dialog for editing */
  SpriteDialog* edit_sprite;

  /* The list widget that contains all the sprite names */
  EditorSpriteList* editor_sprite_list;

  /* The vector for storing sprite choices */
  QVector<EditorSprite* > editor_sprites;

  /* A widget for showing the current selected sprite frame sequence */
  QDialog* frames;

  /* The next available ID */
  uint16_t nextID;

protected:
  /* Sets up the painting of all selected files */
  void paintEvent(QPaintEvent *);

  /* Views Frames */
  void mouseDoubleClickEvent(QMouseEvent *e);

public slots:
  /* Adds an Editor Sprite to the toolbox */
  void addEditorSprite(EditorSprite* e);

  /* Returns the currently selected Editor Sprite */
  EditorSprite* getCurrent();

  /* Updates the sprite list */
  void refreshList();

  /* Sets the current Editor Sprite to the given index */
  void setCurrent(int);

  /* Returns a transformed image */
  QPixmap transformPixmap(EditorSprite*,int pos = 0);

signals:
  /* Sends up the Editor Sprite */
  void sendUpEditorSprite(EditorSprite* sprite);

public:
  QPixmap setBrightness(int value, QPixmap original);
  QPixmap setColor(int red, int blue, int green, QPixmap original);
};
#endif // SpriteView_H
