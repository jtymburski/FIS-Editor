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
 * MESSAGE OUTPUT MANIPULATOR FUNCTION
 *===========================================================================*/

void messageOutput(QtMsgType type, const QMessageLogContext &context,
                     const QString &msg)
{
  (void)context;

  QString output;
  switch (type) {
    case QtDebugMsg:    output = QString("mesage: ") + msg; break;
    case QtWarningMsg:  output = QString("warning: ") + msg; break;
    case QtCriticalMsg: output = QString("critical: ") + msg; break;
    case QtFatalMsg:    output = QString("fatal: ") + msg; break;
    default: return;
  }

  QFile file("output_log.txt");
  if (file.open(QIODevice::WriteOnly | QIODevice::Append))
    QTextStream(&file) << output << "\n";
}

/*============================================================================
 * MAIN FUNCTION
 *===========================================================================*/

int main(int argc, char *argv[])
{
  /* Setup the sound */
//  Sound::initiateSDL();
//  qDebug() << "[DEBUG] Sound configured: " << Sound::statusSDL();

  /* Trigger output to log file - uncomment when needed */
  //qInstallMessageHandler(messageOutput);

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
