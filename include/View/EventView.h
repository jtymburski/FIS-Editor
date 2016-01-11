/*******************************************************************************
 * Class Name: EventView
 * Date Created: February 10, 2015
 * Inheritance: QFrame
 * Description: The class is for managing the display and edit of the
 *              EditorEvent class.
 ******************************************************************************/
#ifndef EVENTVIEW_H
#define EVENTVIEW_H

class ConvoDialog;

#include <QCheckBox>
#include <QComboBox>
#include <QFrame>
#include <QGridLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QStackedWidget>
#include <QTextEdit>
#include <QTreeWidget>
#include <QVBoxLayout>

#include "Database/EditorEvent.h"
//#include "Database/GameDatabase.h"
#include "EditorEnumDb.h"

class EventView : public QFrame
{
  Q_OBJECT
public:
  /* Constructor function */
  EventView(EditorEvent* event = NULL, QWidget* parent = NULL,
            EventClassifier limiter = EventClassifier::NOEVENT,
            bool view_only = false);

  /* Destructor function */
  ~EventView();

private:
  /* Right click actions that can be disabled */
  QAction* action_delete;
  QAction* action_option;

  /* List of available events to edit in this set */
  QVector<EventClassifier> available_events;

  /* Start battle event control widgets */
  QPushButton* battle_eventlose;
  QPushButton* battle_eventwin;
  QCheckBox* battle_losegg;
  QCheckBox* battle_restorehealth;
  QCheckBox* battle_restoreqd;
  QCheckBox* battle_windisappear;

  /* The one shot selection in the view */
  QCheckBox* check_oneshot;

  /* The category of the event view */
  QComboBox* combo_category;

  /* Sound selection widget */
  QComboBox* combo_sound;

  /* The conversation event - convo tree */
  QTreeWidget* convo_tree;

  /* Game database */
  //GameDatabase* database;

  /* The connected event - original and working version */
  EditorEvent* event;

  /* Give item event control widgets */
  QSpinBox* item_count;
  QComboBox* item_name;

  /* The limiter for view events */
  EventClassifier limiter;

  /* The list of objects used in possible events */
  QVector<QString> list_items;
  QVector<QString> list_maps;
  QVector<QPair<QString,QString>> list_map_ios;
  QVector<QString> list_map_items;
  QVector<QString> list_map_npcs;
  QVector<QString> list_map_persons;
  QVector<QString> list_map_things;
  QList<QString> list_sounds;
  QVector<QString> list_submaps;
  //QVector<QString> list_things_no_io;

  /* Switch maps event - map name view box */
  QComboBox* map_name;

  /* Notification event text edit box */
  QTextEdit* notification_edit;

  /* Conversation pop-up */
  ConvoDialog* pop_convo;

  /* Event pop-up for battle executes */
  QDialog* pop_event;
  EditorEvent pop_event_ctrl;
  Event* pop_event_edit;
  QGridLayout* pop_event_layout;
  EventView* pop_event_view;

  /* Property control widgets */
  QCheckBox* prop_active_mod;
  QCheckBox* prop_active_val;
  QCheckBox* prop_forced_mod;
  QCheckBox* prop_forced_val;
  QCheckBox* prop_freeze_mod;
  QCheckBox* prop_freeze_val;
  QComboBox* prop_id;
  QCheckBox* prop_inactive_dis;
  QCheckBox* prop_inactive_mod;
  QSpinBox* prop_inactive_val;
  QCheckBox* prop_reset_mod;
  QCheckBox* prop_reset_val;
  QCheckBox* prop_respawn_dis;
  QCheckBox* prop_respawn_mod;
  QSpinBox* prop_respawn_val;
  QLabel* prop_speed_desc;
  QCheckBox* prop_speed_mod;
  QSpinBox* prop_speed_val;
  QCheckBox* prop_track_mod;
  QComboBox* prop_track_opt;
  QCheckBox* prop_visible_mod;
  QCheckBox* prop_visible_val;

