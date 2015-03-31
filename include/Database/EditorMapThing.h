/*******************************************************************************
 * Class Name: EditorMapThing
 * Date Created: February 7, 2015
 * Inheritance: EditorTemplate
 * Description: The class for managing the interfacing with MapThing and filling
 *              it with data. The management pop-up is ThingDialog.
 ******************************************************************************/
#ifndef EDITORMAPTHING_H
#define EDITORMAPTHING_H

#include "Database/EditorMatrix.h"
#include "Database/EditorSprite.h"
#include "Database/EditorTemplate.h"
#include "Game/Map/MapThing.h"

class EditorMapThing : public EditorTemplate
{
public:
  /* Constructor function */
  EditorMapThing(int id = -1, QString name = "Default Thing",
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
  Event event;

  /* The view matrix */
  EditorMatrix* matrix;

  /* The actual thing for in game */
  MapThing thing;

  /* The rendering tile icons */
  TileIcons* tile_icons;

  /* The X/Y position of the thing */
  int x;
  int y;

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorMapThing &source, bool inc_matrix = true);

  /* Sets the matrix in the class - this is used by children to utilize the
   * printing of the thing */
  bool setMatrix(EditorMatrix* matrix, bool first_call = false);

  /* Unsets the matrix in the class - no deletion occurs */
  void unsetMatrix();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Gets the base thing of the thing */
  EditorMapThing* getBaseThing() const;

  /* Gets the description of the thing */
  QString getDescription() const;

  /* Returns the dialog image for the thing */
  EditorSprite* getDialogImage();

  /* Returns the event object of the thing */
  Event getEvent() const;

  /* Gets the ID for the thing */
  virtual int getID() const;

  /* Returns the matrix in the thing class */
  EditorMatrix* getMatrix();

  /* Gets the user submitted name of the thing */
  virtual QString getName() const;

  /* Gets the proper formatted name and ID for listing */
  virtual QString getNameList();
  QString getNameList(bool shortened);

  /* Returns the x and y coordinate of the thing */
  int getX();
  int getY();

  /* Returns if the thing is visible */
  bool isVisible() const;

  /* Loads the thing data */
  virtual void load(XmlData data, int index);

  /* Paint a sprite in the thing */
  bool paint(QPainter* painter, QRect rect, int offset_x = 0, int offset_y = 0);
  bool paint(int frame_index, QPainter* painter, QRect rect,
             int offset_x = 0, int offset_y = 0);

  /* Saves the thing data */
  virtual void save(FileHandler* fh, bool game_only = false);

  /* Sets the base reference thing */
  virtual void setBase(EditorMapThing* thing);

  /* Sets the description of the thing */
  void setDescription(QString description);

  /* Sets the dialog image, with the path */
  void setDialogImage(QString path);

  /* Sets the interaction event */
  void setEvent(Event event);

  /* Sets the thing id (Backend) */
  virtual void setID(int id);

  /* Sets the user submitted name */
  virtual void setName(QString name);

  /* Sets the rendering tile icons */
  virtual void setTileIcons(TileIcons* icons);

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
