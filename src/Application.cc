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
}

Application::~Application()
{
}
