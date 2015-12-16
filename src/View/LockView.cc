/*******************************************************************************
 * Class Name: LockView
 * Date Created: November 17, 2015
 * Inheritance: QFrame
 * Description: The class is for managing the display and edit of the
 *              EditorLock class.
 ******************************************************************************/
#include "View/LockView.h"
//#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/ 

/*
 * Description: Constructor function for instantiating the lock view with an
 *              EditorLock and a parent widget.
 *
 * Inputs: EditorLock* lock - the lock controller for the Lock from EventSet
 *         QWidget* parent - the parent widget
 *         bool view_only - true if the event is only for viewing and no edit
 */
LockView::LockView(EditorLock* lock, QWidget* parent, bool view_only)
        : QFrame(parent)
{
  /* Initialize variables */
  this->lock = nullptr;
  this->view_only = view_only;

  /* Create the layout */
  createLayout();

  /* Set the lock */
  setLock(lock);
}

/*
 * Description: Destructor function
 */
LockView::~LockView()
{
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
  
/*
 * Description: Creates the view layout with QT functional widgets.
 *
 * Inputs: none
 * Output: none
 */
void LockView::createLayout()
{
  /* Layout */
  QVBoxLayout* layout = new QVBoxLayout(this);

  /* Label title for widget */
  QLabel* lbl_title = new QLabel("<b>Lock</b>", this);
  layout->addWidget(lbl_title, 0, Qt::AlignCenter);

  /* Combo box for event category */
  combo_category = new QComboBox(this);
  combo_category->addItem("--");
  combo_category->addItem("Trigger");
  combo_category->addItem("Have Item");
  connect(combo_category, SIGNAL(currentIndexChanged(int)),
          this, SLOT(categoryChanged(int)));
  layout->addWidget(combo_category, 0, Qt::AlignCenter);

  /* Widget for blank control */
  QWidget* widget_unset = new QWidget(this);
  QLabel* lbl_not_used = new QLabel("NO LOCK", this);
  QVBoxLayout* layout_unset = new QVBoxLayout(widget_unset);
  layout_unset->addWidget(lbl_not_used, 0, Qt::AlignCenter);

  /* Widget for trigger control */
  QWidget* widget_trigger = new QWidget(this);
  trigger_permanent = new QCheckBox("Permanent", this);
  connect(trigger_permanent, SIGNAL(stateChanged(int)),
          this, SLOT(triggerPermanentChanged(int)));
  QGridLayout* layout_trigger = new QGridLayout(widget_trigger);
  layout_trigger->addWidget(trigger_permanent, 0, 0, Qt::AlignHCenter);

  /* Widget for have item control */
  QWidget* widget_have = new QWidget(this);
  QLabel* lbl_have_item = new QLabel("Item:", this);
  QLabel* lbl_have_count = new QLabel("Count:", this);
  item_name = new QComboBox(this);
  item_name->setMinimumWidth(200);
  connect(item_name, SIGNAL(currentIndexChanged(int)),
          this, SLOT(haveItemChanged(int)));
  item_count = new QSpinBox(this);
  item_count->setMinimum(1);
  item_count->setMaximum(100000);
  connect(item_count, SIGNAL(valueChanged(int)),
          this, SLOT(haveCountChanged(int)));
  item_consume = new QCheckBox("Consume", this);
  connect(item_consume, SIGNAL(stateChanged(int)),
          this, SLOT(haveConsumeChanged(int)));
  item_permanent = new QCheckBox("Permanent", this);
  connect(item_permanent, SIGNAL(stateChanged(int)),
          this, SLOT(havePermanentChanged(int)));
  QGridLayout* layout_have = new QGridLayout(widget_have);
  layout_have->addWidget(lbl_have_item, 0, 0);
  layout_have->addWidget(item_name, 0, 1, 1, 2);
  layout_have->addWidget(lbl_have_count, 1, 0);
  layout_have->addWidget(item_count, 1, 1);
  layout_have->addWidget(item_consume, 2, 0, 1, 2);
  layout_have->addWidget(item_permanent, 2, 2);
  layout_have->setColumnStretch(2, 1);

  /* Stacked widget for housing all the different views for categories */
  view_stack = new QStackedWidget(this);
  view_stack->addWidget(widget_unset);
  view_stack->addWidget(widget_trigger);
  view_stack->addWidget(widget_have);
  layout->addWidget(view_stack, 1, Qt::AlignCenter);

  /* Configure parent widget */
  setFrameStyle(QFrame::Panel);
  setLayout(layout);
  setLineWidth(1);
  QPalette palette;
  palette.setColor(QPalette::Foreground, QColor(168, 168, 168));
  setPalette(palette);
  setMaximumSize(EditorEnumDb::kEVENT_VIEW_W, EditorEnumDb::kEVENT_VIEW_H);
  setMinimumSize(EditorEnumDb::kEVENT_VIEW_W, EditorEnumDb::kEVENT_VIEW_H);
}

/*
 * Description: Updates the data in the widgets. CreateLayout() must be called
 *              prior.
 *
 * Inputs: none
 * Output: none
 */
void LockView::setLayoutData()
{
  if(lock != nullptr)
  {
    /* Set the selected category */
    if((int)lock->getLockType() != combo_category->currentIndex())
      combo_category->setCurrentIndex((int)lock->getLockType());

    /* Set the data, based on the lock */
    if(lock->getLockType() == LockedState::ITEM)
    {
      /* Data */
      item_count->setValue(lock->getHaveItemCount());
      item_consume->setChecked(lock->getHaveItemConsume());
      item_permanent->setChecked(lock->isPermanent());

      /* Attempt to find the item name in the combo box */
      int index = -1;
      for(int i = 0; i < list_items.size(); i++)
      {
        QStringList set = list_items[i].split(":");
        if(set.size() == 2)
        {
          if(set.front().toInt() == lock->getHaveItemID())
          {
            item_name->setCurrentIndex(i);
            index = i;
          }
        }
      }

      /* If index < 0 (not found), set to first */
      if(index < 0)
        item_name->setCurrentIndex(0);
    }
    else if(lock->getLockType() == LockedState::TRIGGER)
    {
      trigger_permanent->setChecked(lock->isPermanent());
    }

    /* Enable the widget */
    combo_category->setEnabled(true);
  }
  else
  {
    combo_category->setCurrentIndex(0);
    combo_category->setDisabled(true);
  }

  /* View handling */
  setDisabled(view_only);
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
  
/*
 * Description: Slot which triggers when the category drop down changes.
 *              Switches the view and changes the type of event.
 *
 * Inputs: int index - the index in the combo box
 * Output: none
 */
void LockView::categoryChanged(int index)
{
  /* Update the stack */
  view_stack->setCurrentIndex(index);

  /* Update the lock */
  if(lock != nullptr)
  {
    if((int)lock->getLockType() != index)
    {
      /* Change the lock */
      if(index == (int)LockedState::NONE)
        lock->setLockBlank();
      else if(index == (int)LockedState::TRIGGER)
        lock->setLockTrigger();
      else if(index == (int)LockedState::ITEM)
        lock->setLockHaveItem();
    }

    /* Update the layout */
    setLayoutData();
    emit updated();
  }
}

/*
 * Description: Slot which triggers when the have item consumed flag in the have
 *              item lock widget is changed. Updates the lock
 *
 * Inputs: int state - the state of the check box
 * Output: none
 */
void LockView::haveConsumeChanged(int state)
{
  if(lock != nullptr)
    lock->setLockHaveItem(lock->getHaveItemID(), lock->getHaveItemCount(),
                          state == Qt::Checked, lock->isPermanent());
}

/*
 * Description: Slot which triggers when the have item count widget in the have
 *              item lock is changed. Updates the lock
 *
 * Inputs: int index - the count value
 * Output: none
 */
void LockView::haveCountChanged(int index)
{
  if(lock != nullptr)
    lock->setLockHaveItem(lock->getHaveItemID(), index,
                          lock->getHaveItemConsume(), lock->isPermanent());
}

/*
 * Description: Slot which triggers when the have item ID changes in the combo
 *              box. Updates the lock
 *
 * Inputs: int index - the index in the combo box
 * Output: none
 */
void LockView::haveItemChanged(int index)
{
  if(lock != nullptr)
  {
    if(index >= 0 && index < list_items.size())
    {
      QStringList list = list_items[index].split(":");
      if(list.size() == 2)
        lock->setLockHaveItem(list.front().toInt(), lock->getHaveItemCount(),
                              lock->getHaveItemConsume(), lock->isPermanent());
    }
  }
}

/*
 * Description: Slot which triggers when the have item permanent flag in the
 *              have item lock widget is changed. Updates the lock
 *
 * Inputs: int state - the state of the check box
 * Output: none
 */
void LockView::havePermanentChanged(int state)
{
  if(lock != nullptr)
    lock->setLockHaveItem(lock->getHaveItemID(), lock->getHaveItemCount(),
                          lock->getHaveItemConsume(), state == Qt::Checked);
}

/*
 * Description: Slot which triggers when the trigger lock permanent flag in the
 *              trigger lock widget is changed. Updates the lock
 *
 * Inputs: int state - the state of the check box
 * Output: none
 */
void LockView::triggerPermanentChanged(int state)
{
  if(lock != nullptr)
    lock->setLockTrigger(state == Qt::Checked);
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Returns the list of items, used for lock creation.
 *
 * Inputs: none
 * Output: QVector<QString> - list of all items (for have item lock)
 */
QVector<QString> LockView::getListItems()
{
  return list_items;
}

/*
 * Description: Returns the editor lock which is being manipulated by the
 *              widget.
 *
 * Inputs: none
 * Output: EditorLock* - pointer to the lock being edited
 */
EditorLock* LockView::getLock()
{
  return lock;
}

/*
 * Description: Sets the list of items, used for lock creation
 *
 * Inputs: QVector<QString> - list of all items (for have item lock)
 * Output: none
 */
void LockView::setListItems(QVector<QString> items)
{
  /* Set new data */
  list_items = items;

  /* Block signals */
  item_name->blockSignals(true);

  /* Give Items */
  item_name->clear();
  for(int i = 0; i < list_items.size(); i++)
    item_name->addItem(list_items[i]);

  /* Update and unblock */
  setLayoutData();
  item_name->blockSignals(false);
}

/*
 * Description: Sets the editor lock being edited by the lock view.
 *
 * Inputs: EditorLock* lock - the new reference lock to edit
 * Output: none
 */
void LockView::setLock(EditorLock* lock)
{
  /* Set new lock */
  this->lock = lock;

  /* After-math processing */
  setLayoutData();
}

/*
 * Description: Updates the editor lock. Triggered by an external class when
 *              it modifies the reference lock.
 *
 * Inputs: none
 * Output: none
 */
void LockView::updateLock()
{
  setLayoutData();
}
