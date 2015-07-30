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
bool MapRender::menuIOs(EditorTile* t, QMenu* menu)
{
  bool set = false;
  QVector<EditorMapIO*> stack = t->getIOs();

  /* Go through the thing stack */
  for(int i = 0; i < stack.size(); i++)
  {
    QAction* action = new QAction(QString::number(i) + ": ", menu);
    if(stack[i] != NULL)
    {
      action->setText(action->text() + stack[i]->getName());
      set = true;
    }
    else
    {
      action->setText(action->text() + "UNSET");
      action->setDisabled(true);
    }

    menu->addAction(action);
  }

  if(menu->actions().size() >= 10)
  {
    connect(menu->actions()[0], SIGNAL(triggered()), this, SLOT(tileIO0()));
    connect(menu->actions()[1], SIGNAL(triggered()), this, SLOT(tileIO1()));
    connect(menu->actions()[2], SIGNAL(triggered()), this, SLOT(tileIO2()));
    connect(menu->actions()[3], SIGNAL(triggered()), this, SLOT(tileIO3()));
    connect(menu->actions()[4], SIGNAL(triggered()), this, SLOT(tileIO4()));
    connect(menu->actions()[5], SIGNAL(triggered()), this, SLOT(tileIO5()));
    connect(menu->actions()[6], SIGNAL(triggered()), this, SLOT(tileIO6()));
    connect(menu->actions()[7], SIGNAL(triggered()), this, SLOT(tileIO7()));
    connect(menu->actions()[8], SIGNAL(triggered()), this, SLOT(tileIO8()));
    connect(menu->actions()[9], SIGNAL(triggered()), this, SLOT(tileIO9()));
  }

  return set;
}

/* Menu adding for tile click */
bool MapRender::menuItems(EditorTile* t, QMenu* menu)
{
  bool set = false;
  QVector<EditorMapItem*> stack = t->getItems();

  /* Go through the item stack */
  for(int i = 0; i < EditorTile::kMAX_ITEMS; i++)
  {
    QAction* action = new QAction(QString::number(i) + ": ", menu);
    if(stack.size() > i && stack[i] != NULL)
    {
      action->setText(action->text() + stack[i]->getName());
      set = true;
    }
    else
    {
      action->setText(action->text() + "UNSET");
      action->setDisabled(true);
    }

    menu->addAction(action);
  }

  if(menu->actions().size() >= EditorTile::kMAX_ITEMS)
  {
    connect(menu->actions()[0], SIGNAL(triggered()), this, SLOT(tileItem0()));
    connect(menu->actions()[1], SIGNAL(triggered()), this, SLOT(tileItem1()));
    connect(menu->actions()[2], SIGNAL(triggered()), this, SLOT(tileItem2()));
    connect(menu->actions()[3], SIGNAL(triggered()), this, SLOT(tileItem3()));
    connect(menu->actions()[4], SIGNAL(triggered()), this, SLOT(tileItem4()));
    connect(menu->actions()[5], SIGNAL(triggered()), this, SLOT(tileItem5()));
    connect(menu->actions()[6], SIGNAL(triggered()), this, SLOT(tileItem6()));
    connect(menu->actions()[7], SIGNAL(triggered()), this, SLOT(tileItem7()));
    connect(menu->actions()[8], SIGNAL(triggered()), this, SLOT(tileItem8()));
    connect(menu->actions()[9], SIGNAL(triggered()), this, SLOT(tileItem9()));
  }

  return set;
}

