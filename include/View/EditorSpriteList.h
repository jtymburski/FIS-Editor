/*******************************************************************************
 * Class Name: EditorSpriteList
 * Date Created: January 19, 2014
 * Inheritance: QListWidget
 * Description: The list of editor sprites
 ******************************************************************************/
#ifndef EDITORSPRITELIST_H
#define EDITORSPRITELIST_H

#include <QListWidget>
#include <QMouseEvent>
#include <QDialog>
#include <QHBoxLayout>
#include <QVector>
#include <QLabel>
#include <QDebug>
#include <QMenu>
#include "Database/EditorSprite.h"
#include "Dialog/SpriteDialog.h"

class EditorSpriteList : public QListWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorSpriteList(QWidget* parent = 0, EditorSprite* c = 0);

  /* Destructor Function */
  ~EditorSpriteList();
public slots:
  /* Sets the current sprite */
  void setCurrentSprite(EditorSprite* e);

  /* Views the frame sequence */
  void viewFrameSequence();

  /* Opens the sprite editing dialog */
  void editSprite();

protected:
  /* Right click menu */
  void mousePressEvent(QMouseEvent *event);
  /* Double click */
  void mouseDoubleClickEvent(QMouseEvent *event);
private:
  /* Right click menu */
  QMenu* rightclick_menu;

  /* Current Editor Sprite */
  EditorSprite* currentsprite;

  /* Sprite Creation dialog */
  SpriteDialog* spriteeditordialog;

signals:
  /* Updates the current sprite after editing it */
  void updateSprites();
};

#endif // EDITORSPRITELIST_H
