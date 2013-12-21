/******************************************************************************
* Name: Main function call
* Date Created: November 11, 2013
* Inheritance: none
* Description: This is the first call of the entire editor and where it all
*              begins. Merely sets up the sound and QT and runs until the 
*              application close sequence is called. Built to create joy for 
*              all between the age of 17.7-17.9
******************************************************************************/
#include <QApplication>
#include <QDebug>

#include "Application.h"
//#include "Sound.h"

/*============================================================================
 * MAIN FUNCTION
 *===========================================================================*/

int main(int argc, char *argv[])
{
  /* Setup the sound */
//  Sound::initiateSDL();
//  qDebug() << "[DEBUG] Sound configured: " << Sound::statusSDL();

  /* Setup QT */
  QApplication qt_app(argc, argv);
  Application* app = new Application();
  app->show();

  /* Run QT App */
  int qt_result = qt_app.exec();

  /* Clean up QT and the game */
  app->hide();
  delete app;
	
  /* Clean up SDL */
//  Sound::cleanupSDL();

  return qt_result;
}
