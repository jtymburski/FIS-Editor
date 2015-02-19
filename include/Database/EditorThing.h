/*******************************************************************************
 * Class Name: EditorThing
 * Date Created: February 7, 2015
 * Inheritance: EditorTemplate
 * Description: The class for managing the interfacing with MapThing and filling
 *              it with data. The management pop-up is ThingDialog.
 ******************************************************************************/
#ifndef EDITORTHING_H
#define EDITORTHING_H

#include "Database/EditorMatrix.h"
#include "Database/EditorSprite.h"
#include "Database/EditorTemplate.h"
#include "Game/Map/MapThing.h"

class EditorThing : public EditorTemplate
{
public:
  /* Constructor function */
  EditorThing(int id = -1, QString name = "Default Thing",
              QString description = "");

  /* Copy constructor */
  EditorThing(const EditorThing &source);

  /* Destructor function */
  virtual ~EditorThing();

private:
  /* A base thing object */
  EditorThing* base;

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
  void copySelf(const EditorThing &source);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Gets the base thing of the thing */
  EditorThing* getBase() const;

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

  /* Returns the x and y coordinate of the thing */
  int getX();
  int getY();

  /* Returns if the thing is visible */
  bool isVisible() const;

  /* Paint the thing */
  // TODO: QPainter* painter, int offset_x, int offset_y, QRect bound??
  //bool paint(QPainter* painter, QRect rect);
  //bool paint(QPainter* painter, int x, int y, int w, int h);
  //bool paint(int index, QPainter* painter, QRect rect);
  //bool paint(int index, QPainter* painter, int x, int y, int w, int h);

  /* Sets the base reference thing */
  void setBase(EditorThing* thing);

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
  void setTileIcons(TileIcons* icons);

  /* Sets the visibility of the thing */
  void setVisibility(bool visible);

  /* Sets the x and y coordinate of the thing */
  bool setX(int x);
  bool setY(int y);

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  /* The copy operator */
  EditorThing& operator= (const EditorThing &source);
};

#endif // EDITORTHING_H
