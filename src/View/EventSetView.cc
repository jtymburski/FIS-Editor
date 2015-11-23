/*******************************************************************************
 * Class Name: EventSetView
 * Date Created: November 22, 2015
 * Inheritance: QFrame
 * Description: The class is for managing the display of the EditorEventSet
 *              class. It includes a trigger to edit it from outside callers.
 ******************************************************************************/
#include "View/EventSetView.h"

/*=============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
EventSetView::EventSetView(EditorEventSet* event_set, QWidget* parent)
            : QFrame(parent)
{
  /* Create the layout */
  createLayout();

  /* Set the event set */
  setEventSet(event_set);
}

/* Destructor function */
EventSetView::~EventSetView()
{
  setEventSet(nullptr);
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
  
/* Creates the layout and widgets for this controller */
void EventSetView::createLayout()
{
  /* Layout */
  QVBoxLayout* layout = new QVBoxLayout(this);

  /* Label title for widget */
  QLabel* lbl_title = new QLabel("<b>Event Set</b>", this);
  layout->addWidget(lbl_title, 0, Qt::AlignCenter);

  /* Label for lock */
  lbl_lock = new QLabel("Lock: None", this);
  layout->addWidget(lbl_lock, 0, Qt::AlignCenter);

  /* Label for lock event */
  lbl_lock_event = new QLabel("Lock Event: None", this);
  layout->addWidget(lbl_lock_event, 0, Qt::AlignCenter);

  /* Label for unlock events */
  lbl_unlock_events = new QLabel("Unlock Events: None", this);
  layout->addWidget(lbl_unlock_events, 0, Qt::AlignCenter);

  /* Edit button */
  btn_edit = new QPushButton("Edit", this);
  connect(btn_edit, SIGNAL(clicked()), this, SLOT(buttonEdit()));
  layout->addWidget(btn_edit, 0, Qt::AlignCenter);

  /* Configure parent widget */
  setFrameStyle(QFrame::Panel);
  setLineWidth(1);
  QPalette palette;
  palette.setColor(QPalette::Foreground, QColor(168, 168, 168));
  setPalette(palette);
  setMaximumSize(EditorEnumDb::kEVENT_VIEW_W, EditorEnumDb::kEVENT_VIEW_H - 50);
  setMinimumSize(EditorEnumDb::kEVENT_VIEW_W, EditorEnumDb::kEVENT_VIEW_H - 50);
}

/* Set layout data */
void EventSetView::setLayoutData()
{
  /* Event set is valid */
  if(event_set != nullptr)
  {
    QVector<QString> str_set = event_set->getTextSummary();

    /* Lock label */
    if(str_set.size() > 0)
      lbl_lock->setText(str_set[0]);
    else
      lbl_lock->setText("");

    /* Lock event label */
    if(str_set.size() > 1)
      lbl_lock_event->setText(str_set[1]);
    else
      lbl_lock_event->setText("");

    /* Unlock events label */
    if(str_set.size() > 2)
      lbl_unlock_events->setText(str_set[2]);
    else
      lbl_unlock_events->setText("");

    /* Edit button enable */
    btn_edit->setEnabled(true);
  }
  /* Event set is invalid */
  else
  {
    /* Set the text to notify of no set */
    lbl_lock->setText("");
    lbl_lock_event->setText("NO SET AVAILABLE");
    lbl_unlock_events->setText("");

    /* Edit button disable */
    btn_edit->setDisabled(true);
  }
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
  
/* Button control triggers */
void EventSetView::buttonEdit()
{
  if(event_set != nullptr)
    emit editSet(event_set);
}

/* Event updated trigger */
void EventSetView::eventUpdated()
{
  setLayoutData();
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Returns the event set */
EditorEventSet* EventSetView::getEventSet()
{
  return event_set;
}

/* Sets the event set */
void EventSetView::setEventSet(EditorEventSet* set)
{
  event_set = set;
  setLayoutData();
}
