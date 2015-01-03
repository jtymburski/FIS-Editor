/*******************************************************************************
 * Class Name: GameView
 * Date Created: December 22, 2014
 * Inheritance: QStackedWidget
 * Description: View of the game editor component for all game related data:
 *              maps,
 ******************************************************************************/
#include "View/GameView.h"
#include <QDebug>

/* Constructor function */
GameView::GameView(QWidget* parent) : QStackedWidget(parent)
{
  map_view = new MapView(this);
  addWidget(map_view);

  person_view = new QWidget(this);
  addWidget(person_view);

  party_view = new QWidget(this);
  addWidget(party_view);

  item_view = new QWidget(this);
  addWidget(item_view);

  action_view = new EditorAction(this);
  action_view->setDisabled(true);
  addWidget(action_view);
  
  race_view = new QWidget(this);
  addWidget(race_view);
  
  battleclass_view = new QWidget(this);
  addWidget(battleclass_view);
  
  skillset_view = new EditorSkillset(this);
  skillset_view->setDisabled(true);
  addWidget(skillset_view);
  
  skill_view = new EditorSkill(this);
  skill_view->setDisabled(true);
  addWidget(skill_view);
  
  equipment_view = new QWidget(this);
  addWidget(equipment_view);
  
  bubby_view = new QWidget(this);
  addWidget(bubby_view);
  
  person_view->setStyleSheet("background-color:red;");
  party_view->setStyleSheet("background-color:blue;");
  item_view->setStyleSheet("background-color:green;");
  //action_view->setStyleSheet("background-color:black;");
  race_view->setStyleSheet("background-color:purple;");
  battleclass_view->setStyleSheet("background-color:pink;");
  skillset_view->setStyleSheet("background-color:orange;");
  //skill_view->setStyleSheet("background-color:yellow;");
  equipment_view->setStyleSheet("background-color:violet;");
  bubby_view->setStyleSheet("background-color:black;");

  setViewMode(EditorEnumDb::MAPVIEW);
}

/* Destructor function */
GameView::~GameView()
{
  delete map_view;
}

/* Returns the MapView */
MapView* GameView::getMapView()
{
  return map_view;
}

/* Sets the map view */
void GameView::setMapView(MapView *view)
{

}

/* Returns the Editor Action View */
EditorAction* GameView::getActionView()
{
  return action_view;
}

/* Sets the Editor Action View */
void GameView::setActionView(EditorAction *action)
{
  disconnect(action_view,SIGNAL(nameChange(QString)),
          this,SIGNAL(nameChange(QString)));
  refreshView(EditorEnumDb::ACTIONVIEW, action_view, action);
  action_view = action;
  connect(action_view,SIGNAL(nameChange(QString)),
          this,SIGNAL(nameChange(QString)));
  action_view->getEditedAction();
}

/* Returns the Editor Skill View */
EditorSkill* GameView::getSkillView()
{
  return skill_view;
}

/* Sets the Editor Skill View */
void GameView::setSkillView(EditorSkill *skill)
{
  disconnect(skill_view,SIGNAL(nameChange(QString)),
          this,SIGNAL(nameChange(QString)));
  refreshView(EditorEnumDb::SKILLVIEW, skill_view, skill);
  skill_view = skill;
  connect(skill_view,SIGNAL(nameChange(QString)),
          this,SIGNAL(nameChange(QString)));
//  action_view->getEditedAction();
}

/* Returns the Editor Skillset View */
EditorSkillset* GameView::getSkillsetView()
{
  return skillset_view;
}

/* Sets the Editor Skillset View */
void GameView::setSkillsetView(EditorSkillset *skillset)
{
  disconnect(skillset_view,SIGNAL(nameChange(QString)),
          this,SIGNAL(nameChange(QString)));
  refreshView(EditorEnumDb::SKILLSETVIEW, skillset_view, skillset);
  skillset_view = skillset;
  connect(skillset_view,SIGNAL(nameChange(QString)),
          this,SIGNAL(nameChange(QString)));
//  action_view->getEditedAction();
}


/* Refresh view */
void GameView::refreshView(EditorEnumDb::ViewMode mode, QWidget *old,
                           QWidget *replacement)
{
  removeWidget(old);
  insertWidget(static_cast<int>(mode), replacement);
  setViewMode(mode);
}

/* Sets The View Mode */
void GameView::setViewMode(EditorEnumDb::ViewMode v)
{
  mode = v;
  setCurrentIndex(static_cast<int>(v));
}

void GameView::deactivateCurrentView()
{
  switch(mode)
  {
    case EditorEnumDb::MAPVIEW:
      map_view->setDisabled(true);
      break;
    case EditorEnumDb::PERSONVIEW:
      person_view->setDisabled(true);
      break;
    case EditorEnumDb::PARTYVIEW:
      party_view->setDisabled(true);
      break;
    case EditorEnumDb::ITEMVIEW:
      item_view->setDisabled(true);
      break;
    case EditorEnumDb::ACTIONVIEW:
      action_view->setDisabled(true);
      break;
    case EditorEnumDb::RACEVIEW:
      race_view->setDisabled(true);
      break;
    case EditorEnumDb::BATTLECLASSVIEW:
      battleclass_view->setDisabled(true);
      break;
    case EditorEnumDb::SKILLSETVIEW:
      skillset_view->setDisabled(true);
      break;
    case EditorEnumDb::SKILLVIEW:
      skill_view->setDisabled(true);
      break;
    case EditorEnumDb::EQUIPMENTVIEW:
      equipment_view->setDisabled(true);
      break;
    case EditorEnumDb::BUBBYVIEW:
      bubby_view->setDisabled(true);
      break;
    default:
      break;

  }
}
