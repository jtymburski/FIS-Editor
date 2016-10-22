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
#include <QProcess>
#include <QScrollArea>
#include <QTabWidget>
#include <QTextStream>
#include <QToolBar>
#include <QTabWidget>
#include <QGraphicsView>
#include <QListWidget>
#include <QStatusBar>
#include <QTreeView>
#include <QDesktopWidget>

#include "Database/GameDatabase.h"
#include "FileHandler.h"
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
  Application(QWidget* parent = NULL);

  /* Destructor function */
  ~Application();

private:
  /* Actions */
  QAction* action_export;
  QAction* action_load;
  QAction* action_new;
  QAction* action_play;
  QAction* action_recent;
  QAction* action_save;
  QAction* action_saveas;
  QAction* action_zoom_in;
  QAction* action_zoom_out;

  /* Pen Actions */
  QAction* action_basic;
  QAction* action_blockplace;
  QAction* action_fill;
  QAction* action_move;
  QAction* action_passall;
  QAction* action_passE;
  QAction* action_passN;
  QAction* action_passS;
  QAction* action_passW;

  /* Brush and Menu Tabs */
  QToolBar* bar_brush;
  QToolBar* bar_map_ctrl;
  QToolBar* bar_menu;

  /* File name - last used save as */
  QString file_name;

  /* Game Database */
  GameDatabase* game_database;
  QDockWidget* game_db_dock;

  /* Game View */
  GameView* game_view;

  /* Menus */
  QMenu* menu_cursor;
  QMenu* menu_edit;

  /* Process execution variable */
  QProcess run_process;

  /* View action pointers */
  QAction *viewalllayers_action;

  /* The users login name */
  QString username;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Create progress dialog */
  QProgressDialog* createProgressDialog(int total_count, QString title_text,
                                        QString label_text);

  /* Export application to run in Univursa */
  void exportGame(QString filename);

  /* Load application */
  void loadApp(QString filename);

  /* Save application */
  void saveApp();

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
  /* Disables control for when in editing view with map */
  void disableControl(bool disabled);

  /* Export action */
  void exportTo();

  /* Slot for layer changing */
  void layerChanged(EditorEnumDb::Layer layer);

  /* Load action */
  void load();

  /* New game */
  bool newGame();

  /* Plays the current sub-map in the game */
  void play();
  void playFinished(int);

  /* Save and save as action */
  void save();
  void saveAs();

  /* Sets To Action */
  void setAction(EditorAction* action);

  /* Sets To Battle Class */
  void setBattleClass(EditorCategory* battle_class);

  /* Sets to battle scene */
  void setBattleScene(EditorBattleScene* scene, bool save);

  /* Sets To Bubby */
  //void setBubby(EditorBubby* bubby);

  /* Sets normal cursors */
  void setCursorBasic(bool checked);
  void setCursorBlock(bool checked);
  void setCursorEraser(bool checked);
  void setCursorFill(bool checked);
  void setCursorMove(bool checked);

  /* Sets To Equipment */
  //void setEquipment(EditorEquipment* equipment);

  /* Sets To Item */
  void setItem(EditorItem* item);

  /* Sets To Map */
  void setMap(EditorMap* map);

  /* Sets To Party */
  void setParty(EditorParty* party);

  /* Sets passability cursors */
  void setPassAllCursor(bool checked);
  void setPassEastCursor(bool checked);
  void setPassNorthCursor(bool checked);
  void setPassSouthCursor(bool checked);
  void setPassWestCursor(bool checked);

  /* Sets To Person */
  void setPerson(EditorPerson* person);

  /* Sets To Race */
  void setRace(EditorCategory* race);

  /* Sets To Skill */
  void setSkill(EditorSkill* skill);

  /* Sets To Skillset */
  void setSkillset(EditorSkillset* skill_set);

  /* Sets to any view */
  void setView(EditorEnumDb::ViewMode);
  void setViewItem();
  void setViewMap();
  void setViewParty();
  void setViewPerson();

  /* Shows and hides the game database view */
  void showDatabase();

  /* Zoom in or out in the map */
  void zoomInMap();
  void zoomOutMap();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
};

#endif // APPLICATION_H
