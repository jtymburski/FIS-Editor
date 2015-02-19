/*******************************************************************************
 * Class Name: EditorTile
 * Date Created: January 5, 2015
 * Inheritance: QGraphicsItem
 * Description: A tile representation in a single map
 ******************************************************************************/
#include "Database/EditorTile.h"
#include <QDebug>

/* Constant Implementation - see header file for descriptions */
const uint8_t EditorTile::kLOWER_COUNT_MAX = 5;
const uint8_t EditorTile::kMAX_ITEMS = 20;
const uint8_t EditorTile::kRENDER_DEPTH = 10;
const uint8_t EditorTile::kUPPER_COUNT_MAX = 5;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function
 *
 * Input: x/y coordinates on the map grid and a z depth
 *        TileIcons* icons - the rendering tile icons. Default NULL
 */
EditorTile::EditorTile(int x, int y, TileIcons* icons)
{
  setAcceptHoverEvents(true);

  /* Class control */
  hovered = false;
  tile.setStatus(Tile::ACTIVE);
  visible_grid = true;
  visible_passability = false;
  x_pos = x;
  y_pos = y;

  /* Layer control */
  TileRenderInfo temp;
  temp.sprite = NULL;
  temp.thing = NULL;
  temp.visible = true;

  /* Prep editor sprites in tile */
  layer_base.sprite = NULL;
  layer_base.visible = true;
  layer_enhancer.sprite = NULL;
  layer_enhancer.visible = true;
  for(int i = 0; i < kLOWER_COUNT_MAX; i++)
    layers_lower.push_back(temp);
  for(int i = 0; i < kUPPER_COUNT_MAX; i++)
    layers_upper.push_back(temp);

  /* Prep editor things in tile */
  for(uint8_t i = 0; i < kRENDER_DEPTH; i++)
    things.push_back(temp);

  /* Set tile icons */
  setTileIcons(icons);
}

/*
 * Description: Copy constructor function
 *
 * Inputs: EditorTile &source - the source to copy from
 */
EditorTile::EditorTile(const EditorTile &source) : EditorTile()
{
  copySelf(source);
}

/*
 * Description: Destructor function
 */
