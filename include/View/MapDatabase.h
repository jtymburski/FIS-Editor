/*******************************************************************************
 * Class Name: MapDatabase
 * Date Created: December 23, 2014
 * Inheritance: QWidget
 * Description: Database on the left hand side that contains all of the sprite
 * creation/selection and editing components.
 ******************************************************************************/
#ifndef MAPDATABASE_H
#define MAPDATABASE_H

#include <QTabWidget>
#include "View/RawImageView.h"
#include "View/SpriteView.h"

class MapDatabase : public QTabWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  MapDatabase(QWidget* parent = NULL);

  /* Destructor function */
  ~MapDatabase();

private:
  RawImageView* raw_view;
  SpriteView* sprite_view;

protected:
  /* Paints the sprite in a bounding box */
//  void paintEvent(QPaintEvent *);

public slots:
  /* Loads the image with a valid given path */
//  void loadSprite(QString path);

  /* Returns the views */
  RawImageView* getRawView();
  SpriteView* getSpriteView();

signals:
  /* Emits to parent when selected to deselect all others */
//  void chosen(int);
};

#endif // MAPDATABASE_H
