/*******************************************************************************
 * Class Name: EditorMap
 * Date Created: December 27, 2014
 * Inheritance: QObject, EditorTemplate
 * Description: The map interface to connect and edit in the editor
 ******************************************************************************/
#ifndef EDITORMAP_H
#define EDITORMAP_H

#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QPushButton>
#include <QString>
#include <QVector>

#include "Database/EditorSprite.h"
#include "Database/EditorTemplate.h"
#include "Database/EditorMapThing.h"
#include "Database/EditorTile.h"
#include "EditorEnumDb.h"
#include "EditorHelpers.h"
#include "FileHandler.h"

/* Struct for sub map info storage */
struct SubMapInfo
{
  int id;
  QString name;
  QVector<QVector<EditorTile*>> tiles;

  QVector<EditorMapThing*> things;
};

class EditorMap : public QObject, public EditorTemplate
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorMap();//QWidget* parent = NULL);

  /* Constructor function with id and name plus optional width and height
   * of base map */
  EditorMap(int id, QString name, int width = 0, int height = 0,
            TileIcons* icons = NULL);

  /* Copy constructor */
  EditorMap(const EditorMap &source);

  /* Destructor function */
  virtual ~EditorMap();

private:
  /* Active references in the map - used when displaying a map */
  HoverInfo active_info;
  SubMapInfo* active_submap;

  /* The base map things */
  QVector<EditorMapThing*> base_things;

  /* The map set ID */
  int id;

  /* The name of the map set */
  QString name;

  /* The map sprites */
  QVector<EditorSprite*> sprites;

  /* The set of sub-maps */
  QVector<SubMapInfo*> sub_maps;

  /* Rendering tile icons */
  TileIcons* tile_icons;

  /*------------------- Constants -----------------------*/
  const static int kBASE_ID_IOS; /* The instant base ID for MapIOs */
  const static int kBASE_ID_ITEMS; /* The instant base ID for items */
  const static int kBASE_ID_PERSON; /* The instant base ID for persons */
  const static int kBASE_ID_NPC; /* The instant base ID for npcs */
  const static int kBASE_ID_THING; /* The base thing ID */
  const static int kUNSET_ID; /* The unset ID */

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Attempts to add thing to the current sub-map */
  bool addThing(EditorMapThing* thing, SubMapInfo* map = NULL);

  /* Adds tile sprite data */
  void addTileSpriteData(FileHandler* fh, int index, EditorEnumDb::Layer layer);
  void addTileSpriteData(FileHandler* fh, SubMapInfo* map,
                         EditorEnumDb::Layer layer);

  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorMap &source);

  /* Loads sub-map info */
  void loadSubMap(SubMapInfo* map, XmlData data, int index);

  /* Recursively erase all similar adjoining tiles */
  void recursiveErase(int x, int y, EditorEnumDb::Layer layer,
                      EditorSprite* target, SubMapInfo* map);

  /* Recursively fill all similar adjoining tiles with the selected sprite */
  void recursiveFill(int x, int y, EditorEnumDb::Layer layer,
                     EditorSprite* target, SubMapInfo* map);

  /* Saves the sub-map */
  void saveSubMap(FileHandler* fh, bool game_only,
                  SubMapInfo* map, bool first = false);

  /* Updates the tiles that contain the hover information struct */
  bool updateHoverThing(bool unset = false);

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Thing instant changed */
  void thingInstanceChanged();

