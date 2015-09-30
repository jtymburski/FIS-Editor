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
EditorSound::EditorSound(QWidget* parent) : QWidget(parent)
{
  /* Base data */
  id = 0;
  name_base = "Blank";
  name_curr = name_base;

  /* Layout */
  createLayout();
  loadWorkingInfo();
}

/* Constructor function with id and name */
EditorSound::EditorSound(int id, QString name, QWidget* parent)
           : EditorSound(parent)
{
  setID(id);
  setName(name);
  saveWorking();
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

/* Creates interface layout */
void EditorSound::createLayout()
{
  // TODO
}

/* Loads working info into UI objects */
void EditorSound::loadWorkingInfo()
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
  return name_curr;
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
void EditorSound::resetWorking()
{
  // TODO
}

/* Saves the object data */
void EditorSound::save(FileHandler* fh, bool game_only)
{
  // TODO
}

/* Saves the working set trigger */
void EditorSound::saveWorking()
{
  // TODO
}

/* Sets the ID of the party */
void EditorSound::setID(int id)
{
  this->id = id;
  //edit_id->setText(QString::number(id));
}

/* Sets the name of the party */
void EditorSound::setName(QString name)
{
  name_curr = name;
  //edit_name->setText(name);
  emit nameChange(name);
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
