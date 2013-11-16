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

  /* Calls all setup functions */
  setupSidebar();
  setupMapView();
  setupTopMenu();
}

/*
 * Description: Destructor function
 */
Application::~Application()
{
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
  /* Sets up a scroll area with the toolbox, sizes everything as needed */
  toolbox = new SpriteToolbox(this);
  sidebar = new QScrollArea;
  sidebar->setBackgroundRole(QPalette::Dark);
  sidebar->setWidget(toolbox);
  sidebar->setMinimumSize(290,68);
  sidebar->setMaximumWidth(290);
  toolbox->setMinimumSize(290,68);
  toolbox->setMaximumWidth(290);

  /* Sets up the dock which contains the scroll area with the toolbox widget */
  dock = new QDockWidget("Toolbox");
  dock->setWidget(sidebar);
  addDockWidget(Qt::LeftDockWidgetArea,dock);
  dock->setAllowedAreas(Qt::AllDockWidgetAreas);
  dock->setFeatures(QDockWidget::DockWidgetMovable
                    | QDockWidget::DockWidgetFloatable);
}

/*
 * Description: Sets up the Map View
 *
 * Inputs: none
 */
void Application::setupMapView()
{
  /* Sets up the main map view widget */
  map_editor = new MapEditor();
  map_editor->setMinimumSize(512,512);
  map_editor->setMaximumSize(1280,720);
  setCentralWidget(map_editor);
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
  connect(quit_action,SIGNAL(triggered()), this, SLOT(closeEditor()));


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
}

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/

/*
 * Description: Opens up a creepy close dialog that makes sure you want to exit
 */
void Application::closeEditor()
{
  QString message = "Are you sure you want to do this ";
  message.append(username);
  message.append("...");

  if(QMessageBox::question(this, "Exit Editor",
                           message,
                           QMessageBox::Ok | QMessageBox::Cancel,
                           QMessageBox::Ok) == QMessageBox::Ok)
    close();
}

