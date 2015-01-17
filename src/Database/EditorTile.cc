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
  temp.visible = true;

  layer_base.sprite = NULL;
  layer_base.visible = true;
  layer_enhancer.sprite = NULL;
  layer_enhancer.visible = true;
  for(int i = 0; i < kLOWER_COUNT_MAX; i++)
    layers_lower.push_back(temp);
  for(int i = 0; i < kUPPER_COUNT_MAX; i++)
    layers_upper.push_back(temp);

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
 */
QRectF EditorTile::boundingRect() const
{
  int size = EditorHelpers::getTileSize();
  return QRectF(x_pos * size, y_pos * size, size, size);
}

/*
 * Description : Gets the game tile
 *
 * Output: Tile pointer
 */
Tile* EditorTile::getGameTile()
{
  return &tile;
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
 */
void EditorTile::paint(QPainter *painter,
                        const QStyleOptionGraphicsItem*, QWidget*)
{
  int size = EditorHelpers::getTileSize();
  QRect bound(x_pos * size, y_pos * size, size, size);

  /* Render the tile sprites */
  if(layer_base.visible && layer_base.sprite != NULL)
    layer_base.sprite->paint(painter, bound);

  if(layer_enhancer.visible && layer_enhancer.sprite !=NULL)
    layer_enhancer.sprite->paint(painter, bound);

  for(int i = 0; i < layers_lower.size(); i++)
    if(layers_lower[i].visible && layers_lower[i].sprite != NULL)
      layers_lower[i].sprite->paint(painter, bound);

  for(int i = 0; i < layers_upper.size(); i++)
    if(layers_upper[i].visible && layers_upper[i].sprite != NULL)
      layers_upper[i].sprite->paint(painter, bound);

  /* Render the grid */
  if(visible_grid)
  {
    QPen pen(QColor(255, 255, 255, 128));

    if(hovered)
    {
      pen.setColor(QColor(0,255,100,255));
      //pen.setWidth(2);
    }

    /* Set pen and draw rect */
    painter->setPen(pen);
    painter->drawRect(1 + (x_pos * size), 1 + (y_pos * size),
                      size - 2, size - 2);
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
 * Description: Places the currently selected sprite onto the active map layer
 *
 * Inputs: EditorEnumDb::Layer - the layer to change the sprite
 *         EditorSprite* - the new sprite
 * Output: bool - true if the sprite was changed
 */
bool EditorTile::place(EditorEnumDb::Layer layer, EditorSprite* sprite)
{
  if(sprite != NULL)
  {
    switch(layer)
    {
      case EditorEnumDb::BASE:
        layer_base.sprite = sprite;
        tile.setBase(sprite->getSprite());

        /* Default to fully passable */
        tile.setBasePassability(Direction::NORTH, true);
        tile.setBasePassability(Direction::EAST, true);
        tile.setBasePassability(Direction::SOUTH, true);
        tile.setBasePassability(Direction::WEST, true);
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
 * Output: none
 */
void EditorTile::setHover(bool hover)
{
  hovered = hover;
  update();
}

/* Sets the passability based on layer and direction */
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
