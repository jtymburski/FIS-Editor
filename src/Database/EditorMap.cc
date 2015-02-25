/*******************************************************************************
 * Class Name: EditorMap
 * Date Created: December 27, 2014
 * Inheritance: QObject, EditorTemplate
 * Description: The map interface to connect and edit in the editor
 ******************************************************************************/
#include "Database/EditorMap.h"
#include <QDebug>

/* Constant Implementation - see header file for descriptions */
const int EditorMap::kBASE_ID_IOS = 30000;
const int EditorMap::kBASE_ID_ITEMS = 40000;
const int EditorMap::kBASE_ID_PERSON = 0;
const int EditorMap::kBASE_ID_NPC = 10000;
const int EditorMap::kBASE_ID_THING = 20000;
const int EditorMap::kUNSET_ID = -1;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: The set-up constructor
 *
 * Inputs: none
 */
EditorMap::EditorMap() : QObject()
{
  active_submap = NULL;
  id = kUNSET_ID;
  name = "";
  tile_icons = NULL;

  clearHoverInfo();
}

/*
 * Description: Non-blank constructor with id and name.
 *
 * Inputs: int id - the id of the editor map
 *         QString name - the string name of the map
 *         int width - width of base map in tiles
 *         int height - height of base map in tiles
 *         TileIcons* icons - the rendering tile icons
 */
EditorMap::EditorMap(int id, QString name, int width, int height,
                     TileIcons* icons)
         : EditorMap()
{
  setID(id);
  setName(name);
  setTileIcons(icons);

  /* Set up base map, if applicable */
  if(width > 0 && height > 0)
    setMap(0, "MAIN", width, height);
}

/*
 * Description: The copy constructor for copying all data from a source copy
 *              class.
 *
 * Inputs: const EditorMap &source - the source to copy the data from
 */
EditorMap::EditorMap(const EditorMap &source) : EditorMap()
{
  copySelf(source);
}

/*
 * Description: Desctructor function
 */
