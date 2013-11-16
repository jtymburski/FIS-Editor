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
#include <QHBoxLayout>
#include "SpriteChoice.h"

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
};

#endif // SPRITETOOLBOX_H
