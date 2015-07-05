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
#include "Database/EditorSkillset.h"
#include "Database/GameDatabase.h"

class GameView : public QStackedWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  GameView(QWidget* parent = NULL);

  /* Destructor function */
  ~GameView();

private:
  /* Blank views */
  EditorAction* null_action;
  QWidget* null_battleclass;
  QWidget* null_bubby;
  QWidget* null_equipment;
  EditorItem* null_item;
  QWidget* null_party;
  QWidget* null_person;
  QWidget* null_race;
  EditorSkill* null_skill;
  EditorSkillset* null_skillset;

  /* The views, as they're set */
  EditorAction* view_action;
  QWidget* view_battleclass;
  QWidget* view_bubby;
  QWidget* view_equipment;
  EditorItem* view_item;
  MapView* view_map;
  QWidget* view_party;
  QWidget* view_person;
  QWidget* view_race;
  EditorSkill* view_skill;
  EditorSkillset* view_skillset;

  EditorEnumDb::ViewMode mode;

protected:
  /* Paints the sprite in a bounding box */
//  void paintEvent(QPaintEvent *);

signals:
  /* Emits upon name change to game object */
  void nameChange(QString);

  /* Updated data to pass into map database */
  void updatedItems(QVector<QString> items);
  void updatedMaps(QVector<QString> maps);

  /* Updates event objects in the map database class */
  void updateEventObjects();

public slots:
  /* Loads the image with a valid given path */
//  void loadSprite(QString path);

  /* View Get/Sets */
  MapView* getMapView();
  //void setMapView(MapView* view);

  EditorAction* getActionView();
  void setActionView(EditorAction* sprite);

  EditorSkill* getSkillView();
  void setSkillView(EditorSkill* action);

  EditorSkillset* getSkillsetView();
  void setSkillsetView(EditorSkillset* action);

  EditorItem* getItemView();
  void setItemView(EditorItem* item);

  /* Sets The View Mode */
  void setViewMode(EditorEnumDb::ViewMode);

  //void deactivateCurrentView();

public:
  /* Refresh View */
  void refreshView(EditorEnumDb::ViewMode mode, QWidget* old,
                   QWidget* replacement);
};
#endif // GAMEVIEW_H
