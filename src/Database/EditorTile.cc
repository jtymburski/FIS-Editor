/*******************************************************************************
 * Class Name: EditorTile
 * Date Created: January 5, 2015
 * Inheritance: QGraphicsItem
 * Description: A tile representation in a single map
 ******************************************************************************/
#include "Database/EditorTile.h"
//#include <QDebug>

/* Constant Implementation - see header file for descriptions */
const uint8_t EditorTile::kLOWER_COUNT_MAX = 5;
const uint8_t EditorTile::kMAX_ITEMS = 10;
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
          : QGraphicsItem()
{
  setAcceptHoverEvents(true);

  /* Class control */
  hovered = false;
  tile.setStatus(Tile::ACTIVE);
  visible_events = false;
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
  for(uint8_t i = 0; i < Helpers::getRenderDepth(); i++)
    ios.push_back(temp);
  items.push_back(temp);
  for(uint8_t i = 0; i < Helpers::getRenderDepth(); i++)
    npcs.push_back(temp);
  for(uint8_t i = 0; i < Helpers::getRenderDepth(); i++)
    persons.push_back(temp);
  for(uint8_t i = 0; i < Helpers::getRenderDepth(); i++)
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
  /* Delete events */
  unsetEventEnter();
  unsetEventExit();

  /* Sprite layers */
  layer_base.sprite = NULL;
  layer_enhancer.sprite = NULL;
  layers_lower.clear();
  layers_upper.clear();

  /* Things */
  ios.clear();
  items.clear();
  npcs.clear();
  persons.clear();
  things.clear();

  /* Tile itself */
  tile.clear();
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: The copy function that is called by any copying methods in the
 *              class. Utilized by the copy constructor and the copy operator.
 *
 * Inputs: const EditorTile &source - the reference tile class
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

  /* Copy events */
  setEventEnter(source.set_enter);
  setEventExit(source.set_exit);

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

  // TODO: ADD THING, PERSON, NPC, ITEM, AND IO. No, handled in map.
}

/*
 * Description: Returns if there is a valid hover IO, the active layer is
 *              a IO layer, and the placing pen is a IO placement.
 *
 * Inputs: none
 * Output: bool - true if the hover IO should be shown
 */
bool EditorTile::isHoverIO()
{
  if(hover_info->hover_tile != NULL && hover_info->active_io != NULL &&
     hover_info->active_cursor == EditorEnumDb::BASIC &&
     hover_info->active_layer == EditorEnumDb::IO &&
     !hover_info->path_edit_mode)
  {
    return true;
  }
  return false;
}

/*
 * Description: Returns if there is a valid hover item, the active layer is
 *              a item layer, and the placing pen is an item placement.
 *
 * Inputs: none
 * Output: bool - true if the hover item should be shown
 */
bool EditorTile::isHoverItem()
{
  if(hover_info->hover_tile == this && hover_info->active_item != NULL &&
     hover_info->active_cursor == EditorEnumDb::BASIC &&
     hover_info->active_layer == EditorEnumDb::ITEM &&
     !hover_info->path_edit_mode)
  {
    return true;
  }
  return false;
}

/*
 * Description: Returns if there is a valid hover move thing.
 *
 * Inputs: none
 * Output: bool - true if the hover move thing should be shown
 */
bool EditorTile::isHoverMove()
{
  if(hover_info->hover_tile != nullptr && hover_info->move_thing != nullptr &&
     hover_info->active_cursor == EditorEnumDb::MOVE &&
     !hover_info->path_edit_mode)
  {
    return true;
  }
  return false;
}

/*
 * Description: Returns if there is a valid hover npc, the active layer is
 *              a npc layer, and the placing pen is a npc placement.
 *
 * Inputs: none
 * Output: bool - true if the hover npc should be shown
 */
bool EditorTile::isHoverNPC()
{
  if(hover_info->hover_tile != NULL && hover_info->active_npc != NULL &&
     hover_info->active_cursor == EditorEnumDb::BASIC &&
     hover_info->active_layer == EditorEnumDb::NPC &&
     !hover_info->path_edit_mode)
  {
    return true;
  }
  return false;
}

/*
 * Description: Returns if there is a valid hover person, the active layer is
 *              a person layer, and the placing pen is a person placement.
 *
 * Inputs: none
 * Output: bool - true if the hover person should be shown
 */
bool EditorTile::isHoverPerson()
{
  if(hover_info->hover_tile != NULL && hover_info->active_person != NULL &&
     hover_info->active_cursor == EditorEnumDb::BASIC &&
     hover_info->active_layer == EditorEnumDb::PERSON &&
     !hover_info->path_edit_mode)
  {
    return true;
  }
  return false;
}

/*
 * Description: Returns if there is a valid hover sprite, the active layer is
 *              a sprite layer, and the placing pen is a sprite placement.
 *
 * Inputs: none
 * Output: bool - true if the hover sprite should be shown
 */
bool EditorTile::isHoverSprite()
{
  if(hovered && hover_info->active_sprite != NULL &&
     (hover_info->active_cursor == EditorEnumDb::BASIC ||
      hover_info->active_cursor == EditorEnumDb::FILL ||
      hover_info->active_cursor == EditorEnumDb::BLOCKPLACE) &&
     (hover_info->active_layer == EditorEnumDb::BASE ||
      hover_info->active_layer == EditorEnumDb::ENHANCER ||
      hover_info->active_layer == EditorEnumDb::LOWER1 ||
      hover_info->active_layer == EditorEnumDb::LOWER2 ||
      hover_info->active_layer == EditorEnumDb::LOWER3 ||
      hover_info->active_layer == EditorEnumDb::LOWER4 ||
      hover_info->active_layer == EditorEnumDb::LOWER5 ||
      hover_info->active_layer == EditorEnumDb::UPPER1 ||
      hover_info->active_layer == EditorEnumDb::UPPER2 ||
      hover_info->active_layer == EditorEnumDb::UPPER3 ||
      hover_info->active_layer == EditorEnumDb::UPPER4 ||
      hover_info->active_layer == EditorEnumDb::UPPER5) &&
     !hover_info->path_edit_mode)
  {
    return true;
  }
  return false;
}

/*
 * Description: Returns if there is a valid hover thing, the active layer is
 *              a thing layer, and the placing pen is a thing placement.
 *
 * Inputs: none
 * Output: bool - true if the hover thing should be shown
 */
