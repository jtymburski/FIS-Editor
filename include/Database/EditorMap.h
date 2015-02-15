/*******************************************************************************
 * Class Name: EditorMap
 * Date Created: December 27, 2014
 * Inheritance: EditorTemplate
 * Description: The map interface to connect and edit in the editor
 ******************************************************************************/
#ifndef EDITORMAP_H
#define EDITORMAP_H

#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QVector>
#include <QWidget>

#include "Database/EditorSprite.h"
#include "Database/EditorTemplate.h"
#include "Database/EditorThing.h"
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
};

class EditorMap : public EditorTemplate
{
//  Q_OBJECT
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
  /* The map set ID */
  int id;

  /* The name of the map set */
  QString name;

  /* The map sprites */
  QVector<EditorSprite*> sprites;

  /* The set of sub-maps */
  QVector<SubMapInfo*> sub_maps;

  /* The map things */
  QVector<EditorThing*> things;

  /* Rendering tile icons */
  TileIcons* tile_icons;

  /*------------------- Constants -----------------------*/
  const static int kUNSET_ID; /* The unset ID */

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Adds tile sprite data */
  void addTileSpriteData(FileHandler* fh, int index, EditorEnumDb::Layer layer);
  void addTileSpriteData(FileHandler* fh, SubMapInfo* map,
                         EditorEnumDb::Layer layer);

  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorMap &source);

  /* Loads sub-map info */
  void loadSubMap(SubMapInfo* map, XmlData data, int index);

  /* Saves the sub-map */
  void saveSubMap(FileHandler* fh, bool game_only,
                  SubMapInfo* map, bool first = false);

signals:
public slots:

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the active layers in a string */
  QString getActiveLayers(int map_index, int x, int y);

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
  int getNextThingID();

  /* Return stored sprite information */
  EditorSprite* getSprite(int id);
  EditorSprite* getSpriteByIndex(int index);
  int getSpriteCount();
  int getSpriteIndex(int id);
  QVector<EditorSprite*> getSprites();

  /* Return stored thing information */
  EditorThing* getThing(int id);
  EditorThing* getThingByIndex(int index);
  int getThingCount();
  int getThingIndex(int id);
  QVector<QString> getThingList();
  QVector<EditorThing*> getThings();

  /* Returns the tile icons */
  TileIcons* getTileIcons();

  /* Loads the map */
  void load(XmlData data, int index);

  /* Saves the map */
  void save(FileHandler* fh, bool game_only = false, int sub_index = -1);

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
  int setThing(EditorThing* thing);

  /* Sets the rendering tile icons */
  void setTileIcons(TileIcons* icons);

  /* Sets layer visibility */
  void setVisibility(EditorEnumDb::Layer layer, bool visible);

  /* Set grid and passability visibility */
  void setVisibilityGrid(bool);
  void setVisibilityPass(bool);

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
  bool unsetThing(int id);
  bool unsetThingByIndex(int index);
  void unsetThings();

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
