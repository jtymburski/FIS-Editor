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
#include <QActionGroup>
#include <QToolBar>
#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QDockWidget>
#include <windows.h>
#include <QScrollArea>
#include <QTabWidget>
#include <QToolBar>
#include <QTabWidget>
#include <QListWidget>
#include "Map/MapEditor.h"
#include "EditorSpriteToolbox.h"

class Application : public QMainWindow
{
  Q_OBJECT

public:
  /* Constructor function */
  Application(QWidget* parent = 0);

  /* Destructor function */
  ~Application();

private:
  /* Map layer tabs */
  QToolBar* toolbar;
  QListWidget* sidetoolbar;

  /* Toolbox pointer */
  QTabWidget* tab;
  SpriteToolbox* images_tab;
  EditorSpriteToolbox* sprites_tab;

  /* Scroll area pointer */
  QScrollArea* images_tab_scrollwrapper;
  QScrollArea* sprites_tab_scrollwrapper;

  /* Sidebar dock pointer */
  QDockWidget* dock;
  QDockWidget* layer_dock;

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

  /* View action pointers */
  QAction *viewalllayers_action;

  /* Map Mode Shift Actions (Active Layers) */
  QActionGroup *active_layers;
  QAction *active_base_layer_01;
  QAction *active_base_layer_02;
  QAction *active_base_layer_03;
  QAction *active_base_layer_04;
  QAction *active_base_layer_05;
  QAction *active_enhancer_layer;
  QAction *active_item_layer;
  QAction *active_lower_layer;
  QAction *active_person_layer;
  QAction *active_thing_layer;
  QAction *active_upper_layer_01;
  QAction *active_upper_layer_02;
  QAction *active_upper_layer_03;
  QAction *active_upper_layer_04;
  QAction *active_upper_layer_05;

  /* Map Mode Shift Actions (Shown Layers) */
  QActionGroup *shown_layers;
  QAction *shown_base_layer_01;
  QAction *shown_base_layer_02;
  QAction *shown_base_layer_03;
  QAction *shown_base_layer_04;
  QAction *shown_base_layer_05;
  QAction *shown_enhancer_layer;
  QAction *shown_item_layer;
  QAction *shown_lower_layer;
  QAction *shown_person_layer;
  QAction *shown_thing_layer;
  QAction *shown_upper_layer_01;
  QAction *shown_upper_layer_02;
  QAction *shown_upper_layer_03;
  QAction *shown_upper_layer_04;
  QAction *shown_upper_layer_05;

  /* The users login name */
  QString username;

  /* The currently selected sprite */
  QString current_sprite_choice;

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

  /* Sets up the Layer Bar */
  void setupLayerBar();

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Custom close event */
  void closeEvent(QCloseEvent *);
/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:

/*============================================================================
 * SLOTS
 *===========================================================================*/
public slots:

  /* Toggles all layers to be on/off */
  void showAllLayers(bool);

  /* Sets the picked sprite */
  void setSprite(QString);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
};

#endif // APPLICATION_H
