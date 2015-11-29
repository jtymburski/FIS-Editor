/*******************************************************************************
 * Class Name: EventDialog
 * Date Created: November 18, 2015
 * Inheritance: QDialog
 * Description: The class is for managing the display and edit of the
 *              EditorEventSet class - within a QDialog widget.
 ******************************************************************************/
#include "Dialog/EventDialog.h"
//#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: The main event dialog to handle the editor event set.
 *
 * Inputs: EditorEventSet& set - the set to edit within the dialog
 *         QWidget* parent - the parent widget
 *         QString window_title - the title to make the window. Defaulted
 */
EventDialog::EventDialog(EditorEventSet* set, QWidget* parent,
                         QString window_title)
           : QDialog(parent)
{
  convo_dialog = nullptr;
  waiting_convo = false;
  waiting_for_submap = false;

  /* Set-up the event set */
  set_original = nullptr;
  set_working = new EditorEventSet();

  /* Layout setup */
  if(window_title.isEmpty())
    createLayout();
  else
    createLayout(window_title);
  setEventSet(set);
}

/*
 * Description: Dstructor function
 */
EventDialog::~EventDialog()
{
  setEventSet(nullptr);
  delete set_working;
  set_working = nullptr;
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: Creates the dialog layout with QT functional widgets.
 *
 * Inputs: QString window_title -
 * Output: none
 */
void EventDialog::createLayout(QString window_title)
{
  /* General dialog control */
  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  setWindowTitle(window_title);

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

/*
 * Description: Updates the data in the widgets. CreateLayout() must be called
 *              prior.
 *
 * Inputs: none
 * Output: none
 */
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

/*
 * Description: Sets the event view with the new event widget
 *
 * Inputs: EditorEvent* event - the editor event widget
 * Output: none
 */
void EventDialog::setViewEvent(EditorEvent* event)
{
  /* Make sure the sub-dialogs are removed */
  editConversation(nullptr, false);

  /* Update the unlock */
  if(event != view_event->getEvent())
  {
    view_event->setEvent(event);
  }

  /* Depending on the status of the set event, bring forward the view */
  if(event != nullptr)
    view_stack->setCurrentIndex(0);
  updateDataAll();
}

/*
 * Description: Sets the lock view with the new lock widget
 *
 * Inputs: EditorLock* lock - the editor lock widget
 * Output: none
 */
void EventDialog::setViewLock(EditorLock* lock)
{
  /* Make sure the sub-dialogs are removed */
  editConversation(nullptr, false);

  /* Update the lock */
  if(lock != view_lock->getLock())
  {
    view_lock->setLock(lock);
  }

  /* Depending on the status of the set event, bring forward the view */
  if(lock != nullptr)
    view_stack->setCurrentIndex(1);
  updateDataAll();
}

/*
 * Description: Update all data lock and events within the set view
 *
 * Inputs: none
 * Output: none
 */
void EventDialog::updateDataAll()
{
  updateDataLock();
  updateDataLockEvent();
  updateDataUnlocks();
}

/*
 * Description: Update the data for the lock within the set widget
 *
 * Inputs: none
 * Output: none
 */
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

/*
 * Description: Update the data for the lock event within the set widget
 *
 * Inputs: none
 * Output: none
 */
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

/*
 * Description: Update the data for the unlock events within the set widget
 *
 * Inputs: none
 * Output: none
 */
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
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: The re-implementation of the close event for the dialog. Cleans
 *              up the existing event references and deletes memory.
 *
 * Inputs: QCloseEvent* event - the close event (accepted after clean-up)
 * Output: none
 */
void EventDialog::closeEvent(QCloseEvent* event)
{
  editConversation(nullptr, false);
  setEventSet(nullptr);

  event->accept();
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

/*
 * Description: Button slot on the add an unlock event buttn. Adds the unlock
 *              event to the tail of the existing sets of events.
 *
 * Inputs: none
 * Output: none
 */
void EventDialog::btnAddUnlock()
{
  /* Add event and update data */
  set_working->addEventUnlocked(EditorEvent());
  updateDataUnlocks();

  /* Select last newly added row */
  list_unlock_events->setCurrentRow(list_unlock_events->count() - 1);
}

/*
 * Description: Button slot on the cancel button. Just closes the dialog and
 *              doesn't save the changes.
 *
 * Inputs: none
 * Output: none
 */
void EventDialog::btnCancel()
{
  close();
  emit cancel();
}

/*
 * Description: Button slot on the move the current unlock event down within
 *              the list. Shifts the event one point down lower towards the
 *              bottom of the set of unlock events.
 *
 * Inputs: none
 * Output: none
 */
void EventDialog::btnDownUnlock()
{
  int selected = set_working->shiftUnlockedDown(
                                      list_unlock_events->currentRow());
  if(selected >= 0)
  {
    updateDataUnlocks();
    list_unlock_events->setCurrentRow(selected);
  }
}

/*
 * Description: Button slot on edit lock data of the set. This edits the
 *              current lock and puts it in the right view.
 *
 * Inputs: none
 * Output: none
 */
void EventDialog::btnEditLock()
{
  setViewLock(set_working->getLockedState());
}

/*
 * Description: Button slot on edit lock event data of the set. This edits the
 *              one lock event and puts it in the right view.
 *
 * Inputs: none
 * Output: none
 */
void EventDialog::btnEditLockEvent()
{
  setViewEvent(set_working->getEventLocked());
}

/*
 * Description: Button slot on edit unlock event data of the set. This edits the
 *              current selected unlock event and puts it in the right view.
 *
 * Inputs: none
 * Output: none
 */
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

/*
 * Description: Button slot on the ok button. Emits ok prior to closing the
 *              dialog. The ok gets handled by the widget parent, which
 *              updates any uses of the event set prior to accepting changes.
 *
 * Inputs: none
 * Output: none
 */
void EventDialog::btnOk()
{
  editConversation(nullptr, false);
  updateSet();
  close();
  emit ok();
}

/*
 * Description: Button slot on the remove an unlock event. This removes the
 *              selected unlock event from the working set and moves the
 *              remainder up in the list.
 *
 * Inputs: none
 * Output: none
 */
void EventDialog::btnRemoveUnlock()
{
  if(list_unlock_events->currentRow() >= 0)
  {
    EditorEvent* curr = set_working->getEventUnlocked(
                                              list_unlock_events->currentRow());
    if(view_event->getEvent() == curr)
      setViewEvent();
    set_working->unsetEventUnlocked(list_unlock_events->currentRow());
    updateDataUnlocks();
  }
}

/*
 * Description: Button slot on the move the current unlock event up within
 *              the list. Shifts the event one point higher towards the
 *              bottom of the set of unlock events.
 *
 * Inputs: none
 * Output: none
 */
void EventDialog::btnUpUnlock()
{
  int selected = set_working->shiftUnlockedUp(list_unlock_events->currentRow());
  if(selected >= 0)
  {
    updateDataUnlocks();
    list_unlock_events->setCurrentRow(selected);
  }
}

/*
 * Description: Edits the current conversation instance trigger. Triggered
 *              from EventView. Required to lock out the pop-up.
 *
 * Inputs: Conversation* convo - the conversation segment to edit
 *         bool is_option - true if the segment is an option
 * Output: none
 */
void EventDialog::editConversation(Conversation* convo, bool is_option)
{
  if(convo_dialog != NULL)
  {
    disconnect(convo_dialog->getEventView(), SIGNAL(selectTile()),
               this, SLOT(selectTileConvo()));
    disconnect(convo_dialog, SIGNAL(success()),
               view_event, SLOT(updateConversation()));
    delete convo_dialog;
  }
  convo_dialog = NULL;

  /* Create the new conversation dialog */
  if(convo != nullptr)
  {
    convo_dialog = new ConvoDialog(convo, is_option, this);
    convo_dialog->setListThings(getListThings());
    convo_dialog->getEventView()->setListItems(getListItems());
    convo_dialog->getEventView()->setListMaps(getListMaps());
    convo_dialog->getEventView()->setListSounds(getListSounds());
    convo_dialog->getEventView()->setListSubmaps(getListSubmaps());
    connect(convo_dialog->getEventView(), SIGNAL(selectTile()),
            this, SLOT(selectTileConvo()));
    connect(convo_dialog, SIGNAL(success()),
            view_event, SLOT(updateConversation()));
    convo_dialog->show();
  }
}

/*
 * Description: Slot which triggers when the selected unlock event in the list
 *              changes. Updates enabled/disabled buttons.
 *
 * Inputs: int current_row - the new selected row in the list
 * Output: none
 */
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

/*
 * Description: Slot which triggers when the user double clicks on the unlock
 *              event list. Calls btnEditUnlock()
 *
 * Inputs: QListWidgetItem* - not used
 * Output: none
 */
void EventDialog::listUnlockDouble(QListWidgetItem*)
{
  btnEditUnlock();
}

/*
 * Description: Triggers the select tile dialog for the conversation in the
 *              event view in the pop-up. This hides the pop-up and waits for a
 *              tile to be selected on the map render.
 *
 * Inputs: none
 * Output: none
 */
void EventDialog::selectTileConvo()
{
  if(convo_dialog != nullptr)
  {
    convo_dialog->hide();
    selectTileMain();
    waiting_convo = true;
  }
}

/*
 * Description: Triggers the select tile dialog for the pop-up. This hides the
 *              pop-up and waits for a tile to be selected on the map render.
 *
 * Inputs: none
 * Output: none
 */
void EventDialog::selectTileMain()
{
  waiting_convo = false;
  waiting_for_submap = true;
  hide();
  emit selectTile();
}

/*
 * Description: Slot which triggers when the selected unlock parse state
 *              changes.
 *
 * Inputs: int index - the new selected row in the drop down
 * Output: none
 */
void EventDialog::unlockParseChanged(int index)
{
  set_working->setUnlockedState(static_cast<UnlockedState>(index));
}

/*
 * Description: Updates the viewable events within the set, the unlocked and
 *              locked.
 *
 * Inputs: none
 * Output: none
 */
void EventDialog::updateEvent()
{
  updateDataLockEvent();
  updateDataUnlocks();
}

/*
 * Description: Updates the viewable lock within the set.
 *
 * Inputs: none
 * Output: none
 */
void EventDialog::updateLock()
{
  updateDataLock();
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Returns the list of items, used for event creation.
 *
 * Inputs: none
 * Output: QVector<QString> - list of all items (for give item event)
 */
QVector<QString> EventDialog::getListItems()
{
  return view_event->getListItems();
}

/*
 * Description: Returns the list of maps, used for event creation.
 *
 * Inputs: none
 * Output: QVector<QString> - list of all maps (for change map event)
 */
QVector<QString> EventDialog::getListMaps()
{
  return view_event->getListMaps();
}

/*
 * Description: Returns a list of sounds being used in the event dialog.
 *              Used when required for neighboring calls.
 *
 * Inputs: none
 * Output: QList<QString> - the sound string list
 */
QList<QString> EventDialog::getListSounds()
{
  return view_event->getListSounds();
}

/*
 * Description: Returns the list of sub-maps, used for event creation.
 *
 * Inputs: none
 * Output: QVector<QString> - list of all sub-maps (for teleport event)
 */
QVector<QString> EventDialog::getListSubmaps()
{
  return view_event->getListSubmaps();
}

/*
 * Description: Returns the list of things, used for event creation.
 *
 * Inputs: none
 * Output: QVector<QString> - list of all things (for teleport event)
 */
QVector<QString> EventDialog::getListThings()
{
  return view_event->getListThings();
}

/*
 * Description: Returns a reference to the original editor event set being
 *              edited within the event dialog
 *
 * Inputs: none
 * Output: EditorEventSet* - the original set reference
 */
EditorEventSet* EventDialog::getSetOriginal()
{
  return set_original;
}

/*
 * Description: Returns a reference to the working editor event set being
 *              edited within the event dialog
 *
 * Inputs: none
 * Output: EditorEventSet* - the working set reference
 */
EditorEventSet* EventDialog::getSetWorking()
{
  return set_working;
}

/*
 * Description: Returns the event view that is used within the dialog
 *
 * Inputs: none
 * Output: EventView* - the reference event view widget from within
 */
EventView* EventDialog::getViewEvent()
{
  return view_event;
}

/*
 * Description: Returns the lock view that is used within the dialog
 *
 * Inputs: none
 * Output: LockView* - the reference lock view widget from within
 */
LockView* EventDialog::getViewLock()
{
  return view_lock;
}

/*
 * Description: Sets the list of items, used for event creation
 *
 * Inputs: QVector<QString> - list of all items (for give item event)
 * Output: none
 */
void EventDialog::setListItems(QVector<QString> items)
{
  view_event->setListItems(items);
  view_lock->setListItems(items);
}

/*
 * Description: Sets the list of maps, used for event creation
 *
 * Inputs: QVector<QString> - list of all maps (for change map event)
 * Output: none
 */
void EventDialog::setListMaps(QVector<QString> maps)
{
  view_event->setListMaps(maps);
}

/*
 * Description: Sets the list of sounds, to be used within the event dialog for
 *              the drop down selection and within the event.
 *
 * Inputs: QList<QString> sounds - the sound string list
 * Output: none
 */
void EventDialog::setListSounds(QList<QString> sounds)
{
  view_event->setListSounds(sounds);
}

/*
 * Description: Sets the list of sub-maps, used for event creation.
 *
 * Inputs: QVector<QString> - list of all sub-maps (for teleport event)
 * Output: none
 */
void EventDialog::setListSubmaps(QVector<QString> sub_maps)
{
  view_event->setListSubmaps(sub_maps);
}

/*
 * Description: Sets the list of things, used for event creation.
 *
 * Inputs: QVector<QString> - list of all things (for teleport event)
 * Output: none
 */
void EventDialog::setListThings(QVector<QString> things)
{
  view_event->setListThings(things);
}

/*
 * Description: Sets the event set that gets viewed within the event dialog.
 *              The previous set is not saved and removed.
 *
 * Inputs: EditorEventSet* set - the editor event set to edit
 * Output: none
 */
void EventDialog::setEventSet(EditorEventSet* set)
{
  set_original = set;
  if(set_original != nullptr)
    *set_working = *set_original;
  setLayoutData();
}

/*
 * Description: Updates the editor event set. Triggered by an external class
 *              when it modifies the reference event.
 *
 * Inputs: none
 * Output: none
 */
void EventDialog::updateSet()
{
  if(set_original != nullptr)
  {
    *set_original = *set_working;
  }
}

/*
 * Description: Updates the dialog with the tile which was selected on the
 *              sub-map. This shows the pop-up and updates the event with the
 *              new location. If it was triggered from the embedded event view,
 *              it passes the updated location to it.
 *
 * Inputs: int id - the ID of the sub-map
 *         int x - the x tile location in the sub-map
 *         int y - the y tile location in the sub-map
 * Output: none
 */
void EventDialog::updateSelectedTile(int id, int x, int y)
{
  if(waiting_for_submap)
  {
    waiting_for_submap = false;
    show();
    if(waiting_convo)
    {
      convo_dialog->getEventView()->updateSelectedTile(id, x, y);
      convo_dialog->show();
    }
    else
    {
      view_event->updateSelectedTile(id, x, y);
    }
  }
}
