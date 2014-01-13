/*******************************************************************************
 * Class Name: Application
 * Date Created: November 11, 2013
 * Inheritance: QMainWindow
 * Description: The Main Application that handles all the displays for the 
 *              entire editor. This includes the map, all applicable toolbars.
 ******************************************************************************/
#include "Application.h"


/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function
 *
 * Input: Parent Widget
 */
Application::Application(QWidget* parent) : QMainWindow(parent)
{

  /* Gets the users name in windows only */
  username = getenv("USERNAME");

  current_sprite_choice = "Sup";

  /* Calls all setup functions */
  setupSidebar();
  setupLayerBar();
  setupMapView();
  setupTopMenu();
}

/*
 * Description: Destructor function
 */
Application::~Application()
{
  //qDebug()<<"Removing Application";
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
void Application::setupSidebar()
{
  /* Sets up a scroll area with the images tab */
  images_tab = new ImageSelectionModule(this);
  //images_tab->setFixedSize(288,640);

  /* Sets up a scroll area with the sprites tab */
  sprites_tab = new EditorSpriteToolbox(this);
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
void Application::setupMapView()
{
  /* Sets up the main map view widget */
  map_editor = new MapEditor(sprites_tab,this,100,100);
  map_scroller = new QGraphicsView(map_editor,this);
  map_scroller->ensureVisible(0,0,1,1);
  map_scroller->show();
  map_scroller->setMinimumSize(640,512);
  //map_scroller->setMaximumSize(1280,720);
  connect(shown_base_layer,SIGNAL(toggled(bool)),
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
  setCentralWidget(map_scroller);
}

/*
 * Description: Sets up the Top Menu
 *
 * Inputs: none
 */
void Application::setupTopMenu()
{
  /* Sets up the File menu actions */
  new_action = new QAction("&New",this);
  new_action->setIcon(QIcon("icons/arcadius.png"));
  load_action = new QAction("&Load",this);
  recentfiles_action = new QAction("&Recent Files",this);
  save_action = new QAction("&Save",this);
  saveas_action = new QAction("&Save As",this);
  quit_action = new QAction("&Quit",this);

  /* Sets up file menu itself */
  QMenu* file_menu = menuBar()->addMenu("&File");
  file_menu->addAction(new_action);
  file_menu->addAction(load_action);
  file_menu->addAction(recentfiles_action);
  file_menu->addSeparator();
  file_menu->addAction(save_action);
  file_menu->addAction(saveas_action);
  file_menu->addSeparator();
  file_menu->addAction(quit_action);

  /* Connects File menu actions to slots */
  connect(quit_action,SIGNAL(triggered()), this, SLOT(close()));


  /* Sets up Edit menu actions*/
  undo_action = new QAction("&Undo",this);
  redo_action = new QAction("&Redo",this);
  cut_action = new QAction("&Cut",this);
  copy_action = new QAction("&Copy",this);
  paste_action = new QAction("&Paste",this);
  findreplace_action = new QAction("&Find/Replace",this);
  mapsize_action = new QAction("&Map Size",this);

  /* Sets up Edit menu itself */
  QMenu* edit_menu = menuBar()->addMenu("&Edit");
  edit_menu->addAction(undo_action);
  edit_menu->addAction(redo_action);
  edit_menu->addSeparator();
  edit_menu->addAction(cut_action);
  edit_menu->addAction(copy_action);
  edit_menu->addAction(paste_action);
  edit_menu->addSeparator();
  edit_menu->addAction(findreplace_action);
  edit_menu->addSeparator();
  edit_menu->addAction(mapsize_action);

  /* Connects Edit menu actions to slots */

  /* Sets up View menu actions*/
  viewalllayers_action = new QAction("&View All Layers",this);
  viewalllayers_action->setCheckable(true);

  /* Adds the shortcut 'V' to the view all layers action */
  viewalllayers_action->setShortcut(QKeySequence(Qt::Key_V));

  /* Sets up View menu itself */
  QMenu* view_menu = menuBar()->addMenu("&View");
  view_menu->addAction(shown_lower_layer_01);
  view_menu->addAction(shown_lower_layer_02);
  view_menu->addAction(shown_lower_layer_03);
  view_menu->addAction(shown_lower_layer_04);
  view_menu->addAction(shown_lower_layer_05);
  view_menu->addAction(shown_enhancer_layer);
  view_menu->addAction(shown_item_layer);
  view_menu->addAction(shown_base_layer);
  view_menu->addAction(shown_person_layer);
  view_menu->addAction(shown_thing_layer);
  view_menu->addAction(shown_upper_layer_01);
  view_menu->addAction(shown_upper_layer_02);
  view_menu->addAction(shown_upper_layer_03);
  view_menu->addAction(shown_upper_layer_04);
  view_menu->addAction(shown_upper_layer_05);
  view_menu->addSeparator();
  view_menu->addAction(viewalllayers_action);
  view_menu->addAction(shown_grid);

  /* Connects View menu actions to slots */
  connect(viewalllayers_action,SIGNAL(toggled(bool)),
          this,SLOT(showAllLayers(bool)));
}

/*
 * Description: Sets up the Layer Bar
 *
 * Inputs: none
 */
void Application::setupLayerBar()
{
  /* Sets up the active layer actions, makes them checkable and adds them to
     an action group which allows only one to be active at a time */

  active_lower_layer_01 = new QListWidgetItem("Lower 1");
  active_lower_layer_02 = new QListWidgetItem("Lower 2");
  active_lower_layer_03 = new QListWidgetItem("Lower 3");
  active_lower_layer_04 = new QListWidgetItem("Lower 4");
  active_lower_layer_05 = new QListWidgetItem("Lower 5");
  active_enhancer_layer = new QListWidgetItem("Enhancer");
  active_item_layer = new QListWidgetItem("Item");
  active_base_layer = new QListWidgetItem("Base");
  active_person_layer = new QListWidgetItem("Person");
  active_thing_layer = new QListWidgetItem("Thing");
  active_upper_layer_01 = new QListWidgetItem("Upper 1");
  active_upper_layer_02 = new QListWidgetItem("Upper 2");
  active_upper_layer_03 = new QListWidgetItem("Upper 3");
  active_upper_layer_04 = new QListWidgetItem("Upper 4");
  active_upper_layer_05 = new QListWidgetItem("Upper 5");

  /* Sets up the side toolbar which shows the current active layer */
  sidetoolbar = new QListWidget(this);
  sidetoolbar->addItem(active_lower_layer_01);
  sidetoolbar->addItem(active_lower_layer_02);
  sidetoolbar->addItem(active_lower_layer_03);
  sidetoolbar->addItem(active_lower_layer_04);
  sidetoolbar->addItem(active_lower_layer_05);
  sidetoolbar->addItem(active_enhancer_layer);
  sidetoolbar->addItem(active_item_layer);
  sidetoolbar->addItem(active_base_layer);
  sidetoolbar->addItem(active_person_layer);
  sidetoolbar->addItem(active_thing_layer);
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
  addDockWidget(Qt::RightDockWidgetArea,layer_dock);
  layer_dock->setFeatures(QDockWidget::DockWidgetMovable
                    | QDockWidget::DockWidgetFloatable);

  /* Sets up the shown layer actions, makes them checkable and adds them to
     an action group which allows multiple to be active at a time */
  shown_layers = new QActionGroup(this);
  shown_layers->setExclusive(false);
  shown_lower_layer_01 = new QAction("&Lower 1",shown_layers);
  shown_lower_layer_01->setCheckable(true);
  shown_lower_layer_02 = new QAction("&Lower 2",shown_layers);
  shown_lower_layer_02->setCheckable(true);
  shown_lower_layer_03 = new QAction("&Lower 3",shown_layers);
  shown_lower_layer_03->setCheckable(true);
  shown_lower_layer_04 = new QAction("&Lower 4",shown_layers);
  shown_lower_layer_04->setCheckable(true);
  shown_lower_layer_05 = new QAction("&Lower 5",shown_layers);
  shown_lower_layer_05->setCheckable(true);
  shown_enhancer_layer = new QAction("&Enhancer",shown_layers);
  shown_enhancer_layer->setCheckable(true);
  shown_item_layer = new QAction("&Item",shown_layers);
  shown_item_layer->setCheckable(true);
  shown_base_layer = new QAction("&Base",shown_layers);
  shown_base_layer->setCheckable(true);
  shown_person_layer = new QAction("&Person",shown_layers);
  shown_person_layer->setCheckable(true);
  shown_thing_layer = new QAction("&Thing",shown_layers);
  shown_thing_layer->setCheckable(true);
  shown_upper_layer_01 = new QAction("&Upper 1",shown_layers);
  shown_upper_layer_01->setCheckable(true);
  shown_upper_layer_02 = new QAction("&Upper 2",shown_layers);
  shown_upper_layer_02->setCheckable(true);
  shown_upper_layer_03 = new QAction("&Upper 3",shown_layers);
  shown_upper_layer_03->setCheckable(true);
  shown_upper_layer_04 = new QAction("&Upper 4",shown_layers);
  shown_upper_layer_04->setCheckable(true);
  shown_upper_layer_05 = new QAction("&Upper 5",shown_layers);
  shown_upper_layer_05->setCheckable(true);
  shown_grid = new QAction("&Grid",shown_layers);
  shown_grid->setCheckable(true);
  showAllLayers(true);

  /* Sets up the top toolbar which shows the currently shown layers */
  toolbar = new QToolBar("Tools",this);
  toolbar->addAction(shown_lower_layer_01);
  toolbar->addAction(shown_lower_layer_02);
  toolbar->addAction(shown_lower_layer_03);
  toolbar->addAction(shown_lower_layer_04);
  toolbar->addAction(shown_lower_layer_05);
  toolbar->addAction(shown_enhancer_layer);
  toolbar->addAction(shown_item_layer);
  toolbar->addAction(shown_base_layer);
  toolbar->addAction(shown_person_layer);
  toolbar->addAction(shown_thing_layer);
  toolbar->addAction(shown_upper_layer_01);
  toolbar->addAction(shown_upper_layer_02);
  toolbar->addAction(shown_upper_layer_03);
  toolbar->addAction(shown_upper_layer_04);
  toolbar->addAction(shown_upper_layer_05);
  toolbar->addAction(shown_grid);
  addToolBar(Qt::TopToolBarArea,toolbar);
  toolbar->setFloatable(false);
  toolbar->setMovable(false);
}


/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/

/*
 * Description: Sets all map layers to be visible
 */
void Application::showAllLayers(bool layers)
{
  shown_lower_layer_01->setChecked(layers);
  shown_lower_layer_02->setChecked(layers);
  shown_lower_layer_03->setChecked(layers);
  shown_lower_layer_04->setChecked(layers);
  shown_lower_layer_05->setChecked(layers);
  shown_enhancer_layer->setChecked(layers);
  shown_item_layer->setChecked(layers);
  shown_base_layer->setChecked(layers);
  shown_person_layer->setChecked(layers);
  shown_thing_layer->setChecked(layers);
  shown_upper_layer_01->setChecked(layers);
  shown_upper_layer_02->setChecked(layers);
  shown_upper_layer_03->setChecked(layers);
  shown_upper_layer_04->setChecked(layers);
  shown_upper_layer_05->setChecked(layers);
  shown_grid->setChecked(layers);
}

/*
 * Description: Sets the current image choice to be the given path
 *
 * Inputs: Path
 */
void Application::setSprite(QString path)
{
  current_sprite_choice = path;
}

/*
 * Description: Sets the active layer
 *
 * Inputs: List item that is sent to the map editor as an enum value
 */
void Application::setActiveLayer(QListWidgetItem *layer)
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

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: The custom close event for saving changes
 *
 * Inputs: The event passed
 */
void Application::closeEvent(QCloseEvent *event)
{
  QString message = "Are you sure you want to do this ";
  message.append(username);
  message.append("...");

  if(QMessageBox::question(this, "Exit Editor",
                           message,
                           QMessageBox::Ok | QMessageBox::Cancel,
                           QMessageBox::Ok) == QMessageBox::Ok)
    event->accept();
  else
    event->ignore();
}
