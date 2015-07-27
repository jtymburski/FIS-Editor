/*******************************************************************************
 * Class Name: MapView
 * Date Created: December 22, 2014
 * Inheritance: QMainWindow
 * Description: View of the map editor component for all map related data:
 *              map sprites, map things, etc.
 ******************************************************************************/
#include "View/MapView.h"

/* Constants */
const int MapView::kDEFAULT_ZOOM = 5;
const int MapView::kNUM_ZOOM_STATES = 11;
const float MapView::kZOOM_STATES[] = {0.0625, 0.125, 0.25, 0.5, 0.75, 1,
                                       1.5, 2, 3, 4, 8};

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function
 *
 * Input: Parent Widget
 */
MapView::MapView(QWidget* parent)//, int xsize, int ysize) :
       : QMainWindow(parent)
{
  convo_dialog = NULL;
  cursor_mode = EditorEnumDb::BASIC;
  data_db = false;
  event_convo = false;
  event_enter = false;
  event_exit = false;
  event_external = false;
  event_tile = NULL;

  /* Calls all setup functions */
  setupLeftBar();
  setupRightBar();
  setupMapView();
  setWindowFlags(windowFlags() & ~Qt::Window);

  /* Starts disabled */
  editing_map = NULL;
  setDisabled(true);

  /* Event View */
  pop_event = new QDialog(this);
  pop_event->setWindowTitle("Event Edit");
  QGridLayout* e_layout = new QGridLayout(pop_event);
  event_ctrl = new EditorEvent(EventHandler::createEventTemplate());
  event_view = new EventView(event_ctrl, pop_event);
  connect(event_view, SIGNAL(editConversation(Conversation*,bool)),
          this, SLOT(editConversation(Conversation*,bool)));
  connect(event_view, SIGNAL(selectTile()), this, SLOT(selectTileEvent()));
  e_layout->addWidget(event_view, 0, 0, 1, 4);
  QPushButton* btn_event_ok = new QPushButton("Ok", pop_event);
  btn_event_ok->setDefault(true);
  connect(btn_event_ok, SIGNAL(clicked()), this, SLOT(buttonEventOk()));
  e_layout->addWidget(btn_event_ok, 1, 2);
  QPushButton* btn_event_cancel = new QPushButton("Cancel", pop_event);
  connect(btn_event_cancel, SIGNAL(clicked()), this, SLOT(buttonEventCancel()));
  e_layout->addWidget(btn_event_cancel, 1, 3);
  pop_event->setLayout(e_layout);
  pop_event->hide();
}

/*
 * Description: Destructor function
 */
