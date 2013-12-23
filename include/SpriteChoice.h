/*******************************************************************************
 * Class Name: SpriteChoice
 * Date Created: November 11, 2013
 * Inheritance: QWidget
 * Description: A sprite representation that appears in the toolbox
 ******************************************************************************/
#ifndef SPRITECHOICE_H
#define SPRITECHOICE_H

#include <QImage>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QMenu>
#include "EditorSprite.h"
#include "SpriteCreationDialog.h"
#include "EditorEnumDb.h"

class SpriteChoice : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  SpriteChoice(QWidget* parent = 0, QString path = 0, int id = 0);

  /* Destructor function */
  ~SpriteChoice();

public slots:
  /* Loads the image with a valid given path */
  void loadSprite(QString path);

  /* Deselects the sprite choice manually */
  void deselect();

  /* Attempts to make a sprite from this image, which is passed up to the
   * main application and added to the sprite menu */
  EditorSprite* makeSprite();

protected:
  /* Paints the sprite in a bounding box */
  void paintEvent(QPaintEvent *);

  /* Mouse Enter, Leave and click events */
  void enterEvent(QEvent *);
  void leaveEvent(QEvent *);
  void mousePressEvent(QMouseEvent *);

private:
  /* A Pointer to the image stored */
  QImage* pic;

  /* Right click menu for sprite */
  QMenu* rightclick_menu;
  QAction *spriteify_action;

  /* The view mode for the border */
  EditorEnumDb::Border mode;

  /* The path of the image stored */
  QString path;

  /* Id number */
  int id_number;

  /* Next available id */
  uint16_t next_id;

  /* Create a Sprite dialog */
  SpriteCreationDialog* creation_dialog;

signals:
  /* Emits to parent when selected to deselect all others */
  void chosen(int);

  /* Emits to parent to send an image path */
  void pathOfImage(QString);
};
#endif // SPRITECHOICE_H
