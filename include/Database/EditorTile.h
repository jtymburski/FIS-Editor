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
#include "Database/EditorThing.h"
#include "EditorEnumDb.h"
#include "EditorHelpers.h"
#include "Game/Map/Tile.h"

class EditorTile;

/* Struct for handling hover information */
struct HoverInfo
{
  EditorEnumDb::Layer active_layer;
  EditorEnumDb::CursorMode active_cursor;

  EditorSprite* active_sprite;
  EditorThing* active_thing;

  EditorTile* hover_tile;
};

/* Struct for frame option storage */
struct TileRenderInfo
{
  EditorSprite* sprite;
  EditorThing* thing;
  bool visible;
};

/* Editor Tile Class */
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
  HoverInfo* hover_info;
  bool hovered;
  bool hovered_invalid;

  /* Editor Sprite layers */
  TileRenderInfo layer_base;
  TileRenderInfo layer_enhancer;
  QList<TileRenderInfo> layers_lower;
  QList<TileRenderInfo> layers_upper;

  /* Things on the tile */
  QList<TileRenderInfo> things;

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

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorTile &source);

  /* Determine if hovering sprite or thing in tile */
  bool isHoverSprite();
  bool isHoverThing();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Necessary function for returning the bounding rectangle */
  QRectF boundingRect() const;

  /* Returns the active layers in a string */
  QString getActiveLayers();

  /* Gets the tile for editing */
  Tile* getGameTile();

  /* Gets the hover information */
  HoverInfo* getHoverInfo();

  /* Returns the passability based on layer and direction */
  bool getPassability(EditorEnumDb::Layer layer, Direction direction);

  /* Returns a number between 0 and 15 for what the passability is */
  int getPassabilityNum(EditorEnumDb::Layer layer);

  /* Returns the passability of the thing */
  bool getPassabilityThing(Direction direction);

  /* Returns the passability based on direction and what layers are visible */
  bool getPassabilityVisible(Direction direction);

  /* Returns the map thing pointer(s) for the generic thing */
  EditorThing* getThing(int render_level);

  /* Returns layer visibility */
  bool getVisibility(EditorEnumDb::Layer layer);
  bool getVisibilityThing(int render_level);

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
  bool place();
  bool place(EditorEnumDb::Layer layer, EditorSprite* sprite,
             bool load = false);

  /* Sets the hover state */
  void setHover(bool hover, bool hover_invalid = false);

  /* Sets the hover information - from editor map */
  void setHoverInfo(HoverInfo* new_info);
  void setHoverInfo(bool hover, bool hover_invalid = false,
                    HoverInfo* new_info = NULL);

  /* Sets the passability based on layer and direction */
  void setPassability(EditorEnumDb::Layer layer, bool passable);
  void setPassability(EditorEnumDb::Layer layer, Direction direction,
                      bool passable);

  /* Sets the thing sprite pointer, stored within the class */
  bool setThing(EditorThing* thing);

  /* Sets the rendering tile icons */
  void setTileIcons(TileIcons* icons);

  /* Sets the various layer visibilities */
  void setVisibility(EditorEnumDb::Layer layer, bool visible);
  void setVisibilityBase(bool);
  void setVisibilityEnhancer(bool);
  bool setVisibilityLower(int, bool);
  void setVisibilityThing(bool visible);
  bool setVisibilityThing(int render_level, bool visible);
  bool setVisibilityUpper(int, bool);

  /* Sets the grid visibility */
  void setVisibilityGrid(bool);
  void setVisibilityPass(bool);

  /* Function for removing a sprite from the maps active layer */
  void unplace(EditorEnumDb::Layer layer);
  void unplace(EditorSprite* sprite);

  /* Unsets the stored thing pointer(s) */
  bool unsetThing(EditorThing* thing);
  bool unsetThing(int render_level);
  void unsetThings();

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  /* The copy operator */
  EditorTile& operator= (const EditorTile &source);
};

#endif // EDITORTILE_H
