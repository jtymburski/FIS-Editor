/*******************************************************************************
 * Class Name: EventView
 * Date Created: February 10, 2015
 * Inheritance: QFrame
 * Description: The class is for managing the display and edit of the
 *              EditorEvent class.
 ******************************************************************************/
#include "View/EventView.h"
#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
EventView::EventView(EditorEvent* event, QWidget* parent) : QFrame(parent)
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

  /* Label title for widget */
  QLabel* lbl_title = new QLabel("<b>Event</b>", this);
  layout->addWidget(lbl_title, 0, Qt::AlignCenter);

  /* Combo box for event category */
  combo_category = new QComboBox(this);
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
  QLabel* lbl_not_used = new QLabel("NOT USED", this);
  QVBoxLayout* layout_unset = new QVBoxLayout(widget_unset);
  layout_unset->addWidget(lbl_not_used, 0, Qt::AlignCenter);

  /* Widget for give item control */
  QWidget* widget_give = new QWidget(this);
  QLabel* lbl_give_item = new QLabel("Item:", this);
  QLabel* lbl_give_count = new QLabel("Count:", this);
  item_name = new QLineEdit("", this);
  item_name->setDisabled(true);
  item_name->setMinimumWidth(200);
  item_count = new QSpinBox(this);
  item_count->setMinimum(1);
  item_count->setMaximum(100000);
  connect(item_count, SIGNAL(valueChanged(int)),
          this, SLOT(giveCountChanged(int)));
  QPushButton* btn_item_change = new QPushButton(this);
  btn_item_change->setIcon(QIcon(":/images/icons/32_settings.png"));
  btn_item_change->setIconSize(QSize(24,24));
  btn_item_change->setMaximumSize(30, 30);
  connect(btn_item_change, SIGNAL(clicked()), this, SLOT(giveItemPressed()));
  QGridLayout* layout_give = new QGridLayout(widget_give);
  layout_give->addWidget(lbl_give_item, 0, 0);
  layout_give->addWidget(item_name, 0, 1, 1, 2);
  layout_give->addWidget(btn_item_change, 0, 3);
  layout_give->addWidget(lbl_give_count, 1, 0);
  layout_give->addWidget(item_count, 1, 1);
  layout_give->setColumnStretch(2, 1);

  /* Widget for notification control */
  QWidget* widget_notification = new QWidget(this);
  notification_edit = new QTextEdit(this);
  notification_edit->setMinimumWidth(320);
  connect(notification_edit, SIGNAL(textChanged()),
          this, SLOT(notificationTextChanged()));
  QVBoxLayout* layout_notification = new QVBoxLayout(widget_notification);
  layout_notification->addWidget(notification_edit, 0, Qt::AlignCenter);

  /* Widget for battle execution control */
  QWidget* widget_battle = new QWidget(this);
  QLabel* lbl_no_edit = new QLabel("NO SETTINGS", this);
  QVBoxLayout* layout_battle = new QVBoxLayout(widget_battle);
  layout_battle->addWidget(lbl_no_edit, 0, Qt::AlignCenter);

  /* Widget for switching the map */
  QWidget* widget_map = new QWidget(this);
  QLabel* lbl_map_name = new QLabel("Map:");
  map_name = new QLineEdit("", this);
  map_name->setDisabled(true);
  map_name->setMinimumWidth(200);
  QPushButton* btn_map_change = new QPushButton(this);
  btn_map_change->setIcon(QIcon(":/images/icons/32_settings.png"));
  btn_map_change->setIconSize(QSize(24,24));
  btn_map_change->setMaximumSize(30, 30);
  connect(btn_map_change, SIGNAL(clicked()), this, SLOT(changeMapPressed()));
  QGridLayout* layout_map = new QGridLayout(widget_map);
  layout_map->addWidget(lbl_map_name, 0, 0);
  layout_map->addWidget(map_name, 0, 1);
  layout_map->addWidget(btn_map_change, 0, 2);

  /* Widget for teleportation thing control */
  QWidget* widget_teleport = new QWidget(this);
  QLabel* lbl_tele_map = new QLabel("Sub-Map:", this);
  QLabel* lbl_tele_thing = new QLabel("Thing:", this);
  tele_map = new QLineEdit("", this);
  tele_map->setDisabled(true);
  tele_map->setMinimumWidth(200);
  tele_thing = new QLineEdit("", this);
  tele_thing->setDisabled(true);
  tele_thing->setMinimumWidth(200);
  QPushButton* btn_tele_map = new QPushButton(this);
  btn_tele_map->setIcon(QIcon(":/images/icons/32_settings.png"));
  btn_tele_map->setIconSize(QSize(24,24));
  btn_tele_map->setMaximumSize(30, 30);
  connect(btn_tele_map, SIGNAL(clicked()), this, SLOT(teleportMapPressed()));
  QPushButton* btn_tele_thing = new QPushButton(this);
  btn_tele_thing->setIcon(QIcon(":/images/icons/32_settings.png"));
  btn_tele_thing->setIconSize(QSize(24,24));
  btn_tele_thing->setMaximumSize(30, 30);
  connect(btn_tele_thing, SIGNAL(clicked()),
          this, SLOT(teleportThingPressed()));
  QGridLayout* layout_tele = new QGridLayout(widget_teleport);
  layout_tele->addWidget(lbl_tele_map, 0, 0);
  layout_tele->addWidget(tele_map, 0, 1);
  layout_tele->addWidget(btn_tele_map, 0, 2);
  layout_tele->addWidget(lbl_tele_thing, 1, 0);
  layout_tele->addWidget(tele_thing, 1, 1);
  layout_tele->addWidget(btn_tele_thing, 1, 2);

  /* Widget for conversation control */
  QWidget* widget_convo = new QWidget(this);
  convo_tree = new QTreeWidget(this);
  convo_tree->setMinimumWidth(320);
  QVBoxLayout* layout_convo = new QVBoxLayout(widget_convo);
  layout_convo->addWidget(convo_tree, 0, Qt::AlignCenter);

  /* Stacked widget for housing all the different views for categories */
  view_stack = new QStackedWidget(this);
  view_stack->addWidget(widget_unset);
  view_stack->addWidget(widget_give);
  view_stack->addWidget(widget_notification);
  view_stack->addWidget(widget_battle);
  view_stack->addWidget(widget_map);
  view_stack->addWidget(widget_teleport);
  view_stack->addWidget(widget_convo);
  layout->addWidget(view_stack, 1, Qt::AlignCenter);

  /* Configure parent widget */
  setFrameStyle(QFrame::Panel);
  setLayout(layout);
  setLineWidth(1);
  setMaximumSize(352, 192);
  setMinimumSize(352, 192);
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
  qDebug() << "TODO: CATEGORY CHANGED - " << index;
  view_stack->setCurrentIndex(index);
}

/* The change map button press */
void EventView::changeMapPressed()
{
  qDebug() << "TODO: CHANGE MAP PRESSED";
}

/* The give item event slot changes */
void EventView::giveCountChanged(int index)
{
  qDebug() << "TODO: GIVE ITEM COUNT CHANGED - " << index;
}

/* The give item event slot changes */
void EventView::giveItemPressed()
{
  qDebug() << "TODO: GIVE ITEM PRESSED";
}

/* The notification event text changes */
void EventView::notificationTextChanged()
{
  qDebug() << "TODO: NOTIFICATION TEXT CHANGED";
}

/* The teleport event button presses */
void EventView::teleportMapPressed()
{
  qDebug() << "TODO: TELEPORT MAP PRESSED";
}

/* The teleport event button presses */
void EventView::teleportThingPressed()
{
  qDebug() << "TODO: TELEPORT THING PRESSED";
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
