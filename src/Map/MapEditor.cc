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
  : QGraphicsScene(parent)
{

  remove_action = new QAction("&Remove Active Layer",0);
  setNpass_action = new QAction("&North Passibility",0);
  setEpass_action = new QAction("&East Passibility",0);
  setSpass_action = new QAction("&South Passibility",0);
  setWpass_action = new QAction("&West Passibility",0);
  setApass_action = new QAction("&All Passibility",0);
  setNpass_action->setCheckable(true);
  setEpass_action->setCheckable(true);
  setSpass_action->setCheckable(true);
  setWpass_action->setCheckable(true);
  setApass_action->setCheckable(true);

  rightclick_menu = new QMenu();
  rightclick_menu->hide();
  rightclick_menu->addAction(remove_action);
  rightclick_menu->addSeparator();
  rightclick_menu->addAction(setNpass_action);
  rightclick_menu->addAction(setEpass_action);
  rightclick_menu->addAction(setSpass_action);
  rightclick_menu->addAction(setWpass_action);
  rightclick_menu->addSeparator();
  rightclick_menu->addAction(setApass_action);
  connect(remove_action,SIGNAL(triggered()),this,SLOT(removeCurrent()));

  connect(setNpass_action,SIGNAL(triggered(bool)),
          this,SLOT(setNPassCurrent(bool)));
  connect(setEpass_action,SIGNAL(triggered(bool)),
          this,SLOT(setEPassCurrent(bool)));
  connect(setSpass_action,SIGNAL(triggered(bool)),
          this,SLOT(setSPassCurrent(bool)));
  connect(setWpass_action,SIGNAL(triggered(bool)),
          this,SLOT(setWPassCurrent(bool)));
  connect(setApass_action,SIGNAL(triggered(bool)),
          this,SLOT(setAPassCurrent(bool)));

  /* Sets the background to be black */
  setBackgroundBrush(QBrush(Qt::black));
  setCursorMode(cursor);

  /* Sets the width and height, and all of the layers to be visible */
  width = w;
  height = h;
  blockx = 0;
  blocky = 0;
  blockmodepress = false;
  eraseblock = false;
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

  highlight = new QRubberBand(QRubberBand::Rectangle,0);

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
 * Description: Toggles the passibility view for each tile
 *
 * Inputs: Visiblity boolean
 */
void MapEditor::togglePass(bool toggle)
{
  for(int i=0; i<tiles.size(); i++)
  {
    for(int j=0; j<tiles[i].size(); j++)
    {
      tiles[i][j]->setPass(toggle);
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
  if(event->buttons() == Qt::LeftButton || event->buttons() == Qt::RightButton)
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
      case EditorEnumDb::BLOCKPLACE:
        if(blockmodepress)
          highlight->setGeometry(QRect(origin.x(),origin.y(),
                                       QCursor::pos().x()-origin.x(),
                                       QCursor::pos().y()-origin.y())
                                 .normalized());
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
  origin = QCursor::pos();
  TileWrapper* current =
      qgraphicsitem_cast<TileWrapper*>
      (itemAt(current_position,QTransform()));
  if(event->buttons() & Qt::LeftButton)
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
      case EditorEnumDb::BLOCKPLACE:
        if(current != NULL)
        {
          blockmodepress = true;
          blockx = current->boundingRect().x()/64;
          blocky = current->boundingRect().y()/64;
          highlight->setGeometry(QRect(origin.x(),origin.y(),0,0));
          highlight->show();
        }
        break;
      default:
        break;
    }
  }
  else
  {
    if(cursormode == EditorEnumDb::BLOCKPLACE)
    {
      blockmodepress = true;
      eraseblock = true;
      blockx = current->boundingRect().x()/64;
      blocky = current->boundingRect().y()/64;
      highlight->setGeometry(QRect(origin.x(),origin.y(),0,0));
      highlight->show();
    }
    else if(rightclick_menu->isHidden())
    {
      setNpass_action->setChecked(
            current->gameTile()->getBasePassability(Direction::NORTH));
      setEpass_action->setChecked(
            current->gameTile()->getBasePassability(Direction::EAST));
      setSpass_action->setChecked(
            current->gameTile()->getBasePassability(Direction::SOUTH));
      setWpass_action->setChecked(
            current->gameTile()->getBasePassability(Direction::WEST));
      if(current->gameTile()->getBasePassability(Direction::NORTH) &&
         current->gameTile()->getBasePassability(Direction::EAST) &&
         current->gameTile()->getBasePassability(Direction::SOUTH) &&
         current->gameTile()->getBasePassability(Direction::WEST))
        setApass_action->setChecked(true);
      else
        setApass_action->setChecked(false);
      rightclick_menu->exec(QCursor::pos());
    }
    else
      rightclick_menu->hide();
  }
}

void MapEditor::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  TileWrapper* current =
      qgraphicsitem_cast<TileWrapper*>
      (itemAt(event->scenePos(),QTransform()));

  if(cursormode == EditorEnumDb::BLOCKPLACE)
  {
    highlight->hide();
    int currentx = current->boundingRect().x()/64;
    int currenty = current->boundingRect().y()/64;

    if(!eraseblock)
    {
      if(blockx <= currentx && blocky <= currenty)
      {
        for(int i=0; i<abs(currentx-blockx)+1; i++)
        {
          for(int j=0; j<abs(currenty-blocky)+1; j++)
            tiles[i+blockx][j+blocky]->place();
        }
      }
      else if(blockx <= currentx && blocky > currenty)
      {
        for(int i=0; i<abs(currentx-blockx)+1; i++)
        {
          for(int j=0; j<abs(currenty-blocky)+1; j++)
            tiles[i+blockx][j+currenty]->place();
        }
      }
      else if(blockx > currentx && blocky <= currenty)
      {
        for(int i=0; i<abs(currentx-blockx)+1; i++)
        {
          for(int j=0; j<abs(currenty-blocky)+1; j++)
            tiles[i+currentx][j+blocky]->place();
        }
      }
      else if(blockx > currentx && blocky > currenty)
      {
        for(int i=0; i<abs(currentx-blockx)+1; i++)
        {
          for(int j=0; j<abs(currenty-blocky)+1; j++)
            tiles[i+currentx][j+currenty]->place();
        }
      }
    }
    else
    {
      if(blockx <= currentx && blocky <= currenty)
      {
        for(int i=0; i<abs(currentx-blockx)+1; i++)
        {
          for(int j=0; j<abs(currenty-blocky)+1; j++)
            tiles[i+blockx][j+blocky]->unplace();
        }
      }
      else if(blockx <= currentx && blocky > currenty)
      {
        for(int i=0; i<abs(currentx-blockx)+1; i++)
        {
          for(int j=0; j<abs(currenty-blocky)+1; j++)
            tiles[i+blockx][j+currenty]->unplace();
        }
      }
      else if(blockx > currentx && blocky <= currenty)
      {
        for(int i=0; i<abs(currentx-blockx)+1; i++)
        {
          for(int j=0; j<abs(currenty-blocky)+1; j++)
            tiles[i+currentx][j+blocky]->unplace();
        }
      }
      else if(blockx > currentx && blocky > currenty)
      {
        for(int i=0; i<abs(currentx-blockx)+1; i++)
        {
          for(int j=0; j<abs(currenty-blocky)+1; j++)
            tiles[i+currentx][j+currenty]->unplace();
        }
      }
    }
  }
  blockx = 0;
  blocky = 0;
  blockmodepress = false;
  eraseblock = false;
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

/*
 * Description : Sets passibility
 *
 * Input : toggle
 */
void MapEditor::setNPassCurrent(bool toggle)
{
  TileWrapper* current =
      qgraphicsitem_cast<TileWrapper*>
      (itemAt(current_position,QTransform()));
  if(current != NULL)
  {
    current->gameTile()->setBasePassability(Direction::NORTH,toggle);
    current->update();
  }
}

/*
 * Description : Sets passibility
 *
 * Input : toggle
 */
void MapEditor::setEPassCurrent(bool toggle)
{
  TileWrapper* current =
      qgraphicsitem_cast<TileWrapper*>
      (itemAt(current_position,QTransform()));
  if(current != NULL)
  {
    current->gameTile()->setBasePassability(Direction::EAST,toggle);
    current->update();
  }
}

/*
 * Description : Sets passibility
 *
 * Input : toggle
 */
void MapEditor::setSPassCurrent(bool toggle)
{
  TileWrapper* current =
      qgraphicsitem_cast<TileWrapper*>
      (itemAt(current_position,QTransform()));
  if(current != NULL)
  {
    current->gameTile()->setBasePassability(Direction::SOUTH,toggle);
    current->update();
  }
}

/*
 * Description : Sets passibility
 *
 * Input : toggle
 */
void MapEditor::setWPassCurrent(bool toggle)
{
  TileWrapper* current =
      qgraphicsitem_cast<TileWrapper*>
      (itemAt(current_position,QTransform()));
  if(current != NULL)
  {
    current->gameTile()->setBasePassability(Direction::WEST,toggle);
    current->update();
  }
}

/*
 * Description : Sets passibility
 *
 * Input : toggle
 */
void MapEditor::setAPassCurrent(bool toggle)
{
  TileWrapper* current =
      qgraphicsitem_cast<TileWrapper*>
      (itemAt(current_position,QTransform()));
  if(current != NULL)
  {
    current->gameTile()->setBasePassability(Direction::NORTH,toggle);
    current->gameTile()->setBasePassability(Direction::EAST,toggle);
    current->gameTile()->setBasePassability(Direction::SOUTH,toggle);
    current->gameTile()->setBasePassability(Direction::WEST,toggle);
    current->update();
  }
}
