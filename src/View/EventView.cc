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
EventView::EventView(EditorEvent* event, QWidget* parent,
                     bool conversation_enabled)
         : QFrame(parent)
{
  /* Initialize variables */
  database = NULL;
  this->event = NULL;
  rightclick_index = "";

  /* Create the layout */
  createLayout(conversation_enabled);

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
void EventView::createLayout(bool conversation_enabled)
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
  if(conversation_enabled)
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
  QWidget* widget_convo;
  if(conversation_enabled)
  {
    widget_convo = new QWidget(this);
    convo_tree = new QTreeWidget(this);
    convo_tree->setContextMenuPolicy(Qt::CustomContextMenu);
    convo_tree->setMinimumWidth(320);
    convo_tree->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    convo_tree->header()->hide();
    connect(convo_tree, SIGNAL(itemCollapsed(QTreeWidgetItem*)),
            this, SLOT(resizeTree(QTreeWidgetItem*)));
    connect(convo_tree, SIGNAL(itemExpanded(QTreeWidgetItem*)),
            this, SLOT(resizeTree(QTreeWidgetItem*)));
    connect(convo_tree, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(convoMenuRequested(QPoint)));
    connect(convo_tree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
            this, SLOT(convoDoubleClick(QTreeWidgetItem*,int)));
    QVBoxLayout* layout_convo = new QVBoxLayout(widget_convo);
    layout_convo->addWidget(convo_tree, 0, Qt::AlignCenter);

    /* Right click menu control */
    rightclick_menu = new QMenu("Convo Edit", this);
    QAction* action_edit = new QAction("Edit", rightclick_menu);
    connect(action_edit, SIGNAL(triggered()), this, SLOT(rightClickEdit()));
    rightclick_menu->addAction(action_edit);
    QAction* action_before = new QAction("Insert Before", rightclick_menu);
    connect(action_before, SIGNAL(triggered()),
            this, SLOT(rightClickInsertBefore()));
    rightclick_menu->addAction(action_before);
    QAction* action_after = new QAction("Insert After", rightclick_menu);
    connect(action_after, SIGNAL(triggered()),
            this, SLOT(rightClickInsertAfter()));
    rightclick_menu->addAction(action_after);
    action_option = new QAction("Insert Option", rightclick_menu);
    connect(action_option, SIGNAL(triggered()),
            this, SLOT(rightClickInsertOption()));
    rightclick_menu->addAction(action_option);
    action_delete = new QAction("Delete", rightclick_menu);
    connect(action_delete, SIGNAL(triggered()), this, SLOT(rightClickDelete()));
    rightclick_menu->addAction(action_delete);
    rightclick_menu->hide();
  }

  /* Stacked widget for housing all the different views for categories */
  view_stack = new QStackedWidget(this);
  view_stack->addWidget(widget_unset);
  view_stack->addWidget(widget_give);
  view_stack->addWidget(widget_notification);
  view_stack->addWidget(widget_battle);
  view_stack->addWidget(widget_map);
  view_stack->addWidget(widget_teleport);
  if(conversation_enabled)
    view_stack->addWidget(widget_convo);
  layout->addWidget(view_stack, 1, Qt::AlignCenter);

  /* Configure parent widget */
  setFrameStyle(QFrame::Panel);
  setLayout(layout);
  setLineWidth(1);
  setMaximumSize(352, 192);
  setMinimumSize(352, 192);
}

/* Returns the convesation item, based on the index, of the tree widget */
QTreeWidgetItem* EventView::getConvo(QString base_index)
{
  QTreeWidgetItem* item = NULL;

  if(!base_index.isEmpty())
  {
    QStringList list = base_index.split(".");
    QTreeWidgetItem* ref = NULL;

    for(int i = 0; i < list.size(); i++)
    {
      if(ref == NULL && i == 0)
        ref = convo_tree->topLevelItem(list[i].toInt() - 1);
      else if(ref != NULL && i != 0)
        ref = ref->child(list[i].toInt() - 1);
    }

    item = ref;
  }

  return item;
}

