/*******************************************************************************
 * Class Name: EditorSound
 * Date Created: September 29, 2015
 * Inheritance: QWidget
 * Description: Editor Sound storage interface
 ******************************************************************************/
#include "Database/EditorSound.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor Function */
EditorSound::EditorSound()
{
  id = 0;
  name = "New Sound";
}

/* Constructor function with id and name */
EditorSound::EditorSound(int id, QString name) : EditorSound()
{
  setID(id);
  setName(name);
}

/* Copy constructor */
EditorSound::EditorSound(const EditorSound &source)
{
  copySelf(source);
}

/* Destructor function */
EditorSound::~EditorSound()
{
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
  
/* Copy function, to be called by a copy or equal operator constructor */
void EditorSound::copySelf(const EditorSound &source)
{
  // TODO
}

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/
  
/* Button Triggers */
//void btnItemAdd();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
  
/* Returns the ID of the party */
int EditorSound::getID() const
{
  return id;
}

/* Returns the name of the party */
QString EditorSound::getName() const
{
  return name;
}

/* Returns the name of the party for listing */
QString EditorSound::getNameList()
{
  return EditorHelpers::getListString(getID(), getName());
}

/* Loads the object data */
void EditorSound::load(XmlData data, int index)
{
  // TODO
}

/* Resets the working set trigger */
//void EditorSound::resetWorking()
//{
//}

/* Saves the object data */
void EditorSound::save(FileHandler* fh, bool game_only)
{
  // TODO
}

/* Saves the working set trigger */
//void EditorSound::saveWorking()
//{
//}

/* Sets the ID of the party */
void EditorSound::setID(int id)
{
  this->id = id;
}

/* Sets the name of the party */
void EditorSound::setName(QString name)
{
  this->name = name;
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
  
/* The copy operator */
EditorSound& EditorSound::operator= (const EditorSound &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
