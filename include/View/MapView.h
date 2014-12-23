/*******************************************************************************
 * Class Name: MapView
 * Date Created: December 22, 2014
 * Inheritance: QMainWindow
 * Description: View of the map editor component for all map related data:
 *              map sprites, map things, etc.
 ******************************************************************************/
#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QGraphicsView>
#include <QMainWindow>
#include <QStatusBar>
#include "View/MapRender.h"

class MapView : public QMainWindow
{
  Q_OBJECT
public:
  /* Constructor Function */
  MapView(QWidget* parent = NULL);

  /* Destructor function */
  ~MapView();

private:
  /* Map View pointer */
  QGraphicsView* map_scroller;
  MapRender* map_editor;

  /* Map Data Bar */
  QStatusBar* map_data;

protected:
  /* Paints the sprite in a bounding box */
//  void paintEvent(QPaintEvent *);

public slots:
  /* Sets the status bar to have the current tile hovered over */
  void setCurrentTile(int x,int y);

signals:
  /* Emits to parent when selected to deselect all others */
//  void chosen(int);

public:
  /* Sets up the Map View */
  void setupMapView(SpriteView* toolbox, int x, int y,
                    EditorEnumDb::CursorMode cursor = EditorEnumDb::BASIC);
};
#endif // MAPVIEW_H
