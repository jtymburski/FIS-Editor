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

/*
 * Description: Constructor function for instantiating the event view with an
 *              EditorEvent and a parent widget.
 *
 * Inputs: EditorEvent* event - the event controller for the Event from handler
 *         QWidget* parent - the parent widget
 *         bool conversation_enabled - is the conversation option available??
 */
EventView::EventView(EditorEvent* event, QWidget* parent,
                     bool conversation_enabled)
         : QFrame(parent)
{
  /* Initialize variables */
  //database = NULL;
  this->event = NULL;
  rightclick_index = "";

  /* Create the layout */
  createLayout(conversation_enabled);

  /* Set the event */
  setEvent(event);
}

/*
 * Description: Destructor function
 */
EventView::~EventView()
{
  disconnect(combo_category, SIGNAL(currentIndexChanged(int)),
             this, SLOT(categoryChanged(int)));
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: Creates the dialog layout with QT functional widgets.
 *
 * Inputs: bool conversation_enabled - is the conversation widget enabled?
 * Output: none
 */
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
  combo_category->addItem("Just Sound");
  combo_category->addItem("Take Item");
  combo_category->addItem("Unlock: Thing");
  combo_category->addItem("Unlock: Tile");
  combo_category->addItem("Unlock: IO");
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
  layout_give->setRowStretch(0, 1);
  layout_give->addWidget(lbl_give_item, 1, 0);
  layout_give->addWidget(item_name, 1, 1, 1, 2);
  layout_give->addWidget(lbl_give_count, 2, 0);
  layout_give->addWidget(item_count, 2, 1);
  layout_give->setColumnStretch(2, 1);
  layout_give->setRowStretch(3, 1);

  /* Widget for notification control */
  QWidget* widget_notification = new QWidget(this);
  notification_edit = new QTextEdit(this);
  notification_edit->setMinimumWidth(320);
  connect(notification_edit, SIGNAL(textChanged()),
          this, SLOT(notificationTextChanged()));
  QVBoxLayout* layout_notification = new QVBoxLayout(widget_notification);
  layout_notification->addWidget(notification_edit);

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
  layout_tele->setRowStretch(0, 1);
  layout_tele->addWidget(lbl_tele_map, 1, 0);
  layout_tele->addWidget(tele_map, 1, 1);
  layout_tele->addWidget(btn_tele_map, 1, 2);
  layout_tele->addWidget(lbl_tele_thing, 2, 0);
  layout_tele->addWidget(tele_thing, 2, 1);
  layout_tele->setRowStretch(3, 1);

  /* Widget for sound control */
  QWidget* widget_sound = new QWidget(this);
  QLabel* lbl_sound = new QLabel("NO SETTINGS", this);
  QVBoxLayout* layout_sound = new QVBoxLayout(widget_sound);
  layout_sound->addWidget(lbl_sound, 0, Qt::AlignCenter);

  /* Widget for take item control */
  QWidget* widget_take = new QWidget(this);
  QLabel* lbl_take_item = new QLabel("Item:", this);
  QLabel* lbl_take_count = new QLabel("Count:", this);
  take_name = new QComboBox(this);
  take_name->setMinimumWidth(200);
  connect(take_name, SIGNAL(currentIndexChanged(int)),
          this, SLOT(takeItemChanged(int)));
  take_count = new QSpinBox(this);
  take_count->setMinimum(1);
  take_count->setMaximum(100000);
  connect(take_count, SIGNAL(valueChanged(int)),
          this, SLOT(takeCountChanged(int)));
  QGridLayout* layout_take = new QGridLayout(widget_take);
  layout_take->setRowStretch(0, 1);
  layout_take->addWidget(lbl_take_item, 1, 0);
  layout_take->addWidget(take_name, 1, 1, 1, 2);
  layout_take->addWidget(lbl_take_count, 2, 0);
  layout_take->addWidget(take_count, 2, 1);
  layout_take->setColumnStretch(2, 1);
  layout_take->setRowStretch(3, 1);

  /* Widget for unlock thing control */
  QWidget* widget_unlock_thing = new QWidget(this);
  //widget_unlock_thing->setDisabled(true); // TODO: Remove when implemented
  QLabel* lbl_unth = new QLabel("Thing:", this);
  unth_name = new QComboBox(this);
  connect(unth_name, SIGNAL(currentIndexChanged(QString)),
          this, SLOT(unlockThingChanged(QString)));
  /* -- */
  QGroupBox* unth_view = new QGroupBox("View", this);
  unth_view_enable = new QCheckBox("GoTo Unlock", unth_view);
  connect(unth_view_enable, SIGNAL(stateChanged(int)),
          this, SLOT(unlockThingView(int)));
  unth_view_scroll = new QCheckBox("Scroll", unth_view);
  unth_view_scroll->setDisabled(true);
  connect(unth_view_scroll, SIGNAL(stateChanged(int)),
          this, SLOT(unlockThingViewScroll(int)));
  QLabel* lbl_unth_view = new QLabel("Time at Unlock", this);
  unth_view_time = new QSpinBox(this);
  unth_view_time->setMaximum(1000000);
  unth_view_time->setDisabled(true);
  connect(unth_view_time, SIGNAL(valueChanged(int)),
          this, SLOT(unlockThingViewTime(int)));
  QGridLayout* layout_unth_view = new QGridLayout(unth_view);
  QMargins m = layout_unth_view->contentsMargins();
  layout_unth_view->setContentsMargins(m.left(), 0, m.right(), 0);
  layout_unth_view->addWidget(unth_view_enable, 0, 0);
  layout_unth_view->addWidget(unth_view_scroll, 0, 1);
  layout_unth_view->addWidget(lbl_unth_view, 1, 0);
  layout_unth_view->addWidget(unth_view_time, 1, 1);
  /* -- */
  QGridLayout* layout_unth = new QGridLayout(widget_unlock_thing);
  layout_unth->setContentsMargins(m.left(), 0, m.right(), 0);
  layout_unth->setRowStretch(0, 1);
  layout_unth->addWidget(lbl_unth, 1, 0);
  layout_unth->addWidget(unth_name, 1, 1, 1, 3);
  layout_unth->addWidget(unth_view, 2, 0, 1, 4);
  layout_unth->setRowStretch(3, 1);

  /* Widget for unlock tile control */
  QWidget* widget_unlock_tile = new QWidget(this);
  //widget_unlock_tile->setDisabled(true); // TODO: Remove when implemented
  QLabel* lbl_unti = new QLabel("Tile:", this);
  unti_location = new QLineEdit("", this);
  unti_location->setDisabled(true);
  QPalette pal2 = unti_location->palette();
  pal2.setColor(QPalette::Disabled, QPalette::Text,
                pal2.color(QPalette::Active, QPalette::Text));
  unti_location->setPalette(pal2);
  QPushButton* btn_unti_map = new QPushButton(this);
  connect(btn_unti_map, SIGNAL(clicked()), this, SLOT(unlockTilePressed()));
  btn_unti_map->setIcon(QIcon(":/images/icons/32_settings.png"));
  btn_unti_map->setIconSize(QSize(24,24));
  btn_unti_map->setMaximumSize(30, 30);
  /* -- */
  QGroupBox* unti_event = new QGroupBox("Events", this);
  unti_event_enter = new QCheckBox("Enter", unti_event);
  connect(unti_event_enter, SIGNAL(stateChanged(int)),
          this, SLOT(unlockTileEnter(int)));
  unti_event_exit = new QCheckBox("Exit", unti_event);
  connect(unti_event_exit, SIGNAL(stateChanged(int)),
          this, SLOT(unlockTileExit(int)));
  QGridLayout* layout_unti_event = new QGridLayout(unti_event);
  layout_unti_event->setContentsMargins(m.left(), 0, m.right(), 0);
  layout_unti_event->addWidget(unti_event_enter, 0, 0);
  layout_unti_event->addWidget(unti_event_exit, 0, 1);
  /* -- */
  QGroupBox* unti_view = new QGroupBox("View", this);
  unti_view_enable = new QCheckBox("GoTo Unlock", unti_view);
  connect(unti_view_enable, SIGNAL(stateChanged(int)),
          this, SLOT(unlockTileView(int)));
  unti_view_scroll = new QCheckBox("Scroll", unti_view);
  unti_view_scroll->setDisabled(true);
  connect(unth_view_scroll, SIGNAL(stateChanged(int)),
          this, SLOT(unlockTileViewScroll(int)));
  QLabel* lbl_unti_view = new QLabel("Time at Unlock", this);
  unti_view_time = new QSpinBox(this);
  unti_view_time->setMaximum(1000000);
  unti_view_time->setDisabled(true);
  connect(unti_view_time, SIGNAL(valueChanged(int)),
          this, SLOT(unlockTileViewTime(int)));
  QGridLayout* layout_unti_view = new QGridLayout(unti_view);
  layout_unti_view->setContentsMargins(m.left(), 0, m.right(), 0);
  layout_unti_view->addWidget(unti_view_enable, 0, 0);
  layout_unti_view->addWidget(unti_view_scroll, 0, 1);
  layout_unti_view->addWidget(lbl_unti_view, 1, 0);
  layout_unti_view->addWidget(unti_view_time, 1, 1);
  /* -- */
  QGridLayout* layout_unti = new QGridLayout(widget_unlock_tile);
  layout_unti->setContentsMargins(m.left(), 0, m.right(), 0);
  layout_unti->setRowStretch(0, 1);
  layout_unti->addWidget(lbl_unti, 1, 0);
  layout_unti->addWidget(unti_location, 1, 1, 1, 2);
  layout_unti->addWidget(btn_unti_map, 1, 3);
  layout_unti->addWidget(unti_event, 2, 0, 1, 4);
  layout_unti->addWidget(unti_view, 3, 0, 1, 4);
  layout_unti->setRowStretch(4, 1);
  layout_unti->setColumnStretch(1, 1);

  /* Widget for unlock io control */
  QWidget* widget_unlock_io = new QWidget(this);
  widget_unlock_io->setDisabled(true); // TODO: Remove when implemented
  QLabel* lbl_unio = new QLabel("IO:", this);
  unio_name = new QComboBox(this);
  connect(unio_name, SIGNAL(currentIndexChanged(QString)),
          this, SLOT(unlockIOChanged(QString)));
  /* -- */
  QGroupBox* unio_mode = new QGroupBox("Mode", this);
  unio_mode_lock = new QCheckBox("Main Lock", unio_mode);
  connect(unio_mode_lock, SIGNAL(stateChanged(int)),
          this, SLOT(unlockIOModeLock(int)));
  unio_mode_states = new QCheckBox("States", unio_mode);
  connect(unio_mode_states, SIGNAL(stateChanged(int)),
          this, SLOT(unlockIOModeStates(int)));
  QHBoxLayout* layout_unio_mode = new QHBoxLayout(unio_mode);
  layout_unio_mode->setContentsMargins(m.left(), 0, m.right(), 0);
  layout_unio_mode->addWidget(unio_mode_lock);
  layout_unio_mode->addWidget(unio_mode_states);
  /* -- */
  QLabel* lbl_unio_state = new QLabel("State:", this);
  unio_state = new QComboBox(this);
  unio_state->setDisabled(true);
  connect(unio_state, SIGNAL(currentIndexChanged(int)),
          this, SLOT(unlockIOStateChanged(int)));
  /* -- */
  unio_event = new QGroupBox("Events", this);
  unio_event->setDisabled(true);
  unio_event_enter = new QCheckBox("Enter", unio_event);
  connect(unio_event_enter, SIGNAL(stateChanged(int)),
          this, SLOT(unlockIOStateEnter(int)));
  unio_event_exit = new QCheckBox("Exit", unio_event);
  connect(unio_event_exit, SIGNAL(stateChanged(int)),
          this, SLOT(unlockIOStateExit(int)));
  unio_event_use = new QCheckBox("Use", unio_event);
  connect(unio_event_use, SIGNAL(stateChanged(int)),
          this, SLOT(unlockIOStateUse(int)));
  unio_event_walk = new QCheckBox("Walkover", unio_event);
  connect(unio_event_walk, SIGNAL(stateChanged(int)),
          this, SLOT(unlockIOStateWalk(int)));
  QGridLayout* layout_unio_event = new QGridLayout(unio_event);
  layout_unio_event->setContentsMargins(m.left(), 0, m.right(), 0);
  layout_unio_event->addWidget(unio_event_enter, 0, 0);
  layout_unio_event->addWidget(unio_event_exit, 0, 1);
  layout_unio_event->addWidget(unio_event_use, 1, 0);
  layout_unio_event->addWidget(unio_event_walk, 1, 1);
  /* -- */
  QGroupBox* unio_view = new QGroupBox("View", this);
  unio_view_enable = new QCheckBox("GoTo Unlock", unio_view);
  connect(unio_view_enable, SIGNAL(stateChanged(int)),
          this, SLOT(unlockIOView(int)));
  unio_view_scroll = new QCheckBox("Scroll", unio_view);
  unio_view_scroll->setDisabled(true);
  connect(unio_view_scroll, SIGNAL(stateChanged(int)),
          this, SLOT(unlockIOViewScroll(int)));
  QLabel* lbl_unio_view = new QLabel("Time at Unlock", this);
  unio_view_time = new QSpinBox(this);
  unio_view_time->setMaximum(1000000);
  unio_view_time->setDisabled(true);
  connect(unio_view_time, SIGNAL(valueChanged(int)),
          this, SLOT(unlockIOViewTime(int)));
  QGridLayout* layout_unio_view = new QGridLayout(unio_view);
  layout_unio_view->setContentsMargins(m.left(), 0, m.right(), 0);
  layout_unio_view->addWidget(unio_view_enable, 0, 0);
  layout_unio_view->addWidget(unio_view_scroll, 0, 1);
  layout_unio_view->addWidget(lbl_unio_view, 1, 0);
  layout_unio_view->addWidget(unio_view_time, 1, 1);
  /* -- */
  QGridLayout* layout_unio = new QGridLayout(widget_unlock_io);
  layout_unio->setContentsMargins(m.left(), 0, m.right(), 0);
  layout_unio->addWidget(lbl_unio, 0, 0);
  layout_unio->addWidget(unio_name, 0, 1, 1, 3);
  layout_unio->addWidget(unio_mode, 1, 0, 1, 4);
  layout_unio->addWidget(lbl_unio_state, 2, 0);
  layout_unio->addWidget(unio_state, 2, 1, 1, 3);
  layout_unio->addWidget(unio_event, 3, 0, 1, 4);
  layout_unio->addWidget(unio_view, 4, 0, 1, 4);

  /* Widget for conversation control */
  QWidget* widget_convo;
  if(conversation_enabled)
  {
    widget_convo = new QWidget(this);
    convo_tree = new QTreeWidget(this);
    convo_tree->setContextMenuPolicy(Qt::CustomContextMenu);
    //convo_tree->setMinimumWidth(320);
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
    layout_convo->addWidget(convo_tree);

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
  view_stack->addWidget(widget_sound);
  view_stack->addWidget(widget_take);
  view_stack->addWidget(widget_unlock_thing);
  view_stack->addWidget(widget_unlock_tile);
  view_stack->addWidget(widget_unlock_io);
  if(conversation_enabled)
    view_stack->addWidget(widget_convo);
  layout->addWidget(view_stack, 1, Qt::AlignCenter);

  /* Sound widget */
  QHBoxLayout* sound_layout = new QHBoxLayout();
  sound_layout->setContentsMargins(8, 0, 8, 0);
  QLabel* lbl_sound_2 = new QLabel("Sound:", this);
  sound_layout->addWidget(lbl_sound_2);
  combo_sound = new QComboBox(this);
  combo_sound->setDisabled(true);
  connect(combo_sound, SIGNAL(currentIndexChanged(QString)),
          this, SLOT(changedSound(QString)));
  sound_layout->addWidget(combo_sound, 1);
  layout->addLayout(sound_layout);

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
 * Description: Returns the conversation item based on the base index. This
 *              parses the tree to find the item. The base index is in the
 *              form: 1.5.7, not 1.5.1.1.1.1.1.1.1.
 *
 * Inputs: QString base_index - the base index representation of the segment
 * Output: QTreeWidgetItem* - the found corresponding item. NULL if failed.
 */
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

/*
 * Description: Returns the conversation index of the reference item
 *              corresponding to the parent item. This is a recursive call and
 *              by taking an item in the widget, and NULL parent, it searches
 *              the entire tree and returns the base index of the item. Opposite
 *              call of getConvo().
 *
 * Inputs: QTreeWidgetItem* ref - the reference searching for
 *         QTreeWidgetItem* parent - the previous call reference
 * Output: QString - the resulting base index
 */
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

/*
 * Description: Updates the data in the widgets. CreateLayout() must be called
 *              prior.
 *
 * Inputs: none
 * Output: none
 */
void EventView::setLayoutData()
{
  if(event != NULL)
  {
    /* Set the selected category */
    if((int)event->getEventType() != combo_category->currentIndex())
    {
      //combo_category->blockSignals(true);
      combo_category->setCurrentIndex((int)event->getEventType());
      //int index = (int)event->getEventType();
      //combo_category->setCurrentIndex(index);
      //view_stack->setCurrentIndex(index);
      //combo_sound->setEnabled(index != (int)EventClassifier::NOEVENT);

      //combo_category->blockSignals(false);
    }

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
      {
        item_name->setCurrentIndex(0);
        if(list_items.size() > 0)
          giveItemChanged(0);
      }
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
      {
        map_name->setCurrentIndex(0);
        if(list_maps.size() > 0)
          changeMapChanged(0);
      }
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
    if(event->getEventType() == EventClassifier::TAKEITEM)
    {
      take_count->setValue(event->getTakeItemCount());

      /* Attempt to find the item name in the combo box */
      int index = -1;
      for(int i = 0; i < list_items.size(); i++)
      {
        QStringList set = list_items[i].split(":");
        if(set.size() == 2)
        {
          if(set.front().toInt() == event->getTakeItemID())
          {
            take_name->setCurrentIndex(i);
            index = i;
          }
        }
      }

      /* If index < 0 (not found), set to first */
      if(index < 0)
      {
        take_name->setCurrentIndex(0);
        if(list_items.size() > 0)
          takeItemChanged(0);
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
      {
        tele_thing->setCurrentIndex(0);
        if(list_things.size() > 0)
          teleportThingChanged(0);
      }
    }
    else if(event->getEventType() == EventClassifier::UNLOCKTHING)
    {
      /* Attempt to find thing in combo box */
      int index = -1;
      for(int i = 0; (index < 0) && (i < list_things_no_io.size()); i++)
      {
        QStringList set = list_things_no_io[i].split(":");
        if(set.size() == 2)
          if(set.front().toInt() == event->getUnlockThingID())
            index = i;
      }
      if(index >= 0)
      {
        unth_name->setCurrentIndex(index);
      }
      else
      {
        unth_name->setCurrentIndex(0);
        if(list_things_no_io.size() > 0)
          unlockThingChanged(list_things_no_io.front());
      }

      /* View data */
      bool view, scroll;
      EventSet::dataEnumView(event->getUnlockViewMode(), view, scroll);
      unth_view_enable->setChecked(view);
      unth_view_scroll->setChecked(scroll);
      unth_view_time->setValue(event->getUnlockViewTime());
    }
    else if(event->getEventType() == EventClassifier::UNLOCKTILE)
    {
      /* Attempt to find the map name */
      QString name = "";
      for(int i = 0; i < list_submaps.size(); i++)
      {
        QStringList set = list_submaps[i].split(":");
        if(set.size() == 2)
          if(set.front().toInt() == event->getUnlockTileSection())
            name = set.last();
      }

      /* Load the name and information */
      unti_location->setText(QString::number(event->getUnlockTileSection()) +
                             ": " + name + "  | X: " +
                             QString::number(event->getUnlockTileX()) +
                             " | Y: " +
                             QString::number(event->getUnlockTileY()));

      /* Event data */
      bool enter, exit;
      EventSet::dataEnumTileEvent(event->getUnlockTileMode(), enter, exit);
      unti_event_enter->setChecked(enter);
      unti_event_exit->setChecked(exit);

      /* View data */
      bool view, scroll;
      EventSet::dataEnumView(event->getUnlockViewMode(), view, scroll);
      unti_view_enable->setChecked(view);
      unti_view_scroll->setChecked(scroll);
      unti_view_time->setValue(event->getUnlockViewTime());
    }
    else if(event->getEventType() == EventClassifier::UNLOCKIO)
    {
      /* Update state to combo */
//      int index = unio_name->currentIndex();
//      unio_state->clear();
//      if(index >= 0 && index < list_ios.size())
//      {
//        QStringList split_set = list_ios[index].second.split(',');
//        unio_state->addItem("-1: All States");
//        for(int i = 0; i < split_set.size(); i++)
//          unio_state->addItem(split_set[i] + ": State");
//      }

      // TODO
    }

    /* Data for sounds */
    if(event->getEventType() != EventClassifier::NOEVENT &&
       list_sounds.size() > 0)
    {
      int index = -1;
      if(event->getSoundID() >= 0)
      {
        for(int i = 0; i < list_sounds.size(); i++)
        {
          QStringList str_list = list_sounds[i].split(':');
          if(str_list.size() >= 2)
            if(str_list.front().toInt() == event->getSoundID())
              index = i;
        }
      }

      if(index >= 0)
      {
        combo_sound->setCurrentIndex(index);
      }
      else
      {
        combo_sound->setCurrentIndex(0);
        if(list_sounds.size() > 0)
          event->setSoundID(-1);
      }
    }

    /* Enable the widget */
    combo_category->setEnabled(true);
  }
  else
  {
    combo_category->setCurrentIndex(0);
    combo_category->setDisabled(true);
  }
}

/*
 * Description: Updates the convo tree with the new conversation reference,
 *              a parent widget item, and if this convo should be an option.
 *
 * Inputs: Conversation* ref - the reference conversation to update
 *         QTreeWidgetItem* parent - the parent widget that the item is being
 *                                   added to.
 *         bool option - true if the convo is an option
 * Output: none
 */
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

/*
 * Description: Slot which triggers when the category drop down changes.
 *              Switches the view and changes the type of event.
 *
 * Inputs: int index - the index in the combo box
 * Output: none
 */
void EventView::categoryChanged(int index)
{
//  /* Message box - warning of change - tried and caused problems */
//  QMessageBox msg_box;
//  msg_box.setInformativeText("Are you sure you want to change event?");
//  msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
//  msg_box.setDefaultButton(QMessageBox::No);
//  int ret = msg_box.exec();

//  /* Parse the return value */
//  if(ret == QMessageBox::Yes)
//  {

  /* Update the stack */
  view_stack->setCurrentIndex(index);

  /* If anything except no classification, enables sound widget */
  combo_sound->setEnabled(index != (int)EventClassifier::NOEVENT);

  /* Update the event */
  if(event != NULL)
  {
    if((int)event->getEventType() != index)
    {
      /* Change the event */
      if(index == (int)EventClassifier::GIVEITEM)
        event->setEventGiveItem();
      else if(index == (int)EventClassifier::JUSTSOUND)
        event->setEventSound();
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
      else if(index == (int)EventClassifier::TAKEITEM)
        event->setEventTakeItem();
      else if(index == (int)EventClassifier::TELEPORTTHING)
        event->setEventTeleport();
      else if(index == (int)EventClassifier::UNLOCKIO)
        event->setEventUnlockIO();
      else if(index == (int)EventClassifier::UNLOCKTHING)
        event->setEventUnlockThing();
      else if(index == (int)EventClassifier::UNLOCKTILE)
        event->setEventUnlockTile();
    }

    /* Update the layout */
    setLayoutData();
    emit updated();
  }
//  }
//  else
//  {
//    combo_category->blockSignals(true);
//    combo_category->setCurrentIndex(view_stack->currentIndex());
//    combo_category->blockSignals(false);
//  }
}

/*
 * Description: Triggers when the change map index combo box is changed.
 *
 * Inputs: int index - the index in the change map combo box
 * Output: none
 */
void EventView::changeMapChanged(int index)
{
  if(index >= 0 && index < list_maps.size())
  {
    QStringList list = list_maps[index].split(":");
    if(list.size() == 2)
    {
      event->setEventStartMap(list.front().toInt(), event->getSoundID());
    }
  }
}

/*
 * Description: Triggers when the sound combo box is changed.
 *
 * Inputs: QString text - the new entry in the combo box
 * Output: none
 */
void EventView::changedSound(const QString & text)
{
  QStringList str_list = text.split(':');

  /* If the list is two long, it is proper format - 001: Sound Example */
  if(str_list.size() >= 2)
  {
    event->setSoundID(str_list.front().toInt());
  }
  /* Otherwise, unset the sound ID */
  else
  {
    event->setSoundID(-1);
  }
}

/*
 * Description: Slot which triggers when a conversation segment is double
 *              clicked. Initializes the edit of the segment (ConvoDialog).
 *
 * Inputs: QTreeWidgetItem* - not used
 *         int - not used
 * Output: none
 */
void EventView::convoDoubleClick(QTreeWidgetItem*, int)
{
  QTreeWidgetItem* selected = convo_tree->currentItem();
  if(selected != NULL)
  {
    rightclick_index = getConvoIndex(selected);
    rightClickEdit();
  }
}

/*
 * Description: Slot which triggers when a conversation segment is right
 *              clicked. Opens the manipulation menu.
 *
 * Inputs: QPoint point - where the right click was triggered
 * Output: none
 */
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

/*
 * Description: Slot which triggers when the give item count widget in the give
 *              item event is changed. Updates the event
 *
 * Inputs: int index - the count value
 * Output: none
 */
void EventView::giveCountChanged(int index)
{
  event->setEventGiveItem(event->getGiveItemID(), index, event->getSoundID());
}

/*
 * Description: Slot which triggers when the give item ID changes in the combo
 *              box. Updates the event
 *
 * Inputs: int index - the index in the combo box
 * Output: none
 */
void EventView::giveItemChanged(int index)
{
  if(index >= 0 && index < list_items.size())
  {
    QStringList list = list_items[index].split(":");
    if(list.size() == 2)
    {
      event->setEventGiveItem(list.front().toInt(), event->getGiveItemCount(),
                              event->getSoundID());
    }
  }
}

/*
 * Description: Slot which triggers when the notification text in the edit
 *              text box is manipulated. Updates the event.
 *
 * Inputs: none
 * Output: none
 */
void EventView::notificationTextChanged()
{
  event->setEventNotification(notification_edit->toPlainText(),
                              event->getSoundID());
}

/*
 * Description: Slot which triggers anytime the tree is expanded or collapsed.
 *              Forces an auto resize on the widget to make the conversation
 *              scrollable.
 *
 * Inputs: QTreeWidgetItem* - not used
 * Output: none
 */
void EventView::resizeTree(QTreeWidgetItem*)
{
  convo_tree->resizeColumnToContents(0);
}

/*
 * Description: Slot which triggers when the delete option is selected from the
 *              conversation right click menu. Removes the current conversation
 *              index and all children. Warns through pop-up prior to execution
 *
 * Inputs: none
 * Output: none
 */
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

/*
 * Description: Slot which triggers when the edit option is selected from the
 *              conversation right click menu. Takes the current conversation
 *              segment and triggers a conversation edit (handled by parent).
 *
 * Inputs: none
 * Output: none
 */
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

/*
 * Description: Slot which triggers an insert after when it's selected from the
 *              conversation right click menu. Creates a new segment and inserts
 *              it after the selected.
 *
 * Inputs: none
 * Output: none
 */
void EventView::rightClickInsertAfter()
{
  if(!rightclick_index.isEmpty())
  {
    Conversation after = EventSet::createBlankConversation();
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

/*
 * Description: Slot which triggers an insert before when it's selected from the
 *              conversation right click menu. Creates a new segment and inserts
 *              it before the selected.
 *
 * Inputs: none
 * Output: none
 */
void EventView::rightClickInsertBefore()
{
  if(!rightclick_index.isEmpty())
  {
    Conversation before = EventSet::createBlankConversation();
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

/*
 * Description: Slot which triggers an insert option when it's selected from the
 *              conversation right click menu. Creates a new segment and inserts
 *              it as an option on the current. If the current has children,
 *              the first child becomes an option and this becomes the second.
 *
 * Inputs: none
 * Output: none
 */
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
      Conversation option = EventSet::createBlankConversation();
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

/*
 * Description: Slot which triggers when the take item count widget in the take
 *              item event is changed. Updates the event
 *
 * Inputs: int index - the count value
 * Output: none
 */
void EventView::takeCountChanged(int index)
{
  event->setEventTakeItem(event->getTakeItemID(), index, event->getSoundID());
}

/*
 * Description: Slot which triggers when the take item ID changes in the combo
 *              box. Updates the event
 *
 * Inputs: int index - the index in the combo box
 * Output: none
 */
void EventView::takeItemChanged(int index)
{
  if(index >= 0 && index < list_items.size())
  {
    QStringList list = list_items[index].split(":");
    if(list.size() == 2)
      event->setEventTakeItem(list.front().toInt(), event->getTakeItemCount(),
                              event->getSoundID());
  }
}

/*
 * Description: Slot which triggers when the select tile on map is pressed.
 *              Triggers selectTile() which is picked up by the parent and
 *              triggers a hide and then select tile before re-showing.
 *
 * Inputs: none
 * Output: none
 */
void EventView::teleportMapPressed()
{
  emit selectTile();
}

/*
 * Description: Slot which triggers when the teleport thing combo box is
 *              changed. Updates the event with the new thing information.
 *
 * Inputs: int index - the index in the combo box of the element
 * Output: none
 */
void EventView::teleportThingChanged(int index)
{
  if(index >= 0 && index < list_things.size())
  {
    QStringList list = list_things[index].split(":");
    if(list.size() == 2)
      event->setEventTeleport(list.front().toInt(), event->getTeleportSection(),
                              event->getTeleportX(), event->getTeleportY(),
                              event->getSoundID());
  }
}

/* The unlock IO triggers */
// TODO: Comment
void EventView::unlockIOChanged(const QString & text)
{
  // TODO: REF
  //event->setEventUnlockIO(event->getUnlockIOID(), event->getUnlockIOMode(),
  //                        event->getUnlockIOState(),
  //                        event->getUnlockIOEventMode(),
  //                        event->getUnlockViewMode(),
  //                        event->getUnlockViewTime(), event->getSoundID());

  QStringList list = text.split(":");
  if(list.size() == 2)
  {
    event->setEventUnlockIO(list.front().toInt(), event->getUnlockIOMode(),
                            -1, event->getUnlockIOEventMode(),
                            event->getUnlockViewMode(),
                            event->getUnlockViewTime(), event->getSoundID());

    /* Update state to combo */
    int index = unio_name->currentIndex();
    unio_state->clear();
    if(index >= 0 && index < list_ios.size())
    {
      QStringList split_set = list_ios[index].second.split(',');
      unio_state->addItem("-1: All States");
      for(int i = 0; i < split_set.size(); i++)
        unio_state->addItem(split_set[i] + ": State");
    }
  }
}

/* The unlock IO triggers */
// TODO: Comment
void EventView::unlockIOModeLock(int state)
{
  // TODO
}

/* The unlock IO triggers */
// TODO: Comment
void EventView::unlockIOModeStates(int state)
{
  // TODO
}

/* The unlock IO triggers */
// TODO: Comment
void EventView::unlockIOStateChanged(int index)
{
  // TODO
}

/* The unlock IO triggers */
// TODO: Comment
void EventView::unlockIOStateEnter(int state)
{
  // TODO
}

/* The unlock IO triggers */
// TODO: Comment
void EventView::unlockIOStateExit(int state)
{
  // TODO
}

/* The unlock IO triggers */
// TODO: Comment
void EventView::unlockIOStateUse(int state)
{
  // TODO
}

/* The unlock IO triggers */
// TODO: Comment
void EventView::unlockIOStateWalk(int state)
{
  // TODO
}

/* The unlock IO triggers */
// TODO: Comment
void EventView::unlockIOView(int state)
{
  // TODO
}

/* The unlock IO triggers */
// TODO: Comment
void EventView::unlockIOViewScroll(int state)
{
  // TODO
}

/* The unlock IO triggers */
// TODO: Comment
void EventView::unlockIOViewTime(int time)
{
  // TODO
}

/* The unlock thing triggers */
// TODO: Comment
void EventView::unlockThingChanged(const QString & text)
{
  QStringList list = text.split(":");
  if(list.size() == 2)
    event->setEventUnlockThing(list.front().toInt(), event->getUnlockViewMode(),
                               event->getUnlockViewTime(), event->getSoundID());
}

/* The unlock thing triggers */
// TODO: Comment
void EventView::unlockThingView(int state)
{
  /* Save the data to the event */
  bool scroll, view;
  UnlockView view_mode = event->getUnlockViewMode();
  EventSet::dataEnumView(view_mode, view, scroll);
  view = (state == Qt::Checked);
  view_mode = EventSet::createEnumView(view, scroll);
  event->setEventUnlockThing(event->getUnlockThingID(), view_mode,
                             event->getUnlockViewTime(), event->getSoundID());

  /* Proceed to enable/disable widget */
  unth_view_scroll->setEnabled(view);
  unth_view_time->setEnabled(view);
}

/* The unlock thing triggers */
// TODO: Comment
void EventView::unlockThingViewScroll(int state)
{
  bool scroll, view;
  UnlockView view_mode = event->getUnlockViewMode();
  EventSet::dataEnumView(view_mode, view, scroll);
  scroll = (state == Qt::Checked);
  view_mode = EventSet::createEnumView(view, scroll);
  event->setEventUnlockThing(event->getUnlockThingID(), view_mode,
                             event->getUnlockViewTime(), event->getSoundID());
}

/* The unlock thing triggers */
// TODO: Comment
void EventView::unlockThingViewTime(int time)
{
  if(time >= 0)
    event->setEventUnlockThing(event->getUnlockThingID(),
                               event->getUnlockViewMode(),
                               time, event->getSoundID());
}

/* The unlock tile triggers */
// TODO: Comment
void EventView::unlockTileEnter(int state)
{
  bool enter, exit;
  UnlockTileMode mode = event->getUnlockTileMode();
  EventSet::dataEnumTileEvent(mode, enter, exit);
  enter = (state == Qt::Checked);
  mode = EventSet::createEnumTileEvent(enter, exit);
  event->setEventUnlockTile(event->getUnlockTileSection(),
                            event->getUnlockTileX(), event->getUnlockTileY(),
                            mode, event->getUnlockViewMode(),
                            event->getUnlockViewTime(), event->getSoundID());
}

/* The unlock tile triggers */
// TODO: Comment
void EventView::unlockTileExit(int state)
{
  bool enter, exit;
  UnlockTileMode mode = event->getUnlockTileMode();
  EventSet::dataEnumTileEvent(mode, enter, exit);
  exit = (state == Qt::Checked);
  mode = EventSet::createEnumTileEvent(enter, exit);
  event->setEventUnlockTile(event->getUnlockTileSection(),
                            event->getUnlockTileX(), event->getUnlockTileY(),
                            mode, event->getUnlockViewMode(),
                            event->getUnlockViewTime(), event->getSoundID());
}

/* The unlock tile triggers */
// TODO: Comment
void EventView::unlockTilePressed()
{
  emit selectTile();
}

/* The unlock tile triggers */
// TODO: Comment
void EventView::unlockTileView(int state)
{
  /* Save the data to the event */
  bool scroll, view;
  UnlockView view_mode = event->getUnlockViewMode();
  EventSet::dataEnumView(view_mode, view, scroll);
  view = (state == Qt::Checked);
  view_mode = EventSet::createEnumView(view, scroll);
  event->setEventUnlockTile(event->getUnlockTileSection(),
                            event->getUnlockTileX(), event->getUnlockTileY(),
                            event->getUnlockTileMode(), view_mode,
                            event->getUnlockViewTime(), event->getSoundID());

  /* Proceed to enable/disable widget */
  unti_view_scroll->setEnabled(view);
  unti_view_time->setEnabled(view);
}

/* The unlock tile triggers */
// TODO: Comment
void EventView::unlockTileViewScroll(int state)
{
  bool scroll, view;
  UnlockView view_mode = event->getUnlockViewMode();
  EventSet::dataEnumView(view_mode, view, scroll);
  scroll = (state == Qt::Checked);
  view_mode = EventSet::createEnumView(view, scroll);
  event->setEventUnlockTile(event->getUnlockTileSection(),
                            event->getUnlockTileX(), event->getUnlockTileY(),
                            event->getUnlockTileMode(), view_mode,
                            event->getUnlockViewTime(), event->getSoundID());
}

/* The unlock tile triggers */
// TODO: Comment
void EventView::unlockTileViewTime(int time)
{
  if(time >= 0)
    event->setEventUnlockTile(event->getUnlockTileSection(),
                              event->getUnlockTileX(), event->getUnlockTileY(),
                              event->getUnlockTileMode(),
                              event->getUnlockViewMode(),
                              time, event->getSoundID());
}

/*
 * Description: Updates the conversation after an edit. Attempts to maintain
 *              the same index after execution.
 *
 * Inputs: none
 * Output: none
 */
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

/*
 * Description: Returns the editor event which is being manipulated by the
 *              widget.
 *
 * Inputs: none
 * Output: EditorEvent* - pointer to the event being edited
 */
EditorEvent* EventView::getEvent()
{
  return event;
}

/* Returns the list of objects, used for event creation */
// TODO: Comment
QVector<QPair<QString,QString>> EventView::getListIOs()
{
  return list_ios;
}

/*
 * Description: Returns the list of items, used for event creation.
 *
 * Inputs: none
 * Output: QVector<QString> - list of all items (for give item event)
 */
QVector<QString> EventView::getListItems()
{
  return list_items;
}

/*
 * Description: Returns the list of maps, used for event creation.
 *
 * Inputs: none
 * Output: QVector<QString> - list of all maps (for change map event)
 */
QVector<QString> EventView::getListMaps()
{
  return list_maps;
}

/*
 * Description: Returns the list of sounds, used for event creation.
 *
 * Inputs: none
 * Output: QList<QString> - list of all sounds (for all valid events)
 */
QList<QString> EventView::getListSounds()
{
  return list_sounds;
}

/*
 * Description: Returns the list of sub-maps, used for event creation.
 *
 * Inputs: none
 * Output: QVector<QString> - list of all sub-maps (for teleport event)
 */
QVector<QString> EventView::getListSubmaps()
{
  return list_submaps;
}

/*
 * Description: Returns the list of things, used for event creation.
 *
 * Inputs: none
 * Output: QVector<QString> - list of all things (for teleport event)
 */
QVector<QString> EventView::getListThings()
{
  return list_things;
}

/*
 * Description: Sets the event being edited by the event view.
 *
 * Inputs: EditorEvent* event - the new reference event to edit
 * Output: none
 */
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
// TODO: Comment
void EventView::setListIOs(QVector<QPair<QString,QString>> ios)
{
  /* Data parsing - main IO list */
  list_ios = ios;

  /* Block singals */
  unio_name->blockSignals(true);

  /* Load in to unlock IO combo box */
  unio_name->clear();
  for(int i = 0; i < list_ios.size(); i++)
    unio_name->addItem(list_ios[i].first);

  /* Update data and unblock signals */
  setLayoutData();
  unio_name->blockSignals(false);
}

/*
 * Description: Sets the list of items, used for event creation
 *
 * Inputs: QVector<QString> - list of all items (for give item event)
 * Output: none
 */
void EventView::setListItems(QVector<QString> items)
{
  /* Set new data */
  list_items = items;

  /* Block signals */
  item_name->blockSignals(true);
  take_name->blockSignals(true);

  /* Give Items */
  item_name->clear();
  for(int i = 0; i < list_items.size(); i++)
    item_name->addItem(list_items[i]);

  /* Take Items */
  take_name->clear();
  for(int i = 0; i < list_items.size(); i++)
    take_name->addItem(list_items[i]);

  /* Update and unblock */
  setLayoutData();
  item_name->blockSignals(false);
  take_name->blockSignals(false);
}

/*
 * Description: Sets the list of maps, used for event creation
 *
 * Inputs: QVector<QString> - list of all maps (for change map event)
 * Output: none
 */
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

/*
 * Description: Sets the list of sounds, used for event creation
 *
 * Inputs: QList<QString> - list of all sounds (for all valid events)
 * Output: none
 */
void EventView::setListSounds(QList<QString> sounds)
{
  list_sounds = sounds;
  combo_sound->blockSignals(true);
  combo_sound->clear();
  for(int i = 0; i < list_sounds.size(); i++)
    combo_sound->addItem(list_sounds[i]);
  setLayoutData();
  combo_sound->blockSignals(false);
}

/*
 * Description: Sets the list of sub-maps, used for event creation.
 *
 * Inputs: QVector<QString> - list of all sub-maps (for teleport event)
 * Output: none
 */
void EventView::setListSubmaps(QVector<QString> sub_maps)
{
  list_submaps = sub_maps;
  setLayoutData();
}

/*
 * Description: Sets the list of things, used for event creation.
 *
 * Inputs: QVector<QString> - list of all things (for teleport event)
 * Output: none
 */
void EventView::setListThings(QVector<QString> things)
{
  /* Data parsing - main thing list */
  list_things = things;

  /* Data parsing - thing with no IO/IO split */
  //list_ios.clear();
  list_things_no_io.clear();
  for(int i = 0; i < things.size(); i++)
  {
    QStringList set = things[i].split(":");

    //if(set.size() == 2 && set.front().toInt() >= EditorEnumDb::kBASE_ID_IOS)
    //  list_ios.push_back(things[i]);
    //else
    if(set.size() != 2 || set.front().toInt() < EditorEnumDb::kBASE_ID_IOS)
      list_things_no_io.push_back(things[i]);
  }

  /* Block singals */
  tele_thing->blockSignals(true);
  unth_name->blockSignals(true);
  //unio_name->blockSignals(true);

  /* Load in to teleport thing combo box */
  tele_thing->clear();
  for(int i = 0; i < list_things.size(); i++)
    tele_thing->addItem(list_things[i]);

  /* Load in to unlock thing combo box */
  unth_name->clear();
  for(int i = 0; i < list_things_no_io.size(); i++)
    unth_name->addItem(list_things_no_io[i]);

  /* Load in to unlock IO combo box */
  //unio_name->clear();
  //for(int i = 0; i < list_ios.size(); i++)
  //  unio_name->addItem(list_ios[i]);

  /* Update data and unblock signals */
  setLayoutData();
  tele_thing->blockSignals(false);
  unth_name->blockSignals(false);
  //unio_name->blockSignals(false);
}

/*
 * Description: Updates the editor event. Triggered by an external class when
 *              it modifies the reference event.
 *
 * Inputs: none
 * Output: none
 */
void EventView::updateEvent()
{
  setLayoutData();
}

/*
 * Description: Updates the dialog with the tile which was selected on the
 *              sub-map. This shows the pop-up and updates the event with the
 *              new location.
 *
 * Inputs: int id - the ID of the sub-map
 *         int x - the x tile location in the sub-map
 *         int y - the y tile location in the sub-map
 * Output: none
 */
void EventView::updateSelectedTile(int id, int x, int y)
{
  if(event != nullptr)
  {
    if(event->getEventType() == EventClassifier::TELEPORTTHING)
      event->setEventTeleport(event->getTeleportThingID(), id, x, y,
                              event->getSoundID());
    else if(event->getEventType() == EventClassifier::UNLOCKTILE)
      event->setEventUnlockTile(id, x, y, event->getUnlockTileMode(),
                                event->getUnlockViewMode(),
                                event->getUnlockViewTime(), event->getSoundID());
    setLayoutData();
  }
}
