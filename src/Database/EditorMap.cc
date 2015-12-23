/*******************************************************************************
 * Class Name: EditorMap
 * Date Created: December 27, 2014
 * Inheritance: QObject, EditorTemplate
 * Description: The map interface to connect and edit in the editor
 ******************************************************************************/
#include "Database/EditorMap.h"
#include <QDebug>

/* Constant Implementation - see header file for descriptions */
//const int EditorMap::kBASE_ID_IOS = 30000;
//const int EditorMap::kBASE_ID_ITEMS = 40000;
//const int EditorMap::kBASE_ID_PERSON = 0;
//const int EditorMap::kBASE_ID_NPC = 10000;
//const int EditorMap::kBASE_ID_THING = 20000;
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
  visible_path = true;

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
  clearAll();
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: Attempts to add the IO to the current sub-map. IO needs
 *              to be given an x and y start location prior to calling this
 *              function.
 *
 * Inputs: EditorMapIO* io - the IO to attempt to add
 *         SubMapInfo* map - the map to add the IO to
 *         bool existing - false if new IO. true otherwise
 * Output: bool - true if the IO was added (does not delete if fails)
 */
bool EditorMap::addIO(EditorMapIO* io, SubMapInfo* map, bool existing)
{
  int x = io->getX();
  int y = io->getY();
  int w = io->getMatrix()->getWidth();
  int h = io->getMatrix()->getHeight();

  /* Ensure map isn't null */
  if(map == NULL)
    map = active_submap;

  /* Check if IO can be placed */
  bool valid = false;
  if(x >= 0 && y >= 0 && (x+w) <= map->tiles.size() &&
     (y+h) <= map->tiles[x].size())
  {
    valid = true;

    /* Check each tile */
    for(int i = 0; i < w; i++)
      for(int j = 0; j < h; j++)
        if(!io->isAllNull(i, j))
          if(map->tiles[x+i][y+j]->getIO(
                                io->getMatrix()->getRenderDepth(i, j)) != NULL)
            valid = false;
  }

  /* If valid, place IO */
  if(valid)
  {
    /* Add the IO to tiles */
    for(int i = x; i < (w+x); i++)
      for(int j = y; j < (h+y); j++)
        map->tiles[i][j]->setIO(io);

    /* Add to stack and emit new signals */
    if(!existing)
    {
      map->ios.push_back(io);
      emit ioInstanceChanged(io->getNameList());
      setHoverThing(-1);
    }
  }

  return valid;
}

/*
 * Description: Attempts to add the item to the current sub-map. Item needs
 *              to be given an x and y start location prior to calling this
 *              function.
 *
 * Inputs: EditorMapItem* item - the item to attempt to add
 *         SubMapInfo* map - the map to add the item to
 *         bool existing - false if new item. true otherwise
 * Output: bool - true if the item was added (does not delete if fails)
 */
bool EditorMap::addItem(EditorMapItem* item, SubMapInfo* map, bool existing)
{
  int x = item->getX();
  int y = item->getY();
  int w = item->getMatrix()->getWidth();
  int h = item->getMatrix()->getHeight();

  /* Ensure map isn't null. If not set, use active sub-map */
  if(map == NULL)
    map = active_submap;

  /* Check if item can be placed */
  bool valid = false;
  if(x >= 0 && y >= 0 && x <= map->tiles.size() &&
     y <= map->tiles[x].size() && w == 1 && h == 1)
  {
    /* Check validity of item */
    if(!item->isAllNull(0, 0) &&
       map->tiles[x][y]->getItems().size() < EditorTile::kMAX_ITEMS)
    {
      valid = true;
    }
  }

  /* If valid, place item */
  if(valid)
  {
    /* Add the item to the tile */
    map->tiles[x][y]->addItem(item);

    /* Add to stack and emit new signals */
    if(!existing)
    {
      map->items.push_back(item);
      emit itemInstanceChanged(item->getNameList());
      setHoverThing(-1);
    }
  }

  return valid;
}

/*
 * Description: Attempts to add the npc to the current sub-map. NPC needs
 *              to be given an x and y start location prior to calling this
 *              function.
 *
 * Inputs: EditorMapNPC* npc - the npc to attempt to add
 *         SubMapInfo* map - the map to add the npc to
 *         bool existing - false if new npc. true otherwise
 * Output: bool - true if the npc was added (does not delete if fails)
 */
bool EditorMap::addNPC(EditorMapNPC* npc, SubMapInfo* map, bool existing)
{
  int x = npc->getX();
  int y = npc->getY();
  int w = npc->getMatrix()->getWidth();
  int h = npc->getMatrix()->getHeight();

  /* Ensure map isn't null. If not set, use active sub-map */
  if(map == NULL)
    map = active_submap;

  /* Check if npc can be placed */
  bool valid = false;
  if(x >= 0 && y >= 0 && (x+w) <= map->tiles.size() &&
     (y+h) <= map->tiles[x].size())
  {
    valid = true;

    /* Check each tile */
    for(int i = 0; i < w; i++)
    {
      for(int j = 0; j < h; j++)
      {
        if(!npc->isAllNull(i, j))
        {
          int depth = npc->getMatrix()->getRenderDepth(i, j);

          if(map->tiles[x+i][y+j]->getNPC(depth) != NULL ||
             map->tiles[x+i][y+j]->getPerson(depth) != NULL)
            valid = false;
        }
      }
    }
  }

  /* If valid, configure npc */
  if(valid)
  {
    /* Tile placement */
    for(int i = x; i < (w+x); i++)
      for(int j = y; j < (h+y); j++)
        map->tiles[i][j]->setNPC(npc);

    /* Add to stack and emit new signals */
    if(!existing)
    {
      map->npcs.push_back(npc);
      emit npcInstanceChanged(npc->getNameList());
      setHoverThing(-1);

      /* Update path colors and connect signals */
      recolorNPCPaths(map);
      connect(npc->getPath(), SIGNAL(hoverInit(EditorNPCPath*)),
              this, SLOT(npcHoverPathChanged(EditorNPCPath*)));
      if(map == active_submap)
        emit npcPathAdd(npc->getPath());
    }
  }

  return valid;
}

/*
 * Description: Attempts to add the person to the current sub-map. Person needs
 *              to be given an x and y start location prior to calling this
 *              function.
 *
 * Inputs: EditorMapPerson* person - the person to attempt to add
 *         SubMapInfo* map - the map to add the person to
 *         bool existing - false if new person. true otherwise
 * Output: bool - true if the person was added (does not delete if fails)
 */
bool EditorMap::addPerson(EditorMapPerson* person, SubMapInfo* map,
                          bool existing)
{
  int x = person->getX();
  int y = person->getY();
  int w = person->getMatrix()->getWidth();
  int h = person->getMatrix()->getHeight();

  /* Ensure map isn't null. If not set, use active sub-map */
  if(map == NULL)
    map = active_submap;

  /* Check if person can be placed */
  bool valid = false;
  if(x >= 0 && y >= 0 && (x+w) <= map->tiles.size() &&
     (y+h) <= map->tiles[x].size())
  {
    valid = true;

    /* Check each tile */
    for(int i = 0; i < w; i++)
    {
      for(int j = 0; j < h; j++)
      {
        if(!person->isAllNull(i, j))
        {
          int depth = person->getMatrix()->getRenderDepth(i, j);

          if(map->tiles[x+i][y+j]->getPerson(depth) != NULL ||
             map->tiles[x+i][y+j]->getNPC(depth) != NULL)
            valid = false;
        }
      }
    }
  }

  /* If valid, place person */
  if(valid)
  {
    /* Add the person to tiles */
    for(int i = x; i < (w+x); i++)
      for(int j = y; j < (h+y); j++)
        map->tiles[i][j]->setPerson(person);

    /* Add to stack and emit new signals */
    if(!existing)
    {
      map->persons.push_back(person);
      emit personInstanceChanged(person->getNameList());
      setHoverThing(-1);
    }
  }

  return valid;
}

/*
 * Description: Attempts to add the thing to the current sub-map. Thing needs
 *              to be given an x and y start location prior to calling this
 *              function.
 *
 * Inputs: EditorMapThing* thing - the thing to attempt to add
 *         SubMapInfo* map - the map to add the thing to
 *         bool existing - false if new thing. true otherwise
 * Output: bool - true if the thing was added (does not delete if fails)
 */
bool EditorMap::addThing(EditorMapThing* thing, SubMapInfo* map, bool existing)
{
  int x = thing->getX();
  int y = thing->getY();
  int w = thing->getMatrix()->getWidth();
  int h = thing->getMatrix()->getHeight();

  /* Ensure map isn't null */
  if(map == NULL)
    map = active_submap;

  /* Check if thing can be placed */
  bool valid = false;
  if(x >= 0 && y >= 0 && (x+w) <= map->tiles.size() &&
     (y+h) <= map->tiles[x].size())
  {
    valid = true;

    /* Check each tile */
    for(int i = 0; i < w; i++)
      for(int j = 0; j < h; j++)
        if(!thing->isAllNull(i, j))
          if(map->tiles[x+i][y+j]->getThing(
                          thing->getMatrix()->getRenderDepth(i, j)) != NULL)
            valid = false;
  }

  /* If valid, place thing */
  if(valid)
  {
    /* Add the thing to tiles */
    for(int i = x; i < (w+x); i++)
      for(int j = y; j < (h+y); j++)
        map->tiles[i][j]->setThing(thing);

    /* Add to stack and emit new signals */
    if(!existing)
    {
      map->things.push_back(thing);
      emit thingInstanceChanged(thing->getNameList());
      setHoverThing(-1);
    }
  }

  return valid;
}

/*
 * Description: Adds tile pass data to the file handler from the selected sub
 *              map index in the layer and optimizes the data set. The data
 *              set is sorted with the first level is based on the passability
 *              (0-15 = no pass to N,E,S,W) and the second is a stack of all
 *              points of that passability.
 *
 * Inputs: FileHandler* fh - the file handling control pointer
 *         QProgressDialog* save_dialog - dialog for handling save status
 *         QList<QList<QPoint>> data_set - the coordinates of the passability
 * Output: none
 */
void EditorMap::addTilePassData(FileHandler* fh, QProgressDialog* save_dialog,
                                QList<QList<QPoint>> data_set)
{
  /* Optimize */
  QList<QPair<QString,QString>> pass_set =
                                        EditorHelpers::optimizePoints(data_set);

  /* Loop through all x,y string pairs for sprite set */
  for(int i = 1; i < pass_set.size(); i++)
  {
    if(!pass_set[i].first.isEmpty())
    {
      fh->writeXmlElement("x", "index", pass_set[i].first.toStdString());
      fh->writeXmlElement("y", "index", pass_set[i].second.toStdString());
      fh->writeXmlData("passability",
                       EditorHelpers::getPassabilityStr(i).toStdString());
      fh->writeXmlElementEnd();
      fh->writeXmlElementEnd();
    }
  }

  /* Save status */
  save_dialog->setValue(save_dialog->value() + 1);
}

/*
 * Description: Adds tile sprite data to the file handler from the selected sub
 *              map index in the layer and optimizes the data set. The data
 *              set is sorted with the first level is based on the sprite ID
 *              and the second is a stack of all points of that sprite ID.
 *
 * Inputs: FileHandler* fh - the file handling control pointer
 *         QProgressDialog* save_dialog - dialog for handling save status
 *         QList<QList<QPoint>> data_set - the coordinates of the sprites
 * Output: none
 */
void EditorMap::addTileSpriteData(FileHandler* fh, QProgressDialog* save_dialog,
                                  QList<QList<QPoint>> data_set)
{
  /* Optimize */
  QList<QPair<QString,QString>> sprite_set =
                                        EditorHelpers::optimizePoints(data_set);

  /* Loop through all x,y string pairs for sprite set */
  for(int i = 0; i < sprite_set.size(); i++)
  {
    if(!sprite_set[i].first.isEmpty())
    {
      fh->writeXmlElement("x", "index", sprite_set[i].first.toStdString());
      fh->writeXmlElement("y", "index", sprite_set[i].second.toStdString());
      fh->writeXmlData("sprite_id", QString::number(i).toStdString());
      fh->writeXmlElementEnd();
      fh->writeXmlElementEnd();
    }
  }

  /* Save status */
  save_dialog->setValue(save_dialog->value() + 1);
}

/*
 * Description: Clears all set map data and leaves just a clean construct.
 *
 * Inputs: none
 * Output: none
 */
