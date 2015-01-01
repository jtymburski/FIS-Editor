/*******************************************************************************
 * Class Name: GameView
 * Date Created: December 22, 2014
 * Inheritance: QStackedWidget
 * Description: View of the game editor component for all game related data:
 *              maps,
 ******************************************************************************/
#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QStackedWidget>
#include "View/MapView.h"
#include "Database/GameDatabase.h"

class GameView : public QStackedWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  GameView(QWidget* parent = NULL);

  /* Destructor function */
  ~GameView();

  /* Refresh View */
  void refreshView(EditorEnumDb::ViewMode mode, QWidget* old,
                   QWidget* replacement);

private:
  /* The number of same images that trail this one */
//  int followers;
  MapView* map_view;
  //TODO : Delete the following temporary widgets
  QWidget* person_view;
  QWidget* party_view;
  QWidget* item_view;
  EditorAction* action_view;
  QWidget* race_view;
  QWidget* battleclass_view;
  QWidget* skillset_view;
  EditorSkill* skill_view;
  QWidget* equipment_view;
  QWidget* bubby_view;

  EditorEnumDb::ViewMode mode;

protected:
  /* Paints the sprite in a bounding box */
//  void paintEvent(QPaintEvent *);

public slots:
  /* Loads the image with a valid given path */
//  void loadSprite(QString path);

  /* View Get/Sets */
  MapView* getMapView();
  void setMapView(MapView* view);

  EditorAction* getActionView();
  void setActionView(EditorAction* sprite);

  EditorSkill* getSkillView();
  void setSkillView(EditorSkill* action);


  /* Sets The View Mode */
  void setViewMode(EditorEnumDb::ViewMode);

signals:
  /* Emits to parent when selected to deselect all others */
//  void chosen(int);
  void nameChange(QString);
};
#endif // GAMEVIEW_H
