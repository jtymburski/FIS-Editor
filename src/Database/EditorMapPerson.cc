/*******************************************************************************
 * Class Name: EditorMapPerson
 * Date Created: March 3, 2015
 * Inheritance: EditorMapThing
 * Description: The class for managing the interfacing with MapPerson and 
 *              filling it with data. The management pop-up is PersonDialog.
 ******************************************************************************/
#include "Database/EditorMapPerson.h"
#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/
 
/* Constructor function */
EditorMapPerson::EditorMapPerson(int id, QString name, QString description)
               : EditorMapThing(id, name, description)
{
  initializeMatrixes();
}
                
/* Copy constructor function */
EditorMapPerson::EditorMapPerson(const EditorMapPerson &source)
               : EditorMapPerson()
{
  copySelf(source);
}
  
/* Destructor function */
EditorMapPerson::~EditorMapPerson()
{
  deleteMatrixes();
}
  
/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* Delete defines matrixes stored in class - called once at destruction */
void EditorMapPerson::deleteMatrixes()
{
  // TODO
}
  
/* Initialize matrixes stored in class - called once at start */
void EditorMapPerson::initializeMatrixes()
{
  // TODO
}
  
/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/* Copy function, to be called by a copy or equal operator constructor */
void EditorMapPerson::copySelf(const EditorMapPerson &source)
{
  EditorMapThing::copySelf(source, false);
  
  // TODO: COPY PERSON DATA
}
  
/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/* The copy operator */
EditorMapPerson& EditorMapPerson::operator= (const EditorMapPerson &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
