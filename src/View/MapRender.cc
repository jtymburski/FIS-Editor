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
MapRender::MapRender(QWidget* parent)
         : QGraphicsScene(parent)
{
  /* Data init */
  editing_map = NULL;
  path_edit = NULL;
  tile_select = false;

  /* Sets the background to be black */
  setBackgroundBrush(QBrush(Qt::black));
}

/*
 * Description: Destructor function
 */
MapRender::~MapRender()
{
  /* Clear out the rendering map */
  setMapEditor(NULL);
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* Path click manipulation - for editing paths */
void MapRender::pathClickLeft(int x, int y)
{
  int index = 0;
  EditorNPCPath::HoverState state = path_edit->getHoverState(index, x, y);
  if(path_edit->getIndexMove() >= 1)
  {
    if(state == EditorNPCPath::ON_PATH || state == EditorNPCPath::GENERAL)
      path_edit->updateIndexMove(x, y);
  }
  else if(state == EditorNPCPath::ON_NODE)
  {
    path_edit->setIndexMove(index);
  }
  else if(state == EditorNPCPath::ON_PATH)
  {
    path_edit->insertNodeBefore(index, x, y);
  }
  else if(state == EditorNPCPath::GENERAL)
  {
    path_edit->appendNode(x, y);
  }
}

/* Path click manipulation - for editing paths */
// TODO: Comment
void MapRender::pathClickRight(int x, int y)
{
  QList<Path> list = path_edit->getNodes();
  bool success = false;
  for(int i = 1; !success && (i < list.size()); i++)
  {
    if(list[i].x == x && list[i].y == y)
    {
      path_edit->deleteNode(i);
      success = true;
    }
  }
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/* General event processing */
// TODO: Comment
bool MapRender::event(QEvent *event)
{
  /* If leaving the widget, nullify the hover event */
  if(event->type() == QEvent::Leave && editing_map != NULL)
  {
    editing_map->setHoverTile(NULL);
    if(path_edit != NULL)
      path_edit->setHoverNode();
  }

  return QGraphicsScene::event(event);
}

/*
 * Description: Mouse Move Event (Handles all individual tile events, including
 *              hovering box color changes, and placement while the mouse
 *              button is pressed.)
 *
 * Inputs: Mouse Event
 */
void MapRender::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  if(editing_map != NULL)
  {
    bool new_hover = false;
    EditorTile* active_tile = editing_map->getHoverInfo()->hover_tile;

    /* Check if left the current tile */
    if(active_tile != NULL)
    {
      QRectF bound = active_tile->boundingRect();
      if(!bound.contains(event->scenePos()))
      {
        editing_map->setHoverTile(NULL);
        active_tile = NULL;

        /* Remove hover trigger to path edit */
        if(path_edit != NULL)
          path_edit->setHoverNode();
      }
    }

    /* Check which tile it's hovering on now */
    if(active_tile == NULL)
    {
      QGraphicsItem* hover_item = itemAt(event->scenePos(), QTransform());
      if(hover_item != NULL && hover_item->zValue() == 0)
      {
        active_tile = (EditorTile*)hover_item;
        editing_map->setHoverTile(active_tile);
        new_hover = true;
        emit sendCurrentPosition(active_tile->getX(), active_tile->getY());

        /* New hover trigger to path edit */
        if(path_edit != NULL)
          path_edit->setHoverNode(active_tile->getX(), active_tile->getY());
      }
      else
      {
        emit sendCurrentPosition(-1, -1);
      }
    }

    /* If a new hover tile, execute the click */
    if(new_hover && path_edit == NULL && (event->buttons() & Qt::LeftButton ||
                                          event->buttons() & Qt::RightButton) &&
       editing_map->getHoverInfo()->active_cursor != EditorEnumDb::BLOCKPLACE)
    {
      editing_map->clickTrigger(false, event->buttons() & Qt::RightButton);
    }

    QGraphicsScene::mouseMoveEvent(event);
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
  /* Ensure editing map is valid */
  if(editing_map != NULL && editing_map->getHoverInfo()->hover_tile != NULL)
  {
    /* If click, proceed to trigger item click */
    if(tile_select)
    {
      emit sendSelectedTile(editing_map->getCurrentMap()->id,
                            editing_map->getHoverInfo()->hover_tile->getX(),
                            editing_map->getHoverInfo()->hover_tile->getY());
      tile_select = false;
    }
    else if(path_edit != NULL)
    {
      int hover_x = editing_map->getHoverInfo()->hover_tile->getX();
      int hover_y = editing_map->getHoverInfo()->hover_tile->getY();

      if(event->button() == Qt::LeftButton)
        pathClickLeft(hover_x, hover_y);
      else if(event->button() == Qt::RightButton)
        pathClickRight(hover_x, hover_y);
    }
    else if(event->button() == Qt::LeftButton)
    {
      if(editing_map->getHoverInfo()->active_cursor == EditorEnumDb::BLOCKPLACE)
      {
        block_origin = event->scenePos().toPoint();
        block_erase = false;
      }
      else
      {
        editing_map->clickTrigger();
      }
    }
    else if(event->button() == Qt::RightButton)
    {
      if(editing_map->getHoverInfo()->active_cursor == EditorEnumDb::BLOCKPLACE)
      {
        block_origin = event->scenePos().toPoint();
        block_erase = true;
      }
      else
      {
        editing_map->clickTrigger(true, true);
      }
    }
  }
}

/* Mouse release event */
void MapRender::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  /* Ensure editing map is valid */
  if(editing_map != NULL && editing_map->getHoverInfo()->hover_tile != NULL)
  {
    /* If click release, and block place, proceed */
    if(editing_map->getHoverInfo()->active_cursor == EditorEnumDb::BLOCKPLACE)
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
        editing_map->clickTrigger(tile_set, block_erase);
      }
    }
  }
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

