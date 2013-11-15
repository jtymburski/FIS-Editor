/*******************************************************************************
 * Class Name: Application
 * Date Created: November 11, 2013
 * Inheritance: QMainWindow
 * Description: The Main Application that handles all the displays for the 
 *              entire editor. This includes the map, all applicable toolbars.
 ******************************************************************************/
#ifndef APPLICATION_H
#define APPLICATION_H

#include <QMainWindow>
#include <QAction>
#include <QToolBar>
#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QDockWidget>
#include <windows.h>
#include "Map/MapEditor.h"
#include "SpriteToolbox.h"

class Application : public QMainWindow
{
  Q_OBJECT

public:
  /* Constructor function */
  Application(QWidget* parent = 0);

  /* Destructor function */
  ~Application();

  /* Enumerator: Application options to be selected */
  //enum MenuState{OFF,MAIN,CONTINUE,INOPTIONS,INEXIT,SECRET}; // OLD
  //enum AppItems{TITLESCREEN = 0,
  //              GAME        = 1,
  //              OPTIONS     = 2,
  //              EXIT        = 3};

private:
  /* Map editor pointer */
  QWidget* sidebar;
  SpriteToolbox* toolbox;
  QDockWidget* dock;
  MapEditor* map_editor;
  QAction *new_action;
  QAction *load_action;
  QAction *save_action;
  QAction *quit_action;
  QAction *saveas_action;
  QAction *recentfiles_action;
  QAction *undo_action;
  QAction *redo_action;
  QAction *findreplace_action;
  QAction *copy_action;
  QAction *paste_action;
  QAction *cut_action;
  QAction *mapsize_action;

  
  /*------------------- Constants -----------------------*/
  //const static short kTICK_DELAY; /* The tick time, in ms */

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Switches the widget inside the stack (plus appropriate calls needed) */
  //void switchWidget(int index);
  void setupTopMenu();
  QString username;
/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  //void closeEvent(QCloseEvent* event);

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* The closing signal, to shutdown the app */
  //void closing();

/*============================================================================
 * SLOTS
 *===========================================================================*/
public slots:
  /* Called when the game is closed to return the application back to title */
  void closeEditor();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
  /* The function called for closing. Clean-up put here */
  //void exit();
};

#endif // APPLICATION_H