MapView::~MapView()
{
  /* Delete event controller */
  buttonEventCancel();
  event_view->setEvent(NULL);
  delete event_ctrl;
  event_ctrl = NULL;
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* Fill event pop-up with map data */
// TODO: Comment
void MapView::fillEventWithData()
{
  if(editing_map != NULL)
  {
    /* Get data */
    QVector<QString> thing_list = editing_map->getPersonList(0, true, true);
    thing_list.push_front("0: Player");
    thing_list << editing_map->getNPCList(0, true, true);
    thing_list << editing_map->getThingList(0, true, true);
    thing_list << editing_map->getIOList(0, true, true);

    /* Load data in */
    event_view->setListSubmaps(editing_map->getMapList());
    event_view->setListThings(thing_list);

    data_db = false;
    emit updateEventObjects();
  }
}

/*
 * Description: Sets up the sidebar
 *
 * Inputs: none
 */
void MapView::setupLeftBar()
{
  map_database = new MapDatabase(this);
  connect(map_database, SIGNAL(updateEventObjects()),
          this, SLOT(updateEventObjectsDb()));
  connect(map_database, SIGNAL(ensureVisible(QGraphicsItem*)),
          this, SLOT(ensureVisible(QGraphicsItem*)));
  connect(map_database, SIGNAL(pathEditTrigger(EditorNPCPath*)),
          this, SLOT(pathEditTrigger(EditorNPCPath*)));

  /* Sets up the dock which contains the sprites and images tabs */
  QDockWidget* dock = new QDockWidget("Toolbox");
  dock->setAllowedAreas(Qt::LeftDockWidgetArea);
  dock->setWidget(map_database);
  addDockWidget(Qt::LeftDockWidgetArea,dock);
  dock->setFeatures(QDockWidget::DockWidgetMovable
                    | QDockWidget::DockWidgetFloatable);
}

/*
 * Description: Sets up the Map View
 *
 * Inputs: none
 */
void MapView::setupMapView()//int x, int y)
{
  /* Sets up the main map view widget */
  map_render = new MapRender(this);

  /* Set up the view - scroller */
  map_render_view = new QGraphicsView(map_render, this);
  zoom_state = kDEFAULT_ZOOM;
  map_render_view->scale(kZOOM_STATES[zoom_state], kZOOM_STATES[zoom_state]);
  map_render_view->ensureVisible(0,0,1,1);
  map_render_view->show();
  setCentralWidget(map_render_view);

  /* Sets up the map status bar */
  map_data = new QStatusBar(this);
  map_data->setStyleSheet("border-top: 1px solid #999999");
  setStatusBar(map_data);

  /* Connections - map objects to this */
  connect(map_render, SIGNAL(sendCurrentPosition(int,int)),
          this, SLOT(setCurrentTile(int,int)));
  connect(map_render, SIGNAL(tileEventEnter()), this, SLOT(tileEventEnter()));
  connect(map_render, SIGNAL(tileEventExit()), this, SLOT(tileEventExit()));
  connect(map_render, SIGNAL(sendSelectedTile(int,int,int)),
          this, SLOT(sendSelectedTile(int,int,int)));
  connect(map_database, SIGNAL(selectTile()), this, SLOT(selectTileDb()));

  /* Connections - map control to others */
  connect(map_control, SIGNAL(updateMap()),
          map_render, SLOT(updateRenderingMap())); 
  connect(map_control, SIGNAL(updateAllLists()),
          map_database, SLOT(updateAllLists()));

  /* Connections - map database to others */
  connect(map_database, SIGNAL(changeLayer(EditorEnumDb::Layer)),
          map_control, SLOT(changeLayer(EditorEnumDb::Layer)));

  /* Connections - map render to others */
  connect(map_render, SIGNAL(tileSprite(EditorEnumDb::Layer)),
          map_database->getSpriteView(),
          SLOT(editHoverSprite(EditorEnumDb::Layer)));
  connect(map_render, SIGNAL(tileThing(int)),
          map_database->getThingView(), SLOT(editHoverInstance(int)));
  connect(map_render, SIGNAL(tilePerson(int)),
          map_database->getPersonView(), SLOT(editHoverInstance(int)));
  connect(map_render, SIGNAL(tileNPC(int)),
          map_database->getNPCView(), SLOT(editHoverInstance(int)));
  connect(map_render, SIGNAL(tileItem(int)),
          map_database->getItemView(), SLOT(editHoverInstance(int)));
  connect(map_render, SIGNAL(tileIO(int)),
          map_database->getIOView(), SLOT(editHoverInstance(int)));
}

/*
 * Description: Sets up the Layer Bar
 *
 * Inputs: none
 */
void MapView::setupRightBar()
{
  map_control = new MapControl(this);

  /* Sets up the active layer dock */
  QDockWidget* layer_dock = new QDockWidget("Active Layer");
  layer_dock->setWidget(map_control);
  layer_dock->setAllowedAreas(Qt::RightDockWidgetArea);
  addDockWidget(Qt::RightDockWidgetArea,layer_dock);
  layer_dock->setFeatures(QDockWidget::DockWidgetMovable
                    | QDockWidget::DockWidgetFloatable);

  /* Connections */
  connect(map_control, SIGNAL(layerChanged(EditorEnumDb::Layer)),
          this, SIGNAL(layerChanged(EditorEnumDb::Layer)));
}

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/

/* Event button triggers */
// TODO: Comment
void MapView::buttonEventCancel()
{
  if(event_enter || event_exit)
  {
    /* If conversation dialog is visible, hide it first */
    if(convo_dialog != NULL && convo_dialog->isVisible())
      convo_dialog->buttonCancel();

    /* Then the event pop-up */
    pop_event->hide();
    event_ctrl->setEventBlank();
    event_view->update();
    event_enter = false;
    event_exit = false;
    event_tile = NULL;
  }
}

/* Event button triggers */
// TODO: Comment
void MapView::buttonEventOk()
{
  if(event_enter || event_exit)
  {
    /* If conversation dialog is visible, hide it first */
    if(convo_dialog != NULL && convo_dialog->isVisible())
      convo_dialog->buttonCancel();

    /* Hide pop-up */
    pop_event->hide();

    /* Set the event */
    if(event_tile != NULL)
    {
      if(event_enter)
        event_tile->setEventEnter(*event_ctrl->getEvent());
      else if(event_exit)
        event_tile->setEventExit(*event_ctrl->getEvent());
    }

    /* Clear out the event control and view class */
    event_ctrl->setEventBlank(false);
    event_enter = false;
    event_exit = false;
    event_tile = NULL;
    event_view->update();
  }
}

/* Edit conversation trigger */
// TODO: Comment
void MapView::editConversation(Conversation* convo, bool is_option)
{
  if(convo_dialog != NULL)
  {
    disconnect(convo_dialog->getEventView(), SIGNAL(selectTile()),
               this, SLOT(selectTileConvo()));
    disconnect(convo_dialog, SIGNAL(success()),
               event_view, SLOT(updateConversation()));
    delete convo_dialog;
  }
  convo_dialog = NULL;

  /* Create the new conversation dialog */
  convo_dialog = new ConvoDialog(convo, is_option, this);
  convo_dialog->setListThings(event_view->getListThings(), false);
  convo_dialog->getEventView()->setListItems(event_view->getListItems());
  convo_dialog->getEventView()->setListMaps(event_view->getListMaps());
  convo_dialog->getEventView()->setListSubmaps(event_view->getListSubmaps());
  connect(convo_dialog->getEventView(), SIGNAL(selectTile()),
          this, SLOT(selectTileConvo()));
  connect(convo_dialog, SIGNAL(success()),
          event_view, SLOT(updateConversation()));
  convo_dialog->show();
}

/* Ensures the following item is visible in scene */
// TODO: Comment
void MapView::ensureVisible(QGraphicsItem* item)
{
  // TODO: FIX
  if(map_render_view != NULL)
    map_render_view->ensureVisible(item, 100, 100);
}

/* Path edit trigger */
// TODO: Comment
void MapView::pathEditTrigger(EditorNPCPath* path)
{
  if(map_render != NULL)
  {
    map_render->pathEditTrigger(path);

    /* If new path to edit, disable upper control */
    if(path != NULL)
    {
      map_control->disableControl(true);
      emit disableControl(true);
    }
    /* Otherwise, enable upper control */
    else
    {
      map_control->disableControl(false);
      emit disableControl(false);
    }
  }
}

/* Select a tile trigger */
// TODO: Comment
void MapView::selectTileConvo()
{
  if(convo_dialog != NULL)
  {
    selectTileEvent();
    event_convo = true;
    convo_dialog->hide();
  }
}

/* Select a tile trigger */
// TODO: Comment
void MapView::selectTileDb()
{
  event_external = true;
  map_render->selectTile();
}

/* Select a tile trigger */
// TODO: Comment
void MapView::selectTileEvent()
{
  event_external = false;
  event_convo = false;
  pop_event->hide();
  map_render->selectTile();
}

/* Send the selected tile to the event controller */
// TODO: Comment
void MapView::sendSelectedTile(int id, int x, int y)
{
  if(event_external)
  {
    map_database->sendSelectedTile(id, x, y);
  }
  else
  {
    pop_event->show();
    if(event_convo)
    {
      convo_dialog->getEventView()->updateSelectedTile(id, x, y);
      convo_dialog->show();
    }
    else
    {
      event_view->updateSelectedTile(id, x, y);
    }
  }
}

/*
 * Description: Sets the position into the status bar
 *
 * Input: tile coordinates
 * Output: none
 */
void MapView::setCurrentTile(int x, int y)
{
  map_data->clearMessage();

  /* Adds the total map data to the status bar */
  QString mapsize = "Map: ";
  mapsize.append(QString::number(map_render->getMapWidth()));
  mapsize.append(",");
  mapsize.append(QString::number(map_render->getMapHeight()));
  mapsize.append(" | ");

  /* Adds the tile coordinate information to the status bar */
  if(x!=-1 || y!=-1)
  {
    /* The position section */
    QString coordinates = "X:";
    coordinates.append(QString::number(x));
    coordinates.append(", Y:");
    coordinates.append(QString::number(y));
    mapsize.append(coordinates);

    /* The active layer section */
    mapsize.append("          ");
    QString active_layer = "Layers: ";
    active_layer += editing_map->getSetLayers(
                                       map_control->getCurrentMapIndex(), x, y);
    mapsize.append(active_layer);
  }

  map_data->showMessage(mapsize);
}

/* Tile enter/exit event slots */
// TODO: Comment
void MapView::tileEventEnter()
{
  /* Close the pop-up if open */
  if(event_enter || event_exit)
    buttonEventCancel();

  /* Set the event and open the new pop-up */
  event_tile = editing_map->getHoverInfo()->hover_tile;
  if(event_tile != NULL)
  {
    event_ctrl->setEvent(EventHandler::copyEvent(event_tile->getEventEnter()));
    event_view->updateEvent();
    pop_event->setWindowTitle("Enter Event Edit");
    pop_event->show();
    event_enter = true;

    fillEventWithData();
  }
}

/* Tile enter/exit event slots */
// TODO: Comment
void MapView::tileEventExit()
{
  /* Close the pop-up if open */
  if(event_enter || event_exit)
    buttonEventCancel();

  /* Set the event and open the new pop-up */
  event_tile = editing_map->getHoverInfo()->hover_tile;
  if(event_tile != NULL)
  {
    event_ctrl->setEvent(EventHandler::copyEvent(event_tile->getEventExit()));
    event_view->updateEvent();
    pop_event->setWindowTitle("Exit Event Edit");
    pop_event->show();
    event_exit = true;

    fillEventWithData();
  }
}

/* Updates event objects in the map database */
// TODO: Comment
void MapView::updateEventObjectsDb()
{
  data_db = true;
  emit updateEventObjects();
}

/* Updated data to pass into map database */
// TODO: Comment
void MapView::updatedItems(QVector<QString> items)
{
  if(data_db)
    map_database->updatedItems(items);
  else
    event_view->setListItems(items);
}

/* Updated data to pass into map database */
// TODO: Comment
void MapView::updatedMaps(QVector<QString> maps)
{
  if(data_db)
    map_database->updatedMaps(maps);
  else
    event_view->setListMaps(maps);
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Returns the current sub-map index */
int MapView::getCurrentSubMap()
{
  return map_control->getCurrentMapIndex();
}

/* Returns the map being edited */
EditorMap* MapView::getMapEditor()
{
  return editing_map;
}

/* Returns the Map Editor (center portion */
MapRender* MapView::getMapEditorView()
{
  return map_render;
}

/* Sets the internal cursor mode */
void MapView::setCursorMode(EditorEnumDb::CursorMode mode)
{
  cursor_mode = mode;
  if(editing_map != NULL)
    editing_map->setHoverCursor(mode);

  /* Update the map render, with the appropriate settings */
  if(mode == EditorEnumDb::BLOCKPLACE)
    map_render_view->setDragMode(QGraphicsView::RubberBandDrag);
  else
    map_render_view->setDragMode(QGraphicsView::NoDrag);

  /* Pass along to scene */
  //map_render->setCursorMode(mode); // TODO: REMOVE
}

/* Sets the map being edited */
void MapView::setMapEditor(EditorMap* editor)
{
  /* Remove connections to the old map */
  map_render->setMapEditor(NULL);
  map_control->setMapEditor(NULL);
  map_database->setMapEditor(NULL);
  setDisabled(true);

  /* Set the new editing map */
  editing_map = editor;

  /* Load in the new info */
  if(editing_map != NULL)
  {
    /* Un-disable view */
    setDisabled(false);

    /* Set up editing map */
    editing_map->clearHoverInfo();
    editing_map->setHoverCursor(cursor_mode);

    /* Connect the Map Control and Database panel */
    map_render->setMapEditor(editing_map);
    map_database->setMapEditor(editing_map);
    map_control->setMapEditor(editing_map);

    /* Add the top sub-map by default */
    //if(editing_map->getMap(0) != NULL)
    //  map_render->setRenderingMap(editing_map->getMap(0));
  }
}

/* Zooms the map view in or out */
bool MapView::zoomIn()
{
  /* Pre-checks */
  if(zoom_state < (kNUM_ZOOM_STATES - 1))
  {
    float current_zoom = kZOOM_STATES[zoom_state];
    zoom_state++;
    float new_zoom = kZOOM_STATES[zoom_state] / current_zoom;
    map_render_view->scale(new_zoom, new_zoom);

    /* Check if the next one can occur */
    if(zoom_state == (kNUM_ZOOM_STATES - 1))
      return true;
  }

  return false;
}

/* Zooms the map view in or out */
bool MapView::zoomOut()
{
  /* Pre-checks */
  if(zoom_state > 0)
  {
    float current_zoom = kZOOM_STATES[zoom_state];
    zoom_state--;
    float new_zoom = kZOOM_STATES[zoom_state] / current_zoom;
    map_render_view->scale(new_zoom, new_zoom);

    /* Check if the next one can occur */
    if(zoom_state == 0)
      return true;
  }

  return false;
}
