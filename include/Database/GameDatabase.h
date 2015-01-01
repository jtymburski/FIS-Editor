#ifndef GAMEDATABASE_H
#define GAMEDATABASE_H
/*******************************************************************************
 * Class Name: GameDatabase
 * Date Created: December 23, 2014
 * Inheritance: QWidget
 * Description: Far left view that determines what the game view will become.
 ******************************************************************************/
#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QVector>
#include <QPushButton>
#include <QPair>
#include <QLabel>
#include "EditorEnumDb.h"
#include "Database/EditorMap.h"
#include "Database/EditorPerson.h"
#include "Database/EditorParty.h"
#include "Database/EditorItem.h"
#include "Database/EditorAction.h"
#include "Database/EditorCategory.h"
#include "Database/EditorSkillset.h"
#include "Database/EditorSkill.h"
#include "Database/EditorEquipment.h"
#include "Database/EditorBubby.h"

class GameDatabase : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  GameDatabase(QWidget* parent = NULL);

  /* Destructor function */
  ~GameDatabase();

private:
  /* List Widgets */
  QListWidget* top_view;
  QListWidget* bottom_view;

  /* Layout */
  QVBoxLayout* layout;

  /* Vector for actual data in bottom list, populated from game xml data */
  QVector<QPair<QString,EditorMap*>* > map_pair;
  QVector<QPair<QString,EditorPerson*>* > person_pair;
  QVector<QPair<QString,EditorParty*>* > party_pair;
  QVector<QPair<QString,EditorItem*>* > item_pair;
  QVector<QPair<QString,EditorAction*>* > action_pair;
  QVector<QPair<QString,EditorCategory*>* > race_pair;
  QVector<QPair<QString,EditorCategory*>* > battleclass_pair;
  QVector<QPair<QString,EditorSkillset*>* > skillset_pair;
  QVector<QPair<QString,EditorSkill*>* > skill_pair;
  QVector<QPair<QString,EditorEquipment*>* > equipment_pair;
  QVector<QPair<QString,EditorBubby*>* > bubby_pair;

  /* Currently selected object */
  QPair<QString,EditorMap*>* current_map;
  QPair<QString,EditorPerson*>* current_person;
  QPair<QString,EditorParty*>* current_party;
  QPair<QString,EditorItem*>* current_item;
  QPair<QString,EditorAction*>* current_action;
  QPair<QString,EditorCategory*>* current_race;
  QPair<QString,EditorCategory*>* current_battleclass;
  QPair<QString,EditorSkillset*>* current_skillset;
  QPair<QString,EditorSkill*>* current_skill;
  QPair<QString,EditorEquipment*>* current_equipment;
  QPair<QString,EditorBubby*>* current_bubby;

  int current_map_index;
  int current_person_index;
  int current_party_index;
  int current_item_index;
  int current_action_index;
  int current_race_index;
  int current_battleclass_index;
  int current_skillset_index;
  int current_skill_index;
  int current_equipment_index;
  int current_bubby_index;

  int current_map_id;
  int current_person_id;
  int current_party_id;
  int current_item_id;
  int current_action_id;
  int current_race_id;
  int current_battleclass_id;
  int current_skillset_id;
  int current_skill_id;
  int current_equipment_id;
  int current_bubby_id;

  int current_map_selection;
  int current_person_selection;
  int current_party_selection;
  int current_item_selection;
  int current_action_selection;
  int current_race_selection;
  int current_battleclass_selection;
  int current_skillset_selection;
  int current_skill_selection;
  int current_equipment_selection;
  int current_bubby_selection;


  QPushButton* new_button;
  QPushButton* del_button;
  QPushButton* import_button;
  QPushButton* duplicate_button;

  QString* current_name;

protected:
public slots:
  void modifyBottomList(int index);
  void modifySelection(QModelIndex index);
  void modifyIndex(int);
  void updateBottomListName(QString str);
  void rowChange(int);

  void createNewResource();
  void duplicateResource();
  void deleteResource();

signals:
  void changeMode(EditorEnumDb::ViewMode);
  void changeMap(QPair<QString,EditorMap*>*);
  void changePerson(QPair<QString,EditorPerson*>*);
  void changeParty(QPair<QString,EditorParty*>*);
  void changeItem(QPair<QString,EditorItem*>*);
  void changeAction(QPair<QString,EditorAction*>*);
  void changeRace(QPair<QString,EditorCategory*>*);
  void changeBattleclass(QPair<QString,EditorCategory*>*);
  void changeSkillset(QPair<QString,EditorSkillset*>*);
  void changeSkill(QPair<QString,EditorSkill*>*);
  void changeEquipment(QPair<QString,EditorEquipment*>*);
  void changeBubby(QPair<QString,EditorBubby*>*);
};
#endif // GAMEDATABASE_H
