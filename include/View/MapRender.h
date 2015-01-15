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
#include "View/TileRender.h"

class MapRender : public QGraphicsScene
{
  Q_OBJECT

public:
  /* Constructor function */
  MapRender(SpriteView* toolbox = NULL,
            QWidget* parent = NULL,
            EditorEnumDb::CursorMode cursor = EditorEnumDb::BASIC);

  /* Destructor function */
  ~MapRender();

private:
  /* Boolean values for determining which layers are shown */
  bool base, enhancer;
  bool lower1, lower2, lower3, lower4, lower5;
  bool upper1, upper2, upper3, upper4, upper5;

  /* Right click menu */
  QMenu* rightclick_menu;
  QAction* remove_action;
  QAction* setNpass_action;
  QAction* setEpass_action;
  QAction* setSpass_action;
  QAction* setWpass_action;
  QAction* setApass_action;

  /* Map layer tabs */
  QTabWidget* layer_tabs;

  /* The list of tiles */
  QList<QList<TileRender*> > tiles;

  /* Rubber Band */
  QRubberBand* highlight;
  QPoint origin;

  /* Dimensions of the map */
  int width;
  int height;

  /* Block Select Startpoint */
  int blockx, blocky;
  bool blockmodepress;
  bool eraseblock;

  /* Cursor type */
  EditorEnumDb::CursorMode cursormode;

  /* Position of current (Only used for removeCurrent() */
  QPointF current_position;
  QPointF up_position;
  QPointF down_position;
  QPointF left_position;
  QPointF right_position;

  /* ---- NEW DATA ---- */

  /* The rendering map */
  SubMapInfo* map;

  /*------------------- Constants -----------------------*/
  //const static int kELEMENT_DATA;     /* Element data type for sprite */

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Paint event */
  void paintEvent(QPaintEvent *);
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Sends the tile position to the map status bar */
  void sendCurrentPosition(int,int);

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* Functions to toggle each layer to be shown */
  void toggleBase(bool);
  void toggleEnhancer(bool);
  void toggleLower1(bool);
  void toggleLower2(bool);
  void toggleLower3(bool);
  void toggleLower4(bool);
  void toggleLower5(bool);
  void toggleUpper1(bool);
  void toggleUpper2(bool);
  void toggleUpper3(bool);
  void toggleUpper4(bool);
  void toggleUpper5(bool);

  /* Function to toggle the grid */
  void toggleGrid(bool);
  void togglePass(bool);

  /* Function to set the current layer for editing */
  void setEditingLayer(EditorEnumDb::Layer);

  /* removes the current sprite from the active layer */
  void removeCurrent();
  void setNPassCurrent(bool);
  void setEPassCurrent(bool);
  void setSPassCurrent(bool);
  void setWPassCurrent(bool);
  void setAPassCurrent(bool);

  /* Sets the cursor mode */
  void setCursorMode(EditorEnumDb::CursorMode mode);

  /* Returns the width and height */
  int getMapWidth();
  int getMapHeight();

  /* Recursively fills areas with tiles */
  void recursiveFill(int x, int y, int target, int replace);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the rendering sub-map */
  SubMapInfo* getRenderingMap();

  /* Sets the rendering sub-map */
  void setRenderingMap(SubMapInfo* map);
};

#endif // MAPRENDER_H
