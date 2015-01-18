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
  EditorTile(int x = 0, int y = 0, TileIcons* icons = NULL);

  /* Copy constructor function */
  EditorTile(const EditorTile &source);

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

  /* The rendering tile icons */
  TileIcons* tile_icons;

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
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorTile &source);

public slots:
public:
  /* Necessary function for returning the bounding rectangle */
  QRectF boundingRect() const;

  /* Gets the tile for editing */
  Tile* getGameTile();

  /* Returns the passability based on layer and direction */
  bool getPassability(EditorEnumDb::Layer layer, Direction direction);

  /* Returns a number between 0 and 15 for what the passability is */
  int getPassabilityNum(EditorEnumDb::Layer layer);

  /* Returns the passability based on direction and what layers are visible */
  bool getPassabilityVisible(Direction direction);

  /* Returns layer visibility */
  bool getVisibility(EditorEnumDb::Layer layer);

  /* Returns grid and passability visibility */
  bool getVisibilityGrid();
  bool getVisibilityPass();

  /* Returns X and Y, in tile set */
  int getX();
  int getY();

  /* Returns the sprite based on layer and direction */
  EditorSprite* getSprite(EditorEnumDb::Layer layer);

  /* Painting function for Tile Wrapper */
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
             QWidget* widget = NULL);

  /* Function to place a current sprite on the maps active layer */
  bool place(EditorEnumDb::Layer layer, EditorSprite* sprite);

  /* Sets the hover state */
  void setHover(bool);

  /* Sets the passability based on layer and direction */
  void setPassability(EditorEnumDb::Layer layer, Direction direction,
                      bool passable);

  /* Sets the rendering tile icons */
  void setTileIcons(TileIcons* icons);

  /* Sets the various layer visibilities */
  void setVisibility(EditorEnumDb::Layer layer, bool visible);
  void setVisibilityBase(bool);
  void setVisibilityEnhancer(bool);
  bool setVisibilityLower(int, bool);
  bool setVisibilityUpper(int, bool);

  /* Sets the grid visibility */
  void setVisibilityGrid(bool);
  void setVisibilityPass(bool);

  /* Function for removing a sprite from the maps active layer */
  void unplace(EditorEnumDb::Layer layer);
  void unplace(EditorSprite* sprite);

/* Operator functions */
public:
  /* The copy operator */
  EditorTile& operator= (const EditorTile &source);
};

#endif // EDITORTILE_H