/* Returns the conversation index of the tree widget */
QString EventView::getConvoIndex(QTreeWidgetItem* ref, QTreeWidgetItem* parent)
{
  /* If parent is NULL, start at head element of list */
  if(parent == NULL)
  {
    for(int i = 0; i < convo_tree->topLevelItemCount(); i++)
    {
      if(convo_tree->topLevelItem(i) == ref)
      {
        return QString::number(i+1);
      }
      else
      {
        QString result = getConvoIndex(ref, convo_tree->topLevelItem(i));
        if(!result.isEmpty())
          return (QString::number(i+1) + result);
      }
    }
  }
  /* Otherwise, use parent tree widget item */
  else
  {
    for(int i = 0; i < parent->childCount(); i++)
    {
      if(parent->child(i) == ref)
      {
        return ("." + QString::number(i+1));
      }
      else
      {
        QString result = getConvoIndex(ref, parent->child(i));
        if(!result.isEmpty())
          return ("." + QString::number(i+1) + result);
      }
    }
  }

  return "";
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
      /* Clear the tree */
      convo_tree->clear();

      /* Conversation reference */
      QTreeWidgetItem* parent = NULL;
      Conversation* ref = event->getConversation();

      /* Proceed if not null */
      if(ref != NULL)
      {
        /* Recursive add */
        updateConvoTree(ref, parent);
        convo_tree->resizeColumnToContents(0);
      }
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
      for(int i = 0; (index < 0) && (i < list_things.size()); i++)
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

/* Update conversation data */
void EventView::updateConvoTree(Conversation* ref, QTreeWidgetItem* parent,
                                bool option)
{
  bool is_option = false;

  /* Create the item */
  QTreeWidgetItem* item;
  if(parent != NULL)
    item = new QTreeWidgetItem(parent);
  else
    item = new QTreeWidgetItem(convo_tree);

  /* Generate the text */
  QString text = "";
  if(ref->action_event.classification != EventClassifier::NOEVENT)
    text += "@";
  if(option)
    item->setText(0, text + QString::fromStdString(ref->text));
  else
    item->setText(0, text + QString::number(ref->thing_id) + ": " +
                     QString::fromStdString(ref->text));

  /* Change the parent, if relevant for the next */
  if(ref->next.size() > 1)
  {
    parent = item;
    is_option = true;
  }
  else if(option)
  {
    parent = item;
  }

  /* Loop through and add next convos */
  for(uint32_t i = 0; i < ref->next.size(); i++)
  {
    updateConvoTree(&ref->next[i], parent, is_option);
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

/* Conversation item double clicked trigger */
void EventView::convoDoubleClick(QTreeWidgetItem*, int)
{
  QTreeWidgetItem* selected = convo_tree->currentItem();
  if(selected != NULL)
  {
    rightclick_index = getConvoIndex(selected);
    rightClickEdit();
  }
}

/* The convo menu requested - on right click */
void EventView::convoMenuRequested(QPoint point)
{
  QTreeWidgetItem* clicked_item = convo_tree->itemAt(point);
  if(clicked_item != NULL)
  {
    /* Get the index */
    QString base_index = getConvoIndex(clicked_item);
    QString item_index = EditorEvent::convertConversationIndex(base_index);
    if(!item_index.isEmpty())
    {
      /* Enable/disable actions */
      if(item_index != "1")
        action_delete->setEnabled(true);
      else
        action_delete->setDisabled(true);
      if(EditorEvent::couldBeOption(base_index, clicked_item->childCount()))
        action_option->setEnabled(true);
      else
        action_option->setDisabled(true);

      /* Execute the cursor */
      rightclick_index = base_index;
      rightclick_menu->exec(QCursor::pos());
    }
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

/* The right click slots, for conversation */
void EventView::rightClickDelete()
{
  if(!rightclick_index.isEmpty())
  {
    /* Message box - warning of change */
    QMessageBox msg_box;
    msg_box.setText("This will delete the node and all children.");
    msg_box.setInformativeText("Are you sure?");
    msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msg_box.setDefaultButton(QMessageBox::No);
    int ret = msg_box.exec();

    /* Parse the return value */
    if(ret == QMessageBox::Yes)
    {
      QString index = EditorEvent::convertConversationIndex(rightclick_index);
      QString new_index = event->deleteConversation(index);
      setLayoutData();

      /* Determine the modified index */
      QString modified_index = "";
      QStringList mod_list_1 = index.split(".");
      QStringList mod_list_2 = new_index.split(".");
      QStringList right_list = rightclick_index.split(".");
      for(int i = 0; i < right_list.size() - 1; i++)
        modified_index += right_list[i] + ".";
      if(mod_list_1.size() > mod_list_2.size())
      {
        if(right_list.last().toInt() > 1)
          modified_index += QString::number(right_list.last().toInt() - 1);
        else
          modified_index.chop(1);
      }
      else if(mod_list_1.last().toInt() != mod_list_2.last().toInt())
      {
        if(right_list.last().toInt() > 1)
          modified_index += QString::number(right_list.last().toInt() - 1);
        else
          modified_index.chop(1);
      }
      else if(index == new_index)
      {
        if(right_list.size() == 1)
          modified_index = QString::number(right_list.last().toInt() + 1);
        else
        {
          modified_index.chop(1);
        }
      }

      /* Select the closest item */
      QTreeWidgetItem* new_select = getConvo(modified_index);
      convo_tree->setCurrentItem(new_select);
    }
  }

  rightclick_index = "";
}

/* The right click slots, for conversation */
void EventView::rightClickEdit()
{
  if(!rightclick_index.isEmpty())
  {
    QString index = EditorEvent::convertConversationIndex(rightclick_index);
    Conversation* ref = event->getConversation(index);
    if(ref != NULL)
    {
      /* Determine if it's an option */
      bool is_option = false;
      QStringList list = rightclick_index.split(".");
      if(list.size() % 2 == 0)
        is_option = true;

      emit editConversation(ref, is_option);
    }
  }

  rightclick_index = "";
}

/* The right click slots, for conversation */
void EventView::rightClickInsertAfter()
{
  if(!rightclick_index.isEmpty())
  {
    Conversation after = EventHandler::createEmptyConversation();
    after.text = "New Entry - After";
    QString index = EditorEvent::convertConversationIndex(rightclick_index);
    QString new_index = event->insertConversationAfter(index, after);
    setLayoutData();

    /* Select the previously selected item */
    QTreeWidgetItem* new_select = getConvo(rightclick_index);
    convo_tree->setCurrentItem(new_select);
    if(new_select->childCount() > 0)
      new_select->setExpanded(true);
  }

  rightclick_index = "";
}

/* The right click slots, for conversation */
void EventView::rightClickInsertBefore()
{
  if(!rightclick_index.isEmpty())
  {
    Conversation before = EventHandler::createEmptyConversation();
    before.text = "New Entry - Before";
    QString index = EditorEvent::convertConversationIndex(rightclick_index);
    QString new_index = event->insertConversationBefore(index, before);
    setLayoutData();

    /* Select the previously selected item */
    QTreeWidgetItem* new_select = getConvo(rightclick_index);
    convo_tree->setCurrentItem(new_select);
  }

  rightclick_index = "";
}

/* The right click slots, for conversation */
void EventView::rightClickInsertOption()
{
  if(!rightclick_index.isEmpty())
  {
    QString index = EditorEvent::convertConversationIndex(rightclick_index);
    QStringList list = index.split('.');
    QStringList orig_list = rightclick_index.split('.');
    if(list.last() == "1")
    {
      /* Set-up new index for insertion */
      QString new_index = "";
      for(int i = 0; i < list.size() - 1; i++)
        new_index += list[i] + ".";
      new_index += "2";

      /* Set the new conversation option */
      Conversation option = EventHandler::createEmptyConversation();
      option.text = "New Entry - Option";
      event->setConversation(new_index, option);
      setLayoutData();

      /* Set-up new index for selection */
      QString select_index = "";
      for(int i = 0; i < orig_list.size() - 1; i++)
        select_index += orig_list[i] + ".";
      select_index += QString::number(orig_list.last().toInt() - 1) + ".2";

      /* Select the previously selected (now an option) */
      QTreeWidgetItem* new_select = getConvo(select_index);
      convo_tree->setCurrentItem(new_select);
    }
  }

  rightclick_index = "";
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

/* Update the layout trigger */
void EventView::updateConversation()
{
  if(event->getEventType() == EventClassifier::STARTCONVO)
  {
    QString index = getConvoIndex(convo_tree->currentItem());
    setLayoutData();
    convo_tree->setCurrentItem(getConvo(index));
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