  /* Right click menu on conversation dialog */
  QString rightclick_index;
  QMenu* rightclick_menu;

  /* Take item event control widgets */
  QSpinBox* take_count;
  QComboBox* take_name;

  /* Teleport event - view text boxes for map location and thing */
  QLineEdit* tele_map;
  QComboBox* tele_thing;

  /* Unlock thing control widgets */
  QComboBox* unth_name;
  QCheckBox* unth_view_enable;
  QCheckBox* unth_view_scroll;
  QSpinBox* unth_view_time;

  /* Unlock tile control widgets */
  QCheckBox* unti_event_enter;
  QCheckBox* unti_event_exit;
  QLineEdit* unti_location;
  QCheckBox* unti_view_enable;
  QCheckBox* unti_view_scroll;
  QSpinBox* unti_view_time;

  /* Unlock IO control widgets */
  QGroupBox* unio_event;
  QCheckBox* unio_event_enter;
  QCheckBox* unio_event_exit;
  QCheckBox* unio_event_use;
  QCheckBox* unio_event_walk;
  QCheckBox* unio_mode_lock;
  QCheckBox* unio_mode_states;
  QComboBox* unio_name;
  QComboBox* unio_state;
  QCheckBox* unio_view_enable;
  QCheckBox* unio_view_scroll;
  QSpinBox* unio_view_time;

  /* View only flag */
  bool view_only;

  /* View stack for sub-widgets */
  QStackedWidget* view_stack;

  /* Waiting triggers */
  bool waiting_tile_convo;
  bool waiting_tile_event;
  bool waiting_tile;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Creates the layout and widgets for this controller */
  void createLayout();//bool conversation_enabled = true);

  /* Edit conversation trigger */
  void editConversation(Conversation* convo, bool is_option);

  /* Edit event trigger to spawn the event pop-up */
  void editEvent(Event* edit_event = nullptr);

  /* Returns the convesation item, based on the index, of the tree widget */
  QTreeWidgetItem* getConvo(QString base_index);

  /* Returns the conversation index of the tree widget */
  QString getConvoIndex(QTreeWidgetItem* ref, QTreeWidgetItem* parent = NULL);

  /* Tile select main call */
  void selectTileMain();

  /* Set layout data */
  void setLayoutData();

  /* Update conversation data */
  void updateConvoTree(Conversation* ref, QTreeWidgetItem* parent,
                       bool option = false);

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Edit conversation trigger */
  //void editConversation(Conversation* convo, bool is_option);

  /* Edit event trigger */
  //void editEvent(Event* edit_event);

  /* Select tile trigger */
  void selectTile();

  /* Updated trigger */
  void updated();

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* The start battle event slot changes */
  void battleEventLoseEdit();
  void battleEventWinEdit();
  void battleHealthFlagChange(int state);
  void battleLoseFlagChange(int state);
  void battleQDFlagChange(int state);
  void battleWinFlagChange(int state);

  /* Category changed */
  void categoryChanged(int index);

  /* The change map button press */
  void changeMapChanged(int index);

  /* One shot choice changed */
  void changedOneShot(int state);

  /* Sound reference updated */
  void changedSound(const QString & text);

  /* Conversation item double clicked trigger */
  void convoDoubleClick(QTreeWidgetItem*, int);

  /* The convo menu requested - on right click */
  void convoMenuRequested(QPoint point);

  /* The give item event slot changes */
  void giveCountChanged(int index);
  void giveItemChanged(int index);

  /* The notification event text changes */
  void notificationTextChanged();

  /* Event pop-up slot triggers */
  void popEventCancel();
  void popEventOk();

