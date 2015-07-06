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
  middleclick_menu = NULL;
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

/* Menu adding for tile click */
void MapRender::menuSprites(EditorTile* t, QMenu* menu)
{
  /* Base */
  QAction* act_b = new QAction("B: UNSET", menu);
  act_b->setDisabled(true);
  if(t->getSprite(EditorEnumDb::BASE) != NULL)
  {
    act_b->setText("B: " + t->getSprite(EditorEnumDb::BASE)->getName());
    act_b->setEnabled(true);
  }
  connect(act_b, SIGNAL(triggered()), this, SLOT(tileSpriteB()));
  menu->addAction(act_b);

  /* Enhancer */
  QAction* act_e = new QAction("E: UNSET", menu);
  act_e->setDisabled(true);
  if(t->getSprite(EditorEnumDb::ENHANCER) != NULL)
  {
    act_e->setText("E: " + t->getSprite(EditorEnumDb::ENHANCER)->getName());
    act_e->setEnabled(true);
  }
  connect(act_e, SIGNAL(triggered()), this, SLOT(tileSpriteE()));
  menu->addAction(act_e);

  /* Lowers */
  for(int i = (int)EditorEnumDb::LOWER1; i <= (int)EditorEnumDb::LOWER5; i++)
  {
    int ref = i - EditorEnumDb::LOWER1 + 1;
    QAction* act_l = new QAction("L" + QString::number(ref) + ": UNSET", menu);
    act_l->setDisabled(true);
    if(t->getSprite((EditorEnumDb::Layer)i) != NULL)
    {
      act_l->setText("L" + QString::number(ref) + ": " +
                     t->getSprite((EditorEnumDb::Layer)i)->getName());
      act_l->setEnabled(true);
    }
    menu->addAction(act_l);
  }
  connect(menu->actions()[2], SIGNAL(triggered()), this, SLOT(tileSpriteL1()));
  connect(menu->actions()[3], SIGNAL(triggered()), this, SLOT(tileSpriteL2()));
  connect(menu->actions()[4], SIGNAL(triggered()), this, SLOT(tileSpriteL3()));
  connect(menu->actions()[5], SIGNAL(triggered()), this, SLOT(tileSpriteL4()));
  connect(menu->actions()[6], SIGNAL(triggered()), this, SLOT(tileSpriteL5()));

  /* Uppers */
  for(int i = (int)EditorEnumDb::UPPER1; i <= (int)EditorEnumDb::UPPER5; i++)
  {
    int ref = i - EditorEnumDb::UPPER1 + 1;
    QAction* act_u = new QAction("U" + QString::number(ref) + ": UNSET", menu);
    act_u->setDisabled(true);
    if(t->getSprite((EditorEnumDb::Layer)i) != NULL)
    {
      act_u->setText("U" + QString::number(ref) + ": " +
                     t->getSprite((EditorEnumDb::Layer)i)->getName());
      act_u->setEnabled(true);
    }
    menu->addAction(act_u);
  }
  connect(menu->actions()[7], SIGNAL(triggered()), this, SLOT(tileSpriteU1()));
  connect(menu->actions()[8], SIGNAL(triggered()), this, SLOT(tileSpriteU2()));
  connect(menu->actions()[9], SIGNAL(triggered()), this, SLOT(tileSpriteU3()));
  connect(menu->actions()[10], SIGNAL(triggered()), this, SLOT(tileSpriteU4()));
  connect(menu->actions()[11], SIGNAL(triggered()), this, SLOT(tileSpriteU5()));
}

/* Mouse event */
bool MapRender::mouseEvent(QGraphicsSceneMouseEvent* event)
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

    return new_hover;
  }
  return false;
}

/* Path click manipulation - for editing paths */
void MapRender::pathClickLeft(int x, int y)
{
  int index = 0;

  EditorNPCPath::HoverState state = path_edit->getHoverState(index, x, y);
  if(state == EditorNPCPath::ON_NODE)
  {
    path_edit->setIndexMove(index);
  }
  else if(state == EditorNPCPath::ON_PATH)
  {
    path_edit->insertNodeBefore(index, x, y);
    path_edit->setIndexMove(index);
  }
  else if(state == EditorNPCPath::GENERAL)
  {
    path_edit->appendNode(x, y);
    path_edit->setIndexMove(path_edit->getNodes().size() - 1);
  }
}

