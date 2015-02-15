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
  database = NULL;
  this->event = NULL;

  /* Create the layout */
  createLayout();

  /* Set the event */
  setEvent(event);
}

/* Destructor function */
EventView::~EventView()
{
  disconnect(combo_category, SIGNAL(currentIndexChanged(int)),
             this, SLOT(categoryChanged(int)));
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
  item_name = new QComboBox(this);
  item_name->setMinimumWidth(200);
  connect(item_name, SIGNAL(currentIndexChanged(int)),
          this, SLOT(giveItemChanged(int)));
  item_count = new QSpinBox(this);
  item_count->setMinimum(1);
  item_count->setMaximum(100000);
  connect(item_count, SIGNAL(valueChanged(int)),
          this, SLOT(giveCountChanged(int)));
  QGridLayout* layout_give = new QGridLayout(widget_give);
  layout_give->addWidget(lbl_give_item, 0, 0);
  layout_give->addWidget(item_name, 0, 1, 1, 2);
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
  map_name = new QComboBox(this);
  map_name->setMinimumWidth(200);
  connect(map_name, SIGNAL(currentIndexChanged(int)),
          this, SLOT(changeMapChanged(int)));
  QGridLayout* layout_map = new QGridLayout(widget_map);
  layout_map->addWidget(lbl_map_name, 0, 0);
  layout_map->addWidget(map_name, 0, 1);

  /* Widget for teleportation thing control */
  QWidget* widget_teleport = new QWidget(this);
  QLabel* lbl_tele_map = new QLabel("Sub-Map:", this);
  QLabel* lbl_tele_thing = new QLabel("Thing:", this);
  tele_map = new QLineEdit("", this);
  tele_map->setDisabled(true);
  tele_map->setMinimumWidth(200);
  QPalette pal = tele_map->palette();
  pal.setColor(QPalette::Disabled, QPalette::Text,
               pal.color(QPalette::Active, QPalette::Text));
  tele_map->setPalette(pal);
  tele_thing = new QComboBox(this);
  tele_thing->setMinimumWidth(200);
  connect(tele_thing, SIGNAL(currentIndexChanged(int)),
          this, SLOT(teleportThingChanged(int)));
  QPushButton* btn_tele_map = new QPushButton(this);
  btn_tele_map->setIcon(QIcon(":/images/icons/32_settings.png"));
  btn_tele_map->setIconSize(QSize(24,24));
  btn_tele_map->setMaximumSize(30, 30);
  connect(btn_tele_map, SIGNAL(clicked()), this, SLOT(teleportMapPressed()));
  QGridLayout* layout_tele = new QGridLayout(widget_teleport);
  layout_tele->addWidget(lbl_tele_map, 0, 0);
  layout_tele->addWidget(tele_map, 0, 1);
  layout_tele->addWidget(btn_tele_map, 0, 2);
  layout_tele->addWidget(lbl_tele_thing, 1, 0);
  layout_tele->addWidget(tele_thing, 1, 1);

  /* Widget for conversation control */
  QWidget* widget_convo = new QWidget(this);
  convo_tree = new QTreeWidget(this);
  convo_tree->setMinimumWidth(320);
  convo_tree->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  convo_tree->header()->hide();
  connect(convo_tree, SIGNAL(itemCollapsed(QTreeWidgetItem*)),
          this, SLOT(resizeTree(QTreeWidgetItem*)));
  connect(convo_tree, SIGNAL(itemExpanded(QTreeWidgetItem*)),
          this, SLOT(resizeTree(QTreeWidgetItem*)));
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
  if(event != NULL)
  {
    /* Set the selected category */
    if((int)event->getEventType() != combo_category->currentIndex())
      combo_category->setCurrentIndex((int)event->getEventType());

    /* Set the data, based on the event */
    if(event->getEventType() == EventClassifier::GIVEITEM)
    {
      item_count->setValue(event->getGiveItemCount());

      /* Attempt to find the item name in the combo box */
      int index = -1;
      for(int i = 0; i < list_items.size(); i++)
      {
        QStringList set = list_items[i].split(":");
        if(set.size() == 2)
        {
          if(set.front().toInt() == event->getGiveItemID())
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
    else if(event->getEventType() == EventClassifier::NOTIFICATION)
    {
      notification_edit->setPlainText(event->getNotification());
    }
    else if(event->getEventType() == EventClassifier::RUNMAP)
    {
      /* Attempt to find the map name in the combo box */
      int index = -1;
      for(int i = 0; i < list_maps.size(); i++)
      {
        QStringList set = list_maps[i].split(":");
        if(set.size() == 2)
        {
          if(set.front().toInt() == event->getStartMapID())
          {
            map_name->setCurrentIndex(i);
            index = i;
          }
        }
      }

      /* If index < 0 (not found), set to first */
      if(index < 0)
        map_name->setCurrentIndex(0);
    }
    else if(event->getEventType() == EventClassifier::STARTCONVO)
    {
      QTreeWidgetItem* first_layer_1 = new QTreeWidgetItem(convo_tree);
      first_layer_1->setText(0, "First entry");
      QTreeWidgetItem* first_layer_2 = new QTreeWidgetItem(convo_tree);
      first_layer_2->setText(0, "Second entry");
      QTreeWidgetItem* first_layer_3 = new QTreeWidgetItem(convo_tree);
      first_layer_3->setText(0, "Third entry");
      QTreeWidgetItem* item_yes = new QTreeWidgetItem(first_layer_3);
      item_yes->setText(0, "Yes");
      QTreeWidgetItem* item_no = new QTreeWidgetItem(first_layer_3);
      item_no->setText(0, "No");
      QTreeWidgetItem* item_no_1 = new QTreeWidgetItem(item_no);
      item_no_1->setText(0, "No - Part 1 asdkfasldkfajs;dlf as;dlf as;dldkfj as;ldkj asd;lkfj as;lkdfjasd;lkdj a;lksd aslkd jl;");
      QTreeWidgetItem* item_no_2 = new QTreeWidgetItem(item_no);
      item_no_2->setText(0, "No - Part 2");
      QTreeWidgetItem* item_no_3 = new QTreeWidgetItem(item_no);
      item_no_3->setText(0, "No - Part 3");
      QTreeWidgetItem* item_maybe = new QTreeWidgetItem(first_layer_3);
      item_maybe->setText(0, "Maybe");

      //QTreeWidgetItem *cities = new QTreeWidgetItem(treeWidget);
      //cities->setText(0, tr("Cities"));
      //QTreeWidgetItem *osloItem = new QTreeWidgetItem(cities);
      //osloItem->setText(0, tr("Oslo"));
      //osloItem->setText(1, tr("Yes"));

      // TODO: ALL OF CONVERSATION
    }
    else if(event->getEventType() == EventClassifier::TELEPORTTHING)
    {
      /* Attempt to find the map name in the combo box */
      QString name = "";
      for(int i = 0; i < list_submaps.size(); i++)
      {
        QStringList set = list_submaps[i].split(":");
        if(set.size() == 2)
          if(set.front().toInt() == event->getTeleportSection())
            name = set.last();
      }

      /* Load the name and information */
      tele_map->setText(QString::number(event->getTeleportSection()) +
                        ": " + name + "  | X: " +
                        QString::number(event->getTeleportX()) + " | Y: " +
                        QString::number(event->getTeleportY()));

      /* Attempt to find the thing in the combo box */
      int index = -1;
      for(int i = 0; i < list_things.size(); i++)
      {
        QStringList set = list_things[i].split(":");
        if(set.size() == 2)
        {
          if(set.front().toInt() == event->getTeleportThingID())
          {
            tele_thing->setCurrentIndex(i);
            index = i;
          }
        }
      }

      /* If index < 0 (not found), set to first */
      if(index < 0)
        tele_thing->setCurrentIndex(0);
    }

    /* Enable the widget */
    setEnabled(true);
  }
  else
  {
    combo_category->setCurrentIndex(0);
    setDisabled(true);
  }
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

/* Category changed */
void EventView::categoryChanged(int index)
{
//  /* Message box - warning of change */
//  QMessageBox msg_box;
//  msg_box.setInformativeText("Are you sure you want to change event?");
//  msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
//  msg_box.setDefaultButton(QMessageBox::No);
//  int ret = msg_box.exec();

//  /* Parse the return value */
//  if(ret == QMessageBox::Yes)
//  {
    view_stack->setCurrentIndex(index);

    if(event != NULL)
    {
      if((int)event->getEventType() != index)
      {
        /* Change the event */
        if(index == (int)EventClassifier::GIVEITEM)
          event->setEventGiveItem();
        else if(index == (int)EventClassifier::NOEVENT)
          event->setEventBlank();
        else if(index == (int)EventClassifier::NOTIFICATION)
          event->setEventNotification();
        else if(index == (int)EventClassifier::RUNBATTLE)
          event->setEventStartBattle();
        else if(index == (int)EventClassifier::RUNMAP)
          event->setEventStartMap();
        else if(index == (int)EventClassifier::STARTCONVO)
          event->setEventConversation();
        else if(index == (int)EventClassifier::TELEPORTTHING)
          event->setEventTeleport();
      }

      /* Update the layout */
      setLayoutData();
    }
//  }
//  else
//  {
//    combo_category->blockSignals(true);
//    combo_category->setCurrentIndex(view_stack->currentIndex());
//    combo_category->blockSignals(false);
//  }
}

/* The change map button press */
void EventView::changeMapChanged(int index)
{
  if(index >= 0 && index < list_maps.size())
  {
    QStringList list = list_maps[index].split(":");
    if(list.size() == 2)
      event->setEventStartMap(list.front().toInt());
  }
}

/* The give item event slot changes */
void EventView::giveCountChanged(int index)
{
  event->setEventGiveItem(event->getGiveItemID(), index);
}

/* The give item event slot changes */
void EventView::giveItemChanged(int index)
{
  if(index >= 0 && index < list_items.size())
  {
    QStringList list = list_items[index].split(":");
    if(list.size() == 2)
      event->setEventGiveItem(list.front().toInt(), event->getGiveItemCount());
  }
}

/* The notification event text changes */
void EventView::notificationTextChanged()
{
  event->setEventNotification(notification_edit->toPlainText());
}

/* Resize the QTreeWidget for the items */
void EventView::resizeTree(QTreeWidgetItem*)
{
  convo_tree->resizeColumnToContents(0);
}

/* The teleport event button presses */
void EventView::teleportMapPressed()
{
  emit selectTile();
}

/* The teleport event button presses */
void EventView::teleportThingChanged(int index)
{
  if(index >= 0 && index < list_things.size())
  {
    QStringList list = list_things[index].split(":");
    if(list.size() == 2)
      event->setEventTeleport(list.front().toInt(), event->getTeleportSection(),
                              event->getTeleportX(), event->getTeleportY());
  }
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Returns the event */
EditorEvent* EventView::getEvent()
{
  return event;
}
/* Returns the list of objects, used for event creation */
QVector<QString> EventView::getListItems()
{
  return list_items;
}

/* Returns the list of objects, used for event creation */
QVector<QString> EventView::getListMaps()
{
  return list_maps;
}

/* Returns the list of objects, used for event creation */
QVector<QString> EventView::getListSubmaps()
{
  return list_submaps;
}

/* Returns the list of objects, used for event creation */
QVector<QString> EventView::getListThings()
{
  return list_things;
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

/* Sets the list of objects, used for event creation */
void EventView::setListItems(QVector<QString> items)
{
  list_items = items;
  item_name->blockSignals(true);
  item_name->clear();
  for(int i = 0; i < list_items.size(); i++)
    item_name->addItem(list_items[i]);
  setLayoutData();
  item_name->blockSignals(false);
}

/* Sets the list of objects, used for event creation */
void EventView::setListMaps(QVector<QString> maps)
{
  list_maps = maps;
  map_name->blockSignals(true);
  map_name->clear();
  for(int i = 0; i < list_maps.size(); i++)
    map_name->addItem(list_maps[i]);
  setLayoutData();
  map_name->blockSignals(false);
}

/* Sets the list of objects, used for event creation */
void EventView::setListSubmaps(QVector<QString> sub_maps)
{
  list_submaps = sub_maps;
  setLayoutData();
}

/* Sets the list of objects, used for event creation */
void EventView::setListThings(QVector<QString> things)
{
  list_things = things;
  tele_thing->blockSignals(true);
  tele_thing->clear();
  for(int i = 0; i < list_things.size(); i++)
    tele_thing->addItem(list_things[i]);
  setLayoutData();
  tele_thing->blockSignals(false);
}

/* Update the selected tile for the thing */
void EventView::updateSelectedTile(int id, int x, int y)
{
  if(event != NULL)
  {
    event->setEventTeleport(event->getTeleportThingID(), id, x, y);
    setLayoutData();
  }
}
