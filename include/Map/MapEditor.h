/*******************************************************************************
 * Class Name: MapEditor
 * Date Created: November 11, 2013
 * Inheritance: QGLWidget
 * Description: The map editor widget that gives a snapshot view of the window
 *              to make changes to the map from.
 ******************************************************************************/
#ifndef MAPEDITOR_H
#define MAPEDITOR_H

#include <QGLWidget>
#include <QTabWidget>

class MapEditor : public QGLWidget
{
  Q_OBJECT

public:
  /* Constructor function */
  MapEditor();

  /* Destructor function */
  ~MapEditor();

  /* Sets the sprite path */
  void setPath(QString sprite);

protected:
  void paintGL();
private:
  /* Map layer tabs */
  QTabWidget* layer_tabs;

  QString spritepath;
  /*------------------- Constants -----------------------*/
  //const static int kELEMENT_DATA;     /* Element data type for sprite */
};

#endif // MAPEDITOR_H