void EditorMap::clearAll()
{
  /* First delete all sub-maps */
  unsetMaps();

  /* Then, Remove all things */
  unsetThings();
  unsetIOs();
  unsetItems();
  unsetPersons();
  unsetNPCs();

  /* Remove all sprites */
  unsetSprites();
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
  /* Clear all prior */
  clearAll();

  /* Add const values */
  name = source.name;

  /* Add sprites */
  for(int i = 0; i < source.sprites.size(); i++)
    sprites.push_back(new EditorSprite(*source.sprites[i]));

  /* Add base things */
  for(int i = 0; i < source.base_things.size(); i++)
    base_things.push_back(new EditorMapThing(*source.base_things[i]));

  /* Add base ios */
  for(int i = 0; i < source.base_ios.size(); i++)
    base_ios.push_back(new EditorMapIO(*source.base_ios[i]));

  /* Add base items */
  for(int i = 0; i < source.base_items.size(); i++)
    base_items.push_back(new EditorMapItem(*source.base_items[i]));

  /* Add base persons */
  for(int i = 0; i < source.base_persons.size(); i++)
    base_persons.push_back(new EditorMapPerson(*source.base_persons[i]));

  /* Add base npcs */
  for(int i = 0; i < source.base_npcs.size(); i++)
    base_npcs.push_back(new EditorMapNPC(*source.base_npcs[i]));

  /* Add sub-maps */
  for(int i = 0; i < source.sub_maps.size(); i++)
  {
    /* Copy the initial tile */
    sub_maps.push_back(new SubMapInfo);
    sub_maps.last()->id = source.sub_maps[i]->id;
    sub_maps.last()->name = source.sub_maps[i]->name;
    sub_maps.last()->path_top = NULL;
    sub_maps.last()->music = source.sub_maps[i]->music;
    sub_maps.last()->weather = source.sub_maps[i]->weather;
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

    /* Add instance map things */
    for(int j = 0; j < source.sub_maps[i]->things.size(); j++)
    {
      EditorMapThing* ref = source.sub_maps[i]->things[j];
      EditorMapThing* t = new EditorMapThing(*ref);
      t->setX(ref->getX());
      t->setY(ref->getY());
      t->setBase(getThing(ref->getBaseThing()->getID()));
      setThing(t, source.sub_maps[i]->id);
    }

    /* Add instance map ios */
    for(int j = 0; j < source.sub_maps[i]->ios.size(); j++)
    {
      EditorMapIO* ref = source.sub_maps[i]->ios[j];
      EditorMapIO* io = new EditorMapIO(*ref);
      io->setX(ref->getX());
      io->setY(ref->getY());
      io->setBase(getIO(ref->getBaseIO()->getID()));
      setIO(io, source.sub_maps[i]->id);
    }

    /* Add instance map items */
    for(int j = 0; j < source.sub_maps[i]->items.size(); j++)
    {
      EditorMapItem* ref = source.sub_maps[i]->items[j];
      EditorMapItem* item = new EditorMapItem(*ref);
      item->setX(ref->getX());
      item->setY(ref->getY());
      item->setBase(getItem(ref->getBaseItem()->getID()));
      setItem(item, source.sub_maps[i]->id);
    }

    /* Add instance map persons */
    for(int j = 0; j < source.sub_maps[i]->persons.size(); j++)
    {
      EditorMapPerson* ref = source.sub_maps[i]->persons[j];
      EditorMapPerson* p = new EditorMapPerson(*ref);
      p->setX(ref->getX());
      p->setY(ref->getY());
      p->setBase(getPerson(ref->getBasePerson()->getID()));
      setPerson(p, source.sub_maps[i]->id);
    }

    /* Add instance map npcs */
    for(int j = 0; j < source.sub_maps[i]->npcs.size(); j++)
    {
      EditorMapNPC* ref = source.sub_maps[i]->npcs[j];
      EditorMapNPC* n = new EditorMapNPC(*ref);
      n->setX(ref->getX());
      n->setY(ref->getY());
      n->setBase(getNPC(ref->getBaseNPC()->getID()));
      setNPC(n, source.sub_maps[i]->id);
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
  /* -------------- NAME -------------- */
  if(element == "name")
  {
    map->name = QString::fromStdString(data.getDataString());
  }
  /* -------------- WIDTH -------------- */
  else if(element == "width")
  {
    setMap(map->id, map->name, data.getDataInteger(),map->tiles.front().size());
  }
  /* -------------- HEIGHT -------------- */
  else if(element == "height")
  {
    setMap(map->id, map->name, map->tiles.size(), data.getDataInteger());
  }
  /* -------------- MUSIC ----------------*/
  else if(element == "music")
  {
    int music_id = data.getDataInteger();
    if(music_id >= 0)
      map->music.push_back(music_id);
  }
  /* -------------- WEATHER ----------------*/
  else if(element == "weather")
  {
    int music_id = data.getDataInteger();
    if(music_id >= 0)
      map->weather = music_id;
  }
  /* -------------- TILES -------------- */
  else if((element == "base" || element == "enhancer" ||
           element == "lower" || element == "upper") &&
          data.getElement(index + 1) == "x" &&
          data.getElement(index + 2) == "y")
  {
    /* Determine the layer */
    EditorEnumDb::Layer layer = EditorEnumDb::NO_LAYER;
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
  /* -------------- TILE EVENTS -------------*/
  else if(element == "tileevent" && data.getElement(index + 1) == "x" &&
          data.getElement(index + 2) == "y")
  {
    /* Get the category */
    QString category = QString::fromStdString(data.getKeyValue(index));
    int x = QString::fromStdString(data.getKeyValue(index + 1)).toInt();
    int y = QString::fromStdString(data.getKeyValue(index + 2)).toInt();

    /* Ensure x/y is in range */
    if(x >= 0 && x < map->tiles.size() && y >= 0 && y < map->tiles[x].size())
    {
      if(category == "enter")
      {
        EditorEventSet* set = map->tiles[x][y]->getEventEnter();
        set->load(data, index + 2);

        // TODO: Remove
        //EditorEvent edit_event(map->tiles[x][y]->getEventEnter());
        //edit_event.load(data, index + 3);
        //if(edit_event.getEvent() != NULL)
        //  map->tiles[x][y]->setEventEnter(*edit_event.getEvent(), false);
        //edit_event.setEventBlank(false);
      }
      else if(category == "enterset")
      {
        EditorEventSet* set = map->tiles[x][y]->getEventEnter();
        set->load(data, index + 3);
      }
      else if(category == "exit")
      {
        EditorEventSet* set = map->tiles[x][y]->getEventExit();
        set->load(data, index + 2);

        // TODO: Remove
        //EditorEvent edit_event(map->tiles[x][y]->getEventExit());
        //edit_event.load(data, index + 3);
        //if(edit_event.getEvent() != NULL)
        //  map->tiles[x][y]->setEventExit(*edit_event.getEvent(), false);
        //edit_event.setEventBlank(false);
      }
      else if(category == "exitset")
      {
        EditorEventSet* set = map->tiles[x][y]->getEventExit();
        set->load(data, index + 3);
      }
    }
  }
  /* -------------- MAP THING -------------- */
  else if(element == "mapthing")
  {
    int thing_id = QString::fromStdString(data.getKeyValue(index)).toInt();
    EditorMapThing* thing = getThing(thing_id, map->id);

    /* Create new thing if it doesn't exist */
    if(thing == NULL)
    {
      thing = new EditorMapThing(thing_id);
      thing->setTileIcons(getTileIcons());

      /* Find insertion location */
      int index = -1;
      bool near = false;
      for(int i = 0; !near && (i < map->things.size()); i++)
      {
        if(map->things[i]->getID() > thing->getID())
        {
          index = i;
          near = true;
        }
      }

      /* If near, insert at index. Otherwise, append */
      if(near)
        map->things.insert(index, thing);
      else
        map->things.append(thing);
    }

    /* Continue to parse the data in the thing */
    QString element = QString::fromStdString(data.getElement(index + 1));
    if(element == "base")
    {
      /* Get name and desc. if it has been changed */
      EditorMapThing default_thing;
      QString default_name = "";
      QString default_desc = "";
      if(default_thing.getName() != thing->getName())
        default_name = thing->getName();
      if(default_thing.getDescription() != thing->getDescription())
        default_desc = thing->getDescription();

      /* Set the base */
      EditorMapThing* base_thing = getThing(data.getDataInteger());
      if(base_thing != NULL)
      {
        thing->setBase(base_thing);
        if(default_name != "")
          thing->setName(default_name);
        if(default_desc != "")
          thing->setDescription(default_desc);
      }
    }
    else
    {
      thing->load(data, index + 1);
    }
  }
  /* -------------- MAP IO -------------- */
  else if(element == "mapio")
  {
    int io_id = QString::fromStdString(data.getKeyValue(index)).toInt();
    EditorMapIO* io = getIO(io_id, map->id);

    /* Create new IO if it doesn't exist */
    if(io == NULL)
    {
      io = new EditorMapIO(io_id);
      io->setTileIcons(getTileIcons());

      /* Find insertion location */
      int index = -1;
      bool near = false;
      for(int i = 0; !near && (i < map->ios.size()); i++)
      {
        if(map->ios[i]->getID() > io->getID())
        {
          index = i;
          near = true;
        }
      }

      /* If near, insert at index. Otherwise, append */
      if(near)
        map->ios.insert(index, io);
      else
        map->ios.append(io);
    }

    /* Continue to parse the data in the thing */
    QString element = QString::fromStdString(data.getElement(index + 1));
    if(element == "base")
    {
      /* Get name and desc. if it has been changed */
      EditorMapIO default_io;
      QString default_name = "";
      QString default_desc = "";
      if(default_io.getName() != io->getName())
        default_name = io->getName();
      if(default_io.getDescription() != io->getDescription())
        default_desc = io->getDescription();

      /* Set the base */
      EditorMapIO* base_io = getIO(data.getDataInteger());
      if(base_io != NULL)
      {
        io->setBase(base_io);
        if(default_name != "")
          io->setName(default_name);
        if(default_desc != "")
          io->setDescription(default_desc);
      }
    }
    else
    {
      io->load(data, index + 1);
    }
  }
  /* -------------- MAP Item -------------- */
  else if(element == "mapitem")
  {
    int item_id = QString::fromStdString(data.getKeyValue(index)).toInt();
    EditorMapItem* item = getItem(item_id, map->id);

    /* Create new item if it doesn't exist */
    if(item == NULL)
    {
      item = new EditorMapItem(item_id);
      item->setTileIcons(getTileIcons());

      /* Find insertion location */
      int index = -1;
      bool near = false;
      for(int i = 0; !near && (i < map->items.size()); i++)
      {
        if(map->items[i]->getID() > item->getID())
        {
          index = i;
          near = true;
        }
      }

      /* If near, insert at index. Otherwise, append */
      if(near)
        map->items.insert(index, item);
      else
        map->items.append(item);
    }

    /* Continue to parse the data in the thing */
    QString element = QString::fromStdString(data.getElement(index + 1));
    if(element == "base")
    {
      /* Get name and desc. if it has been changed */
      EditorMapItem default_item;
      QString default_name = "";
      QString default_desc = "";
      if(default_item.getName() != item->getName())
        default_name = item->getName();
      if(default_item.getDescription() != item->getDescription())
        default_desc = item->getDescription();

      /* Set the base */
      EditorMapItem* base_item = getItem(data.getDataInteger());
      if(base_item != NULL)
      {
        item->setBase(base_item);
        if(default_name != "")
          item->setName(default_name);
        if(default_desc != "")
          item->setDescription(default_desc);
      }
    }
    else
    {
      item->load(data, index + 1);
    }
  }
  /* -------------- MAP PERSON -------------- */
  else if(element == "mapperson")
  {
    int person_id = QString::fromStdString(data.getKeyValue(index)).toInt();
    EditorMapPerson* person = getPerson(person_id, map->id);

    /* Create new person if it doesn't exist */
    if(person == NULL)
    {
      person = new EditorMapPerson(person_id);
      person->setTileIcons(getTileIcons());

      /* Find insertion location */
      int index = -1;
      bool near = false;
      for(int i = 0; !near && (i < map->persons.size()); i++)
      {
        if(map->persons[i]->getID() > person->getID())
        {
          index = i;
          near = true;
        }
      }

      /* If near, insert at index. Otherwise, append */
      if(near)
        map->persons.insert(index, person);
      else
        map->persons.append(person);
    }

    /* Continue to parse the data in the person */
    QString element = QString::fromStdString(data.getElement(index + 1));
    if(element == "base")
    {
      /* Get name and desc. if it has been changed */
      EditorMapPerson default_person;
      QString default_name = "";
      QString default_desc = "";
      if(default_person.getName() != person->getName())
        default_name = person->getName();
      if(default_person.getDescription() != person->getDescription())
        default_desc = person->getDescription();

      /* Set the base */
      EditorMapPerson* base_person = getPerson(data.getDataInteger());
      if(base_person != NULL)
      {
        person->setBase(base_person);
        if(default_name != "")
          person->setName(default_name);
        if(default_desc != "")
          person->setDescription(default_desc);
      }
    }
    else
    {
      person->load(data, index + 1);
    }
  }
  /* -------------- MAP NPC -------------- */
  else if(element == "mapnpc")
  {
    int npc_id = QString::fromStdString(data.getKeyValue(index)).toInt();
    EditorMapNPC* npc = getNPC(npc_id, map->id);

    /* Create new npc if it doesn't exist */
    if(npc == NULL)
    {
      npc = new EditorMapNPC(npc_id);
      npc->setTileIcons(getTileIcons());

      /* Find insertion location */
      int index = -1;
      bool near = false;
      for(int i = 0; !near && (i < map->npcs.size()); i++)
      {
        if(map->npcs[i]->getID() > npc->getID())
        {
          index = i;
          near = true;
        }
      }

      /* If near, insert at index. Otherwise, append */
      if(near)
        map->npcs.insert(index, npc);
      else
        map->npcs.append(npc);
    }

    /* Continue to parse the data in the npc */
    QString element = QString::fromStdString(data.getElement(index + 1));
    if(element == "base")
    {
      /* Get name and desc. if it has been changed */
      EditorMapNPC default_npc;
      QString default_name = "";
      QString default_desc = "";
      if(default_npc.getName() != npc->getName())
        default_name = npc->getName();
      if(default_npc.getDescription() != npc->getDescription())
        default_desc = npc->getDescription();

      /* Set the base */
      EditorMapNPC* base_npc = getNPC(data.getDataInteger());
      if(base_npc != NULL)
      {
        npc->setBase(base_npc);
        if(default_name != "")
          npc->setName(default_name);
        if(default_desc != "")
          npc->setDescription(default_desc);
      }
    }
    else
    {
      npc->load(data, index + 1);
    }
  }
}

/*
 * Description: Re-colors all the paths for each npc in the passed in sub-map.
 *              This will sort the list by x coordinate + y coordinate from
 *              least to greatest and then 1 by 1, assigns one of the 12
 *              available colors.
 *
 * Inputs: SubMapInfo* map - the sub map to update the path colors for
 * Output: none
 */
void EditorMap::recolorNPCPaths(SubMapInfo* map)
{
  QMap<int, EditorNPCPath*> sort_set;

  /* Go through all npc paths and add to sort set */
  for(int i = 0; i < map->npcs.size(); i++)
    sort_set.insertMulti(map->npcs[i]->getX() + map->npcs[i]->getY(),
                         map->npcs[i]->getPath());

  /* With the sorted list, go through and assign colors */
  int color_index = 0;
  QList<EditorNPCPath*> values = sort_set.values();
  for(int i = 0; i < values.size(); i++)
  {
    values[i]->setColorPreset(color_index);

    /* Increment color index and check range */
    color_index++;
    if(values[i]->getColorPresetCount() == color_index)
      color_index = 0;
  }
}

/*
 * Description: Recursively erases all similar adjoining tiles, based on the
 *              target sprite. Stops when either the sprite is not equivalent
 *              or the end of the map is reached.
 *
 * Inputs: int x - the x tile location
 *         int y - the y tile location
 *         EditorEnumDb::Layer layer - the relevant layer to check
 *         EditorSprite* target - the target sprite that must be equal
 *         SubMapInfo* map - the sub-map to erase from
 * Output: none
 */
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
 * Description: Resizes the passed in sub map to the designated width and
 *              height.
 *
 * Inputs: SubMapInfo* map - the non-NULL sub map reference
 *         int width - the new width of the map
 *         int height - the new height of the map
 * Output: bool - true if the resize was successful
 */
bool EditorMap::resizeMap(SubMapInfo* map, int width, int height)
{
  /* Unselect hover tile */
  setHoverTile(nullptr);

  /* Remove all things for processing */
  tilesThingRemove(true);
  tilesIORemove(true);
  tilesItemRemove(true);
  tilesPersonRemove(true);
  tilesNPCRemove(true);

  /* Reference tile */
  EditorTile* ref_tile = map->tiles.front().front();

  /* If smaller, delete tiles on width */
  if(map->tiles.size() > width)
  {
    while(map->tiles.size() > width)
    {
      for(int i = 0; i < map->tiles.back().size(); i++)
        delete map->tiles.back()[i];
      map->tiles.removeLast();
    }
  }
  /* If larger, add tiles on width */
  else if(map->tiles.size() < width)
  {
    for(int i = map->tiles.size(); i < width; i++)
    {
      QVector<EditorTile*> row;

      for(int j = 0; j < map->tiles.front().size(); j++)
      {
        row.push_back(new EditorTile(i, j, getTileIcons()));
        row.last()->setHoverInfo(getHoverInfo());
        row.last()->setVisibilityEvents(ref_tile->getVisibilityEvents());
        row.last()->setVisibilityGrid(ref_tile->getVisibilityGrid());
        row.last()->setVisibilityPass(ref_tile->getVisibilityPass());
      }

      map->tiles.push_back(row);
    }
  }

  /* If smaller, delete tiles on height */
  if(map->tiles.front().size() > height)
  {
    while(map->tiles.front().size() > height)
    {
      for(int i = 0; i < map->tiles.size(); i++)
      {
        delete map->tiles[i].back();
        map->tiles[i].removeLast();
      }
    }
  }
  /* If larger, add tiles on height */
  else if(map->tiles.front().size() < height)
  {
    for(int i = 0; i < map->tiles.size(); i++)
    {
      for(int j = map->tiles[i].size(); j < height; j++)
      {
        map->tiles[i].push_back(new EditorTile(i, j, getTileIcons()));
        map->tiles[i].last()->setHoverInfo(getHoverInfo());
        map->tiles[i].last()->setVisibilityEvents(
                                              ref_tile->getVisibilityEvents());
        map->tiles[i].last()->setVisibilityGrid(ref_tile->getVisibilityGrid());
        map->tiles[i].last()->setVisibilityPass(ref_tile->getVisibilityPass());
      }
    }
  }

  /* Add all things back for processing */
  tilesThingAdd(true);
  tilesIOAdd(true);
  tilesItemAdd(true);
  tilesPersonAdd(true);
  tilesNPCAdd(true);

  return true;
}

/*
 * Description: Saves the sub map pointer passed in to the file handler. If
 *              game only, some parts are skipped. If first, it makes the
 *              sub-map a main.
 *
 * Inputs: FileHandler* fh - the file handling control pointer
 *         QProgressDialog* save_dialog - dialog used for saving status
 *         bool game_only - only game applicable data
 *         SubMapInfo* map - the sub map struct
 *         bool first - only the first used
 * Output: none
 */
void EditorMap::saveSubMap(FileHandler* fh, QProgressDialog* save_dialog,
                           bool game_only, SubMapInfo* map, bool first)
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

  /* Add music and weather IDs */
  for(int i = 0; i < map->music.size(); i++)
    if(map->music[i] >= 0)
      fh->writeXmlData("music", map->music[i]);
  if(map->weather >= 0)
    fh->writeXmlData("weather", map->weather);

  /* Sprite/Pass initial starting point - fill starting arrays */
  QList<QList<QList<QPoint>>> pass_stack;
  QList<QList<QList<QPoint>>> sprite_stack;
  QList<QPoint> blank_stack;
  int max_layer = EditorEnumDb::NO_LAYER;
  int max_pass = EditorHelpers::getPassabilityNum(true, true, true, true);
  int max_sprite = getMaxSpriteID();
  for(int i = 0; i < max_layer; i++)
  {
    QList<QList<QPoint>> sprite_layer;
    QList<QList<QPoint>> pass_layer;

    /* Fill sprites and passability */
    if(sprite_layer.size() == 0)
    {
      for(int j = 0; j <= max_sprite; j++)
        sprite_layer.append(blank_stack);
      for(int j = 0; j <= max_pass; j++)
        pass_layer.append(blank_stack);
    }

    /* Append to parents */
    sprite_stack.append(sprite_layer);
    pass_stack.append(pass_layer);
  }

  /* Event stack starting point */
  QList<QList<QList<int>>> event_stack;
  QList<QList<int>> event_stack_empty;
  event_stack.push_back(event_stack_empty);
  event_stack.push_back(event_stack_empty);
  QList<int> blank_stack2;

  /* Loop through all tiles and sort the data */
  for(int i = 0; i < map->tiles.size(); i++)
  {
    event_stack.front().push_back(blank_stack2);
    event_stack.back().push_back(blank_stack2);

    for(int j = 0; j < map->tiles[i].size(); j++)
    {
      /* Loop through and get all layers */
      for(int k = 0; k < max_layer; k++)
      {
        EditorEnumDb::Layer layer = (EditorEnumDb::Layer)k;
        EditorSprite* sprite = map->tiles[i][j]->getSprite(layer);
        int pass = map->tiles[i][j]->getPassabilityNum(layer);

        /* Sprite stack add */
        if(sprite != NULL)
          sprite_stack[k][sprite->getID()].push_back(QPoint(i, j));

        /* Passability stack add */
        if(pass > 0 && map->tiles[i][j]->getSprite(layer) != NULL)
          pass_stack[k][pass].push_back(QPoint(i, j));
      }

      /* Event add */
      if(map->tiles[i][j]->isEventEnterSet())
        event_stack.front()[i].push_back(j);
      if(map->tiles[i][j]->isEventExitSet())
        event_stack.back()[i].push_back(j);
    }
  }

  /* Add base tiles */
  fh->writeXmlElement("base");
  addTileSpriteData(fh, save_dialog, sprite_stack[(int)EditorEnumDb::BASE]);
  addTilePassData(fh, save_dialog, pass_stack[(int)EditorEnumDb::BASE]);
  fh->writeXmlElementEnd();

  /* Add enhancer tiles */
  fh->writeXmlElement("enhancer");
  addTileSpriteData(fh, save_dialog, sprite_stack[(int)EditorEnumDb::ENHANCER]);
  fh->writeXmlElementEnd();

  /* Add lower tiles */
  fh->writeXmlElement("lower", "index", "0");
  addTileSpriteData(fh, save_dialog, sprite_stack[(int)EditorEnumDb::LOWER1]);
  addTilePassData(fh, save_dialog, pass_stack[(int)EditorEnumDb::LOWER1]);
  fh->writeXmlElementEnd();
  fh->writeXmlElement("lower", "index", "1");
  addTileSpriteData(fh, save_dialog, sprite_stack[(int)EditorEnumDb::LOWER2]);
  addTilePassData(fh, save_dialog, pass_stack[(int)EditorEnumDb::LOWER2]);
  fh->writeXmlElementEnd();
  fh->writeXmlElement("lower", "index", "2");
  addTileSpriteData(fh, save_dialog, sprite_stack[(int)EditorEnumDb::LOWER3]);
  addTilePassData(fh, save_dialog, pass_stack[(int)EditorEnumDb::LOWER3]);
  fh->writeXmlElementEnd();
  fh->writeXmlElement("lower", "index", "3");
  addTileSpriteData(fh, save_dialog, sprite_stack[(int)EditorEnumDb::LOWER4]);
  addTilePassData(fh, save_dialog, pass_stack[(int)EditorEnumDb::LOWER4]);
  fh->writeXmlElementEnd();
  fh->writeXmlElement("lower", "index", "4");
  addTileSpriteData(fh, save_dialog, sprite_stack[(int)EditorEnumDb::LOWER5]);
  addTilePassData(fh, save_dialog, pass_stack[(int)EditorEnumDb::LOWER5]);
  fh->writeXmlElementEnd();

  /* Add upper tiles */
  fh->writeXmlElement("upper", "index", "0");
  addTileSpriteData(fh, save_dialog, sprite_stack[(int)EditorEnumDb::UPPER1]);
  fh->writeXmlElementEnd();
  fh->writeXmlElement("upper", "index", "1");
  addTileSpriteData(fh, save_dialog, sprite_stack[(int)EditorEnumDb::UPPER2]);
  fh->writeXmlElementEnd();
  fh->writeXmlElement("upper", "index", "2");
  addTileSpriteData(fh, save_dialog, sprite_stack[(int)EditorEnumDb::UPPER3]);
  fh->writeXmlElementEnd();
  fh->writeXmlElement("upper", "index", "3");
  addTileSpriteData(fh, save_dialog, sprite_stack[(int)EditorEnumDb::UPPER4]);
  fh->writeXmlElementEnd();
  fh->writeXmlElement("upper", "index", "4");
  addTileSpriteData(fh, save_dialog, sprite_stack[(int)EditorEnumDb::UPPER5]);
  fh->writeXmlElementEnd();

  /* Add enter events */
  fh->writeXmlElement("tileevent", "type", "enterset");
  for(int i = 0; i < event_stack.front().size(); i++)
  {
    bool x_element = false;

    /* Loop through Y elements */
    for(int j = 0; j < event_stack.front()[i].size(); j++)
    {
      EditorTile* t = map->tiles[i][event_stack.front()[i][j]];
      if(t->isEventEnterSet())
      {
        if(!x_element)
        {
          fh->writeXmlElement("x", "index", i);
          x_element = true;
        }
        fh->writeXmlElement("y", "index", event_stack.front()[i][j]);
        t->getEventEnter()->save(fh, game_only, "", true);
        fh->writeXmlElementEnd();
      }
    }

    /* X element end */
    if(x_element)
      fh->writeXmlElementEnd();
  }
  fh->writeXmlElementEnd();

  /* Add exit events */
  fh->writeXmlElement("tileevent", "type", "exitset");
  for(int i = 0; i < event_stack.back().size(); i++)
  {
    bool x_element = false;

    /* Loop through Y elements */
    for(int j = 0; j < event_stack.back()[i].size(); j++)
    {
      EditorTile* t = map->tiles[i][event_stack.back()[i][j]];
      if(t->isEventExitSet())
      {
        if(!x_element)
        {
          fh->writeXmlElement("x", "index", i);
          x_element = true;
        }
        fh->writeXmlElement("y", "index", event_stack.back()[i][j]);
        t->getEventExit()->save(fh, game_only, "", true);
        fh->writeXmlElementEnd();
      }
    }

    /* X element end */
    if(x_element)
      fh->writeXmlElementEnd();
  }
  fh->writeXmlElementEnd();

  /* Add things */
  for(int i = 0; i < map->things.size(); i++)
  {
    map->things[i]->save(fh, game_only);
    save_dialog->setValue(save_dialog->value()+1);
  }

  /* Add ios */
  for(int i = 0; i < map->ios.size(); i++)
  {
    map->ios[i]->save(fh, game_only);
    save_dialog->setValue(save_dialog->value()+1);
  }

  /* Add items */
  for(int i = 0; i < map->items.size(); i++)
  {
    map->items[i]->save(fh, game_only);
    save_dialog->setValue(save_dialog->value()+1);
  }

  /* Add persons */
  for(int i = 0; i < map->persons.size(); i++)
  {
    map->persons[i]->save(fh, game_only);
    save_dialog->setValue(save_dialog->value()+1);
  }

  /* Add npcs */
  for(int i = 0; i < map->npcs.size(); i++)
  {
    map->npcs[i]->save(fh, game_only);
    save_dialog->setValue(save_dialog->value()+1);
  }

  /* End element */
  fh->writeXmlElementEnd();
}

/*
 * Description: Sets the hover thing, being flagged when selecting an instance
 *              in the list in View.
 *
 * Inputs: EditorMapThing* thing - the thing to set as hover
 * Output: bool - true if the instance was changed (unsets the old regardless)
 */
bool EditorMap::setHoverThing(EditorMapThing* thing)
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

/*
 * Description: Updates the tiles that contain the hover information with the
 *              relevant thing. If unset is false, it sets it to display.
 *              Otherwise, it unsets it and stops it from displaying. This works
 *              for things and all children (person, etc)
 *
 * Inputs: bool unset - unset the hover thing?
 * Output: bool - true if the hover thing was updated
 */
bool EditorMap::updateHoverThing(bool unset)
{
  /* Determine the thing to work with */
  EditorMapThing* thing = NULL;
  if(active_submap != NULL && active_info.hover_tile != NULL &&
     active_info.active_cursor == EditorEnumDb::BASIC)
  {
    if(active_info.active_layer == EditorEnumDb::THING)
      thing = active_info.active_thing;
    else if(active_info.active_layer == EditorEnumDb::IO)
      thing = active_info.active_io;
    else if(active_info.active_layer == EditorEnumDb::ITEM)
      thing = active_info.active_item;
    else if(active_info.active_layer == EditorEnumDb::PERSON)
      thing = active_info.active_person;
    else if(active_info.active_layer == EditorEnumDb::NPC)
      thing = active_info.active_npc;
  }

  /* Pre-checks to determine if this is valid to execute */
  if(thing != NULL)
  {
    SubMapInfo* map = active_submap;
    EditorMatrix* ref = thing->getMatrix();
    int hover_x = active_info.hover_tile->getX();
    int hover_y = active_info.hover_tile->getY();
    int hover_w = hover_x + ref->getWidth();
    int hover_h = hover_y + ref->getHeight();

    /* If unset, the hover is being removed from the tiles */
    if(unset)
    {
      /* Go through and unset hover on all */
      for(int i = hover_x; (i < hover_w && i < map->tiles.size()); i++)
        for(int j = hover_y; (j < hover_h && j < map->tiles[i].size()); j++)
          map->tiles[i][j]->setHover(false);

      /* Re-set on base hover tile */
      active_info.hover_tile->setHover(true);
    }
    /* Otherwise, it is being added */
    else if(!active_info.path_edit_mode)
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
        for(int j = hover_y; (j < hover_h && j < map->tiles[i].size()); j++)
          map->tiles[i][j]->setHover(true, invalid);
    }

    return true;
  }
  return false;
}

/*
 * Description: Updates the given tile range within the sub-map
 *
 * Inputs: int x - the x top left tile location
 *         int y - the y top left tile location
 *         int w - the width of tiles for the update
 *         int h - the height of tiles for the update
 * Output: none
 */
void EditorMap::updateTiles(SubMapInfo* map, int x, int y, int w, int h)
{
  if(map != nullptr)
  {
    for(int i = x; i < map->tiles.size() && i < (x + w); i++)
      for(int j = y; j < map->tiles[i].size() && j < (y + h); j++)
        map->tiles[i][j]->update();
  }
}

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/

/*
 * Description: Slot which fires each time the hover npc on the active sub-map
 *              changes. This is used to change the rendering depth to push all
 *              nodes of the hover path to the top.
 *
 * Inputs: EditorNPCPath* path - the hovered path reference
 * Output: none
 */
void EditorMap::npcHoverPathChanged(EditorNPCPath* path)
{
  if(active_submap != NULL && path != NULL)
  {
    /* Set the top npc back to default */
    if(active_submap->path_top != NULL)
      active_submap->path_top->setZValue(1);
    active_submap->path_top = NULL;

    /* Set the new npc path as the top */
    path->setZValue(2);
    active_submap->path_top = path;
  }
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Clears the hover information for the editor map. Called on
 *              initial construction and each time the editor map is laoded in
 *              as the display map.
 *
 * Inputs: none
 * Output: none
 */
void EditorMap::clearHoverInfo()
{
  active_info.active_cursor = EditorEnumDb::NO_CURSOR;
  active_info.active_layer = EditorEnumDb::NO_LAYER;
  active_info.path_edit_mode = false;

  active_info.active_io = NULL;
  active_info.active_item = NULL;
  active_info.active_npc = NULL;
  active_info.active_person = NULL;
  active_info.active_sprite = NULL;
  active_info.active_thing = NULL;

  active_info.hover_tile = NULL;
  active_info.selected_thing = QRect();

  /* Clean the active submap */
  active_submap = NULL;
}

/*
 * Description: This is the click trigger, passed in by the map rendering
 *              engine. If it's a single executing task (fill), single is true
 *              If it's right click, right_click is true.
 *
 * Inputs: bool single - was this triggered by a click? false if holding click
 *                       and changing hover tile
 *         bool right_click - true if the trigger was a right click
 * Output: none
 */
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
        EditorMapThing* new_thing = new EditorMapThing(id);
        new_thing->setBase(active_info.active_thing);
        new_thing->setX(active_info.hover_tile->getX());
        new_thing->setY(active_info.hover_tile->getY());

        /* Attempt to place - if failed, delete */
        if(!addThing(new_thing, NULL, false))
          delete new_thing;
      }
      else if(cursor == EditorEnumDb::ERASER)
      {
        int max = Helpers::getRenderDepth();

        /* Loop through all to find the top thing */
        EditorMapThing* found = NULL;
        for(int i = max - 1; found == NULL && i >= 0; i--)
          found = active_info.hover_tile->getThing(i);

        /* If found, remove from tiles and delete */
        if(found != NULL)
          unsetThing(found->getID(), true);
      }
    }
    /* Check on the layer - io */
    else if(layer == EditorEnumDb::IO)
    {
      /* Check cursor */
      if(cursor == EditorEnumDb::BASIC && active_info.active_io != NULL)
      {
        /* Create the IO */
        int id = getNextIOID(true);
        EditorMapIO* new_io = new EditorMapIO(id);
        new_io->setBase(active_info.active_io);
        new_io->setX(active_info.hover_tile->getX());
        new_io->setY(active_info.hover_tile->getY());

        /* Attempt to place - if failed, delete */
        if(!addIO(new_io, NULL, false))
          delete new_io;
      }
      else if(cursor == EditorEnumDb::ERASER)
      {
        int max = Helpers::getRenderDepth();

        /* Loop through all to find the top IO */
        EditorMapIO* found = NULL;
        for(int i = max - 1; found == NULL && i >= 0; i--)
          found = active_info.hover_tile->getIO(i);

        /* If found, remove from tiles and delete */
        if(found != NULL)
          unsetIO(found->getID(), true);
      }
    }
    /* Check on the layer - item */
    else if(layer == EditorEnumDb::ITEM)
    {
      /* Check cursor */
      if(cursor == EditorEnumDb::BASIC && active_info.active_item != NULL)
      {
        /* Create the thing */
        int id = getNextItemID(true);
        EditorMapItem* new_item = new EditorMapItem(id);
        new_item->setBase(active_info.active_item);
        new_item->setX(active_info.hover_tile->getX());
        new_item->setY(active_info.hover_tile->getY());

        /* Attempt to place - if failed, delete */
        if(!addItem(new_item, NULL, false))
          delete new_item;
      }
      else if(cursor == EditorEnumDb::ERASER)
      {
        /* Loop through all to find the top item */
        EditorMapItem* found = NULL;
        if(active_info.hover_tile->getItems().front() != NULL)
          found = active_info.hover_tile->getItems().last();

        /* If found, remove from tiles and delete */
        if(found != NULL)
          unsetItem(found->getID(), true);
      }
    }
    /* Check on the layer - person */
    else if(layer == EditorEnumDb::PERSON)
    {
      /* Check cursor */
      if(cursor == EditorEnumDb::BASIC && active_info.active_person != NULL)
      {
        /* Create the person */
        int id = getNextPersonID(true);
        EditorMapPerson* new_person = new EditorMapPerson(id);
        new_person->setBase(active_info.active_person);
        new_person->setX(active_info.hover_tile->getX());
        new_person->setY(active_info.hover_tile->getY());

        /* Attempt to place - if failed, delete */
        if(!addPerson(new_person, NULL, false))
          delete new_person;
      }
      else if(cursor == EditorEnumDb::ERASER)
      {
        int max = Helpers::getRenderDepth();

        /* Loop through all to find the top person */
        EditorMapPerson* found = NULL;
        for(int i = max - 1; found == NULL && i >= 0; i--)
          found = active_info.hover_tile->getPerson(i);

        /* If found, remove from tiles and delete */
        if(found != NULL)
          unsetPerson(found->getID(), true);
      }
    }
    /* Check on the layer - npc */
    else if(layer == EditorEnumDb::NPC)
    {
      /* Check cursor */
      if(cursor == EditorEnumDb::BASIC && active_info.active_npc != NULL)
      {
        /* Create the npc */
        int id = getNextNPCID(true);
        EditorMapNPC* new_npc = new EditorMapNPC(id);
        new_npc->setBase(active_info.active_npc);
        new_npc->setX(active_info.hover_tile->getX());
        new_npc->setY(active_info.hover_tile->getY());

        /* Attempt to place - if failed, delete */
        if(!addNPC(new_npc, NULL, false))
          delete new_npc;
      }
      else if(cursor == EditorEnumDb::ERASER)
      {
        int max = Helpers::getRenderDepth();

        /* Loop through all to find the top npc */
        EditorMapNPC* found = NULL;
        for(int i = max - 1; found == NULL && i >= 0; i--)
          found = active_info.hover_tile->getNPC(i);

        /* If found, remove from tiles and delete */
        if(found != NULL)
          unsetNPC(found->getID(), true);
      }
    }
  }
}

