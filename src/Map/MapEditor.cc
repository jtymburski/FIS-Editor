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
  setBackgroundBrush(QBrush(Qt::black));
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
  setSceneRect(0,0,width*64,height*64);

  for(int i=0; i<width; i++)
  {
    QList<TileWrapper*> stack;
    for(int j=0; j<height; j++)
      stack.push_back(new TileWrapper(i,j));
    tiles.push_back(stack);
  }
  for(int i=0; i<width; i++)
  {
      for(int j=0; j<height; j++)
      {
        addItem(tiles[i][j]);
        tiles[i][j]->setToolbox(tool);
      }
  }
}

/* Destructor function */
MapEditor::~MapEditor()
{
  //qDebug()<<"Removing MapEditor";
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
void MapEditor::toggleEnhancer(bool toggle)
{
  for(int i=0; i<tiles.size(); i++)
  {
    for(int j=0; j<tiles[i].size(); j++)
    {
      tiles[i][j]->setEnhancer(toggle);
      tiles[i][j]->update();
    }
  }
}
void MapEditor::toggleLower1(bool toggle)
{
  for(int i=0; i<tiles.size(); i++)
  {
    for(int j=0; j<tiles[i].size(); j++)
    {
      tiles[i][j]->setLower1(toggle);
      tiles[i][j]->update();
    }
  }
}
void MapEditor::toggleLower2(bool toggle)
{
  for(int i=0; i<tiles.size(); i++)
  {
    for(int j=0; j<tiles[i].size(); j++)
    {
      tiles[i][j]->setLower2(toggle);
      tiles[i][j]->update();
    }
  }
}
void MapEditor::toggleLower3(bool toggle)
{
  for(int i=0; i<tiles.size(); i++)
  {
    for(int j=0; j<tiles[i].size(); j++)
    {
      tiles[i][j]->setLower3(toggle);
      tiles[i][j]->update();
    }
  }
}
void MapEditor::toggleLower4(bool toggle)
{
  for(int i=0; i<tiles.size(); i++)
  {
    for(int j=0; j<tiles[i].size(); j++)
    {
      tiles[i][j]->setLower4(toggle);
      tiles[i][j]->update();
    }
  }
}
void MapEditor::toggleLower5(bool toggle)
{
  for(int i=0; i<tiles.size(); i++)
  {
    for(int j=0; j<tiles[i].size(); j++)
    {
      tiles[i][j]->setLower5(toggle);
      tiles[i][j]->update();
    }
  }
}
void MapEditor::toggleUpper1(bool toggle)
{
  for(int i=0; i<tiles.size(); i++)
  {
    for(int j=0; j<tiles[i].size(); j++)
    {
      tiles[i][j]->setUpper1(toggle);
      tiles[i][j]->update();
    }
  }
}
void MapEditor::toggleUpper2(bool toggle)
{
  for(int i=0; i<tiles.size(); i++)
  {
    for(int j=0; j<tiles[i].size(); j++)
    {
      tiles[i][j]->setUpper2(toggle);
      tiles[i][j]->update();
    }
  }
}
void MapEditor::toggleUpper3(bool toggle)
{
  for(int i=0; i<tiles.size(); i++)
  {
    for(int j=0; j<tiles[i].size(); j++)
    {
      tiles[i][j]->setUpper3(toggle);
      tiles[i][j]->update();
    }
  }
}
void MapEditor::toggleUpper4(bool toggle)
{
  for(int i=0; i<tiles.size(); i++)
  {
    for(int j=0; j<tiles[i].size(); j++)
    {
      tiles[i][j]->setUpper4(toggle);
      tiles[i][j]->update();
    }
  }
}
void MapEditor::toggleUpper5(bool toggle)
{
  for(int i=0; i<tiles.size(); i++)
  {
    for(int j=0; j<tiles[i].size(); j++)
    {
      tiles[i][j]->setUpper5(toggle);
      tiles[i][j]->update();
    }
  }
}
void MapEditor::toggleGrid(bool toggle)
{
  for(int i=0; i<tiles.size(); i++)
  {
    for(int j=0; j<tiles[i].size(); j++)
    {
      tiles[i][j]->setGrid(toggle);
      tiles[i][j]->update();
    }
  }
}

void MapEditor::setEditingLayer(EditorEnumDb::Layer active)
{
  for(int i=0; i<tiles.size(); i++)
  {
    for(int j=0; j<tiles[i].size(); j++)
    {
      tiles[i][j]->active_layer = active;
    }
  }
}

void MapEditor::paintEvent(QPaintEvent *)
{
 // QPainter painter(this);

}