  /* The property widget slots */
  void propertyActiveMod(int state);
  void propertyActiveVal(int state);
  void propertyIDChange(const QString& text);
  void propertyInactiveDisable(int state);
  void propertyInactiveMod(int state);
  void propertyInactiveVal(int value);
  void propertyForcedMod(int state);
  void propertyForcedVal(int state);
  void propertyFreezeMod(int state);
  void propertyFreezeVal(int state);
  void propertyResetMod(int state);
  void propertyResetVal(int state);
  void propertyRespawnDisable(int state);
  void propertyRespawnMod(int state);
  void propertyRespawnVal(int value);
  void propertySpeedMod(int state);
  void propertySpeedVal(int value);
  void propertyTrackChange(const QString& text);
  void propertyTrackMod(int state);
  void propertyTypeChange(const QString& text);
  void propertyVisibleMod(int state);
  void propertyVisibleVal(int state);

  /* Resize the QTreeWidget for the items */
  void resizeTree(QTreeWidgetItem*);

  /* The right click slots, for conversation */
  void rightClickDelete();
  void rightClickEdit();
  void rightClickInsertAfter();
  void rightClickInsertBefore();
  void rightClickInsertOption();

  /* Select tile sub calls */
  void selectTileConvo();
  void selectTileEvent();

  /* The take item event slot changes */
  void takeCountChanged(int index);
  void takeItemChanged(int index);

  /* The teleport event button presses */
  void teleportMapPressed();
  void teleportThingChanged(int index);

  /* The unlock IO triggers */
  void unlockIOChanged(const QString & text);
  void unlockIOModeLock(int state);
  void unlockIOModeStates(int state);
  void unlockIOStateChanged(int index);
  void unlockIOStateEnter(int state);
  void unlockIOStateExit(int state);
  void unlockIOStateUse(int state);
  void unlockIOStateWalk(int state);
  void unlockIOView(int state);
  void unlockIOViewScroll(int state);
  void unlockIOViewTime(int time);

  /* The unlock thing triggers */
  void unlockThingChanged(const QString & text);
  void unlockThingView(int state);
  void unlockThingViewScroll(int state);
  void unlockThingViewTime(int time);

  /* The unlock tile triggers */
  void unlockTileEnter(int state);
  void unlockTileExit(int state);
  void unlockTilePressed();
  void unlockTileView(int state);
  void unlockTileViewScroll(int state);
  void unlockTileViewTime(int time);

  /* Update the layout trigger */
  void updateBattle();
  void updateConversation();
  void updateLists(bool things = false, bool ios = false, bool items = false,
                   bool persons = false, bool npcs = false);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Close all pop-ups */
  void closeAllPopups();

  /* Returns the event */
  EditorEvent* getEvent();

  /* Returns the list of objects, used for event creation */
  QVector<QString> getListItems();
  QVector<QString> getListMaps();
  QVector<QPair<QString,QString>> getListMapIOs();
  QVector<QString> getListMapItems();
  QVector<QString> getListMapNPCs();
  QVector<QString> getListMapPersons();
  QVector<QString> getListMapThings();
  QList<QString> getListSounds();
  QVector<QString> getListSubmaps();


  /* Sets the editor event */
  void setEvent(EditorEvent* event);

  /* Sets the list of objects, used for event creation */
  void setListItems(QVector<QString> items);
  void setListMaps(QVector<QString> maps);
  void setListMapIOs(QVector<QPair<QString,QString>> ios);
  void setListMapItems(QVector<QString> items);
  void setListMapNPCs(QVector<QString> npcs);
  void setListMapPersons(QVector<QString> persons);
  void setListMapThings(QVector<QString> things);
  void setListMapThings(QVector<QString> things,
                        QVector<QPair<QString,QString>> ios,
                        QVector<QString> items, QVector<QString> persons,
                        QVector<QString> npcs);
  void setListSounds(QList<QString> sounds);
  void setListSubmaps(QVector<QString> sub_maps);

  /* Updates the event */
  void updateEvent();

  /* Update the selected tile for the thing */
  void updateSelectedTile(int id, int x, int y);
};

#endif // EVENTVIEW_H
