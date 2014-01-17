/*******************************************************************************
 * Class Name: SpriteToolbox
 * Date Created: November 14, 2013
 * Inheritance: QWidget
 * Description: The side toolbar that gives access to sprites.
 ******************************************************************************/
#ifndef EDITORSPRITETOOLBOX_H
#define EDITORSPRITETOOLBOX_H

#include <QWidget>
#include <QPainter>
#include <QVector>
#include <QVBoxLayout>
#include <QListWidget>
#include <QScrollArea>
#include <QMouseEvent>
#include <QDialog>
#include <QLabel>
#include "EditorSprite.h"

class EditorSpriteToolbox : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorSpriteToolbox(QWidget* parent = 0);

  /* Destructor function */
  ~EditorSpriteToolbox();
public slots:
  /* Adds an Editor Sprite to the toolbox */
  void addEditorSprite(EditorSprite* e);

  /* Returns the currently selected Editor Sprite */
  EditorSprite* getCurrent();

  /* Sets the current Editor Sprite to the given index */
  void setCurrent(int);

protected:
  /* Sets up the painting of all selected files */
  void paintEvent(QPaintEvent *);

  /* Views Frames */
  void mouseDoubleClickEvent(QMouseEvent *e);

private:
  /* The vector for storing sprite choices */
  QVector<EditorSprite* > editor_sprites;

  /* The next available ID */
  uint16_t nextID;

  /* The current Sprite selected in the toolbox */
  EditorSprite* current;

  /* The list widget that contains all the sprite names */
  QListWidget* editor_sprite_list;

  /* A widget for showing the current selected sprite frame sequence */
  QDialog* frames;

signals:

};
#endif // EDITORSPRITETOOLBOX_H
