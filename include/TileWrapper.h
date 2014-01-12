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
#include <QGraphicsItem>
#include <QList>
#include <QGraphicsSceneHoverEvent>
#include "EditorSpriteToolbox.h"
#include "EditorEnumDb.h"
#include "Game/Map/Tile.h"

class TileWrapper : public QGraphicsItem
{
  //Q_OBJECT
public:
  /* Constructor Function */
  TileWrapper(int x = 0, int y = 0, int z = 0);

  /* Destructor Function */
  ~TileWrapper();

  QRectF boundingRect() const;

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget);

public:
  void setBase(bool);
  void setEnhancer(bool);
  void setLower1(bool);
  void setLower2(bool);
  void setLower3(bool);
  void setLower4(bool);
  void setLower5(bool);
  void setUpper1(bool);
  void setUpper2(bool);
  void setUpper3(bool);
  void setUpper4(bool);
  void setUpper5(bool);
  void setGrid(bool);
  void setToolbox(EditorSpriteToolbox* tool);
  void editBase(EditorSprite* selection);
  void editEnhancer(EditorSprite* selection);
  void editLower(EditorSprite* selection, int position = 0);
  void editUpper(EditorSprite* selection, int position = 0);
  EditorEnumDb::Layer active_layer;
protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *);
  void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
private:
  bool base,enhancer;
  bool lower1,lower2,lower3,lower4,lower5;
  bool upper1,upper2,upper3,upper4,upper5;
  bool grid;
  Tile* tile;
  EditorSpriteToolbox* toolbox;
  EditorSprite* base_layer;
  EditorSprite* enhancer_layer;
  QList<EditorSprite*> lower_layers;
  QList<EditorSprite*> upper_layers;
  int xpos, ypos, zpos;
//signals:
};

#endif // TILEWRAPPER_H
