/*******************************************************************************
 * Class Name: MapEditor
 * Date Created: November 11, 2013
 * Inheritance: QGLWidget
 * Description: The map editor widget that gives a snapshot view of the window
 *              to make changes to the map from.
 ******************************************************************************/
#include "Map/MapEditor.h"

/* Constant Implementation - see header file for descriptions */
//const int Map::kELEMENT_DATA = 0;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
MapEditor::MapEditor() : QGLWidget()
{
  spritepath = "Sup";
}

/* Destructor function */
MapEditor::~MapEditor()
{
}

void MapEditor::setPath(QString sprite)
{
  spritepath = sprite;
}

void MapEditor::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  qglColor(QColor(255,255,255,128));
  renderText(0,0,0,spritepath);
}
