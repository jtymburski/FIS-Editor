/*******************************************************************************
 * Class Name: MapRender
 * Date Created: November 11, 2013
 * Inheritance: QGLWidget
 * Description: The map editor widget that gives a snapshot view of the window
 *              to make changes to the map from.
 ******************************************************************************/
#ifndef MAPRENDER_H
#define MAPRENDER_H

#include <QDebug>
#include <QGraphicsView>
#include <QTabWidget>
#include <QGraphicsScene>
#include <QList>
#include <QRect>
#include <QWidget>

#include "Database/EditorMap.h"
#include "Database/EditorNPCPath.h" // TESTING: REMOVE
#include "EnumDb.h"

class MapRender : public QGraphicsScene
{
  Q_OBJECT

public:
  /* Constructor function */
  MapRender(QWidget* parent = NULL);

  /* Destructor function */
  ~MapRender();

private:
  /* Active Hover tile */
  //EditorTile* active_tile;

  /* Block data */
  QPointF block_origin;
  bool block_erase;

  /* Cursor type */
  //EditorEnumDb::CursorMode cursor_mode;

  /* The editing map */
  EditorMap* editing_map;

  /* The rendering map */
  //SubMapInfo* map; // TODO: REMOVE??

  /* TESTING: REMOVE */
  EditorNPCPath* test_path;
  EditorNPCPath* test_path_2;

  /* In tile select mode */
  bool tile_select;

  /*------------------- Constants -----------------------*/
  //const static int kELEMENT_DATA;     /* Element data type for sprite */

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* General event processing */
  bool event(QEvent *event);

  /* Mouse move events */
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Sends the tile position to the map status bar */
  void sendCurrentPosition(int,int);

  /* Sends the selected tile to the thing pop-up */
  void sendSelectedTile(int id, int x, int y);

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* Select a tile trigger */
  void selectTile();

  /* Update the rendering sub-map */
  void updateRenderingMap();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the map being edited */
  EditorMap* getMapEditor();

  /* Returns the width and height */
  int getMapHeight();
  int getMapWidth();

  /* Sets the map being edited */
  void setMapEditor(EditorMap* editor);

  /* Update the entire scene */
  void updateAll();
};

#endif // MAPRENDER_H
