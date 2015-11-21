/*******************************************************************************
 * Class Name: EventDialog
 * Date Created: November 18, 2015
 * Inheritance: QDialog
 * Description: The class is for managing the display and edit of the
 *              EditorEventSet class - within a QDialog widget.
 ******************************************************************************/
#ifndef EVENTDIALOG_H
#define EVENTDIALOG_H

//#include <QComboBox>
#include <QDialog>
//#include <QFrame>
//#include <QGridLayout>
//#include <QHeaderView>
//#include <QLabel>
//#include <QLineEdit>
#include <QListWidget>
//#include <QPushButton>
//#include <QSpinBox>
//#include <QStackedWidget>
//#include <QTextEdit>
//#include <QTreeWidget>
//#include <QVBoxLayout>

//#include "Database/EditorEvent.h"
#include "Database/EditorEventSet.h"
//#include "Database/GameDatabase.h"
#include "Dialog/ConvoDialog.h"
#include "View/EventView.h"
#include "View/LockView.h"

class EventDialog : public QDialog
{
  Q_OBJECT
public:
  /* Constructor function */
  EventDialog(EditorEventSet* set = nullptr, QWidget* parent = nullptr);

  /* Destructor function */
  ~EventDialog();

private:
  /* Right click actions that can be disabled */
  //QAction* action_delete;
  //QAction* action_option;

  /* Combo box for unlock parse category */
  QComboBox* box_unlock;

  /* Push button widgets */
  QPushButton* btn_down;
  QPushButton* btn_edit;
  QPushButton* btn_rem;
  QPushButton* btn_up;

  /* The category of the event view */
  //QComboBox* combo_category;

  /* Sound selection widget */
  //QComboBox* combo_sound;

  /* The conversation dialog */
  ConvoDialog* convo_dialog;

  /* The conversation event - convo tree */
  //QTreeWidget* convo_tree;

  /* Game database */
  //GameDatabase* database;

  /* The connected event - original and working version */
  //EditorEvent* event;

  /* Give item event control widgets */
  //QSpinBox* item_count;
  //QComboBox* item_name;

  /* Label widgets */
  QLabel* lbl_lock_event;
  QLabel* lbl_lock;

  /* List widgets */
  QListWidget* list_unlock_events;

  /* Switch maps event - map name view box */
  //QComboBox* map_name;

  /* Notification event text edit box */
  //QTextEdit* notification_edit;

  /* Right click menu on conversation dialog */
  //QString rightclick_index;
  //QMenu* rightclick_menu;

  /* The connected editor event set - to be modified by the view */
  EditorEventSet* set_original;
  EditorEventSet* set_working;

  /* Take item event control widgets */
  //QSpinBox* take_count;
  //QComboBox* take_name;

  /* Teleport event - view text boxes for map location and thing */
  //QLineEdit* tele_map;
  //QComboBox* tele_thing;

  /* The view event and lock widgets */
  EventView* view_event;
  LockView* view_lock;

  /* View stack for sub-widgets */
  QStackedWidget* view_stack;

  /* Waiting for sub-map data */
  bool waiting_convo;
  bool waiting_for_submap;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Creates the layout and widgets for this controller */
  void createLayout();

  /* Returns the convesation item, based on the index, of the tree widget */
  //QTreeWidgetItem* getConvo(QString base_index);

  /* Returns the conversation index of the tree widget */
  //QString getConvoIndex(QTreeWidgetItem* ref, QTreeWidgetItem* parent = NULL);

  /* Set layout data */
  void setLayoutData();

  /* Set view information */
  void setViewEvent(EditorEvent* event = nullptr);
  void setViewLock(EditorLock* lock = nullptr);

  /* Update data for the various areas */
  void updateDataAll();
  void updateDataLock();
  void updateDataLockEvent();
  void updateDataUnlocks();

  /* Update conversation data */
  //void updateConvoTree(Conversation* ref, QTreeWidgetItem* parent,
  //                     bool option = false);

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Edit conversation trigger */
  //void editConversation(Conversation* convo, bool is_option);

  /* Ok pressed */
  void ok();

  /* Select tile trigger */
  void selectTile();

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* Button control triggers */
  void btnAddUnlock();
  void btnCancel();
  void btnDownUnlock();
  void btnEditLock();
  void btnEditLockEvent();
  void btnEditUnlock();
  void btnOk();
  void btnRemoveUnlock();
  void btnUpUnlock();

  /* Category changed */
  //void categoryChanged(int index);

  /* The change map button press */
  //void changeMapChanged(int index);

  /* Sound reference updated */
  //void changedSound(const QString & text);

  /* Conversation item double clicked trigger */
  //void convoDoubleClick(QTreeWidgetItem*, int);

  /* The convo menu requested - on right click */
  //void convoMenuRequested(QPoint point);

  /* Edit conversation trigger */
  void editConversation(Conversation* convo, bool is_option);

  /* The give item event slot changes */
  //void giveCountChanged(int index);
  //void giveItemChanged(int index);

  /* Unlock list changes */
  void listUnlockChange(int current_row);
  void listUnlockDouble(QListWidgetItem*);

  /* The notification event text changes */
  //void notificationTextChanged();

  /* Resize the QTreeWidget for the items */
  //void resizeTree(QTreeWidgetItem*);

  /* The right click slots, for conversation */
  //void rightClickDelete();
  //void rightClickEdit();
  //void rightClickInsertAfter();
  //void rightClickInsertBefore();
  //void rightClickInsertOption();

  /* Select tile trigger */
  void selectTileConvo();
  void selectTileMain();

  /* The give item event slot changes */
  //void takeCountChanged(int index);
  //void takeItemChanged(int index);

  /* The teleport event button presses */
  //void teleportMapPressed();
  //void teleportThingChanged(int index);

  /* Unlock parse changed */
  void unlockParseChanged(int index);

  /* Update the layout trigger */
  //void updateConversation();

  /* Update view data associated to the lock or event */
  void updateEvent();
  void updateLock();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the event */
  //EditorEvent* getEvent();

  /* Returns the list of objects, used for event creation */
  QVector<QString> getListItems();
  QVector<QString> getListMaps();
  QList<QString> getListSounds();
  QVector<QString> getListSubmaps();
  QVector<QString> getListThings();

  /* Returns the event set */
  EditorEventSet* getSetOriginal();
  EditorEventSet* getSetWorking();

  /* Returns views */
  EventView* getViewEvent();
  LockView* getViewLock();

  /* Sets the editor event */
  //void setEvent(EditorEvent* event);

  /* Sets the list of objects, used for event creation */
  void setListItems(QVector<QString> items);
  void setListMaps(QVector<QString> maps);
  void setListSounds(QList<QString> sounds);
  void setListSubmaps(QVector<QString> sub_maps);
  void setListThings(QVector<QString> things);

  /* Sets the event set */
  void setEventSet(EditorEventSet* set);

  /* Updates the event set */
  void updateSet();

  /* Update the selected tile for the thing */
  void updateSelectedTile(int id, int x, int y);
};

#endif // EVENTDIALOG_H
