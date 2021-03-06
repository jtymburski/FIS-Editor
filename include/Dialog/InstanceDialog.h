/*******************************************************************************
 * Class Name: InstanceDialog
 * Date Created: April 1, 2015
 * Inheritance: QDialog
 * Description: The dialog that displays the map thing instant and the
 *              information related to it. Allows manipulation of the data.
 *              It also contains the path information used for map npcs.
 ******************************************************************************/
#ifndef INSTANCEDIALOG_H
#define INSTANCEDIALOG_H

#include <QCheckBox>
#include <QCloseEvent>
#include <QDialog>
#include <QGridLayout>
#include <QListWidget>
#include <QMessageBox>

#include "Database/EditorMapIO.h"
#include "Database/EditorMapItem.h"
#include "Database/EditorMapNPC.h"
#include "Dialog/EventDialog.h"
#include "Dialog/NodeDialog.h"
#include "View/EventSetView.h"

class InstanceDialog : public QDialog
{
  Q_OBJECT
public:
  /* Constructor function */
  InstanceDialog(EditorMapIO* edit_io, QWidget* parent = nullptr);
  InstanceDialog(EditorMapItem* edit_item, QWidget* parent = nullptr);
  InstanceDialog(EditorMapNPC* edit_npc, QWidget* parent = nullptr);
  InstanceDialog(EditorMapPerson* edit_person, QWidget* parent = nullptr);
  InstanceDialog(EditorMapThing* edit_thing = NULL, QWidget* parent = nullptr);

  /* Destructor function */
  ~InstanceDialog();

private:
  /* Check boxes for base event and interaction control */
  QCheckBox* box_active;
  QCheckBox* box_base_event;
  QCheckBox* box_base_speed;
  QCheckBox* box_interaction;
  QCheckBox* box_lock_base;
  QCheckBox* box_respawn;
  QCheckBox* box_states_enter;
  QCheckBox* box_states_exit;
  QCheckBox* box_states_use;
  QCheckBox* box_states_walk;

  /* Button widgets */
  QPushButton* btn_edit_nodes;
  QPushButton* btn_lock;
  QPushButton* btn_states_enter;
  QPushButton* btn_states_exit;
  QPushButton* btn_states_use;
  QPushButton* btn_states_walk;

  /* Combo box control for algorithms and tracking */
  QComboBox* combo_algorithm;
  QComboBox* combo_dir;
  QComboBox* combo_party;
  QComboBox* combo_sound;
  QComboBox* combo_states;
  QComboBox* combo_tracking;
  QComboBox* combo_visible;
  QComboBox* combo_walkover;

  /* Description text box */
  QTextEdit* edit_description;

  /* Event view and control */
  EventSetView* event_view;

  /* The event set dialog */
  EventDialog* event_dialog;

  /* Label widgets */
  QLabel* lbl_lock_data;
  QLabel* lbl_speed_result;

  /* The line edit for thing data */
  QLineEdit* line_name;

  /* The node list */
  QListWidget* list_nodes;

  /* The list of objects used in possible events/dialog */
  QList<QString> list_items;
  QList<QString> list_maps;
  QList<QPair<QString,QString>> list_map_ios;
  QList<QString> list_map_items;
  QList<QString> list_map_npcs;
  QList<QString> list_map_persons;
  QList<QString> list_map_things;
  QList<QString> list_parties;
  QList<QString> list_sounds;
  QList<QString> list_submaps;

  /* Lock view and control */
  EditorLock* lock_ctrl;
  LockView* lock_view;

  /* Pop-ups within the dialog */
  QDialog* pop_lock;

  /* Spin control widgets */
  QSpinBox* spin_count;
  QSpinBox* spin_respawn;
  QSpinBox* spin_speed;
  QSpinBox* spin_track_maintain;
  QSpinBox* spin_track_release;
  QSpinBox* spin_track_start;

  /* The working and original thing */
  EditorEnumDb::Layer thing_type;
  EditorMapThing* thing_original;
  EditorMapThing* thing_working;

  /* Waiting for sub-map data */
  //bool waiting_convo;
  bool waiting_for_submap;
  bool waiting_path;

