/*******************************************************************************
 * Class Name: EditorTile
 * Date Created: January 5, 2015
 * Inheritance: QGraphicsItem
 * Description: A tile representation in a single map
 ******************************************************************************/
#ifndef EDITORTILE_H
#define EDITORTILE_H

#include <QAction>
#include <QGraphicsItem>
#include <QGraphicsSceneHoverEvent>
#include <QList>
#include <QMenu>
#include <QPainter>
#include <QWidget>

#include "Database/EditorSprite.h"
#include "EditorEnumDb.h"
#include "EditorHelpers.h"
#include "Game/Map/Tile.h"

/* Struct for frame option storage */
struct TileRenderInfo
{
  EditorSprite* sprite;
  bool visible;
};

class EditorTile : public QGraphicsItem
{
public:
  /* Constructor Function */
  EditorTile(int x = 0, int y = 0);

  /* Destructor Function */
  ~EditorTile();

  /* Control functions */
  enum ItemType { Type_TileRender = UserType + 1};
  int type() const { return Type_TileRender; }

private:
  /* Is the item hovered */
  bool hovered;

  /*Editor Sprite layers */
  TileRenderInfo layer_base;
  TileRenderInfo layer_enhancer;
  QList<TileRenderInfo> layers_lower;
  QList<TileRenderInfo> layers_upper;

  /* The Tile that will be placed into Univursa.exe */
  Tile tile;

  /* Visibility painting control */
  bool visible_grid;
  bool visible_passability;

  /* Positions and depths for the tiles on the map grid */
  int x_pos, y_pos;

  /*------------------- Constants -----------------------*/
  const static uint8_t kLOWER_COUNT_MAX; /* The max number of lower layers */
  const static uint8_t kMAX_ITEMS; /* The max number of items stored */
  const static uint8_t kUPPER_COUNT_MAX; /* The max number of upper layers */

protected:
  /* Hover events */
  void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
  void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);

public slots:
public:
  /* Necessary function for returning the bounding rectangle */
  QRectF boundingRect() const;

  /* Gets the tile for editing */
  Tile* getGameTile();

  /* Returns the passability based on layer and direction */
  bool getPassability(EditorEnumDb::Layer layer, Direction direction);

  /* Returns the sprite based on layer and direction */
  EditorSprite* getSprite(EditorEnumDb::Layer layer);

  /* Painting function for Tile Wrapper */
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
             QWidget* widget = NULL);

  /* Function to place a current sprite on the maps active layer */
  bool place(EditorEnumDb::Layer layer, EditorSprite* sprite);

  /* Sets the various layer visibilities */
  void setVisibilityBase(bool);
  void setVisibilityEnhancer(bool);
  bool setVisibilityLower(int, bool);
  bool setVisibilityUpper(int, bool);

  /* Sets the grid visibility */
  void setVisibilityGrid(bool);
  void setVisibilityPass(bool);

  /* Function for removing a sprite from the maps active layer */
  void unplace(EditorEnumDb::Layer layer);
};

#endif // EDITORTILE_H
