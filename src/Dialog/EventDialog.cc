/*******************************************************************************
 * Class Name: EventDialog
 * Date Created: November 18, 2015
 * Inheritance: QDialog
 * Description: The class is for managing the display and edit of the
 *              EditorEventSet class - within a QDialog widget.
 ******************************************************************************/
#include "Dialog/EventDialog.h"
#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
EventDialog::EventDialog(EditorEventSet* set, QWidget* parent)
           : QDialog(parent)
{
  convo_dialog = nullptr;
  waiting_convo = false;
  waiting_for_submap = false;

  /* Set-up the event set */
  set_original = nullptr;
  set_working = new EditorEventSet();

  /* Layout setup */
  createLayout();
  setEventSet(set);
}

/* Destructor function */
EventDialog::~EventDialog()
{
  setEventSet(nullptr);
  delete set_working;
  set_working = nullptr;
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* Creates the layout and widgets for this controller */
void EventDialog::createLayout()
{
  /* General dialog control */
  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  setWindowTitle("Event Set Edit");

  /* Layout setup */
  QGridLayout* layout = new QGridLayout(this);
  layout->setRowMinimumHeight(1, 10);
  layout->setRowMinimumHeight(7, 10);
  layout->setRowMinimumHeight(9, 10);
  //layout->setColumnStretch(2, 1);
  //layout->setRowStretch(4, 1);

  /* Title */
  QLabel* lbl_title = new QLabel("Event Set", this);
  QFont font = lbl_title->font();
  //font.setPointSize(12);
  font.setBold(true);
  lbl_title->setFont(font);
  layout->addWidget(lbl_title, 0, 0, 1, 11, Qt::AlignHCenter);

  /* Lock */
  lbl_lock = new QLabel("Lock: ITEM #", this);
  lbl_lock->setStyleSheet("border: 1px solid #a8a8a8");
  layout->addWidget(lbl_lock, 2, 0, 1, 4);
  QPushButton* btn_lock = new QPushButton("Edit", this);
  connect(btn_lock, SIGNAL(clicked()), this, SLOT(btnEditLock()));
  layout->addWidget(btn_lock, 2, 4);

  /* Lock Event */
  lbl_lock_event = new QLabel("Lock Event: TELEPORT THING", this);
  lbl_lock_event->setStyleSheet("border: 1px solid #a8a8a8");
  layout->addWidget(lbl_lock_event, 3, 0, 1, 4);
  QPushButton* btn_lock_event = new QPushButton("Edit", this);
  connect(btn_lock_event, SIGNAL(clicked()), this, SLOT(btnEditLockEvent()));
  layout->addWidget(btn_lock_event, 3, 4);

  /* Unlock event category */
  QLabel* lbl_unlock_type = new QLabel("Unlock Parse", this);
  layout->addWidget(lbl_unlock_type, 4, 0, 1, 2);
  box_unlock = new QComboBox(this);
  QStringList unlock_list;
  unlock_list << "None" << "Ordered" << "First than Random";
  box_unlock->addItems(unlock_list);
  connect(box_unlock, SIGNAL(currentIndexChanged(int)),
          this, SLOT(unlockParseChanged(int)));
  layout->addWidget(box_unlock, 4, 2, 1, 3);

  /* Unlock Events */
  list_unlock_events = new QListWidget(this);
  connect(list_unlock_events, SIGNAL(currentRowChanged(int)),
          this, SLOT(listUnlockChange(int)));
  connect(list_unlock_events, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
          this, SLOT(listUnlockDouble(QListWidgetItem*)));
  layout->addWidget(list_unlock_events, 5, 0, 1, 5);
  QPushButton* btn_add = new QPushButton("Add", this);
  connect(btn_add, SIGNAL(clicked()), this, SLOT(btnAddUnlock()));
  layout->addWidget(btn_add, 6, 0);
  btn_edit = new QPushButton("Edit", this);
  btn_edit->setDisabled(true);
  connect(btn_edit, SIGNAL(clicked()), this, SLOT(btnEditUnlock()));
  layout->addWidget(btn_edit, 6, 1);
  btn_rem = new QPushButton("Remove", this);
  btn_rem->setDisabled(true);
  connect(btn_rem, SIGNAL(clicked()), this, SLOT(btnRemoveUnlock()));
  layout->addWidget(btn_rem, 6, 2);
  btn_up = new QPushButton("Up", this);
  connect(btn_up, SIGNAL(clicked()), this, SLOT(btnUpUnlock()));
  layout->addWidget(btn_up, 6, 3);
  btn_down = new QPushButton("Down", this);
  connect(btn_down, SIGNAL(clicked()), this, SLOT(btnDownUnlock()));
  layout->addWidget(btn_down, 6, 4);

  /* Event and Lock View */
  view_stack = new QStackedWidget(this);
  view_event = new EventView(nullptr, this);
  connect(view_event, SIGNAL(editConversation(Conversation*,bool)),
          this, SLOT(editConversation(Conversation*,bool)));
  connect(view_event, SIGNAL(selectTile()), this, SLOT(selectTileMain()));
  connect(view_event, SIGNAL(updated()), this, SLOT(updateEvent()));
  view_stack->addWidget(view_event);
  view_lock = new LockView(nullptr, this);
  connect(view_lock, SIGNAL(updated()), this, SLOT(updateLock()));
  view_stack->addWidget(view_lock);
  layout->addWidget(view_stack, 2, 6, 5, 5);

  /* Separation */
  //QFrame* frm_horiz1 = new QFrame(this);
  //frm_horiz1->setFrameShape(QFrame::HLine);
  //frm_horiz1->setLineWidth(1);
  //layout->addWidget(frm_horiz1, 8, 0, 1, 11);

  /* Save/Cancel buttons */
  QPushButton* btn_ok = new QPushButton("Ok", this);
  connect(btn_ok, SIGNAL(clicked()), this, SLOT(btnOk()));
  btn_ok->setDefault(true);
  layout->addWidget(btn_ok, 10, 9);
  QPushButton* btn_cancel = new QPushButton("Cancel", this);
  connect(btn_cancel, SIGNAL(clicked()), this, SLOT(btnCancel()));
  layout->addWidget(btn_cancel, 10, 10);

  /* Finally resize and lock size at minimum */
  updateGeometry();
  setFixedSize(minimumSizeHint());
}

/* Set layout data */
void EventDialog::setLayoutData()
{
  /* Update lock */
  updateDataLock();

  /* Update lock event */
  updateDataLockEvent();

  /* Update unlock event(s) */
  box_unlock->setCurrentIndex(
                        static_cast<int>(set_working->getUnlockedState()));
  updateDataUnlocks();
}

/* Set view information */
void EventDialog::setViewEvent(EditorEvent* event)
{
  if(event != view_event->getEvent())
  {
    view_event->setEvent(event);
  }

  /* Depending on the status of the set event, bring forward the view */
  if(event != nullptr)
    view_stack->setCurrentIndex(0);
  updateDataAll();
}

/* Set view information */
void EventDialog::setViewLock(EditorLock* lock)
{
  if(lock != view_lock->getLock())
  {
    view_lock->setLock(lock);
  }

  /* Depending on the status of the set event, bring forward the view */
  if(lock != nullptr)
    view_stack->setCurrentIndex(1);
  updateDataAll();
}

/* Update data for the various areas */
void EventDialog::updateDataAll()
{
  updateDataLock();
  updateDataLockEvent();
  updateDataUnlocks();
}

/* Update data for the various areas */
void EventDialog::updateDataLock()
{
  lbl_lock->setText(set_working->getLockedState()->getTextSummary());

  /* Check if the label font should be bold */
  QFont font;
  if(view_stack->currentWidget() == view_lock &&
     set_working->getLockedState() == view_lock->getLock())
  {
    font.setBold(true);
  }
  lbl_lock->setFont(font);
}

/* Update data for the various areas */
void EventDialog::updateDataLockEvent()
{
  lbl_lock_event->setText(set_working->getEventLocked()->getTextSummary());

  /* Check if the font should be bold */
  QFont font;
  if(view_stack->currentWidget() == view_event &&
     set_working->getEventLocked() == view_event->getEvent())
  {
    font.setBold(true);
  }
  lbl_lock_event->setFont(font);
}

/* Update data for the various areas */
void EventDialog::updateDataUnlocks()
{
  /* Remove old items */
  int curr_row = list_unlock_events->currentRow();
  list_unlock_events->clear();

  /* Add new items */
  int total = set_working->getEventUnlockedTotal();
  for(int i = 0; i < total; i++)
  {
    /* Get event */
    EditorEvent* event = set_working->getEventUnlocked(i);

    /* Add item and make bold, if relevant */
    list_unlock_events->addItem(event->getTextSummary());
    if(view_stack->currentWidget() == view_event &&
       event == view_event->getEvent())
    {
      QFont font;
      font.setBold(true);
      list_unlock_events->item(list_unlock_events->count() - 1)->setFont(font);
    }
  }

  /* Re-select index */
  if(curr_row >= 0)
  {
    if(curr_row >= total)
      list_unlock_events->setCurrentRow(total - 1);
    else
      list_unlock_events->setCurrentRow(curr_row);
  }
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
  
/* Button control triggers */
void EventDialog::btnAddUnlock()
{
  /* Add event and update data */
  set_working->addEventUnlocked(EditorEvent());
  updateDataUnlocks();

  /* Select last newly added row */
  list_unlock_events->setCurrentRow(list_unlock_events->count() - 1);
}

/* Button control triggers */
void EventDialog::btnCancel()
{
  close();
}

/* Button control triggers */
void EventDialog::btnDownUnlock()
{
  // TODO
}

/* Button control triggers */
void EventDialog::btnEditLock()
{
  setViewLock(set_working->getLockedState());
}

/* Button control triggers */
void EventDialog::btnEditLockEvent()
{
  setViewEvent(set_working->getEventLocked());
}

/* Button control triggers */
void EventDialog::btnEditUnlock()
{
  if(list_unlock_events->currentRow() >= 0)
  {
    EditorEvent* event = set_working->getEventUnlocked(
                                              list_unlock_events->currentRow());
    if(event != nullptr)
      setViewEvent(event);
  }
}

/* Button control triggers */
void EventDialog::btnOk()
{
  updateSet();
  emit ok();
  close();
}

/* Button control triggers */
void EventDialog::btnRemoveUnlock()
{
  if(list_unlock_events->currentRow() >= 0)
  {
    setViewEvent();
    set_working->unsetEventUnlocked(list_unlock_events->currentRow());
    updateDataUnlocks();
  }
}

/* Button control triggers */
void EventDialog::btnUpUnlock()
{
  // TODO
}

/* Edit conversation trigger */
void EventDialog::editConversation(Conversation* convo, bool is_option)
{

}

/* Unlock list changes */
void EventDialog::listUnlockChange(int current_row)
{
  /* Edit and remove buttons */
  btn_edit->setEnabled(current_row >= 0);
  btn_rem->setEnabled(current_row >= 0);

  /* Up button */
  btn_up->setEnabled(current_row > 0);

  /* Down button */
  btn_down->setEnabled(current_row >= 0 &&
                       current_row < (list_unlock_events->count() - 1));
}

/* Unlock list changes */
void EventDialog::listUnlockDouble(QListWidgetItem*)
{
  btnEditUnlock();
}

/* Select tile trigger */
void EventDialog::selectTileConvo()
{

}

/* Select tile trigger */
void EventDialog::selectTileMain()
{

}

/* Unlock parse changed */
void EventDialog::unlockParseChanged(int index)
{
  set_working->setUnlockedState(static_cast<UnlockedState>(index));
}

/* Update view data associated to the lock or event */
void EventDialog::updateEvent()
{
  updateDataLockEvent();
  updateDataUnlocks();
}

/* Update view data associated to the lock or event */
void EventDialog::updateLock()
{
  updateDataLock();
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Returns the list of objects, used for event creation */
QVector<QString> EventDialog::getListItems()
{
  return view_event->getListItems();
}

/* Returns the list of objects, used for event creation */
QVector<QString> EventDialog::getListMaps()
{
  return view_event->getListMaps();
}

/* Returns the list of objects, used for event creation */
QList<QString> EventDialog::getListSounds()
{
  return view_event->getListSounds();
}

/* Returns the list of objects, used for event creation */
QVector<QString> EventDialog::getListSubmaps()
{
  return view_event->getListSubmaps();
}

/* Returns the list of objects, used for event creation */
QVector<QString> EventDialog::getListThings()
{
  return view_event->getListThings();
}

/* Returns the event set */
EditorEventSet* EventDialog::getSetOriginal()
{
  return set_original;
}

/* Returns the event set */
EditorEventSet* EventDialog::getSetWorking()
{
  return set_working;
}

/* Returns views */
EventView* EventDialog::getViewEvent()
{
  return view_event;
}

/* Returns views */
LockView* EventDialog::getViewLock()
{
  return view_lock;
}

/* Sets the list of objects, used for event creation */
void EventDialog::setListItems(QVector<QString> items)
{
  view_event->setListItems(items);
  view_lock->setListItems(items);
}

/* Sets the list of objects, used for event creation */
void EventDialog::setListMaps(QVector<QString> maps)
{
  view_event->setListMaps(maps);
}

/* Sets the list of objects, used for event creation */
void EventDialog::setListSounds(QList<QString> sounds)
{
  view_event->setListSounds(sounds);
}

/* Sets the list of objects, used for event creation */
void EventDialog::setListSubmaps(QVector<QString> sub_maps)
{
  view_event->setListSubmaps(sub_maps);
}

/* Sets the list of objects, used for event creation */
void EventDialog::setListThings(QVector<QString> things)
{
  view_event->setListThings(things);
}

/* Sets the event set */
void EventDialog::setEventSet(EditorEventSet* set)
{
  set_original = set;
  if(set_original != nullptr)
    *set_working = *set_original;
  setLayoutData();
}

/* Updates the event set */
void EventDialog::updateSet()
{
  if(set_original != nullptr)
  {
    *set_original = *set_working;
  }
}

/* Update the selected tile for the thing */
void EventDialog::updateSelectedTile(int id, int x, int y)
{
  // TODO
}
