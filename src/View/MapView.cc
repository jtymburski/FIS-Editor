/*******************************************************************************
 * Class Name: MapView
 * Date Created: December 22, 2014
 * Inheritance: QMainWindow
 * Description: View of the map editor component for all map related data:
 *              map sprites, map things, etc.
 ******************************************************************************/
#include "View/MapView.h"

/* Constructor function */
MapView::MapView(QWidget* parent) : QMainWindow(parent)
{
  map_scroller = NULL;
  map_editor = NULL;
}

/* Destructor function */
MapView::~MapView()
{

}

/*******************************************************************************
 * PUBLIC SLOT FUNCTIONS
 ******************************************************************************/

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
  mapsize.append(QString::number(map_editor->getMapWidth()));
  mapsize.append(",");
  mapsize.append(QString::number(map_editor->getMapHeight()));
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

/*******************************************************************************
 * PUBLIC FUNCTIONS
 ******************************************************************************/

/*
 * Description: Sets up the Map View
 *
 * Inputs: int x - x size of map
 *         int y - y size of map
 * Output: none
 */
void MapView::setupMapView(SpriteView* toolbox, int x, int y,
                           EditorEnumDb::CursorMode cursor)
{
  /* Sets up the main map view widget */
  map_editor = new MapRender(toolbox,this,x,y,cursor);
  map_scroller = new QGraphicsView(map_editor,this);
  map_scroller->ensureVisible(0,0,1,1);
  map_scroller->show();
  map_scroller->setMinimumSize(640,512);
  //map_scroller->setMaximumSize(1280,720);
/*  connect(shown_base_layer,SIGNAL(toggled(bool)),
          map_editor,SLOT(toggleBase(bool)));
  connect(shown_enhancer_layer,SIGNAL(toggled(bool)),
          map_editor,SLOT(toggleEnhancer(bool)));
  connect(shown_lower_layer_01,SIGNAL(toggled(bool)),
          map_editor,SLOT(toggleLower1(bool)));
  connect(shown_lower_layer_02,SIGNAL(toggled(bool)),
          map_editor,SLOT(toggleLower2(bool)));
  connect(shown_lower_layer_03,SIGNAL(toggled(bool)),
          map_editor,SLOT(toggleLower3(bool)));
  connect(shown_lower_layer_04,SIGNAL(toggled(bool)),
          map_editor,SLOT(toggleLower4(bool)));
  connect(shown_lower_layer_05,SIGNAL(toggled(bool)),
          map_editor,SLOT(toggleLower5(bool)));
  connect(shown_upper_layer_01,SIGNAL(toggled(bool)),
          map_editor,SLOT(toggleUpper1(bool)));
  connect(shown_upper_layer_02,SIGNAL(toggled(bool)),
          map_editor,SLOT(toggleUpper2(bool)));
  connect(shown_upper_layer_03,SIGNAL(toggled(bool)),
          map_editor,SLOT(toggleUpper3(bool)));
  connect(shown_upper_layer_04,SIGNAL(toggled(bool)),
          map_editor,SLOT(toggleUpper4(bool)));
  connect(shown_upper_layer_05,SIGNAL(toggled(bool)),
          map_editor,SLOT(toggleUpper5(bool)));
  connect(shown_grid,SIGNAL(toggled(bool)),
          map_editor,SLOT(toggleGrid(bool)));
  connect(shown_pass,SIGNAL(toggled(bool)),
          map_editor,SLOT(togglePass(bool)));*/
  setCentralWidget(map_scroller);

  /* Sets up the map status bar */
  map_data = new QStatusBar(this);
  connect(map_editor,SIGNAL(sendCurrentPosition(int,int)),
          this,SLOT(setCurrentTile(int,int)));

  setStatusBar(map_data);
}
