/*******************************************************************************
 * Class Name: MapView
 * Date Created: December 22, 2014
 * Inheritance: QMainWindow
 * Description: View of the map editor component for all map related data:
 *              map sprites, map things, etc.
 ******************************************************************************/
#include "View/MapView.h"


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
  cursor_mode = EditorEnumDb::BASIC;

  /* Calls all setup functions */
  setupLeftBar();
  setupRightBar();
  setupMapView();
  setWindowFlags(windowFlags() & ~Qt::Window);

  /* Starts disabled */
  editing_map = NULL;
  setDisabled(true);
}

/*
 * Description: Destructor function
 */
MapView::~MapView()
{
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: Recursively fills all of the similar adjoining tiles with the
 *              selected sprite
 *
 * Inputs: x and y positions for the tile, and target(the tile's ID)
 *         and replacement (New sprite) id numbers
 */
void MapView::recursiveFill(int x, int y, EditorEnumDb::Layer layer,
                            EditorSprite* target, SubMapInfo* map)
{
  if(x >= 0 && y >= 0 && x < map->tiles.size() && y < map->tiles[x].size() &&
     map->tiles[x][y]->getSprite(layer) == target)
  {
    /* Place sprite */
    map->tiles[x][y]->place(layer,map_database->getSpriteView()->getSelected());

    /* Recursively proceed */
    recursiveFill(x + 1, y, layer, target, map);
    recursiveFill(x - 1, y, layer, target, map);
    recursiveFill(x, y + 1, layer, target, map);
    recursiveFill(x, y - 1, layer, target, map);
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
  //map_render_view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
  //map_render_view->setDragMode(QGraphicsView::ScrollHandDrag);
  map_render_view->ensureVisible(0,0,1,1);
  map_render_view->show();
  setCentralWidget(map_render_view);

  /* Sets up the map status bar */
  map_data = new QStatusBar(this);
  map_data->setStyleSheet("border-top: 1px solid #999999");
  setStatusBar(map_data);

  /* Connections */
  connect(map_render, SIGNAL(sendCurrentPosition(int,int)),
          this, SLOT(setCurrentTile(int,int)));
  connect(map_control, SIGNAL(addMap(SubMapInfo*)),
          map_render, SLOT(setRenderingMap(SubMapInfo*)));
  connect(map_render, SIGNAL(itemClick(EditorTile*)),
          this, SLOT(itemClick(EditorTile*)));
  connect(map_render, SIGNAL(itemMassClick(QList<EditorTile*>,bool)),
          this, SLOT(itemMassClick(QList<EditorTile*>,bool)));
  connect(map_render, SIGNAL(passSet(EditorTile*)),
          this, SLOT(passSet(EditorTile*)));
  connect(map_render, SIGNAL(passUnset(EditorTile*)),
          this, SLOT(passUnset(EditorTile*)));
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
}

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/

/* Adds things/sprites to tile, based on pens */
void MapView::itemClick(EditorTile* tile)
{
  // TODO: Future. Determine what is selected - sprites, things, etc.
  EditorSprite* selected = map_database->getSpriteView()->getSelected();
  EditorEnumDb::Layer layer = map_control->getSelectedLayer();

  /* Only proceed if selection is valid */
  if(selected != NULL && tile != NULL)
  {
    if(cursor_mode == EditorEnumDb::BASIC)
    {
      tile->place(layer, selected);
    }
    else if(cursor_mode == EditorEnumDb::ERASER)
    {
      tile->unplace(layer);
    }
    else if(cursor_mode == EditorEnumDb::FILL)
    {
      recursiveFill(tile->getX(), tile->getY(), layer,
                    tile->getSprite(layer), map_render->getRenderingMap());
    }
  }
}

/* Mass add/delete of rect of tiles */
void MapView::itemMassClick(QList<EditorTile*> tiles, bool erase)
{
  EditorSprite* selected = map_database->getSpriteView()->getSelected();
  EditorEnumDb::Layer layer = map_control->getSelectedLayer();

  /* Only proceed if selection is valid */
  if(selected != NULL)
  {
    if(cursor_mode == EditorEnumDb::BLOCKPLACE)
    {
      for(int i = 0; i < tiles.size(); i++)
      {
        if(erase)
          tiles[i]->unplace(layer);
        else
          tiles[i]->place(layer, selected);
      }
    }
  }
}

/* Passability set and unset on given tile */
void MapView::passSet(EditorTile* tile)
{
  if(tile != NULL)
  {
    if(cursor_mode == EditorEnumDb::PASS_N ||
       cursor_mode == EditorEnumDb::PASS_ALL)
      tile->setPassability(map_control->getSelectedLayer(),
                           Direction::NORTH, true);
    if(cursor_mode == EditorEnumDb::PASS_E ||
       cursor_mode == EditorEnumDb::PASS_ALL)
      tile->setPassability(map_control->getSelectedLayer(),
                           Direction::EAST, true);
    if(cursor_mode == EditorEnumDb::PASS_S ||
       cursor_mode == EditorEnumDb::PASS_ALL)
      tile->setPassability(map_control->getSelectedLayer(),
                           Direction::SOUTH, true);
    if(cursor_mode == EditorEnumDb::PASS_W ||
       cursor_mode == EditorEnumDb::PASS_ALL)
      tile->setPassability(map_control->getSelectedLayer(),
                           Direction::WEST, true);
  }
}

/* Passability set and unset on given tile */
void MapView::passUnset(EditorTile* tile)
{
  if(tile != NULL)
  {
    if(cursor_mode == EditorEnumDb::PASS_N ||
       cursor_mode == EditorEnumDb::PASS_ALL)
      tile->setPassability(map_control->getSelectedLayer(),
                           Direction::NORTH, false);
    if(cursor_mode == EditorEnumDb::PASS_E ||
       cursor_mode == EditorEnumDb::PASS_ALL)
      tile->setPassability(map_control->getSelectedLayer(),
                           Direction::EAST, false);
    if(cursor_mode == EditorEnumDb::PASS_S ||
       cursor_mode == EditorEnumDb::PASS_ALL)
      tile->setPassability(map_control->getSelectedLayer(),
                           Direction::SOUTH, false);
    if(cursor_mode == EditorEnumDb::PASS_W ||
       cursor_mode == EditorEnumDb::PASS_ALL)
      tile->setPassability(map_control->getSelectedLayer(),
                           Direction::WEST, false);
  }
}

/*
 * Description: Sets the position into the status bar
 *
 * Input: tile coordinates
 */
void MapView::setCurrentTile(int x, int y)
{
  map_data->clearMessage();

  /* Adds the total map data to the status bar */
  QString mapsize = "Map Size: ";
  mapsize.append(QString::number(map_render->getMapWidth()));
  mapsize.append(",");
  mapsize.append(QString::number(map_render->getMapHeight()));
  mapsize.append("          ");

  /* Adds the tile coordinate information to the status bar */
  if(x!=-1 || y!=-1)
  {
    QString coordinates = "Position: X:";
    coordinates.append(QString::number(x));
    coordinates.append(", Y:");
    coordinates.append(QString::number(y));
    mapsize.append(coordinates);
  }

  map_data->showMessage(mapsize);
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

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

  /* Update the map render, with the appropriate settings */
  if(mode == EditorEnumDb::BLOCKPLACE)
    map_render_view->setDragMode(QGraphicsView::RubberBandDrag);
  else
    map_render_view->setDragMode(QGraphicsView::NoDrag);

  /* Pass along to scene */
  map_render->setCursorMode(mode);
}

/* Sets the map being edited */
void MapView::setMapEditor(EditorMap* editor)
{
  /* Remove connections to the old map */
  map_render->setRenderingMap(NULL);
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

    /* Connect the Map Control and Database panel */
    map_database->setMapEditor(editing_map);
    map_control->setMapEditor(editing_map);

    /* Add the top sub-map by default */
    //if(editing_map->getMap(0) != NULL)
    //  map_render->setRenderingMap(editing_map->getMap(0));
  }
}
