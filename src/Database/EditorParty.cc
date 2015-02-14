/*******************************************************************************
 * Class Name: EditorParty
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Party
 ******************************************************************************/
#include "Database/EditorParty.h"

EditorParty::EditorParty(QWidget *parent) : QWidget(parent)
{
  name = "";
}

/* Constructor function with id and name */
EditorParty::EditorParty(int id, QString name, QWidget* parent)
           : EditorParty(parent)
{
  setID(id);
  setName(name);
}

/* Copy constructor */
EditorParty::EditorParty(const EditorParty &source) : EditorParty()
{
  copySelf(source);
}

/* Destructor function */
EditorParty::~EditorParty()
{
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/* Copy function, to be called by a copy or equal operator constructor */
void EditorParty::copySelf(const EditorParty &source)
{
  name = source.name;
  party = source.party;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

EditorParty* EditorParty::clone()
{
  return this;
}

/* Returns the ID of the party */
int EditorParty::getID() const
{
  return party.getID();
}

/* Returns the name of the party */
QString EditorParty::getName() const
{
  return name;
}

/* Returns the name of the party for listing */
QString EditorParty::getNameList()
{
  return EditorHelpers::getListString(getID(), getName());
}

/* Sets the ID of the party */
void EditorParty::setID(int id)
{
  party.setID(id);
}

/* Sets the name of the party */
void EditorParty::setName(QString name)
{
  this->name = name;
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/* The copy operator */
EditorParty& EditorParty::operator= (const EditorParty &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
