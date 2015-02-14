/*******************************************************************************
 * Class Name: EditorBubby
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Bubby inherited from item display and modifier.
 ******************************************************************************/
#include "Database/EditorBubby.h"

EditorBubby::EditorBubby(QWidget *parent) : QWidget(parent)
{
}

/* Constructor function with id and name */
EditorBubby::EditorBubby(int id, QString name, QWidget* parent)
           : EditorBubby(parent)
{
  setID(id);
  setName(name);
}

/* Copy constructor */
EditorBubby::EditorBubby(const EditorBubby &source) : EditorBubby()
{
  copySelf(source);
}

EditorBubby::~EditorBubby()
{
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/* Copy function, to be called by a copy or equal operator constructor */
void EditorBubby::copySelf(const EditorBubby &source)
{
  bubby = source.bubby;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

EditorBubby* EditorBubby::clone()
{
  return this;
}

/* Returns the ID of the bubby */
int EditorBubby::getID() const
{
  return bubby.getGameID();
}

/* Returns the name of the bubby */
QString EditorBubby::getName() const
{
  return QString::fromStdString(bubby.getName());
}

/* Returns the name of the bubby for listing */
QString EditorBubby::getNameList()
{
  return EditorHelpers::getListString(getID(), getName());
}

/* Sets the ID of the bubby */
void EditorBubby::setID(int id)
{
  bubby.setGameID(id);
}

/* Sets the name of the bubby */
void EditorBubby::setName(QString name)
{
  bubby.setName(name.toStdString());
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/* The copy operator */
EditorBubby& EditorBubby::operator= (const EditorBubby &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
