/*******************************************************************************
 * Class Name: SpriteToolbox
 * Date Created: November 14, 2013
 * Inheritance: QWidget
 * Description: The side toolbar that gives access to sprites.
 ******************************************************************************/
#ifndef SPRITETOOLBOX_H
#define SPRITETOOLBOX_H

#include <QWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QPainter>
#include <QDebug>
#include <QVector>
#include <qmath.h>
#include <QHBoxLayout>
#include "SpriteChoice.h"
#include "EditorSprite.h"

class SpriteToolbox : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  SpriteToolbox(QWidget* parent = 0);

  /* Destructor function */
  ~SpriteToolbox();

public slots:
  /* Opens the file selection dialog */
  void openDialog();

  /* Deselects all sprites except the chosen one */
  void deselectOthers(int);

  /* Picks a sprite path for map operations */
  void setSprite(QString);

protected:
  /* Sets up the painting of all selected files */
  void paintEvent(QPaintEvent *);
private:
  /* The button that opens the selection dialog */
  QPushButton* directory;

  /* The selection dialog */
  QFileDialog* select_files;

  /* The vector for storing sprite choices */
  QVector<SpriteChoice* > sprites;

  /* Currently selected Sprite path */
  QString path;
signals:

  /* Emits to parent to send an image path */
  void pathOfImage(QString);
  /* Sends up the edited sprite */
  void sendUpEditorSprite(EditorSprite* e);
};

#endif // SPRITETOOLBOX_H
