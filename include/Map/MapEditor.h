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
#include <QList>
#include <QWidget>
#include "TileWrapper.h"

class MapEditor : public QWidget
{
  Q_OBJECT

public:
  /* Constructor function */
  MapEditor(QWidget* parent = 0,int w = 100, int h = 100);

  /* Destructor function */
  ~MapEditor();

protected:
  void paintEvent(QPaintEvent *);
private:
  /* Map layer tabs */
  QTabWidget* layer_tabs;

  QList<QList<TileWrapper*> > tiles;
  int width;
  int height;
  /*------------------- Constants -----------------------*/
  //const static int kELEMENT_DATA;     /* Element data type for sprite */
};

#endif // MAPEDITOR_H
