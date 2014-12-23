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
MapView::MapView(QWidget* parent, int xsize, int ysize) :
  QMainWindow(parent)
{
  x_size = xsize;
  y_size = ysize;

  cursor_mode = EditorEnumDb::BASIC;
  current_sprite_choice = "Sup";
  map_editor = NULL;

  /* Calls all setup functions */
  setupSidebar();
  setupLayerBar();
  setupMapView();
  setWindowFlags(windowFlags() & ~Qt::Window);
}

/*
 * Description: Destructor function
 */
MapView::~MapView()
{
  //qDebug()<<"Removing MapView";
  delete map_editor;
  delete images_tab;
  delete sprites_tab;
  map_editor = NULL;
  images_tab = NULL;
  sprites_tab = NULL;
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: Sets up the sidebar
 *
 * Inputs: none
 */
void MapView::setupSidebar()
{
  /* Sets up a scroll area with the images tab */
  images_tab = new RawImageView(this);

  /* Sets up a scroll area with the sprites tab */
  sprites_tab = new SpriteView(this);
  sprites_tab->setFixedSize(288,600);

  connect(images_tab->getToolbox(),SIGNAL(sendUpEditorSprite(EditorSprite*)),
          sprites_tab,SLOT(addEditorSprite(EditorSprite*)));


  tab = new QTabWidget(this);
  tab->addTab(images_tab,"Raw Images");
  tab->addTab(sprites_tab,"Sprites");
  //tab->setMinimumSize(290,68);
  //tab->setMaximumWidth(290);

  /* Sets up the dock which contains the sprites and images tabs */
  dock = new QDockWidget("Toolbox");
  dock->setAllowedAreas(Qt::LeftDockWidgetArea);
  dock->setWidget(tab);
  addDockWidget(Qt::LeftDockWidgetArea,dock);
  dock->setFeatures(QDockWidget::DockWidgetMovable
                    | QDockWidget::DockWidgetFloatable);

  /* Connects sprite picking */
  connect(images_tab->getToolbox(),SIGNAL(pathOfImage(QString)),this,
          SLOT(setSprite(QString)));
}

/*
 * Description: Sets up the Map View
 *
 * Inputs: none
 */
void MapView::setupMapView(int x, int y)
{
  /* Sets up the main map view widget */
  map_editor = new MapRender(sprites_tab,this,x,y,cursor_mode);
  map_scroller = new QGraphicsView(map_editor,this);
  map_scroller->ensureVisible(0,0,1,1);
  map_scroller->show();
  map_scroller->setMinimumSize(640,512);
  //map_scroller->setMaximumSize(1280,720);
//  connect(shown_base_layer,SIGNAL(toggled(bool)),
//          map_editor,SLOT(toggleBase(bool)));
//  connect(shown_enhancer_layer,SIGNAL(toggled(bool)),
//          map_editor,SLOT(toggleEnhancer(bool)));
//  connect(shown_lower_layer_01,SIGNAL(toggled(bool)),
//          map_editor,SLOT(toggleLower1(bool)));
//  connect(shown_lower_layer_02,SIGNAL(toggled(bool)),
//          map_editor,SLOT(toggleLower2(bool)));
//  connect(shown_lower_layer_03,SIGNAL(toggled(bool)),
//          map_editor,SLOT(toggleLower3(bool)));
//  connect(shown_lower_layer_04,SIGNAL(toggled(bool)),
//          map_editor,SLOT(toggleLower4(bool)));
//  connect(shown_lower_layer_05,SIGNAL(toggled(bool)),
//          map_editor,SLOT(toggleLower5(bool)));
//  connect(shown_upper_layer_01,SIGNAL(toggled(bool)),
//          map_editor,SLOT(toggleUpper1(bool)));
//  connect(shown_upper_layer_02,SIGNAL(toggled(bool)),
//          map_editor,SLOT(toggleUpper2(bool)));
//  connect(shown_upper_layer_03,SIGNAL(toggled(bool)),
//          map_editor,SLOT(toggleUpper3(bool)));
//  connect(shown_upper_layer_04,SIGNAL(toggled(bool)),
//          map_editor,SLOT(toggleUpper4(bool)));
//  connect(shown_upper_layer_05,SIGNAL(toggled(bool)),
//          map_editor,SLOT(toggleUpper5(bool)));
//  connect(shown_grid,SIGNAL(toggled(bool)),
//          map_editor,SLOT(toggleGrid(bool)));
//  connect(shown_pass,SIGNAL(toggled(bool)),
//          map_editor,SLOT(togglePass(bool)));
  setCentralWidget(map_scroller);

  /* Sets up the map status bar */
  map_data = new QStatusBar(this);
  connect(map_editor,SIGNAL(sendCurrentPosition(int,int)),
          this,SLOT(setCurrentTile(int,int)));

  setStatusBar(map_data);
}

/*
 * Description: Sets up the Layer Bar
 *
 * Inputs: none
 */
void MapView::setupLayerBar()
{
  /* Sets up the active layer actions, makes them checkable and adds them to
     an action group which allows only one to be active at a time */

  active_base_layer = new QListWidgetItem("Base");
  active_enhancer_layer = new QListWidgetItem("Enhancer");
  active_lower_layer_01 = new QListWidgetItem("Lower 1");
  active_lower_layer_02 = new QListWidgetItem("Lower 2");
  active_lower_layer_03 = new QListWidgetItem("Lower 3");
  active_lower_layer_04 = new QListWidgetItem("Lower 4");
  active_lower_layer_05 = new QListWidgetItem("Lower 5");
  active_item_layer = new QListWidgetItem("Item");
  active_thing_layer = new QListWidgetItem("Thing");
  active_person_layer = new QListWidgetItem("Person");
  active_upper_layer_01 = new QListWidgetItem("Upper 1");
  active_upper_layer_02 = new QListWidgetItem("Upper 2");
  active_upper_layer_03 = new QListWidgetItem("Upper 3");
  active_upper_layer_04 = new QListWidgetItem("Upper 4");
  active_upper_layer_05 = new QListWidgetItem("Upper 5");

  /* Sets up the side toolbar which shows the current active layer */
  sidetoolbar = new QListWidget(this);
  sidetoolbar->addItem(active_base_layer);
  sidetoolbar->setCurrentRow(0);
  sidetoolbar->addItem(active_enhancer_layer);
  sidetoolbar->addItem(active_lower_layer_01);
  sidetoolbar->addItem(active_lower_layer_02);
  sidetoolbar->addItem(active_lower_layer_03);
  sidetoolbar->addItem(active_lower_layer_04);
  sidetoolbar->addItem(active_lower_layer_05);
  //sidetoolbar->addItem(active_item_layer);
  //sidetoolbar->addItem(active_thing_layer);
  //sidetoolbar->addItem(active_person_layer);
  sidetoolbar->addItem(active_upper_layer_01);
  sidetoolbar->addItem(active_upper_layer_02);
  sidetoolbar->addItem(active_upper_layer_03);
  sidetoolbar->addItem(active_upper_layer_04);
  sidetoolbar->addItem(active_upper_layer_05);
  connect(sidetoolbar,SIGNAL(itemClicked(QListWidgetItem*)),
          this,SLOT(setActiveLayer(QListWidgetItem*)));

  /* Sets up the active layer dock */
  layer_dock = new QDockWidget("Active Layer");
  layer_dock->setWidget(sidetoolbar);
  layer_dock->setAllowedAreas(Qt::RightDockWidgetArea);
  addDockWidget(Qt::RightDockWidgetArea,layer_dock);
  layer_dock->setFeatures(QDockWidget::DockWidgetMovable
                    | QDockWidget::DockWidgetFloatable);
}


/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/

/*
 * Description: Sets the current image choice to be the given path
 *
 * Inputs: Path
 */
void MapView::setSprite(QString path)
{
  current_sprite_choice = path;
}

/*
 * Description: Sets the active layer
 *
 * Inputs: List item that is sent to the map editor as an enum value
 */
void MapView::setActiveLayer(QListWidgetItem *layer)
{
  if(layer->text()== "Base")
  {
    map_editor->setEditingLayer(EditorEnumDb::BASE);
  }
  else if(layer->text()== "Enhancer")
  {
    map_editor->setEditingLayer(EditorEnumDb::ENHANCER);
  }
  else if(layer->text()== "Lower 1")
  {
    map_editor->setEditingLayer(EditorEnumDb::LOWER1);
  }
  else if(layer->text()== "Lower 2")
  {
    map_editor->setEditingLayer(EditorEnumDb::LOWER2);
  }
  else if(layer->text()== "Lower 3")
  {
    map_editor->setEditingLayer(EditorEnumDb::LOWER3);
  }
  else if(layer->text()== "Lower 4")
  {
    map_editor->setEditingLayer(EditorEnumDb::LOWER4);
  }
  else if(layer->text()== "Lower 5")
  {
    map_editor->setEditingLayer(EditorEnumDb::LOWER5);
  }
  else if(layer->text()== "Upper 1")
  {
    map_editor->setEditingLayer(EditorEnumDb::UPPER1);
  }
  else if(layer->text()== "Upper 2")
  {
    map_editor->setEditingLayer(EditorEnumDb::UPPER2);
  }
  else if(layer->text()== "Upper 3")
  {
    map_editor->setEditingLayer(EditorEnumDb::UPPER3);
  }
  else if(layer->text()== "Upper 4")
  {
    map_editor->setEditingLayer(EditorEnumDb::UPPER4);
  }
  else if(layer->text()== "Upper 5")
  {
    map_editor->setEditingLayer(EditorEnumDb::UPPER5);
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
