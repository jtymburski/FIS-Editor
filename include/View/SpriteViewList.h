/*******************************************************************************
 * Class Name: SpriteViewList
 * Date Created: January 19, 2014
 * Inheritance: QListWidget
 * Description: The list of editor sprites
 ******************************************************************************/
#ifndef SpriteViewList_H
#define SpriteViewList_H

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

class SpriteViewList : public QListWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  SpriteViewList(QWidget* parent = NULL);

  /* Destructor Function */
  ~SpriteViewList();

private:
  /* Right click menu */
  QMenu* rightclick_menu;

protected:
  /* Double click */
  void mouseDoubleClickEvent(QMouseEvent *event);

  /* Right click menu */
  void mousePressEvent(QMouseEvent *event);

signals:
  /* Edit sprite signal */
  void editSprite();

  /* View frame sequence signal */
  void viewFrameSequence();

public slots:
};

#endif // SpriteViewList_H
