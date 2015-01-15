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
#include <QSizePolicy>

#include "Database/EditorMap.h"
#include "View/MapRender.h"
#include "View/RawImageView.h"
#include "View/SpriteView.h"
#include "View/RawImageList.h"
#include "View/MapDatabase.h"
#include "View/MapControl.h"

class MapView : public QMainWindow
{
  Q_OBJECT

public:
  /* Constructor function */
  MapView(QWidget* parent = NULL);//, int xsize = 100, int ysize = 100);

  /* Destructor function */
  ~MapView();

private:
  /* The editing map */
  EditorMap* editing_map;

  /* Map Control pointer - right portion */
  MapControl* map_control;

  /* Map Status Bar - data at the bottom */
  QStatusBar* map_data;

  /* Map DB pointer - left portion */
  MapDatabase* map_database;

  /* Map editor - center portion */
  MapRender* map_editor;
  //QGraphicsView*

  /* Scroll area pointer */
  //QScrollArea* sprites_tab_scrollwrapper;

  /* Sidebar dock pointer */
  //QDockWidget* dock;
  //QDockWidget* layer_dock;

  /* The currently selected sprite */
  QString current_sprite_choice;

  /* Map size for initial creation */
  //int x_size;
  //int y_size;

  /* Cursor Mode */
  //EditorEnumDb::CursorMode cursor_mode;

  /* Map View pointer */
  //QGraphicsView* map_scroller;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Sets up the database bar */
  void setupLeftBar();

  /* Sets up the Map View - center portion */
  void setupMapView();//int x = 0, int y = 0);

  /* Sets up the control bar */
  void setupRightBar();

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
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* Sets the picked sprite */
  void setSprite(QString);

  /* Sets the Active Layer on the Map */
  void setActiveLayer(QListWidgetItem* layer);

  /* Sets the status bar to have the current tile hovered over */
  void setCurrentTile(int,int);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the map being edited */
  EditorMap* getMapEditor();

  /* Returns the Map Editor (center portion */
  MapRender* getMapEditorView();

  /* Sets the map being edited */
  void setMapEditor(EditorMap* editor);
};
#endif // MAPVIEW_H
