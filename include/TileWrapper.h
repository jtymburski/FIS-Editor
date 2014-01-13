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

public:
  /* Necessary function for returning the bounding rectangle */
  QRectF boundingRect() const;

  /* Painting function for Tile Wrapper */
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
             QWidget *widget);

public:
  /* Sets the various layer visibilities */
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

  /* Sets the grid visibility */
  void setGrid(bool);

  /* Sets the toolbox that the tile gets the current Editor Sprite from */
  void setToolbox(EditorSpriteToolbox* tool);

  /* Edits the various layers */
  void editBase(EditorSprite* selection);
  void editEnhancer(EditorSprite* selection);
  void editLower(EditorSprite* selection, int position = 0);
  void editUpper(EditorSprite* selection, int position = 0);

  /* Variable for the currently active layer */
  EditorEnumDb::Layer active_layer;
protected:
  /* Mouse press and enter events for placement of sprites onto tile layers */
  void mousePressEvent(QGraphicsSceneMouseEvent *);
  void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
private:
  /* Toggling visibility variables */
  bool base,enhancer;
  bool lower1,lower2,lower3,lower4,lower5;
  bool upper1,upper2,upper3,upper4,upper5;
  bool grid;

  /* The Tile that will be placed into Univursa.exe */
  Tile* tile;

  /* A Pointer to a toolbox containing the current Editor Sprite */
  EditorSpriteToolbox* toolbox;

  /*Editor Sprite layers */
  EditorSprite* base_layer;
  EditorSprite* enhancer_layer;
  QList<EditorSprite*> lower_layers;
  QList<EditorSprite*> upper_layers;

  /* Positions and depths for the tiles on the map grid */
  int xpos, ypos, zpos;
};

#endif // TILEWRAPPER_H