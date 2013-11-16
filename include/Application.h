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
#include <QScrollArea>
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

private:
  /* Scroll area pointer */
  QScrollArea* sidebar;

  /* Toolbox pointer */
  SpriteToolbox* toolbox;

  /* Sidebar dock pointer */
  QDockWidget* dock;

  /* Map View pointer */
  MapEditor* map_editor;

  /* File action pointers */
  QAction *new_action;
  QAction *load_action;
  QAction *recentfiles_action;
  QAction *save_action;
  QAction *saveas_action;
  QAction *quit_action;

  /* Edit action pointers */
  QAction *undo_action;
  QAction *redo_action;
  QAction *cut_action;
  QAction *copy_action;
  QAction *paste_action;
  QAction *findreplace_action;
  QAction *mapsize_action;

  /* The users login name */
  QString username;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Sets up the Top Menu */
  void setupTopMenu();

  /* Sets up the Sidebar */
  void setupSidebar();

  /* Sets up the Map View */
  void setupMapView();
/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:

/*============================================================================
 * SLOTS
 *===========================================================================*/
public slots:
  /* Called when the editor is closed */
  void closeEditor();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
};

#endif // APPLICATION_H
