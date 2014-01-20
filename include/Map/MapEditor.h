/*******************************************************************************
 * Class Name: MapEditor
 * Date Created: November 11, 2013
 * Inheritance: QGLWidget
 * Description: The map editor widget that gives a snapshot view of the window
 *              to make changes to the map from.
 ******************************************************************************/
#ifndef MAPEDITOR_H
#define MAPEDITOR_H

#include <QTabWidget>
#include <QGraphicsScene>
#include <QList>
#include <QWidget>
#include "TileWrapper.h"

class MapEditor : public QGraphicsScene
{
  Q_OBJECT

public:
  /* Constructor function */
  MapEditor(EditorSpriteToolbox* toolbox = 0,
            QWidget* parent = 0,int w = 100,
            int h = 100, EditorEnumDb::CursorMode cursor = EditorEnumDb::BASIC);

  /* Destructor function */
  ~MapEditor();

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

  /* Function to set the current layer for editing */
  void setEditingLayer(EditorEnumDb::Layer);

  /* removes the current sprite from the active layer */
  void removeCurrent();

  /* Sets the cursor mode */
  void setCursorMode(EditorEnumDb::CursorMode mode);

  /* Returns the width and height */
  int getMapWidth();
  int getMapHeight();

signals:
  /* Sends the tile position to the map status bar */
  void sendCurrentPosition(int,int);

protected:
  /* Paint event */
  void paintEvent(QPaintEvent *);
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

private:
  /* Boolean values for determining which layers are shown */
  bool base,enhancer;
  bool lower1,lower2,lower3,lower4,lower5;
  bool upper1,upper2,upper3,upper4,upper5;

  /* Right click menu */
  QMenu* rightclick_menu;
  QAction* remove_action;

  /* Map layer tabs */
  QTabWidget* layer_tabs;

  /* The list of tiles */
  QList<QList<TileWrapper*> > tiles;

  /* Dimensions of the map */
  int width;
  int height;

  /* Cursor type */
  EditorEnumDb::CursorMode cursormode;

  /* Position of current (Only used for removeCurrent() */
  QPointF current_position;
  /*------------------- Constants -----------------------*/
  //const static int kELEMENT_DATA;     /* Element data type for sprite */
};

#endif // MAPEDITOR_H
