/*******************************************************************************
 * Class Name: MapView
 * Date Created: December 22, 2014
 * Inheritance: QMainWindow
 * Description: View of the map editor component for all map related data:
 *              map sprites, map things, etc.
 ******************************************************************************/
#ifndef MAPVIEW_H
#define MAPVIEW_H

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
#include <QScrollArea>
#include <QTabWidget>
#include <QToolBar>
#include <QGraphicsView>
#include <QListWidget>
#include <QStatusBar>
#include <QTreeView>
#include <QDesktopWidget>
#include "View/MapRender.h"
#include "View/RawImageView.h"
#include "View/SpriteView.h"
#include "View/RawImageList.h"

class MapView : public QMainWindow
{
  Q_OBJECT

public:
  /* Constructor function */
  MapView(QWidget* parent = 0, int xsize = 100, int ysize = 100);

  /* Destructor function */
  ~MapView();


  /* Map View pointer */
  QGraphicsView* map_scroller;
  MapRender* map_editor;

private:
  /* Map layer tabs */
  QListWidget* sidetoolbar;

  /* Toolbox pointer */
  QTabWidget* tab;
  RawImageView* images_tab;
  SpriteView* sprites_tab;

  /* Scroll area pointer */
  QScrollArea* sprites_tab_scrollwrapper;

  /* Sidebar dock pointer */
  QDockWidget* dock;
  QDockWidget* layer_dock;


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

  /* The currently selected sprite */
  QString current_sprite_choice;


  /* Map size for initial creation */
  int x_size;
  int y_size;

  /* Cursor Mode */
  EditorEnumDb::CursorMode cursor_mode;

  /* Map Data Bar */
  QStatusBar* map_data;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:

  /* Sets up the Sidebar */
  void setupSidebar();

  /* Sets up the Layer Bar */
  void setupLayerBar();

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
//protected:

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  void sendUpEditorSprite(EditorSprite* e);

/*============================================================================
 * SLOTS
 *===========================================================================*/
public slots:
  /* Sets the picked sprite */
  void setSprite(QString);

  /* Sets up the Map View */
  void setupMapView(int x = 0, int y = 0);

  /* Sets the Active Layer on the Map */
  void setActiveLayer(QListWidgetItem* layer);

  /* Sets the status bar to have the current tile hovered over */
  void setCurrentTile(int,int);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
};
#endif // MAPVIEW_H