bool EditorTile::isHoverThing()
{
  if(hover_info->hover_tile != NULL && hover_info->active_thing != NULL &&
     hover_info->active_cursor == EditorEnumDb::BASIC &&
     hover_info->active_layer == EditorEnumDb::THING &&
     !hover_info->path_edit_mode)
  {
    return true;
  }
  return false;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Adds the item sprite pointer, stored within the class. This is
 *              restricted by the max number of items on each tile (10).
 *
 * Inputs: EditorMapItem item - the item to add to the tile
 * Output: bool - true if the item was added
 */
bool EditorTile::addItem(EditorMapItem* item)
{
  if(items.size() < kMAX_ITEMS)
  {
    if(items.size() == 1 && items.front().thing == NULL)
    {
      items.front().thing = item;
    }
    else
    {
      TileRenderInfo temp;
      temp.sprite = NULL;
      temp.thing = item;
      temp.visible = items.front().visible;

      items.push_back(temp);
    }
    update();
    return true;
  }
  return false;
}

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

  /* Get base info, if relevant */
  if(layer_base.sprite != NULL)
    layer_string += "B(" + QString::number(layer_base.sprite->getID()) + "),";

  /* Get enhancer info, if relevant */
  if(layer_enhancer.sprite != NULL)
    layer_string += "E(" + QString::number(layer_enhancer.sprite->getID())
                 + "),";

  /* Get lower info, if relevant */
  for(int i = 0; i < layers_lower.size(); i++)
    if(layers_lower[i].sprite != NULL)
      layer_string += "L" + QString::number(i + 1) + "("
                   + QString::number(layers_lower[i].sprite->getID()) + "),";

  /* Get thing info, if relevant */
  for(int i = 0; i < things.size(); i++)
    if(things[i].thing != NULL)
      layer_string += "MT" + QString::number(i) + "("
                   + QString::number(things[i].thing->getID()) + "),";

  /* Get item info, if relevant */
  QString item_string = "MI(";
  for(int i = 0; i < items.size(); i++)
    if(items[i].thing != NULL)
      item_string += QString::number(items[i].thing->getID()) + ",";
  if(items.size() > 1 || items.front().thing != NULL)
  {
    item_string.chop(1);
    item_string +=  "),";
    layer_string += item_string;
  }

  /* Get IO info, if relevant */
  for(int i = 0; i < ios.size(); i++)
    if(ios[i].thing != NULL)
      layer_string += "MIO" + QString::number(i) + "("
                   + QString::number(ios[i].thing->getID()) + "),";

  /* Get person info, if relevant */
  for(int i = 0; i < persons.size(); i++)
    if(persons[i].thing != NULL)
      layer_string += "MP" + QString::number(i) + "("
                   + QString::number(persons[i].thing->getID()) + "),";

  /* Get npc info, if relevant */
  for(int i = 0; i < npcs.size(); i++)
    if(npcs[i].thing != NULL)
      layer_string += "MN" + QString::number(i) + "("
                   + QString::number(npcs[i].thing->getID()) + "),";

  /* Get upper info, if relevant */
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
 * Description: Returns the tile enter event set.
 *
 * Inputs: none
 * Output: Event - the enter triggered event set on the tile
 */
EditorEventSet* EditorTile::getEventEnter()
{
  return &set_enter;
}

/*
 * Description: Returns the tile exit event set.
 *
 * Inputs: none
 * Output: Event - the exit triggered event set on the tile
 */
EditorEventSet* EditorTile::getEventExit()
{
  return &set_exit;
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
 * Description: Returns the map IO pointer for the IO at the rendering
 *              level.
 *
 * Inputs: int render_level - the render level inside the tile
 * Output: EditorMapIO* - the IO at the render level
 */
EditorMapIO* EditorTile::getIO(int render_level)
{
  if(render_level >= 0 && render_level < Helpers::getRenderDepth())
    return (EditorMapIO*)ios[render_level].thing;
  return NULL;
}

/*
 * Description: Returns the stack of all IOs, up to the render depth limiter.
 *              If the indicated render depth is unset, the pointer will be
 *              NULL.
 *
 * Inputs: none
 * Output: QVector<EditorMapIO*> - the stack of IOs, corresponding to depth
 */
QVector<EditorMapIO*> EditorTile::getIOs()
{
  QVector<EditorMapIO*> stack;

  for(int i = 0; i < ios.size(); i++)
    stack.push_back((EditorMapIO*)ios[i].thing);

  return stack;
}

/*
 * Description: Returns the item in the stack on the tile with the matching
 *              base item ID. Returns NULL if not found.
 *
 * Inputs: int id - the id of the base item
 * Output: EditorMapItem* - the found item. NULL if not found.
 */
EditorMapItem* EditorTile::getItemBaseID(int id)
{
  EditorMapItem* found_item = NULL;

  for(int i = 0; i < items.size(); i++)
    if(items[i].thing->getBaseThing()->getID() == id)
      found_item = (EditorMapItem*)items[i].thing;

  return found_item;
}

/*
 * Description: Returns the item in the stack on the tile with the matching
 *              item ID. Returns NULL if not found.
 *
 * Inputs: int id - the id of the item
 * Output: EditorMapItem* - the found item. NULL if not found.
 */
EditorMapItem* EditorTile::getItemID(int id)
{
  EditorMapItem* found_item = NULL;

  for(int i = 0; i < items.size(); i++)
    if(items[i].thing->getID() == id)
      found_item = (EditorMapItem*)items[i].thing;

  return found_item;
}

/*
 * Description: Returns the map item pointer for the npc at the rendering
 *              index stack.
 *
 * Inputs: int index - the index of the item in the stack
 * Output: EditorMapItem* - the item at the index
 */
EditorMapItem* EditorTile::getItem(int index)
{
  if(index >= 0 && index < items.size())
    return (EditorMapItem*)items[index].thing;
  return NULL;
}

/*
 * Description: Returns all items on the tile.
 *
 * Inputs: none
 * Output: QVector<EditorMapItem*> - the stack of all items on the tile
 */
QVector<EditorMapItem*> EditorTile::getItems()
{
  QVector<EditorMapItem*> stack;

  for(int i = 0; i < items.size(); i++)
    stack.push_back((EditorMapItem*)items[i].thing);

  return stack;
}

/*
 * Description: Returns the map npc pointer for the npc at the rendering
 *              level.
 *
 * Inputs: int render_level - the render level inside the tile
 * Output: EditorMapNPC* - the npc at the render level
 */
EditorMapNPC* EditorTile::getNPC(int render_level)
{
  if(render_level >= 0 && render_level < Helpers::getRenderDepth())
    return (EditorMapNPC*)npcs[render_level].thing;
  return NULL;
}

/*
 * Description: Returns the stack of all NPCs, up to the render depth limiter.
 *              If the indicated render depth is unset, the pointer will be
 *              NULL.
 *
 * Inputs: none
 * Output: QVector<EditorMapNPC*> - the stack of NPCs, corresponding to depth
 */
QVector<EditorMapNPC*> EditorTile::getNPCs()
{
  QVector<EditorMapNPC*> stack;

  for(int i = 0; i < npcs.size(); i++)
    stack.push_back((EditorMapNPC*)npcs[i].thing);

  return stack;
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
    case EditorEnumDb::THING:
      return getPassabilityThing(direction);
      break;
    case EditorEnumDb::IO:
      return getPassabilityIO(direction);
      break;
    case EditorEnumDb::PERSON:
      return getPassabilityPerson(direction);
      break;
    case EditorEnumDb::NPC:
      return getPassabilityNPC(direction);
      break;
    case EditorEnumDb::ENHANCER:
    case EditorEnumDb::ITEM:
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
 * Description: Returns the passability of the io(s) in the tile. Only
 *              relevant if the render depth is 0. Otherwise, true
 *
 * Inputs: Direction direction - the direction leaving the tile
 * Output: bool - true if that direction is passable
 */
bool EditorTile::getPassabilityIO(Direction direction)
{
  EditorMapThing* io = ios[0].thing;

  if(io != NULL)
  {
    EditorTileSprite* sprite = io->getMatrix()->getSprite(
                                    getX() - io->getX(), getY() - io->getY());
    if(sprite != NULL)
      return sprite->getPassability(direction);
  }
  return true;
}

/*
 * Description: Returns the passability of the npc(s) in the tile. Only
 *              relevant if the render depth is 0. Otherwise, true
 *
 * Inputs: Direction direction - the direction leaving the tile
 * Output: bool - true if that direction is passable
 */
bool EditorTile::getPassabilityNPC(Direction direction)
{
  EditorMapThing* npc = npcs[0].thing;

  if(npc != NULL)
  {
    EditorTileSprite* sprite = npc->getMatrix()->getSprite(
                                    getX() - npc->getX(), getY() - npc->getY());
    if(sprite != NULL)
      return sprite->getPassability(direction);
  }
  return true;
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
 * Description: Returns the passability of the person(s) in the tile. Only
 *              relevant if the render depth is 0. Otherwise, true
 *
 * Inputs: Direction direction - the direction leaving the tile
 * Output: bool - true if that direction is passable
 */
bool EditorTile::getPassabilityPerson(Direction direction)
{
  EditorMapThing* person = persons[0].thing;

  if(person != NULL)
  {
    EditorTileSprite* sprite = person->getMatrix()->getSprite(
                              getX() - person->getX(), getY() - person->getY());
    if(sprite != NULL)
      return sprite->getPassability(direction);
  }
  return true;
}

/*
 * Description: Returns the passability of the thing(s) in the tile. Only
 *              relevant if the render depth is 0. Otherwise, true
 *
 * Inputs: Direction direction - the direction leaving the tile
 * Output: bool - true if that direction is passable
 */
bool EditorTile::getPassabilityThing(Direction direction)
{
  EditorMapThing* thing = things[0].thing;

  if(thing != NULL)
  {
    EditorTileSprite* sprite = thing->getMatrix()->getSprite(
                                getX() - thing->getX(), getY() - thing->getY());
    if(sprite != NULL)
      return sprite->getPassability(direction);
  }
  return true;
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
  if(things[0].visible)
    passable &= getPassability(EditorEnumDb::THING, direction);
  if(ios[0].visible)
    passable &= getPassability(EditorEnumDb::IO, direction);
  if(persons[0].visible)
    passable &= getPassability(EditorEnumDb::PERSON, direction);
  if(npcs[0].visible)
    passable &= getPassability(EditorEnumDb::NPC, direction);

  return passable;
}

/*
 * Description: Returns the map person pointer for the person at the rendering
 *              level.
 *
 * Inputs: int render_level - the render level inside the tile
 * Output: EditorMapPerson* - the person at the render level
 */
EditorMapPerson* EditorTile::getPerson(int render_level)
{
  if(render_level >= 0 && render_level < Helpers::getRenderDepth())
    return (EditorMapPerson*)persons[render_level].thing;
  return NULL;
}

/*
 * Description: Returns the stack of all persons, up to the render depth
 *              limiter. If the indicated render depth is unset, the pointer
 *              will be NULL.
 *
 * Inputs: none
 * Output: QVector<EditorMapPerson*> - the stack of persons, by depth
 */
QVector<EditorMapPerson*> EditorTile::getPersons()
{
  QVector<EditorMapPerson*> stack;

  for(int i = 0; i < persons.size(); i++)
    stack.push_back((EditorMapPerson*)persons[i].thing);

  return stack;
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
 * Description: Returns the map thing pointer for the thing at the rendering
 *              level.
 *
 * Inputs: int render_level - the render level inside the tile
 * Output: EditorMapThing* - the thing at the render level
 */
EditorMapThing* EditorTile::getThing(int render_level)
{
  if(render_level >= 0 && render_level < Helpers::getRenderDepth())
    return things[render_level].thing;
  return NULL;
}

/*
 * Description: Returns the stack of all things, up to the render depth
 *              limiter. If the indicated render depth is unset, the pointer
 *              will be NULL.
 *
 * Inputs: none
 * Output: QVector<EditorMapThing*> - the stack of things, by depth
 */
QVector<EditorMapThing*> EditorTile::getThings()
{
  QVector<EditorMapThing*> stack;

  for(int i = 0;i < things.size(); i++)
    stack.push_back((EditorMapThing*)things[i].thing);

  return stack;
}

/*
 * Description: Returns the top thing (or children) address based on the layer.
 *              If the layer is not a thing layer or nothing is found, it
 *              returns null.
 *
 * Inputs: EditorEnumDb::Layer layer - the layer to search for the thing
 * Output: EditorMapThing* - the thing reference found (instance)
 */
EditorMapThing* EditorTile::getThingTop(EditorEnumDb::Layer layer)
{
  EditorMapThing* found = nullptr;

  /* ---- THINGS ---- */
  if(layer == EditorEnumDb::THING)
  {
    for(int i = things.size() - 1; found == nullptr && i >= 0; i--)
      found = things[i].thing;
  }
  /* ---- IOS ---- */
  else if(layer == EditorEnumDb::IO)
  {
    for(int i = ios.size() - 1; found == nullptr && i >= 0; i--)
      found = ios[i].thing;
  }
  /* ---- ITEMS ---- */
  else if(layer == EditorEnumDb::ITEM)
  {
    for(int i = items.size() - 1; found == nullptr && i >= 0; i--)
      found = items[i].thing;
  }
  /* ---- PERSONS ---- */
  else if(layer == EditorEnumDb::PERSON)
  {
    for(int i = persons.size() - 1; found == nullptr && i >= 0; i--)
      found = persons[i].thing;
  }
  /* ---- NPCS ---- */
  else if(layer == EditorEnumDb::NPC)
  {
    for(int i = npcs.size() - 1; found == nullptr && i >= 0; i--)
      found = npcs[i].thing;
  }

  return found;
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
 * Description: Returns the visibility of the IO render level
 *
 * Inputs: int render_level - the render level to acquire the visibility
 * Output: bool - true if the IO layer is visible
 */
bool EditorTile::getVisibilityIO(int render_level)
{
  if(render_level >= 0 && render_level < Helpers::getRenderDepth())
    return ios[render_level].visible;
  return false;
}

/*
 * Description: Returns the visibility of the item stack
 *
 * Inputs: none
 * Output: bool - true if the item stack is visible
 */
bool EditorTile::getVisibilityItems()
{
  return items.front().visible;
}

/*
 * Description: Returns the visibility of the npc render level
 *
 * Inputs: int render_level - the render level to acquire the visibility
 * Output: bool - true if the npc layer is visible
 */
bool EditorTile::getVisibilityNPC(int render_level)
{
  if(render_level >= 0 && render_level < Helpers::getRenderDepth())
    return npcs[render_level].visible;
  return false;
}

/*
 * Description: Returns the visibility of the person render level
 *
 * Inputs: int render_level - the render level to acquire the visibility
 * Output: bool - true if the person layer is visible
 */
bool EditorTile::getVisibilityPerson(int render_level)
{
  if(render_level >= 0 && render_level < Helpers::getRenderDepth())
    return persons[render_level].visible;
  return false;
}

/*
 * Description: Returns the visibility of the thing render level
 *
 * Inputs: int render_level - the render level to acquire the visibility
 * Output: bool - true if the thing layer is visible
 */
bool EditorTile::getVisibilityThing(int render_level)
{
  if(render_level >= 0 && render_level < Helpers::getRenderDepth())
    return things[render_level].visible;
  return false;
}

/*
 * Description: Returns if the event notifiers are visible on the tile.
 *
 * Inputs: none
 * Output: bool - true if event notifiers are visible
 */
bool EditorTile::getVisibilityEvents()
{
  return visible_events;
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
 * Description: Returns true if the enter event set is a valid event and is not
 *              empty.
 *
 * Inputs: none
 * Output: bool - true if the tile enter event set is a valid event
 */
bool EditorTile::isEventEnterSet() const
{
  return !set_enter.isEmpty();
}

/*
 * Description: Returns true if the exit event set is a valid event and is not
 *              empty.
 *
 * Inputs: none
 * Output: bool - true if the tile exit event set is a valid event
 */
bool EditorTile::isEventExitSet() const
{
  return !set_exit.isEmpty();
}

/*
 * Description: Paints the tile
 *
 * Input: Required fields, mostly unused
 * Output: none
 */
void EditorTile::paint(QPainter *painter,
                       const QStyleOptionGraphicsItem*, QWidget*)
{
  int size = EditorHelpers::getTileSize();
  QRect bound(x_pos * size, y_pos * size, size, size);

  /* Determine if it's a hover thing - special state */
  int diff_x = 0;
  int diff_y = 0;
  bool hover_io = isHoverIO();
  bool hover_item = isHoverItem();
  bool hover_move = isHoverMove();
  bool hover_npc = isHoverNPC();
  bool hover_person = isHoverPerson();
  bool hover_sprite = isHoverSprite();
  bool hover_thing = isHoverThing();
  if(hover_thing || hover_io || hover_person || hover_npc || hover_move)
  {
    diff_x = x_pos - hover_info->hover_tile->getX();
    diff_y = y_pos - hover_info->hover_tile->getY();

    /* Use the differential to make sure the sprite is valid */
    if(hover_thing && hover_info->active_thing->isAllNull(diff_x, diff_y))
      hover_thing = false;
    if(hover_io && hover_info->active_io->isAllNull(diff_x, diff_y))
      hover_io = false;
    else if(hover_move && hover_info->move_thing->isAllNull(diff_x, diff_y))
      hover_move = false;
    if(hover_person && hover_info->active_person->isAllNull(diff_x, diff_y))
      hover_person = false;
    if(hover_npc && hover_info->active_npc->isAllNull(diff_x, diff_y))
      hover_npc = false;
  }

  /* Render the base */
  if(layer_base.visible &&
     (!hovered || hover_info->active_layer != EditorEnumDb::BASE ||
      hover_info->active_cursor != EditorEnumDb::ERASER))
  {
    if(hover_sprite && hover_info->active_layer == EditorEnumDb::BASE)
      hover_info->active_sprite->paint(painter, bound);
    else if(layer_base.sprite != NULL)
      layer_base.sprite->paint(painter, bound);
  }

  /* Render the enhancer */
  if(layer_enhancer.visible &&
     (!hovered || hover_info->active_layer != EditorEnumDb::ENHANCER ||
      hover_info->active_cursor != EditorEnumDb::ERASER))
  {
    if(hover_sprite && hover_info->active_layer == EditorEnumDb::ENHANCER)
      hover_info->active_sprite->paint(painter, bound);
    else if(layer_enhancer.sprite != NULL)
      layer_enhancer.sprite->paint(painter, bound);
  }

  /* Render the lower */
  for(int i = 0; i < layers_lower.size(); i++)
  {
    EditorEnumDb::Layer layer =
                           (EditorEnumDb::Layer)((int)EditorEnumDb::LOWER1 + i);

    if(layers_lower[i].visible &&
       (!hovered || hover_info->active_layer != layer ||
        hover_info->active_cursor != EditorEnumDb::ERASER))
    {
      if(hover_sprite && hover_info->active_layer == layer)
        hover_info->active_sprite->paint(painter, bound);
      else if(layers_lower[i].sprite != NULL)
        layers_lower[i].sprite->paint(painter, bound);
    }
  }

  /* Render the things (and children) */
  for(uint8_t i = 0; i < Helpers::getRenderDepth(); i++)
  {
    /* Paint the thing */
    if(things[i].visible && things[i].thing != NULL)
      things[i].thing->paint(0, painter, bound, x_pos - things[i].thing->getX(),
                             y_pos - things[i].thing->getY());

    /* Paint the io */
    if(ios[i].visible && ios[i].thing != NULL)
      ios[i].thing->paint(0, painter, bound, x_pos - ios[i].thing->getX(),
                          y_pos - ios[i].thing->getY());

    /* Paint the top item */
    if(i == 0 && items.front().visible && items.last().thing != NULL)
      items.last().thing->paint(0, painter, bound);

    /* Paint the person */
    if(persons[i].visible && persons[i].thing != NULL)
      persons[i].thing->paint(0, painter, bound,
                              x_pos - persons[i].thing->getX(),
                              y_pos - persons[i].thing->getY());

    /* Paint the npc */
    else if(npcs[i].visible && npcs[i].thing != NULL)
      npcs[i].thing->paint(0, painter, bound,
                           x_pos - npcs[i].thing->getX(),
                           y_pos - npcs[i].thing->getY());
  }

  /* If hover thing is true, render it */
  if(hover_thing)
    hover_info->active_thing->paint(painter, bound, diff_x, diff_y);
  /* If hover npc is true, render it */
  else if(hover_io)
    hover_info->active_io->paint(painter, bound, diff_x, diff_y);
  /* If hover item is true, render it */
  else if(hover_item)
    hover_info->active_item->paint(painter, bound);
  /* If hover person is true, render it */
  else if(hover_person)
    hover_info->active_person->paint(painter, bound, diff_x, diff_y);
  /* If hover npc is true, render it */
  else if(hover_npc)
    hover_info->active_npc->paint(painter, bound, diff_x, diff_y);
  else if(hover_move)
    hover_info->move_thing->paint(painter, bound, diff_x, diff_y);

  /* Render the upper */
  for(int i = 0; i < layers_upper.size(); i++)
  {
    EditorEnumDb::Layer layer =
                           (EditorEnumDb::Layer)((int)EditorEnumDb::UPPER1 + i);

    if(layers_upper[i].visible &&
       (!hovered || hover_info->active_layer != layer ||
        hover_info->active_cursor != EditorEnumDb::ERASER))
    {
      if(hover_sprite && hover_info->active_layer == layer)
        hover_info->active_sprite->paint(painter, bound);
      else if(layers_upper[i].sprite != NULL)
        layers_upper[i].sprite->paint(painter, bound);
    }
  }

  /* Render the grid */
  if(visible_grid)
  {
    QRect rect(bound.x() + 1, bound.y() + 1,
                      bound.width() - 2, bound.height() - 2);
    painter->setPen(QColor(255, 255, 255, 128));
    painter->drawRect(rect);

    /* Thing border points */
    int x1 = hover_info->selected_thing.x();
    int y1 = hover_info->selected_thing.y();
    int x2 = x1 + hover_info->selected_thing.width() - 1;
    int y2 = y1 + hover_info->selected_thing.height() - 1;

    /* Draw border */
    if(x_pos >= x1 && x_pos <= x2 && y_pos >= y1 && y_pos <= y2)
    {
      painter->setPen(QColor(255, 255, 0, 255));
      if(x_pos == x1)
        painter->drawLine(rect.x(), rect.y(),
                          rect.x(), rect.y() + rect.height());
      if(x_pos == x2)
        painter->drawLine(rect.x() + rect.width(), rect.y(),
                          rect.x() + rect.width(), rect.y() + rect.height());
      if(y_pos == y1)
        painter->drawLine(rect.x(), rect.y(),
                          rect.x() + rect.width(), rect.y());
      if(y_pos == y2)
        painter->drawLine(rect.x(), rect.y() + rect.height(),
                          rect.x() + rect.width(), rect.y() + rect.height());
    }
  }
  if(hovered)
  {
    QColor color(200, 200, 200, 128);
    /* -- HOVER THING CONTROL -- */
    if(hover_thing)
    {
      EditorMatrix* matrix = hover_info->active_thing->getMatrix();
      int depth = matrix->getRenderDepth(diff_x, diff_y);

      if(hovered_invalid || getThing(depth) != NULL)
        color = QColor(200, 0, 0, 128);
      else
        color = QColor(0, 200, 0, 128);
    }
    /* -- HOVER IO CONTROL -- */
    if(hover_io)
    {
      EditorMatrix* matrix = hover_info->active_io->getMatrix();
      int depth = matrix->getRenderDepth(diff_x, diff_y);

      if(hovered_invalid || getIO(depth) != NULL)
        color = QColor(200, 0, 0, 128);
      else
        color = QColor(0, 200, 0, 128);
    }
    /* -- HOVER ITEM CONTROL -- */
    else if(hover_item)
    {
      if(items.size() >= kMAX_ITEMS)
        color = QColor(200, 0, 0, 128);
      else
        color = QColor(0, 200, 0, 128);
    }
    /* -- HOVER PERSON CONTROL -- */
    else if(hover_person)
    {
      EditorMatrix* matrix = hover_info->active_person->getMatrix();
      int depth = matrix->getRenderDepth(diff_x, diff_y);

      if(hovered_invalid || getPerson(depth) != NULL || getNPC(depth) != NULL)
        color = QColor(200, 0, 0, 128);
      else
        color = QColor(0, 200, 0, 128);
    }
    /* -- HOVER NPC CONTROL -- */
    else if(hover_npc)
    {
      EditorMatrix* matrix = hover_info->active_npc->getMatrix();
      int depth = matrix->getRenderDepth(diff_x, diff_y);

      if(hovered_invalid || getPerson(depth) != NULL || getNPC(depth) != NULL)
        color = QColor(200, 0, 0, 128);
      else
        color = QColor(0, 200, 0, 128);
    }
    /* -- HOVER MOVE CONTROL -- */
    else if(hover_move)
    {
      EditorMapThing* ref_thing = hover_info->move_thing;
      int depth = ref_thing->getMatrix()->getRenderDepth(diff_x, diff_y);

      /* Determine type and if valid */
      bool invalid = hovered_invalid;
      if(!invalid)
      {
        ThingBase type = hover_info->move_thing->getClass();
        if(type == ThingBase::THING)
          invalid = (getThing(depth) != NULL && getThing(depth) != ref_thing);
        else if(type == ThingBase::INTERACTIVE)
          invalid = (getIO(depth) != NULL && getIO(depth) != ref_thing);
        else if(type == ThingBase::ITEM)
          invalid = (items.size() >= kMAX_ITEMS);
        else if(type == ThingBase::PERSON)
          invalid = ((getPerson(depth) != NULL &&
                      getPerson(depth) != ref_thing) || getNPC(depth) != NULL);
        else if(type == ThingBase::NPC)
          invalid = ((getNPC(depth) != NULL &&
                      getNPC(depth) != ref_thing) || getPerson(depth) != NULL);
      }

      /* Final decision */
      if(invalid)
        color = QColor(200, 0, 0, 128);
      else
        color = QColor(0, 200, 0, 128);
    }
    painter->fillRect(x_pos * size + 1, y_pos * size + 1, size - 2, size - 2,
                      color);
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

  /* Render the event notification */
  if(visible_events)
  {
    bool enter = isEventEnterSet();
    bool exit = isEventExitSet();

    if(enter || exit)
    {
      /* Draw rect */
      QColor color;
      if(enter && exit)
        color = QColor(255, 216, 1, 128);
      else if(enter)
        color = QColor(229, 103, 23, 128);
      else if(exit)
        color = QColor(23, 150, 230, 128);
      painter->fillRect(x_pos * size + 1, y_pos * size + 1, size - 2, size - 2,
                        color);
    }
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
 * Description: Sets the enter event set on the tile.
 *
 * Inputs: EditorEventSet set - the event set to connect to the enter trigger
 * Output: none
 */
void EditorTile::setEventEnter(EditorEventSet set)
{
  set_enter = set;
  update();
}

/*
 * Description: Sets the exit event set on the tile.
 *
 * Inputs: EditorEventSet set - the event set to connect to the exit trigger
 * Output: none
 */
void EditorTile::setEventExit(EditorEventSet set)
{
  set_exit = set;
  update();
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
 * Description: Sets the io sprite pointer, stored within the class.
 *
 * Inputs: EditorMapIO* io - the io to add to the tile (uses the
 *                           internal render depth and position)
 * Output: bool - true if the io is set
 */
bool EditorTile::setIO(EditorMapIO* io)
{
  if(io != NULL)
  {
    /* Determine the x, y in the matrix */
    int x = x_pos - io->getX();
    int y = y_pos - io->getY();
    if(x >= 0 && x < io->getMatrix()->getWidth() &&
       y >= 0 && y < io->getMatrix()->getHeight())
    {
      /* Determine the render level */
      int render_level = io->getMatrix()->getRenderDepth(x, y);
      if(render_level >= 0 && render_level < Helpers::getRenderDepth() &&
         !io->isAllNull(x, y))
      {
        /* Set the new npc */
        ios[render_level].thing = io;
        update();
        return true;
      }
    }
  }
  return false;
}

/*
 * Description: Sets the npc sprite pointer, stored within the class.
 *
 * Inputs: EditorMapNPC* npc - the npc to add to the tile (uses the
 *                             internal render depth and position)
 * Output: bool - true if the npc is set
 */
bool EditorTile::setNPC(EditorMapNPC* npc)
{
  if(npc != NULL)
  {
    /* Determine the x, y in the matrix */
    int x = x_pos - npc->getX();
    int y = y_pos - npc->getY();
    if(x >= 0 && x < npc->getMatrix()->getWidth() &&
       y >= 0 && y < npc->getMatrix()->getHeight())
    {
      /* Determine the render level */
      int render_level = npc->getMatrix()->getRenderDepth(x, y);
      if(render_level >= 0 && render_level < Helpers::getRenderDepth() &&
         !npc->isAllNull(x, y))
      {
        /* Set the new npc */
        npcs[render_level].thing = npc;
        update();
        return true;
      }
    }
  }
  return false;
}

/*
 * Description: Sets the passability of the passed in layer in all directions.
 *
 * Inputs: EditorEnumDb::Layer layer - the layer to update the passability for
 *         bool passable - is that layer passable??
 * Output: none
 */
void EditorTile::setPassability(EditorEnumDb::Layer layer, bool passable)
{
  setPassability(layer, Direction::NORTH, passable);
  setPassability(layer, Direction::EAST, passable);
  setPassability(layer, Direction::SOUTH, passable);
  setPassability(layer, Direction::WEST, passable);
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

/*
 * Description: Takes a number between 0 and 15 and extracts the directional
 *              passability for the given layer. Then, it sets it for the
 *              passed in layer. This number is the binary realization in base
 *              10 of the 4 passability directions.
 *
 * Inputs: EditorEnumDb::Layer layer - the layer to check passability on
 *         int - the base 10 integer representation of passability
 * Output: bool - returns if the layer was in range
 */
bool EditorTile::setPassabilityNum(EditorEnumDb::Layer layer, int num_ref)
{
  if(layer == EditorEnumDb::BASE || layer == EditorEnumDb::LOWER1 ||
     layer == EditorEnumDb::LOWER2 || layer == EditorEnumDb::LOWER3 ||
     layer == EditorEnumDb::LOWER4 || layer == EditorEnumDb::LOWER5)
  {
    /* Determine the passability */
    bool north, east, south, west;
    EditorHelpers::getPassability(EditorHelpers::getPassabilityStr(num_ref),
                                  north, east, south, west);

    /* Set the passability */
    setPassability(layer, Direction::NORTH, north);
    setPassability(layer, Direction::EAST, east);
    setPassability(layer, Direction::SOUTH, south);
    setPassability(layer, Direction::WEST, west);

    return true;
  }
  return false;
}

/*
 * Description: Sets the person sprite pointer, stored within the class.
 *
 * Inputs: EditorMapPerson* person - the person to add to the tile (uses the
 *                                   internal render depth and position)
 * Output: bool - true if the person is set
 */
bool EditorTile::setPerson(EditorMapPerson* person)
{
  if(person != NULL)
  {
    /* Determine the x, y in the matrix */
    int x =  x_pos - person->getX();
    int y = y_pos - person->getY();
    if(x >= 0 && x < person->getMatrix()->getWidth() &&
       y >= 0 && y < person->getMatrix()->getHeight())
    {
      /* Determine the render level */
      int render_level = person->getMatrix()->getRenderDepth(x, y);
      if(render_level >= 0 && render_level < Helpers::getRenderDepth() &&
         !person->isAllNull(x, y))
      {
        /* Set the new person */
        persons[render_level].thing = person;
        update();
        return true;
      }
    }
  }
  return false;
}

/*
 * Description: Sets the thing sprite pointer, stored within the class.
 *
 * Inputs: EditorMapThing* thing - the thing to add to the tile (uses the
 *                                 internal render depth and position)
 * Output: bool - true if the thing is set
 */
bool EditorTile::setThing(EditorMapThing* thing)
{
  if(thing != NULL)
  {
    /* Determine the x, y in the matrix */
    int x =  x_pos - thing->getX();
    int y = y_pos - thing->getY();
    if(x >= 0 && x < thing->getMatrix()->getWidth() &&
       y >= 0 && y < thing->getMatrix()->getHeight())
    {
      /* Determine the render level */
      int render_level = thing->getMatrix()->getRenderDepth(x, y);
      if(render_level >= 0 && render_level < Helpers::getRenderDepth() &&
         !thing->isAllNull(x, y))
      {
        /* Set the new thing */
        things[render_level].thing = thing;
        update();
        return true;
      }
    }
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
  else if(layer == EditorEnumDb::THING)
    setVisibilityThing(visible);
  else if(layer == EditorEnumDb::IO)
    setVisibilityIO(visible);
  else if(layer == EditorEnumDb::ITEM)
    setVisibilityItems(visible);
  else if(layer == EditorEnumDb::PERSON)
    setVisibilityPerson(visible);
  else if(layer == EditorEnumDb::NPC)
    setVisibilityNPC(visible);
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

/* Description: Sets the event notifier visibility.
 *
 * Inputs: bool toggle - notifier visibility
 * Output: none
 */
void EditorTile::setVisibilityEvents(bool toggle)
{
  visible_events = toggle;
  update();
}

/*
 * Description: Sets the visibility of all layers of the ios.
 *
 * Inputs: bool visible - true if the io layers are visible
 * Output: none
 */
void EditorTile::setVisibilityIO(bool visible)
{
  for(int i = 0; i < ios.size(); i++)
    ios[i].visible = visible;
  update();
}

/*
 * Description: Sets the visibility of the render level of the io in tile.
 *
 * Inputs: int render_level - the render level to modify the visibility
 *         bool visible - true if the render level should be visible
 * Output: bool - true if the visibility was set
 */
bool EditorTile::setVisibilityIO(int render_level, bool visible)
{
  if(render_level >= 0 && render_level < Helpers::getRenderDepth())
  {
    ios[render_level].visible = visible;
    return true;
  }
  return false;
}

/*
 * Description: Sets the grid visibility
 *
 * Input: Visibility toggle
 * Output: none
 */
void EditorTile::setVisibilityGrid(bool toggle)
{
  visible_grid = toggle;
  update();
}

/*
 * Description: Sets the visibility of the stack of npcs.
 *
 * Inputs: bool visible - true if the item stack is visible
 * Output: none
 */
void EditorTile::setVisibilityItems(bool visible)
{
  for(int i = 0; i < items.size(); i++)
    items[i].visible = visible;
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
 * Description: Sets the visibility of all layers of the npcs.
 *
 * Inputs: bool visible - true if the npc layers are visible
 * Output: none
 */
void EditorTile::setVisibilityNPC(bool visible)
{
  for(int i = 0; i < npcs.size(); i++)
    npcs[i].visible = visible;
  update();
}

/*
 * Description: Sets the visibility of the render level of the npc in tile.
 *
 * Inputs: int render_level - the render level to modify the visibility
 *         bool visible - true if the render level should be visible
 * Output: bool - true if the visibility was set
 */
bool EditorTile::setVisibilityNPC(int render_level, bool visible)
{
  if(render_level >= 0 && render_level < Helpers::getRenderDepth())
  {
    npcs[render_level].visible = visible;
    return true;
  }
  return false;
}

/*
 * Description: Sets the passability visibility
 *
 * Input: bool toggle - Visibility toggle
 * Output: none
 */
void EditorTile::setVisibilityPass(bool toggle)
{
  visible_passability = toggle;
  update();
}

/*
 * Description: Sets the visibility of all layers of the persons.
 *
 * Inputs: bool visible - true if the person layers are visible
 * Output: none
 */
void EditorTile::setVisibilityPerson(bool visible)
{
  for(int i = 0; i < persons.size(); i++)
    persons[i].visible = visible;
  update();
}

/*
 * Description: Sets the visibility of the render level of the person in tile.
 *
 * Inputs: int render_level - the render level to modify the visibility
 *         bool visible - true if the render level should be visible
 * Output: bool - true if the visibility was set
 */
bool EditorTile::setVisibilityPerson(int render_level, bool visible)
{
  if(render_level >= 0 && render_level < Helpers::getRenderDepth())
  {
    persons[render_level].visible = visible;
    return true;
  }
  return false;
}

/*
 * Description: Sets the visibility of all layers of the thing.
 *
 * Inputs: bool visible - true if the thing layers are visible
 * Output: none
 */
void EditorTile::setVisibilityThing(bool visible)
{
  for(int i = 0; i < things.size(); i++)
    things[i].visible = visible;
  update();
}

/*
 * Description: Sets the visibility of the render level of the things in tile.
 *
 * Inputs: int render_level - the render level to modify the visibility
 *         bool visible - true if the render level should be visible
 * Output: bool - true if the visibility was set
 */
bool EditorTile::setVisibilityThing(int render_level, bool visible)
{
  if(render_level >= 0 && render_level < Helpers::getRenderDepth())
  {
    things[render_level].visible = visible;
    return true;
  }
  return false;
}

/*
 * Description: Sets the visibility to mirror all the information in said
 *              reference tile.
 *
 * Inputs: EditorTile* ref - the reference to extract the information from
 * Output: none
 */
void EditorTile::setVisibilityTile(EditorTile* ref)
{
  if(ref != nullptr)
  {
    /* Tile based visibility */
    layer_base.visible = ref->layer_base.visible;
    layer_enhancer.visible = ref->layer_enhancer.visible;
    for(int i = 0; i < layers_lower.size(); i++)
      layers_lower[i].visible = ref->layers_lower.front().visible;
    for(int i = 0; i < layers_upper.size(); i++)
      layers_upper[i].visible = ref->layers_upper.front().visible;
    for(int i = 0; i < ios.size(); i++)
      ios[i].visible = ref->ios.front().visible;
    for(int i = 0; i < items.size(); i++)
      items[i].visible = ref->items.front().visible;
    for(int i = 0; i < npcs.size(); i++)
      npcs[i].visible = ref->npcs.front().visible;
    for(int i = 0; i < persons.size(); i++)
      persons[i].visible = ref->persons.front().visible;
    for(int i = 0; i < things.size(); i++)
      things[i].visible = ref->things.front().visible;

    /* Interface based visibility */
    visible_events = ref->visible_events;
    visible_grid = ref->visible_grid;
    visible_passability = ref->visible_passability;

    /* Finally update */
    update();
  }
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

/*
 * Description: Unsets the enter event on the tile and replaces it with a blank
 *              version of the struct.
 *
 * Inputs: none
 * Output: none
 */
void EditorTile::unsetEventEnter()
{
  set_enter.clear();
}

/*
 * Description: Unsets the exit event on the tile and replaces it with a blank
 *              version of the struct.
 *
 * Inputs: none
 * Output: none
 */
void EditorTile::unsetEventExit()
{
  set_exit.clear();
}

/*
 * Description: Unsets the IO pointer in the tile. Searches through all
 *              render levels and removes it if the pointer is found.
 *
 * Inputs: EditorMapIO* io - the io to remove from the tile
 * Output: bool - true if the io was found and removed
 */
bool EditorTile::unsetIO(EditorMapIO* io)
{
  for(int i = 0; i < ios.size(); i++)
  {
    if(ios[i].thing == io)
    {
      ios[i].thing = NULL;
      update();
      return true;
    }
  }

  return false;
}

/*
 * Description: Unsets the IO pointer in the tile at the render level.
 *
 * Inputs: int render_level - the render depth to remove the IO from
 * Output: bool - true if a IO was removed at the passed depth
 */
bool EditorTile::unsetIO(int render_level)
{
  if(render_level >= 0 && render_level < ios.size())
  {
    if(ios[render_level].thing != NULL)
    {
      ios[render_level].thing = NULL;
      update();
      return true;
    }
  }

  return false;
}

/*
 * Description: Unsets the IOs in all render levels.
 *
 * Inputs: none
 * Output: none
 */
void EditorTile::unsetIOs()
{
  for(int i = 0; i < ios.size(); i++)
    ios[i].thing = NULL;
  update();
}

/*
 * Description: Unsets the item pointer in the tile. Searches through all
 *              the items in the stack and removes it if the pointer is found.
 *
 * Inputs: EditorMapItem* item - the item to remove from the tile
 * Output: bool - true if the item was found and removed
 */
bool EditorTile::unsetItem(EditorMapItem* item)
{
  for(int i = 0; i < items.size(); i++)
  {
    if(items[i].thing == item)
    {
      if(items.size() > 1)
        items.removeAt(i);
      else
        items[i].thing = NULL;
      update();
      return true;
    }
  }
  return false;
}

/*
 * Description: Unsets the item pointer in the tile at the render level.
 *
 * Inputs: int index - the item index in the stack
 * Output: bool - true if a item was removed at the passed index
 */
bool EditorTile::unsetItem(int index)
{
  if(index >= 0 && index < items.size())
  {
    if(items[index].thing != NULL)
    {
      if(items.size() > 1)
        items.removeAt(index);
      else
        items[index].thing = NULL;
      update();
      return true;
    }
  }

  return false;
}

/*
 * Description: Unsets all the items in the stack.
 *
 * Inputs: none
 * Output: none
 */
void EditorTile::unsetItems()
{
  while(items.size() > 1)
    items.removeLast();
  items.front().thing = NULL;
  update();
}

/*
 * Description: Unsets the npc pointer in the tile. Searches through all
 *              render levels and removes it if the pointer is found.
 *
 * Inputs: EditorMapNPC* npc - the npc to remove from the tile
 * Output: bool - true if the npc was found and removed
 */
bool EditorTile::unsetNPC(EditorMapNPC* npc)
{
  for(int i = 0; i < npcs.size(); i++)
  {
    if(npcs[i].thing == npc)
    {
      npcs[i].thing = NULL;
      update();
      return true;
    }
  }

  return false;
}

/*
 * Description: Unsets the npc pointer in the tile at the render level.
 *
 * Inputs: int render_level - the render depth to remove the npc from
 * Output: bool - true if a npc was removed at the passed depth
 */
bool EditorTile::unsetNPC(int render_level)
{
  if(render_level >= 0 && render_level < npcs.size())
  {
    if(npcs[render_level].thing != NULL)
    {
      npcs[render_level].thing = NULL;
      update();
      return true;
    }
  }

  return false;
}

/*
 * Description: Unsets the npcs in all render levels.
 *
 * Inputs: none
 * Output: none
 */
void EditorTile::unsetNPCs()
{
  for(int i = 0; i < npcs.size(); i++)
    npcs[i].thing = NULL;
  update();
}

/*
 * Description: Unsets the person pointer in the tile. Searches through all
 *              render levels and removes it if the pointer is found.
 *
 * Inputs: EditorMapPerson* person - the person to remove from the tile
 * Output: bool - true if the person was found and removed
 */
bool EditorTile::unsetPerson(EditorMapPerson* person)
{
  for(int i = 0; i < persons.size(); i++)
  {
    if(persons[i].thing == person)
    {
      persons[i].thing = NULL;
      update();
      return true;
    }
  }

  return false;
}

/*
 * Description: Unsets the person pointer in the tile at the render level.
 *
 * Inputs: int render_level - the render depth to remove the person from
 * Output: bool - true if a person was removed at the passed depth
 */
bool EditorTile::unsetPerson(int render_level)
{
  if(render_level >= 0 && render_level < persons.size())
  {
    if(persons[render_level].thing != NULL)
    {
      persons[render_level].thing = NULL;
      update();
      return true;
    }
  }

  return false;
}

/*
 * Description: Unsets the persons in all render levels.
 *
 * Inputs: none
 * Output: none
 */
void EditorTile::unsetPersons()
{
  for(int i = 0; i < persons.size(); i++)
    persons[i].thing = NULL;
  update();
}

/*
 * Description: Unsets the thing pointer in the tile. Searches through all
 *              render levels and removes it if the pointer is found.
 *
 * Inputs: EditorMapThing* thing - the thing to remove from the tile
 * Output: bool - true if the thing was found and removed
 */
bool EditorTile::unsetThing(EditorMapThing* thing)
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

/*
 * Description: Unsets the thing pointer in the tile at the render level.
 *
 * Inputs: int render_level - the render depth to remove the thing from
 * Output: bool - true if a thing was removed at the passed depth
 */
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

/*
 * Description: Unsets the things in all render levels.
 *
 * Inputs: none
 * Output: none
 */
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
