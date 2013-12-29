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
  images_tab = new SpriteToolbox(this);
  images_tab_scrollwrapper = new QScrollArea(this);
  images_tab_scrollwrapper->setBackgroundRole(QPalette::Dark);
  images_tab_scrollwrapper->setWidget(images_tab);
  images_tab_scrollwrapper->setMinimumSize(290,68);
  images_tab_scrollwrapper->setMaximumWidth(290);
  images_tab->setMinimumSize(290,68);
  images_tab->setMaximumWidth(290);

  /* Sets up a scroll area with the sprites tab */
  sprites_tab = new EditorSpriteToolbox(this);
  sprites_tab_scrollwrapper = new QScrollArea(this);
  sprites_tab_scrollwrapper->setBackgroundRole(QPalette::Dark);
  sprites_tab_scrollwrapper->setWidget(sprites_tab);
  sprites_tab_scrollwrapper->setMinimumSize(290,68);
  sprites_tab_scrollwrapper->setMaximumWidth(290);
  sprites_tab->setMinimumSize(290,68);
  sprites_tab->setMaximumWidth(290);


  connect(images_tab,SIGNAL(sendUpEditorSprite(EditorSprite*)),sprites_tab,
          SLOT(addEditorSprite(EditorSprite*)));


  tab = new QTabWidget(this);
  tab->addTab(images_tab_scrollwrapper,"Raw Images");
  tab->addTab(sprites_tab_scrollwrapper,"Sprites");
  //tab->setMinimumSize(290,68);
  //tab->setMaximumWidth(290);

  /* Sets up the dock which contains the sprites and images tabs */
  dock = new QDockWidget("Toolbox");
  dock->setWidget(tab);
  addDockWidget(Qt::LeftDockWidgetArea,dock);
  dock->setFeatures(QDockWidget::DockWidgetMovable
                    | QDockWidget::DockWidgetFloatable);

  /* Connects sprite picking */
  connect(images_tab,SIGNAL(pathOfImage(QString)),this,
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
  map_scroller = new QScrollArea(this);
  map_editor = new MapEditor(sprites_tab,this,50,50);
  map_editor->show();
  map_scroller->setWidget(map_editor);
  map_scroller->setMinimumSize(640,512);
  map_scroller->setMaximumSize(1280,720);
  connect(shown_lower_layer,SIGNAL(toggled(bool)),
          map_editor,SLOT(toggleBase(bool)));
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
  view_menu->addAction(shown_base_layer_01);
  view_menu->addAction(shown_base_layer_02);
  view_menu->addAction(shown_base_layer_03);
  view_menu->addAction(shown_base_layer_04);
  view_menu->addAction(shown_base_layer_05);
  view_menu->addAction(shown_enhancer_layer);
  view_menu->addAction(shown_item_layer);
  view_menu->addAction(shown_lower_layer);
  view_menu->addAction(shown_person_layer);
  view_menu->addAction(shown_thing_layer);
  view_menu->addAction(shown_upper_layer_01);
  view_menu->addAction(shown_upper_layer_02);
  view_menu->addAction(shown_upper_layer_03);
  view_menu->addAction(shown_upper_layer_04);
  view_menu->addAction(shown_upper_layer_05);
  view_menu->addSeparator();
  view_menu->addAction(viewalllayers_action);

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
  active_layers = new QActionGroup(this);
  active_layers->setExclusive(true);
  active_base_layer_01 = new QAction("Base 1",active_layers);
  active_base_layer_01->setCheckable(true);
  active_base_layer_02 = new QAction("Base 2",active_layers);
  active_base_layer_02->setCheckable(true);
  active_base_layer_03 = new QAction("Base 3",active_layers);
  active_base_layer_03->setCheckable(true);
  active_base_layer_04 = new QAction("Base 4",active_layers);
  active_base_layer_04->setCheckable(true);
  active_base_layer_05 = new QAction("Base 5",active_layers);
  active_base_layer_05->setCheckable(true);
  active_enhancer_layer = new QAction("Enhancer",active_layers);
  active_enhancer_layer->setCheckable(true);
  active_item_layer = new QAction("Item",active_layers);
  active_item_layer->setCheckable(true);
  active_lower_layer = new QAction("Lower",active_layers);
  active_lower_layer->setCheckable(true);
  active_person_layer = new QAction("Person",active_layers);
  active_person_layer->setCheckable(true);
  active_thing_layer = new QAction("Thing",active_layers);
  active_thing_layer->setCheckable(true);
  active_upper_layer_01 = new QAction("Upper 1",active_layers);
  active_upper_layer_01->setCheckable(true);
  active_upper_layer_02 = new QAction("Upper 2",active_layers);
  active_upper_layer_02->setCheckable(true);
  active_upper_layer_03 = new QAction("Upper 3",active_layers);
  active_upper_layer_03->setCheckable(true);
  active_upper_layer_04 = new QAction("Upper 4",active_layers);
  active_upper_layer_04->setCheckable(true);
  active_upper_layer_05 = new QAction("Upper 5",active_layers);
  active_upper_layer_05->setCheckable(true);

  /* Sets up the side toolbar which shows the current active layer */
  sidetoolbar = new QListWidget(this);
  sidetoolbar->addItem(active_base_layer_01->text());
  sidetoolbar->addItem(active_base_layer_02->text());
  sidetoolbar->addItem(active_base_layer_03->text());
  sidetoolbar->addItem(active_base_layer_04->text());
  sidetoolbar->addItem(active_base_layer_05->text());
  sidetoolbar->addItem(active_enhancer_layer->text());
  sidetoolbar->addItem(active_item_layer->text());
  sidetoolbar->addItem(active_lower_layer->text());
  sidetoolbar->addItem(active_person_layer->text());
  sidetoolbar->addItem(active_thing_layer->text());
  sidetoolbar->addItem(active_upper_layer_01->text());
  sidetoolbar->addItem(active_upper_layer_02->text());
  sidetoolbar->addItem(active_upper_layer_03->text());
  sidetoolbar->addItem(active_upper_layer_04->text());
  sidetoolbar->addItem(active_upper_layer_05->text());

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
  shown_base_layer_01 = new QAction("&Base 1",shown_layers);
  shown_base_layer_01->setCheckable(true);
  shown_base_layer_02 = new QAction("&Base 2",shown_layers);
  shown_base_layer_02->setCheckable(true);
  shown_base_layer_03 = new QAction("&Base 3",shown_layers);
  shown_base_layer_03->setCheckable(true);
  shown_base_layer_04 = new QAction("&Base 4",shown_layers);
  shown_base_layer_04->setCheckable(true);
  shown_base_layer_05 = new QAction("&Base 5",shown_layers);
  shown_base_layer_05->setCheckable(true);
  shown_enhancer_layer = new QAction("&Enhancer",shown_layers);
  shown_enhancer_layer->setCheckable(true);
  shown_item_layer = new QAction("&Item",shown_layers);
  shown_item_layer->setCheckable(true);
  shown_lower_layer = new QAction("&Lower",shown_layers);
  shown_lower_layer->setCheckable(true);
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

  /* Sets up the top toolbar which shows the currently shown layers */
  toolbar = new QToolBar("Tools",this);
  toolbar->addAction(shown_base_layer_01);
  toolbar->addAction(shown_base_layer_02);
  toolbar->addAction(shown_base_layer_03);
  toolbar->addAction(shown_base_layer_04);
  toolbar->addAction(shown_base_layer_05);
  toolbar->addAction(shown_enhancer_layer);
  toolbar->addAction(shown_item_layer);
  toolbar->addAction(shown_lower_layer);
  toolbar->addAction(shown_person_layer);
  toolbar->addAction(shown_thing_layer);
  toolbar->addAction(shown_upper_layer_01);
  toolbar->addAction(shown_upper_layer_02);
  toolbar->addAction(shown_upper_layer_03);
  toolbar->addAction(shown_upper_layer_04);
  toolbar->addAction(shown_upper_layer_05);
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
  shown_base_layer_01->setChecked(layers);
  shown_base_layer_02->setChecked(layers);
  shown_base_layer_03->setChecked(layers);
  shown_base_layer_04->setChecked(layers);
  shown_base_layer_05->setChecked(layers);
  shown_enhancer_layer->setChecked(layers);
  shown_item_layer->setChecked(layers);
  shown_lower_layer->setChecked(layers);
  shown_person_layer->setChecked(layers);
  shown_thing_layer->setChecked(layers);
  shown_upper_layer_01->setChecked(layers);
  shown_upper_layer_02->setChecked(layers);
  shown_upper_layer_03->setChecked(layers);
  shown_upper_layer_04->setChecked(layers);
  shown_upper_layer_05->setChecked(layers);
}

void Application::setSprite(QString path)
{
  current_sprite_choice = path;
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
