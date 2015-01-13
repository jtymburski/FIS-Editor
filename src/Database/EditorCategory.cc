/*******************************************************************************
 * Class Name: EditorCategory
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: The category that defines persons and what they are capable of.
 ******************************************************************************/
#include "Database/EditorCategory.h"

EditorCategory::EditorCategory(QWidget *parent) : QWidget(parent)
{
}

/* Constructor function with id and name */
EditorCategory::EditorCategory(int id, QString name, QWidget* parent)
              : EditorCategory()
{
  setID(id);
  setName(name);
}

/* Copy constructor */
EditorCategory::EditorCategory(const EditorCategory &source) : EditorCategory()
{
  copySelf(source);
}

EditorCategory::~EditorCategory()
{
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/* Copy function, to be called by a copy or equal operator constructor */
void EditorCategory::copySelf(const EditorCategory &source)
{
  category = source.category;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

EditorCategory* EditorCategory::clone()
{
  return this;
}

/* Returns the ID of the category */
int EditorCategory::getID()
{
  return category.getID();
}

/* Returns the name of the category */
QString EditorCategory::getName()
{
  return QString::fromStdString(category.getName());
}

/* Returns the name of the category for listing */
QString EditorCategory::getNameList()
{
  return EditorHelpers::getListString(getID(), getName());
}

/* Sets the ID of the category */
void EditorCategory::setID(int id)
{
  category.setID(id);
}

/* Sets the name of the category */
void EditorCategory::setName(QString name)
{
  category.setName(name.toStdString());
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/* The copy operator */
EditorCategory& EditorCategory::operator= (const EditorCategory &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
