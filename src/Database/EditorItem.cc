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

EditorItem::~EditorItem()
{
}

EditorItem* EditorItem::clone()
{
  return this;
}
