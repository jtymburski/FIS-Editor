/*******************************************************************************
 * Class Name: EditorMap
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Map
 ******************************************************************************/
#include "Database/EditorMap.h"

EditorMap::EditorMap(QWidget *parent) : QWidget(parent)
{
}

EditorMap::~EditorMap()
{
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

EditorMap* EditorMap::clone()
{
  return this;
}