/*
 * Description: Click trigger from executing a rectangle select pen.
 *              Passes in all tiles that were below the rect. Erase is true
 *              if it was a right to left rect. Otherwise, place active sprite.
 *
 * Inputs: QList<EditorTile*> tiles - the tile stack under the rect
 *         bool erase - true if the rect is an erase block
 * Output: none
 */
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
bool EditorMap::copySubMap(SubMapInfo* copy_map, SubMapInfo* new_map)
{
  if(copy_map != NULL && new_map != NULL)
  {
    new_map->name = copy_map->name;
    new_map->music = copy_map->music;
    new_map->weather = copy_map->weather;

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
        row.push_back(new EditorTile(i, j, tile_icons));
        *row.last() = *copy_map->tiles[i][j];
        row.last()->setHoverInfo(copy_map->tiles[i][j]->getHoverInfo());
      }

      new_map->tiles.push_back(row);
    }

    /* Add thing instances */
    while(new_map->things.size() > 0)
      unsetThingByIndex(0, new_map->id);
    for(int i = 0; i < copy_map->things.size(); i++)
    {
      EditorMapThing* thing = new EditorMapThing(*copy_map->things[i]);
      thing->setID(getNextThingID(true));
      thing->setX(copy_map->things[i]->getX());
      thing->setY(copy_map->things[i]->getY());
      setThing(thing, new_map->id);
    }

    /* Add io instances */
    while(new_map->ios.size() > 0)
      unsetIOByIndex(0, new_map->id);
    for(int i = 0; i < copy_map->ios.size(); i++)
    {
      EditorMapIO* io = new EditorMapIO(*copy_map->ios[i]);
      io->setID(getNextIOID(true));
      io->setX(copy_map->ios[i]->getX());
      io->setY(copy_map->ios[i]->getY());
      setIO(io, new_map->id);
    }

    /* Add item instances */
    while(new_map->items.size() > 0)
      unsetItemByIndex(0, new_map->id);
    for(int i = 0; i < copy_map->items.size(); i++)
    {
      EditorMapItem* item = new EditorMapItem(*copy_map->items[i]);
      item->setID(getNextItemID(true));
      item->setX(copy_map->items[i]->getX());
      item->setY(copy_map->items[i]->getY());
      setItem(item, new_map->id);
    }

    /* Add person instances */
    while(new_map->persons.size() > 0)
      unsetPersonByIndex(0, new_map->id);
    for(int i = 0; i < copy_map->persons.size(); i++)
    {
      EditorMapPerson* person = new EditorMapPerson(*copy_map->persons[i]);
      person->setID(getNextPersonID(true));
      person->setX(copy_map->persons[i]->getX());
      person->setY(copy_map->persons[i]->getY());
      setPerson(person, new_map->id);
    }

    /* Add npc instances */
    while(new_map->npcs.size() > 0)
      unsetNPCByIndex(0, new_map->id);
    for(int i = 0; i < copy_map->npcs.size(); i++)
    {
      EditorMapNPC* npc = new EditorMapNPC(*copy_map->npcs[i]);
      npc->setID(getNextNPCID(true));
      npc->setX(copy_map->npcs[i]->getX());
      npc->setY(copy_map->npcs[i]->getY());
      setNPC(npc, new_map->id);
    }

    return true;
  }
  return false;
}

/*
 * Description: Ctrl click on tile switches selected thing/sprite to the one
 *              on the top of the selected tile.
 *
 * Inputs: none
 * Output: none
 */
