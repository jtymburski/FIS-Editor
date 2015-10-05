/*******************************************************************************
 * Class Name: GameDatabase
 * Date Created: December 23, 2014
 * Inheritance: QWidget
 * Description: Far left view that determines what the game view will become.
 ******************************************************************************/
#ifndef GAMEDATABASE_H
#define GAMEDATABASE_H

#include <QInputDialog>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QProgressDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>

#include "Database/EditorAction.h"
#include "Database/EditorBubby.h"
#include "Database/EditorCategory.h"
#include "Database/EditorEquipment.h"
#include "Database/EditorItem.h"
#include "Database/EditorMap.h"
#include "Database/EditorParty.h"
#include "Database/EditorPerson.h"
#include "Database/EditorSkillset.h"
#include "Database/EditorSkill.h"
#include "Database/EditorSoundDb.h"
#include "EditorEnumDb.h"
#include "FileHandler.h"

class GameDatabase : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  GameDatabase(QWidget* parent = NULL);

  /* Destructor function */
  ~GameDatabase();

private:
  /* Buttons at bottom of database */
  QPushButton* button_delete;
  QPushButton* button_duplicate;
  QPushButton* button_import;
  QPushButton* button_new;

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
  EditorSoundDb* data_sounds;

  /* Layout */
  QVBoxLayout* layout;

  /* The map modifying dialog */
  QDialog* mapsize_dialog;

  /* Right click menu on bottom list */
  QMenu* rightclick_menu;

  /* The tile icons */
  TileIcons tile_icons;

  /* List Widgets */
  QListWidget* view_bottom;
  QListWidget* view_top;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Add object in the correct spot in the array */
  void addAction(EditorAction* action);
  void addClass(EditorCategory* cat_class);
  void addItem(EditorItem* item);
  void addParty(EditorParty* party);
  void addPerson(EditorPerson* person);
  void addRace(EditorCategory* cat_race);
  void addSkill(EditorSkill* skill);
  void addSkillSet(EditorSkillset* set);

  /* Change objects trigger call */
  void changeAction(int index, bool forced = false, bool save = false);
  void changeClass(int index, bool forced = false, bool save = false);
  void changeItem(int index, bool forced = false, bool save = false);
  void changeParty(int index, bool forced = false, bool save = false);
  void changePerson(int index, bool forced = false, bool save = false);
  void changeRace(int index, bool forced = false, bool save = false);
  void changeSkill(int index, bool forced = false, bool save = false);
  void changeSkillSet(int index, bool forced = false, bool save = false);

  /* Get object, based on ID */
  EditorAction* getAction(int id);
  EditorCategory* getClass(int id);
  EditorItem* getItem(int id);
  EditorParty* getParty(int id);
  EditorPerson* getPerson(int id);
  EditorCategory* getRace(int id);
  EditorSkill* getSkill(int id);
  EditorSkillset* getSkillSet(int id);

  /* Check if the core object is protected */
  bool isObjectProtected();

  /* Called to load object data */
  void loadAction(XmlData data, int index);
  void loadClass(XmlData data, int index);
  void loadItem(XmlData data, int index);
  void loadParty(XmlData data, int index);
  void loadPerson(XmlData data, int index);
  void loadRace(XmlData data, int index);
  void loadSkill(XmlData data, int index);
  void loadSkillSet(XmlData data, int index);

  /* Called upon load finish - for clean up */
  void loadFinish();

  /* Update calls for objects (to fill in information required from others) */
  void updateItems();
  void updateParties();
  void updatePersons();
  void updateSkills();
  void updateSkillSets();

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:

/*============================================================================
 * SIGNALS
 *===========================================================================*/
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

  /* Updated data to pass into map database */
  void updatedItems(QVector<QString> items);
  void updatedMaps(QVector<QString> maps);
  void updatedParties(QVector<QString> parties);

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* Creates a new map, from the dialog */
  void createNewMap();

  /* Creates a new resource -> new button */
  void createNewResource();

  /* Deletes an existing resource -> delete button */
  void deleteResource();

  /* Duplicates a selected resource -> duplicate */
  void duplicateResource();

  /* Imports a resource */
  void importResource();

  /* Right click list menu on bottom list */
  void listMenuRequested(const QPoint & pos);

  /* Triggers on double click of the bottom list -> updates view */
  void modifySelection(QListWidgetItem* item);

  /* Renames the selected map */
  void renameMap();

  /* Triggers on row change on top list -> shifts view */
  void rowChange(int);

  /* Triggered by save all button */
  void saveAll();

  /* Updates the bottom list -> when a name changes */
  void updateBottomListName(QString str);

  /* Updates event objects in the map database class */
  void updateEventObjects();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Create the starting point */
  void createStartObjects();

  /* Delete the game */
  void deleteAll();

  /* Get save count */
  int getSaveCount(bool selected_map = false, int sub_index = -1);

  /* Get current map */
  EditorMap* getCurrentMap();

  /* Returns the sound database, for connection */
  EditorSoundDb* getSoundDatabase();

  /* Load the game */
  void load(FileHandler* fh, QProgressDialog* dialog);

  /* Modifies the bottom list with the passed in index */
  void modifyBottomList(int index);

  /* Save the game */
  void save(FileHandler* fh, QProgressDialog* dialog, bool game_only = false,
            bool selected_map = false, int sub_index = -1);
};

#endif // GAMEDATABASE_H
