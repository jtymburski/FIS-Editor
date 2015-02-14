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

/* Copy constructor */
EditorPerson::EditorPerson(const EditorPerson &source) : EditorPerson()
{
  copySelf(source);
}

EditorPerson::~EditorPerson()
{
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/* Copy function, to be called by a copy or equal operator constructor */
void EditorPerson::copySelf(const EditorPerson &source)
{
  person = source.person;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

EditorPerson* EditorPerson::clone()
{
  return this;
}

/* Returns the ID of the person */
int EditorPerson::getID() const
{
  return person.getGameID();
}

/* Returns the name of the person */
QString EditorPerson::getName() const
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

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/* The copy operator */
EditorPerson& EditorPerson::operator= (const EditorPerson &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
