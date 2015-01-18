/*******************************************************************************
 * Class Name: EditorMap
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Map
 ******************************************************************************/
#include "Database/EditorMap.h"
#include <QDebug>

/* Constant Implementation - see header file for descriptions */
const int EditorMap::kUNSET_ID = -1;

/*
 * Description: The set-up constructor
 *
 * Inputs: none
 */
EditorMap::EditorMap()//QWidget *parent) : QWidget(parent)
{
  id = kUNSET_ID;
  name = "";
  tile_icons = NULL;
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

/* Copy constructor */
EditorMap::EditorMap(const EditorMap &source)
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

/* Adds tile sprite data */
// TODO: Fix for passability
// TODO: Optimize for rects dissilation in a multi section field
void EditorMap::addTileSpriteData(FileHandler* fh, int index,
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


    for(int j = 0; j < sub_maps[index]->tiles.size(); j++)
    {
      for(int k = 0; k < sub_maps[index]->tiles[j].size(); k++)
      {
        if(sub_maps[index]->tiles[j][k]->getSprite(layer) == sprites[i])
        {
          int pass = sub_maps[index]->tiles[j][k]->getPassabilityNum(layer);
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

/* Copy function, to be called by a copy or equal operator constructor */
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
        row.push_back(new EditorTile(*source.sub_maps[i]->tiles[j][k]));
      sub_maps.last()->tiles.push_back(row);
    }

    /* Fix the tile pointers to the new sprite sets */
    for(int j = 0; j < sub_maps.last()->tiles.size(); j++)
    {
      for(int k = 0; k < sub_maps.last()->tiles[j].size(); k++)
      {
        for(int m = 0; m < EditorEnumDb::INVALID; m++)
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

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Returns the ID of the editor map.
 *
 * Inputs: none
 * Output: int - map id. Less than 0 if unset
 */
int EditorMap::getID()
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
QString EditorMap::getName()
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
 * Description: Returns the tile icons that is stored within the editor map.
 *
 * Inputs: none
 * Output: TileIcons* - the icon struct.
 */
TileIcons* EditorMap::getTileIcons()
{
  return tile_icons;
}

/* Loads the map */
void EditorMap::load(FileHandler* fh)
{
  // TODO: Future - fix as per game database and match
}

/* Saves the map */
void EditorMap::save(FileHandler* fh, bool game_only)
{
  if(fh != NULL)
  {
    fh->writeXmlElement("map", "id", getID());

    /* Add sprites */
    for(int i = 0; i < sprites.size(); i++)
      sprites[i]->save(fh, game_only);

    /* Add the sub-maps */
    for(int i = 0; i < sub_maps.size(); i++)
    {
      /* Initial element */
      if(i == 0)
        fh->writeXmlElement("main");
      else
        fh->writeXmlElement("section", "index", sub_maps[i]->id);

      /* Add size and name */
      if(!game_only)
        fh->writeXmlData("name", sub_maps[i]->name.toStdString());
      fh->writeXmlData("width", sub_maps[i]->tiles.size());
      fh->writeXmlData("height", sub_maps[i]->tiles.front().size());

      /* Add base tiles */
      fh->writeXmlElement("base");
      addTileSpriteData(fh, i, EditorEnumDb::BASE);
      fh->writeXmlElementEnd();

      /* Add enhancer tiles */
      fh->writeXmlElement("enhancer");
      addTileSpriteData(fh, i, EditorEnumDb::ENHANCER);
      fh->writeXmlElementEnd();

      /* Add lower tiles */
      fh->writeXmlElement("lower", "index", "0");
      addTileSpriteData(fh, i, EditorEnumDb::LOWER1);
      fh->writeXmlElementEnd();
      fh->writeXmlElement("lower", "index", "1");
      addTileSpriteData(fh, i, EditorEnumDb::LOWER2);
      fh->writeXmlElementEnd();
      fh->writeXmlElement("lower", "index", "2");
      addTileSpriteData(fh, i, EditorEnumDb::LOWER3);
      fh->writeXmlElementEnd();
      fh->writeXmlElement("lower", "index", "3");
      addTileSpriteData(fh, i, EditorEnumDb::LOWER4);
      fh->writeXmlElementEnd();
      fh->writeXmlElement("lower", "index", "4");
      addTileSpriteData(fh, i, EditorEnumDb::LOWER5);
      fh->writeXmlElementEnd();

      /* Add upper tiles */
      fh->writeXmlElement("upper", "index", "0");
      addTileSpriteData(fh, i, EditorEnumDb::UPPER1);
      fh->writeXmlElementEnd();
      fh->writeXmlElement("upper", "index", "1");
      addTileSpriteData(fh, i, EditorEnumDb::UPPER2);
      fh->writeXmlElementEnd();
      fh->writeXmlElement("upper", "index", "2");
      addTileSpriteData(fh, i, EditorEnumDb::UPPER3);
      fh->writeXmlElementEnd();
      fh->writeXmlElement("upper", "index", "3");
      addTileSpriteData(fh, i, EditorEnumDb::UPPER4);
      fh->writeXmlElementEnd();
      fh->writeXmlElement("upper", "index", "4");
      addTileSpriteData(fh, i, EditorEnumDb::UPPER5);
      fh->writeXmlElementEnd();

      /* End element */
      fh->writeXmlElementEnd();
    }

    fh->writeXmlElementEnd();
  }
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
    for(int i = 0; i < (int)EditorEnumDb::INVALID; i++)
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
        row.push_back(new EditorTile(i, j, tile_icons));

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
  for(int i = 0; i < sub_maps.size(); i++)
    delete sub_maps[i];
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
  for(int i = 0; i < sprites.size(); i++)
  {
    /* Remove all sprites related to this index from all tiles */
    for(int i = 0; i < sub_maps.size(); i++)
    {
      for(int j = 0; j < sub_maps[i]->tiles.size(); j++)
        for(int k = 0; k < sub_maps[i]->tiles[j].size(); k++)
          sub_maps[i]->tiles[j][k]->unplace(sprites[i]);
    }

    /* Finally, delete the sprite */
    delete sprites[i];
  }
  sprites.clear();
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