EditorMap::~EditorMap()
{
  unsetMaps();
  unsetSprites();
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/* Attempts to add thing to the current sub-map */
// TODO: Comment
bool EditorMap::addThing(EditorThing* thing)
{
  int x = thing->getX();
  int y = thing->getY();
  int w = thing->getMatrix()->getWidth();
  int h = thing->getMatrix()->getHeight();

  /* Check if thing can be placed */
  bool valid = false;
  if(x >= 0 && y >= 0 && (x+w) <= active_submap->tiles.size() &&
     (y+h) <= active_submap->tiles[x].size())
  {
    valid = true;

    /* Check each tile */
    for(int i = 0; i < w; i++)
      for(int j = 0; j < h; j++)
        if(active_submap->tiles[x+i][y+j]->getThing(
                        thing->getMatrix()->getRenderDepth(i, j)) != NULL)
          valid = false;
  }

  /* If valid, place thing */
  if(valid)
  {
    /* Add the thing to tiles */
    for(int i = x; i < (w+x); i++)
      for(int j = y; j < (h+y); j++)
        active_submap->tiles[i][j]->setThing(thing);
  }

  return valid;
}

/*
 * Description: Adds tile sprite data to the file handler from the selected sub
 *              map index in the layer.
 *
 * Inputs: FileHandler* fh - the file handling control pointer
 *         int index - index of the sub-map
 *         EditorEnumDb::Layer layer - the layer to insert the data into
 * Output: none
 */
void EditorMap::addTileSpriteData(FileHandler* fh, int index,
                                  EditorEnumDb::Layer layer)
{
  addTileSpriteData(fh, sub_maps[index], layer);
}

/*
 * Description: Adds tile sprite data to the file handler from the selected sub
 *              map in the layer.
 *
 * Inputs: FileHandler* fh - the file handling control pointer
 *         SubMapInfo* map - the sub map reference pointer
 *         EditorEnumDb::Layer layer - the layer to insert the data into
 * Output: none
 */
void EditorMap::addTileSpriteData(FileHandler* fh, SubMapInfo* map,
                                  EditorEnumDb::Layer layer)
{
  QList<QPoint> empty_set;
  int max_pass = EditorHelpers::getPassabilityNum(true, true, true, true);
  QString sep = ",";

  for(int i = 0; i < sprites.size(); i++)
  {
    QList<QList<QPoint>> coords;
    for(int j = 0; j <= max_pass; j++)
      coords.push_back(empty_set);


    for(int j = 0; j < map->tiles.size(); j++)
    {
      for(int k = 0; k < map->tiles[j].size(); k++)
      {
        if(map->tiles[j][k]->getSprite(layer) == sprites[i])
        {
          int pass = map->tiles[j][k]->getPassabilityNum(layer);
          coords[pass].push_back(QPoint(j, k));
        }
      }
    }

    /* Parse all coords */
    for(int j = 0; j < coords.size(); j++)
    {
      if(coords[j].size() > 0)
      {
        QList<QPair<QString,QString>> str_set =
                                     EditorHelpers::rectilinearSplit(coords[j]);

        QString x_set = "";
        QString y_set = "";
        for(int k = 0; k < str_set.size(); k++)
        {
          x_set += str_set[k].first + sep;
          y_set += str_set[k].second + sep;
        }
        x_set.chop(1);
        y_set.chop(1);

        /* Print the elements */
        fh->writeXmlElement("x", "index", x_set.toStdString());
        fh->writeXmlElement("y", "index", y_set.toStdString());
        fh->writeXmlData("sprite_id",
                         QString::number(sprites[i]->getID()).toStdString());
        if(layer == EditorEnumDb::BASE || layer == EditorEnumDb::LOWER1 ||
           layer == EditorEnumDb::LOWER2 || layer == EditorEnumDb::LOWER3 ||
           layer == EditorEnumDb::LOWER4 || layer == EditorEnumDb::LOWER5)
        {
          QString passability = EditorHelpers::getPassabilityStr(j);
          if(!passability.isEmpty())
            fh->writeXmlData("passability", passability.toStdString());
        }
        fh->writeXmlElementEnd();
        fh->writeXmlElementEnd();
      }
    }
  }
}

/*
 * Description: Copies all data from source editor thing to this editor
 *              thing.
 *
 * Inputs: EditorThing &source - the source to copy from
 * Output: none
 */
void EditorMap::copySelf(const EditorMap &source)
{
  /* Add const values */
  name = source.name;

  /* Add sprites */
  for(int i = 0; i < source.sprites.size(); i++)
    sprites.push_back(new EditorSprite(*source.sprites[i]));

  /* Add sub-maps */
  for(int i = 0; i < source.sub_maps.size(); i++)
  {
    /* Copy the initial tile */
    sub_maps.push_back(new SubMapInfo);
    sub_maps.last()->id = source.sub_maps[i]->id;
    sub_maps.last()->name = source.sub_maps[i]->name;
    for(int j = 0; j < source.sub_maps[i]->tiles.size(); j++)
    {
      QVector<EditorTile*> row;
      for(int k = 0; k < source.sub_maps[i]->tiles[j].size(); k++)
      {
        row.push_back(new EditorTile(*source.sub_maps[i]->tiles[j][k]));
        row.last()->setHoverInfo(&active_info);
      }
      sub_maps.last()->tiles.push_back(row);
    }

    /* Fix the tile pointers to the new sprite sets */
    for(int j = 0; j < sub_maps.last()->tiles.size(); j++)
    {
      for(int k = 0; k < sub_maps.last()->tiles[j].size(); k++)
      {
        for(int m = 0; m < EditorEnumDb::NO_LAYER; m++)
        {
          EditorTile* tile = sub_maps.last()->tiles[j][k];
          EditorSprite* sprite = tile->getSprite((EditorEnumDb::Layer)m);
          if(sprite != NULL)
            tile->place((EditorEnumDb::Layer)m, getSprite(sprite->getID()));
        }
      }
    }
  }
}

/*
 * Description: Loads the sub-map info from the xml data and index of the data
 *              stack.
 *
 * Inputs: SubMapInfo* map - the sub-map info to load data into
 *         XmlData data - the current xml data to load in
 *         int index - the offset index into the data stack
 * Output: none
 */
void EditorMap::loadSubMap(SubMapInfo* map, XmlData data, int index)
{
  QString element = QString::fromStdString(data.getElement(index));
  if(element == "name")
  {
    map->name = QString::fromStdString(data.getDataString());
  }
  else if(element == "width")
  {
    setMap(map->id, map->name, data.getDataInteger(),map->tiles.front().size());
  }
  else if(element == "height")
  {
    setMap(map->id, map->name, map->tiles.size(), data.getDataInteger());
  }
  else if((element == "base" || element == "enhancer" ||
           element == "lower" || element == "upper") &&
          data.getElement(index + 1) == "x" &&
          data.getElement(index + 2) == "y")
  {
    /* Determine the layer */
    EditorEnumDb::Layer layer;
    if(element == "base")
      layer = EditorEnumDb::BASE;
    else if(element == "enhancer")
      layer = EditorEnumDb::ENHANCER;
    else if(element == "lower")
      layer = (EditorEnumDb::Layer)(static_cast<int>(EditorEnumDb::LOWER1) +
                       QString::fromStdString(data.getKeyValue(index)).toInt());
    else if(element == "upper")
      layer = (EditorEnumDb::Layer)(static_cast<int>(EditorEnumDb::UPPER1) +
                       QString::fromStdString(data.getKeyValue(index)).toInt());

    /* Determine the sprite, if applicable. or passability, if applicable */
    int last = data.getNumElements() - 1;
    bool pass_north = false;
    bool pass_east = false;
    bool pass_south = false;
    bool pass_west = false;
    EditorSprite* sprite = NULL;
    if(data.getElement(last) == "sprite_id")
      sprite = getSprite(QString::fromStdString(data.getDataString()).toInt());
    else if(data.getElement(last) == "passability")
      EditorHelpers::getPassability(
                       QString::fromStdString(data.getDataString()), pass_north,
                       pass_east, pass_south, pass_west);

    /* Parse through x and y index */
    int parse = 0;
    QStringList x_list = QString::fromStdString(data.getKeyValue(index + 1))
                          .split(",");
    QStringList y_list = QString::fromStdString(data.getKeyValue(index + 2))
                          .split(",");
    while(parse < x_list.size() && parse < y_list.size())
    {
      QStringList x_pair = x_list[parse].split("-");
      QStringList y_pair = y_list[parse].split("-");

      /* Loop through all tiles */
      for(int i = x_pair.front().toInt(); i <= x_pair.back().toInt(); i++)
      {
        for(int j = y_pair.front().toInt(); j <= y_pair.back().toInt(); j++)
        {
          if(map->tiles.size() > i && map->tiles[i].size() > j)
          {
            EditorTile* tile = map->tiles[i][j];
            if(data.getElement(last) == "sprite_id")
              tile->place(layer, sprite, true);
            else if(data.getElement(last) == "passability")
            {
              tile->setPassability(layer, Direction::NORTH, pass_north);
              tile->setPassability(layer, Direction::EAST, pass_east);
              tile->setPassability(layer, Direction::SOUTH, pass_south);
              tile->setPassability(layer, Direction::WEST, pass_west);
            }
          }
        }
      }

      /* Increment */
      parse++;
    }
  }
}

/* Recursively erase all similar adjoining tiles */
// TODO: Comment
void EditorMap::recursiveErase(int x, int y, EditorEnumDb::Layer layer,
                               EditorSprite* target, SubMapInfo* map)
{
  if(x >= 0 && y >= 0 && x < map->tiles.size() && y < map->tiles[x].size() &&
     map->tiles[x][y]->getSprite(layer) == target && target != NULL)
  {
    /* Un-place sprite */
    map->tiles[x][y]->unplace(layer);

    /* Recursively proceed */
    recursiveErase(x + 1, y, layer, target, map);
    recursiveErase(x - 1, y, layer, target, map);
    recursiveErase(x, y + 1, layer, target, map);
    recursiveErase(x, y - 1, layer, target, map);
  }
}

/*
 * Description: Recursively fills all of the similar adjoining tiles with the
 *              selected sprite
 *
 * Inputs: x and y positions for the tile, and target(the tile's ID)
 *         and replacement (New sprite) id numbers
 */
void EditorMap::recursiveFill(int x, int y, EditorEnumDb::Layer layer,
                            EditorSprite* target, SubMapInfo* map)
{
  if(x >= 0 && y >= 0 && x < map->tiles.size() && y < map->tiles[x].size() &&
     map->tiles[x][y]->getSprite(layer) == target)
  {
    /* Place sprite */
    map->tiles[x][y]->place(layer, active_info.active_sprite);

    /* Recursively proceed */
    recursiveFill(x + 1, y, layer, target, map);
    recursiveFill(x - 1, y, layer, target, map);
    recursiveFill(x, y + 1, layer, target, map);
    recursiveFill(x, y - 1, layer, target, map);
  }
}

/*
 * Description: Saves the sub map pointer passed in to the file handler. If
 *              game only, some parts are skipped. If first, it makes the
 *              sub-map a main.
 *
 * Inputs: FileHandler* fh - the file handling control pointer
 *         bool game_only - only game applicable data
 *         SubMapInfo* map - the sub map struct
 *         bool first - only the first used
 * Output: none
 */
void EditorMap::saveSubMap(FileHandler* fh, bool game_only,
                           SubMapInfo* map, bool first)
{
  /* Initial element */
  if(first)
    fh->writeXmlElement("main");
  else
    fh->writeXmlElement("section", "id", map->id);

  /* Add size and name */
  if(!game_only)
    fh->writeXmlData("name", map->name.toStdString());
  fh->writeXmlData("width", map->tiles.size());
  fh->writeXmlData("height", map->tiles.front().size());

  /* Add base tiles */
  fh->writeXmlElement("base");
  addTileSpriteData(fh, map, EditorEnumDb::BASE);
  fh->writeXmlElementEnd();

  /* Add enhancer tiles */
  fh->writeXmlElement("enhancer");
  addTileSpriteData(fh, map, EditorEnumDb::ENHANCER);
  fh->writeXmlElementEnd();

  /* Add lower tiles */
  fh->writeXmlElement("lower", "index", "0");
  addTileSpriteData(fh, map, EditorEnumDb::LOWER1);
  fh->writeXmlElementEnd();
  fh->writeXmlElement("lower", "index", "1");
  addTileSpriteData(fh, map, EditorEnumDb::LOWER2);
  fh->writeXmlElementEnd();
  fh->writeXmlElement("lower", "index", "2");
  addTileSpriteData(fh, map, EditorEnumDb::LOWER3);
  fh->writeXmlElementEnd();
  fh->writeXmlElement("lower", "index", "3");
  addTileSpriteData(fh, map, EditorEnumDb::LOWER4);
  fh->writeXmlElementEnd();
  fh->writeXmlElement("lower", "index", "4");
  addTileSpriteData(fh, map, EditorEnumDb::LOWER5);
  fh->writeXmlElementEnd();

  /* Add upper tiles */
  fh->writeXmlElement("upper", "index", "0");
  addTileSpriteData(fh, map, EditorEnumDb::UPPER1);
  fh->writeXmlElementEnd();
  fh->writeXmlElement("upper", "index", "1");
  addTileSpriteData(fh, map, EditorEnumDb::UPPER2);
  fh->writeXmlElementEnd();
  fh->writeXmlElement("upper", "index", "2");
  addTileSpriteData(fh, map, EditorEnumDb::UPPER3);
  fh->writeXmlElementEnd();
  fh->writeXmlElement("upper", "index", "3");
  addTileSpriteData(fh, map, EditorEnumDb::UPPER4);
  fh->writeXmlElementEnd();
  fh->writeXmlElement("upper", "index", "4");
  addTileSpriteData(fh, map, EditorEnumDb::UPPER5);
  fh->writeXmlElementEnd();

  /* End element */
  fh->writeXmlElementEnd();
}

/* Updates the tiles that contain the hover information struct */
// TODO: Comment
bool EditorMap::updateHoverThing(bool unset)
{
  /* Pre-checks to determine if this is valid to execute */
  if(active_submap != NULL &&
     active_info.active_cursor == EditorEnumDb::BASIC &&
     active_info.active_layer == EditorEnumDb::THING &&
     active_info.active_thing != NULL && active_info.hover_tile != NULL)
  {
    SubMapInfo* map = active_submap;
    EditorMatrix* ref = active_info.active_thing->getMatrix();
    int hover_x = active_info.hover_tile->getX();
    int hover_y = active_info.hover_tile->getY();
    int hover_w = hover_x + ref->getWidth();
    int hover_h = hover_y + ref->getHeight();

    /* If unset, the hover is being removed from the tiles */
    if(unset)
    {
      /* Go through and unset hover on all */
      for(int i = hover_x; (i < hover_w && i < map->tiles.size()); i++)
      {
        for(int j = hover_y; (j < hover_h && j < map->tiles[i].size()); j++)
        {
          map->tiles[i][j]->setHover(false);
        }
      }

      /* Re-set on base hover tile */
      active_info.hover_tile->setHover(true);
    }
    /* Otherwise, it is being added */
    else
    {
      /* Check if it would be valid */
      bool invalid = false;
      if((hover_w > map->tiles.size()) ||
         (hover_h > map->tiles[hover_x].size()))
      {
        invalid = true;
      }

      /* Go through and set hover on all */
      for(int i = hover_x; (i < hover_w && i < map->tiles.size()); i++)
      {
        for(int j = hover_y; (j < hover_h &&
                              j < map->tiles[i].size()); j++)
        {
          map->tiles[i][j]->setHover(true, invalid);
        }
      }
    }

    return true;
  }
  return false;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Clears the hover information - called on first initiation of map */
// TODO: Comment
void EditorMap::clearHoverInfo()
{
  active_info.active_cursor = EditorEnumDb::NO_CURSOR;
  active_info.active_layer = EditorEnumDb::NO_LAYER;
  active_info.active_sprite = NULL;
  active_info.active_thing = NULL;
  active_info.hover_tile = NULL;
  active_info.selected_thing = QRect();

  /* Clear the active hover on all tiles in all sub-maps */
  /* Not necessary, I think */
  //for(int i = 0; i < sub_maps.size(); i++)
  //  for(int j = 0; j < sub_maps[i]->tiles.size(); j++)
  //    for(int k = 0; k < sub_maps[i]->tiles[j].size(); k++)
  //      sub_maps[i]->tiles[j][k]->setHover(false);

  active_submap = NULL;
}

/* Click trigger on tile in map */
// TODO: Comment
void EditorMap::clickTrigger(bool single, bool right_click)
{
  EditorEnumDb::CursorMode cursor = active_info.active_cursor;
  EditorEnumDb::Layer layer = active_info.active_layer;

  /* Make sure there's a hover sprite */
  if(active_info.hover_tile != NULL)
  {
    /* Check on the layer - base sprite */
    if(layer == EditorEnumDb::BASE || layer == EditorEnumDb::ENHANCER ||
       layer == EditorEnumDb::LOWER1 || layer == EditorEnumDb::LOWER2 ||
       layer == EditorEnumDb::LOWER3 || layer == EditorEnumDb::LOWER4 ||
       layer == EditorEnumDb::LOWER5 || layer == EditorEnumDb::UPPER1 ||
       layer == EditorEnumDb::UPPER2 || layer == EditorEnumDb::UPPER3 ||
       layer == EditorEnumDb::UPPER4 || layer == EditorEnumDb::UPPER5)
    {
      EditorSprite* sprite = active_info.active_sprite;

      /* ---- BASIC PLACE CURSOR ---- */
      if(cursor == EditorEnumDb::BASIC && sprite != NULL)
      {
        active_info.hover_tile->place(layer, sprite);
      }
      /* ---- ERASER CURSOR ---- */
      else if(cursor == EditorEnumDb::ERASER)
      {
        EditorTile* tile = active_info.hover_tile;

        if(right_click)
          recursiveErase(tile->getX(), tile->getY(), layer,
                         tile->getSprite(layer), active_submap);
        else
          active_info.hover_tile->unplace(layer);
      }
      /* ---- FILL CURSOR ---- */
      else if(single && cursor == EditorEnumDb::FILL && sprite != NULL &&
              sprite != active_info.hover_tile->getSprite(layer))
      {
        EditorTile* tile = active_info.hover_tile;

        recursiveFill(tile->getX(), tile->getY(), layer,
                      tile->getSprite(layer), active_submap);
      }
      /* ---- ALL PASSABILITY CURSOR ---- */
      else if(cursor == EditorEnumDb::PASS_ALL)
      {
        active_info.hover_tile->setPassability(layer, !right_click);
      }
      /* ---- NORTH PASSABILITY CURSOR ---- */
      else if(cursor == EditorEnumDb::PASS_N)
      {
        active_info.hover_tile->setPassability(layer, Direction::NORTH,
                                               !right_click);
      }
      /* ---- EAST PASSABILITY CURSOR ---- */
      else if(cursor == EditorEnumDb::PASS_E)
      {
        active_info.hover_tile->setPassability(layer, Direction::EAST,
                                               !right_click);
      }
      /* ---- SOUTH PASSABILITY CURSOR ---- */
      else if(cursor == EditorEnumDb::PASS_S)
      {
        active_info.hover_tile->setPassability(layer, Direction::SOUTH,
                                               !right_click);
      }
      /* ---- WEST PASSABILITY CURSOR ---- */
      else if(cursor == EditorEnumDb::PASS_W)
      {
        active_info.hover_tile->setPassability(layer, Direction::WEST,
                                               !right_click);
      }
    }
    /* Check on the layer - thing */
    else if(layer == EditorEnumDb::THING)
    {
      /* Check cursor */
      if(cursor == EditorEnumDb::BASIC && active_info.active_thing != NULL)
      {
        /* Create the thing */
        int id = getNextThingID(true);
        EditorThing* new_thing = new EditorThing(id);
        new_thing->setBase(active_info.active_thing);
        new_thing->setX(active_info.hover_tile->getX());
        new_thing->setY(active_info.hover_tile->getY());

        /* Attempt to place */
        if(addThing(new_thing))
        {
          active_submap->things.push_back(new_thing);
          emit thingInstanceChanged();
          setHoverThing(-1);
        }
        else
        {
          delete new_thing;
        }
      }
      else if(cursor == EditorEnumDb::ERASER)
      {
        int max = Helpers::getRenderDepth();

        /* Loop through all to find the top thing */
        EditorThing* found = NULL;
        for(int i = max - 1; found == NULL && i >= 0; i--)
          if(active_info.hover_tile->getThing(i) != NULL)
            found = active_info.hover_tile->getThing(i);

        /* If found, remove from tiles and delete */
        if(found != NULL)
          unsetThing(found->getID(), true);
      }
    }
  }
}

/* Click trigger on tile in map */
// TODO: Comment
void EditorMap::clickTrigger(QList<EditorTile*> tiles, bool erase)
{
  EditorEnumDb::CursorMode cursor = active_info.active_cursor;
  EditorEnumDb::Layer layer = active_info.active_layer;

  /* Make sure there's a hover sprite */
  if(active_info.hover_tile != NULL)
  {
    /* Check on the layer - base sprite */
    if(layer == EditorEnumDb::BASE || layer == EditorEnumDb::ENHANCER ||
       layer == EditorEnumDb::LOWER1 || layer == EditorEnumDb::LOWER2 ||
       layer == EditorEnumDb::LOWER3 || layer == EditorEnumDb::LOWER4 ||
       layer == EditorEnumDb::LOWER5 || layer == EditorEnumDb::UPPER1 ||
       layer == EditorEnumDb::UPPER2 || layer == EditorEnumDb::UPPER3 ||
       layer == EditorEnumDb::UPPER4 || layer == EditorEnumDb::UPPER5)
    {
      /* Check cursor - only block palce for group of tiles */
      if(cursor == EditorEnumDb::BLOCKPLACE)
      {
        for(int i = 0; i < tiles.size(); i++)
        {
          if(tiles[i] != NULL)
          {
            if(erase)
              tiles[i]->unplace(layer);
            else
              tiles[i]->place(layer, active_info.active_sprite);
          }
        }
      }
    }
  }
}

/* Returns current references for lists in map */
// TODO: Comment
SubMapInfo* EditorMap::getCurrentMap()
{
  return active_submap;
}

/* Returns sub-map index for the active sub-map */
// TODO: Comment
int EditorMap::getCurrentMapIndex()
{
  /* Ensure the active map isn't null */
  if(active_submap != NULL)
  {
    for(int i = 0; i < sub_maps.size(); i++)
      if(sub_maps[i] == active_submap)
        return i;

    /* If sub-map not found, it doesn't exist -> nullify it */
    setCurrentMap(-1);
  }

  return -1;
}

/* Returns current references for lists in map */
// TODO: Comment
int EditorMap::getCurrentSpriteIndex()
{
  /* Ensure the active sprite isn't null */
  if(active_info.active_sprite != NULL)
  {
    for(int i = 0; i < sprites.size(); i++)
      if(sprites[i] == active_info.active_sprite)
        return i;

    /* If sprite not found, it doesn't exist -> nullify it */
    setCurrentSprite(-1);
  }

  return -1;
}

/* Returns current references for lists in map */
// TODO: Comment
int EditorMap::getCurrentThingIndex()
{
  /* Ensure the active thing isn't null */
  if(active_info.active_thing != NULL)
  {
    for(int i = 0; i < base_things.size(); i++)
      if(base_things[i] == active_info.active_thing)
        return i;

    /* thing not found, it doesn't exist -> nullify it */
    setCurrentMap(-1);
  }

  return -1;
}

/* Returns the hover information */
// TODO: Comment
HoverInfo* EditorMap::getHoverInfo()
{
  return &active_info;
}

/*
 * Description: Returns the ID of the editor map.
 *
 * Inputs: none
 * Output: int - map id. Less than 0 if unset
 */
int EditorMap::getID() const
{
  return id;
}

/*
 * Description: Returns the map with the corresponding ID. NULL if ID doesn't
 *              match any map.
 *
 * Inputs: int id - map id
 * Output: SubMapInfo* - the struct with map info. NULL if not found.
 */
SubMapInfo* EditorMap::getMap(int id)
{
  if(id >= 0)
    for(int i = 0; i < sub_maps.size(); i++)
      if(sub_maps[i]->id == id)
        return sub_maps[i];
  return NULL;
}

/*
 * Description: Returns the map with the corresponding index. NULL if index is
 *              out of range.
 *
 * Inputs: int index - the index in the sub-map array.
 * Output: SubMapInfo* - the sub-map struct to return.
 */
SubMapInfo* EditorMap::getMapByIndex(int index)
{
  if(index >= 0 && index < sub_maps.size())
    return sub_maps[index];
  return NULL;
}

/*
 * Description: Returns the number of maps within the editor map (master).
 *
 * Inputs: none
 * Output: int - the number of maps
 */
int EditorMap::getMapCount()
{
  return sub_maps.size();
}

/*
 * Description: Returns the index in the array of maps that match the sub-Map
 *              ID.
 *
 * Inputs: int id - map id
 * Output: int - the index in the array
 */
int EditorMap::getMapIndex(int id)
{
  if(id >= 0)
    for(int i = 0; i < sub_maps.size(); i++)
      if(sub_maps[i]->id == id)
        return i;
  return -1;
}

/*
 * Description: Returns the list of maps in the format "ID: NAME"
 *
 * Inputs: none
 * Output: QVector<QString> - the list of strings of each sub-map.
 */
QVector<QString> EditorMap::getMapList()
{
  QVector<QString> list;

  for(int i = 0; i < sub_maps.size(); i++)
    list.push_back(QString::number(sub_maps[i]->id) + ": " + sub_maps[i]->name);

  return list;
}

/*
 * Description: Returns the name plus ID for the sub-map at the index
 *
 * Inputs: int index - the sub-map name index
 * Output: QString - list name
 */
QString EditorMap::getMapNameList(int index)
{
  if(index >= 0 && index < sub_maps.size())
    return EditorHelpers::getListString(sub_maps[index]->id,
                                        sub_maps[index]->name);
  return "";
}

/*
 * Description: Returns the vector of all maps within the editor map.
 *
 * Inputs: none
 * Output: QVector<SubMapInfo*> - the set of all sub-maps
 */
QVector<SubMapInfo*> EditorMap::getMaps()
{
  return sub_maps;
}

/*
 * Description: Returns the name of the master editor Map
 *
 * Inputs: none
 * Output: QString - the string of the map
 */
QString EditorMap::getName() const
{
  return name;
}

/* Description: Returns the name plus ID for the map editor
 *
 * Inputs: none
 * Output: QString - list name
 */
QString EditorMap::getNameList()
{
  return EditorHelpers::getListString(id, name);
}

/*
 * Description: Returns the next available map ID that can be used for a new
 *              map.
 *
 * Inputs: none
 * Output: int - the id to use
 */
int EditorMap::getNextMapID()
{
  bool found = false;
  int id = 0;

  for(int i = 0; !found && (i < sub_maps.size()); i++)
  {
    if(sub_maps[i]->id != i)
    {
      id = i;
      found = true;
    }
  }

  /* If nothing found, just make it the last ID + 1 */
  if(!found && sub_maps.size() > 0)
    id = sub_maps.last()->id + 1;

  return id;
}

/*
 * Description: Returns the next available sprite ID that can be used for a new
 *              sprite.
 *
 * Inputs: none
 * Output: int - the id to use
 */
int EditorMap::getNextSpriteID()
{
  bool found = false;
  int id = 0;

  for(int i = 0; !found && (i < sprites.size()); i++)
  {
    if(sprites[i]->getID() != i)
    {
      id = i;
      found = true;
    }
  }

  /* If nothing found, just make it the last ID + 1 */
  if(!found && sprites.size() > 0)
    id = sprites.last()->getID() + 1;

  return id;
}

/*
 * Description: Returns the next available thing ID that can be used for a new
 *              thing.
 *
 * Inputs: bool from_sub - true if it should be unset in sub-maps instead
 * Output: int - the id to use
 */
int EditorMap::getNextThingID(bool from_sub)
{
  bool found = false;
  int id = 0;

  /* If not from sub map, check base for base ID */
  if(!from_sub)
  {
    for(int i = 0; !found && (i < base_things.size()); i++)
    {
      if(base_things[i]->getID() != i)
      {
        id = i;
        found = true;
      }
    }

    /* If nothing found, just make it the last ID + 1 */
    if(!found && base_things.size() > 0)
      id = base_things.last()->getID() + 1;
  }
  /* Otherwise, check the sub-maps for available ID */
  else
  {
    /* Compile the IDs of all things in all sub-maps */
    QVector<int> id_list;
    for(int i = 0; i < sub_maps.size(); i++)
      for(int j = 0; j < sub_maps[i]->things.size(); j++)
        id_list.push_back(sub_maps[i]->things[j]->getID());

    /* Sort the list */
    qSort(id_list);

    /* Find the next available ID */
    id = kBASE_ID_THING;
    for(int i = 0; !found && (i < id_list.size()); i++)
    {
      if(id_list[i] != (kBASE_ID_THING + i))
      {
        id = (kBASE_ID_THING + i);
        found = true;
      }
    }

    /* If nothing found, just make it the last ID + 1 */
    if(!found && id_list.size() > 0)
      id = id_list.last() + 1;
  }

  return id;
}

/*
 * Description: Returns the active layers in the given tile, in comma delimited
 *              string format.
 *
 * Inputs: int map_index - the index of the sub-map
 *         int x - the x coordinate of the tile
 *         int y - the y coordinate of the tile
 * Output: QString - the comma delimited string format: B,E,L1, etc
 */
QString EditorMap::getSetLayers(int map_index, int x, int y)
{
  if(map_index >= 0 && map_index < sub_maps.size())
  {
    if(x >= 0 && x < sub_maps[map_index]->tiles.size() &&
       y >= 0 && y < sub_maps[map_index]->tiles[x].size())
    {
      return sub_maps[map_index]->tiles[x][y]->getActiveLayers();
    }
  }

  return "";
}

/*
 * Description: Returns the sprite with the corresponding ID.
 *
 * Inputs: int id - the id of the sprite to get
 * Output: EditorSprite* - the pointer to match the ID. NULL if not found.
 */
EditorSprite* EditorMap::getSprite(int id)
{
  if(id >= 0)
    for(int i = 0; i < sprites.size(); i++)
      if(sprites[i]->getID() == id)
        return sprites[i];
  return NULL;
}

/*
 * Description: Returns the sprite at the corresponding index in the list.
 *
 * Inputs: int index - the index in the array (0 to size - 1)
 * Output: EditorSprite* - the pointer to match the index. NULL if out of range
 */
EditorSprite* EditorMap::getSpriteByIndex(int index)
{
  if(index >= 0 && index < sprites.size())
    return sprites[index];
  return NULL;
}

/*
 * Description: Returns the number of sprites in the list.
 *
 * Inputs: none
 * Output: int - the number of sprites in the map set
 */
int EditorMap::getSpriteCount()
{
  return sprites.size();
}

/*
 * Description: Returns the index in the list of sprites of the matching id.
 *              Less than 0 if none match.
 *
 * Inputs: int id - the id to find the index for
 * Output: int - the index of the sprite. Less than 0 if nothing matches
 */
int EditorMap::getSpriteIndex(int id)
{
  if(id >= 0)
    for(int i = 0; i < sprites.size(); i++)
      if(sprites[i]->getID() == id)
        return i;
  return -1;
}

/*
 * Description: Returns the list of all sprites in the editor Map
 *
 * Inputs: none
 * Output: QVector<EditorSprite*> - list of all sprites
 */
QVector<EditorSprite*> EditorMap::getSprites()
{
  return sprites;
}

/*
 * Description: Returns the thing with the corresponding ID.
 *
 * Inputs: int id - the id of the thing to get
 *         int sub_map - the sub-map to get the things for (<0 is base)
 * Output: EditorThing* - the pointer to match the ID. NULL if not found.
 */
EditorThing* EditorMap::getThing(int id, int sub_map)
{
  if(id >= 0)
  {
    /* If sub map ref is less than 0, get from base set */
    if(sub_map < 0)
    {
      for(int i = 0; i < base_things.size(); i++)
        if(base_things[i]->getID() == id)
          return base_things[i];
    }
    /* Otherwise, get from sub map */
    else if(sub_map < sub_maps.size())
    {
      for(int i = 0; i < sub_maps[sub_map]->things.size(); i++)
        if(sub_maps[sub_map]->things[i]->getID() == id)
          return sub_maps[sub_map]->things[i];
    }
  }

  return NULL;
}

/*
 * Description: Returns the thing at the corresponding index in the list.
 *
 * Inputs: int index - the index in the array (0 to size - 1)
 *         int sub_map - the sub-map to get the things for (<0 is base)
 * Output: EditorThing* - the pointer to match the index. NULL if out of range
 */
EditorThing* EditorMap::getThingByIndex(int index, int sub_map)
{
  /* If sub map ref is less than 0, get from base set */
  if(sub_map < 0)
  {
    if(index >= 0 && index < base_things.size())
      return base_things[index];
  }
  /* Otherwise, get from sub map */
  else if(sub_map < sub_maps.size())
  {
    if(index >= 0 && index < sub_maps[sub_map]->things.size())
      return sub_maps[sub_map]->things[index];
  }
  return NULL;
}

/*
 * Description: Returns the number of things in the list.
 *
 * Inputs: int sub_map - the sub-map to get the things for (<0 is base)
 * Output: int - the number of things in the map set
 */
int EditorMap::getThingCount(int sub_map)
{
  /* If sub map ref is less than 0, get from base set */
  if(sub_map < 0)
    return base_things.size();
  /* Otherwise, get from sub map */
  else if(sub_map < sub_maps.size())
    return sub_maps[sub_map]->things.size();
  return 0;
}

/*
 * Description: Returns the index in the list of things of the matching id.
 *              Less than 0 if none match.
 *
 * Inputs: int id - the id to find the index for
 *         int sub_map - the sub-map to get the things for (<0 is base)
 * Output: int - the index of the thing. Less than 0 if nothing matches
 */
int EditorMap::getThingIndex(int id, int sub_map)
{
  if(id >= 0)
  {
    /* If sub map ref is less than 0, get from base set */
    if(sub_map < 0)
    {
      for(int i = 0; i < base_things.size(); i++)
        if(base_things[i]->getID() == id)
          return i;
    }
    /* Otherwise, get from sub map */
    else if(sub_map < sub_maps.size())
    {
      for(int i = 0; i < sub_maps[sub_map]->things.size(); i++)
        if(sub_maps[sub_map]->things[i]->getID() == id)
          return i;
    }
  }
  return -1;
}

/*
 * Description: Returns a list of all things in the format of a HEADER row
 *              with the following rows as "ID: NAME".
 *
 * Inputs: int sub_map - the sub-map to get the things for (<0 is base)
 *         bool all_submaps - true if all sub-maps should be stacked together
 * Output: QVector<QString> - list of all things
 */
QVector<QString> EditorMap::getThingList(int sub_map, bool all_submaps)
{
  QVector<QString> stack;
  stack.push_back("MAP THINGS");

  /* If sub map ref is less than 0, get from base set */
  if(sub_map < 0)
  {
    /* Go through all things and add them to the list */
    for(int i = 0; i < base_things.size(); i++)
      stack.push_back(base_things[i]->getNameList());
  }
  /* Otherwise, get from sub map */
  else if(sub_map < sub_maps.size())
  {
    /* Determine the range */
    int start = sub_map;
    int end = sub_map;
    if(all_submaps)
    {
      start = 0;
      end = sub_maps.size() - 1;
    }

    /* Go through the maps and add them to the list */
    for(int i = start; i <= end; i++)
      for(int j = 0; j < sub_maps[i]->things.size(); j++)
        stack.push_back(sub_maps[i]->things[j]->getNameList());
  }

  return stack;
}

/*
 * Description: Returns the list of all things in the editor map
 *
 * Inputs: int sub_map - the sub-map to get the things for (<0 is base)
 * Output: QVector<EditorThing*> - list of all things
 */
QVector<EditorThing*> EditorMap::getThings(int sub_map)
{
  /* If sub map ref is less than 0, get from base set */
  if(sub_map < 0)
  {
    return base_things;
  }
  /* Otherwise, get from sub map */
  else if(sub_map < sub_maps.size())
  {
    return sub_maps[sub_map]->things;
  }

  /* Otherwise, return blank list */
  QVector<EditorThing*> blank_list;
  return blank_list;
}

/*
 * Description: Returns the tile icons that is stored within the editor map.
 *
 * Inputs: none
 * Output: TileIcons* - the icon struct.
 */
TileIcons* EditorMap::getTileIcons()
{
  return tile_icons;
}

/*
 * Description: Loads the map data from the XML struct and offset index.
 *
 * Inputs: XmlData data - the XML data tree struct
 *         int index - the offset index into the struct
 * Output: none
 */
void EditorMap::load(XmlData data, int index)
{
  /* Parse the data for the map */
  if(data.getElement(index) == "name")
  {
    setName(QString::fromStdString(data.getDataString()));
  }
  else if(data.getElement(index) == "sprite" && data.getKey(index) == "id")
  {
    int sprite_id = QString::fromStdString(data.getKeyValue(index)).toInt();
    EditorSprite* sprite = getSprite(sprite_id);

    /* Create new sprite if it doesn't exist */
    if(sprite == NULL)
    {
      sprite = new EditorSprite();
      sprite->setID(sprite_id);
      setSprite(sprite);
    }

    /* Continue to parse the data in the sprite */
    sprite->load(data, index + 1);
  }
  else if(data.getElement(index) == "main" ||
          (data.getElement(index) == "section" && data.getKey(index) == "id"))
  {
    int id = -1;
    if(data.getElement(index) == "main")
      id = 0;
    else
      id = QString::fromStdString(data.getKeyValue(index)).toInt();

    /* Attempt to get map. If it doesn't exist, make new */
    SubMapInfo* access_map = getMap(id);
    if(access_map == NULL)
    {
      int index = setMap(id, "TEMP", 1, 1);
      access_map = getMapByIndex(index);
    }

    /* Proceed to modify the map as needed */
    loadSubMap(access_map, data, index + 1);
  }
}

/*
 * Description: Saves the map data to the file handling pointer.
 *
 * Inputs: FileHandler* fh - the file handling pointer
 *         bool game_only - true if the data should include game only relevant
 *         int sub_index - the sub map index to save
 * Output: none
 */
// TODO: Once thing creation is done, need to have create temporary player
//       for sub_map saving if one doesn't exist
void EditorMap::save(FileHandler* fh, bool game_only, int sub_index)
{
  if(fh != NULL)
  {
    fh->writeXmlElement("map", "id", getID());
    if(!game_only)
      fh->writeXmlData("name", getName().toStdString());

    /* Add sprites */
    for(int i = 0; i < sprites.size(); i++)
      sprites[i]->save(fh, game_only);

    /* Save all maps if sub_index is out of range */
    if(sub_index <= 0 || sub_index >= sub_maps.size())
    {
      for(int i = 0; i < sub_maps.size(); i++)
        saveSubMap(fh, game_only, sub_maps[i], i == 0);
    }
    /* Otherwise, just save the only map */
    else
    {
      saveSubMap(fh, game_only, sub_maps[sub_index], true);
    }

    fh->writeXmlElementEnd();
  }
}

/* Sets the current references for the selected sprite(s) or thing(s) */
// TODO: Comment
bool EditorMap::setCurrentMap(int index)
{
  if(index >= -1 && index < sub_maps.size())
  {
    /* If index is -1, unset the current sub-map */
    if(index == -1)
      active_submap = NULL;
    /* Otherwise, index is in valid range of sub-map set */
    else
      active_submap = sub_maps[index];

    /* Clear out the hover info */
    active_info.hover_tile = NULL;
    active_info.selected_thing = QRect();

    /* Clear the new sub-map hover tiles */
    if(active_submap != NULL)
      for(int i = 0; i < sub_maps[index]->tiles.size(); i++)
        for(int j = 0; j < sub_maps[index]->tiles[i].size(); j++)
          sub_maps[index]->tiles[i][j]->setHover(false);

    /* Trigger thing instance update */
    emit thingInstanceChanged();

    return true;
  }
  return false;
}

/* Sets the current references for the selected sprite(s) or thing(s) */
/* -1 unselects */
// TODO: Comment
bool EditorMap::setCurrentSprite(int index)
{
  if(index >= -1 && index < sprites.size())
  {
    /* If index is -1, unset the current sprite */
    if(index == -1)
      active_info.active_sprite = NULL;
    /* Otherwise, index is in valid range of sprite set */
    else
      active_info.active_sprite = sprites[index];

    /* If the tile in the active info isn't null, update it */
    if(active_info.hover_tile != NULL)
      active_info.hover_tile->update();

    return true;
  }
  return false;
}

/* Sets the current references for the selected sprite(s) or thing(s) */
/* -1 unselects */
// TODO: Comment
bool EditorMap::setCurrentThing(int index)
{
  if(index >= -1 && index < base_things.size())
  {
    /* Unset the hover, if relevant */
    updateHoverThing(true);

    /* If index is -1, unset the current thing */
    if(index == -1)
      active_info.active_thing = NULL;
    /* Otherwise, index is in valid range of base thing set */
    else
      active_info.active_thing = base_things[index];

    /* Set the hover, if relevant */
    updateHoverThing();

    return true;
  }
  return false;
}

/* Sets the hover information */
// TODO: Comment
void EditorMap::setHoverCursor(EditorEnumDb::CursorMode cursor)
{
  /* First, unset hover if relevant */
  updateHoverThing(true);

  /* Set the cursor */
  active_info.active_cursor = cursor;

  /* Try and set hover, if relevant */
  if(!updateHoverThing() && active_info.hover_tile != NULL)
    active_info.hover_tile->update();
}

/* Sets the hover information */
// TODO: Comment
void EditorMap::setHoverLayer(EditorEnumDb::Layer layer)
{
  /* First, unset hover if relevant */
  updateHoverThing(true);

  /* Set the layer */
  active_info.active_layer = layer;

  /* Try and set hover, if relevant */
  if(!updateHoverThing() && active_info.hover_tile != NULL)
    active_info.hover_tile->update();
}

/* Sets the hover information */
// TODO: Comment
bool EditorMap::setHoverThing(int id)
{
  /* First clear the existing one */
  QRect existing = active_info.selected_thing;
  active_info.selected_thing = QRect();

  /* Then, set the new one */
  if(active_submap != NULL)
  {
    /* First, update the old tiles to remove line */
    for(int i = 0; i < existing.width(); i++)
      for(int j = 0; j < existing.height(); j++)
        active_submap->tiles[existing.x() + i][existing.y() + j]->update();

    /* Next, try and get the new thing */
    EditorThing* thing = getThing(id, active_submap->id);
    if(thing != NULL)
    {
      active_info.selected_thing = QRect(thing->getX(), thing->getY(),
                                         thing->getMatrix()->getWidth(),
                                         thing->getMatrix()->getHeight());
      for(int i = 0; i < active_info.selected_thing.width(); i++)
        for(int j = 0; j < active_info.selected_thing.height(); j++)
          active_submap->tiles[thing->getX() + i][thing->getY() + j]->update();

      return true;
    }
  }

  return false;
}

/* Sets the hover information */
// TODO: Comment
void EditorMap::setHoverTile(EditorTile* tile)
{
  /* First, unset hover if relevant */
  updateHoverThing(true);

  /* Set the hover tile */
  if(active_info.hover_tile != NULL)
    active_info.hover_tile->setHover(false);
  active_info.hover_tile = tile;
  if(active_info.hover_tile != NULL)
    active_info.hover_tile->setHover(true);

  /* Try and set hover, if relevant */
  if(!updateHoverThing() && active_info.hover_tile != NULL)
    active_info.hover_tile->update();
}

/*
 * Description: Sets the ID of the editor map. Must be less than 0, which will
 *              then unset it.
 *
 * Inputs: int id - the id of the editor map
 * Output: none
 */
void EditorMap::setID(int id)
{
  if(id < 0)
    this->id = kUNSET_ID;
  else
    this->id = id;
}

/*
 * Description: Sets the map with id, name, and set of tiles.
 *
 * Inputs: int id - the id of the Map
 *         QString name - the name of the map
 *         QVector<QVector<EditorTile*>> tiles - the set of tiles
 * Output: int - if >= 0, it's the index where set in list. Otherwise unset
 */
int EditorMap::setMap(int id, QString name,
                      QVector<QVector<EditorTile*>> tiles)
{
  if(id >= 0)
  {
    bool found = false;
    int index = -1;
    bool near = false;

    /* Find if the ID exists */
    for(int i = 0; !found && !near && (i < sub_maps.size()); i++)
    {
      if(sub_maps[i]->id == id)
      {
        index = i;
        found = true;
      }
      else if(sub_maps[i]->id > id)
      {
        index = i;
        near = true;
      }
    }

    /* If found, modify the index with the new information */
    if(found)
    {
      sub_maps[index]->name = name;

      /* Delete existing tiles */
      for(int i = 0; i < sub_maps[index]->tiles.size(); i++)
        for(int j = 0; j < sub_maps[index]->tiles[i].size(); j++)
          delete sub_maps[index]->tiles[i][j];

      sub_maps[index]->tiles = tiles;
    }
    else
    {
      SubMapInfo* info = new SubMapInfo;
      info->id = id;
      info->name = name;
      info->tiles = tiles;

      /* If near, insert the information into the index */
      if(near)
        sub_maps.insert(index, info);
      /* Otherwise, append the new ID */
      else
      {
        index = sub_maps.size();
        sub_maps.append(info);
      }
    }

    /* Modifiy grid and passability */
    EditorTile* ref_tile = sub_maps.front()->tiles.front().front();
    bool grid = ref_tile->getVisibilityGrid();
    bool pass = ref_tile->getVisibilityPass();
    setVisibilityGrid(grid);
    setVisibilityPass(pass);
    for(int i = 0; i < (int)EditorEnumDb::NO_LAYER; i++)
      setVisibility((EditorEnumDb::Layer)i,
                    ref_tile->getVisibility((EditorEnumDb::Layer)i));

    return index;
  }

  return id;
}

/*
 * Description: Sets the map with id, name, and width/height of tile set.
 *
 * Inputs: int id - the id of the Map
 *         QString name - the name of the map
 *         int width - width of tiles in matrix
 *         int height - height of tiles in matrix
 * Output: int - if >= 0, it's the index where set in list. Otherwise unset
 */
int EditorMap::setMap(int id, QString name, int width, int height)
{
  if(id >= 0 && width > 0 && height > 0)
  {
    QVector<QVector<EditorTile*>> tiles;

    /* Create the blank set of tiles */
    for(int i = 0; i < width; i++)
    {
      QVector<EditorTile*> row;

      for(int j = 0; j < height; j++)
      {
        row.push_back(new EditorTile(i, j, tile_icons));
        row.last()->setHoverInfo(&active_info);
      }

      tiles.push_back(row);
    }

    /* Set up the map */
    return setMap(id, name, tiles);
  }

  return -1;
}

/*
 * Description: Sets the name of the editor map.
 *
 * Inputs: QString name - map name
 * Output: none
 */
void EditorMap::setName(QString name)
{
  this->name = name;
}

/*
 * Description: Sets the sprite in the set within the editor map. If a sprite,
 *              already exists with the ID, it deletes the existing one.
 *
 * Inputs: EditorSprite* sprite - the new sprite to set in
 * Output: int - the index if set. If < 0, it is not set.
 */
int EditorMap::setSprite(EditorSprite* sprite)
{
  if(sprite != NULL && sprite->getID() >= 0)
  {
    bool found = false;
    int index = -1;
    bool near = false;

    /* Find if the ID exists */
    for(int i = 0; !found && !near && (i < sprites.size()); i++)
    {
      if(sprites[i]->getID() == sprite->getID())
      {
        index = i;
        found = true;
      }
      else if(sprites[i]->getID() > sprite->getID())
      {
        index = i;
        near = true;
      }
    }

    /* If found, modify the index with the new information */
    if(found)
    {
      unsetSpriteByIndex(index);
      sprites.insert(index, sprite);
    }
    else if(near)
    {
      sprites.insert(index, sprite);
    }
    else
    {
      sprites.append(sprite);
      index = sprites.size() - 1;
    }

    return index;
  }

  return -1;
}

/*
 * Description: Sets the thing in the set within the editor map. If a thing,
 *              already exists with the ID, it deletes the existing one.
 *
 * Inputs: EditorThing* thing - the new thing to set in
 *         int sub_map - the sub-map to set the things for (<0 is base)
 * Output: int - the index if set. If < 0, it is not set.
 */
int EditorMap::setThing(EditorThing* thing, int sub_map)
{
  if(thing != NULL && thing->getID() >= 0)
  {
    bool found = false;
    int index = -1;
    bool near = false;

    /* If sub map id is less than 0, work with base thing */
    if(sub_map < 0)
    {
      /* Find if the ID exists */
      for(int i = 0; !found && !near && (i < base_things.size()); i++)
      {
        if(base_things[i]->getID() == thing->getID())
        {
          index = i;
          found = true;
        }
        else if(base_things[i]->getID() > thing->getID())
        {
          index = i;
          near = true;
        }
      }

      /* If found, modify the index with the new information */
      if(found)
      {
        unsetThingByIndex(index);
        base_things.insert(index, thing);
      }
      else if(near)
      {
        base_things.insert(index, thing);
      }
      else
      {
        base_things.append(thing);
        index = base_things.size() - 1;
      }
    }
    /* Otherwise, work with the sub id */
    else if(sub_map < sub_maps.size())
    {
      /* Check to make sure the thing could be added */
      int x_start = thing->getX();
      int y_start = thing->getY();
      int x_end = x_start + thing->getMatrix()->getWidth();
      int y_end = y_start + thing->getMatrix()->getHeight();
      if(x_start >= 0 && x_end <= sub_maps[sub_map]->tiles.size() &&
         y_start >= 0 && y_end <= sub_maps[sub_map]->tiles[x_start].size())
      {
        /* First remove the existing id, if one exists */
        unsetThing(thing->getID(), true);

        /* Now insert into the proper location in the thing stack in the map */
        for(int i = 0; !near && (i < sub_maps[sub_map]->things.size()); i++)
        {
          if(sub_maps[sub_map]->things[i]->getID() > thing->getID())
          {
            index = i;
            near = true;
          }
        }

        /* If near, insert at index. Otherwise, append */
        if(near)
        {
          sub_maps[sub_map]->things.insert(index, thing);
        }
        else
        {
          sub_maps[sub_map]->things.append(thing);
          index = sub_maps[sub_map]->things.size() - 1;
        }

        /* Add to tile */
        for(int i = x_start; i < x_end; i++)
          for(int j = y_start; j < y_end; j++)
            sub_maps[sub_map]->tiles[i][j]->setThing(thing);
      }
    }

    return index;
  }
  return -1;
}

/*
 * Description: Sets the rendering tile icons reference for painting.
 *
 * Inputs: TileIcons* icons - the icons to render on the map
 * Output: none
 */
void EditorMap::setTileIcons(TileIcons* icons)
{
  tile_icons = icons;

  for(int i = 0; i < sub_maps.size(); i++)
    for(int j = 0; j < sub_maps[i]->tiles.size(); j++)
      for(int k = 0; k < sub_maps[i]->tiles[j].size(); k++)
        sub_maps[i]->tiles[j][k]->setTileIcons(icons);
}

/*
 * Description: Sets the layer visibility in all tiles within the sub-maps.
 *
 * Inputs: EditorEnumDb::Layer layer - the layer to switch visibility on
 *         bool visible - true if the layer is visible
 * Output: none
 */
void EditorMap::setVisibility(EditorEnumDb::Layer layer, bool visible)
{
  for(int i = 0; i < sub_maps.size(); i++)
    for(int j = 0; j < sub_maps[i]->tiles.size(); j++)
      for(int k = 0; k < sub_maps[i]->tiles[j].size(); k++)
        sub_maps[i]->tiles[j][k]->setVisibility(layer, visible);
}

/*
 * Description: Sets the grid visibility on all sub-map tiles.
 *              Controlled by the map view.
 *
 * Inputs: bool visible - is the grid visible?
 * Output: none
 */
void EditorMap::setVisibilityGrid(bool visible)
{
  for(int i = 0; i < sub_maps.size(); i++)
    for(int j = 0; j < sub_maps[i]->tiles.size(); j++)
      for(int k = 0; k < sub_maps[i]->tiles[j].size(); k++)
        sub_maps[i]->tiles[j][k]->setVisibilityGrid(visible);
}

/*
 * Description: Sets the passability visibility on all sub-map tiles.
 *              Controlled by the map view.
 *
 * Inputs: bool visible - is the passability visible?
 * Output: none
 */
void EditorMap::setVisibilityPass(bool visible)
{
  for(int i = 0; i < sub_maps.size(); i++)
    for(int j = 0; j < sub_maps[i]->tiles.size(); j++)
      for(int k = 0; k < sub_maps[i]->tiles[j].size(); k++)
        sub_maps[i]->tiles[j][k]->setVisibilityPass(visible);
}

/* Thing processing for updating with the new data */
// TODO: Comment
void EditorMap::thingAddToTiles()
{
  if(active_submap != NULL)
  {
    for(int i = 0; i < active_submap->things.size(); i++)
    {
      if(!addThing(active_submap->things[i]))
      {
        delete active_submap->things[i];
        active_submap->things.remove(i);
        i--;
      }
    }
  }
}

/* Thing processing for updating with the new data */
// TODO: Comment
void EditorMap::thingRemoveFromTiles()
{
  if(active_submap != NULL)
  {
    /* Clear selection */
    setHoverThing(-1);

    for(int i = 0; i < active_submap->things.size(); i++)
    {
      EditorThing* thing = active_submap->things[i];
      int x = thing->getX();
      int y = thing->getY();
      int w = thing->getMatrix()->getWidth();
      int h = thing->getMatrix()->getHeight();

      for(int j = 0; j < w; j++)
        for(int k = 0; k < h; k++)
          active_submap->tiles[x+j][y+k]->
                           unsetThing(thing->getMatrix()->getRenderDepth(j, k));
    }
  }
}

/*
 * Description: Updates all tiles and forces a paint
 *
 * Inputs: none
 * Output: none
 */
void EditorMap::updateAll()
{
  for(int i = 0; i < sub_maps.size(); i++)
    for(int j = 0; j < sub_maps[i]->tiles.size(); j++)
      for(int k = 0; k < sub_maps[i]->tiles[j].size(); k++)
        sub_maps[i]->tiles[j][k]->update();
}

/*
 * Description: Unset the map within the list that correspond to the ID.
 *
 * Inputs: int id - the map id
 * Output: bool - true if deleted
 */
bool EditorMap::unsetMap(int id)
{
  int index = getMapIndex(id);
  return unsetMapByIndex(index);
}

/*
 * Description: Unsets the map within the list that correspond to the index.
 *
 * Inputs: int index - the sub-map index
 * Output: bool - true if deleted
 */
bool EditorMap::unsetMapByIndex(int index)
{
  if(index >= 0 && index < sub_maps.size())
  {
    /* Delete all things from sub-map */
    while(sub_maps[index]->things.size() > 0)
      unsetThingByIndex(0, index);

    /* Delete the sub-map */
    delete sub_maps[index];
    sub_maps.remove(index);

    // TODO: have this remove the teleport events related

    return true;
  }
  return false;
}

/*
 * Description: Unsets all maps within the EditorMap set.
 *
 * Inputs: none
 * Output: none
 */
void EditorMap::unsetMaps()
{
  while(sub_maps.size() > 0)
    unsetMapByIndex(0);
  sub_maps.clear();
}

/*
 * Description: Unset the sprite within the list that correspond to the ID.
 *
 * Inputs: int id - the sprite id
 * Output: bool - was a sprite deleted?
 */
bool EditorMap::unsetSprite(int id)
{
  int index = getSpriteIndex(id);
  return unsetSpriteByIndex(index);
}

/*
 * Description: Unset the sprite within the list that correspond to the index.
 *
 * Inputs: int index - the sprite index corresponder
 * Output: bool - was a sprite deleted?
 */
bool EditorMap::unsetSpriteByIndex(int index)
{
  if(index >= 0 && index < sprites.size())
  {
    /* Remove all sprites related to this index from all tiles */
    for(int i = 0; i < sub_maps.size(); i++)
    {
      for(int j = 0; j < sub_maps[i]->tiles.size(); j++)
        for(int k = 0; k < sub_maps[i]->tiles[j].size(); k++)
          sub_maps[i]->tiles[j][k]->unplace(sprites[index]);
    }

    /* Finally, delete the sprite */
    delete sprites[index];
    sprites.remove(index);

    return true;
  }

  return false;
}

/*
 * Description: Unsets all sprites within the EditorMap set.
 *
 * Inputs: none
 * Output: none
 */
void EditorMap::unsetSprites()
{
  while(sprites.size() > 0)
    unsetSpriteByIndex(0);
  sprites.clear();
}

/*
 * Description: Unset the thing within the list that correspond to the ID.
 *
 * Inputs: int id - the thing id
 *         bool from_sub - true if it should be unset in sub-maps instead
 * Output: bool - was a thing deleted?
 */
bool EditorMap::unsetThing(int id, bool from_sub)
{
  /* If not sub-maps, it's base and try to remove ID */
  if(!from_sub)
  {
    int index = getThingIndex(id);
    return unsetThingByIndex(index);
  }
  /* Otherwise, check all sub-maps */
  else
  {
    bool deleted = false;

    for(int i = 0; i < sub_maps.size(); i++)
    {
      int index = getThingIndex(id, i);
      deleted |= unsetThingByIndex(index, i);
    }

    return deleted;
  }
}

/*
 * Description: Unset the thing within the list that correspond to the index.
 *
 * Inputs: int index - the thing index corresponder
 *         int sub_map - the sub-map to get the things for (<0 is base)
 * Output: bool - was a thing deleted?
 */
bool EditorMap::unsetThingByIndex(int index, int sub_map)
{
  if(sub_map < 0)
  {
    if(index >= 0 && index < base_things.size())
    {
      /* Remove all things related to this index - sub things */
      for(int i = 0; i < sub_maps.size(); i++)
      {
        for(int j = 0; j < sub_maps[i]->things.size(); j++)
        {
          if(sub_maps[i]->things[j]->getBase() != NULL &&
             sub_maps[i]->things[j]->getBase()->getID() ==
             base_things[index]->getID())
          {
            unsetThingByIndex(j, i);
            j--;
          }
        }
      }

      /* Finally, delete the thing */
      delete base_things[index];
      base_things.remove(index);

      return true;
    }
  }
  else if(sub_map < sub_maps.size())
  {
    if(index >= 0 && index < sub_maps[sub_map]->things.size())
    {
      EditorThing* ref = sub_maps[sub_map]->things[index];

      /* Remove the instances from tiles */
      int x = ref->getX();
      int y = ref->getY();
      for(int i = x; i < (ref->getMatrix()->getWidth() + x) &&
                     i < sub_maps[sub_map]->tiles.size(); i++)
      {
        for(int j = y; j < (ref->getMatrix()->getHeight() + y) &&
                                 j < sub_maps[sub_map]->tiles[i].size(); j++)
        {
          sub_maps[sub_map]->tiles[i][j]->unsetThing(ref);
        }
      }

      /* Finally, delete the thing */
      delete ref;
      sub_maps[sub_map]->things.remove(index);

      /* Update list */
      emit thingInstanceChanged();
      setHoverThing(-1);

      return true;
    }
  }

  return false;
}

/*
 * Description: Unsets all map things within the EditorMap set.
 *
 * Inputs: bool from_sub - true if it should be unset in sub-maps instead
 * Output: none
 */
void EditorMap::unsetThings(bool from_sub)
{
  /* If not sub-maps, it's base and try to remove ID */
  if(!from_sub)
  {
    while(base_things.size() > 0)
      unsetThingByIndex(0);
    base_things.clear();
  }
  /* Otherwise, check all sub-maps */
  else
  {
    for(int i = 0; i < sub_maps.size(); i++)
    {
      while(sub_maps[i]->things.size() > 0)
        unsetThingByIndex(0, i);
      sub_maps[i]->things.clear();
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
 * Inputs: const EditorMap &source - the source class constructor
 * Output: EditorMap& - pointer to the copied class
 */
EditorMap& EditorMap::operator= (const EditorMap &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Creates the map dialog for editing the name and size.
 *
 * Inputs: QWidget* parent - the parent that owns the dialog
 *         QString name - name of new/edited map
 *         int width - the width of tiles set
 *         int height - the height of tiles set
 * Output: QDialog* the dialog pointer. Parent to delete
 */
QDialog* EditorMap::createMapDialog(QWidget* parent, QString title,
                                    QString name, int width, int height)
{
  QDialog* mapsize_dialog = new QDialog(parent);
  mapsize_dialog->setWindowTitle(title);

  /* Sets up map size dialog */
  QGridLayout* layout = new QGridLayout(mapsize_dialog);

  /* Name */
  layout->addWidget(new QLabel("Name"), 0, 0);
  QLineEdit* name_edit = new QLineEdit(name, mapsize_dialog);
  layout->addWidget(name_edit, 0, 1);

  /* Map Size */
  layout->addWidget(new QLabel("Width:"),1,0);
  QLineEdit* sizexedit = new QLineEdit("100",mapsize_dialog);
  if(width > 0)
    sizexedit->setText(QString::number(width));
  layout->addWidget(sizexedit,1,1);
  layout->addWidget(new QLabel("Height:"),2,0);
  QLineEdit* sizeyedit = new QLineEdit("100",mapsize_dialog);
  if(height > 0)
    sizeyedit->setText(QString::number(height));
  layout->addWidget(sizeyedit,2,1);

  /* Confirmation button */
  QPushButton* ok = new QPushButton("&Create Map",mapsize_dialog);
  layout->addWidget(ok,3,0,1,2);

  return mapsize_dialog;
}

/*
 * Description: Copies the sub-map information from a base map to a new map.
 *              It does not copy the ID. It does not take ownership of newly
 *              created tiles.
 *
 * Inputs: SubMapInfo* copy_map - the map to copy information from
 *         SubMapInfo* new_map - the map to copy information to
 *         TileIcons* icons - the icons to load into the tiles
 * Output: bool - true if the info was copied
 */
bool EditorMap::copySubMap(SubMapInfo* copy_map, SubMapInfo* new_map, 
                           TileIcons* icons)
{
  if(copy_map != NULL && new_map != NULL)
  {
    new_map->name = copy_map->name;

    /* Delete all tiles in the new map -> not relevant */
    for(int i = 0; i < new_map->tiles.size(); i++)
      for(int j = 0; j < new_map->tiles[i].size(); j++)
        delete new_map->tiles[i][j];
    new_map->tiles.clear();

    /* Add a copy of all tiles in copy to new */
    for(int i = 0; i < copy_map->tiles.size(); i++)
    {
      QVector<EditorTile*> row;

      for(int j = 0; j < copy_map->tiles[i].size(); j++)
      {
        row.push_back(new EditorTile(i, j, icons));
        *row.last() = *copy_map->tiles[i][j];
        row.last()->setHoverInfo(copy_map->tiles[i][j]->getHoverInfo());
      }

      new_map->tiles.push_back(row);
    }

    return true;
  }
  return false;
}

/*
 * Description: Returns the push button in the map dialog, that was created
 *              above. This will not return expected results on dialogs not
 *              created with the createMapDialog() function.
 *
 * Inputs: QDialog* dialog - the dialog pointer
 * Output: QPushButton* - the button in the dialog
 */
QPushButton* EditorMap::getDialogButton(QDialog* dialog)
{
  QPushButton* button = NULL;

  if(dialog != NULL)
  {
    QGridLayout* layout = (QGridLayout*)dialog->layout();
    QLayoutItem* item = layout->itemAtPosition(3, 0);
    button = (QPushButton*)item->widget();
  }

  return button;
}

/*
 * Description: Returns the height stored within the dialog.
 *
 * Inputs: QDialog* dialog - the dialog pointer
 * Output: int - the height, in tiles. Invalid is -1
 */
int EditorMap::getDialogHeight(QDialog* dialog)
{
  if(dialog != NULL)
  {
    QGridLayout* layout = (QGridLayout*)dialog->layout();
    QLayoutItem* item = layout->itemAtPosition(2, 1);
    return (((QLineEdit*)item->widget())->text()).toInt();
  }

  return -1;
}

/*
 * Description: Returns the name stored within the dialog.
 *
 * Inputs: QDialog* dialog - the dialog pointer
 * Output: QString - the name, in string format. Invalid if blank
 */
QString EditorMap::getDialogName(QDialog* dialog)
{
  if(dialog != NULL)
  {
    QGridLayout* layout = (QGridLayout*)dialog->layout();
    QLayoutItem* item = layout->itemAtPosition(0, 1);
    return ((QLineEdit*)item->widget())->text();
  }

  return "";
}

/*
 * Description: Returns the width stored within the dialog.
 *
 * Inputs: QDialog* dialog - the dialog pointer
 * Output: int - the width, in tiles. Invalid is -1
 */
int EditorMap::getDialogWidth(QDialog* dialog)
{
  if(dialog != NULL)
  {
    QGridLayout* layout = (QGridLayout*)dialog->layout();
    QLayoutItem* item = layout->itemAtPosition(1, 1);
    return (((QLineEdit*)item->widget())->text()).toInt();
  }

  return -1;
}
