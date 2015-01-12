/*******************************************************************************
 * Class Name: EditorPerson
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Person
 ******************************************************************************/
#include "Database/EditorPerson.h"

EditorPerson::EditorPerson(QWidget* parent) : QWidget(parent)
{
}

/* Constructor function with id and name */
EditorPerson::EditorPerson(int id, QString name, QWidget* parent)
            : EditorPerson(parent)
{
  setID(id);
  setName(name);
}

EditorPerson::~EditorPerson()
{
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

EditorPerson* EditorPerson::clone()
{
  return this;
}

/* Returns the ID of the person */
int EditorPerson::getID()
{
  return person.getGameID();
}

/* Returns the name of the person */
QString EditorPerson::getName()
{
  return QString::fromStdString(person.getName());
}

/* Returns the name of the person for listing */
QString EditorPerson::getNameList()
{
  return EditorHelpers::getListString(getID(), getName());
}

/* Sets the ID of the person */
void EditorPerson::setID(int id)
{
  person.setGameID(id);
}

/* Sets the name of the person */
void EditorPerson::setName(QString name)
{
  person.setName(name.toStdString());
}