void EditorMap::ctrlClickTrigger()
{
  EditorEnumDb::Layer layer = active_info.active_layer;
  EditorTile* tile = active_info.hover_tile;

  if(tile != NULL)
  {
    bool finished = false;

    /* ---- THINGS ---- */
    if(layer == EditorEnumDb::THING)
    {
      QVector<EditorMapThing*> set = tile->getThings();
      for(int i = set.size() - 1; !finished && i >= 0; i--)
      {
        if(set[i] != NULL)
        {
          emit thingInstanceChanged(set[i]->getNameList());
          finished = true;
        }
      }
    }
    /* ---- IOS ---- */
    else if(layer == EditorEnumDb::IO)
    {
      QVector<EditorMapIO*> set = tile->getIOs();
      for(int i = set.size() - 1; !finished && i >= 0; i--)
      {
        if(set[i] != NULL)
        {
          emit ioInstanceChanged(set[i]->getNameList());
          finished = true;
        }
      }
    }
    /* ---- ITEMS ---- */
    else if(layer == EditorEnumDb::ITEM)
    {
      QVector<EditorMapItem*> set = tile->getItems();
      for(int i = set.size() - 1; !finished && i >= 0; i--)
      {
        if(set[i] != NULL)
        {
          emit itemInstanceChanged(set[i]->getNameList());
          finished = true;
        }
      }
    }
    /* ---- PERSONS ---- */
    else if(layer == EditorEnumDb::PERSON)
    {
      QVector<EditorMapPerson*> set = tile->getPersons();
      for(int i = set.size() - 1; !finished && i >= 0; i--)
      {
        if(set[i] != NULL)
        {
          emit personInstanceChanged(set[i]->getNameList());
          finished = true;
        }
      }
    }
    /* ---- NPCS ---- */
    else if(layer == EditorEnumDb::NPC)
    {
      QVector<EditorMapNPC*> set = tile->getNPCs();
      for(int i = set.size() - 1; !finished && i >= 0; i--)
      {
        if(set[i] != NULL)
        {
          emit npcInstanceChanged(set[i]->getNameList());
          finished = true;
        }
      }
    }
    else
    {
      EditorSprite* sprite = tile->getSprite(layer);
      if(sprite != NULL)
        emit spriteIndexChange(getSpriteIndex(sprite->getID()));
    }
  }
}

/*
 * Description: Returns the current selected base IO in the list of IOs.
 *              On click, if the layer and pen is correct, this is the IO
 *              that instantized and then placed.
 *
 * Inputs: none
 * Output: int - the index in the IO stack
 */
int EditorMap::getCurrentIOIndex()
{
  /* Ensure the active IO isn't null */
  if(active_info.active_io != NULL)
  {
    for(int i = 0; i < base_ios.size(); i++)
      if(base_ios[i] == active_info.active_io)
        return i;

    /* IO not found, it doesn't exist -> nullify it */
    setCurrentIO(-1);
  }

  return -1;
}

/*
 * Description: Returns the current selected base item in the list of items.
 *              On click, if the layer and pen is correct, this is the item
 *              that instantized and then placed.
 *
 * Inputs: none
 * Output: int - the index in the item stack
 */
int EditorMap::getCurrentItemIndex()
{
  /* Ensure the active item isn't null */
  if(active_info.active_item != NULL)
  {
    for(int i = 0; i < base_items.size(); i++)
      if(base_items[i] == active_info.active_item)
        return i;

    /* Item not found, it doesn't exist -> nullify it */
    setCurrentItem(-1);
  }

  return -1;
}

/*
 * Description: Returns the current sub-map being rendered. Controlled by
 *              MapControl class.
 *
 * Inputs: none
 * Output: SubMapInfo* - the sub-map struct pointer
 */
SubMapInfo* EditorMap::getCurrentMap()
{
  return active_submap;
}

/*
 * Description: Returns the current sub-map index being rendered. Controlled by
 *              MapControl class.
 *
 * Inputs: none
 * Output: int - index in the sub-map stack. -1 if not set
 */
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

/*
 * Description: Returns the current selected base npc in the list of npcs.
 *              On click, if the layer and pen is correct, this is the npc
 *              that instantized and then placed.
 *
 * Inputs: none
 * Output: int - the index in the npc stack
 */
int EditorMap::getCurrentNPCIndex()
{
  /* Ensure the active npc isn't null */
  if(active_info.active_npc != NULL)
  {
    for(int i = 0; i < base_npcs.size(); i++)
      if(base_npcs[i] == active_info.active_npc)
        return i;

    /* NPC not found, it doesn't exist -> nullify it */
    setCurrentNPC(-1);
  }

  return -1;
}

/*
 * Description: Returns the current selected base person in the list of persons.
 *              On click, if the layer and pen is correct, this is the person
 *              that instantized and then placed.
 *
 * Inputs: none
 * Output: int - the index in the person stack
 */
int EditorMap::getCurrentPersonIndex()
{
  /* Ensure the active person isn't null */
  if(active_info.active_person != NULL)
  {
    for(int i = 0; i < base_persons.size(); i++)
      if(base_persons[i] == active_info.active_person)
        return i;

    /* Person not found, it doesn't exist -> nullify it */
    setCurrentPerson(-1);
  }

  return -1;
}

/*
 * Description: Returns the current selected sprite index in the list of
 *              sprites. On click, this is the sprite that is placed.
 *
 * Inputs: none
 * Output: int - the index in the sprite stack
 */
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

/*
 * Description: Returns the current selected base thing in the list of things.
 *              On click, if the layer and pen is correct, this is the thing
 *              that instantized and then placed.
 *
 * Inputs: none
 * Output: int - the index in the thing stack
 */
int EditorMap::getCurrentThingIndex()
{
  /* Ensure the active thing isn't null */
  if(active_info.active_thing != NULL)
  {
    for(int i = 0; i < base_things.size(); i++)
      if(base_things[i] == active_info.active_thing)
        return i;

    /* thing not found, it doesn't exist -> nullify it */
    setCurrentThing(-1);
  }

  return -1;
}

/*
 * Description: Returns the hover information of the current map. Contains all
 *              relevant data, such as hover tile, hover thing, sprite, etc.
 *
 * Inputs: none
 * Output: HoverInfo* - ref to hover info struct
 */
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
 * Description: Returns the IO with the corresponding ID.
 *
 * Inputs: int id - the id of the IO to get
 *         int sub_map - the sub-map to get the IOs for (<0 is base)
 * Output: EditorMapIO* - the pointer to match the ID. NULL if not found.
 */
EditorMapIO* EditorMap::getIO(int id, int sub_map)
{
  if(id >= 0)
  {
    /* If sub map ref is less than 0, get from base set */
    if(sub_map < 0)
    {
      for(int i = 0; i < base_ios.size(); i++)
        if(base_ios[i]->getID() == id)
          return base_ios[i];
    }
    /* Otherwise, get from sub map */
    else if(sub_map < sub_maps.size())
    {
      for(int i = 0; i < sub_maps[sub_map]->ios.size(); i++)
        if(sub_maps[sub_map]->ios[i]->getID() == id)
          return sub_maps[sub_map]->ios[i];
    }
  }

  return NULL;
}

/*
 * Description: Returns the IO at the corresponding index in the list.
 *
 * Inputs: int index - the index in the array (0 to size - 1)
 *         int sub_map - the sub-map to get the IOs for (<0 is base)
 * Output: EditorMapIO* - the pointer to match the index. NULL if out of
 *                        range
 */
EditorMapIO* EditorMap::getIOByIndex(int index, int sub_map)
{
  /* If sub map ref is less than 0, get from base set */
  if(sub_map < 0)
  {
    if(index >= 0 && index < base_ios.size())
      return base_ios[index];
  }
  /* Otherwise, get from sub map */
  else if(sub_map < sub_maps.size())
  {
    if(index >= 0 && index < sub_maps[sub_map]->ios.size())
      return sub_maps[sub_map]->ios[index];
  }
  return NULL;
}

/*
 * Description: Returns the number of IOs in the list.
 *
 * Inputs: int sub_map - the sub-map to get the IOs for (<0 is base)
 * Output: int - the number of IOs in the map set
 */
int EditorMap::getIOCount(int sub_map)
{
  /* If sub map ref is less than 0, get from base set */
  if(sub_map < 0)
    return base_ios.size();
  /* Otherwise, get from sub map */
  else if(sub_map < sub_maps.size())
    return sub_maps[sub_map]->ios.size();
  return 0;
}

/*
 * Description: Returns the index in the list of IOs of the matching id.
 *              Less than 0 if none match.
 *
 * Inputs: int id - the id to find the index for
 *         int sub_map - the sub-map to get the IOs for (<0 is base)
 * Output: int - the index of the IO. Less than 0 if nothing matches
 */
