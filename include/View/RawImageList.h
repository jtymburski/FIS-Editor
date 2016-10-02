/*******************************************************************************
 * Class Name: RawImageList
 * Date Created: November 14, 2013
 * Inheritance: QWidget
 * Description: The side toolbar that gives access to sprites.
 ******************************************************************************/
#ifndef RAWIMAGELIST_H
#define RAWIMAGELIST_H

#include <QWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QPainter>
#include <QDebug>
#include <QVector>
#include <qmath.h>
#include <QHBoxLayout>
#include <QDir>
#include <QFileSystemModel>
#include "View/RawImage.h"
#include "Database/EditorSprite.h"

class RawImageList : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  RawImageList(QWidget* parent = 0, QFileSystemModel* module = 0);

  /* Destructor function */
  ~RawImageList();

private:
  /* The button that opens the selection dialog */
  QPushButton* directory;

  /* The selection dialog */
  QFileDialog* select_files;

  /* The vector for storing sprite choices */
  QVector<RawImage* > sprites;

  /* Currently selected Sprite path */
  QString path;

  /* Pointer to the directory selection dialog */
  QFileSystemModel* directory_module;

protected:
  /* Sets up the painting of all selected files */
  void paintEvent(QPaintEvent *);

public slots:
  /* Opens the file selection dialog */
  void openDialog();

  /* Inputs the files from the given directory */
  void switchDirectory(QModelIndex);

  /* Deselects all sprites except the chosen one */
  void deselectOthers(int);

  /* Picks a sprite path for map operations */
  void setSprite(QString);

signals:
  /* Emits to parent to send an image path */
  void pathOfImage(QString);
  /* Sends up the edited sprite */
  void sendUpEditorSprite(EditorSprite* e);
};

#endif // RAWIMAGELIST_H
