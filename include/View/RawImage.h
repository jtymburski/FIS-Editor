/*******************************************************************************
 * Class Name: RawImage
 * Date Created: November 11, 2013
 * Inheritance: QWidget
 * Description: A sprite representation that appears in the toolbox
 ******************************************************************************/
#ifndef RAWIMAGE_H
#define RAWIMAGE_H

#include <QImage>
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QMenu>
#include "Database/EditorSprite.h"
#include "Dialog/SpriteDialog.h"
#include "EditorEnumDb.h"

class RawImage : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  RawImage(QWidget* parent = 0, QString path = 0,
               int id = 0, int followers = 0);

  /* Destructor function */
  ~RawImage();

private:
  /* The number of same images that trail this one */
  int followers;

  /* A Pointer to the image stored */
  QImage* pic;

  /* Right click menu for sprite */
  QMenu* rightclick_menu;
  QAction* spriteify_action;

  /* The view mode for the border */
  EditorEnumDb::Border mode;

  /* The path of the image stored */
  QString path;

  /* Id number */
  int id_number;

  /* Create a Sprite dialog */
  SpriteDialog* creation_dialog;

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Paints the sprite in a bounding box */
  void paintEvent(QPaintEvent *);

  /* Mouse Enter, Leave and Click events */
  void enterEvent(QEvent *);
  void leaveEvent(QEvent *);
  void mousePressEvent(QMouseEvent *);
  void mouseDoubleClickEvent(QMouseEvent *);

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Emits to parent when selected to deselect all others */
  void chosen(int);

  /* Emits to parent to send an image path */
  void pathOfImage(QString);

  /* Passes up the created Editor Sprite */
  void sendUpEditorSprite(EditorSprite* e);

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* Loads the image with a valid given path */
  void loadSprite(QString path);

  /* Deselects the sprite choice manually */
  void deselect();

  /* Attempts to make a sprite from this image, which is passed up to the
   * main application and added to the sprite menu */
  void makeSprite();
};
#endif // RAWIMAGE_H
