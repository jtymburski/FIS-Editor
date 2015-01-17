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
#include "EnumDb.h"

class MapRender : public QGraphicsScene
{
  Q_OBJECT

public:
  /* Constructor function */
  MapRender(QWidget* parent = NULL,
            EditorEnumDb::CursorMode cursor = EditorEnumDb::BASIC);

  /* Destructor function */
  ~MapRender();

private:
  /* Active Hover tile */
  EditorTile* active_tile;

  /* Block data */
  QPointF block_origin;
  bool block_erase;

  /* Cursor type */
  EditorEnumDb::CursorMode cursor_mode;

  /* The rendering map */
  SubMapInfo* map;

  /*------------------- Constants -----------------------*/
  //const static int kELEMENT_DATA;     /* Element data type for sprite */

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Mouse move events */
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Adds item, as dictated by parent classes, to the tile */
  void itemClick(EditorTile* tile); // GOOD
  void itemMassClick(QList<EditorTile*> tiles, bool erase);

  /* Sets and unsets passability on tile */
  void passSet(EditorTile* tile);
  void passUnset(EditorTile* tile);

  /* Sends the tile position to the map status bar */
  void sendCurrentPosition(int,int);

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* Sets the rendering sub-map */
  void setRenderingMap(SubMapInfo* map);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the active tile */
  EditorTile* getActiveTile();

  /* Returns the width and height */
  int getMapHeight();
  int getMapWidth();

  /* Returns the rendering sub-map */
  SubMapInfo* getRenderingMap();

  /* Sets the cursor mode */
  void setCursorMode(EditorEnumDb::CursorMode mode);

  /* Update the entire scene */
  void updateAll();
};

#endif // MAPRENDER_H
