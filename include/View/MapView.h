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
#include "Dialog/EventDialog.h"
#include "EditorEnumDb.h"
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
  MapView(QWidget* parent = nullptr);

  /* Destructor function */
  ~MapView();

private:
  /* Cursor Mode */
  EditorEnumDb::CursorMode cursor_mode;

  /* Is the data for database, or instead view */
  bool data_db;

  /* The editing map */
  EditorMap* editing_map;

  /* The event set dialog */
  EventDialog* event_dialog;

  /* Event control */
  bool event_enter;
  bool event_exit;
  bool event_external;
  EditorTile* event_tile;

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
  /* Fill event pop-up with map data */
  void fillEventWithData();

  /* Sets up the database bar */
  void setupLeftBar();

  /* Sets up the Map View - center portion */
  void setupMapView();

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

  /* Signal for layer changing */
  void layerChanged(EditorEnumDb::Layer layer);

  /* Passes the editor sprite up */
  void sendUpEditorSprite(EditorSprite* e);

  /* Updates event objects in the map database or view class */
  void updateEventObjects();
  void updateMusicObjects();
  void updateSoundObjects();

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* Event button triggers */
  void buttonEventCancel();
  void buttonEventOk();

  /* Edit event set trigger */
  void editEventSet(EditorEventSet* set, QString window_title = "");

  /* Ensures the following item is visible in scene */
  void ensureVisible(QGraphicsItem* item);

  /* Path edit trigger */
  void pathEditTrigger(EditorNPCPath* path);

  /* Select a tile trigger */
  //void selectTileConvo(); // TODO: Remove?
  void selectTileDb();
  void selectTileEvent();

  /* Send the selected tile to the event controller */
  void sendSelectedTile(int, int, int);

  /* Sets the status bar to have the current tile hovered over */
  void setCurrentTile(int,int);

  /* Tile enter/exit event slots */
  void tileEventEnter();
  void tileEventExit();

  /* Updates event/sound objects in the map database */
  void updateEventObjectsDb();
  void updateSoundObjectsDb();

  /* Updated data to pass into map database */
  void updatedItems(QVector<QString> items);
  void updatedMaps(QVector<QString> maps);
  void updatedMusic(QList<QString> music_list);
  void updatedParties(QVector<QString> parties);
  void updatedSounds(QList<QString> sound_list);

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
