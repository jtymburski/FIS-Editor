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
  map_editor = new MapEditor();
  setCentralWidget(map_editor);

  username = getenv("USERNAME");

  setupTopMenu();

  setFixedSize(1280,720);
}

Application::~Application()
{
}

void Application::setupTopMenu()
{
  newAction = new QAction("&New",this);
  newAction->setIcon(QIcon("icons/arcadius.png"));
  loadAction = new QAction("&Load",this);
  saveAction = new QAction("&Save",this);
  quitAction = new QAction("&Quit",this);

  connect(quitAction,SIGNAL(triggered()), this, SLOT(closeEditor()));

  QMenu* file_menu = menuBar()->addMenu("&File");
  file_menu->addAction(newAction);
  file_menu->addAction(loadAction);
  file_menu->addAction(saveAction);
  file_menu->addSeparator();
  file_menu->addAction(quitAction);
  QMenu* edit_menu = menuBar()->addMenu("&Edit");
  edit_menu->addAction(newAction);
  edit_menu->addAction(loadAction);
  edit_menu->addAction(saveAction);
  edit_menu->addSeparator();
  edit_menu->addAction(quitAction);
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
