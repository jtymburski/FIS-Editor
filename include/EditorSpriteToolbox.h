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
  void addEditorSprite(EditorSprite* e);
  EditorSprite* getCurrent();

protected:
  /* Sets up the painting of all selected files */
  void paintEvent(QPaintEvent *);
private:
  /* The vector for storing sprite choices */
  QVector<EditorSprite* > editor_sprites;

  uint16_t nextID;
  EditorSprite* current;

signals:

};
#endif // EDITORSPRITETOOLBOX_H
