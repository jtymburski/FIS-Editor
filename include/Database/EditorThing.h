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
#include "Database/EditorTemplate.h"
#include "Game/Map/MapThing.h"

class EditorThing : public EditorTemplate
{
public:
  /* Constructor function */
  EditorThing(int id = -1, QString name = "", QString description = "");

  /* Copy constructor */
  EditorThing(const EditorThing &source);

  /* Destructor function */
  virtual ~EditorThing();

private:
  /* The view matrix */
  EditorMatrix* matrix;

  /* The actual thing for in game */
  MapThing thing;

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
  /* Gets the description of the thing */
  QString getDescription();

  /* Gets the ID for the thing */
  virtual int getID();

  /* Returns the matrix in the thing class */
  EditorMatrix* getMatrix();

  /* Gets the user submitted name of the thing */
  virtual QString getName();

  /* Gets the proper formatted name and ID for listing */
  virtual QString getNameList();

  /* Sets the description of the thing */
  void setDescription(QString description);

  /* Sets the thing id (Backend) */
  virtual void setID(int id);

  /* Sets the user submitted name */
  virtual void setName(QString name);

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  /* The copy operator */
  EditorThing& operator= (const EditorThing &source);
};

#endif // EDITORTHING_H
