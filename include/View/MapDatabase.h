/*******************************************************************************
 * Class Name: MapDatabase
 * Date Created: December 23, 2014
 * Inheritance: QWidget
 * Description: Database on the left hand side that contains all of the sprite
 * creation/selection and editing components.
 ******************************************************************************/
#ifndef MAPDATABASE_H
#define MAPDATABASE_H

#include <QListWidget>
#include <QWidget>

#include "View/RawImageView.h"
#include "View/SpriteView.h"

class MapDatabase : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  MapDatabase(QWidget* parent = NULL);

  /* Destructor function */
  ~MapDatabase();

private:
  /* Buttons for database control */
  QPushButton* button_delete;
  QPushButton* button_duplicate;
  QPushButton* button_import;
  QPushButton* button_new;

  /* Layout */
  QVBoxLayout* layout;

  /* The Views */
  RawImageView* view_raw;
  SpriteView* view_sprite;
  QListWidget* view_top;

protected:
  /* Paints the sprite in a bounding box */
//  void paintEvent(QPaintEvent *);

signals:
  /* Emits to parent when selected to deselect all others */
//  void chosen(int);

public slots:
  /* Button clicks */
  void buttonDelete();
  void buttonDuplicate();
  void buttonImport();
  void buttonNew();

  /* Updates based on selected index */
  void updateSelected(int index);

public:
  /* Returns the views */
  RawImageView* getRawView();
  SpriteView* getSpriteView();

  /* Sets the map editor */
  void setMapEditor(EditorMap* editing_map);
};

#endif // MAPDATABASE_H