EditorTile::~EditorTile()
{
  layer_base.sprite = NULL;
  layer_enhancer.sprite = NULL;
  layers_lower.clear();
  layers_upper.clear();
  tile.clear();
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: The copy function that is called by any copying methods in the
 *              class. Utilized by the copy constructor and the copy operator.
 *
 * Inputs: const EditorTile &source - the reference sprite class
 * Output: none
 */
void EditorTile::copySelf(const EditorTile &source)
{
  /* Copy normal variables */
  hovered = false;
  tile = source.tile;
  visible_grid = source.visible_grid;
  visible_passability = source.visible_passability;
  x_pos = source.x_pos;
  y_pos = source.y_pos;

  /* Copy base */
  layer_base.sprite = source.layer_base.sprite;
  layer_base.visible = source.layer_base.visible;

  /* Copy enhancer */
  layer_enhancer.sprite = source.layer_enhancer.sprite;
  layer_enhancer.visible = source.layer_enhancer.visible;

  /* Copy lower */
  for(int i = 0; i < layers_lower.size(); i++)
  {
    layers_lower[i].sprite = source.layers_lower[i].sprite;
    layers_lower[i].visible = source.layers_lower[i].visible;
  }

  /* Copy upper */
  for(int i = 0; i < layers_upper.size(); i++)
  {
    layers_upper[i].sprite = source.layers_upper[i].sprite;
    layers_upper[i].visible = source.layers_upper[i].visible;
  }
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Returns the bounding rectangle (Needed by API)
 *
 * Inputs: none
 * Output: QRectF - a float rect struct
 */
QRectF EditorTile::boundingRect() const
{
  int size = EditorHelpers::getTileSize();
  return QRectF(x_pos * size, y_pos * size, size, size);
}

/*
 * Description: Returns the active layers of the tile, as a comma-delimited
 *              string.
 *
 * Inputs: none
 * Output: QString - comma-delimited string format - B,E,L1, etc.
 */
QString EditorTile::getActiveLayers()
{
  QString layer_string = "";

  /* Make the string */
  if(layer_base.sprite != NULL)
    layer_string += "B(" + QString::number(layer_base.sprite->getID()) + "),";
  if(layer_enhancer.sprite != NULL)
    layer_string += "E(" + QString::number(layer_enhancer.sprite->getID())
                 + "),";
  for(int i = 0; i < layers_lower.size(); i++)
    if(layers_lower[i].sprite != NULL)
      layer_string += "L" + QString::number(i + 1) + "("
                   + QString::number(layers_lower[i].sprite->getID()) + "),";
  for(int i = 0; i < layers_upper.size(); i++)
    if(layers_upper[i].sprite != NULL)
      layer_string += "U" + QString::number(i + 1) + "("
                   + QString::number(layers_upper[i].sprite->getID()) + "),";

  /* String clean-up */
  if(layer_string.size() > 0)
    layer_string.chop(1);

  return layer_string;
}

/*
 * Description : Gets the game tile
 *
 * Inputs: none
 * Output: Tile pointer
 */
Tile* EditorTile::getGameTile()
{
  return &tile;
}

/*
 * Description: Returns the hover information for the tile.
 *
 * Inputs: none
 * Output: HoverInfo* - hover information struct reference. do not delete.
 */
HoverInfo* EditorTile::getHoverInfo()
{
  return hover_info;
}

/*
 * Description: Returns the passability at the indicated layer in the
 *              indicated direction.
 *
 * Inputs: EditorEnumDb::Layer - the layer to grab the passability
 *         Direction direction - the direction to get the passability in
 * Output: bool - true if passable in that direction.
 */
bool EditorTile::getPassability(EditorEnumDb::Layer layer, Direction direction)
{
  switch(layer)
  {
    case EditorEnumDb::BASE:
      return tile.getBasePassability(direction);
      break;
    case EditorEnumDb::LOWER1:
      return tile.getLowerPassability(0, direction);
      break;
    case EditorEnumDb::LOWER2:
      return tile.getLowerPassability(1, direction);
      break;
    case EditorEnumDb::LOWER3:
      return tile.getLowerPassability(2, direction);
      break;
    case EditorEnumDb::LOWER4:
      return tile.getLowerPassability(3, direction);
      break;
    case EditorEnumDb::LOWER5:
      return tile.getLowerPassability(4, direction);
      break;
    case EditorEnumDb::ENHANCER:
    case EditorEnumDb::UPPER1:
    case EditorEnumDb::UPPER2:
    case EditorEnumDb::UPPER3:
    case EditorEnumDb::UPPER4:
    case EditorEnumDb::UPPER5:
      return true;
      break;
    default:
      break;
  }
  return false;
}

/*
 * Description: Returns a number between 0 and 15 for what the passability is.
 *              This number is the binary realization in base 10 of the 4
 *              passability directions.
 *
 * Inputs: EditorEnumDb::Layer layer - the layer to check passability on
 * Output: int - the base 10 integer representation of passability
 */
int EditorTile::getPassabilityNum(EditorEnumDb::Layer layer)
{
  return EditorHelpers::getPassabilityNum(
                                        getPassability(layer, Direction::NORTH),
                                        getPassability(layer, Direction::EAST),
                                        getPassability(layer, Direction::SOUTH),
                                        getPassability(layer, Direction::WEST));
}

/*
 * Description: Returns passability based on visible layers and a direction.
 *
 * Inputs: Direction direction - the direction trying to exit.
 * Output: bool - true if passable
 */
bool EditorTile::getPassabilityVisible(Direction direction)
{
  bool passable = true;

  if(layer_base.visible)
    passable &= getPassability(EditorEnumDb::BASE, direction);
  if(layers_lower[0].visible)
    passable &= getPassability(EditorEnumDb::LOWER1, direction);
  if(layers_lower[1].visible)
    passable &= getPassability(EditorEnumDb::LOWER2, direction);
  if(layers_lower[2].visible)
    passable &= getPassability(EditorEnumDb::LOWER3, direction);
  if(layers_lower[3].visible)
    passable &= getPassability(EditorEnumDb::LOWER4, direction);
  if(layers_lower[4].visible)
    passable &= getPassability(EditorEnumDb::LOWER5, direction);

  return passable;
}

/* Returns the map thing pointer(s) for the generic thing */
// TODO: Comment
EditorThing* EditorTile::getThing(int render_level)
{
  if(render_level >= 0 && render_level < kRENDER_DEPTH)
    return things[render_level].thing;
  return NULL;
}

/*
 * Description: Returns the visibility of the passed in layer.
 *
 * Inputs: EditorEnumDb::Layer layer - the layer to get visibility for
 * Output: bool - true if that layer is visible
 */
bool EditorTile::getVisibility(EditorEnumDb::Layer layer)
{
  if(layer == EditorEnumDb::BASE)
    return layer_base.visible;
  else if(layer == EditorEnumDb::ENHANCER)
    return layer_enhancer.visible;
  else if(layer == EditorEnumDb::LOWER1)
    return layers_lower[0].visible;
  else if(layer == EditorEnumDb::LOWER2)
    return layers_lower[1].visible;
  else if(layer == EditorEnumDb::LOWER3)
    return layers_lower[2].visible;
  else if(layer == EditorEnumDb::LOWER4)
    return layers_lower[3].visible;
  else if(layer == EditorEnumDb::LOWER5)
    return layers_lower[4].visible;
  else if(layer == EditorEnumDb::UPPER1)
    return layers_upper[0].visible;
  else if(layer == EditorEnumDb::UPPER2)
    return layers_upper[1].visible;
  else if(layer == EditorEnumDb::UPPER3)
    return layers_upper[2].visible;
  else if(layer == EditorEnumDb::UPPER4)
    return layers_upper[3].visible;
  else if(layer == EditorEnumDb::UPPER5)
    return layers_upper[4].visible;
  return true;
}

/* Returns layer visibility */
// TODO: Comment
bool EditorTile::getVisibilityThing(int render_level)
{
  if(render_level >= 0 && render_level < kRENDER_DEPTH)
    return things[render_level].visible;
  return false;
}

/*
 * Description: Returns if the grid is visible on the tile.
 *
 * Inputs: none
 * Output: bool - true if grid is visible
 */
bool EditorTile::getVisibilityGrid()
{
  return visible_grid;
}

/*
 * Description: Returns if the passability is visible on the tile.
 *
 * Inputs: none
 * Output: bool - true if passability is visible
 */
bool EditorTile::getVisibilityPass()
{
  return visible_passability;
}

/*
 * Description: Returns the X of the tile in the map array.
 *
 * Inputs: none
 * Output: int - array interpretation of X
 */
int EditorTile::getX()
{
  return x_pos;
}

/*
 * Description: Returns the Y of the tile in the map array.
 *
 * Inputs: none
 * Output: int - array interpretation of Y
 */
int EditorTile::getY()
{
  return y_pos;
}

/*
 * Description: Returns the sprite at the indicated layer
 *
 * Inputs: EditorEnumDb::Layer - the layer to grab the sprite
 * Output: EditorSprite* - the sprite pointer
 */
EditorSprite* EditorTile::getSprite(EditorEnumDb::Layer layer)
{
  switch(layer)
  {
    case EditorEnumDb::BASE:
      return layer_base.sprite;
      break;
    case EditorEnumDb::ENHANCER:
      return layer_enhancer.sprite;
      break;
    case EditorEnumDb::LOWER1:
      return layers_lower[0].sprite;
      break;
    case EditorEnumDb::LOWER2:
      return layers_lower[1].sprite;
      break;
    case EditorEnumDb::LOWER3:
      return layers_lower[2].sprite;
      break;
    case EditorEnumDb::LOWER4:
      return layers_lower[3].sprite;
      break;
    case EditorEnumDb::LOWER5:
      return layers_lower[4].sprite;
      break;
    case EditorEnumDb::UPPER1:
      return layers_upper[0].sprite;
      break;
    case EditorEnumDb::UPPER2:
      return layers_upper[1].sprite;
      break;
    case EditorEnumDb::UPPER3:
      return layers_upper[2].sprite;
      break;
    case EditorEnumDb::UPPER4:
      return layers_upper[3].sprite;
      break;
    case EditorEnumDb::UPPER5:
      return layers_upper[4].sprite;
      break;
    default:
      break;
  }
  return NULL;
}

/*
 * Description: Paints the tile
 *
 * Input: Required fields, mostly unused
 * Output: none
 */
// TODO: Update for new render information
void EditorTile::paint(QPainter *painter,
                        const QStyleOptionGraphicsItem*, QWidget*)
{
  int size = EditorHelpers::getTileSize();
  QRect bound(x_pos * size, y_pos * size, size, size);

  /* Render the base */
  if(layer_base.visible && layer_base.sprite != NULL)
    layer_base.sprite->paint(painter, bound);

  /* Render the enhancer */
  if(layer_enhancer.visible && layer_enhancer.sprite !=NULL)
    layer_enhancer.sprite->paint(painter, bound);

  /* Render the lower */
  for(int i = 0; i < layers_lower.size(); i++)
    if(layers_lower[i].visible && layers_lower[i].sprite != NULL)
      layers_lower[i].sprite->paint(painter, bound);

  /* Render the map things */
  for(int i = 0; i < things.size(); i++)
  {
    //if(things)
  }

  /* Render the upper */
  for(int i = 0; i < layers_upper.size(); i++)
    if(layers_upper[i].visible && layers_upper[i].sprite != NULL)
      layers_upper[i].sprite->paint(painter, bound);

  /* Render the grid */
  if(visible_grid)
  {
    painter->setPen(QColor(255, 255, 255, 128));
    painter->drawRect(1 + (x_pos * size), 1 + (y_pos * size),
                      size - 2, size - 2);
  }
  if(hovered)
  {
    painter->fillRect(x_pos * size + 1, y_pos * size + 1, size - 2, size - 2,
                      QColor(200, 200, 200, 128));
  }

  /* Render the passability */
  if(visible_passability && tile_icons != NULL)
  {
    /* North Passability */
    if(getPassabilityVisible(Direction::NORTH))
      painter->drawPixmap(x_pos * size, y_pos * size, size, size,
                          *tile_icons->passN);
    else
      painter->drawPixmap(x_pos * size, y_pos * size, size, size,
                          *tile_icons->nopassN);

    /* East Passability */
    if(getPassabilityVisible(Direction::EAST))
      painter->drawPixmap(x_pos * size, y_pos * size, size, size,
                          *tile_icons->passE);
    else
      painter->drawPixmap(x_pos * size, y_pos * size, size, size,
                          *tile_icons->nopassE);

    /* South Passability */
    if(getPassabilityVisible(Direction::SOUTH))
      painter->drawPixmap(x_pos * size, y_pos * size, size, size,
                          *tile_icons->passS);
    else
      painter->drawPixmap(x_pos * size, y_pos * size, size, size,
                          *tile_icons->nopassS);

    /* West Passability */
    if(getPassabilityVisible(Direction::WEST))
      painter->drawPixmap(x_pos * size, y_pos * size, size, size,
                          *tile_icons->passW);
    else
      painter->drawPixmap(x_pos * size, y_pos * size, size, size,
                          *tile_icons->nopassW);
  }
}

/*
 * Description: Places the currently selected sprite onto the active map layer.
 *              Only relevant if the tile is currently being hovered and the
 *              hover information has been set.
 *
 * Inputs: none
 * Output: bool - true if the sprite was changed
 */
bool EditorTile::place()
{
  if(hovered && hover_info != NULL)
    return place(hover_info->active_layer, hover_info->active_sprite);
  return false;
}

/*
 * Description: Places the currently selected sprite onto the active map layer
 *
 * Inputs: EditorEnumDb::Layer - the layer to change the sprite
 *         EditorSprite* - the new sprite
 *         bool load - if triggered by a file load
 * Output: bool - true if the sprite was changed
 */
bool EditorTile::place(EditorEnumDb::Layer layer, EditorSprite* sprite,
                       bool load)
{
  if(sprite != NULL)
  {
    switch(layer)
    {
      case EditorEnumDb::BASE:
        layer_base.sprite = sprite;
        tile.setBase(sprite->getSprite());

        /* Default to fully passable */
        if(!load)
        {
          tile.setBasePassability(Direction::NORTH, true);
          tile.setBasePassability(Direction::EAST, true);
          tile.setBasePassability(Direction::SOUTH, true);
          tile.setBasePassability(Direction::WEST, true);
        }
        break;
      case EditorEnumDb::ENHANCER:
        layer_enhancer.sprite = sprite;
        tile.setEnhancer(sprite->getSprite());
        break;
      case EditorEnumDb::LOWER1:
        layers_lower[0].sprite = sprite;
        tile.insertLower(sprite->getSprite(), 0);
        break;
      case EditorEnumDb::LOWER2:
        layers_lower[1].sprite = sprite;
        tile.insertLower(sprite->getSprite(), 1);
        break;
      case EditorEnumDb::LOWER3:
        layers_lower[2].sprite = sprite;
        tile.insertLower(sprite->getSprite(), 2);
        break;
      case EditorEnumDb::LOWER4:
        layers_lower[3].sprite = sprite;
        tile.insertLower(sprite->getSprite(), 3);
        break;
      case EditorEnumDb::LOWER5:
        layers_lower[4].sprite = sprite;
        tile.insertLower(sprite->getSprite(), 4);
        break;
      case EditorEnumDb::UPPER1:
        layers_upper[0].sprite = sprite;
        tile.insertUpper(sprite->getSprite(), 0);
        break;
      case EditorEnumDb::UPPER2:
        layers_upper[1].sprite = sprite;
        tile.insertUpper(sprite->getSprite(), 1);
        break;
      case EditorEnumDb::UPPER3:
        layers_upper[2].sprite = sprite;
        tile.insertUpper(sprite->getSprite(), 2);
        break;
      case EditorEnumDb::UPPER4:
        layers_upper[3].sprite = sprite;
        tile.insertUpper(sprite->getSprite(), 3);
        break;
      case EditorEnumDb::UPPER5:
        layers_upper[4].sprite = sprite;
        tile.insertUpper(sprite->getSprite(), 4);
        break;
      default:
        break;
    }
    update();
    return true;
  }
  return false;
}

/*
 * Description: Sets the hover state of the QGraphicsItem.
 *
 * Inputs: bool hover - is it being hovered on?
 *         bool hover_invalid - is the hovering tile un-setable (default false)
 * Output: none
 */
void EditorTile::setHover(bool hover, bool hover_invalid)
{
  hovered = hover;
  hovered_invalid = hover_invalid;
  update();
}

/*
 * Description: Sets the hover info of the tile - pointer reference from
 *              EditorMap (this class will not delete it)
 *
 * Inputs: HoverInfo* new_info - a struct with the hover information
 * Output: none
 */
void EditorTile::setHoverInfo(HoverInfo* new_info)
{
  hover_info = new_info;
  update();
}

/*
 * Description: Sets the combined hover status and hover information.
 *
 * Inputs: bool hover - is it being hovered on?
 *         bool hover_invalid - is the hovering tile un-setable (default false)
 *         HoverInfo* new_info - a struct with the hover information
 * Output: none
 */
void EditorTile::setHoverInfo(bool hover, bool hover_invalid,
                              HoverInfo* new_info)
{
  setHover(hover, hover_invalid);
  setHoverInfo(new_info);
}

/*
 * Description: Sets the passability of the passed in layer and the selected
 *              direction.
 *
 * Inputs: EditorEnumDb::Layer layer - the layer to update the passability for
 *         Direction direction - the direction to manipulate
 *         bool passable - is that direction and layer passable??
 * Output: none
 */
void EditorTile::setPassability(EditorEnumDb::Layer layer, Direction direction,
                                bool passable)
{
  switch(layer)
  {
    case EditorEnumDb::BASE:
      tile.setBasePassability(direction, passable);
      break;
    case EditorEnumDb::LOWER1:
      tile.setLowerPassability(0, direction, passable);
      break;
    case EditorEnumDb::LOWER2:
      tile.setLowerPassability(1, direction, passable);
      break;
    case EditorEnumDb::LOWER3:
      tile.setLowerPassability(2, direction, passable);
      break;
    case EditorEnumDb::LOWER4:
      tile.setLowerPassability(3, direction, passable);
      break;
    case EditorEnumDb::LOWER5:
      tile.setLowerPassability(4, direction, passable);
      break;
    case EditorEnumDb::ENHANCER:
    case EditorEnumDb::UPPER1:
    case EditorEnumDb::UPPER2:
    case EditorEnumDb::UPPER3:
    case EditorEnumDb::UPPER4:
    case EditorEnumDb::UPPER5:
    default:
      break;
  }
  update();
}

/* Sets the thing sprite pointer, stored within the class */
// TODO: Comment
bool EditorTile::setThing(EditorThing* thing, int render_level)
{
  if(thing != NULL && render_level >= 0 && render_level < kRENDER_DEPTH)
  {
    unsetThing(render_level);
    things[render_level].thing = thing;
    update();

    return true;
  }
  return false;
}

/*
 * Description: Sets the tile icons, for rendering purposes.
 *
 * Inputs: TileIcons* icons - the rendering icon pointer. Managed by gamedb
 * Output: none
 */
void EditorTile::setTileIcons(TileIcons* icons)
{
  tile_icons = icons;
}

/*
 * Description: Sets the visiblity based on the layer and visible status.
 *
 * Inputs: EditorEnumDb::Layer layer - the layer to modify visibility
 *         bool visible - is the layer visible?
 * Output: none
 */
void EditorTile::setVisibility(EditorEnumDb::Layer layer, bool visible)
{
  if(layer == EditorEnumDb::BASE)
    setVisibilityBase(visible);
  else if(layer == EditorEnumDb::ENHANCER)
    setVisibilityEnhancer(visible);
  else if(layer == EditorEnumDb::LOWER1)
    setVisibilityLower(0, visible);
  else if(layer == EditorEnumDb::LOWER2)
    setVisibilityLower(1, visible);
  else if(layer == EditorEnumDb::LOWER3)
    setVisibilityLower(2, visible);
  else if(layer == EditorEnumDb::LOWER4)
    setVisibilityLower(3, visible);
  else if(layer == EditorEnumDb::LOWER5)
    setVisibilityLower(4, visible);
  else if(layer == EditorEnumDb::UPPER1)
    setVisibilityUpper(0, visible);
  else if(layer == EditorEnumDb::UPPER2)
    setVisibilityUpper(1, visible);
  else if(layer == EditorEnumDb::UPPER3)
    setVisibilityUpper(2, visible);
  else if(layer == EditorEnumDb::UPPER4)
    setVisibilityUpper(3, visible);
  else if(layer == EditorEnumDb::UPPER5)
    setVisibilityUpper(4, visible);
}

/*
 * Description: Sets the base layer visibility
 *
 * Input: Visibility toggle
 */
void EditorTile::setVisibilityBase(bool toggle)
{
  layer_base.visible = toggle;
  update();
}

/*
 * Description: Sets the enhancer layer visibility
 *
 * Input: Visibility toggle
 */
void EditorTile::setVisibilityEnhancer(bool toggle)
{
  layer_enhancer.visible = toggle;
  update();
}

/*
 * Description: Sets the grid visibility
 *
 * Input: Visibility toggle
 */
void EditorTile::setVisibilityGrid(bool toggle)
{
  visible_grid = toggle;
  update();
}

/*
 * Description: Sets the lower layer visibility as per the index
 *
 * Input: Visibility toggle
 * Output: bool - success if set
 */
bool EditorTile::setVisibilityLower(int index, bool toggle)
{
  if(index >= 0 && index < kLOWER_COUNT_MAX)
  {
    layers_lower[index].visible = toggle;
    update();
    return true;
  }
  return false;
}

/*
 * Description: Sets the passability visibility
 *
 * Input: Visibility toggle
 */
void EditorTile::setVisibilityPass(bool toggle)
{
  visible_passability = toggle;
  update();
}

/* Sets the various layer visibilities */
// TODO: Comment
void EditorTile::setVisibilityThing(bool visible)
{
  for(int i = 0; i < things.size(); i++)
    things[i].visible = visible;
}

/* Sets the various layer visibilities */
// TODO: Comment
bool EditorTile::setVisibilityThing(int render_level, bool visible)
{
  if(render_level >= 0 && render_level < kRENDER_DEPTH)
  {
    things[render_level].visible = visible;
    return true;
  }
  return false;
}

/*
 * Description: Sets the upper layer visibility as per the index
 *
 * Input: Visibility toggle
 * Output: bool - success if set
 */
bool EditorTile::setVisibilityUpper(int index, bool toggle)
{
  if(index >= 0 && index < kUPPER_COUNT_MAX)
  {
    layers_upper[index].visible = toggle;
    update();
    return true;
  }
  return false;
}

/*
 * Description: Removes the currently selected sprite onto the active map layer
 *
 * Inputs: EditorEnumDb::Layer - the layer to remove the sprite from
 * Output: none
 */
void EditorTile::unplace(EditorEnumDb::Layer layer)
{
  switch(layer)
  {
    case EditorEnumDb::BASE:
      layer_base.sprite = NULL;
      tile.unsetBase();
      break;
    case EditorEnumDb::ENHANCER:
      layer_enhancer.sprite = NULL;
      tile.unsetEnhancer();
      break;
    case EditorEnumDb::LOWER1:
      layers_lower[0].sprite = NULL;
      tile.unsetLower(0);
      break;
    case EditorEnumDb::LOWER2:
      layers_lower[1].sprite = NULL;
      tile.unsetLower(1);
      break;
    case EditorEnumDb::LOWER3:
      layers_lower[2].sprite = NULL;
      tile.unsetLower(2);
      break;
    case EditorEnumDb::LOWER4:
      layers_lower[3].sprite = NULL;
      tile.unsetLower(3);
      break;
    case EditorEnumDb::LOWER5:
      layers_lower[4].sprite = NULL;
      tile.unsetLower(4);
      break;
    case EditorEnumDb::UPPER1:
      layers_upper[0].sprite = NULL;
      tile.unsetUpper(0);
      break;
    case EditorEnumDb::UPPER2:
      layers_upper[1].sprite = NULL;
      tile.unsetUpper(1);
      break;
    case EditorEnumDb::UPPER3:
      layers_upper[2].sprite = NULL;
      tile.unsetUpper(2);
      break;
    case EditorEnumDb::UPPER4:
      layers_upper[3].sprite = NULL;
      tile.unsetUpper(3);
      break;
    case EditorEnumDb::UPPER5:
      layers_upper[4].sprite = NULL;
      tile.unsetUpper(4);
      break;
    default:
      break;
  }
  update();
}

/*
 * Description: Removes the editor sprite pointer from all layers.
 *
 * Inputs: EditorSprite* sprite - the sprite to remove
 * Output: none
 */
void EditorTile::unplace(EditorSprite* sprite)
{
  /* Check base */
  if(layer_base.sprite == sprite)
  {
    layer_base.sprite = NULL;
    tile.unsetBase();
  }

  /* Check enhancer */
  if(layer_enhancer.sprite == sprite)
  {
    layer_enhancer.sprite = NULL;
    tile.unsetEnhancer();
  }

  /* Check lowers */
  for(int i = 0; i < layers_lower.size(); i++)
  {
    if(layers_lower[i].sprite == sprite)
    {
      layers_lower[i].sprite = NULL;
      tile.unsetLower(i);
    }
  }

  /* Check uppers */
  for(int i = 0; i < layers_upper.size(); i++)
  {
    if(layers_upper[i].sprite == sprite)
    {
      layers_upper[i].sprite = NULL;
      tile.unsetUpper(i);
    }
  }

  update();
}

/* Unsets the stored thing pointer(s) */
// TODO: Comment
bool EditorTile::unsetThing(EditorThing* thing)
{
  for(int i = 0; i < things.size(); i++)
  {
    if(things[i].thing == thing)
    {
      things[i].thing = NULL;
      update();
      return true;
    }
  }

  return false;
}

/* Unsets the stored thing pointer(s) */
// TODO: Comment
bool EditorTile::unsetThing(int render_level)
{
  if(render_level >= 0 && render_level < things.size())
  {
    if(things[render_level].thing != NULL)
    {
      things[render_level].thing = NULL;
      update();
      return true;
    }
  }

  return false;
}

/* Unsets the stored thing pointer(s) */
// TODO: Comment
void EditorTile::unsetThings()
{
  for(int i = 0; i < things.size(); i++)
    things[i].thing = NULL;
  update();
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/*
 * Description: Copy operator construction. This is called when the variable
 *              already exists and equal operator used with another object.
 *
 * Inputs: const EditorTile &source - the source class constructor
 * Output: EditorTile& - reference to the copied class
 */
EditorTile& EditorTile::operator= (const EditorTile &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
