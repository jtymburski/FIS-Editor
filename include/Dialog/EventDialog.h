/*******************************************************************************
 * Class Name: EventDialog
 * Date Created: November 18, 2015
 * Inheritance: QDialog
 * Description: The class is for managing the display and edit of the
 *              EditorEventSet class - within a QDialog widget.
 ******************************************************************************/
#ifndef EVENTDIALOG_H
#define EVENTDIALOG_H

#include <QCloseEvent>
#include <QDialog>
#include <QListWidget>

#include "Database/EditorEventSet.h"
#include "Dialog/ConvoDialog.h"
#include "View/EventView.h"
#include "View/LockView.h"

class EventDialog : public QDialog
{
  Q_OBJECT
public:
  /* Constructor function */
  EventDialog(EditorEventSet* set = nullptr, QWidget* parent = nullptr,
              QString window_title = "",
              EventClassifier limiter = EventClassifier::NOEVENT,
              bool view_only = false);

  /* Destructor function */
  ~EventDialog();

private:
  /* Combo box for unlock parse category */
  QComboBox* box_unlock;

  /* Push button widgets */
  QPushButton* btn_down;
  QPushButton* btn_edit;
  QPushButton* btn_rem;
  QPushButton* btn_up;

  /* Label widgets */
  QLabel* lbl_lock_event;
  QLabel* lbl_lock;

  /* List widgets */
  QListWidget* list_unlock_events;

  /* The connected editor event set - to be modified by the view */
  EditorEventSet* set_original;
  EditorEventSet* set_working;

  /* The view event and lock widgets */
  EventView* view_event;
  LockView* view_lock;

  /* View stack for sub-widgets */
  QStackedWidget* view_stack;

  /* View only flag */
  bool view_only;

  /* Waiting for sub-map data */
  bool waiting_for_submap;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Creates the layout and widgets for this controller */
  void createLayout(QString window_title = "Event Set Edit",
                    EventClassifier limiter = EventClassifier::NOEVENT);

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
  /* Cancel pressed */
  void cancel();

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

  /* Unlock list changes */
  void listUnlockChange(int current_row);
  void listUnlockDouble(QListWidgetItem*);

  /* Select tile trigger */
  void selectTileMain();

  /* Unlock parse changed */
  void unlockParseChanged(int index);

  /* Update view data associated to the lock or event */
  void updateEvent();
  void updateLock();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the list of objects, used for event creation */
  QList<QString> getListItems();
  QList<QString> getListMaps();
  QList<QPair<QString,QString>> getListMapIOs();
  QList<QString> getListMapItems();
  QList<QString> getListMapNPCs();
  QList<QString> getListMapPersons();
  QList<QString> getListMapThings();
  QList<QString> getListSounds();
  QList<QString> getListSubmaps();

  /* Returns the event set */
  EditorEventSet* getSetOriginal();
  EditorEventSet* getSetWorking();

  /* Returns views */
  EventView* getViewEvent();
  LockView* getViewLock();

  /* Sets the list of objects, used for event creation */
  void setListItems(QList<QString> items);
  void setListMaps(QList<QString> maps);
  void setListMapThings(QList<QString> things,
                        QList<QPair<QString,QString>> ios,
                        QList<QString> items, QList<QString> persons,
                        QList<QString> npcs);
  void setListSounds(QList<QString> sounds);
  void setListSubmaps(QList<QString> sub_maps);

  /* Sets the event set */
  void setEventSet(EditorEventSet* set);

  /* Updates the event set */
  void updateSet();

  /* Update the selected tile for the thing */
  void updateSelectedTile(int id, int x, int y);
};

#endif // EVENTDIALOG_H
