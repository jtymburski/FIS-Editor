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
#include <QMap>
#include <QMessageBox>
#include <QObject>
#include <QProgressDialog>
#include <QPushButton>
#include <QString>
#include <QVector>

#include "Database/EditorEvent.h"
#include "Database/EditorTile.h"
#include "EditorEnumDb.h"
#include "EditorHelpers.h"
#include "FileHandler.h"

/* Struct for sub map info storage */
struct SubMapInfo
{
  /* Core data */
  int id;
  QString name;
  QVector<QVector<EditorTile*>> tiles;
  EditorNPCPath* path_top;

  /* Things and children */
  QVector<EditorMapIO*> ios;
  QVector<EditorMapItem*> items;
  QVector<EditorMapNPC*> npcs;
  QVector<EditorMapPerson*> persons;
  QVector<EditorMapThing*> things;

  /* Lay Overs */
  QVector<LayOver> lays_over;
  QVector<LayOver> lays_under;

  /* Scenes, music, and weather */
  QVector<int> battle_scenes;
  QVector<int> music;
  int weather;

  /* Editor control variables */
  QPointF center_point;
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
  QVector<EditorMapIO*> base_ios;
  QVector<EditorMapItem*> base_items;
  QVector<EditorMapNPC*> base_npcs;
  QVector<EditorMapPerson*> base_persons;
  QVector<EditorMapThing*> base_things;

  /* The base set of battle scenes */
  QVector<int> battle_scenes;

  /* The map set ID */
  int id;

  /* The name of the map set */
  QString name;

  /* Reference tile - contains information stored in tiles */
  EditorTile ref_tile;

  /* The map sprites */
  QVector<EditorSprite*> sprites;

  /* The set of sub-maps */
  QVector<SubMapInfo*> sub_maps;

  /* Rendering tile icons */
  TileIcons* tile_icons;

  /* Visibility status of path */
  bool visible_path;

  /*------------------- Constants -----------------------*/
  const static int kUNSET_ID; /* The unset ID */

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Attempts to add io to the current sub-map */
  bool addIO(EditorMapIO* io, SubMapInfo* map = NULL,
             bool existing = true);

  /* Attempts to add item to the current sub-map */
  bool addItem(EditorMapItem* item, SubMapInfo* map = NULL,
               bool existing = true);

  /* Attempts to add npc to the current sub-map */
  bool addNPC(EditorMapNPC* npc, SubMapInfo* map = NULL,
              bool existing = true);

  /* Attempts to add person to the current sub-map */
  bool addPerson(EditorMapPerson* person, SubMapInfo* map = NULL,
                 bool existing = true);

  /* Attempts to add thing to the current sub-map */
  bool addThing(EditorMapThing* thing, SubMapInfo* map = NULL,
                bool existing = true);

  /* Attempts to add thing by internal type */
  bool addThingGeneric(EditorMapThing* thing, SubMapInfo* map = nullptr,
                       bool existing = true);

  /* Adds tile sprite data */
  void addTilePassData(FileHandler* fh, QProgressDialog* save_dialog,
                       QList<QList<QPoint>> data_set);
  void addTileSpriteData(FileHandler* fh, QProgressDialog* save_dialog,
                         QList<QList<QPoint>> data_set);

