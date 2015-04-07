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
  /* Cursor Mode */
  EditorEnumDb::CursorMode cursor_mode;

  /* The editing map */
  EditorMap* editing_map;

  /* Map Control pointer - right portion */
  MapControl* map_control;

  /* Map Status Bar - data at the bottom */
  QStatusBar* map_data;

  /* Map DB pointer - left portion */
  MapDatabase* map_database;

  /* Map editor - center portion */
  MapRender* map_render;
  QGraphicsView* map_render_view;

  /* The current zoom state */
  int zoom_state;

  /* Constants */
  static const int kDEFAULT_ZOOM;
  static const int kNUM_ZOOM_STATES;
  static const float kZOOM_STATES[];

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
  /* Disables control on high level for when in editing view with map */
  void disableControl(bool disabled);

  /* Passes the editor sprite up */
  void sendUpEditorSprite(EditorSprite* e);

  /* Updated data to pass into map database */
  void updatedItems(QVector<QString> items);
  void updatedMaps(QVector<QString> maps);

  /* Updates event objects in the map database class */
  void updateEventObjects();

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* Ensures the following item is visible in scene */
  void ensureVisible(QGraphicsItem* item);

  /* Path edit trigger */
  void pathEditTrigger(EditorNPCPath* path);

  /* Sets the status bar to have the current tile hovered over */
  void setCurrentTile(int,int);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the current sub-map index */
  int getCurrentSubMap();

  /* Returns the map being edited */
  EditorMap* getMapEditor();

  /* Returns the Map Editor (center portion */
  MapRender* getMapEditorView();

  /* Sets the internal cursor mode */
  void setCursorMode(EditorEnumDb::CursorMode mode);

  /* Sets the map being edited */
  void setMapEditor(EditorMap* editor);

  /* Zooms the map view in or out */
  bool zoomIn();
  bool zoomOut();
};
#endif // MAPVIEW_H
