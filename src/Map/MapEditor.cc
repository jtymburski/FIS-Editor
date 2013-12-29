/*******************************************************************************
 * Class Name: MapEditor
 * Date Created: November 11, 2013
 * Inheritance: QGLWidget
 * Description: The map editor widget that gives a snapshot view of the window
 *              to make changes to the map from.
 ******************************************************************************/
#include "Map/MapEditor.h"
#include <QDebug>
/* Constant Implementation - see header file for descriptions */
//const int Map::kELEMENT_DATA = 0;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
MapEditor::MapEditor(EditorSpriteToolbox* tool, QWidget* parent, int w, int h)
{
  width = w;
  height = h;
  base = true;
  enhancer = true;
  lower1 = true;
  lower2 = true;
  lower3 = true;
  lower4 = true;
  lower5 = true;
  upper1 = true;
  upper2 = true;
  upper3 = true;
  upper4 = true;
  upper5 = true;
  setFixedSize(width*64,height*64);
  for(int i=0; i<width; i++)
  {
    QList<TileWrapper*> stack;
    for(int j=0; j<height; j++)
      stack.push_back(new TileWrapper(this));
    tiles.push_back(stack);
  }
  for(int i=0; i<width; i++)
  {
      for(int j=0; j<height; j++)
      {
        tiles[i][j]->move(i*64,j*64);
        tiles[i][j]->setToolbox(tool);
        //qDebug()<<i<<","<<j;
        //tiles[i][j]->update();
      }
  }
}

/* Destructor function */
MapEditor::~MapEditor()
{
  for(int i=0; i<width; i++)
  {
    for(int j=0; j<height; j++)
    {
      delete tiles[i][j];
      tiles[i][j] = NULL;
    }
  }
}

void MapEditor::toggleBase(bool toggle)
{
  for(int i=0; i<tiles.size(); i++)
  {
    for(int j=0; j<tiles[i].size(); j++)
    {
      tiles[i][j]->setBase(toggle);
      tiles[i][j]->update();
    }
  }
}

void MapEditor::paintEvent(QPaintEvent *)
{
  QPainter painter(this);

}

