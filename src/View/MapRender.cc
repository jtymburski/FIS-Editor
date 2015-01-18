/*******************************************************************************
 * Class Name: MapRender
 * Date Created: November 11, 2013
 * Inheritance: QGLWidget
 * Description: The map editor widget that gives a snapshot view of the window
 *              to make changes to the map from.
 ******************************************************************************/
#include "View/MapRender.h"

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
MapRender::MapRender(QWidget* parent,
                     EditorEnumDb::CursorMode cursor)
         : QGraphicsScene(parent)
{
  /* Data init */
  active_tile = NULL;
  setCursorMode(cursor);
  map = NULL;

  /* Sets the background to be black */
  setBackgroundBrush(QBrush(Qt::black));
}

/*
 * Description: Destructor function
 */
MapRender::~MapRender()
{
  /* Clear out the rendering map */
  setRenderingMap(NULL);
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: Mouse Move Event (Handles all individual tile events, including
 *              hovering box color changes, and placement while the mouse
 *              button is pressed.)
 *
 * Inputs: Mouse Event
 */
void MapRender::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  bool new_hover = false;

  /* Check if left the current tile */
  if(active_tile != NULL)
  {
    QRectF bound = active_tile->boundingRect();
    if(!bound.contains(event->scenePos()))
    {
      active_tile->setHover(false);
      active_tile = NULL;
    }
  }

  /* Check which tile it's hovering on now */
  if(active_tile == NULL)
  {
    QGraphicsItem* hover_item = itemAt(event->scenePos(), QTransform());
    if(hover_item != NULL)
    {
      active_tile = (EditorTile*)hover_item;
      active_tile->setHover(true);
      new_hover = true;
    }

    emit sendCurrentPosition(active_tile->getX(), active_tile->getY());
  }

  /* If a new hover tile, check if a button is pressed and trigger item click */
  if(new_hover && (event->buttons() & Qt::LeftButton ||
                   event->buttons() & Qt::RightButton))
  {
    if(cursor_mode == EditorEnumDb::BASIC ||
       cursor_mode == EditorEnumDb::ERASER)
    {
      emit itemClick(active_tile);
    }
    else if(cursor_mode == EditorEnumDb::PASS_ALL ||
             cursor_mode == EditorEnumDb::PASS_N ||
             cursor_mode == EditorEnumDb::PASS_E ||
             cursor_mode == EditorEnumDb::PASS_S ||
             cursor_mode == EditorEnumDb::PASS_W)
    {
      if(event->buttons() & Qt::LeftButton)
        emit passSet(active_tile);
      else
        emit passUnset(active_tile);
    }
  }
}

/*
 * Description: Mouse Press event, handles placement of the selected tile
 *              onto the map
 *
 * Inputs: Mouse Event
 */
void MapRender::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  /* If click, proceed to trigger item click for normal pens */
  if(event->button() == Qt::LeftButton)
  {
    if(cursor_mode == EditorEnumDb::BASIC ||
       cursor_mode == EditorEnumDb::ERASER ||
       cursor_mode == EditorEnumDb::FILL)
      emit itemClick(active_tile);
    else if(cursor_mode == EditorEnumDb::BLOCKPLACE)
    {
      block_origin = event->scenePos().toPoint();
      block_erase = false;
    }
    else if(cursor_mode == EditorEnumDb::PASS_ALL ||
             cursor_mode == EditorEnumDb::PASS_N ||
             cursor_mode == EditorEnumDb::PASS_E ||
             cursor_mode == EditorEnumDb::PASS_S ||
             cursor_mode == EditorEnumDb::PASS_W)
    {
      emit passSet(active_tile);
    }
  }
  else if(event->button() == Qt::RightButton)
  {
    if(cursor_mode == EditorEnumDb::BLOCKPLACE)
    {
      block_origin = event->scenePos();
      block_erase = true;
    }
    else if(cursor_mode == EditorEnumDb::PASS_ALL ||
             cursor_mode == EditorEnumDb::PASS_N ||
             cursor_mode == EditorEnumDb::PASS_E ||
             cursor_mode == EditorEnumDb::PASS_S ||
             cursor_mode == EditorEnumDb::PASS_W)
    {
      emit passUnset(active_tile);
    }
  }
}

/* Mouse release event */
void MapRender::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  /* If click release, and block place, proceed */
  if(cursor_mode == EditorEnumDb::BLOCKPLACE)
  {
    if((event->button() == Qt::LeftButton && !block_erase) ||
       (event->button() == Qt::RightButton && block_erase))
    {
      QRectF rect = EditorHelpers::normalizePoints(block_origin,
                                                   event->scenePos());
      QList<QGraphicsItem*> item_set =
                                    items(rect, Qt::IntersectsItemBoundingRect);
      QList<EditorTile*> tile_set;
      for(int i = 0; i < item_set.size(); i++)
        tile_set.push_back((EditorTile*)item_set[i]);
      emit itemMassClick(tile_set, block_erase);
    }
  }
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

/* Sets the rendering sub-map */
void MapRender::setRenderingMap(SubMapInfo* map)
{
  /* Remove services from active tile */
  if(active_tile != NULL)
  {
    active_tile->setHover(false);
    active_tile = NULL;
  }

  /* Clean up the existing list */
  QList<QGraphicsItem*> existing_items = items();
  for(int i = 0; i < existing_items.size(); i++)
    removeItem(existing_items[i]);
  setSceneRect(0, 0, 0, 0);

  /* Set the new map */
  this->map = map;

  /* Add in the new map info */
  if(map != NULL)
  {
    /* Add tiles */
    for(int i = 0; i < map->tiles.size(); i++)
      for(int j = 0; j < map->tiles[i].size(); j++)
        addItem(map->tiles[i][j]);

    /* Set the size of the map scene */
    if(map->tiles.size() > 0)
      setSceneRect(0, 0, map->tiles.size() * EditorHelpers::getTileSize(),
                   map->tiles.front().size() * EditorHelpers::getTileSize());
  }
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Returns the active tile */
EditorTile* MapRender::getActiveTile()
{
  return active_tile;
}

/*
 * Description: Gets the height
 *
 * Output: Height
 */
int MapRender::getMapHeight()
{
  if(map != NULL && map->tiles.size() > 0)
    return map->tiles.front().size();
  return 0;
}

/*
 * Description: Gets the width
 *
 * Output: Width
 */
int MapRender::getMapWidth()
{
  if(map != NULL)
    return map->tiles.size();
  return 0;
}

/* Returns the rendering sub-map */
SubMapInfo* MapRender::getRenderingMap()
{
  return map;
}

/*
 * Description: Sets the cursor mode
 *
 * Input: Cursor mode
 */
void MapRender::setCursorMode(EditorEnumDb::CursorMode mode)
{
  cursor_mode = mode;
}

/* Update the entire scene */
void MapRender::updateAll()
{
  update(sceneRect());
}