int EditorMap::getIOIndex(int id, int sub_map)
{
  if(id >= 0)
  {
    /* If sub map ref is less than 0, get from base set */
    if(sub_map < 0)
    {
      for(int i = 0; i < base_ios.size(); i++)
        if(base_ios[i]->getID() == id)
          return i;
    }
    /* Otherwise, get from sub map */
    else if(sub_map < sub_maps.size())
    {
      for(int i = 0; i < sub_maps[sub_map]->ios.size(); i++)
        if(sub_maps[sub_map]->ios[i]->getID() == id)
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
 *         bool shortened - should the sub-map name be shortened (just ID:NAME
 *                          instead of BASEID(ID):NAME).
 * Output: QVector<QString> - list of all things
 */
QVector<QString> EditorMap::getIOList(int sub_map, bool all_submaps,
                                      bool shortened)
{
  QVector<QString> stack;
  stack.push_back("-- INTERACTIVE OBJECTS --");

  /* If sub map ref is less than 0, get from base set */
  if(sub_map < 0)
  {
    /* Go through all ios and add them to the list */
    for(int i = 0; i < base_ios.size(); i++)
      stack.push_back(base_ios[i]->getNameList());
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
      for(int j = 0; j < sub_maps[i]->ios.size(); j++)
        stack.push_back(sub_maps[i]->ios[j]->getNameList(shortened));
  }

  return stack;
}

/* Returns stored IO information */
// TODO: Comment
QVector<QPair<QString,QString>> EditorMap::getIOListSet(int sub_map,
                                                        bool all_submaps,
                                                        bool shortened)
{
  QVector<QPair<QString,QString>> set;
  //set.push_back(QPair<QString,QString>("-- INTERACTIVE OBJECTS --", ""));

  /* If sub map ref is less than 0, get from base set */
  if(sub_map < 0)
  {
    /* Go through all ios and add them to the list */
    for(int i = 0; i < base_ios.size(); i++)
      set.push_back(QPair<QString,QString>(base_ios[i]->getNameList(),
                                           base_ios[i]->getStateList()));
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
      for(int j = 0; j < sub_maps[i]->ios.size(); j++)
        set.push_back(QPair<QString,QString>(
                        sub_maps[i]->ios[j]->getNameList(shortened),
                        sub_maps[i]->ios[j]->getStateList()));
  }

  return set;
}

/*
 * Description: Returns the list of all IOs in the editor map
 *
 * Inputs: int sub_map - the sub-map to get the IOs for (<0 is base)
 * Output: QVector<EditorMapIO*> - list of all IOs
 */
QVector<EditorMapIO*> EditorMap::getIOs(int sub_map)
{
  /* If sub map ref is less than 0, get from base set */
  if(sub_map < 0)
  {
    return base_ios;
  }
  /* Otherwise, get from sub map */
  else if(sub_map < sub_maps.size())
  {
    return sub_maps[sub_map]->ios;
  }

  /* Otherwise, return blank list */
  QVector<EditorMapIO*> blank_list;
  return blank_list;
}

/*
 * Description: Returns the item with the corresponding ID.
 *
 * Inputs: int id - the id of the item to get
 *         int sub_map - the sub-map to get the items for (<0 is base)
 * Output: EditorMapItem* - the pointer to match the ID. NULL if not found.
 */
EditorMapItem* EditorMap::getItem(int id, int sub_map)
{
  if(id >= 0)
  {
    /* If sub map ref is less than 0, get from base set */
    if(sub_map < 0)
    {
      for(int i = 0; i < base_items.size(); i++)
        if(base_items[i]->getID() == id)
          return base_items[i];
    }
    /* Otherwise, get from sub map */
    else if(sub_map < sub_maps.size())
    {
      for(int i = 0; i < sub_maps[sub_map]->items.size(); i++)
        if(sub_maps[sub_map]->items[i]->getID() == id)
          return sub_maps[sub_map]->items[i];
    }
  }

  return NULL;
}

/*
 * Description: Returns the item at the corresponding index in the list.
 *
 * Inputs: int index - the index in the array (0 to size - 1)
 *         int sub_map - the sub-map to get the items for (<0 is base)
 * Output: EditorMapItem* - the pointer to match the index. NULL if out of
 *                         range
 */
EditorMapItem* EditorMap::getItemByIndex(int index, int sub_map)
{
  /* If sub map ref is less than 0, get from base set */
  if(sub_map < 0)
  {
    if(index >= 0 && index < base_items.size())
      return base_items[index];
  }
  /* Otherwise, get from sub map */
  else if(sub_map < sub_maps.size())
  {
    if(index >= 0 && index < sub_maps[sub_map]->items.size())
      return sub_maps[sub_map]->items[index];
  }
  return NULL;
}

/*
 * Description: Returns the number of items in the list.
 *
 * Inputs: int sub_map - the sub-map to get the items for (<0 is base)
 * Output: int - the number of items in the map set
 */
int EditorMap::getItemCount(int sub_map)
{
  /* If sub map ref is less than 0, get from base set */
  if(sub_map < 0)
    return base_items.size();
  /* Otherwise, get from sub map */
  else if(sub_map < sub_maps.size())
    return sub_maps[sub_map]->items.size();
  return 0;
}

/*
 * Description: Returns the index in the list of items of the matching id.
 *              Less than 0 if none match.
 *
 * Inputs: int id - the id to find the index for
 *         int sub_map - the sub-map to get the items for (<0 is base)
 * Output: int - the index of the item. Less than 0 if no item matches
 */
int EditorMap::getItemIndex(int id, int sub_map)
{
  if(id >= 0)
  {
    /* If sub map ref is less than 0, get from base set */
    if(sub_map < 0)
    {
      for(int i = 0; i < base_items.size(); i++)
        if(base_items[i]->getID() == id)
          return i;
    }
    /* Otherwise, get from sub map */
    else if(sub_map < sub_maps.size())
    {
      for(int i = 0; i < sub_maps[sub_map]->items.size(); i++)
        if(sub_maps[sub_map]->items[i]->getID() == id)
          return i;
    }
  }
  return -1;
}

/*
 * Description: Returns a list of all items in the format of a HEADER row
 *              with the following rows as "ID: NAME".
 *
 * Inputs: int sub_map - the sub-map to get the items for (<0 is base)
 *         bool all_submaps - true if all sub-maps should be stacked together
 *         bool shortened - should the sub-map name be shortened (just ID:NAME
 *                          instead of BASEID(ID):NAME).
 * Output: QVector<QString> - list of all items
 */
QVector<QString> EditorMap::getItemList(int sub_map, bool all_submaps,
                                        bool shortened)
{
  QVector<QString> stack;
  stack.push_back("-- ITEMS --");

  /* If sub map ref is less than 0, get from base set */
  if(sub_map < 0)
  {
    /* Go through all items and add them to the list */
    for(int i = 0; i < base_items.size(); i++)
      stack.push_back(base_items[i]->getNameList());
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
      for(int j = 0; j < sub_maps[i]->items.size(); j++)
        stack.push_back(sub_maps[i]->items[j]->getNameList(shortened));
  }

  return stack;
}

/*
 * Description: Returns the list of all items in the editor map
 *
 * Inputs: int sub_map - the sub-map to get the items for (<0 is base)
 * Output: QVector<EditorMapItem*> - list of all items
 */
QVector<EditorMapItem*> EditorMap::getItems(int sub_map)
{
  /* If sub map ref is less than 0, get from base set */
  if(sub_map < 0)
  {
    return base_items;
  }
  /* Otherwise, get from sub map */
  else if(sub_map < sub_maps.size())
  {
    return sub_maps[sub_map]->items;
  }

  /* Otherwise, return blank list */
  QVector<EditorMapItem*> blank_list;
  return blank_list;
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
 * Description: Returns the highest sprite ID in the map set of sprites.
 *
 * Inputs: none
 * Output: int - the max sprite. -1 if no sprites found
 */
int EditorMap::getMaxSpriteID()
{
  int max = -1;

  /* Loop through all sprites */
  for(int i = 0; i < sprites.size(); i++)
    if(sprites[i]->getID() > max)
      max = sprites[i]->getID();

  return max;
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
 * Description: Returns the next available IO ID that can be used for a new
 *              IO.
 *
 * Inputs: bool from_sub - true if it should be unset in sub-maps instead
 * Output: int - the id to use
 */
int EditorMap::getNextIOID(bool from_sub)
{
  bool found = false;
  int id = 0;

  /* If not from sub map, check base for base ID */
  if(!from_sub)
  {
    for(int i = 0; !found && (i < base_ios.size()); i++)
    {
      if(base_ios[i]->getID() != i)
      {
        id = i;
        found = true;
      }
    }

    /* If nothing found, just make it the last ID + 1 */
    if(!found && base_ios.size() > 0)
      id = base_ios.last()->getID() + 1;
  }
  /* Otherwise, check the sub-maps for available ID */
  else
  {
    /* Compile the IDs of all IOs in all sub-maps */
    QVector<int> id_list;
    for(int i = 0; i < sub_maps.size(); i++)
      for(int j = 0; j < sub_maps[i]->ios.size(); j++)
        id_list.push_back(sub_maps[i]->ios[j]->getID());

    /* Sort the list */
    qSort(id_list);

    /* Find the next available ID */
    id = EditorEnumDb::kBASE_ID_IOS;
    for(int i = 0; !found && (i < id_list.size()); i++)
    {
      if(id_list[i] != (id + i))
      {
        id += i;
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
 * Description: Returns the next available item ID that can be used for a new
 *              item.
 *
 * Inputs: none
 * Output: int - the id to use
 */
int EditorMap::getNextItemID(bool from_sub)
{
  bool found = false;
  int id = 0;

  /* If not from sub map, check base for base ID */
  if(!from_sub)
  {
    for(int i = 0; !found && (i < base_items.size()); i++)
    {
      if(base_items[i]->getID() != i)
      {
        id = i;
        found = true;
      }
    }

    /* If nothing found, just make it the last ID + 1 */
    if(!found && base_items.size() > 0)
      id = base_items.last()->getID() + 1;
  }
  /* Otherwise, check the sub-maps for available ID */
  else
  {
    /* Compile the IDs of all things in all sub-maps */
    QVector<int> id_list;
    for(int i = 0; i < sub_maps.size(); i++)
      for(int j = 0; j < sub_maps[i]->items.size(); j++)
        id_list.push_back(sub_maps[i]->items[j]->getID());

    /* Sort the list */
    qSort(id_list);

    /* Find the next available ID */
    id = EditorEnumDb::kBASE_ID_ITEMS;
    for(int i = 0; !found && (i < id_list.size()); i++)
    {
      if(id_list[i] != (id + i))
      {
        id += i;
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
 * Description: Returns the next available npc ID that can be used for a new
 *              npc.
 *
 * Inputs: bool from_sub - true if it should be unset in sub-maps instead
 * Output: int - the id to use
 */
int EditorMap::getNextNPCID(bool from_sub)
{
  bool found = false;
  int id = EditorEnumDb::kBASE_ID_NPC;

  /* If not from sub map, check base for base ID */
  if(!from_sub)
  {
    for(int i = 0; !found && (i < base_npcs.size()); i++)
    {
      if(base_npcs[i]->getID() != (id + i))
      {
        id += i;
        found = true;
      }
    }

    /* If nothing found, just make it the last ID + 1 */
    if(!found && base_npcs.size() > 0)
      id = base_npcs.last()->getID() + 1;
  }
  /* Otherwise, check the sub-maps for available ID */
  else
  {
    /* Compile the IDs of all npcs in all sub-maps */
    QVector<int> id_list;
    for(int i = 0; i < sub_maps.size(); i++)
      for(int j = 0; j < sub_maps[i]->npcs.size(); j++)
        id_list.push_back(sub_maps[i]->npcs[j]->getID());

    /* Sort the list */
    qSort(id_list);

    /* Find the next available ID */
    for(int i = 0; !found && (i < id_list.size()); i++)
    {
      if(id_list[i] != (id + i))
      {
        id += i;
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
 * Description: Returns the next available person ID that can be used for a new
 *              person.
 *
 * Inputs: bool from_sub - true if it should be unset in sub-maps instead
 * Output: int - the id to use
 */
int EditorMap::getNextPersonID(bool from_sub)
{
  bool found = false;
  int id = 0;

  /* If not from sub map, check base for base ID */
  if(!from_sub)
  {
    for(int i = 0; !found && (i < base_persons.size()); i++)
    {
      if(base_persons[i]->getID() != i)
      {
        id = i;
        found = true;
      }
    }

    /* If nothing found, just make it the last ID + 1 */
    if(!found && base_persons.size() > 0)
      id = base_persons.last()->getID() + 1;
  }
  /* Otherwise, check the sub-maps for available ID */
  else
  {
    /* Compile the IDs of all persons in all sub-maps */
    QVector<int> id_list;
    for(int i = 0; i < sub_maps.size(); i++)
      for(int j = 0; j < sub_maps[i]->persons.size(); j++)
        id_list.push_back(sub_maps[i]->persons[j]->getID());

    /* Sort the list */
    qSort(id_list);

    /* Find the next available ID */
    id = EditorEnumDb::kBASE_ID_PERSON;
    for(int i = 0; !found && (i < id_list.size()); i++)
    {
      if(id_list[i] != (id + i))
      {
        id += i;
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
    id = EditorEnumDb::kBASE_ID_THING;
    for(int i = 0; !found && (i < id_list.size()); i++)
    {
      if(id_list[i] != (id + i))
      {
        id += i;
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
 * Description: Returns the npc with the corresponding ID.
 *
 * Inputs: int id - the id of the npc to get
 *         int sub_map - the sub-map to get the npcs for (<0 is base)
 * Output: EditorMapNPC* - the pointer to match the ID. NULL if not found.
 */
EditorMapNPC* EditorMap::getNPC(int id, int sub_map)
{
  if(id >= 0)
  {
    /* If sub map ref is less than 0, get from base set */
    if(sub_map < 0)
    {
      for(int i = 0; i < base_npcs.size(); i++)
        if(base_npcs[i]->getID() == id)
          return base_npcs[i];
    }
    /* Otherwise, get from sub map */
    else if(sub_map < sub_maps.size())
    {
      for(int i = 0; i < sub_maps[sub_map]->npcs.size(); i++)
        if(sub_maps[sub_map]->npcs[i]->getID() == id)
          return sub_maps[sub_map]->npcs[i];
    }
  }

  return NULL;
}

/*
 * Description: Returns the npc at the corresponding index in the list.
 *
 * Inputs: int index - the index in the array (0 to size - 1)
 *         int sub_map - the sub-map to get the npcs for (<0 is base)
 * Output: EditorMapNPC* - the pointer to match the index. NULL if out of
 *                         range
 */
EditorMapNPC* EditorMap::getNPCByIndex(int index, int sub_map)
{
  /* If sub map ref is less than 0, get from base set */
  if(sub_map < 0)
  {
    if(index >= 0 && index < base_npcs.size())
      return base_npcs[index];
  }
  /* Otherwise, get from sub map */
  else if(sub_map < sub_maps.size())
  {
    if(index >= 0 && index < sub_maps[sub_map]->npcs.size())
      return sub_maps[sub_map]->npcs[index];
  }
  return NULL;
}

/*
 * Description: Returns the number of npcs in the list.
 *
 * Inputs: int sub_map - the sub-map to get the npcs for (<0 is base)
 * Output: int - the number of npcs in the map set
 */
int EditorMap::getNPCCount(int sub_map)
{
  /* If sub map ref is less than 0, get from base set */
  if(sub_map < 0)
    return base_npcs.size();
  /* Otherwise, get from sub map */
  else if(sub_map < sub_maps.size())
    return sub_maps[sub_map]->npcs.size();
  return 0;
}

/*
 * Description: Returns the index in the list of npcs of the matching id.
 *              Less than 0 if none match.
 *
 * Inputs: int id - the id to find the index for
 *         int sub_map - the sub-map to get the npcs for (<0 is base)
 * Output: int - the index of the npc. Less than 0 if no npc matches
 */
int EditorMap::getNPCIndex(int id, int sub_map)
{
  if(id >= 0)
  {
    /* If sub map ref is less than 0, get from base set */
    if(sub_map < 0)
    {
      for(int i = 0; i < base_npcs.size(); i++)
        if(base_npcs[i]->getID() == id)
          return i;
    }
    /* Otherwise, get from sub map */
    else if(sub_map < sub_maps.size())
    {
      for(int i = 0; i < sub_maps[sub_map]->npcs.size(); i++)
        if(sub_maps[sub_map]->npcs[i]->getID() == id)
          return i;
    }
  }
  return -1;
}

/*
 * Description: Returns a list of all npcs in the format of a HEADER row
 *              with the following rows as "ID: NAME".
 *
 * Inputs: int sub_map - the sub-map to get the npcs for (<0 is base)
 *         bool all_submaps - true if all sub-maps should be stacked together
 *         bool shortened - should the sub-map name be shortened (just ID:NAME
 *                          instead of BASEID(ID):NAME).
 * Output: QVector<QString> - list of all npcs
 */
QVector<QString> EditorMap::getNPCList(int sub_map, bool all_submaps,
                                       bool shortened)
{
  QVector<QString> stack;
  stack.push_back("-- NPCS --");

  /* If sub map ref is less than 0, get from base set */
  if(sub_map < 0)
  {
    /* Go through all npcs and add them to the list */
    for(int i = 0; i < base_npcs.size(); i++)
      stack.push_back(base_npcs[i]->getNameList());
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
      for(int j = 0; j < sub_maps[i]->npcs.size(); j++)
        stack.push_back(sub_maps[i]->npcs[j]->getNameList(shortened));
  }

  return stack;
}

/*
 * Description: Returns the list of all npcs in the editor map
 *
 * Inputs: int sub_map - the sub-map to get the npcs for (<0 is base)
 * Output: QVector<EditorMapNPC*> - list of all npcs
 */
QVector<EditorMapNPC*> EditorMap::getNPCs(int sub_map)
{
  /* If sub map ref is less than 0, get from base set */
  if(sub_map < 0)
  {
    return base_npcs;
  }
  /* Otherwise, get from sub map */
  else if(sub_map < sub_maps.size())
  {
    return sub_maps[sub_map]->npcs;
  }

  /* Otherwise, return blank list */
  QVector<EditorMapNPC*> blank_list;
  return blank_list;
}

/*
 * Description: Returns the person with the corresponding ID.
 *
 * Inputs: int id - the id of the person to get
 *         int sub_map - the sub-map to get the persons for (<0 is base)
 * Output: EditorMapPerson* - the pointer to match the ID. NULL if not found.
 */
EditorMapPerson* EditorMap::getPerson(int id, int sub_map)
{
  if(id >= 0)
  {
    /* If sub map ref is less than 0, get from base set */
    if(sub_map < 0)
    {
      for(int i = 0; i < base_persons.size(); i++)
        if(base_persons[i]->getID() == id)
          return base_persons[i];
    }
    /* Otherwise, get from sub map */
    else if(sub_map < sub_maps.size())
    {
      for(int i = 0; i < sub_maps[sub_map]->persons.size(); i++)
        if(sub_maps[sub_map]->persons[i]->getID() == id)
          return sub_maps[sub_map]->persons[i];
    }
  }

  return NULL;
}

/*
 * Description: Returns the person at the corresponding index in the list.
 *
 * Inputs: int index - the index in the array (0 to size - 1)
 *         int sub_map - the sub-map to get the persons for (<0 is base)
 * Output: EditorMapPerson* - the pointer to match the index. NULL if out of
 *                            range
 */
EditorMapPerson* EditorMap::getPersonByIndex(int index, int sub_map)
{
  /* If sub map ref is less than 0, get from base set */
  if(sub_map < 0)
  {
    if(index >= 0 && index < base_persons.size())
      return base_persons[index];
  }
  /* Otherwise, get from sub map */
  else if(sub_map < sub_maps.size())
  {
    if(index >= 0 && index < sub_maps[sub_map]->persons.size())
      return sub_maps[sub_map]->persons[index];
  }
  return NULL;
}

/*
 * Description: Returns the number of persons in the list.
 *
 * Inputs: int sub_map - the sub-map to get the persons for (<0 is base)
 * Output: int - the number of persons in the map set
 */
int EditorMap::getPersonCount(int sub_map)
{
  /* If sub map ref is less than 0, get from base set */
  if(sub_map < 0)
    return base_persons.size();
  /* Otherwise, get from sub map */
  else if(sub_map < sub_maps.size())
    return sub_maps[sub_map]->persons.size();
  return 0;
}

/*
 * Description: Returns the index in the list of persons of the matching id.
 *              Less than 0 if none match.
 *
 * Inputs: int id - the id to find the index for
 *         int sub_map - the sub-map to get the persons for (<0 is base)
 * Output: int - the index of the person. Less than 0 if no person matches
 */
int EditorMap::getPersonIndex(int id, int sub_map)
{
  if(id >= 0)
  {
    /* If sub map ref is less than 0, get from base set */
    if(sub_map < 0)
    {
      for(int i = 0; i < base_persons.size(); i++)
        if(base_persons[i]->getID() == id)
          return i;
    }
    /* Otherwise, get from sub map */
    else if(sub_map < sub_maps.size())
    {
      for(int i = 0; i < sub_maps[sub_map]->persons.size(); i++)
        if(sub_maps[sub_map]->persons[i]->getID() == id)
          return i;
    }
  }
  return -1;
}

/*
 * Description: Returns a list of all persons in the format of a HEADER row
 *              with the following rows as "ID: NAME".
 *
 * Inputs: int sub_map - the sub-map to get the persons for (<0 is base)
 *         bool all_submaps - true if all sub-maps should be stacked together
 *         bool shortened - should the sub-map name be shortened (just ID:NAME
 *                          instead of BASEID(ID):NAME).
 * Output: QVector<QString> - list of all persons
 */
QVector<QString> EditorMap::getPersonList(int sub_map, bool all_submaps,
                                          bool shortened)
{
  QVector<QString> stack;
  stack.push_back("-- PERSONS --");

  /* If sub map ref is less than 0, get from base set */
  if(sub_map < 0)
  {
    /* Go through all persons and add them to the list */
    for(int i = 0; i < base_persons.size(); i++)
      stack.push_back(base_persons[i]->getNameList());
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
      for(int j = 0; j < sub_maps[i]->persons.size(); j++)
        stack.push_back(sub_maps[i]->persons[j]->getNameList(shortened));
  }

  return stack;
}

/*
 * Description: Returns the sub map index of the person instance reference.
 *
 * Inputs: EditorMapPerson* ref - the reference of the person pointer
 * Output: SubMapInfo* - the reference sub map
 */
SubMapInfo* EditorMap::getPersonSub(EditorMapPerson* ref)
{
  if(ref != nullptr && ref->getBasePerson() != nullptr)
  {
    for(int i = 0; i < sub_maps.size(); i++)
      for(int j = 0; j < sub_maps[i]->persons.size(); j++)
        if(sub_maps[i]->persons[j] == ref)
          return sub_maps[i];
  }
  return nullptr;
}

/*
 * Description: Returns the list of all persons in the editor map
 *
 * Inputs: int sub_map - the sub-map to get the persons for (<0 is base)
 * Output: QVector<EditorMapPerson*> - list of all persons
 */
QVector<EditorMapPerson*> EditorMap::getPersons(int sub_map)
{
  /* If sub map ref is less than 0, get from base set */
  if(sub_map < 0)
  {
    return base_persons;
  }
  /* Otherwise, get from sub map */
  else if(sub_map < sub_maps.size())
  {
    return sub_maps[sub_map]->persons;
  }

  /* Otherwise, return blank list */
  QVector<EditorMapPerson*> blank_list;
  return blank_list;
}

/*
 * Description: Returns the number of items that will be saved in the given
 *              map. Used for determing how far along the save process is.
 *
 * Inputs: int sub_index - sub index for loading - if relevant
 * Output: int - the total number of items to be saved in the map
 */
int EditorMap::getSaveCount(int sub_index)
{
  int total = 0;

  /* Get count from base items */
  total += sprites.size() + base_things.size() + base_persons.size()
        + base_npcs.size() + base_items.size() + base_ios.size();

  /* Get count from sub-maps */
  int start_index = 0;
  int limit = sub_maps.size();
  if(sub_index > 0 && sub_index < sub_maps.size())
  {
    start_index = sub_index;
    limit = start_index + 1;
  }

  /* Go through the subs */
  for(int i = start_index; i < limit; i++)
  {
    /* Add thing(s) instances */
    total += sub_maps[i]->things.size() + sub_maps[i]->persons.size() +
             sub_maps[i]->npcs.size() + sub_maps[i]->items.size() +
             sub_maps[i]->ios.size();

    /* Add total number of sprites * 12 layers
     * (base, enhancer, lower x 5, upper x 5) */
    if(sub_maps[i]->tiles.size() > 0)
      total += 12 + 6; // 12 different tile layers + 6 with passability
  }

  return total;
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
 * Output: EditorMapThing* - the pointer to match the ID. NULL if not found.
 */
EditorMapThing* EditorMap::getThing(int id, int sub_map)
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
 * Output: EditorMapThing* - the pointer to match the index. NULL if out of
 *                           range
 */
EditorMapThing* EditorMap::getThingByIndex(int index, int sub_map)
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
 *         bool shortened - should the sub-map name be shortened (just ID:NAME
 *                          instead of BASEID(ID):NAME).
 * Output: QVector<QString> - list of all things
 */
QVector<QString> EditorMap::getThingList(int sub_map, bool all_submaps,
                                         bool shortened)
{
  QVector<QString> stack;
  stack.push_back("-- THINGS --");

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
        stack.push_back(sub_maps[i]->things[j]->getNameList(shortened));
  }

  return stack;
}

/*
 * Description: Returns the list of all things in the editor map
 *
 * Inputs: int sub_map - the sub-map to get the things for (<0 is base)
 * Output: QVector<EditorMapThing*> - list of all things
 */
QVector<EditorMapThing*> EditorMap::getThings(int sub_map)
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
  QVector<EditorMapThing*> blank_list;
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
  else if(data.getElement(index) == "mapthing" && data.getKey(index) == "id")
  {
    int thing_id = QString::fromStdString(data.getKeyValue(index)).toInt();
    EditorMapThing* thing = getThing(thing_id);

    /* Create new thing if it doesn't exist */
    if(thing == NULL)
    {
      thing = new EditorMapThing(thing_id);
      thing->setTileIcons(getTileIcons());
      setThing(thing);
    }

    /* Continue to parse the data in the thing */
    thing->load(data, index + 1);
  }
  else if(data.getElement(index) == "mapio" && data.getKey(index) == "id")
  {
    int io_id = QString::fromStdString(data.getKeyValue(index)).toInt();
    EditorMapIO* io = getIO(io_id);

    /* Create new IO if it doesn't exist */
    if(io == NULL)
    {
      io = new EditorMapIO(io_id);
      io->setTileIcons(getTileIcons());
      setIO(io);
    }

    /* Continue to parse the data in the IO */
    io->load(data, index + 1);
  }
  else if(data.getElement(index) == "mapitem" && data.getKey(index) == "id")
  {
    int item_id = QString::fromStdString(data.getKeyValue(index)).toInt();
    EditorMapItem* item = getItem(item_id);

    /* Create new item if it doesn't exist */
    if(item == NULL)
    {
      item = new EditorMapItem(item_id);
      item->setTileIcons(getTileIcons());
      setItem(item);
    }

    /* Continue to parse the data in the item */
    item->load(data, index + 1);
  }
  else if(data.getElement(index) == "mapperson" && data.getKey(index) == "id")
  {
    int person_id = QString::fromStdString(data.getKeyValue(index)).toInt();
    EditorMapPerson* person = getPerson(person_id);

    /* Create new person if it doesn't exist */
    if(person == NULL)
    {
      person = new EditorMapPerson(person_id);
      person->setTileIcons(getTileIcons());
      setPerson(person);
    }

    /* Continue to parse the data in the person */
    person->load(data, index + 1);
  }
  else if(data.getElement(index) == "mapnpc" && data.getKey(index) == "id")
  {
    int npc_id = QString::fromStdString(data.getKeyValue(index)).toInt();
    EditorMapNPC* npc = getNPC(npc_id);

    /* Create new npc if it doesn't exist */
    if(npc == NULL)
    {
      npc = new EditorMapNPC(npc_id);
      npc->setTileIcons(getTileIcons());
      setNPC(npc);
    }

    /* Continue to parse the data in the npc */
    npc->load(data, index + 1);
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
 * Description: Resizes the passed in sub map index to the designated width and
 *              height. Fails if the index of the sub-map does not exist.
 *
 * Inputs: int index - the index of the sub-map in the map set
 *         int width - the new width of the map
 *         int height - the new height of the map
 * Output: bool - true if the resize was successful
 */
bool EditorMap::resizeMap(int index, int width, int height)
{
  SubMapInfo* map = getMapByIndex(index);
  if(map != NULL && width > 0 && height > 0)
    return resizeMap(map, width, height);
  return false;
}

/*
 * Description: Saves the map data to the file handling pointer.
 *
 * Inputs: FileHandler* fh - the file handling pointer
 *         QProgressDialog* dialog - the dialog that holds the amount saved
 *         bool game_only - true if the data should include game only relevant
 *         int sub_index - the sub map index to save
 * Output: none
 */
void EditorMap::save(FileHandler* fh, QProgressDialog* save_dialog,
                     bool game_only, int sub_index)
{
  if(fh != NULL)
  {
    fh->writeXmlElement("map", "id", getID());
    if(!game_only)
      fh->writeXmlData("name", getName().toStdString());

    /* Add sprites */
    for(int i = 0; i < sprites.size(); i++)
    {
      sprites[i]->save(fh, game_only);
      save_dialog->setValue(save_dialog->value()+1);
    }

    /* Add things */
    for(int i = 0; i < base_things.size(); i++)
    {
      base_things[i]->save(fh, game_only);
      save_dialog->setValue(save_dialog->value()+1);
    }

    /* Add ios */
    for(int i = 0; i < base_ios.size(); i++)
    {
      base_ios[i]->save(fh, game_only);
      save_dialog->setValue(save_dialog->value()+1);
    }

    /* Add items */
    for(int i = 0; i < base_items.size(); i++)
    {
      base_items[i]->save(fh, game_only);
      save_dialog->setValue(save_dialog->value()+1);
    }

    /* Add persons */
    for(int i = 0; i < base_persons.size(); i++)
    {
      base_persons[i]->save(fh, game_only);
      save_dialog->setValue(save_dialog->value()+1);
    }

    /* Add npcs */
    for(int i = 0; i < base_npcs.size(); i++)
    {
      base_npcs[i]->save(fh, game_only);
      save_dialog->setValue(save_dialog->value()+1);
    }

    /* Save all maps if sub_index is out of range */
    if(sub_index <= 0 || sub_index >= sub_maps.size())
    {
      for(int i = 0; i < sub_maps.size(); i++)
        saveSubMap(fh, save_dialog, game_only, sub_maps[i], i == 0);
    }
    /* Otherwise, just save the only map */
    else
    {
      saveSubMap(fh, save_dialog, game_only, sub_maps[sub_index], true);
    }

    fh->writeXmlElementEnd();
  }
}

/*
 * Description: Sets the current npc, based on the index in the stack.
 *              Passing in -1 unsets the active npc (to NULL). This sprite is
 *              used in click placement.
 *
 * Inputs: int index - npc index in the list of map npcs
 * Output: bool - true if the active npc was changed
 */
bool EditorMap::setCurrentIO(int index)
{
  if(index >= -1 && index < base_ios.size())
  {
    /* Unset the hover, if relevant */
    updateHoverThing(true);

    /* If index is -1, unset the current IO */
    if(index == -1)
      active_info.active_io = NULL;
    /* Otherwise, index is in valid range of base IO set */
    else
      active_info.active_io = base_ios[index];

    /* Set the hover, if relevant */
    updateHoverThing();

    return true;
  }
  return false;
}

/*
 * Description: Sets the current item, based on the index in the stack.
 *              Passing in -1 unsets the active item (to NULL). This sprite is
 *              used in click placement.
 *
 * Inputs: int index - item index in the list of map items
 * Output: bool - true if the active item was changed
 */
bool EditorMap::setCurrentItem(int index)
{
  if(index >= -1 && index < base_items.size())
  {
    /* Unset the hover, if relevant */
    updateHoverThing(true);

    /* If index is -1, unset the current item */
    if(index == -1)
      active_info.active_item = NULL;
    /* Otherwise, index is in valid range of base item set */
    else
      active_info.active_item = base_items[index];

    /* Set the hover, if relevant */
    updateHoverThing();

    return true;
  }
  return false;
}

/*
 * Description: Sets the current sub-map, based on the index in the stack.
 *              Passing in -1 unsets the active submap (to NULL).
 *
 * Inputs: int index - sub-map index in the list of sub-maps
 * Output: bool - true if the active submap was changed
 */
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

    /* Trigger all instance updates - connected in widgets */
    emit activeSubChanged();
    emit ioInstanceChanged("");
    emit itemInstanceChanged("");
    emit npcInstanceChanged("");
    emit personInstanceChanged("");
    emit thingInstanceChanged("");

    return true;
  }
  return false;
}

/*
 * Description: Sets the current npc, based on the index in the stack.
 *              Passing in -1 unsets the active npc (to NULL). This sprite is
 *              used in click placement.
 *
 * Inputs: int index - npc index in the list of map npcs
 * Output: bool - true if the active npc was changed
 */
bool EditorMap::setCurrentNPC(int index)
{
  if(index >= -1 && index < base_npcs.size())
  {
    /* Unset the hover, if relevant */
    updateHoverThing(true);

    /* If index is -1, unset the current npc */
    if(index == -1)
      active_info.active_npc = NULL;
    /* Otherwise, index is in valid range of base npc set */
    else
      active_info.active_npc = base_npcs[index];

    /* Set the hover, if relevant */
    updateHoverThing();

    return true;
  }
  return false;
}

/*
 * Description: Sets the current person, based on the index in the stack.
 *              Passing in -1 unsets the active person (to NULL). This sprite is
 *              used in click placement.
 *
 * Inputs: int index - thing index in the list of map persons
 * Output: bool - true if the active person was changed
 */
bool EditorMap::setCurrentPerson(int index)
{
  if(index >= -1 && index < base_persons.size())
  {
    /* Unset the hover, if relevant */
    updateHoverThing(true);

    /* If index is -1, unset the current person */
    if(index == -1)
      active_info.active_person = NULL;
    /* Otherwise, index is in valid range of base person set */
    else
      active_info.active_person = base_persons[index];

    /* Set the hover, if relevant */
    updateHoverThing();

    return true;
  }
  return false;
}

/*
 * Description: Sets the current sprite, based on the index in the stack.
 *              Passing in -1 unsets the active sprite (to NULL). This sprite is
 *              used in click placement.
 *
 * Inputs: int index - sprite index in the list of map sprites
 * Output: bool - true if the active sprite was changed
 */
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

/*
 * Description: Sets the current thing, based on the index in the stack.
 *              Passing in -1 unsets the active thing (to NULL). This sprite is
 *              used in click placement.
 *
 * Inputs: int index - thing index in the list of map things
 * Output: bool - true if the active thing was changed
 */
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

/*
 * Description: Sets the current cursor mode, for the hover tile.
 *
 * Inputs: EditorEnumDb::CursorMode cursor - the active cursor enum
 * Output: none
 */
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

/*
 * Description: Sets the hover IO, being flagged when selecting an instance
 *              in the list in MapIOView.
 *
 * Inputs: int id - the id of the IO instance
 * Output: bool - true if the instance was changed (unsets the old regardless)
 */
bool EditorMap::setHoverIO(int id)
{
  if(active_submap != NULL)
    return setHoverThing(getIO(id, active_submap->id));
  return false;
}

/*
 * Description: Sets the hover item, being flagged when selecting an instance
 *              in the list in MapItemView.
 *
 * Inputs: int id - the id of the item instance
 * Output: bool - true if the instance was changed (unsets the old regardless)
 */
bool EditorMap::setHoverItem(int id)
{
  if(active_submap != NULL)
    return setHoverThing(getItem(id, active_submap->id));
  return false;
}

/*
 * Description: Sets the current layer, for the hover tile, as controlled by
 *              MapControl.
 *
 * Inputs: EditorEnumDb::Layer layer - the active layer enum
 * Output: none
 */
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

/*
 * Description: Sets the hover npc, being flagged when selecting an instance
 *              in the list in MapNPCView.
 *
 * Inputs: int id - the id of the npc instance
 * Output: bool - true if the instance was changed (unsets the old regardless)
 */
bool EditorMap::setHoverNPC(int id)
{
  if(active_submap != NULL)
    return setHoverThing(getNPC(id, active_submap->id));
  return false;
}

/*
 * Description: Sets if the map is in a hover path edit mode. Used to notify
 *              the rendering engine on how tiles are displayed on hover.
 *
 * Inputs: bool path_mode - true if in path edit mode
 * Output: none
 */
void EditorMap::setHoverPathMode(bool path_mode)
{
  /* First, unset hover if relevant */
  updateHoverThing(true);

  active_info.path_edit_mode = path_mode;

  if(active_info.hover_tile != NULL)
    active_info.hover_tile->update();
}

/*
 * Description: Sets the hover person, being flagged when selecting an instance
 *              in the list in MapPersonView.
 *
 * Inputs: int id - the id of the person instance
 * Output: bool - true if the instance was changed (unsets the old regardless)
 */
bool EditorMap::setHoverPerson(int id)
{
  if(active_submap != NULL)
    return setHoverThing(getPerson(id, active_submap->id));
  return false;
}

/*
 * Description: Sets the hover thing, being flagged when selecting an instance
 *              in the list in MapThingView.
 *
 * Inputs: int id - the id of the thing instance
 * Output: bool - true if the instance was changed (unsets the old regardless)
 */
bool EditorMap::setHoverThing(int id)
{
  if(active_submap != NULL)
    return setHoverThing(getThing(id, active_submap->id));
  return false;
}

/*
 * Description: Sets the hovering tile. If the pen is place and on thing layer,
 *              it hovers to the size of the placing thing. Otherwise, just
 *              a single tile. Always unsets the previous hovered tiles prior
 *              to proceeding.
 *
 * Inputs: EditorTile* tile - the new hover tile
 * Output: none
 */
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
 * Description: Sets the IO in the set within the editor map. If an IO,
 *              already exists with the ID, it deletes the existing one.
 *
 * Inputs: EditorMapIO* io - the new IO to set in
 *         int sub_map - the sub-map to set the IOs for (<0 is base)
 * Output: int - the index if set. If < 0, it is not set.
 */
int EditorMap::setIO(EditorMapIO* io, int sub_map)
{
  if(io != NULL && io->getID() >= 0)
  {
    bool found = false;
    int index = -1;
    bool near = false;

    /* If sub map id is less than 0, work with base IO */
    if(sub_map < 0)
    {
      /* Find if the ID exists */
      for(int i = 0; !found && !near && (i < base_ios.size()); i++)
      {
        if(base_ios[i]->getID() == io->getID())
        {
          index = i;
          found = true;
        }
        else if(base_ios[i]->getID() > io->getID())
        {
          index = i;
          near = true;
        }
      }

      /* If found, modify the index with the new information */
      if(found)
      {
        unsetIOByIndex(index);
        base_ios.insert(index, io);
      }
      else if(near)
      {
        base_ios.insert(index, io);
      }
      else
      {
        base_ios.append(io);
        index = base_ios.size() - 1;
      }
    }
    /* Otherwise, work with the sub id */
    else if(sub_map < sub_maps.size())
    {
      /* Check to make sure the thing could be added */
      int x_start = io->getX();
      int y_start = io->getY();
      int x_end = x_start + io->getMatrix()->getWidth();
      int y_end = y_start + io->getMatrix()->getHeight();
      if(x_start >= 0 && x_end <= sub_maps[sub_map]->tiles.size() &&
         y_start >= 0 && y_end <= sub_maps[sub_map]->tiles[x_start].size())
      {
        /* First remove the existing id, if one exists */
        unsetIO(io->getID(), true);

        /* Now insert into the proper location in the IO stack in the map */
        for(int i = 0; !near && (i < sub_maps[sub_map]->ios.size()); i++)
        {
          if(sub_maps[sub_map]->ios[i]->getID() > io->getID())
          {
            index = i;
            near = true;
          }
        }

        /* If near, insert at index. Otherwise, append */
        if(near)
        {
          sub_maps[sub_map]->ios.insert(index, io);
        }
        else
        {
          sub_maps[sub_map]->ios.append(io);
          index = sub_maps[sub_map]->ios.size() - 1;
        }

        /* Add to tile */
        for(int i = x_start; i < x_end; i++)
          for(int j = y_start; j < y_end; j++)
            sub_maps[sub_map]->tiles[i][j]->setIO(io);
      }
    }

    return index;
  }
  return -1;
}

/*
 * Description: Sets the item in the set within the editor map. If a item,
 *              already exists with the ID, it deletes the existing one.
 *
 * Inputs: EditorMapItem* item - the new item to set in
 *         int sub_map - the sub-map to set the items for (<0 is base)
 * Output: int - the index if set. If < 0, it is not set.
 */
int EditorMap::setItem(EditorMapItem* item, int sub_map)
{
  if(item != NULL && item->getID() >= 0)
  {
    bool found = false;
    int index = -1;
    bool near = false;

    /* If sub map id is less than 0, work with base item */
    if(sub_map < 0)
    {
      /* Find if the ID exists */
      for(int i = 0; !found && !near && (i < base_items.size()); i++)
      {
        if(base_items[i]->getID() == item->getID())
        {
          index = i;
          found = true;
        }
        else if(base_items[i]->getID() > item->getID())
        {
          index = i;
          near = true;
        }
      }

      /* If found, modify the index with the new information */
      if(found)
      {
        unsetItemByIndex(index);
        base_items.insert(index, item);
      }
      else if(near)
      {
        base_items.insert(index, item);
      }
      else
      {
        base_items.append(item);
        index = base_items.size() - 1;
      }
    }
    /* Otherwise, work with the sub id */
    else if(sub_map < sub_maps.size())
    {
      /* Check to make sure the thing could be added */
      int x_start = item->getX();
      int y_start = item->getY();
      int x_end = x_start + item->getMatrix()->getWidth();
      int y_end = y_start + item->getMatrix()->getHeight();
      if(x_start >= 0 && x_end <= sub_maps[sub_map]->tiles.size() &&
         y_start >= 0 && y_end <= sub_maps[sub_map]->tiles[x_start].size())
      {
        /* First remove the existing id, if one exists */
        unsetItem(item->getID(), true);

        /* Now insert into the proper location in the item stack in the map */
        for(int i = 0; !near && (i < sub_maps[sub_map]->items.size()); i++)
        {
          if(sub_maps[sub_map]->items[i]->getID() > item->getID())
          {
            index = i;
            near = true;
          }
        }

        /* If near, insert at index. Otherwise, append */
        if(near)
        {
          sub_maps[sub_map]->items.insert(index, item);
        }
        else
        {
          sub_maps[sub_map]->items.append(item);
          index = sub_maps[sub_map]->items.size() - 1;
        }

        /* Add to tile */
        for(int i = x_start; i < x_end; i++)
        {
          for(int j = y_start; j < y_end; j++)
          {
            bool add = sub_maps[sub_map]->tiles[i][j]->addItem(item);
            if(!add)
              index = -1;
          }
        }
      }
    }

    return index;
  }
  return -1;
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
      info->path_top = NULL;
      info->weather = -1;

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
    bool events = ref_tile->getVisibilityEvents();
    bool grid = ref_tile->getVisibilityGrid();
    bool pass = ref_tile->getVisibilityPass();
    setVisibilityEvents(events);
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
 *         bool update - should the widget be updated? default true
 * Output: none
 */
void EditorMap::setName(QString name, bool update)
{
  (void)update;

  this->name = name;
}

/*
 * Description: Sets the npc in the set within the editor map. If a npc,
 *              already exists with the ID, it deletes the existing one.
 *
 * Inputs: EditorMapNPC* npc - the new npc to set in
 *         int sub_map - the sub-map to set the npcs for (<0 is base)
 * Output: int - the index if set. If < 0, it is not set.
 */
int EditorMap::setNPC(EditorMapNPC* npc, int sub_map)
{
  if(npc != NULL && npc->getID() >= 0)
  {
    bool found = false;
    int index = -1;
    bool near = false;

    /* If sub map id is less than 0, work with base npc */
    if(sub_map < 0)
    {
      /* Find if the ID exists */
      for(int i = 0; !found && !near && (i < base_npcs.size()); i++)
      {
        if(base_npcs[i]->getID() == npc->getID())
        {
          index = i;
          found = true;
        }
        else if(base_npcs[i]->getID() > npc->getID())
        {
          index = i;
          near = true;
        }
      }

      /* If found, modify the index with the new information */
      if(found)
      {
        unsetNPCByIndex(index);
        base_npcs.insert(index, npc);
      }
      else if(near)
      {
        base_npcs.insert(index, npc);
      }
      else
      {
        base_npcs.append(npc);
        index = base_npcs.size() - 1;
      }
    }
    /* Otherwise, work with the sub id */
    else if(sub_map < sub_maps.size())
    {
      /* Check to make sure the npc could be added */
      int x_start = npc->getX();
      int y_start = npc->getY();
      int x_end = x_start + npc->getMatrix()->getWidth();
      int y_end = y_start + npc->getMatrix()->getHeight();
      if(x_start >= 0 && x_end <= sub_maps[sub_map]->tiles.size() &&
         y_start >= 0 && y_end <= sub_maps[sub_map]->tiles[x_start].size())
      {
        /* First remove the existing id, if one exists */
        unsetNPC(npc->getID(), true);

        /* Now insert into the proper location in the npc stack in the map */
        for(int i = 0; !near && (i < sub_maps[sub_map]->npcs.size()); i++)
        {
          if(sub_maps[sub_map]->npcs[i]->getID() > npc->getID())
          {
            index = i;
            near = true;
          }
        }

        /* If near, insert at index. Otherwise, append */
        if(near)
        {
          sub_maps[sub_map]->npcs.insert(index, npc);
        }
        else
        {
          sub_maps[sub_map]->npcs.append(npc);
          index = sub_maps[sub_map]->npcs.size() - 1;
        }

        /* Update path colors and connect signals */
        recolorNPCPaths(sub_maps[sub_map]);
        connect(npc->getPath(), SIGNAL(hoverInit(EditorNPCPath*)),
                this, SLOT(npcHoverPathChanged(EditorNPCPath*)));
        if(sub_maps[sub_map] == active_submap)
          emit npcPathAdd(npc->getPath());

        /* Add to tiles */
        for(int i = x_start; i < x_end; i++)
          for(int j = y_start; j < y_end; j++)
            sub_maps[sub_map]->tiles[i][j]->setNPC(npc);
      }
    }

    return index;
  }
  return -1;
}

/*
 * Description: Sets if the paths are enabled (and displayed on the screen).
 *              This is used as an over-ride for visibility, when the paths are
 *              to be disabled regardless of visibility status.
 *
 * Inputs: bool enabled - paths enabled??
 * Output: none
 */
void EditorMap::setPathsEnabled(bool enabled)
{
  for(int i = 0; i < sub_maps.size(); i++)
    for(int j = 0; j < sub_maps[i]->npcs.size(); j++)
      sub_maps[i]->npcs[j]->getPath()->setVisibleEdit(enabled);
}

/*
 * Description: Sets the person in the set within the editor map. If a person,
 *              already exists with the ID, it deletes the existing one.
 *
 * Inputs: EditorMapPerson* person - the new person to set in
 *         int sub_map - the sub-map to set the persons for (<0 is base)
 * Output: int - the index if set. If < 0, it is not set.
 */
int EditorMap::setPerson(EditorMapPerson* person, int sub_map)
{
  if(person != NULL && person->getID() >= 0)
  {
    bool found = false;
    int index = -1;
    bool near = false;

    /* If sub map id is less than 0, work with base person */
    if(sub_map < 0)
    {
      /* Find if the ID exists */
      for(int i = 0; !found && !near && (i < base_persons.size()); i++)
      {
        if(base_persons[i]->getID() == person->getID())
        {
          index = i;
          found = true;
        }
        else if(base_persons[i]->getID() > person->getID())
        {
          index = i;
          near = true;
        }
      }

      /* If found, modify the index with the new information */
      if(found)
      {
        unsetPersonByIndex(index);
        base_persons.insert(index, person);
      }
      else if(near)
      {
        base_persons.insert(index, person);
      }
      else
      {
        base_persons.append(person);
        index = base_persons.size() - 1;
      }
    }
    /* Otherwise, work with the sub id */
    else if(sub_map < sub_maps.size())
    {
      /* Check to make sure the person could be added */
      int x_start = person->getX();
      int y_start = person->getY();
      int x_end = x_start + person->getMatrix()->getWidth();
      int y_end = y_start + person->getMatrix()->getHeight();
      if(x_start >= 0 && x_end <= sub_maps[sub_map]->tiles.size() &&
         y_start >= 0 && y_end <= sub_maps[sub_map]->tiles[x_start].size())
      {
        /* First remove the existing id, if one exists */
        unsetPerson(person->getID(), true);

        /* Now insert into the proper location in the person stack in the map */
        for(int i = 0; !near && (i < sub_maps[sub_map]->persons.size()); i++)
        {
          if(sub_maps[sub_map]->persons[i]->getID() > person->getID())
          {
            index = i;
            near = true;
          }
        }

        /* If near, insert at index. Otherwise, append */
        if(near)
        {
          sub_maps[sub_map]->persons.insert(index, person);
        }
        else
        {
          sub_maps[sub_map]->persons.append(person);
          index = sub_maps[sub_map]->persons.size() - 1;
        }

        /* Add to tile */
        for(int i = x_start; i < x_end; i++)
          for(int j = y_start; j < y_end; j++)
            sub_maps[sub_map]->tiles[i][j]->setPerson(person);
      }
    }

    return index;
  }
  return -1;
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
int EditorMap::setThing(EditorMapThing* thing, int sub_map)
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

  /* If layer is npc, pass visibility to paths as well */
  // TODO: TEMP - this should eventually be handled by own enum and default to
  //              NOT visible
  if(layer == EditorEnumDb::NPC)
      setVisibilityPaths(visible);
}

/*
 * Description: Sets the event notifiers visibility on all sub-map tiles.
 *              Controlled by the map view.
 *
 * Inputs: bool visible - is the event notifiers visible?
 * Output: none
 */
void EditorMap::setVisibilityEvents(bool visible)
{
  for(int i = 0; i < sub_maps.size(); i++)
    for(int j = 0; j < sub_maps[i]->tiles.size(); j++)
      for(int k = 0; k < sub_maps[i]->tiles[j].size(); k++)
        sub_maps[i]->tiles[j][k]->setVisibilityEvents(visible);
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

/*
 * Description: Sets the visibility of all paths for npcs on all sub-map
 *              tiles. Controlled by the map view.
 *
 * Inputs: bool visible - is the npc paths visible?
 * Output: none
 */
void EditorMap::setVisibilityPaths(bool visible)
{
  if(visible_path != visible)
  {
    visible_path = visible;

    for(int i = 0; i < sub_maps.size(); i++)
      for(int j = 0; j < sub_maps[i]->npcs.size(); j++)
        sub_maps[i]->npcs[j]->getPath()->setVisibleControl(visible);
  }
}

/*
 * Description: Re-adds all sub-map IOs to tiles. This is called after
 *              calling tilesIORemove(), processing the IO, and then
 *              re-calling. Any IO instances that can't be re-added are
 *              deleted and removed from the list.
 * TODO: FUTURE - warn about IOs being removed??
 *
 * Inputs: bool update_all - true if updating all sub-maps or just active one
 * Output: none
 */
void EditorMap::tilesIOAdd(bool update_all)
{
  /* Update all sub-maps - used for loading */
  if(update_all)
  {
    for(int i = 0; i < sub_maps.size(); i++)
    {
      for(int j = 0; j < sub_maps[i]->ios.size(); j++)
      {
        if(!addIO(sub_maps[i]->ios[j], sub_maps[i]))
        {
          delete sub_maps[i]->ios[j];
          sub_maps[i]->ios.remove(j);
          j--;
        }
      }
    }
  }
  /* Or just the active one */
  else
  {
    if(active_submap != NULL)
    {
      for(int i = 0; i < active_submap->ios.size(); i++)
      {
        if(!addIO(active_submap->ios[i]))
        {
          delete active_submap->ios[i];
          active_submap->ios.remove(i);
          i--;
        }
      }
    }
  }
}

/*
 * Description: Removes all sub-map IOs from tiles. This is called before
 *              processing IO changes, and then calling tilesIOAdd().
 *              Just removes the IOs from the tiles but still stored into
 *              memory.
 *
 * Inputs: bool update_all - true if updating all sub-maps or just active one
 * Output: none
 */
void EditorMap::tilesIORemove(bool update_all)
{
  /* Update all sub-maps - used for loading / resizing */
  if(update_all)
  {
    for(int i = 0; i < sub_maps.size(); i++)
    {
      for(int j = 0; j < sub_maps[i]->ios.size(); j++)
      {
        EditorMapIO* io = sub_maps[i]->ios[j];
        int x = io->getX();
        int y = io->getY();
        int w = io->getMatrix()->getWidth();
        int h = io->getMatrix()->getHeight();

        for(int m = 0; m < w; m++)
          for(int n = 0; n < h; n++)
            sub_maps[i]->tiles[x+m][y+n]->unsetIO(
                                         io->getMatrix()->getRenderDepth(m, n));
      }
    }
  }
  else
  {
    if(active_submap != NULL)
    {
      /* Clear selection */
      setHoverThing(-1);

      for(int i = 0; i < active_submap->ios.size(); i++)
      {
        EditorMapIO* io = active_submap->ios[i];
        int x = io->getX();
        int y = io->getY();
        int w = io->getMatrix()->getWidth();
        int h = io->getMatrix()->getHeight();

        for(int j = 0; j < w; j++)
          for(int k = 0; k < h; k++)
            active_submap->tiles[x+j][y+k]->unsetIO(
                                         io->getMatrix()->getRenderDepth(j, k));
      }
    }
  }
}

/*
 * Description: Re-adds all sub-map items to tiles. This is called after
 *              calling tilesItemRemove(), processing the item, and then
 *              re-calling. Any item instances that can't be re-added are
 *              deleted and removed from the list.
 * TODO: FUTURE - warn about items being removed??
 *
 * Inputs: bool update_all - true if updating all sub-maps or just active one
 * Output: none
 */
void EditorMap::tilesItemAdd(bool update_all)
{
  /* Update all sub-maps - used for loading */
  if(update_all)
  {
    for(int i = 0; i < sub_maps.size(); i++)
    {
      for(int j = 0; j < sub_maps[i]->items.size(); j++)
      {
        if(!addItem(sub_maps[i]->items[j], sub_maps[i]))
        {
          delete sub_maps[i]->items[j];
          sub_maps[i]->items.remove(j);
          j--;
        }
      }
    }
  }
  /* Or just the active one */
  else
  {
    if(active_submap != NULL)
    {
      for(int i = 0; i < active_submap->items.size(); i++)
      {
        if(!addItem(active_submap->items[i]))
        {
          delete active_submap->items[i];
          active_submap->items.remove(i);
          i--;
        }
      }
    }
  }
}

/*
 * Description: Removes all sub-map items from tiles. This is called before
 *              processing item changes, and then calling tilesItemAdd().
 *              Just removes the items from the tiles but still stored into
 *              memory.
 *
 * Inputs: bool update_all - true if updating all sub-maps or just active one
 * Output: none
 */
void EditorMap::tilesItemRemove(bool update_all)
{
  /* Update all sub-maps - used for loading / resizing */
  if(update_all)
  {
    for(int i = 0; i < sub_maps.size(); i++)
    {
      for(int j = 0; j < sub_maps[i]->items.size(); j++)
      {
        EditorMapItem* item = sub_maps[i]->items[j];
        int x = item->getX();
        int y = item->getY();

        sub_maps[i]->tiles[x][y]->unsetItem(item);
      }
    }
  }
  else
  {
    if(active_submap != NULL)
    {
      /* Clear selection */
      setHoverThing(-1);

      for(int i = 0; i < active_submap->items.size(); i++)
      {
        EditorMapItem* item = active_submap->items[i];
        int x = item->getX();
        int y = item->getY();

        active_submap->tiles[x][y]->unsetItem(item);
      }
    }
  }
}

/*
 * Description: Re-adds all sub-map npcs to tiles. This is called after
 *              calling tilesNPCRemove(), processing the npc, and then
 *              re-calling. Any npc instances that can't be re-added are
 *              deleted and removed from the list.
 * TODO: FUTURE - warn about npcs being removed??
 *
 * Inputs: bool update_all - true if updating all sub-maps or just active one
 * Output: none
 */
void EditorMap::tilesNPCAdd(bool update_all)
{
  /* Update all sub-maps - used for loading */
  if(update_all)
  {
    for(int i = 0; i < sub_maps.size(); i++)
    {
      /* Clear top path */
      if(sub_maps[i]->path_top != NULL)
        sub_maps[i]->path_top->setZValue(1);
      sub_maps[i]->path_top = NULL;

      for(int j = 0; j < sub_maps[i]->npcs.size(); j++)
      {
        if(!addNPC(sub_maps[i]->npcs[j], sub_maps[i]))
        {
          /* Signal clean-up */
          EditorNPCPath* path = sub_maps[i]->npcs[j]->getPath();
          disconnect(path, SIGNAL(hoverInit(EditorNPCPath*)),
                     this, SLOT(npcHoverPathChanged(EditorNPCPath*)));
          emit npcPathRemove(path);

          /* Delete npc */
          delete sub_maps[i]->npcs[j];
          sub_maps[i]->npcs.remove(j);
          j--;
        }
        else
        {
          /* Good NPC - check paths */
          sub_maps[i]->npcs[j]->getPath()->checkNodes(0, 0,
                                           sub_maps[i]->tiles.size(),
                                           sub_maps[i]->tiles.front().size());
        }
      }
      recolorNPCPaths(sub_maps[i]);
    }
  }
  /* Or just the active one */
  else
  {
    if(active_submap != NULL)
    {
      for(int i = 0; i < active_submap->npcs.size(); i++)
      {
        if(!addNPC(active_submap->npcs[i]))
        {
          /* Signal clean-up */
          EditorNPCPath* path = active_submap->npcs[i]->getPath();
          disconnect(path, SIGNAL(hoverInit(EditorNPCPath*)),
                     this, SLOT(npcHoverPathChanged(EditorNPCPath*)));
          emit npcPathRemove(path);

          /* Delete npc */
          delete active_submap->npcs[i];
          active_submap->npcs.remove(i);
          i--;
        }
        else
        {
          /* Good NPC - check paths */
          active_submap->npcs[i]->getPath()->checkNodes(0, 0,
                                           active_submap->tiles.size(),
                                           active_submap->tiles.front().size());
        }
      }
    }
  }
}

/*
 * Description: Removes all sub-map npcs from tiles. This is called before
 *              processing npc changes, and then calling tilesNPCAdd().
 *              Just removes the npcs from the tiles but still stored into
 *              memory.
 *
 * Inputs: bool update_all - true if updating all sub-maps or just active one
 * Output: none
 */
void EditorMap::tilesNPCRemove(bool update_all)
{
  /* Clear selection */
  setHoverThing(-1);

  /* Update all sub-maps - used for loading */
  if(update_all)
  {
    for(int i = 0; i < sub_maps.size(); i++)
    {
      for(int j = 0; j < sub_maps[i]->npcs.size(); j++)
      {
        EditorMapNPC* npc = sub_maps[i]->npcs[j];
        int x = npc->getX();
        int y = npc->getY();
        int w = npc->getMatrix()->getWidth();
        int h = npc->getMatrix()->getHeight();

        for(int m = 0; m < w; m++)
          for(int n = 0; n < h; n++)
            sub_maps[i]->tiles[x+m][y+n]->
                               unsetNPC(npc->getMatrix()->getRenderDepth(m, n));
      }
    }
  }
  else
  {
    if(active_submap != NULL)
    {
      for(int i = 0; i < active_submap->npcs.size(); i++)
      {
        EditorMapNPC* npc = active_submap->npcs[i];
        int x = npc->getX();
        int y = npc->getY();
        int w = npc->getMatrix()->getWidth();
        int h = npc->getMatrix()->getHeight();

        for(int j = 0; j < w; j++)
          for(int k = 0; k < h; k++)
            active_submap->tiles[x+j][y+k]->
                               unsetNPC(npc->getMatrix()->getRenderDepth(j, k));
      }
    }
  }
}

/*
 * Description: Re-adds all sub-map persons to tiles. This is called after
 *              calling tilesPersonRemove(), processing the person, and then
 *              re-calling. Any person instances that can't be re-added are
 *              deleted and removed from the list.
 * TODO: FUTURE - warn about persons being removed??
 *
 * Inputs: bool update_all - true if updating all sub-maps or just active one
 * Output: none
 */
void EditorMap::tilesPersonAdd(bool update_all)
{
  /* Update all sub-maps - used for loading */
  if(update_all)
  {
    for(int i = 0; i < sub_maps.size(); i++)
    {
      for(int j = 0; j < sub_maps[i]->persons.size(); j++)
      {
        if(!addPerson(sub_maps[i]->persons[j], sub_maps[i]))
        {
          delete sub_maps[i]->persons[j];
          sub_maps[i]->persons.remove(j);
          j--;
        }
      }
    }
  }
  /* Or just the active one */
  else
  {
    if(active_submap != NULL)
    {
      for(int i = 0; i < active_submap->persons.size(); i++)
      {
        if(!addPerson(active_submap->persons[i]))
        {
          delete active_submap->persons[i];
          active_submap->persons.remove(i);
          i--;
        }
      }
    }
  }
}

/*
 * Description: Removes all sub-map persons from tiles. This is called before
 *              processing person changes, and then calling tilesPersonAdd().
 *              Just removes the persons from the tiles but still stored into
 *              memory.
 *
 * Inputs: bool update_all - true if updating all sub-maps or just active one
 * Output: none
 */
void EditorMap::tilesPersonRemove(bool update_all)
{
  /* Update all sub-maps - used for loading / resizing */
  if(update_all)
  {
    for(int i = 0; i < sub_maps.size(); i++)
    {
      for(int j = 0; j < sub_maps[i]->persons.size(); j++)
      {
        EditorMapPerson* person = sub_maps[i]->persons[j];
        int x = person->getX();
        int y = person->getY();
        int w = person->getMatrix()->getWidth();
        int h = person->getMatrix()->getHeight();

        for(int m = 0; m < w; m++)
          for(int n = 0; n < h; n++)
            sub_maps[i]->tiles[x+m][y+n]->
                         unsetPerson(person->getMatrix()->getRenderDepth(m, n));
      }
    }
  }
  else
  {
    if(active_submap != NULL)
    {
      /* Clear selection */
      setHoverThing(-1);

      for(int i = 0; i < active_submap->persons.size(); i++)
      {
        EditorMapPerson* person = active_submap->persons[i];
        int x = person->getX();
        int y = person->getY();
        int w = person->getMatrix()->getWidth();
        int h = person->getMatrix()->getHeight();

        for(int j = 0; j < w; j++)
          for(int k = 0; k < h; k++)
            active_submap->tiles[x+j][y+k]->
                         unsetPerson(person->getMatrix()->getRenderDepth(j, k));
      }
    }
  }
}

/*
 * Description: Re-adds all sub-map things to tiles. This is called after
 *              calling tilesThingRemove(), processing the thing, and then
 *              re-calling. Any thing instances that can't be re-added are
 *              deleted and removed from the list.
 * TODO: FUTURE - warn about things being removed??
 *
 * Inputs: bool update_all - true if updating all sub-maps or just active one
 * Output: none
 */
void EditorMap::tilesThingAdd(bool update_all)
{
  /* Update all sub-maps - used for loading */
  if(update_all)
  {
    for(int i = 0; i < sub_maps.size(); i++)
    {
      for(int j = 0; j < sub_maps[i]->things.size(); j++)
      {
        if(!addThing(sub_maps[i]->things[j], sub_maps[i]))
        {
          delete sub_maps[i]->things[j];
          sub_maps[i]->things.remove(j);
          j--;
        }
      }
    }
  }
  /* Or just the active one */
  else
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
}

/*
 * Description: Removes all sub-map things from tiles. This is called before
 *              processing thing changes, and then calling tilesThingAdd().
 *              Just removes the things from the tiles but still stored into
 *              memory.
 *
 * Inputs: bool update_all - true if updating all sub-maps or just active one
 * Output: none
 */
void EditorMap::tilesThingRemove(bool update_all)
{
  /* Update all sub-maps - used for loading / resizing */
  if(update_all)
  {
    for(int i = 0; i < sub_maps.size(); i++)
    {
      for(int j = 0; j < sub_maps[i]->things.size(); j++)
      {
        EditorMapThing* thing = sub_maps[i]->things[j];
        int x = thing->getX();
        int y = thing->getY();
        int w = thing->getMatrix()->getWidth();
        int h = thing->getMatrix()->getHeight();

        for(int m = 0; m < w; m++)
          for(int n = 0; n < h; n++)
            sub_maps[i]->tiles[x+m][y+n]->
                           unsetThing(thing->getMatrix()->getRenderDepth(m, n));
      }
    }
  }
  else
  {
    if(active_submap != NULL)
    {
      /* Clear selection */
      setHoverThing(-1);

      for(int i = 0; i < active_submap->things.size(); i++)
      {
        EditorMapThing* thing = active_submap->things[i];
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
 * Description: Updates the tiles related to the thing, assuming its an
 *              instant and is placed on the map
 *
 * Inputs: EditorMapPerson* ref - the reference person to determine the tile set
 * Output: none
 */
void EditorMap::updateTiles(EditorMapPerson* ref)
{
  SubMapInfo* sub_ref = getPersonSub(ref);
  if(sub_ref != nullptr)
    updateTiles(getPersonSub(ref), ref->getX(), ref->getY(),
                ref->getMatrix()->getWidth(), ref->getMatrix()->getHeight());
}

/*
 * Description: Unset the IO within the list that correspond to the ID.
 *
 * Inputs: int id - the IO id
 *         bool from_sub - true if it should be unset in sub-maps instead
 * Output: bool - was a IO deleted?
 */
bool EditorMap::unsetIO(int id, bool from_sub)
{
  /* If not sub-maps, it's base and try to remove ID */
  if(!from_sub)
  {
    int index = getIOIndex(id);
    return unsetIOByIndex(index);
  }
  /* Otherwise, check all sub-maps */
  else
  {
    bool deleted = false;

    for(int i = 0; i < sub_maps.size(); i++)
    {
      int index = getIOIndex(id, i);
      deleted |= unsetIOByIndex(index, i);
    }

    return deleted;
  }
}

/*
 * Description: Unset the IO within the list that correspond to the index.
 *
 * Inputs: int index - the IO index corresponder
 *         int sub_map - the sub-map to get the IOs for (<0 is base)
 * Output: bool - was a IO deleted?
 */
bool EditorMap::unsetIOByIndex(int index, int sub_map)
{
  if(sub_map < 0)
  {
    if(index >= 0 && index < base_ios.size())
    {
      /* Remove all IOs related to this index - sub IOs */
      for(int i = 0; i < sub_maps.size(); i++)
      {
        for(int j = 0; j < sub_maps[i]->ios.size(); j++)
        {
          if(sub_maps[i]->ios[j]->getBaseIO() != NULL &&
             sub_maps[i]->ios[j]->getBaseIO()->getID() ==
             base_ios[index]->getID())
          {
            unsetIOByIndex(j, i);
            j--;
          }
        }
      }

      /* Finally, delete the IO */
      delete base_ios[index];
      base_ios.remove(index);

      return true;
    }
  }
  else if(sub_map < sub_maps.size())
  {
    if(index >= 0 && index < sub_maps[sub_map]->ios.size())
    {
      EditorMapIO* ref = sub_maps[sub_map]->ios[index];

      /* Remove the instances from tiles */
      int x = ref->getX();
      int y = ref->getY();
      for(int i = x; i < (ref->getMatrix()->getWidth() + x) &&
                     i < sub_maps[sub_map]->tiles.size(); i++)
      {
        for(int j = y; j < (ref->getMatrix()->getHeight() + y) &&
                                 j < sub_maps[sub_map]->tiles[i].size(); j++)
        {
          sub_maps[sub_map]->tiles[i][j]->unsetIO(ref);
        }
      }

      /* Finally, delete the IO */
      delete ref;
      sub_maps[sub_map]->ios.remove(index);

      /* Update list */
      emit ioInstanceChanged("");
      setHoverThing(-1);

      return true;
    }
  }

  return false;
}

/*
 * Description: Unsets all map IOs within the EditorMap set.
 *
 * Inputs: bool from_sub - true if it should be unset in sub-maps instead
 * Output: none
 */
void EditorMap::unsetIOs(bool from_sub)
{
  /* If not sub-maps, it's base and try to remove ID */
  if(!from_sub)
  {
    while(base_ios.size() > 0)
      unsetIOByIndex(0);
    base_ios.clear();
  }
  /* Otherwise, check all sub-maps */
  else
  {
    for(int i = 0; i < sub_maps.size(); i++)
    {
      while(sub_maps[i]->ios.size() > 0)
        unsetIOByIndex(0, i);
      sub_maps[i]->ios.clear();
    }
  }
}

/*
 * Description: Unset the item within the list that correspond to the ID.
 *
 * Inputs: int id - the item id
 *         bool from_sub - true if it should be unset in sub-maps instead
 * Output: bool - was a item deleted?
 */
bool EditorMap::unsetItem(int id, bool from_sub)
{
  /* If not sub-maps, it's base and try to remove ID */
  if(!from_sub)
  {
    int index = getItemIndex(id);
    return unsetItemByIndex(index);
  }
  /* Otherwise, check all sub-maps */
  else
  {
    bool deleted = false;

    for(int i = 0; i < sub_maps.size(); i++)
    {
      int index = getItemIndex(id, i);
      deleted |= unsetItemByIndex(index, i);
    }

    return deleted;
  }
}

/*
 * Description: Unset the item within the list that correspond to the index.
 *
 * Inputs: int index - the item index corresponder
 *         int sub_map - the sub-map to get the items for (<0 is base)
 * Output: bool - was a item deleted?
 */
bool EditorMap::unsetItemByIndex(int index, int sub_map)
{
  if(sub_map < 0)
  {
    if(index >= 0 && index < base_items.size())
    {
      /* Remove all items related to this index - sub persons */
      for(int i = 0; i < sub_maps.size(); i++)
      {
        for(int j = 0; j < sub_maps[i]->items.size(); j++)
        {
          if(sub_maps[i]->items[j]->getBaseItem() != NULL &&
             sub_maps[i]->items[j]->getBaseItem()->getID() ==
             base_items[index]->getID())
          {
            unsetItemByIndex(j, i);
            j--;
          }
        }
      }

      /* Finally, delete the item */
      delete base_items[index];
      base_items.remove(index);

      return true;
    }
  }
  else if(sub_map < sub_maps.size())
  {
    if(index >= 0 && index < sub_maps[sub_map]->items.size())
    {
      EditorMapItem* ref = sub_maps[sub_map]->items[index];

      /* Remove the instances from tiles */
      int x = ref->getX();
      int y = ref->getY();
      sub_maps[sub_map]->tiles[x][y]->unsetItem(ref);

      /* Finally, delete the item */
      delete ref;
      sub_maps[sub_map]->items.remove(index);

      /* Update list */
      emit itemInstanceChanged("");
      setHoverItem(-1);

      return true;
    }
  }

  return false;
}

/*
 * Description: Unsets all map items within the EditorMap set.
 *
 * Inputs: bool from_sub - true if it should be unset in sub-maps instead
 * Output: none
 */
void EditorMap::unsetItems(bool from_sub)
{
  /* If not sub-maps, it's base and try to remove ID */
  if(!from_sub)
  {
    while(base_items.size() > 0)
      unsetItemByIndex(0);
    base_items.clear();
  }
  /* Otherwise, check all sub-maps */
  else
  {
    for(int i = 0; i < sub_maps.size(); i++)
    {
      while(sub_maps[i]->items.size() > 0)
        unsetItemByIndex(0, i);
      sub_maps[i]->items.clear();
    }
  }
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

    /* Delete all ios from sub-map */
    while(sub_maps[index]->ios.size() > 0)
      unsetIOByIndex(0, index);

    /* Delete all items froms ub-map */
    while(sub_maps[index]->items.size() > 0)
      unsetItemByIndex(0, index);

    /* Delete all persons from sub-map */
    while(sub_maps[index]->persons.size() > 0)
      unsetPersonByIndex(0, index);

    /* Delete all npcs from sub-map */
    while(sub_maps[index]->npcs.size() > 0)
      unsetNPCByIndex(0, index);

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
 * Description: Unset the npc within the list that correspond to the ID.
 *
 * Inputs: int id - the npc id
 *         bool from_sub - true if it should be unset in sub-maps instead
 * Output: bool - was a npc deleted?
 */
bool EditorMap::unsetNPC(int id, bool from_sub)
{
  /* If not sub-maps, it's base and try to remove ID */
  if(!from_sub)
  {
    int index = getNPCIndex(id);
    return unsetNPCByIndex(index);
  }
  /* Otherwise, check all sub-maps */
  else
  {
    bool deleted = false;

    for(int i = 0; i < sub_maps.size(); i++)
    {
      int index = getNPCIndex(id, i);
      deleted |= unsetNPCByIndex(index, i);
    }

    return deleted;
  }
}

/*
 * Description: Unset the npc within the list that correspond to the index.
 *
 * Inputs: int index - the npc index corresponder
 *         int sub_map - the sub-map to get the npcs for (<0 is base)
 * Output: bool - was a npc deleted?
 */
bool EditorMap::unsetNPCByIndex(int index, int sub_map)
{
  if(sub_map < 0)
  {
    if(index >= 0 && index < base_npcs.size())
    {
      /* Remove all npcs related to this index - sub npcs */
      for(int i = 0; i < sub_maps.size(); i++)
      {
        for(int j = 0; j < sub_maps[i]->npcs.size(); j++)
        {
          if(sub_maps[i]->npcs[j]->getBaseNPC() != NULL &&
             sub_maps[i]->npcs[j]->getBaseNPC()->getID() ==
             base_npcs[index]->getID())
          {
            unsetNPCByIndex(j, i);
            j--;
          }
        }
      }

      /* Finally, delete the npc */
      delete base_npcs[index];
      base_npcs.remove(index);

      return true;
    }
  }
  else if(sub_map < sub_maps.size())
  {
    if(index >= 0 && index < sub_maps[sub_map]->npcs.size())
    {
      EditorMapNPC* ref = sub_maps[sub_map]->npcs[index];

      /* Remove the instances from tiles */
      int x = ref->getX();
      int y = ref->getY();
      for(int i = x; i < (ref->getMatrix()->getWidth() + x) &&
                     i < sub_maps[sub_map]->tiles.size(); i++)
      {
        for(int j = y; j < (ref->getMatrix()->getHeight() + y) &&
                                 j < sub_maps[sub_map]->tiles[i].size(); j++)
        {
          sub_maps[sub_map]->tiles[i][j]->unsetNPC(ref);
        }
      }

      /* Check to make sure its path is not the head path and close signals */
      if(sub_maps[sub_map]->path_top == ref->getPath())
        sub_maps[sub_map]->path_top = NULL;
      disconnect(ref->getPath(), SIGNAL(hoverInit(EditorNPCPath*)),
                 this, SLOT(npcHoverPathChanged(EditorNPCPath*)));
      if(sub_maps[sub_map] == active_submap)
        emit npcPathRemove(ref->getPath());

      /* Finally, delete the npc */
      delete ref;
      sub_maps[sub_map]->npcs.remove(index);

      /* Update list */
      emit npcInstanceChanged("");
      setHoverNPC(-1);

      return true;
    }
  }

  return false;
}

/*
 * Description: Unsets all map npcs within the EditorMap set.
 *
 * Inputs: bool from_sub - true if it should be unset in sub-maps instead
 * Output: none
 */
void EditorMap::unsetNPCs(bool from_sub)
{
  /* If not sub-maps, it's base and try to remove ID */
  if(!from_sub)
  {
    while(base_npcs.size() > 0)
      unsetNPCByIndex(0);
    base_npcs.clear();
  }
  /* Otherwise, check all sub-maps */
  else
  {
    for(int i = 0; i < sub_maps.size(); i++)
    {
      while(sub_maps[i]->npcs.size() > 0)
        unsetNPCByIndex(0, i);
      sub_maps[i]->npcs.clear();
    }
  }
}

/*
 * Description: Unset the person within the list that correspond to the ID.
 *
 * Inputs: int id - the person id
 *         bool from_sub - true if it should be unset in sub-maps instead
 * Output: bool - was a person deleted?
 */
bool EditorMap::unsetPerson(int id, bool from_sub)
{
  /* If not sub-maps, it's base and try to remove ID */
  if(!from_sub)
  {
    int index = getPersonIndex(id);
    return unsetPersonByIndex(index);
  }
  /* Otherwise, check all sub-maps */
  else
  {
    bool deleted = false;

    for(int i = 0; i < sub_maps.size(); i++)
    {
      int index = getPersonIndex(id, i);
      deleted |= unsetPersonByIndex(index, i);
    }

    return deleted;
  }
}

/*
 * Description: Unset the person within the list that correspond to the index.
 *
 * Inputs: int index - the person index corresponder
 *         int sub_map - the sub-map to get the persons for (<0 is base)
 * Output: bool - was a person deleted?
 */
bool EditorMap::unsetPersonByIndex(int index, int sub_map)
{
  if(sub_map < 0)
  {
    if(index >= 0 && index < base_persons.size())
    {
      /* Remove all persons related to this index - sub persons */
      for(int i = 0; i < sub_maps.size(); i++)
      {
        for(int j = 0; j < sub_maps[i]->persons.size(); j++)
        {
          if(sub_maps[i]->persons[j]->getBasePerson() != NULL &&
             sub_maps[i]->persons[j]->getBasePerson()->getID() ==
             base_persons[index]->getID())
          {
            unsetPersonByIndex(j, i);
            j--;
          }
        }
      }

      /* Finally, delete the person */
      delete base_persons[index];
      base_persons.remove(index);

      return true;
    }
  }
  else if(sub_map < sub_maps.size())
  {
    if(index >= 0 && index < sub_maps[sub_map]->persons.size())
    {
      EditorMapPerson* ref = sub_maps[sub_map]->persons[index];

      /* Remove the instances from tiles */
      int x = ref->getX();
      int y = ref->getY();
      for(int i = x; i < (ref->getMatrix()->getWidth() + x) &&
                     i < sub_maps[sub_map]->tiles.size(); i++)
      {
        for(int j = y; j < (ref->getMatrix()->getHeight() + y) &&
                                 j < sub_maps[sub_map]->tiles[i].size(); j++)
        {
          sub_maps[sub_map]->tiles[i][j]->unsetPerson(ref);
        }
      }

      /* Finally, delete the person */
      delete ref;
      sub_maps[sub_map]->persons.remove(index);

      /* Update list */
      emit personInstanceChanged("");
      setHoverPerson(-1);

      return true;
    }
  }

  return false;
}

/*
 * Description: Unsets all map persons within the EditorMap set.
 *
 * Inputs: bool from_sub - true if it should be unset in sub-maps instead
 * Output: none
 */
void EditorMap::unsetPersons(bool from_sub)
{
  /* If not sub-maps, it's base and try to remove ID */
  if(!from_sub)
  {
    while(base_persons.size() > 0)
      unsetPersonByIndex(0);
    base_persons.clear();
  }
  /* Otherwise, check all sub-maps */
  else
  {
    for(int i = 0; i < sub_maps.size(); i++)
    {
      while(sub_maps[i]->persons.size() > 0)
        unsetPersonByIndex(0, i);
      sub_maps[i]->persons.clear();
    }
  }
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
          if(sub_maps[i]->things[j]->getBaseThing() != NULL &&
             sub_maps[i]->things[j]->getBaseThing()->getID() ==
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
      EditorMapThing* ref = sub_maps[sub_map]->things[index];

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
      emit thingInstanceChanged("");
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