/* Menu adding for tile click */
bool MapRender::menuNPCs(EditorTile* t, QMenu* menu)
{
  bool set = false;
  QVector<EditorMapNPC*> stack = t->getNPCs();

  /* Go through the npc stack */
  for(int i = 0; i < stack.size(); i++)
  {
    QAction* action = new QAction(QString::number(i) + ": ", menu);
    if(stack[i] != NULL)
    {
      action->setText(action->text() + stack[i]->getName());
      set = true;
    }
    else
    {
      action->setText(action->text() + "UNSET");
      action->setDisabled(true);
    }

    menu->addAction(action);
  }

  if(menu->actions().size() >= 10)
  {
    connect(menu->actions()[0], SIGNAL(triggered()), this, SLOT(tileNPC0()));
    connect(menu->actions()[1], SIGNAL(triggered()), this, SLOT(tileNPC1()));
    connect(menu->actions()[2], SIGNAL(triggered()), this, SLOT(tileNPC2()));
    connect(menu->actions()[3], SIGNAL(triggered()), this, SLOT(tileNPC3()));
    connect(menu->actions()[4], SIGNAL(triggered()), this, SLOT(tileNPC4()));
    connect(menu->actions()[5], SIGNAL(triggered()), this, SLOT(tileNPC5()));
    connect(menu->actions()[6], SIGNAL(triggered()), this, SLOT(tileNPC6()));
    connect(menu->actions()[7], SIGNAL(triggered()), this, SLOT(tileNPC7()));
    connect(menu->actions()[8], SIGNAL(triggered()), this, SLOT(tileNPC8()));
    connect(menu->actions()[9], SIGNAL(triggered()), this, SLOT(tileNPC9()));
  }

  return set;
}

/* Menu adding for tile click */
bool MapRender::menuPersons(EditorTile* t, QMenu* menu)
{
  bool set = false;
  QVector<EditorMapPerson*> stack = t->getPersons();

  /* Go through the person stack */
  for(int i = 0; i < stack.size(); i++)
  {
    QAction* action = new QAction(QString::number(i) + ": ", menu);
    if(stack[i] != NULL)
    {
      action->setText(action->text() + stack[i]->getName());
      set = true;
    }
    else
    {
      action->setText(action->text() + "UNSET");
      action->setDisabled(true);
    }

    menu->addAction(action);
  }

  if(menu->actions().size() >= 10)
  {
    connect(menu->actions()[0], SIGNAL(triggered()), this, SLOT(tilePerson0()));
    connect(menu->actions()[1], SIGNAL(triggered()), this, SLOT(tilePerson1()));
    connect(menu->actions()[2], SIGNAL(triggered()), this, SLOT(tilePerson2()));
    connect(menu->actions()[3], SIGNAL(triggered()), this, SLOT(tilePerson3()));
    connect(menu->actions()[4], SIGNAL(triggered()), this, SLOT(tilePerson4()));
    connect(menu->actions()[5], SIGNAL(triggered()), this, SLOT(tilePerson5()));
    connect(menu->actions()[6], SIGNAL(triggered()), this, SLOT(tilePerson6()));
    connect(menu->actions()[7], SIGNAL(triggered()), this, SLOT(tilePerson7()));
    connect(menu->actions()[8], SIGNAL(triggered()), this, SLOT(tilePerson8()));
    connect(menu->actions()[9], SIGNAL(triggered()), this, SLOT(tilePerson9()));
  }

  return set;
}

/* Menu adding for tile click */
bool MapRender::menuSprites(EditorTile* t, QMenu* menu)
{
  bool set = false;

  /* Base */
  QAction* act_b = new QAction("B: UNSET", menu);
  act_b->setDisabled(true);
  if(t->getSprite(EditorEnumDb::BASE) != NULL)
  {
    act_b->setText("B: " + t->getSprite(EditorEnumDb::BASE)->getName());
    act_b->setEnabled(true);
    set = true;
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
    set = true;
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
      set = true;
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
      set = true;
    }
    menu->addAction(act_u);
  }
  connect(menu->actions()[7], SIGNAL(triggered()), this, SLOT(tileSpriteU1()));
  connect(menu->actions()[8], SIGNAL(triggered()), this, SLOT(tileSpriteU2()));
  connect(menu->actions()[9], SIGNAL(triggered()), this, SLOT(tileSpriteU3()));
  connect(menu->actions()[10], SIGNAL(triggered()), this, SLOT(tileSpriteU4()));
  connect(menu->actions()[11], SIGNAL(triggered()), this, SLOT(tileSpriteU5()));

  return set;
}

