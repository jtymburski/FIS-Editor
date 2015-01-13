/*******************************************************************************
 * Class Name: EditorMap
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Map
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

class EditorMap : public EditorTemplate
{
//  Q_OBJECT
public:
  /* Constructor Function */
  EditorMap();//QWidget* parent = NULL);

  /* Constructor function with id and name plus optional width and height
   * of base map */
  EditorMap(int id, QString name, int width = 0, int height = 0);

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

  /*------------------- Constants -----------------------*/
  const static int kUNSET_ID; /* The unset ID */

protected:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorMap &source);

public slots:
signals:
public:
  /* Clone */
  EditorMap* clone();

  /* Returns the ID of the map set */
  virtual int getID();

  /* Returns the stored map information */
  SubMapInfo* getMap(int id);
  int getMapCount();
  int getMapIndex(int id);
  QVector<SubMapInfo*> getMaps();

  /* Returns the name of the map set */
  virtual QString getName();
  virtual QString getNameList();

  /* Returns available IDs in the set. Useful for when creating a new one */
  int getNextMapID();
  int getNextSpriteID();

  /* Return stored sprite information */
  EditorSprite* getSprite(int id);
  int getSpriteCount();
  int getSpriteIndex(int id);
  QVector<EditorSprite*> getSprites();

  /* Sets the ID of the map set */
  virtual void setID(int id);

  /* Sets a map, based on ID */
  int setMap(int id, QString name, QVector<QVector<EditorTile*>> tiles);
  int setMap(int id, QString name, int width, int height);

  /* Sets the name of the map set */
  virtual void setName(QString name);

  /* Sets a sprite */
  int setSprite(EditorSprite* sprite);

  /* Unset map(s) */
  void unsetMap(int id);
  void unsetMaps();

  /* Unset sprite(s) */
  void unsetSprite(int id);
  void unsetSprites();

/* Operator functions */
public:
  EditorMap& operator= (const EditorMap &source);

/* Public static functions */
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
