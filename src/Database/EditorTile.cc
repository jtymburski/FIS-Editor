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
 */
EditorTile::EditorTile(int x, int y)
{
  /* Class control */
  setAcceptHoverEvents(true);
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
  *layer_base.sprite = *source.layer_base.sprite;
  layer_base.visible = source.layer_base.visible;

  /* Copy enhancer */
  *layer_enhancer.sprite = *source.layer_enhancer.sprite;
  layer_enhancer.visible = source.layer_enhancer.visible;

  /* Copy lower */
  for(int i = 0; i < layers_lower.size(); i++)
  {
    *layers_lower[i].sprite = *source.layers_lower[i].sprite;
    layers_lower[i].visible = source.layers_lower[i].visible;
  }

  /* Copy upper */
  for(int i = 0; i < layers_upper.size(); i++)
  {
    *layers_upper[i].sprite = *source.layers_upper[i].sprite;
    layers_upper[i].visible = source.layers_upper[i].visible;
  }
}

/*
 * Description: Hover enter event class re-implementation
 *
 * Inputs: QGraphicsSceneHoverEvent* - hover event handler
 * Output: none
 */
void EditorTile::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{
  (void)event;
  hovered = true;
  update();
}

/*
 * Description: Hover exit event class re-implementation
 *
 * Inputs: QGraphicsSceneHoverEvent* - hover event handler
 * Output: none
 */
void EditorTile::hoverLeaveEvent(QGraphicsSceneHoverEvent * event)
{
  (void)event;
  hovered = false;
  update();
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
    if(hovered)
      painter->setPen(QColor(0,255,100,255));
    else
      painter->setPen(QColor(255,255,255,128));

    painter->drawRect(1 + (x_pos * size), 1 + (y_pos * size),
                      size - 2, size - 2);
  }

  /* Render the passability */
  if(visible_passability)
  {
    painter->setPen(QColor(0,0,0,0));
    QPointF Npoints[4] =
    {
      QPointF((x_pos * size) + 0, (y_pos * size) + 0),
      QPointF((x_pos * size) + 4, (y_pos * size) + 4),
      QPointF((x_pos * size) + size - 4, (y_pos * size) + 4),
      QPointF((x_pos * size) + size, (y_pos * size) + 0)
    };
    QPointF Epoints[4] =
    {
      QPointF((x_pos * size) + size,(y_pos*size)+0),
      QPointF((x_pos * size) + size-4,(y_pos*size)+4),
      QPointF((x_pos * size) + size-4,(y_pos*size)+size-4),
      QPointF((x_pos * size) + size,(y_pos*size)+size)
    };
    QPointF Spoints[4] =
    {
      QPointF((x_pos * size) + 0, (y_pos * size) + size),
      QPointF((x_pos * size) + 4, (y_pos * size) + size - 4),
      QPointF((x_pos * size) + size - 4, (y_pos * size) + size - 4),
      QPointF((x_pos * size) + size, (y_pos * size) + size)
    };
    QPointF Wpoints[4] =
    {
      QPointF((x_pos * size) + 0, (y_pos * size) + 0),
      QPointF((x_pos * size) + 4, (y_pos * size) + 4),
      QPointF((x_pos * size) + 4, (y_pos * size) + size - 4),
      QPointF((x_pos * size) + 0, (y_pos * size) + size)
    };
    if(tile.getPassabilityExiting(Direction::NORTH))
      painter->setBrush(QColor(0,255,0,128));
    else
      painter->setBrush(QColor(255,0,0,128));
    painter->drawPolygon(Npoints,4);

    if(tile.getPassabilityExiting(Direction::EAST))
      painter->setBrush(QColor(0,255,0,128));
    else
      painter->setBrush(QColor(255,0,0,128));
    painter->drawPolygon(Epoints,4);

    if(tile.getPassabilityExiting(Direction::SOUTH))
      painter->setBrush(QColor(0,255,0,128));
    else
      painter->setBrush(QColor(255,0,0,128));
    painter->drawPolygon(Spoints,4);

    if(tile.getPassabilityExiting(Direction::WEST))
      painter->setBrush(QColor(0,255,0,128));
    else
      painter->setBrush(QColor(255,0,0,128));
    painter->drawPolygon(Wpoints,4);
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
 * Description: Sets the base layer visibility
 *
 * Input: Visibility toggle
 */
void EditorTile::setVisibilityBase(bool toggle)
{
  layer_base.visible = toggle;
}

/*
 * Description: Sets the enhancer layer visibility
 *
 * Input: Visibility toggle
 */
void EditorTile::setVisibilityEnhancer(bool toggle)
{
  layer_enhancer.visible = toggle;
}

/*
 * Description: Sets the grid visibility
 *
 * Input: Visibility toggle
 */
void EditorTile::setVisibilityGrid(bool toggle)
{
  visible_grid = toggle;
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
