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

EditorBubby::~EditorBubby()
{
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

EditorBubby* EditorBubby::clone()
{
  return this;
}

/* Returns the ID of the bubby */
int EditorBubby::getID()
{
  return bubby.getGameID();
}

/* Returns the name of the bubby */
QString EditorBubby::getName()
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
