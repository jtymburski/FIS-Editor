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

/* Copy constructor */
EditorEquipment::EditorEquipment(const EditorEquipment &source)
               : EditorEquipment()
{
  copySelf(source);
}

EditorEquipment::~EditorEquipment()
{
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/* Copy function, to be called by a copy or equal operator constructor */
void EditorEquipment::copySelf(const EditorEquipment &source)
{
  equipment = source.equipment;
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

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/* The copy operator */
EditorEquipment& EditorEquipment::operator= (const EditorEquipment &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
