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
MapEditor::MapEditor(QWidget* parent, int w, int h)
{
  width = w;
  height = h;
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


void MapEditor::paintEvent(QPaintEvent *)
{
  QPainter painter(this);

}