/* Path click manipulation - for editing paths */
// TODO: Comment
void MapRender::pathClickRight(int x, int y)
{
  QList<Path> list = path_edit->getNodes();
  bool success = false;
  for(int i = 0; !success && (i < list.size()); i++)
  {
    if(list[i].x == x && list[i].y == y)
    {
      path_edit->deleteNode(i);
      success = true;
    }
  }
}

/* Right click menu initialize */
// TODO: Comment
void MapRender::tileClickInit()
{
  /* Delete the old menu if it still exists */
  if(middleclick_menu != NULL)
  {
    delete middleclick_menu;
    middleclick_menu = NULL;
  }

  if(views().size() > 0)
  {
    EditorTile* t = editing_map->getHoverInfo()->hover_tile;

    /* Menu init */
    middleclick_menu = new QMenu("Tile", views().front());
    middleclick_menu->hide();

    /* Sprites */
    QMenu* menu_sprites = new QMenu("Sprites", middleclick_menu);
    menuSprites(t, menu_sprites);

    /* Things */
    QMenu* menu_things = new QMenu("Things", middleclick_menu);
    menu_things->addAction(new QAction("TODO", menu_things));

    /* Items */
    QMenu* menu_items = new QMenu("Items", middleclick_menu);
    menu_items->addAction(new QAction("TODO", menu_items));

    /* IOs */
    QMenu* menu_ios = new QMenu("IOs", middleclick_menu);
    menu_ios->addAction(new QAction("TODO", menu_ios));

    /* Persons */
    QMenu* menu_persons = new QMenu("Persons", middleclick_menu);
    menu_persons->addAction(new QAction("TODO", menu_persons));

    /* NPCs */
    QMenu* menu_npcs = new QMenu("NPCs", middleclick_menu);
    menu_npcs->addAction(new QAction("TODO", menu_npcs));

    /* Add sub-menus */
    middleclick_menu->addMenu(menu_sprites);
    middleclick_menu->addMenu(menu_things);
    middleclick_menu->addMenu(menu_items);
    middleclick_menu->addMenu(menu_ios);
    middleclick_menu->addMenu(menu_persons);
    middleclick_menu->addMenu(menu_npcs);

    /* Execute menu */
    middleclick_menu->exec(QCursor::pos());
  }
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/* General event processing */
// TODO: Comment
bool MapRender::event(QEvent *event)
{
  //qDebug() << event->type();
  //qDebug() << views().front()->x() << "," << views().front()->y() << ","
  //         << views().front()->width() << "," << views().front()->height();
  //qDebug() << geometry();
  //qDebug() << if(views().front()->rect().contains(views().front()->mapFromGlobal(QCursor::pos()))

  QGraphicsView* view = views().front();
  //qDebug() << "Geometry: " << view->geometry();
  //qDebug() << "Cursor: " << QCursor::pos();
  //qDebug() << "Point: " << view->mapFromGlobal(QCursor::pos());

  if(editing_map != NULL)// &&
     //!view->rect().contains(view->mapFromGlobal(QCursor::pos())))
  {
    /* If leaving the widget, nullify the hover event */
    if(event->type() == QEvent::Leave)
    {
      QPoint point = view->mapFromGlobal(QCursor::pos());
      int ref_min = 1;
      int x_ref = view->geometry().width()
                - view->verticalScrollBar()->width() - point.x();
      int y_ref = view->geometry().height()
                - view->horizontalScrollBar()->height() - point.y();

      /* Is actually leaving (for fake events) */
      if(point.x() <= ref_min || point.y() <= ref_min ||
         x_ref <= ref_min || y_ref <= ref_min)
      {
        editing_map->setHoverTile(NULL);
        emit sendCurrentPosition(-1, -1);

        /* Clean up path edit */
        if(path_edit != NULL)
        {
          path_edit->setHoverNode();
          if(path_edit->getIndexMove() >= 0)
            path_edit->unsetIndexMove(true);
        }
      }
    }
  }

  return QGraphicsScene::event(event);
}

/*
 * Description: Mouse Move Event (Handles all individual tile events, including
 *              hovering box color changes, and placement while the mouse
 *              button is pressed.)
 *
 * Inputs: QGraphicsSceneMouseEvent* event - the mouse event
 * Output: none
 */
void MapRender::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  if(editing_map != NULL)
  {
    bool new_hover = mouseEvent(event);

    /* If a new hover tile, execute the click */
    if(new_hover && path_edit == NULL && (event->buttons() & Qt::LeftButton ||
                                          event->buttons() & Qt::RightButton) &&
       editing_map->getHoverInfo()->active_cursor != EditorEnumDb::BLOCKPLACE)
    {
      editing_map->clickTrigger(false, event->buttons() & Qt::RightButton);
    }
  }

  QGraphicsScene::mouseMoveEvent(event);
}

