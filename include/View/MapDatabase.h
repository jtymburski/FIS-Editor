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
  /* The Views */
  RawImageView* raw_view;
  SpriteView* sprite_view;
  QListWidget* top_view;

  /* Layout */
  QVBoxLayout* layout;

protected:
  /* Paints the sprite in a bounding box */
//  void paintEvent(QPaintEvent *);

public slots:
  /* Updates based on selected index */
  void updateSelected(int index);

signals:
  /* Emits to parent when selected to deselect all others */
//  void chosen(int);

public:
  /* Returns the views */
  RawImageView* getRawView();
  SpriteView* getSpriteView();
};

#endif // MAPDATABASE_H
