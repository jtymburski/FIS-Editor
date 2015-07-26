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
  view_map = new MapView(this);
  addWidget(view_map);
  connect(view_map, SIGNAL(updateEventObjects()),
          this, SIGNAL(updateEventObjects()));
  connect(this, SIGNAL(updatedItems(QVector<QString>)),
          view_map, SLOT(updatedItems(QVector<QString>)));
  connect(this, SIGNAL(updatedMaps(QVector<QString>)),
          view_map, SLOT(updatedMaps(QVector<QString>)));

  view_person = new QWidget(this);
  null_person = view_person;
  addWidget(view_person);

  view_party = new QWidget(this);
  null_party = view_party;
  addWidget(view_party);

  view_item = new EditorItem(this);
  view_item->setDisabled(true);
  null_item = view_item;
  addWidget(view_item);

  view_action = new EditorAction(this);
  view_action->setDisabled(true);
  null_action = view_action;
  addWidget(view_action);
  
  view_race = new QWidget(this);
  null_race = view_race;
  addWidget(view_race);
  
  view_battleclass = new QWidget(this);
  null_battleclass = view_battleclass;
  addWidget(view_battleclass);
  
  view_skillset = new EditorSkillset(this);
  view_skillset->setDisabled(true);
  null_skillset = view_skillset;
  addWidget(view_skillset);
  
  view_skill = new EditorSkill(this);
  view_skill->setDisabled(true);
  null_skill = view_skill;
  addWidget(view_skill);
  
  view_equipment = new QWidget(this);
  null_equipment = view_equipment;
  addWidget(view_equipment);
  
  view_bubby = new QWidget(this);
  null_bubby = view_bubby;
  addWidget(view_bubby);
  
  view_person->setStyleSheet("background-color:red;");
  view_party->setStyleSheet("background-color:blue;");
  //view_item->setStyleSheet("background-color:green;");
  //view_action->setStyleSheet("background-color:black;");
  view_race->setStyleSheet("background-color:purple;");
  view_battleclass->setStyleSheet("background-color:pink;");
  //view_skillset->setStyleSheet("background-color:orange;");
  //view_skill->setStyleSheet("background-color:yellow;");
  view_equipment->setStyleSheet("background-color:violet;");
  view_bubby->setStyleSheet("background-color:black;");

  setViewMode(EditorEnumDb::MAPVIEW);
}

/* Destructor function */
GameView::~GameView()
{
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

/* Returns the MapView */
MapView* GameView::getMapView()
{
  return view_map;
}

/* Sets the map view */
//void GameView::setMapView(MapView *view)
//{
//
//}

/* Returns the Editor Action View */
EditorAction* GameView::getActionView()
{
  return view_action;
}

/* Sets the Editor Action View */
void GameView::setActionView(EditorAction *action)
{
  /* Disconnect the old view */
  disconnect(view_action,SIGNAL(nameChange(QString)),
          this,SIGNAL(nameChange(QString)));
  if(action == NULL)
    action = null_action;

  /* Set up the new view */
  refreshView(EditorEnumDb::ACTIONVIEW, view_action, action);
  view_action = action;
  connect(view_action,SIGNAL(nameChange(QString)),
          this,SIGNAL(nameChange(QString)));
  //view_action->getEditedAction();
}

/* Returns the Editor Skill View */
EditorSkill* GameView::getSkillView()
{
  return view_skill;
}

/* Sets the Editor Skill View */
void GameView::setSkillView(EditorSkill *skill)
{
  /* Disconnect the old view */
  disconnect(view_skill,SIGNAL(nameChange(QString)),
          this,SIGNAL(nameChange(QString)));
  if(skill == NULL)
    skill = null_skill;

  /* Set up the new view */
  refreshView(EditorEnumDb::SKILLVIEW, view_skill, skill);
  view_skill = skill;
  connect(view_skill,SIGNAL(nameChange(QString)),
          this,SIGNAL(nameChange(QString)));
}

/* Returns the Editor Item View */
EditorItem* GameView::getItemView()
{
  return view_item;
}

/* Sets the Editor Skill View */
void GameView::setItemView(EditorItem *item)
{
  /* Disconnect the old view */
  disconnect(view_item,SIGNAL(nameChange(QString)),
          this,SIGNAL(nameChange(QString)));
  if(item == NULL)
    item = null_item;

  /* Set up the new view */
  refreshView(EditorEnumDb::ITEMVIEW, view_item, item);
  view_item = item;
  connect(view_item,SIGNAL(nameChange(QString)),
          this,SIGNAL(nameChange(QString)));
}


/* Returns the Editor Skillset View */
EditorSkillset* GameView::getSkillsetView()
{
  return view_skillset;
}


/* Sets the Editor Skillset View */
void GameView::setSkillsetView(EditorSkillset *skillset)
{
  /* Disconnect the old skillset view */
  disconnect(view_skillset,SIGNAL(nameChange(QString)),
          this,SIGNAL(nameChange(QString)));
  if(skillset == NULL)
    skillset = null_skillset;

  /* Set up the new view */
  refreshView(EditorEnumDb::SKILLSETVIEW, view_skillset, skillset);
  view_skillset = skillset;
  connect(view_skillset,SIGNAL(nameChange(QString)),
          this,SIGNAL(nameChange(QString)));
//  view_action->getEditedAction();
}

/* Sets The View Mode */
void GameView::setViewMode(EditorEnumDb::ViewMode v)
{
  mode = v;
  setCurrentIndex(static_cast<int>(v));
}

/* Updates event objects in the map database or view class */
// TODO: Comment
void GameView::updateEventObjectsDb()
{

}

/* Updates event objects in the map database or view class */
// TODO: Comment
void GameView::updateEventObjectsView()
{

}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Refresh view */
void GameView::refreshView(EditorEnumDb::ViewMode mode, QWidget *old,
                           QWidget *replacement)
{
  removeWidget(old);
  insertWidget(static_cast<int>(mode), replacement);
  setViewMode(mode);
}
