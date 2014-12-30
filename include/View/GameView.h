/*******************************************************************************
 * Class Name: GameView
 * Date Created: December 22, 2014
 * Inheritance: QWidget
 * Description: View of the game editor component for all game related data:
 *              maps,
 ******************************************************************************/
#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QWidget>
#include "View/MapView.h"

class GameView : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  GameView(QWidget* parent = NULL);

  /* Destructor function */
  ~GameView();

private:
  /* The number of same images that trail this one */
//  int followers;
  MapView* map_view;
  //TODO : Delete the following temporary widgets
  QWidget* person_view;
  QWidget* party_view;
  QWidget* item_view;
  QWidget* action_view;
  QWidget* race_view;
  QWidget* battleclass_view;
  QWidget* skillset_view;
  QWidget* skill_view;
  QWidget* equipment_view;
  QWidget* bubby_view;

  EditorEnumDb::ViewMode mode;

protected:
  /* Paints the sprite in a bounding box */
//  void paintEvent(QPaintEvent *);

public slots:
  /* Loads the image with a valid given path */
//  void loadSprite(QString path);

  /* Returns the MapView */
  MapView* getMapView();

  /* Sets The View Mode */
  void setViewMode(EditorEnumDb::ViewMode);

signals:
  /* Emits to parent when selected to deselect all others */
//  void chosen(int);
};
#endif // GAMEVIEW_H