public slots:

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Clears the hover information - called on first initiation of map */
  void clearHoverInfo();

  /* Click trigger on tile in map */
  void clickTrigger(bool single = true, bool right_click = false);
  void clickTrigger(QList<EditorTile*> tiles, bool erase);

  /* Returns current references for lists in map */
  SubMapInfo* getCurrentMap();
  int getCurrentMapIndex();
  int getCurrentSpriteIndex();
  int getCurrentThingIndex();

  /* Returns the hover information */
  HoverInfo* getHoverInfo();

  /* Returns the ID of the map set */
  virtual int getID() const;

  /* Returns the stored map information */
  SubMapInfo* getMap(int id);
  SubMapInfo* getMapByIndex(int index);
  int getMapCount();
  int getMapIndex(int id);
  QVector<QString> getMapList();
  QString getMapNameList(int index);
  QVector<SubMapInfo*> getMaps();

  /* Returns the name of the map set */
  virtual QString getName() const;
  virtual QString getNameList();

  /* Returns available IDs in the set. Useful for when creating a new one */
  int getNextMapID();
  int getNextSpriteID();
  int getNextThingID(bool from_sub = false);

  /* Returns the set layers in a string */
  QString getSetLayers(int map_index, int x, int y);

  /* Return stored sprite information */
  EditorSprite* getSprite(int id);
  EditorSprite* getSpriteByIndex(int index);
  int getSpriteCount();
  int getSpriteIndex(int id);
  QVector<EditorSprite*> getSprites();

  /* Return stored thing information */
  EditorMapThing* getThing(int id, int sub_map = -1);
  EditorMapThing* getThingByIndex(int index, int sub_map = -1);
  int getThingCount(int sub_map = -1);
  int getThingIndex(int id, int sub_map = -1);
  QVector<QString> getThingList(int sub_map = -1, bool all_submaps = false,
                                bool shortened = false);
  QVector<EditorMapThing*> getThings(int sub_map = -1);

  /* Returns the tile icons */
  TileIcons* getTileIcons();

  /* Loads the map */
  void load(XmlData data, int index);

  /* Saves the map */
  void save(FileHandler* fh, bool game_only = false, int sub_index = -1);

  /* Sets the current references for the selected sprite(s) or thing(s) */
  bool setCurrentMap(int index);
  bool setCurrentSprite(int index);
  bool setCurrentThing(int index);

  /* Sets the hover information */
  void setHoverCursor(EditorEnumDb::CursorMode cursor);
  void setHoverLayer(EditorEnumDb::Layer layer);
  bool setHoverThing(int id);
  void setHoverTile(EditorTile* tile);

  /* Sets the ID of the map set */
  virtual void setID(int id);

  /* Sets a map, based on ID */
  int setMap(int id, QString name, QVector<QVector<EditorTile*>> tiles);
  int setMap(int id, QString name, int width, int height);

  /* Sets the name of the map set */
  virtual void setName(QString name);

  /* Sets a sprite */
  int setSprite(EditorSprite* sprite);

  /* Sets a thing in the map */
  int setThing(EditorMapThing* thing, int sub_map = -1);

  /* Sets the rendering tile icons */
  void setTileIcons(TileIcons* icons);

  /* Sets layer visibility */
  void setVisibility(EditorEnumDb::Layer layer, bool visible);

  /* Set grid and passability visibility */
  void setVisibilityGrid(bool);
  void setVisibilityPass(bool);

  /* Thing processing for updating with the new data */
  void thingAddToTiles(bool update_all = false);
  void thingRemoveFromTiles();

  /* Update all tiles */
  void updateAll();

  /* Unset map(s) */
  bool unsetMap(int id);
  bool unsetMapByIndex(int index);
  void unsetMaps();

  /* Unset sprite(s) */
  bool unsetSprite(int id);
  bool unsetSpriteByIndex(int index);
  void unsetSprites();

  /* Unset thing(s) */
  bool unsetThing(int id, bool from_sub = false);
  bool unsetThingByIndex(int index, int sub_map = -1);
  void unsetThings(bool from_sub = false);

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  EditorMap& operator= (const EditorMap &source);

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/
public:
  /* Creates the map size and name dialog */
  static QDialog* createMapDialog(QWidget* parent,
                                  QString title = "New Map Details",
                                  QString name = "Awesomeland",
                                  int width = 0, int height = 0);

  /* Copies information, except ID, from one sub-map to another */
  static bool copySubMap(SubMapInfo* copy_map, SubMapInfo* new_map, 
                         TileIcons* icons);

  /* Returns the push button for the map dialog above. Will seg with others */
  static QPushButton* getDialogButton(QDialog* dialog);

  /* Dialog information for map */
  static int getDialogHeight(QDialog* dialog);
  static QString getDialogName(QDialog* dialog);
  static int getDialogWidth(QDialog* dialog);
};

#endif // EDITORMAP_H
