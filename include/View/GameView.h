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

private:
  /* Blank views */
  EditorAction* null_action;
  EditorSounds* null_audio;
  EditorCategory* null_battleclass;
  QWidget* null_bubby;
  QWidget* null_equipment;
  EditorItem* null_item;
  EditorParty* null_party;
  EditorPerson* null_person;
  EditorCategory* null_race;
  EditorSkill* null_skill;
  EditorSkillset* null_skillset;

  /* The views, as they're set */
  EditorAction* view_action;
  EditorSounds* view_audio;
  EditorCategory* view_battleclass;
  QWidget* view_bubby;
  QWidget* view_equipment;
  EditorItem* view_item;
  MapView* view_map;
  EditorParty* view_party;
  EditorPerson* view_person;
  EditorCategory* view_race;
  EditorSkill* view_skill;
  EditorSkillset* view_skillset;

  EditorEnumDb::ViewMode mode;

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Paints the sprite in a bounding box */
//  void paintEvent(QPaintEvent *);

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Emits upon name change to game object */
  void nameChange(QString);

  /* Updated data to pass into map database */
  void updatedItems(QVector<QString> items);
  void updatedMaps(QVector<QString> maps);
  void updatedParties(QVector<QString> parties);

  /* Updates event objects in the map database class */
  void updateEventObjects();

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* View gets */
  EditorAction* getActionView();
  EditorCategory* getClassView();
  EditorItem* getItemView();
  MapView* getMapView();
  EditorParty* getPartyView();
  EditorPerson* getPersonView();
  EditorCategory* getRaceView();
  EditorSkill* getSkillView();
  EditorSkillset* getSkillsetView();

  /* View sets */
  void setActionView(EditorAction* sprite);
  void setClassView(EditorCategory* class_cat);
  void setItemView(EditorItem* item);
  void setPartyView(EditorParty* party);
  void setPersonView(EditorPerson* person);
  void setRaceView(EditorCategory* class_race);
  void setSkillView(EditorSkill* action);
  void setSkillsetView(EditorSkillset* action);

  /* Sets The View Mode */
  void setViewMode(EditorEnumDb::ViewMode);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Refresh View */
  void refreshView(EditorEnumDb::ViewMode mode, QWidget* old,
                   QWidget* replacement, bool change_view = true);

  /* Sets the permanent views */
  void setViewAudio(EditorSounds* view);
};
#endif // GAMEVIEW_H
