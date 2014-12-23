/*******************************************************************************
 * Class Name: TileRender
 * Date Created: December 26, 2013
 * Inheritance: QWidget
 * Description: A tile representation on the mapeditor
 ******************************************************************************/
#ifndef TILERENDER_H
#define TILERENDER_H

#include <QWidget>
#include <QPainter>
#include <QGraphicsItem>
#include <QMenu>
#include <QAction>
#include <QList>
#include <QGraphicsSceneHoverEvent>
#include "View/SpriteView.h"
#include "EditorEnumDb.h"
#include "Game/Map/Tile.h"

class TileRender : public QGraphicsItem
{
  //Q_OBJECT
public:
  /* Constructor Function */
  TileRender(int x = 0, int y = 0, int z = 0);

  /* Destructor Function */
  ~TileRender();

  enum ItemType { Type_TileRender = UserType+1};
  int type() const { return Type_TileRender; }
public:
  /* Necessary function for returning the bounding rectangle */
  QRectF boundingRect() const;

  /* Painting function for Tile Wrapper */
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *,
             QWidget *);

public slots:
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

  /* Gets the tile for editing */
  Tile* gameTile();

  /* Sets the grid visibility */
  void setGrid(bool);
  void setPass(bool);
  void setGridColor(bool);

  /* Sets the toolbox that the tile gets the current Editor Sprite from */
  void setToolbox(SpriteView* tool);

  /* Edits the various layers */
  void editBase(EditorSprite*);
  void editEnhancer(EditorSprite*);
  void editLower(EditorSprite*, int);
  void editUpper(EditorSprite*, int);

  /* Function to place a current sprite on the maps active layer */
  void place();

  /* Function for removing a sprite from the maps active layer */
  void unplace();

  /* Gets the current active layer path */
  int getActivePath();
  int getToolPath();

  /* Transforms the image and returns it */
  QPixmap transformPixmap(EditorSprite* pic,int pos = 0);
  /* Variable for the currently active layer */
  EditorEnumDb::Layer active_layer;
protected:
private:
  /* Toggling visibility variables */
  bool base,enhancer;
  bool lower1,lower2,lower3,lower4,lower5;
  bool upper1,upper2,upper3,upper4,upper5;
  bool grid,pass;

  /* The Tile that will be placed into Univursa.exe */
  Tile* tile;

  /* A Pointer to a toolbox containing the current Editor Sprite */
  SpriteView* toolbox;

  /*Editor Sprite layers */
  EditorSprite* base_layer;
  EditorSprite* enhancer_layer;
  QList<EditorSprite*> lower_layers;
  QList<EditorSprite*> upper_layers;

  /* Positions and depths for the tiles on the map grid */
  int xpos, ypos, zpos;

  /* Grid color toggle */
  bool grid_color;
};

#endif // TILERENDER_H
