/*******************************************************************************
 * Class Name: EventView
 * Date Created: February 10, 2015
 * Inheritance: QWidget
 * Description: The class is for managing the display and edit of the
 *              EditorEvent class.
 ******************************************************************************/
#include "View/EventView.h"
#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
EventView::EventView(EditorEvent* event, QWidget* parent) : QWidget(parent)
{
  /* Initialize variables */
  this->event = NULL;

  /* Create the layout */
  createLayout();

  /* Set the event */
  setEvent(event);
}

/* Destructor function */
EventView::~EventView()
{

}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* Creates the layout and widgets for this controller */
void EventView::createLayout()
{
  /* Layout */
  QVBoxLayout* layout = new QVBoxLayout(this);

  /* Combo box for event category */
  QComboBox* combo_category = new QComboBox(this);
  combo_category->addItem("--");
  combo_category->addItem("Give Item");
  combo_category->addItem("Notification");
  combo_category->addItem("Execute Battle");
  combo_category->addItem("Switch Maps");
  combo_category->addItem("Teleport Thing");
  combo_category->addItem("Conversation");
  connect(combo_category, SIGNAL(currentIndexChanged(int)),
          this, SLOT(categoryChanged(int)));
  layout->addWidget(combo_category, 0, Qt::AlignCenter);

  /* Widget for blank control */
  QWidget* widget_unset = new QWidget(this);

  /* Widget for give item control */
  QWidget* widget_give = new QWidget(this);

  /* Widget for notification control */
  QWidget* widget_notification = new QWidget(this);

  /* Widget for battle execution control */
  QWidget* widget_battle = new QWidget(this);

  /* Widget for switching the map */
  QWidget* widget_map = new QWidget(this);

  /* Widget for teleportation thing control */
  QWidget* widget_teleport = new QWidget(this);

  /* Widget for conversation control */
  QWidget* widget_convo = new QWidget(this);

  /* Stacked widget for housing all the different views for categories */
  QStackedWidget* stack = new QStackedWidget(this);
  stack->addWidget(widget_unset);
  stack->addWidget(widget_give);
  stack->addWidget(widget_notification);
  stack->addWidget(widget_battle);
  stack->addWidget(widget_map);
  stack->addWidget(widget_teleport);
  stack->addWidget(widget_convo);
  layout->addWidget(stack, 1, Qt::AlignCenter);

  /* Add layout to widget */
  setLayout(layout);
}
  
/* Set layout data */
void EventView::setLayoutData()
{
  // TODO
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

/* Category changed */
void EventView::categoryChanged(int index)
{
  qDebug() << index;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Returns the event */
EditorEvent* EventView::getEvent()
{
  return event;
}

/* Sets the editor event */
void EventView::setEvent(EditorEvent* event)
{
  /* If existing event isn't NULL, finish connection */
  if(this->event != NULL)
    this->event = NULL;

  /* Set the event */
  this->event = event;

  /* After-math processing */
  setLayoutData();
}
