/*******************************************************************************
 * Class Name: EventDialog
 * Date Created: November 18, 2015
 * Inheritance: QDialog
 * Description: The class is for managing the display and edit of the
 *              EditorEventSet class - within a QDialog widget.
 ******************************************************************************/
#include "Dialog/EventDialog.h"

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
  layout->setRowMinimumHeight(6, 10);
  layout->setRowMinimumHeight(8, 10);
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
  layout->addWidget(btn_lock, 2, 4);

  /* Lock Event */
  lbl_lock_event = new QLabel("Lock Event: TELEPORT THING", this);
  lbl_lock_event->setStyleSheet("border: 1px solid #a8a8a8");
  layout->addWidget(lbl_lock_event, 3, 0, 1, 4);
  QPushButton* btn_lock_event = new QPushButton("Edit", this);
  layout->addWidget(btn_lock_event, 3, 4);

  /* Unlock Events */
  list_unlock_events = new QListWidget(this);
  layout->addWidget(list_unlock_events, 4, 0, 1, 5);
  QPushButton* btn_add = new QPushButton("Add", this);
  layout->addWidget(btn_add, 5, 0);
  btn_edit = new QPushButton("Edit", this);
  layout->addWidget(btn_edit, 5, 1);
  btn_rem = new QPushButton("Remove", this);
  layout->addWidget(btn_rem, 5, 2);
  btn_up = new QPushButton("Up", this);
  layout->addWidget(btn_up, 5, 3);
  btn_down = new QPushButton("Down", this);
  layout->addWidget(btn_down, 5, 4);

  /* Event and Lock View */
  view_stack = new QStackedWidget(this);
  view_event = new EventView(nullptr, this);
  view_stack->addWidget(view_event);
  view_lock = new LockView(nullptr, this);
  view_stack->addWidget(view_lock);
  layout->addWidget(view_stack, 2, 6, 4, 5);

  /* Separation */
  //QFrame* frm_horiz1 = new QFrame(this);
  //frm_horiz1->setFrameShape(QFrame::HLine);
  //frm_horiz1->setLineWidth(1);
  //layout->addWidget(frm_horiz1, 8, 0, 1, 11);

  /* Save/Cancel buttons */
  QPushButton* btn_ok = new QPushButton("Ok", this);
  layout->addWidget(btn_ok, 9, 9);
  QPushButton* btn_cancel = new QPushButton("Cancel", this);
  layout->addWidget(btn_cancel, 9, 10);

  /* Finally resize and lock size at minimum */
  updateGeometry();
  setFixedSize(minimumSizeHint());
}

/* Set layout data */
void EventDialog::setLayoutData()
{

}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
  
/* Category changed */
//void categoryChanged(int index);

/* Edit conversation trigger */
void EventDialog::editConversation(Conversation* convo, bool is_option)
{

}

/* Select tile trigger */
void EventDialog::selectTileConvo()
{

}

/* Select tile trigger */
void EventDialog::selectTileMain()
{

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

}

/* Update the selected tile for the thing */
void EventDialog::updateSelectedTile(int id, int x, int y)
{

}
