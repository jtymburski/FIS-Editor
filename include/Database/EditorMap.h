/*******************************************************************************
 * Class Name: EditorMap
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Map
 ******************************************************************************/
#ifndef EDITORMAP_H
#define EDITORMAP_H

#include <QString>
#include <QVector>

#include "Database/EditorSprite.h"
#include "Database/EditorTile.h"
#include "EditorEnumDb.h"
#include "EditorHelpers.h"

/* Struct for sub map info storage */
struct SubMapInfo
{
  int id;
  QString name;
  QVector<QVector<EditorTile*>> tiles;
};

class EditorMap// : public QWidget
{
//  Q_OBJECT
public:
  /* Constructor Function */
  EditorMap();//QWidget* parent = NULL);

  /* Destructor function */
  ~EditorMap();

private:
  /* The map set ID */
  int id;

  /* The name of the map set */
  QString name;

  /* The map sprites */
  QVector<EditorSprite*> sprites;

  /* The set of sub-maps */
  QVector<SubMapInfo*> sub_maps;

  /*------------------- Constants -----------------------*/
  const static int kUNSET_ID; /* The unset ID */

protected:
public slots:
signals:
public:
  /* Clone */
  EditorMap* clone();

  /* Returns the ID of the map set */
  int getID();

  /* Returns the stored map information */
  SubMapInfo* getMap(int id);
  int getMapCount();
  int getMapIndex(int id);
  QVector<SubMapInfo*> getMaps();

  /* Returns the name of the map set */
  QString getName();

  /* Returns available IDs in the set. Useful for when creating a new one */
  int getNextMapID();
  int getNextSpriteID();

  /* Return stored sprite information */
  EditorSprite* getSprite(int id);
  int getSpriteCount();
  int getSpriteIndex(int id);
  QVector<EditorSprite*> getSprites();

  /* Sets the ID of the map set */
  void setID(int id);

  /* Sets a map, based on ID */
  int setMap(int id, QString name, QVector<QVector<EditorTile*>> tiles);
  int setMap(int id, QString name, int width, int height);

  /* Sets the name of the map set */
  void setName(QString name);

  /* Sets a sprite */
  int setSprite(EditorSprite* sprite);

  /* Unset map(s) */
  void unsetMap(int id);
  void unsetMaps();

  /* Unset sprite(s) */
  void unsetSprite(int id);
  void unsetSprites();
};

#endif // EDITORMAP_H
