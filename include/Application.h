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
#include <QApplication>
#include <QActionGroup>
#include <QToolBar>
#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QDockWidget>
#ifdef _WIN32
#include <windows.h>
#endif
#include <QScrollArea>
#include <QTabWidget>
#include <QToolBar>
#include <QTabWidget>
#include <QGraphicsView>
#include <QListWidget>
#include <QStatusBar>
#include <QTreeView>
#include <QDesktopWidget>
#include "Database/GameDatabase.h"
#include "View/MapView.h"
#include "View/GameView.h"
#include "View/RawImageView.h"
#include "View/SpriteView.h"
#include "View/RawImageList.h"

class Application : public QMainWindow
{
  Q_OBJECT

public:
  /* Constructor function */
  Application(QWidget* parent = 0);

  /* Destructor function */
  ~Application();

private:
  /* game database */
  GameDatabase* game_database;
  QDockWidget* game_db_dock;

  /* Game View */
  GameView* game_view;

  /* Brush and Menu Tabs */
  QToolBar* brushbar;
  QToolBar* menubar;
  QToolBar* viewbar;

  /* File action pointers */
  QAction *show_menu_action;
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

  /* View action pointers */
  QAction *viewalllayers_action;

  /* Cursor action pointers */
  QActionGroup *cursor_group;
  QAction *erasermode_action;
  QAction *basicmode_action;
  QAction *blockplacemode_action;
  QAction *fill_action;

  /* The users login name */
  QString username;

  /* Cursor menu */
  QMenu* cursor_menu;

  /* Cursor Mode */
  EditorEnumDb::CursorMode cursor_mode;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Sets up the Top Menu */
  void setupTopMenu();

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Custom close event */
  void closeEvent(QCloseEvent *);

/*============================================================================
 * SIGNALS
 *===========================================================================*/
//signals:
/*============================================================================
 * SLOTS
 *===========================================================================*/
public slots:
  /* Sets to Eraser Mode */
  void setBasicCursor();

  /* Sets to Basic Mode */
  void setEraserCursor();

  /* Sets to Block Mode */
  void setBlockCursor();

  /* Sets to Fill Mode */
  void setFillCursor();

  /* Sets to any view */
  void setView(EditorEnumDb::ViewMode);

  /* Sets To Map View */
  void setMapView();

  /* Sets To Person View */
  void setPersonView();

  /* Sets To Party View */
  void setPartyView();

  /* Sets To Item View */
  void setItemView();

  /* Sets To Map */
  void setMap(EditorMap* map);

  /* Sets To Person */
  void setPerson(EditorPerson* person);

  /* Sets To Party */
  void setParty(EditorParty* party);

  /* Sets To Item */
  void setItem(EditorItem* item);

  /* Sets To Action */
  void setAction(EditorAction* action);

  /* Sets To Race */
  void setRace(EditorCategory* race);

  /* Sets To Battle Class */
  void setBattleClass(EditorCategory* battle_class);

  /* Sets To Skillset */
  void setSkillset(EditorSkillset* skill_set);

  /* Sets To Skill */
  void setSkill(EditorSkill* skill);

  /* Sets To Equipment */
  void setEquipment(EditorEquipment* equipment);

  /* Sets To Bubby */
  void setBubby(EditorBubby* bubby);
  
  /* Shows and hides the game database view */
  void showDatabase();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
};

#endif // APPLICATION_H
