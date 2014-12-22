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
//#include <windows.h>
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
#include "Map/MapEditor.h"
#include "ImageSelectionModule.h"
#include "EditorSpriteToolbox.h"
#include "SpriteToolbox.h"
#include "SizeSelector.h"

class Application : public QMainWindow
{
  Q_OBJECT

public:
  /* Constructor function */
  Application(QWidget* parent = 0, int xsize = 100, int ysize = 100);

  /* Destructor function */
  ~Application();

private:
  /* Map layer tabs */
  QToolBar* toolbar;
  QListWidget* sidetoolbar;

  /* Brush and Menu Tabs */
  QToolBar* brushbar;
  QToolBar* menubar;

  /* Toolbox pointer */
  QTabWidget* tab;
  ImageSelectionModule* images_tab;
  EditorSpriteToolbox* sprites_tab;

  /* Scroll area pointer */
  QScrollArea* sprites_tab_scrollwrapper;

  /* Sidebar dock pointer */
  QDockWidget* dock;
  QDockWidget* layer_dock;

  /* Map View pointer */
  QGraphicsView* map_scroller;
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

  /* Cursor action pointers */
  QActionGroup *cursor_group;
  QAction *erasermode_action;
  QAction *basicmode_action;
  QAction *blockplacemode_action;
  QAction *fill_action;

  /* Map Mode Shift Actions (Active Layers) */
  QListWidgetItem *active_lower_layer_01;
  QListWidgetItem *active_lower_layer_02;
  QListWidgetItem *active_lower_layer_03;
  QListWidgetItem *active_lower_layer_04;
  QListWidgetItem *active_lower_layer_05;
  QListWidgetItem *active_enhancer_layer;
  QListWidgetItem *active_item_layer;
  QListWidgetItem *active_base_layer;
  QListWidgetItem *active_person_layer;
  QListWidgetItem *active_thing_layer;
  QListWidgetItem *active_upper_layer_01;
  QListWidgetItem *active_upper_layer_02;
  QListWidgetItem *active_upper_layer_03;
  QListWidgetItem *active_upper_layer_04;
  QListWidgetItem *active_upper_layer_05;

  /* Map Mode Shift Actions (Shown Layers) */
  QActionGroup *shown_layers;
  QAction *shown_lower_layer_01;
  QAction *shown_lower_layer_02;
  QAction *shown_lower_layer_03;
  QAction *shown_lower_layer_04;
  QAction *shown_lower_layer_05;
  QAction *shown_enhancer_layer;
  QAction *shown_item_layer;
  QAction *shown_base_layer;
  QAction *shown_person_layer;
  QAction *shown_thing_layer;
  QAction *shown_upper_layer_01;
  QAction *shown_upper_layer_02;
  QAction *shown_upper_layer_03;
  QAction *shown_upper_layer_04;
  QAction *shown_upper_layer_05;
  QAction *shown_grid;
  QAction *shown_pass;

  /* The users login name */
  QString username;

  /* The currently selected sprite */
  QString current_sprite_choice;

  /* Map size for initial creation */
  int x_size;
  int y_size;

  /* Initial Map Creation */
  SizeSelector* mapsizedialog;

  /* Curosr Mode */
  EditorEnumDb::CursorMode cursor_mode;

  /* Map Data Bar */
  QStatusBar* map_data;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Sets up the Top Menu */
  void setupTopMenu();

  /* Sets up the Sidebar */
  void setupSidebar();

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

  void sendUpEditorSprite(EditorSprite* e);
/*============================================================================
 * SLOTS
 *===========================================================================*/
public slots:

  /* Toggles all layers to be on/off */
  void showAllLayers(bool);

  /* Sets the picked sprite */
  void setSprite(QString);

  /* Sets up the Map View */
  void setupMapView(int x = 0, int y = 0);

  /* Sets the Active Layer on the Map */
  void setActiveLayer(QListWidgetItem* layer);

  /* Sets to Eraser Mode */
  void setBasicCursor();

  /* Sets to Basic Mode */
  void setEraserCursor();

  /* Sets to Block Mode */
  void setBlockCursor();

  /* Sets to Fill Mode */
  void setFillCursor();

  /* Sets the status bar to have the current tile hovered over */
  void setCurrentTile(int,int);
/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
};

#endif // APPLICATION_H
