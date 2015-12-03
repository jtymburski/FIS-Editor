/*******************************************************************************
 * Class Name: EventView
 * Date Created: February 10, 2015
 * Inheritance: QFrame
 * Description: The class is for managing the display and edit of the
 *              EditorEvent class.
 ******************************************************************************/
#ifndef EVENTVIEW_H
#define EVENTVIEW_H

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
            bool conversation_enabled = true);

  /* Destructor function */
  ~EventView();

private:
  /* Right click actions that can be disabled */
  QAction* action_delete;
  QAction* action_option;

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

  /* The list of objects used in possible events */
  //QVector<QString> list_ios;
  QVector<QPair<QString,QString>> list_ios;
  QVector<QString> list_items;
  QVector<QString> list_maps;
  QList<QString> list_sounds;
  QVector<QString> list_submaps;
  QVector<QString> list_things;
  QVector<QString> list_things_no_io;

  /* Switch maps event - map name view box */
  QComboBox* map_name;

  /* Notification event text edit box */
  QTextEdit* notification_edit;

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

  /* View stack for sub-widgets */
  QStackedWidget* view_stack;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Creates the layout and widgets for this controller */
  void createLayout(bool conversation_enabled = true);

  /* Returns the convesation item, based on the index, of the tree widget */
  QTreeWidgetItem* getConvo(QString base_index);

  /* Returns the conversation index of the tree widget */
  QString getConvoIndex(QTreeWidgetItem* ref, QTreeWidgetItem* parent = NULL);

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
  void editConversation(Conversation* convo, bool is_option);

  /* Select tile trigger */
  void selectTile();

  /* Updated trigger */
  void updated();

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* Category changed */
  void categoryChanged(int index);

  /* The change map button press */
  void changeMapChanged(int index);

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

  /* Resize the QTreeWidget for the items */
  void resizeTree(QTreeWidgetItem*);

  /* The right click slots, for conversation */
  void rightClickDelete();
  void rightClickEdit();
  void rightClickInsertAfter();
  void rightClickInsertBefore();
  void rightClickInsertOption();

  /* The give item event slot changes */
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
  void updateConversation();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the event */
  EditorEvent* getEvent();

  /* Returns the list of objects, used for event creation */
  QVector<QPair<QString,QString>> getListIOs();
  QVector<QString> getListItems();
  QVector<QString> getListMaps();
  QList<QString> getListSounds();
  QVector<QString> getListSubmaps();
  QVector<QString> getListThings();

  /* Sets the editor event */
  void setEvent(EditorEvent* event);

  /* Sets the list of objects, used for event creation */
  void setListIOs(QVector<QPair<QString,QString>> ios);
  void setListItems(QVector<QString> items);
  void setListMaps(QVector<QString> maps);
  void setListSounds(QList<QString> sounds);
  void setListSubmaps(QVector<QString> sub_maps);
  void setListThings(QVector<QString> things);

  /* Updates the event */
  void updateEvent();

  /* Update the selected tile for the thing */
  void updateSelectedTile(int id, int x, int y);
};

#endif // EVENTVIEW_H