/* Menu adding for tile click */
bool MapRender::menuThings(EditorTile* t, QMenu* menu)
{
  bool set = false;
  QVector<EditorMapThing*> stack = t->getThings();

  /* Go through the thing stack */
  for(int i = 0; i < stack.size(); i++)
  {
    QAction* action = new QAction(QString::number(i) + ": ", menu);
    if(stack[i] != NULL)
    {
      action->setText(action->text() + stack[i]->getName());
      set = true;
    }
    else
    {
      action->setText(action->text() + "UNSET");
      action->setDisabled(true);
    }

    menu->addAction(action);
  }

  if(menu->actions().size() >= 10)
  {
    connect(menu->actions()[0], SIGNAL(triggered()), this, SLOT(tileThing0()));
    connect(menu->actions()[1], SIGNAL(triggered()), this, SLOT(tileThing1()));
    connect(menu->actions()[2], SIGNAL(triggered()), this, SLOT(tileThing2()));
    connect(menu->actions()[3], SIGNAL(triggered()), this, SLOT(tileThing3()));
    connect(menu->actions()[4], SIGNAL(triggered()), this, SLOT(tileThing4()));
    connect(menu->actions()[5], SIGNAL(triggered()), this, SLOT(tileThing5()));
    connect(menu->actions()[6], SIGNAL(triggered()), this, SLOT(tileThing6()));
    connect(menu->actions()[7], SIGNAL(triggered()), this, SLOT(tileThing7()));
    connect(menu->actions()[8], SIGNAL(triggered()), this, SLOT(tileThing8()));
    connect(menu->actions()[9], SIGNAL(triggered()), this, SLOT(tileThing9()));
  }

  return set;
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
    QFont font_bold = menu_sprites->font();
    font_bold.setBold(true);
    if(menuSprites(t, menu_sprites))
      menu_sprites->menuAction()->setFont(font_bold);

    /* Things */
    QMenu* menu_things = new QMenu("Things", middleclick_menu);
    if(menuThings(t, menu_things))
      menu_things->menuAction()->setFont(font_bold);

    /* Items */
    QMenu* menu_items = new QMenu("Items", middleclick_menu);
    if(menuItems(t, menu_items))
      menu_items->menuAction()->setFont(font_bold);

    /* IOs */
    QMenu* menu_ios = new QMenu("IOs", middleclick_menu);
    if(menuIOs(t, menu_ios))
      menu_ios->menuAction()->setFont(font_bold);

    /* Persons */
    QMenu* menu_persons = new QMenu("Persons", middleclick_menu);
    if(menuPersons(t, menu_persons))
      menu_persons->menuAction()->setFont(font_bold);

    /* NPCs */
    QMenu* menu_npcs = new QMenu("NPCs", middleclick_menu);
    if(menuNPCs(t, menu_npcs))
      menu_npcs->menuAction()->setFont(font_bold);

    /* Tile events */
    QAction* action_enter = new QAction("Enter Event", middleclick_menu);
    if(editing_map->getHoverInfo()->hover_tile->isEventEnterSet())
      action_enter->setFont(font_bold);
    connect(action_enter, SIGNAL(triggered()), this, SIGNAL(tileEventEnter()));
    QAction* action_exit = new QAction("Exit Event", middleclick_menu);
    if(editing_map->getHoverInfo()->hover_tile->isEventExitSet())
      action_exit->setFont(font_bold);
    connect(action_exit, SIGNAL(triggered()), this, SIGNAL(tileEventExit()));

    /* Add sub-menus */
    middleclick_menu->addMenu(menu_sprites);
    middleclick_menu->addMenu(menu_things);
    middleclick_menu->addMenu(menu_items);
    middleclick_menu->addMenu(menu_ios);
    middleclick_menu->addMenu(menu_persons);
    middleclick_menu->addMenu(menu_npcs);
    middleclick_menu->addSeparator();
    middleclick_menu->addAction(action_enter);
    middleclick_menu->addAction(action_exit);

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
      /* Ctrl + Click select layered thing */
      if(QGuiApplication::keyboardModifiers() == Qt::ControlModifier)
      {
        editing_map->ctrlClickTrigger();
      }
      /* Block place */
      else if(editing_map->getHoverInfo()->active_cursor ==
                                                       EditorEnumDb::BLOCKPLACE)
      {
        block_origin = event->scenePos().toPoint();
        block_erase = false;
      }
      /* All other placement */
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
        {
          if(item_set[i]->boundingRect().width() == 64 &&
             item_set[i]->boundingRect().height() == 64)
          {
            tile_set.push_back((EditorTile*)item_set[i]);
          }
        }
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

/* ------ Tile IO slots ------ */
void MapRender::tileIO0() { emit tileIO(0); }
void MapRender::tileIO1() { emit tileIO(1); }
void MapRender::tileIO2() { emit tileIO(2); }
void MapRender::tileIO3() { emit tileIO(3); }
void MapRender::tileIO4() { emit tileIO(4); }
void MapRender::tileIO5() { emit tileIO(5); }
void MapRender::tileIO6() { emit tileIO(6); }
void MapRender::tileIO7() { emit tileIO(7); }
void MapRender::tileIO8() { emit tileIO(8); }
void MapRender::tileIO9() { emit tileIO(9); }

/* ------ Tile item slots ------ */
void MapRender::tileItem0() { emit tileItem(0); }
void MapRender::tileItem1() { emit tileItem(1); }
void MapRender::tileItem2() { emit tileItem(2); }
void MapRender::tileItem3() { emit tileItem(3); }
void MapRender::tileItem4() { emit tileItem(4); }
void MapRender::tileItem5() { emit tileItem(5); }
void MapRender::tileItem6() { emit tileItem(6); }
void MapRender::tileItem7() { emit tileItem(7); }
void MapRender::tileItem8() { emit tileItem(8); }
void MapRender::tileItem9() { emit tileItem(9); }

/* ------ Tile npc slots ------ */
void MapRender::tileNPC0() { emit tileNPC(0); }
void MapRender::tileNPC1() { emit tileNPC(1); }
void MapRender::tileNPC2() { emit tileNPC(2); }
void MapRender::tileNPC3() { emit tileNPC(3); }
void MapRender::tileNPC4() { emit tileNPC(4); }
void MapRender::tileNPC5() { emit tileNPC(5); }
void MapRender::tileNPC6() { emit tileNPC(6); }
void MapRender::tileNPC7() { emit tileNPC(7); }
void MapRender::tileNPC8() { emit tileNPC(8); }
void MapRender::tileNPC9() { emit tileNPC(9); }

/* ------ Tile person slots ------ */
void MapRender::tilePerson0() { emit tilePerson(0); }
void MapRender::tilePerson1() { emit tilePerson(1); }
void MapRender::tilePerson2() { emit tilePerson(2); }
void MapRender::tilePerson3() { emit tilePerson(3); }
void MapRender::tilePerson4() { emit tilePerson(4); }
void MapRender::tilePerson5() { emit tilePerson(5); }
void MapRender::tilePerson6() { emit tilePerson(6); }
void MapRender::tilePerson7() { emit tilePerson(7); }
void MapRender::tilePerson8() { emit tilePerson(8); }
void MapRender::tilePerson9() { emit tilePerson(9); }

/* ------ Tile sprite slots ------ */
void MapRender::tileSpriteB() { emit tileSprite(EditorEnumDb::BASE); }
void MapRender::tileSpriteE() { emit tileSprite(EditorEnumDb::ENHANCER); }
void MapRender::tileSpriteL1() { emit tileSprite(EditorEnumDb::LOWER1); }
void MapRender::tileSpriteL2() { emit tileSprite(EditorEnumDb::LOWER2); }
void MapRender::tileSpriteL3() { emit tileSprite(EditorEnumDb::LOWER3); }
void MapRender::tileSpriteL4() { emit tileSprite(EditorEnumDb::LOWER4); }
void MapRender::tileSpriteL5() { emit tileSprite(EditorEnumDb::LOWER5); }
void MapRender::tileSpriteU1() { emit tileSprite(EditorEnumDb::UPPER1); }
void MapRender::tileSpriteU2() { emit tileSprite(EditorEnumDb::UPPER2); }
void MapRender::tileSpriteU3() { emit tileSprite(EditorEnumDb::UPPER3); }
void MapRender::tileSpriteU4() { emit tileSprite(EditorEnumDb::UPPER4); }
void MapRender::tileSpriteU5() { emit tileSprite(EditorEnumDb::UPPER5); }

/* ------ Tile thing slots ------ */
void MapRender::tileThing0() { emit tileThing(0); }
void MapRender::tileThing1() { emit tileThing(1); }
void MapRender::tileThing2() { emit tileThing(2); }
void MapRender::tileThing3() { emit tileThing(3); }
void MapRender::tileThing4() { emit tileThing(4); }
void MapRender::tileThing5() { emit tileThing(5); }
void MapRender::tileThing6() { emit tileThing(6); }
void MapRender::tileThing7() { emit tileThing(7); }
void MapRender::tileThing8() { emit tileThing(8); }
void MapRender::tileThing9() { emit tileThing(9); }

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