/* NPC Path Add/Remove control */
void MapRender::npcPathAdd(EditorNPCPath* path)
{
  addItem(path);
}

/* NPC Path Add/Remove control */
void MapRender::npcPathRemove(EditorNPCPath* path)
{
  removeItem(path);
}

/* Path edit trigger */
void MapRender::pathEditTrigger(EditorNPCPath* path)
{
  if(editing_map != NULL)
  {
    /* Start the path edit */
    if(path_edit == NULL && path != NULL)
    {
      path_edit = path;
      addItem(path_edit);
      editing_map->setPathsEnabled(false);
    }
    /* Stop the path edit */
    else if(path_edit != NULL && path == NULL)
    {
      removeItem(path_edit);
      path_edit = NULL;
      editing_map->setPathsEnabled(true);
    }
  }
}

/* Select a tile trigger */
void MapRender::selectTile()
{
  tile_select = true;
}

/* Update the rendering sub-map */
void MapRender::updateRenderingMap()
{
  /* Clean up the existing list */
  QList<QGraphicsItem*> existing_items = items();
  for(int i = 0; i < existing_items.size(); i++)
    removeItem(existing_items[i]);
  setSceneRect(0, 0, 0, 0);

  /* Add in the new map info */
  if(editing_map != NULL && editing_map->getCurrentMap() != NULL)
  {
    SubMapInfo* map = editing_map->getCurrentMap();

    /* Add tiles */
    for(int i = 0; i < map->tiles.size(); i++)
      for(int j = 0; j < map->tiles[i].size(); j++)
        addItem(map->tiles[i][j]);

    /* Add npc paths */
    for(int i = 0; i < map->npcs.size(); i++)
      addItem(map->npcs[i]->getPath());

    /* Set the size of the map scene */
    if(map->tiles.size() > 0)
      setSceneRect(0, 0, map->tiles.size() * EditorHelpers::getTileSize(),
                   map->tiles.front().size() * EditorHelpers::getTileSize());
  }
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Returns the map being edited */
EditorMap* MapRender::getMapEditor()
{
  return editing_map;
}

/*
 * Description: Gets the height
 *
 * Output: Height
 */
int MapRender::getMapHeight()
{
  if(editing_map != NULL && editing_map->getCurrentMap() != NULL &&
     editing_map->getCurrentMap()->tiles.size() > 0)
    return editing_map->getCurrentMap()->tiles.front().size();
  return 0;
}

/*
 * Description: Gets the width
 *
 * Output: Width
 */
int MapRender::getMapWidth()
{
  if(editing_map != NULL && editing_map->getCurrentMap() != NULL)
    return editing_map->getCurrentMap()->tiles.size();
  return 0;
}

/* Sets the map being edited */
void MapRender::setMapEditor(EditorMap* editor)
{
  /* Signal disconnection */
  if(editing_map != NULL)
  {
    disconnect(editing_map, SIGNAL(npcPathAdd(EditorNPCPath*)),
               this, SLOT(npcPathAdd(EditorNPCPath*)));
    disconnect(editing_map, SIGNAL(npcPathRemove(EditorNPCPath*)),
               this, SLOT(npcPathRemove(EditorNPCPath*)));
  }

  /* Set the map */
  editing_map = editor;

  /* Update the rendering map */
  updateRenderingMap();

  /* Signal re-connection */
  if(editing_map != NULL)
  {
    connect(editing_map, SIGNAL(npcPathAdd(EditorNPCPath*)),
            this, SLOT(npcPathAdd(EditorNPCPath*)));
    connect(editing_map, SIGNAL(npcPathRemove(EditorNPCPath*)),
            this, SLOT(npcPathRemove(EditorNPCPath*)));
  }
}

/* Update the entire scene */
void MapRender::updateAll()
{
  update(sceneRect());
}
