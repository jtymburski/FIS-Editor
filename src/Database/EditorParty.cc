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

EditorParty::~EditorParty()
{
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

EditorParty* EditorParty::clone()
{
  return this;
}

/* Returns the ID of the party */
int EditorParty::getID()
{
  return party.getID();
}

/* Returns the name of the party */
QString EditorParty::getName()
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