  /* Clear map data */
  void clearAll();

  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorMap &source);

  /* Loads sub-map info */
  void loadSubMap(SubMapInfo* map, XmlData data, int index);

  /* Re-color NPC paths (triggered on add) */
  void recolorNPCPaths(SubMapInfo* map);

  /* Recursively erase all similar adjoining tiles */
  void recursiveErase(int x, int y, EditorEnumDb::Layer layer,
                      EditorSprite* target, SubMapInfo* map);

  /* Recursively fill all similar adjoining tiles with the selected sprite */
  void recursiveFill(int x, int y, EditorEnumDb::Layer layer,
                     EditorSprite* target, SubMapInfo* map);

  /* Resizes sub-maps */
  bool resizeMap(SubMapInfo* map, int width, int height);

  /* Saves the sub-map */
  void saveSubMap(FileHandler* fh, QProgressDialog* save_dialog, bool game_only,
                  SubMapInfo* map, bool first = false);

  /* Sets the hover thing, based on the passed in rect */
  bool setHoverThing(EditorMapThing* thing);

  /* Updates the tiles that contain the hover information struct */
  bool updateHoverThing(bool unset = false);

  /* Updates the given tile range within the sub-map */
  void updateTiles(SubMapInfo* map, int x, int y, int w, int h);

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Active sub-map changed */
  void activeSubChanged();

  /* IO instant changed */
  void ioInstanceChanged(QString name_list);

  /* Item base changed */
  void itemBasesChanged();

  /* Item instant changed */
  void itemInstanceChanged(QString name_list);

  /* Lay-over change on active sub trigger */
  void laysChanged();

  /* NPC instant changed */
  void npcInstanceChanged(QString name_list);

  /* Path mods to the map */
  void npcPathAdd(EditorNPCPath* path);
  void npcPathRemove(EditorNPCPath* path);

  /* Person instant changed */
  void personInstanceChanged(QString name_list);

  /* Sprite index change */
  void spriteIndexChange(int index);

  /* Thing instant changed */
  void thingInstanceChanged(QString name_list);

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/
public slots:
  /* Hover path changed */
  void npcHoverPathChanged(EditorNPCPath* path);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Clears the hover information - called on first initiation of map */
  void clearHoverInfo();

  /* Click trigger on tile in map */
  void clickTrigger(bool single = true, bool right_click = false);
  void clickTrigger(QList<EditorTile*> tiles, bool erase);

  /* Copies information, except ID, from one sub-map to another */
  bool copySubMap(SubMapInfo* copy_map, SubMapInfo* new_map);

  /* Ctrl + click trigger in map */
  void ctrlClickTrigger();

  /* Battle scene core access information */
  int getBattleScene(int index);
  int getBattleSceneCount();
  QVector<int> getBattleScenes();

  /* Returns current references for lists in map */
  int getCurrentIOIndex();
  int getCurrentItemIndex();
  SubMapInfo* getCurrentMap();
  int getCurrentMapIndex();
  int getCurrentNPCIndex();
  int getCurrentPersonIndex();
  int getCurrentSpriteIndex();
  int getCurrentThingIndex();

  /* Returns the hover information */
  HoverInfo* getHoverInfo();

  /* Returns the ID of the map set */
  virtual int getID() const;

  /* Returns stored IO information */
  EditorMapIO* getIO(int id, int sub_map = -1);
  EditorMapIO* getIOByIndex(int index, int sub_map = -1);
  int getIOCount(int sub_map = -1);
  int getIOIndex(int id, int sub_map = -1);
  QList<QString> getIOList(int sub_map = -1, bool all_submaps = false,
                             bool shortened = false);
  QList<QPair<QString,QString>> getIOListSet(int sub_map = -1,
                                             bool all_submaps = false,
                                             bool shortened = false);
  QVector<EditorMapIO*> getIOs(int sub_map = -1);

  /* Return stored item information */
  EditorMapItem* getItem(int id, int sub_map = -1);
  EditorMapItem* getItemByIndex(int index, int sub_map = -1);
  int getItemCount(int sub_map = -1);
  int getItemIndex(int id, int sub_map = -1);
  QList<QString> getItemList(int sub_map = -1, bool all_submaps = false,
                             bool shortened = false);
  QVector<EditorMapItem*> getItems(int sub_map = -1);

  /* Returns the stored map information */
  SubMapInfo* getMap(int id);
  SubMapInfo* getMapByIndex(int index);
  int getMapCount();
  int getMapIndex(int id);
  QList<QString> getMapList();
  QString getMapNameList(int index);
  QVector<SubMapInfo*> getMaps();

  /* Returns the max sprite ID */
  int getMaxSpriteID();

  /* Returns the name of the map set */
  virtual QString getName() const;
  virtual QString getNameList();

  /* Returns available IDs in the set. Useful for when creating a new one */
  int getNextIOID(bool from_sub = false);
  int getNextItemID(bool from_sub = false);
  int getNextMapID();
  int getNextNPCID(bool from_sub = false);
  int getNextPersonID(bool from_sub = false);
  int getNextSpriteID();
  int getNextThingID(bool from_sub = false);

  /* Return stored npc information */
  EditorMapNPC* getNPC(int id, int sub_map = -1);
  EditorMapNPC* getNPCByIndex(int index, int sub_map = -1);
  int getNPCCount(int sub_map = -1);
  int getNPCIndex(int id, int sub_map = -1);
  QList<QString> getNPCList(int sub_map = -1, bool all_submaps = false,
                            bool shortened = false);
  QVector<EditorMapNPC*> getNPCs(int sub_map = -1);

  /* Return stored person information */
  EditorMapPerson* getPerson(int id, int sub_map = -1);
  EditorMapPerson* getPersonByIndex(int index, int sub_map = -1);
  int getPersonCount(int sub_map = -1);
  int getPersonIndex(int id, int sub_map = -1);
  QList<QString> getPersonList(int sub_map = -1, bool all_submaps = false,
                               bool shortened = false);
  SubMapInfo* getPersonSub(EditorMapPerson* ref);
  QVector<EditorMapPerson*> getPersons(int sub_map = -1);

  /* Returns the number of things that will be saved */
  int getSaveCount(int sub_index = -1);

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
  QList<QString> getThingList(int sub_map = -1, bool all_submaps = false,
                              bool shortened = false);
  QVector<EditorMapThing*> getThings(int sub_map = -1);

  /* Returns the tile icons */
  TileIcons* getTileIcons();

  /* Returns if a new thing (or children) is possible to be created */
  bool isSpaceForIO(bool instance = false);
  bool isSpaceForItem(bool instance = false);
  bool isSpaceForNPC(bool instance = false);
  bool isSpaceForPerson(bool instance = false);
  bool isSpaceForThing(bool instance = false);

  /* Loads the map */
  void load(XmlData data, int index);

  /* Resizes sub-maps */
  bool resizeMap(int index, int width, int height);

  /* Saves the map */
  void save(FileHandler* fh, QProgressDialog* save_dialog,
            bool game_only = false, int sub_index = -1);

  /* Sets the battle scene reference ID for the core base */
  bool setBattleScene(int index, int id, bool insert = false);

  /* Sets the current references for the selected sprite(s) or thing(s) */
  bool setCurrentIO(int index);
  bool setCurrentItem(int index);
  bool setCurrentMap(int index);
  bool setCurrentNPC(int index);
  bool setCurrentPerson(int index);
  bool setCurrentSprite(int index);
  bool setCurrentThing(int index);

  /* Sets the hover information */
  void setHoverCursor(EditorEnumDb::CursorMode cursor);
  bool setHoverIO(int id);
  bool setHoverItem(int id);
  void setHoverLayer(EditorEnumDb::Layer layer);
  bool setHoverNPC(int id);
  void setHoverPathMode(bool path_mode);
  bool setHoverPerson(int id);
  bool setHoverThing(int id);
  void setHoverTile(EditorTile* tile);

  /* Sets the ID of the map set */
  virtual void setID(int id);

  /* Sets a IO in the map */
  int setIO(EditorMapIO* io, int sub_map = -1);

  /* Sets a item in the map */
  int setItem(EditorMapItem* item, int sub_map = -1);

  /* Correlates the set of core item data with the map base items */
  bool setItems(QList<ItemData> items, bool delete_invalid = true);

  /* Sets a map, based on ID */
  int setMap(int id, QString name, QVector<QVector<EditorTile*>> tiles,
             bool update_vis = true);
  int setMap(int id, QString name, int width, int height,
             bool update_vis = true);

  /* Sets the name of the map set */
  virtual void setName(QString name, bool update = true);

  /* Sets a npc in the map */
  int setNPC(EditorMapNPC* npc, int sub_map = -1);

  /* Set paths enabled - higher over-ride then visibility */
  void setPathsEnabled(bool enabled);

  /* Sets a person in the map */
  int setPerson(EditorMapPerson* person, int sub_map = -1);

  /* Sets a sprite */
  int setSprite(EditorSprite* sprite);

  /* Sets a thing in the map */
  int setThing(EditorMapThing* thing, int sub_map = -1);

  /* Sets the rendering tile icons */
  void setTileIcons(TileIcons* icons);

  /* Sets layer visibility */
  void setVisibility(EditorEnumDb::Layer layer, bool visible);

  /* Set grid and passability visibility */
  void setVisibilityEvents(bool);
  void setVisibilityGrid(bool);
  void setVisibilityPass(bool);

  /* Sets visibility of npc paths */
  void setVisibilityPaths(bool visible);

  /* Sets the visibility of all sub-maps based on reference internal tile */
  void setVisibilityRef();

  /* Thing processing for updating with the new data */
  void tilesIOAdd(bool update_all = false);
  void tilesIORemove(bool update_all = false);
  void tilesItemAdd(bool update_all = false);
  void tilesItemRemove(bool update_all = false);
  void tilesNPCAdd(bool update_all = false);
  void tilesNPCRemove(bool update_all = false);
  void tilesPersonAdd(bool update_all = false);
  void tilesPersonRemove(bool update_all = false);
  void tilesThingAdd(bool update_all = false);
  void tilesThingRemove(bool update_all = false);

  /* Update all tiles */
  void updateAll();
  void updateLays();

  /* Updates tiles related to person reference */
  void updateTiles(EditorMapPerson* ref);

  /* Unset calls for battle scene information on core */
  bool unsetBattleScene(int index);
  void unsetBattleScenes();

  /* Unset io(s) */
  bool unsetIO(int id, bool from_sub = false);
  bool unsetIOByIndex(int index, int sub_map = -1);
  void unsetIOs(bool from_sub = false);

  /* Unset item(s) */
  bool unsetItem(int id, bool from_sub = false);
  bool unsetItemByIndex(int index, int sub_map = -1);
  void unsetItems(bool from_sub = false);

  /* Unset map(s) */
  bool unsetMap(int id);
  bool unsetMapByIndex(int index);
  void unsetMaps();

  /* Unset npc(s) */
  bool unsetNPC(int id, bool from_sub = false);
  bool unsetNPCByIndex(int index, int sub_map = -1);
  void unsetNPCs(bool from_sub = false);

  /* Unset person(s) */
  bool unsetPerson(int id, bool from_sub = false);
  bool unsetPersonByIndex(int index, int sub_map = -1);
  void unsetPersons(bool from_sub = false);

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

  /* Returns the push button for the map dialog above. Will seg with others */
  static QPushButton* getDialogButton(QDialog* dialog);

  /* Dialog information for map */
  static int getDialogHeight(QDialog* dialog);
  static QString getDialogName(QDialog* dialog);
  static int getDialogWidth(QDialog* dialog);
};

#endif // EDITORMAP_H
