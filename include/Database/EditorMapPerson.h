/*******************************************************************************
 * Class Name: EditorMapPerson
 * Date Created: March 3, 2015
 * Inheritance: EditorMapThing
 * Description: The class for managing the interfacing with MapPerson and 
 *              filling it with data. The management pop-up is PersonDialog.
 ******************************************************************************/
#ifndef EDITORMAPPERSON_H
#define EDITORMAPPERSON_H

#include "Database/EditorMapThing.h"

class EditorMapPerson : public EditorMapThing
{
public:
  /* Constructor function */
  EditorMapPerson(int id = -1, QString name = "Default Person", 
                  QString description = "");
                  
  /* Copy constructor function */
  EditorMapPerson(const EditorMapPerson &source);
  
  /* Destructor function */
  virtual ~EditorMapPerson();
  
private:
  /* The set of thing matrixes for different grounds and directions */
  QList<QList<EditorMatrix*>> matrix_set;
  
/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Delete defines matrixes stored in class - called once at destruction */
  void deleteMatrixes();
  
  /* Initialize matrixes stored in class - called once at start */
  void initializeMatrixes();
  
/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorMapPerson &source);
  
/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public: 
 
/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  /* The copy operator */
  EditorMapPerson& operator= (const EditorMapPerson &source);
};

#endif // EDITORMAPPERSON_H