/*******************************************************************************
 * Class Name: EditorItem
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Item
 ******************************************************************************/
#include "Database/EditorItem.h"

EditorItem::EditorItem(QWidget *parent) : QWidget(parent)
{
}

/* Constructor function with id and name */
EditorItem::EditorItem(int id, QString name, QWidget* parent)
          : EditorItem(parent)
{
  setID(id);
  setName(name);
}

/* Copy constructor */
EditorItem::EditorItem(const EditorItem &source) : EditorItem()
{
  copySelf(source);
}

EditorItem::~EditorItem()
{
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/* Copy function, to be called by a copy or equal operator constructor */
void EditorItem::copySelf(const EditorItem &source)
{
  item = source.item;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

EditorItem* EditorItem::clone()
{
  return this;
}

/* Returns the ID of the item */
int EditorItem::getID()
{
  return item.getGameID();
}

/* Returns the name of the item */
QString EditorItem::getName()
{
  return QString::fromStdString(item.getName());
}

/* Returns the name of the item for listing */
QString EditorItem::getNameList()
{
  return EditorHelpers::getListString(getID(), getName());
}

/* Sets the ID of the item */
void EditorItem::setID(int id)
{
  item.setGameID(id);
}

/* Sets the name of the item */
void EditorItem::setName(QString name)
{
  item.setName(name.toStdString());
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/* The copy operator */
EditorItem& EditorItem::operator= (const EditorItem &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
