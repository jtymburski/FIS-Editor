/*******************************************************************************
 * Class Name: EditorSprite
 * Date Created: November 24, 2013
 * Inheritance: QWidget
 * Description: A sprite representation that appears in the toolbox
 ******************************************************************************/
#ifndef EDITORSPRITE_H
#define EDITORSPRITE_H

#include <QWidget>
#include <QPainter>
#include "EditorEnumDb.h"

class EditorSprite : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorSprite();

  /* Destructor function */
  ~EditorSprite();

  /* Gets the sprite for alteration */
  Sprite* getSprite();

  /* Sets the sprite path */
  void setPath(QString p);

public slots:


protected:
  /* Painting event */
  void paintEvent(QPaintEvent *);

private:
  /* The view mode for the border */
  EditorEnumDb::Border mode;

  /* The actual Sprite for in game */
  Sprite* sprite;

  /* The path to the sprite */
  QString path;

signals:

};
#endif // IMAGECHOICE_H
