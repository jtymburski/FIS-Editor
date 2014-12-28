/*******************************************************************************
 * Class Name: EditorPerson
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Person
 ******************************************************************************/
#include "Database/EditorPerson.h"

EditorPerson::EditorPerson(QWidget *parent) : QWidget(parent)
{
}

EditorPerson::~EditorPerson()
{
}

EditorPerson* EditorPerson::clone()
{
  return this;
}


