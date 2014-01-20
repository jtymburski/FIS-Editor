/*******************************************************************************
 * Class Name: MapEditor
 * Date Created: November 11, 2013
 * Inheritance: QGLWidget
 * Description: The map editor widget that gives a snapshot view of the window
 *              to make changes to the map from.
 ******************************************************************************/
#include "Map/MapEditor.h"
#include <QDebug>
#include <QGraphicsView>
/* Constant Implementation - see header file for descriptions */
//const int Map::kELEMENT_DATA = 0;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function
 *
 * Input: a pointer to the Editor Sprite Toolbox,
 *        the parent widget, and the dimensions of the map
 */
MapEditor::MapEditor(EditorSpriteToolbox* tool, QWidget* parent,
                     int w, int h, EditorEnumDb::CursorMode cursor)
{

  remove_action = new QAction("&Remove Active Layer",0);
  rightclick_menu = new QMenu();
  rightclick_menu->hide();
  rightclick_menu->addAction(remove_action);
  connect(remove_action,SIGNAL(triggered()),this,SLOT(removeCurrent()));

  /* Sets the background to be black */
  setBackgroundBrush(QBrush(Qt::black));
  setCursorMode(cursor);

  /* Sets the width and height, and all of the layers to be visible */
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

  /* Sets the size of the map scene */
  setSceneRect(0,0,width*64,height*64);

  /* Sets up a blank canvas of tiles on the map */
  for(int i=0; i<width; i++)
  {
    QList<TileWrapper*> stack;
    for(int j=0; j<height; j++)
      stack.push_back(new TileWrapper(i,j));
    tiles.push_back(stack);
  }
  /* Adds each tile to the map view and sets its toolbox pointer */
  for(int i=0; i<width; i++)
  {
      for(int j=0; j<height; j++)
      {
        addItem(tiles[i][j]);
        tiles[i][j]->setToolbox(tool);
      }
  }
}

/*
 * Description: Destructor function
 */
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

/*
 * Description: Toggles the base layers for each tile
 *
 * Inputs: Visiblity boolean
 */
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

/*
 * Description: Toggles the enhancer layers for each tile
 *
 * Inputs: Visiblity boolean
 */
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

/*
 * Description: Toggles the lower layers for each tile
 *
 * Inputs: Visiblity boolean
 */
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

/*
 * Description: Toggles the lower layers for each tile
 *
 * Inputs: Visiblity boolean
 */
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

/*
 * Description: Toggles the lower layers for each tile
 *
 * Inputs: Visiblity boolean
 */
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

/*
 * Description: Toggles the lower layers for each tile
 *
 * Inputs: Visiblity boolean
 */
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

/*
 * Description: Toggles the lower layers for each tile
 *
 * Inputs: Visiblity boolean
 */
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

/*
 * Description: Toggles the upper layers for each tile
 *
 * Inputs: Visiblity boolean
 */
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

/*
 * Description: Toggles the upper layers for each tile
 *
 * Inputs: Visiblity boolean
 */
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

/*
 * Description: Toggles the upper layers for each tile
 *
 * Inputs: Visiblity boolean
 */
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

/*
 * Description: Toggles the upper layers for each tile
 *
 * Inputs: Visiblity boolean
 */
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

/*
 * Description: Toggles the upper layers for each tile
 *
 * Inputs: Visiblity boolean
 */
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

/*
 * Description: Toggles the grid for each tile
 *
 * Inputs: Visiblity boolean
 */
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


/*
 * Description: Sets the layer that is currently being edited for each tile
 *
 * Inputs: Layer choice
 */
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

/*
 * Description: Mouse Move Event (Handles all individual tile events, including
 *              hovering box color changes, and placement while the mouse
 *              button is pressed.)
 *
 * Inputs: Mouse Event
 */
void MapEditor::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  TileWrapper* current =
      qgraphicsitem_cast<TileWrapper*>
      (itemAt(event->scenePos(),QTransform()));
  for(int i=0; i<tiles.size(); i++)
  {
    for(int j=0; j<tiles[i].size(); j++)
      tiles[i][j]->setGridColor(false);
  }
  if(current != NULL)
    emit sendCurrentPosition((current->boundingRect().x()/64),
                             (current->boundingRect().y()/64));
  else
    emit sendCurrentPosition(-1,-1);
  if(current != NULL)
    current->setGridColor(true);
  if(event->buttons() == Qt::LeftButton)
  {
    switch(cursormode)
    {
      case EditorEnumDb::BASIC:
        if(current != NULL)
          current->place();
        break;
      case EditorEnumDb::ERASER:
        if(current != NULL)
          current->unplace();
        break;
      default:
        break;
    }
  }
}

/*
 * Description: Mouse Press event, handles placement of the selected tile
 *              onto the map
 *
 * Inputs: Mouse Event
 */
void MapEditor::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  current_position = event->scenePos();
  if(event->buttons() & Qt::LeftButton)
  {
    TileWrapper* current =
        qgraphicsitem_cast<TileWrapper*>
        (itemAt(current_position,QTransform()));
    switch(cursormode)
    {
      case EditorEnumDb::BASIC:
        if(current != NULL)
          current->place();
        break;
      case EditorEnumDb::ERASER:
        if(current != NULL)
          current->unplace();
        break;
      default:
        break;
    }
  }
  else
  {
    if(rightclick_menu->isHidden())
      rightclick_menu->exec(QCursor::pos());
    else
      rightclick_menu->hide();
  }
}


void MapEditor::paintEvent(QPaintEvent *)
{}

/*
 * Description: Removes the currently active layers sprite
 */
void MapEditor::removeCurrent()
{
  TileWrapper* current =
      qgraphicsitem_cast<TileWrapper*>
      (itemAt(current_position,QTransform()));
  if(current != NULL)
    current->unplace();
}

/*
 * Description: Sets the cursor mode
 *
 * Input: Cursor mode
 */
void MapEditor::setCursorMode(EditorEnumDb::CursorMode mode)
{
  cursormode = mode;
}

/*
 * Description: Gets the width
 *
 * Output: Width
 */
int MapEditor::getMapWidth()
{
  return width;
}

/*
 * Description: Gets the height
 *
 * Output: Height
 */
int MapEditor::getMapHeight()
{
  return height;
}