/*
 * Description: Mouse Press event, handles placement of the selected tile
 *              onto the map
 *
 * Inputs: Mouse Event
 */
void MapRender::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  /* Do a mouse check */
  //mouseEvent(event);

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
      {
        pathClickLeft(hover_x, hover_y);
      }
      else if(event->button() == Qt::RightButton)
      {
        if(path_edit->getIndexMove() >= 0)
          path_edit->unsetIndexMove(true);
        else
          pathClickRight(hover_x, hover_y);
      }
    }
    else if(event->button() == Qt::MidButton)
    {
      tileClickInit();
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

  //QGraphicsScene::mousePressEvent(event);
}

/* Mouse release event */
void MapRender::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  /* Ensure editing map is valid */
  if(editing_map != NULL && editing_map->getHoverInfo()->hover_tile != NULL)
  {
    /* If path edit is not null, it takes priority for events */
    if(path_edit != NULL)
    {
      if(path_edit->getIndexMove() >= 0 && event->button() == Qt::LeftButton)
        path_edit->unsetIndexMove();
    }
    /* If click release, and block place, proceed */
    else if(editing_map->getHoverInfo()->active_cursor ==
                                                       EditorEnumDb::BLOCKPLACE)
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

  //QGraphicsScene::mouseReleaseEvent(event);
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

/* Tile slots */
void MapRender::tileSpriteB()
{
  qDebug() << "TODO: BASE EDIT";
}

/* Tile slots */
void MapRender::tileSpriteE()
{
  qDebug() << "TODO: ENHANCER EDIT";
}

/* Tile slots */
void MapRender::tileSpriteL1()
{
  qDebug() << "TODO: LOWER 1 EDIT";
}

/* Tile slots */
void MapRender::tileSpriteL2()
{
  qDebug() << "TODO: LOWER 2 EDIT";
}

/* Tile slots */
void MapRender::tileSpriteL3()
{
  qDebug() << "TODO: LOWER 3 EDIT";
}

/* Tile slots */
void MapRender::tileSpriteL4()
{
  qDebug() << "TODO: LOWER 4 EDIT";
}

/* Tile slots */
void MapRender::tileSpriteL5()
{
  qDebug() << "TODO: LOWER 5 EDIT";
}

/* Tile slots */
void MapRender::tileSpriteU1()
{
  qDebug() << "TODO: UPPER 1 EDIT";
}

/* Tile slots */
void MapRender::tileSpriteU2()
{
  qDebug() << "TODO: UPPER 2 EDIT";
}

/* Tile slots */
void MapRender::tileSpriteU3()
{
  qDebug() << "TODO: UPPER 3 EDIT";
}

/* Tile slots */
void MapRender::tileSpriteU4()
{
  qDebug() << "TODO: UPPER 4 EDIT";
}

/* Tile slots */
void MapRender::tileSpriteU5()
{
  qDebug() << "TODO: UPPER 5 EDIT";
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
