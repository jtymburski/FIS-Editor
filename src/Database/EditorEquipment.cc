/*******************************************************************************
 * Class Name: EditorEquipment
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Equipment for the game.
 ******************************************************************************/
#include "Database/EditorEquipment.h"

EditorEquipment::EditorEquipment(QWidget *parent) : QWidget(parent)
{
}

/* Constructor function with id and name */
EditorEquipment::EditorEquipment(int id, QString name, QWidget* parent)
               : EditorEquipment(parent)
{
  setID(id);
  setName(name);
}

EditorEquipment::~EditorEquipment()
{
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

EditorEquipment* EditorEquipment::clone()
{
  return this;
}

/* Returns the ID of the equipment */
int EditorEquipment::getID()
{
  return equipment.getGameID();
}

/* Returns the name of the equipment */
QString EditorEquipment::getName()
{
  return QString::fromStdString(equipment.getName());
}

/* Returns the name of the equipment for listing */
QString EditorEquipment::getNameList()
{
  return EditorHelpers::getListString(getID(), getName());
}

/* Sets the ID of the equipment */
void EditorEquipment::setID(int id)
{
  equipment.setGameID(id);
}

/* Sets the name of the equipment */
void EditorEquipment::setName(QString name)
{
  equipment.setName(name.toStdString());
}