  /* Private Constants */
  static const int kALGO_COUNT;
  static const std::string kALGO_STATES[];
  static const int kTRACK_MAX;
  static const int kTRACK_MIN;

  /* Public Constants */
public:
  static const int kMAX_ITEM_COUNT; /* Maximum number of items */

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Creates the layout - only called on initial construction */
  void createLayout();

  /* Called on initial set-up of the class */
  void setup();

  /* Update the class data */
  void updateData();

  /* Updates the list of nodes */
  void updateNodes();

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Custom close event */
  void closeEvent(QCloseEvent* event);

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Edit base signal */
  void editBase(EditorMapThing* thing);

  /* Ok pressed */
  void ok(QString name_list);

  /* Path edit start trigger */
  void pathEditStart(EditorNPCPath*);

  /* Person direction changed on base */
  void personChanged(EditorMapPerson* ref);

  /* Select tile trigger */
  void selectTile(EditorEnumDb::MapObjectMode view);

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* Button control triggers */
  void buttonBaseEdit();
  void buttonCancel();
  void buttonEditNodes();
  void buttonLockCancel();
  void buttonLockEdit();
  void buttonLockOk();
  void buttonOk();
  void buttonStateEnter();
  void buttonStateExit();
  void buttonStateUse();
  void buttonStateWalk();

  /* Changed text in line edits */
  void changedDescription();
  void changedName(QString name);

  /* Check box triggers */
  void checkActiveChange(int state);
  void checkBaseChange(int state);
  void checkBaseSpeed(int state);
  void checkInteractionChange(int state);
  void checkLockBase(int state);
  void checkRespawnChange(int state);
  void checkStateEnter(int state);
  void checkStateExit(int state);
  void checkStateUse(int state);
  void checkStateWalk(int state);

  /* Combo box triggers */
  void comboAlgorithmChange(int index);
  void comboDirectionChange(QString item);
  void comboPartyChange(int index);
  void comboSoundChange(const QString& sound);
  void comboStateChange(int index);
  void comboTrackingChange(int index);
  void comboVisibilityChange(int index);
  void comboWalkoverChange(int index);

  /* Item count changed */
  void countChanged(int value);

  /* Edit event set trigger */
  void editEventSet(EditorEventSet* set, QString window_title = "",
                    bool view_only = false);

  /* Edit Single Node trigger */
  void editNode(QListWidgetItem*);

  /* Edit event set clicked ok finish - update event */
  void eventUpdated();

  /* Respawn time changed */
  void respawnChanged(int value);

  /* Select tile trigger */
  void selectTile();

  /* Speed changed */
  void speedChanged(int value);

  /* Tracking setpoint changed */
  void trackMaintainChanged(int value);
  void trackReleaseChanged(int value);
  void trackStartChanged(int value);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the event dialog widget */
  EventDialog* getEventDialog();

  /* Returns the list of objects, used for dialog and event creation */
  QList<QString> getListItems();
  QList<QString> getListMaps();
  QList<QPair<QString,QString>> getListMapIOs();
  QList<QString> getListMapItems();
  QList<QString> getListMapNPCs();
  QList<QString> getListMapPersons();
  QList<QString> getListMapThings();
  QList<QString> getListParties();
  QList<QString> getListSounds();
  QList<QString> getListSubmaps();

  /* Sets the list of parties, used for dialog and event creation */
  void setListItems(QList<QString> items);
  void setListMaps(QList<QString> maps);
  void setListMapThings(QList<QString> things,
                        QList<QPair<QString,QString>> ios,
                        QList<QString> items, QList<QString> persons,
                        QList<QString> npcs);
  void setListParties(QList<QString> parties);
  void setListSounds(QList<QString> sounds);
  void setListSubmaps(QList<QString> sub_maps);

  /* Sets the working thing to the original */
  void updateOriginal();

  /* Update path finished */
  void updatePathFinished();

  /* Update the selected tile for the thing */
  void updateSelectedTile(int id, int x, int y);
};

#endif // INSTANCEDIALOG_H
