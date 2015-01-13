/*******************************************************************************
 * Class Name: GameDatabase
 * Date Created: December 23, 2014
 * Inheritance: QWidget
 * Description: Far left view that determines what the game view will become.
 ******************************************************************************/
#ifndef GAMEDATABASE_H
#define GAMEDATABASE_H

#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QVector>
#include <QPushButton>
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

  /* Currently selected object */
  EditorAction* current_action;
  EditorCategory* current_battleclass;
  EditorBubby* current_bubby;
  EditorEquipment* current_equipment;
  EditorItem* current_item;
  EditorMap* current_map;
  EditorParty* current_party;
  EditorPerson* current_person;
  EditorCategory* current_race;
  EditorSkill* current_skill;
  EditorSkillset* current_skillset;

  /* Vector for actual data in bottom list, populated from game xml data */
  QVector<EditorAction*> data_action;
  QVector<EditorCategory*> data_battleclass;
  QVector<EditorBubby*> data_bubby;
  QVector<EditorEquipment*> data_equipment;
  QVector<EditorItem*> data_item;
  QVector<EditorMap*> data_map;
  QVector<EditorParty*> data_party;
  QVector<EditorPerson*> data_person;
  QVector<EditorCategory*> data_race;
  QVector<EditorSkill*> data_skill;
  QVector<EditorSkillset*> data_skillset;

  /* The map modifying dialog */
  QDialog* mapsize_dialog;

  /* Buttons at bottom of database */
  QPushButton* new_button;
  QPushButton* del_button;
  QPushButton* import_button;
  QPushButton* duplicate_button;

protected:
public slots:
  /* Creates a new map, from the dialog */
  void createNewMap();

  /* Creates a new resource -> new button */
  void createNewResource();

  /* Deletes an existing resource -> delete button */
  void deleteResource();

  /* Duplicates a selected resource -> duplicate */
  void duplicateResource();

  /* Triggers on double click of the bottom list -> updates view */
  void modifySelection(QListWidgetItem* item);

  /* Triggers on row change on top list -> shifts view */
  void rowChange(int);

  /* Updates the bottom list -> when a name changes */
  void updateBottomListName(QString str);

signals:
  void changeAction(EditorAction* action);
  void changeBattleclass(EditorCategory* battle_class);
  void changeBubby(EditorBubby* bubby);
  void changeEquipment(EditorEquipment* equipment);
  void changeItem(EditorItem* item);
  void changeMap(EditorMap* map);
  void changeMode(EditorEnumDb::ViewMode);
  void changeParty(EditorParty* party);
  void changePerson(EditorPerson* person);
  void changeRace(EditorCategory* race);
  void changeSkill(EditorSkill* skill);
  void changeSkillset(EditorSkillset* skill_set);

public:
  /* Modifies the bottom list with the passed in index */
  void modifyBottomList(int index);
};

#endif // GAMEDATABASE_H
