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
  connect(this, SIGNAL(soundFill(QVector<QString>)),
          view_map, SLOT(soundFill(QVector<QString>)));
  connect(view_map, SIGNAL(soundFillTrigger()),
          this, SIGNAL(soundFillTrigger()));
  connect(view_map, SIGNAL(updateEventObjects()),
          this, SIGNAL(updateEventObjects()));
  connect(this, SIGNAL(updatedItems(QVector<QString>)),
          view_map, SLOT(updatedItems(QVector<QString>)));
  connect(this, SIGNAL(updatedMaps(QVector<QString>)),
          view_map, SLOT(updatedMaps(QVector<QString>)));
  connect(this, SIGNAL(updatedParties(QVector<QString>)),
          view_map, SLOT(updatedParties(QVector<QString>)));

  QWidget* blank_widget1 = new QWidget(this);
  addWidget(blank_widget1);

  view_party = new EditorParty(this);
  view_party->setDisabled(true);
  null_party = view_party;
  addWidget(view_party);

  view_person = new EditorPerson(this);
  view_person->setDisabled(true);
  null_person = view_person;
  addWidget(view_person);

  view_bubby = new QWidget(this);
  null_bubby = view_bubby;
  addWidget(view_bubby);

  view_equipment = new QWidget(this);
  null_equipment = view_equipment;
  addWidget(view_equipment);

  view_item = new EditorItem(this);
  view_item->setDisabled(true);
  null_item = view_item;
  addWidget(view_item);

  view_battleclass = new EditorCategory(this);
  view_battleclass->setDisabled(true);
  null_battleclass = view_battleclass;
  addWidget(view_battleclass);

  view_race = new EditorCategory(this);
  view_race->setDisabled(true);
  null_race = view_race;
  addWidget(view_race);
  
  view_skillset = new EditorSkillset(this);
  view_skillset->setDisabled(true);
  null_skillset = view_skillset;
  addWidget(view_skillset);
  
  view_skill = new EditorSkill(this);
  view_skill->setDisabled(true);
  null_skill = view_skill;
  addWidget(view_skill);
  
  view_action = new EditorAction(this);
  view_action->setDisabled(true);
  null_action = view_action;
  addWidget(view_action);

  QWidget* blank_widget2 = new QWidget(this);
  addWidget(blank_widget2);

  view_sounds = new EditorSoundDb(this);
  view_sounds->setDisabled(true);
  null_sounds = view_sounds;
  addWidget(view_sounds);

  /* Styling, as required */
  blank_widget1->setStyleSheet("background-color:black;");
  blank_widget2->setStyleSheet("background-color:black;");
  view_equipment->setStyleSheet("background-color:black;");
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

/* Returns the Editor Action View */
EditorAction* GameView::getActionView()
{
  return view_action;
}

/* Returns the Editor Class Category View */
EditorCategory* GameView::getClassView()
{
  return view_battleclass;
}

/* Returns the Editor Item View */
EditorItem* GameView::getItemView()
{
  return view_item;
}

/* Returns the MapView */
MapView* GameView::getMapView()
{
  return view_map;
}

/* Returns the Editor Party View */
EditorParty* GameView::getPartyView()
{
  return view_party;
}

/* Returns the Editor Person View */
EditorPerson* GameView::getPersonView()
{
  return view_person;
}

/* Returns the Editor Race Category View */
EditorCategory* GameView::getRaceView()
{
  return view_race;
}

/* Returns the Editor Skill View */
EditorSkill* GameView::getSkillView()
{
  return view_skill;
}

/* Returns the Editor Skillset View */
EditorSkillset* GameView::getSkillsetView()
{
  return view_skillset;
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
}

/* Sets the Editor Class Category View */
void GameView::setClassView(EditorCategory* class_cat)
{
  /* Disconnect the old view */
  disconnect(view_battleclass, SIGNAL(nameChange(QString)),
             this, SIGNAL(nameChange(QString)));

  /* Set up the new view */
  if(class_cat == NULL)
    class_cat = null_battleclass;
  refreshView(EditorEnumDb::BATTLECLASSVIEW, view_battleclass, class_cat);
  view_battleclass = class_cat;
  connect(view_battleclass, SIGNAL(nameChange(QString)),
          this, SIGNAL(nameChange(QString)));
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

/* Sets the Editor Party View */
void GameView::setPartyView(EditorParty* party)
{
  /* Disconnect the old view */
  disconnect(view_party, SIGNAL(nameChange(QString)),
             this, SIGNAL(nameChange(QString)));

  /* Set up the new view */
  if(party == NULL)
    party = null_party;
  refreshView(EditorEnumDb::PARTYVIEW, view_party, party);
  view_party = party;
  connect(view_party, SIGNAL(nameChange(QString)),
          this, SIGNAL(nameChange(QString)));
}

/* Sets the Editor Person View */
void GameView::setPersonView(EditorPerson* person)
{
  /* Disconnect the old view */
  disconnect(view_person, SIGNAL(nameChange(QString)),
             this, SIGNAL(nameChange(QString)));

  /* Set up the new view */
  if(person == NULL)
    person = null_person;
  refreshView(EditorEnumDb::PERSONVIEW, view_person, person);
  view_person = person;
  connect(view_person, SIGNAL(nameChange(QString)),
          this, SIGNAL(nameChange(QString)));
}

/* Sets the Editor Race Category View */
void GameView::setRaceView(EditorCategory* class_race)
{
  /* Disconnect the old view */
  disconnect(view_race, SIGNAL(nameChange(QString)),
             this, SIGNAL(nameChange(QString)));

  /* Set up the new view */
  if(class_race == NULL)
    class_race = null_race;
  refreshView(EditorEnumDb::RACEVIEW, view_race, class_race);
  view_race = class_race;
  connect(view_race, SIGNAL(nameChange(QString)),
          this, SIGNAL(nameChange(QString)));
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

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Refresh view */
void GameView::refreshView(EditorEnumDb::ViewMode mode, QWidget *old,
                           QWidget *replacement, bool change_view)
{
  removeWidget(old);
  insertWidget(static_cast<int>(mode), replacement);
  if(change_view)
    setViewMode(mode);
}

/* Sets the permanent views */
void GameView::setViewSounds(EditorSoundDb* view)
{
  refreshView(EditorEnumDb::AUDIOVIEW, view_sounds, view, false);
  view_sounds = view;
}
