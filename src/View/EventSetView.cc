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
/*
 * Description: Constructor function for instantiating the event set view with
 *              an EditorEventSet and a parent widget.
 *
 * Inputs: EditorEventSet* event_set - the event set data from handler
 *         QWidget* parent - the parent widget
 */
EventSetView::EventSetView(EditorEventSet* event_set, QWidget* parent)
            : QFrame(parent)
{
  /* Create the layout */
  createLayout();

  /* Set the event set */
  setEventSet(event_set);
}

/*
 * Description: Destructor function
 */
EventSetView::~EventSetView()
{
  setEventSet(nullptr);
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
  
/*
 * Description: Creates the dialog layout with QT functional widgets.
 *
 * Inputs: none
 * Output: none
 */
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
  //setMaximumSize(EditorEnumDb::kEVENT_VIEW_W, EditorEnumDb::kEVENT_SET_VIEW_H);
  //setMinimumSize(EditorEnumDb::kEVENT_VIEW_W, EditorEnumDb::kEVENT_SET_VIEW_H);
}

/*
 * Description: Updates the data in the widgets. CreateLayout() must be called
 *              prior.
 *
 * Inputs: none
 * Output: none
 */
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

    /* Check if real data exists and bold button if so */
    QFont bold = btn_edit->font();
    bold.setBold(true);
    QFont not_bold = bold;
    not_bold.setBold(false);
    if(!event_set->isEmpty())
      btn_edit->setFont(bold);
    else
      btn_edit->setFont(not_bold);
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
  
/*
 * Description: Button slot on the edit set button. This is triggered for
 *              the event dialog within the parent class.
 *
 * Inputs: none
 * Output: none
 */
void EventSetView::buttonEdit()
{
  if(event_set != nullptr)
    emit editSet(event_set);
}

/*
 * Description: Updates the event set pointer data visible within the widget
 *
 * Inputs: none
 * Output: none
 */
void EventSetView::eventUpdated()
{
  setLayoutData();
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Returns the event set used within the widget.
 *
 * Inputs: none
 * Output: EditorEventSet* - the event set data class for the current parent
 *                           object
 */
EditorEventSet* EventSetView::getEventSet()
{
  return event_set;
}

/*
 * Description: Sets the event set to visualize using the widget
 *
 * Inputs: EditorEventSet* set - the event set to visualize in the widget
 * Output: none
 */
void EventSetView::setEventSet(EditorEventSet* set)
{
  event_set = set;
  setLayoutData();
}
