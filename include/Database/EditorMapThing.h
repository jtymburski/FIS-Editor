/*******************************************************************************
 * Class Name: EditorMapThing
 * Date Created: February 7, 2015
 * Inheritance: EditorTemplate
 * Description: The class for managing the interfacing with MapThing and filling
 *              it with data. The management pop-up is ThingDialog.
 ******************************************************************************/
#ifndef EDITORMAPTHING_H
#define EDITORMAPTHING_H

#include "Database/EditorEventSet.h"
#include "Database/EditorMatrix.h"
#include "Database/EditorSprite.h"
#include "Database/EditorTemplate.h"
#include "Map/MapObjectType.h"
#include "Map/MapThing.h"
#include "Persistence/XmlData.h"

class EditorMapThing : public EditorTemplate
{
public:
  /* Constructor function */
  EditorMapThing(int id = 0, QString name = "Default Name",
                 QString description = "");

  /* Copy constructor */
  EditorMapThing(const EditorMapThing &source);

  /* Destructor function */
  virtual ~EditorMapThing();

private:
  /* A base thing object */
  EditorMapThing* base;

  /* The frame displayed in a dialog when interacting with the thing */
  EditorSprite dialog_image;

  /* The event */
  EditorEventSet set;
  bool event_base = true;

  /* The view matrix */
  EditorMatrix* matrix;

  /* The actual thing for in game */
  core::MapThing thing;

protected:
  /* The rendering tile icons */
  TileIcons* tile_icons;

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorMapThing &source, bool inc_matrix = true);

  /* Saves the thing data - virtualized */
  virtual void saveData(core::XmlWriter* writer, bool game_only = false,
                        bool inc_matrix = true);

  /* Sets the matrix in the class - this is used by children to utilize the
   * printing of the thing */
  bool setMatrix(EditorMatrix* matrix, bool first_call = false);

  /* Unsets the matrix in the class - no deletion occurs */
  void unsetMatrix();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the active respawn active time delay of the thing */
  int getActiveRespawn() const;

  /* Gets the base thing of the thing */
  EditorMapThing* getBaseThing() const;

  /* Gets the thing classification */
  virtual core::MapObjectType getClass() const;

  /* Gets the description of the thing */
  QString getDescription() const;

  /* Returns the dialog image for the thing */
  EditorSprite* getDialogImage();

  /* Returns the event set of the thing */
  EditorEventSet* getEventSet();

  /* Returns the game object reference ID. -1 if unset */
  int getGameID() const;

  /* Gets the ID for the thing */
  int getID() const override;

  /* Returns the matrix in the thing class */
  virtual EditorMatrix* getMatrix() const;

  /* Gets the user submitted name of the thing */
  QString getName() const override;

  /* Gets the proper formatted name and ID for listing */
  QString getNameList() override;
  QString getNameList(bool shortened);

  /* Returns the sound reference ID */
  int getSoundID() const;

  /* Returns the x and y coordinate of the thing */
  int getX();
  int getY();

  /* Checks if the thing is set to be active */
  bool isActive() const;

  /* Returns if the tile sprites in all matrixes at that x, y are null */
  virtual bool isAllNull(int x, int y) const;

  /* Returns if the thing is using the base event */
  bool isBaseEvent() const;

  /* Returns if the thing is visible */
  bool isVisible() const;

  /* Loads the thing data */
  virtual void load(core::XmlData data, int index);

  /* Paint a sprite in the thing */
  bool paint(QPainter* painter, QRect rect, int offset_x = 0, int offset_y = 0);
  bool paint(int frame_index, QPainter* painter, QRect rect,
             int offset_x = 0, int offset_y = 0);

  /* Saves the thing data */
  virtual void save(core::XmlWriter* writer, bool game_only = false);

  /* Sets if the thing is active */
  bool setActive(bool active);
  void setActiveRespawn(int time);

  /* Sets the base reference thing */
  void setBase(EditorMapThing* thing, bool synchronize = true);

  /* Sets the description of the thing */
  void setDescription(QString description);

  /* Sets the dialog image, with the path */
  void setDialogImage(QString path);

  /* Sets the interaction event set */
  void setEventSet(EditorEventSet set);

  /* Sets the game object reference ID. -1 if unset */
  void setGameID(int id);

  /* Sets the thing id (Backend) */
  void setID(int id) override;

  /* Sets the user submitted name */
  void setName(QString name, bool update = true) override;

  /* Sets the connected sound ID */
  void setSoundID(int id);

  /* Sets the rendering tile icons */
  virtual void setTileIcons(TileIcons* icons);

  /* Sets the class to use the base event, if relevant */
  void setUseBaseEvent(bool use_base);

  /* Sets the visibility of the thing */
  void setVisibility(bool visible);

  /* Sets the x and y coordinate of the thing */
  virtual bool setX(int x);
  virtual bool setY(int y);

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  /* The copy operator */
  EditorMapThing& operator= (const EditorMapThing &source);
};

#endif // EDITORMAPTHING_H
