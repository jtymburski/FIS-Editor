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
            QWidget* parent = 0,int w = 100, int h = 100);

  /* Destructor function */
  ~MapEditor();

public slots:
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
  void toggleGrid(bool);
  void setEditingLayer(EditorEnumDb::Layer);

protected:
  void paintEvent(QPaintEvent *);
private:
  bool base,enhancer;
  bool lower1,lower2,lower3,lower4,lower5;
  bool upper1,upper2,upper3,upper4,upper5;
  /* Map layer tabs */
  QTabWidget* layer_tabs;

  QList<QList<TileWrapper*> > tiles;
  int width;
  int height;
  /*------------------- Constants -----------------------*/
  //const static int kELEMENT_DATA;     /* Element data type for sprite */
};

#endif // MAPEDITOR_H
