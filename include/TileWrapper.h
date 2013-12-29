/*******************************************************************************
 * Class Name: TileWrapper
 * Date Created: December 26, 2013
 * Inheritance: QWidget
 * Description: A tile representation on the mapeditor
 ******************************************************************************/
#ifndef TILEWRAPPER_H
#define TILEWRAPPER_H

#include <QWidget>
#include <QPainter>
#include <QList>
#include "EditorSpriteToolbox.h"
#include "Game/Map/Tile.h"

class TileWrapper : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  TileWrapper(QWidget* parent = 0);

  /* Destructor Function */
  ~TileWrapper();
public slots:
  void setBase(bool);
  void setToolbox(EditorSpriteToolbox* tool);
  void editBase(EditorSprite* selection);
  void editEnhancer(EditorSprite* selection);
  void editLower(EditorSprite* selection, int position = 0);
  void editUpper(EditorSprite* selection, int position = 0);
protected:
  void paintEvent(QPaintEvent *);
  void mousePressEvent(QMouseEvent *);
private:
  bool base,enhancer;
  bool lower1,lower2,lower3,lower4,lower5;
  bool upper1,upper2,upper3,upper4,upper5;
  Tile* tile;
  EditorSpriteToolbox* toolbox;
  EditorSprite* base_layer;
  EditorSprite* enhancer_layer;
  QList<EditorSprite*> lower_layers;
  QList<EditorSprite*> upper_layers;
signals:
};

#endif // TILEWRAPPER_H
