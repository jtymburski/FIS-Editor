/*******************************************************************************
 * Class Name: Application
 * Date Created: November 11, 2013
 * Inheritance: QMainWindow
 * Description: The Main Application that handles all the displays for the 
 *              entire editor. This includes the map, all applicable toolbars.
 ******************************************************************************/
#include "Application.h"


/* Constant Implementation - see header file for descriptions */
//const short Application::kTICK_DELAY = 10;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

Application::Application(QWidget* parent) : QMainWindow(parent)
{
  toolbox = new SpriteToolbox(this);
  sidebar = new QWidget(this);
  sidebar->setMaximumSize(1100,720);
  sidebar->setMinimumSize(256,256);
  dock = new QDockWidget("Toolbox");
  dock->setWidget(sidebar);
  map_editor = new MapEditor();
  map_editor->setMinimumSize(512,512);
  map_editor->setMaximumSize(1280,720);
  addDockWidget(Qt::LeftDockWidgetArea,dock);
  dock->setAllowedAreas(Qt::AllDockWidgetAreas);
  dock->setFeatures(QDockWidget::DockWidgetMovable
                    | QDockWidget::DockWidgetFloatable);
  setCentralWidget(map_editor);

  username = getenv("USERNAME");

  setupTopMenu();

  //setFixedSize(1280,720);
}

Application::~Application()
{
}

void Application::setupTopMenu()
{
  new_action = new QAction("&New",this);
  new_action->setIcon(QIcon("icons/arcadius.png"));
  load_action = new QAction("&Load",this);
  save_action = new QAction("&Save",this);
  quit_action = new QAction("&Quit",this);
  saveas_action = new QAction("&Save As",this);
  recentfiles_action = new QAction("&Recent Files",this);
  undo_action = new QAction("&Undo",this);
  redo_action = new QAction("&Redo",this);
  findreplace_action = new QAction("&Find/Replace",this);
  copy_action = new QAction("&Copy",this);
  paste_action = new QAction("&Paste",this);
  cut_action = new QAction("&Cut",this);
  mapsize_action = new QAction("&Map Size",this);

  connect(quit_action,SIGNAL(triggered()), this, SLOT(closeEditor()));

  QMenu* file_menu = menuBar()->addMenu("&File");
  file_menu->addAction(new_action);
  file_menu->addAction(load_action);
  file_menu->addAction(recentfiles_action);
  file_menu->addSeparator();
  file_menu->addAction(save_action);
  file_menu->addAction(saveas_action);
  file_menu->addSeparator();
  file_menu->addAction(quit_action);

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
}

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

