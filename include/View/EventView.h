/*******************************************************************************
 * Class Name: EventView
 * Date Created: February 10, 2015
 * Inheritance: QFrame
 * Description: The class is for managing the display and edit of the
 *              EditorEvent class.
 ******************************************************************************/
#ifndef EVENTVIEW_H
#define EVENTVIEW_H

#include <QComboBox>
#include <QFrame>
#include <QGridLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QStackedWidget>
#include <QTextEdit>
#include <QTreeWidget>
#include <QVBoxLayout>

#include "Database/EditorEvent.h"
#include "Database/GameDatabase.h"

class EventView : public QFrame
{
  Q_OBJECT
public:
  /* Constructor function */
  EventView(EditorEvent* event, QWidget* parent = NULL,
            bool conversation_enabled = true);

  /* Destructor function */
  ~EventView();

private:
  /* Right click actions that can be disabled */
  QAction* action_delete;
  QAction* action_option;

  /* The category of the event view */
  QComboBox* combo_category;

  /* The conversation event - convo tree */
  QTreeWidget* convo_tree;

  /* Game database */
  GameDatabase* database;

  /* The connected event - original and working version */
  EditorEvent* event;

  /* Give item event control widgets */
  QSpinBox* item_count;
  QComboBox* item_name;

  /* The list of objects used in possible events */
  QVector<QString> list_items;
  QVector<QString> list_maps;
  QVector<QString> list_submaps;
  QVector<QString> list_things;

  /* Switch maps event - map name view box */
  QComboBox* map_name;

  /* Notification event text edit box */
  QTextEdit* notification_edit;

  /* Right click menu on conversation dialog */
  QString rightclick_index;
  QMenu* rightclick_menu;

  /* Teleport event - view text boxes for map location and thing */
  QLineEdit* tele_map;
  QComboBox* tele_thing;

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

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* Category changed */
  void categoryChanged(int index);

  /* The change map button press */
  void changeMapChanged(int index);

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

  /* The teleport event button presses */
  void teleportMapPressed();
  void teleportThingChanged(int index);

  /* Update the layout trigger */
  void updateConversation();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the event */
  EditorEvent* getEvent();

  /* Returns the list of objects, used for event creation */
  QVector<QString> getListItems();
  QVector<QString> getListMaps();
  QVector<QString> getListSubmaps();
  QVector<QString> getListThings();

  /* Sets the editor event */
  void setEvent(EditorEvent* event);

  /* Sets the list of objects, used for event creation */
  void setListItems(QVector<QString> items);
  void setListMaps(QVector<QString> maps);
  void setListSubmaps(QVector<QString> sub_maps);
  void setListThings(QVector<QString> things);

  /* Update the selected tile for the thing */
  void updateSelectedTile(int id, int x, int y);
};

#endif // EVENTVIEW_H