/*******************************************************************************
 * Class Name: EventView
 * Date Created: February 10, 2015
 * Inheritance: QFrame
 * Description: The class is for managing the display and edit of the
 *              EditorEvent class.
 ******************************************************************************/
#include "View/EventView.h"
#include "Dialog/ConvoDialog.h"
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
 *         EventClassifier limiter - which events to limit and not allow
 *         bool view_only - true if the event is only for viewing and no edit
 */
EventView::EventView(EditorEvent* event, QWidget* parent,
                     EventClassifier limiter, bool view_only)
         : QFrame(parent)
{
  /* Initialize variables */
  this->event = nullptr;
  this->limiter = limiter;
  pop_convo = nullptr;
  rightclick_index = "";
  this->view_only = view_only;
  waiting_tile_convo = false;
  waiting_tile_event = false;
  waiting_tile = false;

  /* Create list of available events */
  int index = 0;
  while(index <= (int)EventClassifier::UNLOCKTILE)
  {
    /* Get classification */
    EventClassifier curr_opt = static_cast<EventClassifier>(index);

    /* Push back if applicable */
    if((((int)limiter) & ((int)curr_opt)) == 0)
      available_events.push_back(curr_opt);

    /* Increment index */
    if(index < 1)
      index = 1;
    else
      index = (index << 1);
  }

  /* Create the layout */
  createLayout();//conversation_enabled);

  /* Set the event */
  setEvent(event);
}

/*
 * Description: Destructor function
 */
EventView::~EventView()
{
  closeAllPopups();

  disconnect(combo_category, SIGNAL(currentIndexChanged(int)),
             this, SLOT(categoryChanged(int)));
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
void EventView::createLayout()
{
  /* Layout */
  QVBoxLayout* layout = new QVBoxLayout(this);

  /* Label title for widget */
  QLabel* lbl_title = new QLabel("<b>Event</b>", this);
  layout->addWidget(lbl_title, 0, Qt::AlignHCenter);

  /* Combo box for event category */
  combo_category = new QComboBox(this);
  for(int i = 0; i < available_events.size(); i++)
    combo_category->addItem(QString::fromStdString(
                              EventSet::classifierToStr(available_events[i])));
  connect(combo_category, SIGNAL(currentIndexChanged(int)),
          this, SLOT(categoryChanged(int)));
  layout->addWidget(combo_category, 0, Qt::AlignHCenter);

  /* Stacked widget for housing all the different views for categories */
  view_stack = new QStackedWidget(this);
  for(int i = 0; i < available_events.size(); i++)
  {
    /* ---- BATTLE START ---- */
    if(available_events[i] == EventClassifier::BATTLESTART)
    {
      QWidget* widget_battle = new QWidget(this);
      battle_windisappear = new QCheckBox(
                                "On a win, the source thing disappears", this);
      connect(battle_windisappear, SIGNAL(stateChanged(int)),
              this, SLOT(battleWinFlagChange(int)));
      battle_losegg = new QCheckBox("On a loss, game over", this);
      connect(battle_losegg, SIGNAL(stateChanged(int)),
              this, SLOT(battleLoseFlagChange(int)));
      battle_restorehealth = new QCheckBox("Restore health on battle end",
                                                     this);
      connect(battle_restorehealth, SIGNAL(stateChanged(int)),
              this, SLOT(battleHealthFlagChange(int)));
      battle_restoreqd = new QCheckBox("Restore QD on battle end", this);
      connect(battle_restoreqd, SIGNAL(stateChanged(int)),
              this, SLOT(battleQDFlagChange(int)));
      battle_eventwin = new QPushButton("Win Event", this);
      connect(battle_eventwin, SIGNAL(clicked()),
              this, SLOT(battleEventWinEdit()));
      battle_eventlose = new QPushButton("Lose Event", this);
      connect(battle_eventlose, SIGNAL(clicked()),
              this, SLOT(battleEventLoseEdit()));
      QVBoxLayout* layout_battle = new QVBoxLayout(widget_battle);
      layout_battle->addStretch(1);
      layout_battle->addWidget(battle_windisappear, 0, Qt::AlignCenter);
      layout_battle->addWidget(battle_losegg, 0, Qt::AlignCenter);
      layout_battle->addSpacing(12);
      QHBoxLayout* layout_battle_h = new QHBoxLayout();
      layout_battle_h->addStretch(1);
      layout_battle_h->addWidget(battle_eventwin);
      layout_battle_h->addWidget(battle_eventlose);
      layout_battle_h->addStretch(1);
      layout_battle->addLayout(layout_battle_h);
      layout_battle->addSpacing(12);
      layout_battle->addWidget(battle_restorehealth, 0, Qt::AlignCenter);
      layout_battle->addWidget(battle_restoreqd, 0, Qt::AlignCenter);
      layout_battle->addStretch(1);
      view_stack->addWidget(widget_battle);
    }
    /* ---- CONVERSATION ---- */
    else if(available_events[i] == EventClassifier::CONVERSATION)
    {
      QWidget* widget_convo = new QWidget(this);
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
      view_stack->addWidget(widget_convo);
    }
    /* ---- ITEM: GIVE ---- */
    else if(available_events[i] == EventClassifier::ITEMGIVE)
    {
      QWidget* widget_give = new QWidget(this);
      QLabel* lbl_give_item = new QLabel("Item:", this);
      QLabel* lbl_give_count = new QLabel("Count:", this);
      QLabel* lbl_give_chance = new QLabel("Chance:", this);
      item_name = new QComboBox(this);
      EditorHelpers::comboBoxOptimize(item_name);
      item_name->setMinimumWidth(200);
      connect(item_name, SIGNAL(currentIndexChanged(int)),
              this, SLOT(giveItemChanged(int)));
      item_count = new QSpinBox(this);
      item_count->setMinimum(1);
      item_count->setMaximum(100000);
      connect(item_count, SIGNAL(valueChanged(int)),
              this, SLOT(giveCountChanged(int)));
      item_drop = new QCheckBox("Drop Automatically", this);
      item_drop->setToolTip(QString("Checked: drops the item. ") +
                            QString("Unchecked: Insert into inventory and ") +
                            QString("drop items that do not fit"));
      connect(item_drop, SIGNAL(stateChanged(int)),
              this, SLOT(giveAutoDropChanged(int)));
      item_chance = new QSpinBox(this);
      item_chance->setMinimum(1);
      item_chance->setMaximum(EventSet::kGIVE_DEF_CHANCE);
      item_chance->setSuffix(" %");
      connect(item_chance, SIGNAL(valueChanged(int)),
              this, SLOT(giveChanceChanged(int)));
      QGridLayout* layout_give = new QGridLayout(widget_give);
      layout_give->setRowStretch(0, 1);
      layout_give->addWidget(lbl_give_item, 1, 0);
      layout_give->addWidget(item_name, 1, 1, 1, 2);
      layout_give->addWidget(lbl_give_count, 2, 0);
      layout_give->addWidget(item_count, 2, 1);
      layout_give->addWidget(item_drop, 3, 1);
      layout_give->addWidget(lbl_give_chance, 4, 0);
      layout_give->addWidget(item_chance, 4, 1);
      layout_give->setColumnStretch(2, 1);
      layout_give->setRowStretch(5, 1);
      view_stack->addWidget(widget_give);
    }
    /* ---- ITEM: TAKE ---- */
    else if(available_events[i] == EventClassifier::ITEMTAKE)
    {
      QWidget* widget_take = new QWidget(this);
      QLabel* lbl_take_item = new QLabel("Item:", this);
      QLabel* lbl_take_count = new QLabel("Count:", this);
      take_name = new QComboBox(this);
      EditorHelpers::comboBoxOptimize(take_name);
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
      view_stack->addWidget(widget_take);
    }
    /* ---- MAP SWITCH ---- */
    else if(available_events[i] == EventClassifier::MAPSWITCH)
    {
      QWidget* widget_map = new QWidget(this);
      QLabel* lbl_map_name = new QLabel("Map:");
      map_name = new QComboBox(this);
      EditorHelpers::comboBoxOptimize(map_name);
      map_name->setMinimumWidth(200);
      connect(map_name, SIGNAL(currentIndexChanged(int)),
              this, SLOT(changeMapChanged(int)));
      QGridLayout* layout_map = new QGridLayout(widget_map);
      layout_map->addWidget(lbl_map_name, 0, 0);
      layout_map->addWidget(map_name, 0, 1);
      layout_map->setColumnStretch(1, 1);
      view_stack->addWidget(widget_map);
    }
    /* ---- MULTIPLE TRIGGER ---- */
    else if(available_events[i] == EventClassifier::MULTIPLE)
    {
      QWidget* widget_multiple = new QWidget(this);
      mult_list = new QListWidget(this);
      connect(mult_list, SIGNAL(currentRowChanged(int)),
              this, SLOT(multListChange(int)));
      connect(mult_list, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
              this, SLOT(multListDouble(QListWidgetItem*)));
      QPushButton* mult_btn_add = new QPushButton("Add", this);
      connect(mult_btn_add, SIGNAL(clicked()), this, SLOT(multBtnAdd()));
      mult_btn_edit = new QPushButton("Edit", this);
      mult_btn_edit->setDisabled(true);
      connect(mult_btn_edit, SIGNAL(clicked()), this, SLOT(multBtnEdit()));
      mult_btn_rem = new QPushButton("Remove", this);
      mult_btn_rem->setDisabled(true);
      connect(mult_btn_rem, SIGNAL(clicked()), this, SLOT(multBtnRem()));
      mult_btn_up = new QPushButton("Up", this);
      mult_btn_up->setDisabled(true);
      connect(mult_btn_up, SIGNAL(clicked()), this, SLOT(multBtnUp()));
      mult_btn_down = new QPushButton("Down", this);
      mult_btn_down->setDisabled(true);
      connect(mult_btn_down, SIGNAL(clicked()), this, SLOT(multBtnDown()));
      QVBoxLayout* layout_multiple = new QVBoxLayout(widget_multiple);
      layout_multiple->addWidget(mult_list);
      QHBoxLayout* layout_multiple_btns = new QHBoxLayout();
      layout_multiple_btns->addWidget(mult_btn_add);
      layout_multiple_btns->addWidget(mult_btn_edit);
      layout_multiple_btns->addWidget(mult_btn_rem);
      layout_multiple_btns->addWidget(mult_btn_up);
      layout_multiple_btns->addWidget(mult_btn_down);
      layout_multiple->addLayout(layout_multiple_btns);
      view_stack->addWidget(widget_multiple);
    }
    /* ---- NO EVENT ---- */
    else if(available_events[i] == EventClassifier::NOEVENT)
    {
      QWidget* widget_unset = new QWidget(this);
      QLabel* lbl_not_used = new QLabel("NOT USED", this);
      QVBoxLayout* layout_unset = new QVBoxLayout(widget_unset);
      layout_unset->addWidget(lbl_not_used, 0, Qt::AlignCenter);
      view_stack->addWidget(widget_unset);
    }
    /* ---- NOTIFICATION ---- */
    else if(available_events[i] == EventClassifier::NOTIFICATION)
    {
      QWidget* widget_notification = new QWidget(this);
      notification_edit = new QTextEdit(this);
      //notification_edit->setMinimumWidth(320);
      connect(notification_edit, SIGNAL(copyAvailable(bool)),
              this, SLOT(notificationSelected(bool)));
      connect(notification_edit, SIGNAL(textChanged()),
              this, SLOT(notificationTextChanged()));
      notify_btn_brush = new QPushButton(this);
      notify_btn_brush->setIcon(QIcon(":/images/icons/32_brush.png"));
      notify_btn_brush->setIconSize(QSize(24,24));
      notify_btn_brush->setMaximumSize(30, 30);
      connect(notify_btn_brush, SIGNAL(clicked()),
              this, SLOT(notificationBtnBrush()));
      notify_btn_bold = new QPushButton(this);
      notify_btn_bold->setIcon(QIcon(":/images/icons/32_font_bold.png"));
      notify_btn_bold->setIconSize(QSize(24,24));
      notify_btn_bold->setMaximumSize(30, 30);
      connect(notify_btn_bold, SIGNAL(clicked()),
              this, SLOT(notificationBtnBold()));
      notify_btn_italic = new QPushButton(this);
      notify_btn_italic->setIcon(QIcon(":/images/icons/32_font_italic.png"));
      notify_btn_italic->setIconSize(QSize(24,24));
      notify_btn_italic->setMaximumSize(30, 30);
      connect(notify_btn_italic, SIGNAL(clicked()),
              this, SLOT(notificationBtnItalic()));
      notify_btn_underline = new QPushButton(this);
      notify_btn_underline->setIcon(
                               QIcon(":/images/icons/32_font_underline.png"));
      notify_btn_underline->setIconSize(QSize(24,24));
      notify_btn_underline->setMaximumSize(30, 30);
      connect(notify_btn_underline, SIGNAL(clicked()),
              this, SLOT(notificationBtnUnderline()));
      QPushButton* notify_btn_thing = new QPushButton(this);
      notify_btn_thing->setIcon(QIcon(":/images/icons/32_user.png"));
      notify_btn_thing->setIconSize(QSize(24,24));
      notify_btn_thing->setMaximumSize(30, 30);
      connect(notify_btn_thing, SIGNAL(clicked()),
              this, SLOT(notificationBtnThing()));
      QPushButton* notify_btn_item = new QPushButton(this);
      notify_btn_item->setIcon(QIcon(":/images/icons/32_item.png"));
      notify_btn_item->setIconSize(QSize(24,24));
      notify_btn_item->setMaximumSize(30, 30);
      connect(notify_btn_item, SIGNAL(clicked()),
              this, SLOT(notificationBtnItem()));
      QPushButton* notify_btn_preview = new QPushButton(this);
      notify_btn_preview->setIcon(QIcon(":/images/icons/32_export.png"));
      notify_btn_preview->setIconSize(QSize(24,24));
      notify_btn_preview->setMaximumSize(30, 30);
      connect(notify_btn_preview, SIGNAL(clicked()),
              this, SLOT(notificationBtnPreview()));
      notificationSelected(false);
      QVBoxLayout* layout_notification = new QVBoxLayout(widget_notification);
      QHBoxLayout* layout_notify_btns = new QHBoxLayout();
      layout_notify_btns->addWidget(notify_btn_brush);
      layout_notify_btns->addWidget(notify_btn_bold);
      layout_notify_btns->addWidget(notify_btn_italic);
      layout_notify_btns->addWidget(notify_btn_underline);
      layout_notify_btns->addWidget(notify_btn_thing);
      layout_notify_btns->addWidget(notify_btn_item);
      layout_notify_btns->addWidget(notify_btn_preview);
      layout_notification->addLayout(layout_notify_btns);
      layout_notification->addWidget(notification_edit, 1);
      view_stack->addWidget(widget_notification);
    }
    /* ---- PROPERTY MODIFIER ---- */
    else if(available_events[i] == EventClassifier::PROPERTY)
    {
      QWidget* widget_property = new QWidget(this);
      QLabel* lbl_type = new QLabel("Type", this);
      prop_type = new QComboBox(this);
      for(int i = 1; i <= static_cast<int>(ThingBase::INTERACTIVE); i++)
        prop_type->addItem(QString::fromStdString(
                               Helpers::typeToStr(static_cast<ThingBase>(i))));
      connect(prop_type, SIGNAL(currentTextChanged(QString)),
              this, SLOT(propertyTypeChange(QString)));
      QLabel* lbl_id = new QLabel("ID", this);
      prop_id = new QComboBox(this);
      EditorHelpers::comboBoxOptimize(prop_id);
      connect(prop_id, SIGNAL(currentIndexChanged(QString)),
              this, SLOT(propertyIDChange(QString)));
      QLabel* lbl_head_prop = new QLabel("Property", this);
      QLabel* lbl_head_mod = new QLabel("Modify", this);
      QLabel* lbl_head_new = new QLabel("New Value", this);
      QFont font = lbl_head_prop->font();
      font.setUnderline(true);
      lbl_head_prop->setFont(font);
      lbl_head_mod->setFont(font);
      lbl_head_new->setFont(font);
      /* -- thing -- */
      QLabel* lbl_active = new QLabel("Active", this);
      prop_active_mod = new QCheckBox(this);
      connect(prop_active_mod, SIGNAL(stateChanged(int)),
              this, SLOT(propertyActiveMod(int)));
      prop_active_val = new QCheckBox("Is Spawned", this);
      connect(prop_active_val, SIGNAL(stateChanged(int)),
              this, SLOT(propertyActiveVal(int)));
      /* -- */
      QLabel* lbl_respawn = new QLabel("Respawn", this);
      prop_respawn_mod = new QCheckBox(this);
      connect(prop_respawn_mod, SIGNAL(stateChanged(int)),
              this, SLOT(propertyRespawnMod(int)));
      prop_respawn_val = new QSpinBox(this);
      prop_respawn_val->setSuffix(" ms");
      prop_respawn_val->setMinimum(1);
      prop_respawn_val->setMaximum(1800000);
      connect(prop_respawn_val, SIGNAL(valueChanged(int)),
              this, SLOT(propertyRespawnVal(int)));
      prop_respawn_dis = new QCheckBox("Disable", this);
      connect(prop_respawn_dis, SIGNAL(stateChanged(int)),
              this, SLOT(propertyRespawnDisable(int)));
      /* -- */
      QLabel* lbl_visible = new QLabel("Visible", this);
      prop_visible_mod = new QCheckBox(this);
      connect(prop_visible_mod, SIGNAL(stateChanged(int)),
              this, SLOT(propertyVisibleMod(int)));
      prop_visible_val = new QCheckBox("Is Visible", this);
      connect(prop_visible_val, SIGNAL(stateChanged(int)),
              this, SLOT(propertyVisibleVal(int)));
      /* -- person -- */
      QLabel* lbl_freeze = new QLabel("Freeze", this);
      prop_freeze_mod = new QCheckBox(this);
      connect(prop_freeze_mod, SIGNAL(stateChanged(int)),
              this, SLOT(propertyFreezeMod(int)));
      prop_freeze_val = new QCheckBox("Freeze Movement", this);
      connect(prop_freeze_val, SIGNAL(stateChanged(int)),
              this, SLOT(propertyFreezeVal(int)));
      /* -- */
      QLabel* lbl_reset = new QLabel("Reset", this);
      prop_reset_mod = new QCheckBox(this);
      connect(prop_reset_mod, SIGNAL(stateChanged(int)),
              this, SLOT(propertyResetMod(int)));
      prop_reset_val = new QCheckBox("Reset to Start", this);
      connect(prop_reset_val, SIGNAL(stateChanged(int)),
              this, SLOT(propertyResetVal(int)));
      /* -- */
      QLabel* lbl_speed = new QLabel("Speed", this);
      prop_speed_mod = new QCheckBox(this);
      connect(prop_speed_mod, SIGNAL(stateChanged(int)),
              this, SLOT(propertySpeedMod(int)));
      prop_speed_val = new QSpinBox(this);
      prop_speed_val->setMinimum(0);
      prop_speed_val->setMaximum(256);
      connect(prop_speed_val, SIGNAL(valueChanged(int)),
              this, SLOT(propertySpeedVal(int)));
      prop_speed_desc = new QLabel("X ms/tile", this);
      /* -- npc -- */
      QLabel* lbl_forced = new QLabel("Forced", this);
      prop_forced_mod = new QCheckBox(this);
      connect(prop_forced_mod, SIGNAL(stateChanged(int)),
              this, SLOT(propertyForcedMod(int)));
      prop_forced_val = new QCheckBox("Force Interaction", this);
      connect(prop_forced_val, SIGNAL(stateChanged(int)),
              this, SLOT(propertyForcedVal(int)));
      /* -- */
      QLabel* lbl_track = new QLabel("Track", this);
      prop_track_mod = new QCheckBox(this);
      connect(prop_track_mod, SIGNAL(stateChanged(int)),
              this, SLOT(propertyTrackMod(int)));
      prop_track_opt = new QComboBox(this);
      for(int i = 0; i <= static_cast<int>(TrackingState::TOPLAYER); i++)
        prop_track_opt->addItem(QString::fromStdString(
                       Helpers::trackingToStr(static_cast<TrackingState>(i))));
      connect(prop_track_opt, SIGNAL(currentTextChanged(QString)),
              this, SLOT(propertyTrackChange(QString)));
      /* -- io -- */
      QLabel* lbl_inactive = new QLabel("Inactive", this);
      prop_inactive_mod = new QCheckBox(this);
      connect(prop_inactive_mod, SIGNAL(stateChanged(int)),
              this, SLOT(propertyInactiveMod(int)));
      prop_inactive_val = new QSpinBox(this);
      prop_inactive_val->setMaximum(99999999);
      prop_inactive_val->setSuffix(" ms");
      connect(prop_inactive_val, SIGNAL(valueChanged(int)),
              this, SLOT(propertyInactiveVal(int)));
      prop_inactive_dis = new QCheckBox("Disable", this);
      connect(prop_inactive_dis, SIGNAL(stateChanged(int)),
              this, SLOT(propertyInactiveDisable(int)));
      /* -- LAYOUT -- */
      QGridLayout* layout_prop = new QGridLayout(widget_property);
      layout_prop->addWidget(lbl_type, 0, 0);
      layout_prop->addWidget(prop_type, 0, 1, 1, 3);
      layout_prop->addWidget(lbl_id, 1, 0);
      layout_prop->addWidget(prop_id, 1, 1, 1, 3);
      layout_prop->addWidget(lbl_head_prop, 2, 0);
      layout_prop->addWidget(lbl_head_mod, 2, 1, Qt::AlignHCenter);
      layout_prop->addWidget(lbl_head_new, 2, 2, 1, 2, Qt::AlignHCenter);
      /* -- thing -- */
      layout_prop->addWidget(lbl_active, 3, 0);
      layout_prop->addWidget(prop_active_mod, 3, 1, Qt::AlignHCenter);
      layout_prop->addWidget(prop_active_val, 3, 2, 1, 2, Qt::AlignHCenter);
      layout_prop->addWidget(lbl_respawn, 4, 0);
      layout_prop->addWidget(prop_respawn_mod, 4, 1, Qt::AlignHCenter);
      layout_prop->addWidget(prop_respawn_val, 4, 2, Qt::AlignRight);
      layout_prop->addWidget(prop_respawn_dis, 4, 3, Qt::AlignLeft);
      layout_prop->addWidget(lbl_visible, 5, 0);
      layout_prop->addWidget(prop_visible_mod, 5, 1, Qt::AlignHCenter);
      layout_prop->addWidget(prop_visible_val, 5, 2, 1, 2, Qt::AlignHCenter);
      /* -- person -- */
      layout_prop->addWidget(lbl_freeze, 6, 0);
      layout_prop->addWidget(prop_freeze_mod, 6, 1, Qt::AlignHCenter);
      layout_prop->addWidget(prop_freeze_val, 6, 2, 1, 2, Qt::AlignHCenter);
      layout_prop->addWidget(lbl_reset, 7, 0);
      layout_prop->addWidget(prop_reset_mod, 7, 1, Qt::AlignHCenter);
      layout_prop->addWidget(prop_reset_val, 7, 2, 1, 2, Qt::AlignHCenter);
      layout_prop->addWidget(lbl_speed, 8, 0);
      layout_prop->addWidget(prop_speed_mod, 8, 1, Qt::AlignHCenter);
      layout_prop->addWidget(prop_speed_val, 8, 2, Qt::AlignRight);
      layout_prop->addWidget(prop_speed_desc, 8, 3, Qt::AlignLeft);
      /* -- npc -- */
      layout_prop->addWidget(lbl_forced, 9, 0);
      layout_prop->addWidget(prop_forced_mod, 9, 1, Qt::AlignHCenter);
      layout_prop->addWidget(prop_forced_val, 9, 2, 1, 2, Qt::AlignHCenter);
      layout_prop->addWidget(lbl_track, 10, 0);
      layout_prop->addWidget(prop_track_mod, 10, 1, Qt::AlignHCenter);
      layout_prop->addWidget(prop_track_opt, 10, 2, 1, 2);
      /* -- io -- */
      layout_prop->addWidget(lbl_inactive, 11, 0);
      layout_prop->addWidget(prop_inactive_mod, 11, 1, Qt::AlignHCenter);
      layout_prop->addWidget(prop_inactive_val, 11, 2, Qt::AlignRight);
      layout_prop->addWidget(prop_inactive_dis, 11, 3, Qt::AlignLeft);
      view_stack->addWidget(widget_property);
    }
    /* ---- SOUND ONLY ---- */
    else if(available_events[i] == EventClassifier::SOUNDONLY)
    {
      QWidget* widget_sound = new QWidget(this);
      QLabel* lbl_sound = new QLabel("NO SETTINGS", this);
      QVBoxLayout* layout_sound = new QVBoxLayout(widget_sound);
      layout_sound->addWidget(lbl_sound, 0, Qt::AlignCenter);
      view_stack->addWidget(widget_sound);
    }
    /* ---- TELEPORT THING ---- */
    else if(available_events[i] == EventClassifier::TELEPORTTHING)
    {
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
      EditorHelpers::comboBoxOptimize(tele_thing);
      tele_thing->setMinimumWidth(200);
      connect(tele_thing, SIGNAL(currentIndexChanged(int)),
              this, SLOT(teleportThingChanged(int)));
      QPushButton* btn_tele_map = new QPushButton(this);
      btn_tele_map->setIcon(QIcon(":/images/icons/32_settings.png"));
      btn_tele_map->setIconSize(QSize(24,24));
      btn_tele_map->setMaximumSize(30, 30);
      connect(btn_tele_map, SIGNAL(clicked()),
              this, SLOT(teleportMapPressed()));
      QGridLayout* layout_tele = new QGridLayout(widget_teleport);
      layout_tele->setRowStretch(0, 1);
      layout_tele->addWidget(lbl_tele_map, 1, 0);
      layout_tele->addWidget(tele_map, 1, 1);
      layout_tele->addWidget(btn_tele_map, 1, 2);
      layout_tele->addWidget(lbl_tele_thing, 2, 0);
      layout_tele->addWidget(tele_thing, 2, 1);
      layout_tele->setRowStretch(3, 1);
      view_stack->addWidget(widget_teleport);
    }
    /* ---- TRIGGER IO ---- */
    else if(available_events[i] == EventClassifier::TRIGGERIO)
    {
      QWidget* widget_trigger = new QWidget(this);
      QGridLayout* layout_trigger = new QGridLayout(widget_trigger);

      QLabel* lbl_trigger = new QLabel("IO:", this);
      trigger_name = new QComboBox(this);
      EditorHelpers::comboBoxOptimize(trigger_name);
      connect(trigger_name, SIGNAL(currentIndexChanged(QString)),
              this, SLOT(triggerIOChanged(QString)));
      layout_trigger->setRowStretch(0, 1);
      layout_trigger->addWidget(lbl_trigger, 1, 0);
      layout_trigger->addWidget(trigger_name, 1, 1, 1, 3);
      layout_trigger->setRowStretch(2, 1);

      view_stack->addWidget(widget_trigger);
    }
    /* ---- UNLOCK: IO ---- */
    else if(available_events[i] == EventClassifier::UNLOCKIO)
    {
      QWidget* widget_unlock_io = new QWidget(this);
      QLabel* lbl_unio = new QLabel("IO:", this);
      unio_name = new QComboBox(this);
      EditorHelpers::comboBoxOptimize(unio_name);
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
      QMargins m = layout_unio_mode->contentsMargins();
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
      layout_unio->setRowStretch(0, 1);
      layout_unio->addWidget(lbl_unio, 1, 0);
      layout_unio->addWidget(unio_name, 1, 1, 1, 3);
      layout_unio->addWidget(unio_mode, 2, 0, 1, 4);
      layout_unio->addWidget(lbl_unio_state, 3, 0);
      layout_unio->addWidget(unio_state, 3, 1, 1, 3);
      layout_unio->addWidget(unio_event, 4, 0, 1, 4);
      layout_unio->addWidget(unio_view, 5, 0, 1, 4);
      layout_unio->setRowStretch(6, 1);
      view_stack->addWidget(widget_unlock_io);
    }
    /* ---- UNLOCK: THING ---- */
    else if(available_events[i] == EventClassifier::UNLOCKTHING)
    {
      QWidget* widget_unlock_thing = new QWidget(this);
      QLabel* lbl_unth = new QLabel("Thing:", this);
      unth_name = new QComboBox(this);
      EditorHelpers::comboBoxOptimize(unth_name);
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
      view_stack->addWidget(widget_unlock_thing);
    }
    /* ---- UNLOCK: TILE ---- */
    else if(available_events[i] == EventClassifier::UNLOCKTILE)
    {
      QWidget* widget_unlock_tile = new QWidget(this);
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
      QMargins m = layout_unti_event->contentsMargins();
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
      connect(unti_view_scroll, SIGNAL(stateChanged(int)),
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
      view_stack->addWidget(widget_unlock_tile);
    }
    /* ---- OUT OF RANGE: EMPTY WIDGET ---- */
    else
    {
      view_stack->addWidget(new QWidget(this));
    }
  }
  layout->addWidget(view_stack, 1, Qt::AlignHCenter);

  /* One Shot widget */
  check_oneshot = new QCheckBox("Only Trigger Once", this);
  check_oneshot->setDisabled(true);
  connect(check_oneshot, SIGNAL(stateChanged(int)),
          this, SLOT(changedOneShot(int)));
  layout->addWidget(check_oneshot, 0, Qt::AlignHCenter);

  /* Sound widget */
  QHBoxLayout* sound_layout = new QHBoxLayout();
  sound_layout->setContentsMargins(8, 0, 8, 0);
  QLabel* lbl_sound_2 = new QLabel("Sound:", this);
  sound_layout->addWidget(lbl_sound_2);
  combo_sound = new QComboBox(this);
  EditorHelpers::comboBoxOptimize(combo_sound);
  combo_sound->setDisabled(true);
  connect(combo_sound, SIGNAL(currentIndexChanged(QString)),
          this, SLOT(changedSound(QString)));
  sound_layout->addWidget(combo_sound, 1);
  layout->addLayout(sound_layout);

  /* Configure parent widget */
  setFrameStyle(QFrame::Panel);
  setLineWidth(1);
  QPalette palette;
  palette.setColor(QPalette::Foreground, QColor(168, 168, 168));
  setPalette(palette);
  updateGeometry();
  //setMaximumSize(EditorEnumDb::kEVENT_VIEW_W, EditorEnumDb::kEVENT_VIEW_H);
  //setMinimumSize(EditorEnumDb::kEVENT_VIEW_W, EditorEnumDb::kEVENT_VIEW_H);

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

  /* Event Pop-Up for isolated event edits */
  pop_event = new QDialog(this);
  pop_event->setWindowTitle("Event Edit");
  pop_event_layout = new QGridLayout(pop_event);
  pop_event_edit = nullptr;
  pop_event_view = nullptr;
  QPushButton* btn_event_ok = new QPushButton("Ok", pop_event);
  connect(btn_event_ok, SIGNAL(clicked()), this, SLOT(popEventOk()));
  pop_event_layout->addWidget(btn_event_ok, 1, 2);
  QPushButton* btn_event_cancel = new QPushButton("Cancel", pop_event);
  connect(btn_event_cancel, SIGNAL(clicked()), this, SLOT(popEventCancel()));
  pop_event_layout->addWidget(btn_event_cancel, 1, 3);
  pop_event->hide();
  connect(pop_event, SIGNAL(rejected()), this, SLOT(popEventCancel()));
}

/*
 * Description: Edits the current conversation instance trigger. Triggered
 *              from EventView. Required to lock out the pop-up.
 *
 * Inputs: Conversation* convo - the conversation segment to edit
 *         bool is_option - true if the segment is an option
 * Output: none
 */
void EventView::editConversation(Conversation* convo, bool is_option)
{
  /* Close conversation dialog */
  if(pop_convo != nullptr)
  {
    disconnect(pop_convo->getEventView(), SIGNAL(selectTile()),
               this, SLOT(selectTileConvo()));
    disconnect(pop_convo, SIGNAL(success()),
               this, SLOT(updateConversation()));
    delete pop_convo;
  }
  pop_convo = nullptr;

  if(pop_event_edit != nullptr)
    editEvent(nullptr);

  /* Create the new conversation dialog */
  if(convo != nullptr)
  {
    pop_convo = new ConvoDialog(convo, is_option, limiter, this);
    pop_convo->setListMapThings(list_map_things, list_map_ios, list_map_items,
                                list_map_persons, list_map_npcs);
    pop_convo->getEventView()->setListItems(list_items);
    pop_convo->getEventView()->setListMaps(list_maps);
    pop_convo->getEventView()->setListSounds(list_sounds);
    pop_convo->getEventView()->setListSubmaps(list_submaps);
    connect(pop_convo->getEventView(), SIGNAL(selectTile()),
            this, SLOT(selectTileConvo()));
    connect(pop_convo, SIGNAL(success()),
            this, SLOT(updateConversation()));
    pop_convo->setVisible(true);
  }
}

/*
 * Description: Starts the edit event processing by spawning the event pop-up
 *              for the specific passed event structure reference.
 *
 * Inputs: Event* edit_event - the event to edit. If null, just closes the pop
 * Output: none
 */
void EventView::editEvent(Event* edit_event)
{
  /* Check if the current event dialog is being used */
  if(pop_event_edit != nullptr)
  {
    pop_event->hide();
    pop_event_view->setEvent(nullptr);
    pop_event_layout->removeWidget(pop_event_view);
    pop_event_ctrl.setEventBlank();
    delete pop_event_view;
    pop_event_view = nullptr;
    pop_event_edit = nullptr;
  }

  if(pop_convo != nullptr)
    editConversation(nullptr, false);

  /* Open the new dialog */
  if(edit_event != nullptr)
  {
    /* Set data */
    pop_event_edit = edit_event;

    /* Add view */
    int limit_int = static_cast<int>(limiter);
    if(event->getEventType() == EventClassifier::BATTLESTART)
    {
      limit_int = static_cast<int>(EventClassifier::BATTLESTART);
    }
    else if(event->getEventType() == EventClassifier::MULTIPLE)
    {
      limit_int |= static_cast<int>(EventClassifier::MULTIPLE) |
                   static_cast<int>(EventClassifier::CONVERSATION) |
                   static_cast<int>(EventClassifier::BATTLESTART);
    }

    pop_event_view = new EventView(nullptr, pop_event,
                                   (EventClassifier)limit_int);
    pop_event_view->setListItems(list_items);
    pop_event_view->setListMaps(list_maps);
    pop_event_view->setListMapThings(list_map_things, list_map_ios,
                                     list_map_items, list_map_persons,
                                     list_map_npcs);
    pop_event_view->setListSounds(list_sounds);
    pop_event_view->setListSubmaps(list_submaps);
    connect(pop_event_view, SIGNAL(selectTile()),
            this, SLOT(selectTileEvent()));
    pop_event_layout->addWidget(pop_event_view, 0, 0, 1, 4);

    /* Connect up edit event and data */
    pop_event_ctrl.setEvent(*pop_event_edit);
    pop_event_view->setEvent(&pop_event_ctrl);
    pop_event->setVisible(true);
  }
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
 * Description: Triggers the select tile dialog for the primary event view.
 *              This emits the signal to the parent pop-up to hide and select
 *              the tile from the main map.
 *
 * Inputs: none
 * Output: none
 */
void EventView::selectTileMain()
{
  waiting_tile = true;
  waiting_tile_convo = false;
  waiting_tile_event = false;
  emit selectTile();
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
  if(event != nullptr)
  {
    /* Set the selected category */
    QString curr_event = QString::fromStdString(
                             EventSet::classifierToStr(event->getEventType()));
    if(curr_event != combo_category->currentText())
    {
      //combo_category->blockSignals(true);
      combo_category->setCurrentText(curr_event);
      //combo_category->setCurrentIndex((int)event->getEventType());
      //int index = (int)event->getEventType();
      //combo_category->setCurrentIndex(index);
      //view_stack->setCurrentIndex(index);
      //combo_sound->setEnabled(index != (int)EventClassifier::NOEVENT);

      //combo_category->blockSignals(false);
    }

    /* Set the data, based on the event */
    /* -- BATTLE START -- */
    if(event->getEventType() == EventClassifier::BATTLESTART)
    {
      /* Flag check boxes */
      bool win_disappear, lose_gg, restore_health, restore_qd;
      EventSet::dataEnumBattleFlags(event->getStartBattleFlags(), win_disappear,
                                    lose_gg, restore_health, restore_qd);
      battle_windisappear->setChecked(win_disappear);
      battle_losegg->setChecked(lose_gg);
      battle_restorehealth->setChecked(restore_health);
      battle_restoreqd->setChecked(restore_qd);

      /* Win event */
      Event* event_win = event->getStartBattleEventWin();
      QFont bold = battle_eventwin->font();
      bold.setBold(true);
      QFont not_bold = bold;
      not_bold.setBold(false);
      if(event_win->classification != EventClassifier::NOEVENT)
        battle_eventwin->setFont(bold);
      else
        battle_eventwin->setFont(not_bold);

      /* Lose event */
      Event* event_lose = event->getStartBattleEventLose();
      if(event_lose->classification != EventClassifier::NOEVENT)
        battle_eventlose->setFont(bold);
      else
        battle_eventlose->setFont(not_bold);
    }
    /* -- CONVERSATION -- */
    else if(event->getEventType() == EventClassifier::CONVERSATION)
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
    /* -- ITEM GIVE -- */
    else if(event->getEventType() == EventClassifier::ITEMGIVE)
    {
      /* Integers */
      item_chance->setValue(event->getGiveItemChance());
      item_count->setValue(event->getGiveItemCount());

      /* Flags */
      bool auto_drop;
      EventSet::dataEnumGiveFlags(event->getGiveItemFlags(), auto_drop);
      item_drop->setChecked(auto_drop);

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
    /* -- ITEM TAKE -- */
    else if(event->getEventType() == EventClassifier::ITEMTAKE)
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
    /* -- MAP SWITCH -- */
    else if(event->getEventType() == EventClassifier::MAPSWITCH)
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
    /* -- MULTIPLE -- */
    else if(event->getEventType() == EventClassifier::MULTIPLE)
    {
      mult_list->clear();
      std::vector<Event> events = event->getMultipleEvents();
      for(uint32_t i = 0; i < events.size(); i++)
      {
        QString prefix = QString::number(i) + ": ";
        mult_list->addItem(EditorEvent::classToText(events[i].classification,
                                                  prefix, events[i].one_shot));
      }
    }
    /* -- NOTIFICATION -- */
    else if(event->getEventType() == EventClassifier::NOTIFICATION)
    {
      notification_edit->setPlainText(event->getNotification());
    }
    /* -- PROPERTY -- */
    else if(event->getEventType() == EventClassifier::PROPERTY)
    {
      int ref_id = event->getPropertyID();

      /* Select the type */
      int type_index = prop_type->currentIndex();
      prop_type->setCurrentText(QString::fromStdString(
                                Helpers::typeToStr(event->getPropertyType())));
      if(type_index == prop_type->currentIndex())
        propertyTypeChange(prop_type->currentText());

      /* Select the ID */
      int index = -1;
      for(int i = 0; i < prop_id->count(); i++)
      {
        QStringList str_list = prop_id->itemText(i).split(':');
        if(str_list.size() >= 2)
          if(str_list.front().toInt() == ref_id)
            index = i;
      }
      if(index >= 0)
        prop_id->setCurrentIndex(index);
      else
        prop_id->setCurrentIndex(0);

      /* Select the properties to be modded */
      bool active,forced,inactive,move,reset,respawn,speed,track,visible;
      EventSet::dataEnumProperties(event->getPropertyMods(), active, forced,
                                   inactive, move, reset, respawn, speed,
                                   track, visible);
      prop_active_mod->setChecked(active);
      prop_forced_mod->setChecked(forced);
      prop_freeze_mod->setChecked(move);
      prop_inactive_mod->setChecked(inactive);
      prop_reset_mod->setChecked(reset);
      prop_respawn_mod->setChecked(respawn);
      prop_speed_mod->setChecked(speed);
      prop_track_mod->setChecked(track);
      prop_visible_mod->setChecked(visible);

      /* The bool properties */
      EventSet::dataEnumProperties(event->getPropertyBools(), active, forced,
                                   inactive, move, reset, respawn, speed,
                                   track, visible);
      prop_active_val->setChecked(active);
      prop_forced_val->setChecked(forced);
      prop_freeze_val->setChecked(move);
      prop_reset_val->setChecked(reset);
      prop_visible_val->setChecked(visible);

      /* Inactive */
      int inactive_val = event->getPropertyInactive();
      prop_inactive_dis->setChecked(inactive_val < 0);
      propertyInactiveDisable(prop_inactive_dis->checkState());
      if(inactive_val >= 0)
        prop_inactive_val->setValue(inactive_val);

      /* Respawn */
      int respawn_val = event->getPropertyRespawn();
      prop_respawn_dis->setChecked(respawn_val < 0);
      propertyRespawnDisable(prop_respawn_dis->checkState());
      if(respawn_val >= 0)
        prop_respawn_val->setValue(respawn_val);

      /* Speed */
      int speed_val = event->getPropertySpeed();
      if(speed_val >= 0)
        prop_speed_val->setValue(speed_val);

      /* Tracking */
      TrackingState track_val = event->getPropertyTrack();
      prop_track_opt->setCurrentText(QString::fromStdString(
                                         Helpers::trackingToStr(track_val)));

    }
    /* -- TELEPORT THING -- */
    else if(event->getEventType() == EventClassifier::TELEPORTTHING)
    {
      /* Attempt to find the map name in the combo box */
      QString name = "";
      for(int i = 0; i < list_submaps.size(); i++)
      {
        QStringList set = list_submaps[i].split(":");
        if(set.size() >= 2)
        {
          if(set.front().toInt() == event->getTeleportSection())
          {
            for(int i = 1; i < set.size(); i++)
              name += set[i] + ":";
            name.chop(1);
          }
        }
      }

      /* Load the name and information */
      tele_map->setText(QString::number(event->getTeleportSection()) +
                        ":" + name + "  | X: " +
                        QString::number(event->getTeleportX()) + " | Y: " +
                        QString::number(event->getTeleportY()));

      /* Attempt to find the thing in the combo box */
      int index = -1;
      for(int i = 0; (index < 0) && (i < tele_thing->count()); i++)
      {
        QStringList set = tele_thing->itemText(i).split(":");
        if(set.size() == 2)
        {
          if(set.front().toInt() == event->getTeleportThingID())
          {
            tele_thing->setCurrentIndex(i);
            index = i;
          }
        }
      }
      if(index >= 0)
        tele_thing->setCurrentIndex(index);
      else
        tele_thing->setCurrentIndex(0);
      teleportThingChanged(tele_thing->currentIndex());
    }
    /* -- TRIGGER IO -- */
    else if(event->getEventType() == EventClassifier::TRIGGERIO)
    {
      /* Attempt to find thing in combo box */
      int index = -1;
      for(int i = 0; (index < 0) && (i < trigger_name->count()); i++)
      {
        QStringList set = trigger_name->itemText(i).split(":");
        if(set.size() == 2)
          if(set.front().toInt() == event->getTriggerIOID())
            index = i;
      }
      if(index >= 0)
        trigger_name->setCurrentIndex(index);
      else
        trigger_name->setCurrentIndex(0);
      triggerIOChanged(trigger_name->currentText());
    }
    /* -- UNLOCK IO -- */
    else if(event->getEventType() == EventClassifier::UNLOCKIO)
    {
      /* Initial reference data */
      int ref_state = event->getUnlockIOState();

      /* Attempt to find thing in combo box */
      int index = -1;
      for(int i = 0; (index < 0) && (i < unio_name->count()); i++)
      {
        QStringList set = unio_name->itemText(i).split(":");
        if(set.size() == 2)
          if(set.front().toInt() == event->getUnlockIOID())
            index = i;
      }
      if(index >= 0)
        unio_name->setCurrentIndex(index);
      else
        unio_name->setCurrentIndex(0);
      unlockIOChanged(unio_name->currentText());

      /* Main mode check boxes */
      bool lock, events;
      EventSet::dataEnumIOMode(event->getUnlockIOMode(), lock, events);
      unio_mode_lock->setChecked(lock);
      unio_mode_states->setChecked(events);

      /* State info */
      if(events)
      {
        /* Selected state */
        for(int i = 0; i < unio_state->count(); i++)
        {
          QStringList set = unio_state->itemText(i).split(":");
          if(set.size() == 2)
            if(set.front().toInt() == ref_state)
              unio_state->setCurrentIndex(i);
        }

        /* Event check boxes */
        bool enter, exit, use, walkover;
        EventSet::dataEnumIOEvent(event->getUnlockIOEventMode(),
                                  enter, exit, use, walkover);
        unio_event_enter->setChecked(enter);
        unio_event_exit->setChecked(exit);
        unio_event_use->setChecked(use);
        unio_event_walk->setChecked(walkover);
      }

      /* View info */
      bool view, scroll;
      EventSet::dataEnumView(event->getUnlockViewMode(), view, scroll);
      unio_view_enable->setChecked(view);
      unio_view_scroll->setChecked(scroll);
      unio_view_time->setValue(event->getUnlockViewTime());
    }
    /* -- UNLOCK THING -- */
    else if(event->getEventType() == EventClassifier::UNLOCKTHING)
    {
      /* Attempt to find thing in combo box */
      int index = -1;
      for(int i = 0; (index < 0) && (i < unth_name->count()); i++)
      {
        QStringList set = unth_name->itemText(i).split(":");
        if(set.size() == 2)
          if(set.front().toInt() == event->getUnlockThingID())
            index = i;
      }
      if(index >= 0)
        unth_name->setCurrentIndex(index);
      else
        unth_name->setCurrentIndex(0);
      unlockThingChanged(unth_name->currentText());

      /* View data */
      bool view, scroll;
      EventSet::dataEnumView(event->getUnlockViewMode(), view, scroll);
      unth_view_enable->setChecked(view);
      unth_view_scroll->setChecked(scroll);
      unth_view_time->setValue(event->getUnlockViewTime());
    }
    /* -- UNLOCK TILE -- */
    else if(event->getEventType() == EventClassifier::UNLOCKTILE)
    {
      /* Attempt to find the map name */
      QString name = "";
      for(int i = 0; i < list_submaps.size(); i++)
      {
        QStringList set = list_submaps[i].split(":");
        if(set.size() >= 2)
        {
          if(set.front().toInt() == event->getUnlockTileSection())
          {
            for(int i = 1; i < set.size(); i++)
              name += set[i] + ":";
            name.chop(1);
          }
        }
      }

      /* Load the name and information */
      unti_location->setText(QString::number(event->getUnlockTileSection()) +
                             ":" + name + "  | X: " +
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

    /* Data for one shot */
    if(event->getEventType() != EventClassifier::NOEVENT)
    {
      check_oneshot->setChecked(event->isOneShot());
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
    combo_category->setCurrentText(QString::fromStdString(
                         EventSet::classifierToStr(EventClassifier::NOEVENT)));
    //combo_category->setCurrentIndex(0);
    combo_category->setDisabled(true);
  }

  /* View handling */
  setDisabled(view_only);
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

  /* This will be used for flattening once new HTML tags are used */
  //QString s = "<i>Test:</i><img src=\"blah.png\" /><br> A test case";
  //s.remove(QRegExp("<[^>]*>"));
  // // s == "Test: A test case"
  /* End test */

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

/*
 * Description: Updates the property list within the event set. This is based
 *              on the current type.
 *
 * Inputs: QString type_text - the type string for the list update
 * Output: none
 */
void EventView::updateListProperty(const QString& type_text)
{
  updateListProperty(Helpers::typeFromStr(type_text.toStdString()));
}

/*
 * Description: Updates the property list within the event set. This is based
 *              on the current type.
 *
 * Inputs: ThingBase type - the type category for the list update
 * Output: none
 */
void EventView::updateListProperty(const ThingBase& type)
{
  prop_id->blockSignals(true);
  prop_id->clear();
  prop_id->addItem("-1: This " +
                   QString::fromStdString(Helpers::typeToStr(type)));
  if(type == ThingBase::THING)
    prop_id->addItems(list_map_things);
  else if(type == ThingBase::PERSON)
    prop_id->addItems(list_map_persons);
  else if(type == ThingBase::NPC)
    prop_id->addItems(list_map_npcs);
  else if(type == ThingBase::ITEM)
    prop_id->addItems(list_map_items);
  else if(type == ThingBase::INTERACTIVE)
    for(int i = 0; i < list_map_ios.size(); i++)
      prop_id->addItem(list_map_ios[i].first);
  prop_id->blockSignals(false);
}

/*
 * Description: Updates all map object lists within event sets, based on what
 *              has changed
 *
 * Inputs: bool things - true if the things list was updated. default false
 *         bool ios - true if the ios list was updated. default false
 *         bool items - true if the items list was updated. default false
 *         bool persons - true if the persons list was updated. default false
 *         bool npcs - true if the npcs list was updated. default false
 * Output: none
 */
void EventView::updateLists(bool things, bool ios, bool items,
                            bool persons, bool npcs)
{
  (void)items;

  /* The IO names and IDs */
  QList<QString> front_ios;
  for(int i = 0; i < list_map_ios.size(); i++)
    front_ios.push_back(list_map_ios[i].first);

  /* Signal blocks */
  tele_thing->blockSignals(true);
  trigger_name->blockSignals(true);
  unio_name->blockSignals(true);
  unth_name->blockSignals(true);

  /* Teleport Thing */
  bool update_tele = (things || ios || persons || npcs);
  if(update_tele)
  {
    tele_thing->clear();
    //tele_thing->addItem("-1: This Thing");
    tele_thing->addItems(list_map_persons + list_map_npcs
                                          + list_map_things + front_ios);
  }

  /* Trigger IO */
  bool update_trigger = ios;
  if(update_trigger)
  {
    trigger_name->clear();
    trigger_name->addItem("-1: This IO");
    trigger_name->addItems(front_ios);
  }

  /* Unlock IO */
  bool update_unio = ios;
  if(update_unio)
  {
    unio_name->clear();
    unio_name->addItem("-1: This IO");
    unio_name->addItems(front_ios);
  }

  /* Unlock Thing */
  bool update_unth = (things || persons || npcs);
  if(update_unth)
  {
    unth_name->clear();
    unth_name->addItem("-1: This Thing");
    unth_name->addItems(list_map_persons + list_map_npcs + list_map_things);
  }

  /* Restore signal blocks */
  setLayoutData();
  tele_thing->blockSignals(false);
  trigger_name->blockSignals(false);
  unio_name->blockSignals(false);
  unth_name->blockSignals(false);
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

/*
 * Description: Slot triggered on start battle event on the lose event edit
 *              button being pressed. Opens the dialog to edit the lose event.
 *
 * Inputs: none
 * Output: none
 */
void EventView::battleEventLoseEdit()
{
  if(event->getEventType() == EventClassifier::BATTLESTART)
  {
    editEvent(event->getStartBattleEventLose());
  }
}

/*
 * Description: Slot triggered on start battle event on the win event edit
 *              button being pressed. Opens the dialog to edit the lose event.
 *
 * Inputs: none
 * Output: none
 */
void EventView::battleEventWinEdit()
{
  if(event->getEventType() == EventClassifier::BATTLESTART)
  {
    editEvent(event->getStartBattleEventWin());
  }
}

/*
 * Description: Slot triggered on start battle event on the restore health flag
 *              upon battle ending change of state. Updates settings within
 *              the event.
 *
 * Inputs: int state - the new state of the dialog (3 possible)
 * Output: none
 */
void EventView::battleHealthFlagChange(int state)
{
  if(event->getEventType() == EventClassifier::BATTLESTART)
  {
    editEvent(nullptr);

    /* Update the flag */
    bool win_disappear, lose_gg, restore_health, restore_qd;
    BattleFlags flags = event->getStartBattleFlags();
    EventSet::dataEnumBattleFlags(flags, win_disappear, lose_gg,
                                  restore_health, restore_qd);
    restore_health = (state == Qt::Checked);
    flags = EventSet::createEnumBattleFlags(win_disappear, lose_gg,
                                            restore_health, restore_qd);

    /* Update the event data */
    event->setEventStartBattle(flags, *event->getStartBattleEventWin(),
                               *event->getStartBattleEventLose(),
                               event->getSoundID());
  }
}

/*
 * Description: Slot triggered on start battle event on the lose causes game
 *              over flag change of state. Updates settings within the event.
 *
 * Inputs: int state - the new state of the dialog (3 possible)
 * Output: none
 */
void EventView::battleLoseFlagChange(int state)
{
  if(event->getEventType() == EventClassifier::BATTLESTART)
  {
    editEvent(nullptr);

    /* Update the flag */
    bool win_disappear, lose_gg, restore_health, restore_qd;
    BattleFlags flags = event->getStartBattleFlags();
    EventSet::dataEnumBattleFlags(flags, win_disappear, lose_gg,
                                  restore_health, restore_qd);
    lose_gg = (state == Qt::Checked);
    flags = EventSet::createEnumBattleFlags(win_disappear, lose_gg,
                                            restore_health, restore_qd);

    /* Update the event data */
    event->setEventStartBattle(flags, *event->getStartBattleEventWin(),
                               *event->getStartBattleEventLose(),
                               event->getSoundID());

    /* Enable/Disable of the edit button for the lose event */
    battle_eventlose->setDisabled(lose_gg);
  }
}

/*
 * Description: Slot triggered on start battle event on the restore QD flag
 *              upon battle ending change of state. Updates settings within
 *              the event.
 *
 * Inputs: int state - the new state of the dialog (3 possible)
 * Output: none
 */
void EventView::battleQDFlagChange(int state)
{
  if(event->getEventType() == EventClassifier::BATTLESTART)
  {
    editEvent(nullptr);

    /* Update the flag */
    bool win_disappear, lose_gg, restore_health, restore_qd;
    BattleFlags flags = event->getStartBattleFlags();
    EventSet::dataEnumBattleFlags(flags, win_disappear, lose_gg,
                                  restore_health, restore_qd);
    restore_qd = (state == Qt::Checked);
    flags = EventSet::createEnumBattleFlags(win_disappear, lose_gg,
                                            restore_health, restore_qd);

    /* Update the event data */
    event->setEventStartBattle(flags, *event->getStartBattleEventWin(),
                               *event->getStartBattleEventLose(),
                               event->getSoundID());
  }
}

/*
 * Description: Slot triggered on start battle event on the win causes game
 *              over flag change of state. Updates settings within the event.
 *
 * Inputs: int state - the new state of the dialog (3 possible)
 * Output: none
 */
void EventView::battleWinFlagChange(int state)
{
  if(event->getEventType() == EventClassifier::BATTLESTART)
  {
    editEvent(nullptr);

    /* Update the flag */
    bool win_disappear, lose_gg, restore_health, restore_qd;
    BattleFlags flags = event->getStartBattleFlags();
    EventSet::dataEnumBattleFlags(flags, win_disappear, lose_gg,
                                  restore_health, restore_qd);
    win_disappear = (state == Qt::Checked);
    flags = EventSet::createEnumBattleFlags(win_disappear, lose_gg,
                                            restore_health, restore_qd);

    /* Update the event data */
    event->setEventStartBattle(flags, *event->getStartBattleEventWin(),
                               *event->getStartBattleEventLose(),
                               event->getSoundID());

    /* Enable/Disable of the edit button for the win event */
    //battle_eventwin->setDisabled(win_disappear); // Removed [2016-06-05]
  }
}

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
  closeAllPopups();
  view_stack->setCurrentIndex(index);

  /* If anything except no classification, enables sound widget */
  check_oneshot->setEnabled(index != (int)EventClassifier::NOEVENT);
  combo_sound->setEnabled(index != (int)EventClassifier::NOEVENT);

  /* Update the event */
  if(event != nullptr && index >= 0 && index < available_events.size())
  {
    EventClassifier new_class = available_events[index];

    if(event->getEventType() != new_class)
    {
      /* Change the event */
      if(new_class == EventClassifier::BATTLESTART)
        event->setEventStartBattle();
      else if(new_class == EventClassifier::CONVERSATION)
        event->setEventConversation();
      else if(new_class == EventClassifier::ITEMGIVE)
        event->setEventGiveItem();
      else if(new_class == EventClassifier::ITEMTAKE)
        event->setEventTakeItem();
      else if(new_class == EventClassifier::MAPSWITCH)
        event->setEventStartMap();
      else if(new_class == EventClassifier::MULTIPLE)
        event->setEventMultiple();
      else if(new_class == EventClassifier::NOTIFICATION)
        event->setEventNotification();
      else if(new_class == EventClassifier::PROPERTY)
        event->setEventPropMod();
      else if(new_class == EventClassifier::SOUNDONLY)
        event->setEventSound();
      else if(new_class == EventClassifier::TELEPORTTHING)
        event->setEventTeleport();
      else if(new_class == EventClassifier::TRIGGERIO)
        event->setEventTriggerIO();
      else if(new_class == EventClassifier::UNLOCKIO)
        event->setEventUnlockIO();
      else if(new_class == EventClassifier::UNLOCKTHING)
        event->setEventUnlockThing();
      else if(new_class == EventClassifier::UNLOCKTILE)
        event->setEventUnlockTile();
      else if(new_class == EventClassifier::NOEVENT)
        event->setEventBlank();
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
 * Description: Triggers when the one shot trigger check box changes state
 *              by user interaction.
 *
 * Inputs: int state - the state of the checkbox
 * Output: none
 */
void EventView::changedOneShot(int state)
{
  event->setOneShot(state == Qt::Checked);
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
    closeAllPopups();

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
 * Description: Slot which triggers when the auto drop status check box is
 *              changed.
 *
 * Inputs: int state - the state of the checkbox
 * Output: none
 */
void EventView::giveAutoDropChanged(int state)
{
  /* Get the starting data */
  bool auto_drop;
  GiveItemFlags flags = event->getGiveItemFlags();
  EventSet::dataEnumGiveFlags(flags, auto_drop);

  /* Modify */
  auto_drop = (state == Qt::Checked);

  /* Set the new data */
  flags = EventSet::createEnumGiveFlags(auto_drop);
  event->setEventGiveItem(event->getGiveItemID(), event->getGiveItemCount(),
                          flags, event->getGiveItemChance(),
                          event->getSoundID());
}

/*
 * Description: Slot which triggers when the give item chance widget in the give
 *              item event is changed. Updates the event
 *
 * Inputs: int chance - the chance value
 * Output: none
 */
void EventView::giveChanceChanged(int chance)
{
  event->setEventGiveItem(event->getGiveItemID(), event->getGiveItemCount(),
                          event->getGiveItemFlags(), chance,
                          event->getSoundID());
}

/*
 * Description: Slot which triggers when the give item count widget in the give
 *              item event is changed. Updates the event
 *
 * Inputs: int count - the count value
 * Output: none
 */
void EventView::giveCountChanged(int count)
{
  event->setEventGiveItem(event->getGiveItemID(), count,
                          event->getGiveItemFlags(), event->getGiveItemChance(),
                          event->getSoundID());
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
                              event->getGiveItemFlags(),
                              event->getGiveItemChance(), event->getSoundID());
    }
  }
}

/*
 * Description: Slot which triggers when the multiple event add button is
 *              pressed. Adds an event to the bottom of the stack
 *
 * Inputs: none
 * Output: none
 */
void EventView::multBtnAdd()
{
  if(event->getEventType() == EventClassifier::MULTIPLE)
  {
    /* Clear out any edits */
    editEvent();

    /* Access data and update */
    std::vector<Event> set = event->getMultipleEvents();
    event->setEventMultiple(set.size(), EventSet::createBlankEvent());
    updateMultiple();
    mult_list->setCurrentRow(mult_list->count() - 1);
  }
}

/*
 * Description: Slot which triggers when the multiple event down button is
 *              pressed. Takes the selected event and moves it one spot down
 *
 * Inputs: none
 * Output: none
 */
void EventView::multBtnDown()
{
  if(event->getEventType() == EventClassifier::MULTIPLE)
  {
    /* Clear out any edits */
    editEvent();

    /* Access data */
    Event* event_struct = event->getEvent();

    /* Get the current index */
    int index = mult_list->currentRow();
    int new_index = index + 1;
    if(index >= 0 && new_index < static_cast<int>(event_struct->events.size()))
    {
      Event old_loc = event_struct->events[index];
      event_struct->events[index] = event_struct->events[new_index];
      event_struct->events[new_index] = old_loc;
      updateMultiple();
      mult_list->setCurrentRow(new_index);
    }
  }
}

/*
 * Description: Slot which triggers when the multiple event edit button is
 *              pressed. Edits the selected event in the list
 *
 * Inputs: none
 * Output: none
 */
void EventView::multBtnEdit()
{
  if(event->getEventType() == EventClassifier::MULTIPLE)
  {
    editEvent(event->getMultipleEvent(mult_list->currentRow()));
  }
}

/*
 * Description: Slot which triggers when the multiple event remove button is
 *              pressed. Removes the selected event from the list
 *
 * Inputs: none
 * Output: none
 */
void EventView::multBtnRem()
{
  if(event->getEventType() == EventClassifier::MULTIPLE)
  {
    /* Clear out any edits */
    editEvent();

    /* Access data */
    Event* event_struct = event->getEvent();
    int index = mult_list->currentRow();

    /* Determine if index is valid and delete */
    if(index >= 0 && index < static_cast<int>(event_struct->events.size()))
    {
      EventSet::deleteEvent(event_struct->events[index]);
      event_struct->events.erase(event_struct->events.begin() + index);
      updateMultiple();
    }
  }
}

/*
 * Description: Slot which triggers when the multiple event up button is
 *              pressed. Takes the selected event and moves it one spot up
 *
 * Inputs: none
 * Output: none
 */
void EventView::multBtnUp()
{
  if(event->getEventType() == EventClassifier::MULTIPLE)
  {
    /* Clear out any edits */
    editEvent();

    /* Access data */
    Event* event_struct = event->getEvent();

    /* Get the current index */
    int index = mult_list->currentRow();
    int new_index = index - 1;
    if(new_index >= 0 && index < static_cast<int>(event_struct->events.size()))
    {
      Event old_loc = event_struct->events[index];
      event_struct->events[index] = event_struct->events[new_index];
      event_struct->events[new_index] = old_loc;
      updateMultiple();
      mult_list->setCurrentRow(new_index);
    }
  }
}

/*
 * Description: Slot which triggers when the selected index in the list is
 *              changed. Updates the enabled status of the modifier buttons
 *
 * Inputs: int current_row - the new selected row
 * Output: none
 */
void EventView::multListChange(int current_row)
{
  mult_btn_down->setEnabled(current_row >= 0 &&
                            (current_row + 1) < mult_list->count());
  mult_btn_edit->setEnabled(current_row >= 0);
  mult_btn_rem->setEnabled(current_row >= 0);
  mult_btn_up->setEnabled(current_row >= 1);
}

/*
 * Description: Slot which triggers when the selected index in the list is
 *              double clicked. Triggers an edit on the selected event
 *
 * Inputs: QListWidgetItem* - not used
 * Output: none
 */
void EventView::multListDouble(QListWidgetItem*)
{
  multBtnEdit();
}

/*
 * Description: Slot which triggers when the notification color changing button
 *              is pressed. Opens dialog to select from color selection
 *              (QColorDialog).
 *
 * Inputs: none
 * Output: none
 */
void EventView::notificationBtnBrush()
{
  /* Get the color */
  QColorDialog color_dialog;
  if(color_dialog.exec() == QDialog::Accepted)
  {
    QString selected_color = color_dialog.selectedColor().name();
    selected_color.remove(0, 1);

    /* Convert to format and insert in as pseudo html */
    QString selected_text = notification_edit->textCursor().selectedText();
    selected_text = "[" + selected_color + "]" + selected_text +
                    "[/" + selected_color + "]";
    notification_edit->textCursor().insertText(selected_text);
  }
}

/*
 * Description: Slot which triggers when the notification bold button is
 *              pressed. Inserts the relevant pseudo xml for making the
 *              selection bold.
 *
 * Inputs: none
 * Output: none
 */
void EventView::notificationBtnBold()
{
  QString selected_text = notification_edit->textCursor().selectedText();
  selected_text = "[b]" + selected_text + "[/b]";
  notification_edit->textCursor().insertText(selected_text);
}

/*
 * Description: Slot which triggers when the notification italic button is
 *              pressed. Inserts the relevant pseudo xml for making the
 *              selection italic.
 *
 * Inputs: none
 * Output: none
 */
void EventView::notificationBtnItalic()
{
  QString selected_text = notification_edit->textCursor().selectedText();
  selected_text = "[i]" + selected_text + "[/i]";
  notification_edit->textCursor().insertText(selected_text);
}

/*
 * Description: Slot which triggers when the notification item insertion is
 *              pressed. Allows for selection of a item ID to insert into the
 *              notification.
 *
 * Inputs: none
 * Output: none
 */
void EventView::notificationBtnItem()
{
  /* Create input dialog to get selected thing */
  QInputDialog input_dialog;
  input_dialog.setComboBoxItems(list_items);
  input_dialog.setWindowTitle("Item Name Select");
  input_dialog.setLabelText("Select Item Name to Insert:");
  if(input_dialog.exec() == QDialog::Accepted)
  {
    /* Check if the selected is valid and an ID */
    QStringList split = input_dialog.textValue().split(':');
    if(split.size() >= 2)
    {
      /* If valid, insert the number */
      bool ok;
      split.first().toInt(&ok);
      if(ok)
      {
        QString insert_text = "{I" + split.first() + "}";
        notification_edit->textCursor().insertText(insert_text);
      }
    }
  }
}

/*
 * Description: Slot which triggers when the notification preview is selected.
 *              This loads the current xml for a quick preview of how it will
 *              render.
 *
 * Inputs: none
 * Output: none
 */
void EventView::notificationBtnPreview()
{
  /* Compile list */
  QList<QString> list_things = list_map_persons + list_map_npcs +
                               list_map_things;
  for(int i = 0; i < list_map_ios.size(); i++)
    list_things.push_back(list_map_ios[i].first);
  //list_vec += list_map_items;

  /* Handle conversion */
  QString render_text = EditorHelpers::convertXml(
                                            notification_edit->toPlainText(),
                                            list_things, list_items);

  /* Insert warning */
  QString info_text = QString("<font color=\"#900\">* Overlap nested color ") +
                      QString("rendering might not be exactly as shown</font>");

  /* Render */
  QMessageBox msg_box;
  msg_box.setText(render_text);
  msg_box.setInformativeText(info_text);
  msg_box.setWindowTitle("Preview");
  msg_box.exec();
}

/*
 * Description: Slot which triggers when the notification thing insertion is
 *              pressed. Allows for selection of a thing ID to insert into the
 *              notification.
 *
 * Inputs: none
 * Output: none
 */
void EventView::notificationBtnThing()
{
  /* Compile list of all things */
  QList<QString> list = list_map_persons;
  QString player = list.first();
  list.removeFirst();
  list += list_map_npcs + list_map_things;
  for(int i = 0; i < list_map_ios.size(); i++)
    list.push_back(list_map_ios[i].first);
  //list += list_map_items;
  qSort(list);
  list.push_front(player);

  /* Create input dialog to get selected thing */
  QInputDialog input_dialog;
  input_dialog.setComboBoxItems(list);
  input_dialog.setWindowTitle("Thing Name Select");
  input_dialog.setLabelText("Select Thing Name to Insert:");
  if(input_dialog.exec() == QDialog::Accepted)
  {
    /* Check if the selected is valid and an ID */
    QStringList split = input_dialog.textValue().split(':');
    if(split.size() >= 2)
    {
      /* If valid, insert the number */
      bool ok;
      split.first().toInt(&ok);
      if(ok)
      {
        QString insert_text = "{" + split.first() + "}";
        notification_edit->textCursor().insertText(insert_text);
      }
    }
  }
}

/*
 * Description: Slot which triggers when the notification underline button is
 *              pressed. Inserts the relevant pseudo xml for making the
 *              selection underlined.
 *
 * Inputs: none
 * Output: none
 */
void EventView::notificationBtnUnderline()
{
  QString selected_text = notification_edit->textCursor().selectedText();
  selected_text = "[u]" + selected_text + "[/u]";
  notification_edit->textCursor().insertText(selected_text);
}

/*
 * Description: Slot triggered when the text selection in the notification
 *              changes. This controls if the buttons are enabled.
 *
 * Inputs: bool yes - text is selected
 * Output: none
 */
void EventView::notificationSelected(bool yes)
{
  notify_btn_brush->setEnabled(yes);
  notify_btn_bold->setEnabled(yes);
  notify_btn_italic->setEnabled(yes);
  //notify_btn_thing->setEnabled(yes);
  notify_btn_underline->setEnabled(yes);
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
  QString text_set = notification_edit->toPlainText();
  text_set.replace('<', '[');
  text_set.replace('>', ']');
  event->setEventNotification(text_set, event->getSoundID());
}

/*
 * Description: Slot which triggers on the cancel button within the edit event
 *              pop-up. Ends the edit without saving
 *
 * Inputs: none
 * Output: none
 */
void EventView::popEventCancel()
{
  editEvent(nullptr);
}

/*
 * Description: Slot which triggers on the ok button within the edit event
 *              pop-up. Ends the edit after saving the changes
 *
 * Inputs: none
 * Output: none
 */
void EventView::popEventOk()
{
  if(pop_event_edit != nullptr)
  {
    /* Update event */
    *pop_event_edit = EventSet::deleteEvent(*pop_event_edit);
    *pop_event_edit = EventSet::copyEvent(*pop_event_ctrl.getEvent());
    editEvent(nullptr);

    /* Update data visible */
    if(event->getEventType() == EventClassifier::BATTLESTART)
      updateBattle();
    else if(event->getEventType() == EventClassifier::MULTIPLE)
      updateMultiple();
  }
}

/*
 * Description: Slot which triggers when the active modifier check box is
 *              changed. Enables the active to be modified.
 *
 * Inputs: int state - the state of the checkbox
 * Output: none
 */
void EventView::propertyActiveMod(int state)
{
  /* Get the existing */
  bool active, forced, inactive, move, reset, respawn, speed, track, visible;
  EventSet::dataEnumProperties(event->getPropertyMods(), active, forced,
                               inactive, move, reset, respawn, speed,
                               track, visible);

  /* Modify and set */
  active = (state == Qt::Checked);
  ThingProperty prop_mods = EventSet::createEnumProperties(active, forced,
                        inactive, move, reset, respawn, speed, track, visible);
  event->setEventPropMod(event->getPropertyType(), event->getPropertyID(),
                         prop_mods, event->getPropertyBools(),
                         event->getPropertyRespawn(), event->getPropertySpeed(),
                         event->getPropertyTrack(),
                         event->getPropertyInactive(), event->getSoundID());

  /* Enable */
  if(prop_active_mod->isEnabled())
  {
    prop_active_val->setEnabled(active);
  }
}

/*
 * Description: Slot which triggers when the active value mod check box is
 *              changed. If checked, the thing will be set to active
 *
 * Inputs: int state - the state of the checkbox
 * Output: none
 */
void EventView::propertyActiveVal(int state)
{
  /* Get the existing */
  bool active, forced, inactive, move, reset, respawn, speed, track, visible;
  EventSet::dataEnumProperties(event->getPropertyBools(), active, forced,
                               inactive, move, reset, respawn, speed,
                               track, visible);

  /* Modify and set */
  active = (state == Qt::Checked);
  ThingProperty prop = EventSet::createEnumProperties(active, forced, inactive,
                                  move, reset, respawn, speed, track, visible);
  event->setEventPropMod(event->getPropertyType(), event->getPropertyID(),
                         event->getPropertyMods(), prop,
                         event->getPropertyRespawn(), event->getPropertySpeed(),
                         event->getPropertyTrack(),
                         event->getPropertyInactive(), event->getSoundID());
}

/*
 * Description: Slot which triggers when the forced modifier check box is
 *              changed. Enables the forced to be modified.
 *
 * Inputs: int state - the state of the checkbox
 * Output: none
 */
void EventView::propertyForcedMod(int state)
{
  /* Get the existing */
  bool active, forced, inactive, move, reset, respawn, speed, track, visible;
  EventSet::dataEnumProperties(event->getPropertyMods(), active, forced,
                               inactive, move, reset, respawn, speed,
                               track, visible);

  /* Modify and set */
  forced = (state == Qt::Checked);
  ThingProperty prop_mods = EventSet::createEnumProperties(active, forced,
                        inactive, move, reset, respawn, speed, track, visible);
  event->setEventPropMod(event->getPropertyType(), event->getPropertyID(),
                         prop_mods, event->getPropertyBools(),
                         event->getPropertyRespawn(), event->getPropertySpeed(),
                         event->getPropertyTrack(),
                         event->getPropertyInactive(), event->getSoundID());

  /* Enable */
  if(prop_forced_mod->isEnabled())
  {
    prop_forced_val->setEnabled(forced);
  }
}

/*
 * Description: Slot which triggers when the forced value mod check box is
 *              changed. If checked, the thing will be set to forced interact
 *
 * Inputs: int state - the state of the checkbox
 * Output: none
 */
void EventView::propertyForcedVal(int state)
{
  /* Get the existing */
  bool active, forced, inactive, move, reset, respawn, speed, track, visible;
  EventSet::dataEnumProperties(event->getPropertyBools(), active, forced,
                               inactive, move, reset, respawn, speed,
                               track, visible);

  /* Modify and set */
  forced = (state == Qt::Checked);
  ThingProperty prop = EventSet::createEnumProperties(active, forced, inactive,
                                  move, reset, respawn, speed, track, visible);
  event->setEventPropMod(event->getPropertyType(), event->getPropertyID(),
                         event->getPropertyMods(), prop,
                         event->getPropertyRespawn(), event->getPropertySpeed(),
                         event->getPropertyTrack(),
                         event->getPropertyInactive(), event->getSoundID());
}

/*
 * Description: Slot which triggers when the freeze modifier check box is
 *              changed. Enables the freeze to be modified.
 *
 * Inputs: int state - the state of the checkbox
 * Output: none
 */
void EventView::propertyFreezeMod(int state)
{
  /* Get the existing */
  bool active, forced, inactive, move, reset, respawn, speed, track, visible;
  EventSet::dataEnumProperties(event->getPropertyMods(), active, forced,
                               inactive, move, reset, respawn, speed,
                               track, visible);

  /* Modify and set */
  move = (state == Qt::Checked);
  ThingProperty prop_mods = EventSet::createEnumProperties(active, forced,
                        inactive, move, reset, respawn, speed, track, visible);
  event->setEventPropMod(event->getPropertyType(), event->getPropertyID(),
                         prop_mods, event->getPropertyBools(),
                         event->getPropertyRespawn(), event->getPropertySpeed(),
                         event->getPropertyTrack(),
                         event->getPropertyInactive(), event->getSoundID());

  /* Enable */
  if(prop_freeze_mod->isEnabled())
  {
    prop_freeze_val->setEnabled(move);
  }
}

/*
 * Description: Slot which triggers when the freeze value mod check box is
 *              changed. If checked, the thing will be set to frozen
 *
 * Inputs: int state - the state of the checkbox
 * Output: none
 */
void EventView::propertyFreezeVal(int state)
{
  /* Get the existing */
  bool active, forced, inactive, move, reset, respawn, speed, track, visible;
  EventSet::dataEnumProperties(event->getPropertyBools(), active, forced,
                               inactive, move, reset, respawn, speed,
                               track, visible);

  /* Modify and set */
  move = (state == Qt::Checked);
  ThingProperty prop = EventSet::createEnumProperties(active, forced, inactive,
                                  move, reset, respawn, speed, track, visible);
  event->setEventPropMod(event->getPropertyType(), event->getPropertyID(),
                         event->getPropertyMods(), prop,
                         event->getPropertyRespawn(), event->getPropertySpeed(),
                         event->getPropertyTrack(),
                         event->getPropertyInactive(), event->getSoundID());
}

/*
 * Description: Slot which triggers when the selected ID to be modified in the
 *              combo box is changed.
 *
 * Inputs: const QString& text - the new ID text set
 * Output: none
 */
void EventView::propertyIDChange(const QString& text)
{
  int id = -1;
  QStringList str_list = text.split(':');
  if(str_list.size() >= 2)
    id = str_list.front().toInt();

  event->setEventPropMod(event->getPropertyType(), id,
                         event->getPropertyMods(), event->getPropertyBools(),
                         event->getPropertyRespawn(), event->getPropertySpeed(),
                         event->getPropertyTrack(),
                         event->getPropertyInactive(), event->getSoundID());
}

/*
 * Description: Slot which triggers when the inactive value disable check box is
 *              changed. If checked, the inactive delay return will be disabled
 *
 * Inputs: int state - the state of the checkbox
 * Output: none
 */
void EventView::propertyInactiveDisable(int state)
{
  int new_value = 0;

  if(state == Qt::Checked)
  {
    new_value = -1;
    if(prop_inactive_mod->isChecked())
      prop_inactive_val->setEnabled(false);
  }
  else
  {
    new_value = prop_inactive_val->value();
    if(prop_inactive_mod->isChecked())
      prop_inactive_val->setEnabled(true);
  }

  event->setEventPropMod(event->getPropertyType(), event->getPropertyID(),
                         event->getPropertyMods(), event->getPropertyBools(),
                         event->getPropertyRespawn(),
                         event->getPropertySpeed(), event->getPropertyTrack(),
                         new_value, event->getSoundID());
}

/*
 * Description: Slot which triggers when the inactive modifier check box is
 *              changed. Enables the inactive to be modified.
 *
 * Inputs: int state - the state of the checkbox
 * Output: none
 */
void EventView::propertyInactiveMod(int state)
{
  /* Get the existing */
  bool active, forced, inactive, move, reset, respawn, speed, track, visible;
  EventSet::dataEnumProperties(event->getPropertyMods(), active, forced,
                               inactive, move, reset, respawn, speed,
                               track, visible);

  /* Modify and set */
  inactive = (state == Qt::Checked);
  ThingProperty prop_mods = EventSet::createEnumProperties(active, forced,
                        inactive, move, reset, respawn, speed, track, visible);
  event->setEventPropMod(event->getPropertyType(), event->getPropertyID(),
                         prop_mods, event->getPropertyBools(),
                         event->getPropertyRespawn(), event->getPropertySpeed(),
                         event->getPropertyTrack(),
                         event->getPropertyInactive(), event->getSoundID());

  /* Enable */
  prop_inactive_dis->setEnabled(prop_inactive_mod->isEnabled() && inactive);
  prop_inactive_val->setEnabled(prop_inactive_mod->isEnabled() && inactive);
}

/*
 * Description: Slot which triggers when the inactive value mod spin box is
 *              changed. This new value is the new inactive time in ms
 *
 * Inputs: int value - the inactive value in ms
 * Output: none
 */
void EventView::propertyInactiveVal(int value)
{
  if(value >= 0)
    event->setEventPropMod(event->getPropertyType(), event->getPropertyID(),
                           event->getPropertyMods(), event->getPropertyBools(),
                           event->getPropertyRespawn(),
                           event->getPropertySpeed(), event->getPropertyTrack(),
                           value, event->getSoundID());
}

/*
 * Description: Slot which triggers when the reset location modifier check box
 *              is changed. Enables the reset location to be modified.
 *
 * Inputs: int state - the state of the checkbox
 * Output: none
 */
void EventView::propertyResetMod(int state)
{
  /* Get the existing */
  bool active, forced, inactive, move, reset, respawn, speed, track, visible;
  EventSet::dataEnumProperties(event->getPropertyMods(), active, forced,
                               inactive, move, reset, respawn, speed,
                               track, visible);

  /* Modify and set */
  reset = (state == Qt::Checked);
  ThingProperty prop_mods = EventSet::createEnumProperties(active, forced,
                        inactive, move, reset, respawn, speed, track, visible);
  event->setEventPropMod(event->getPropertyType(), event->getPropertyID(),
                         prop_mods, event->getPropertyBools(),
                         event->getPropertyRespawn(), event->getPropertySpeed(),
                         event->getPropertyTrack(),
                         event->getPropertyInactive(), event->getSoundID());

  /* Enable */
  if(prop_reset_mod->isEnabled())
  {
    prop_reset_val->setEnabled(reset);
  }
}

/*
 * Description: Slot which triggers when the reset value mod check box is
 *              changed. If checked, the thing location will be reset
 *
 * Inputs: int state - the state of the checkbox
 * Output: none
 */
void EventView::propertyResetVal(int state)
{
  /* Get the existing */
  bool active, forced, inactive, move, reset, respawn, speed, track, visible;
  EventSet::dataEnumProperties(event->getPropertyBools(), active, forced,
                               inactive, move, reset, respawn, speed,
                               track, visible);

  /* Modify and set */
  reset = (state == Qt::Checked);
  ThingProperty prop = EventSet::createEnumProperties(active, forced, inactive,
                                  move, reset, respawn, speed, track, visible);
  event->setEventPropMod(event->getPropertyType(), event->getPropertyID(),
                         event->getPropertyMods(), prop,
                         event->getPropertyRespawn(), event->getPropertySpeed(),
                         event->getPropertyTrack(),
                         event->getPropertyInactive(), event->getSoundID());
}

/*
 * Description: Slot which triggers when the respawn value disable check box is
 *              changed. If checked, the respawn delay return will be disabled
 *
 * Inputs: int state - the state of the checkbox
 * Output: none
 */
void EventView::propertyRespawnDisable(int state)
{
  int new_value = 0;

  if(state == Qt::Checked)
  {
    new_value = -1;
    if(prop_respawn_mod->isChecked())
      prop_respawn_val->setEnabled(false);
  }
  else
  {
    new_value = prop_respawn_val->value();
    if(prop_respawn_mod->isChecked())
      prop_respawn_val->setEnabled(true);
  }

  event->setEventPropMod(event->getPropertyType(), event->getPropertyID(),
                         event->getPropertyMods(), event->getPropertyBools(),
                         new_value, event->getPropertySpeed(),
                         event->getPropertyTrack(),
                         event->getPropertyInactive(), event->getSoundID());
}

/*
 * Description: Slot which triggers when the respawn time modifier check box is
 *              changed. Enables the respawn time to be modified.
 *
 * Inputs: int state - the state of the checkbox
 * Output: none
 */
void EventView::propertyRespawnMod(int state)
{
  /* Get the existing */
  bool active, forced, inactive, move, reset, respawn, speed, track, visible;
  EventSet::dataEnumProperties(event->getPropertyMods(), active, forced,
                               inactive, move, reset, respawn, speed,
                               track, visible);

  /* Modify and set */
  respawn = (state == Qt::Checked);
  ThingProperty prop_mods = EventSet::createEnumProperties(active, forced,
                        inactive, move, reset, respawn, speed, track, visible);
  event->setEventPropMod(event->getPropertyType(), event->getPropertyID(),
                         prop_mods, event->getPropertyBools(),
                         event->getPropertyRespawn(), event->getPropertySpeed(),
                         event->getPropertyTrack(),
                         event->getPropertyInactive(), event->getSoundID());

  /* Enable */
  if(prop_respawn_mod->isEnabled())
  {
    prop_respawn_dis->setEnabled(respawn);
    prop_respawn_val->setEnabled(respawn);
  }
}

/*
 * Description: Slot which triggers when the respawn value mod spin box is
 *              changed. This new value is the new respawn time in ms
 *
 * Inputs: int value - the respawn value in ms
 * Output: none
 */
void EventView::propertyRespawnVal(int value)
{
  if(value >= 0)
    event->setEventPropMod(event->getPropertyType(), event->getPropertyID(),
                           event->getPropertyMods(), event->getPropertyBools(),
                           value, event->getPropertySpeed(),
                           event->getPropertyTrack(),
                           event->getPropertyInactive(), event->getSoundID());
}

/*
 * Description: Slot which triggers when the speed modifier check box is
 *              changed. Enables the speed to be modified.
 *
 * Inputs: int state - the state of the checkbox
 * Output: none
 */
void EventView::propertySpeedMod(int state)
{
  /* Get the existing */
  bool active, forced, inactive, move, reset, respawn, speed, track, visible;
  EventSet::dataEnumProperties(event->getPropertyMods(), active, forced,
                               inactive, move, reset, respawn, speed,
                               track, visible);

  /* Modify and set */
  speed = (state == Qt::Checked);
  ThingProperty prop_mods = EventSet::createEnumProperties(active, forced,
                        inactive, move, reset, respawn, speed, track, visible);
  event->setEventPropMod(event->getPropertyType(), event->getPropertyID(),
                         prop_mods, event->getPropertyBools(),
                         event->getPropertyRespawn(), event->getPropertySpeed(),
                         event->getPropertyTrack(),
                         event->getPropertyInactive(), event->getSoundID());

  /* Enable */
  if(prop_speed_mod->isEnabled())
  {
    prop_speed_desc->setEnabled(speed);
    prop_speed_val->setEnabled(speed);
    propertySpeedVal(event->getPropertySpeed());
  }
}

/*
 * Description: Slot which triggers when the speed value mod spin box is
 *              changed. This new value is the new speed time in 4096/val =
 *              X tiles/ms
 *
 * Inputs: int value - the speed value
 * Output: none
 */
void EventView::propertySpeedVal(int value)
{
  if(value >= 0)
  {
    event->setEventPropMod(event->getPropertyType(), event->getPropertyID(),
                           event->getPropertyMods(), event->getPropertyBools(),
                           event->getPropertyRespawn(), value,
                           event->getPropertyTrack(),
                           event->getPropertyInactive(), event->getSoundID());

    if(value > 0)
      prop_speed_desc->setText(QString::number(4096 / value) + " ms/tile");
    else
      prop_speed_desc->setText("0 ms/tile");
  }
}

/*
 * Description: Slot which triggers when the selected tracking state to be
 *              modified in the combo box is changed.
 *
 * Inputs: const QString& text - the new tracking text set
 * Output: none
 */
void EventView::propertyTrackChange(const QString& text)
{
  event->setEventPropMod(event->getPropertyType(), event->getPropertyID(),
                         event->getPropertyMods(), event->getPropertyBools(),
                         event->getPropertyRespawn(), event->getPropertySpeed(),
                         Helpers::trackingFromStr(text.toStdString()),
                         event->getPropertyInactive(), event->getSoundID());
}

/*
 * Description: Slot which triggers when the track modifier check box is
 *              changed. Enables the track to be modified.
 *
 * Inputs: int state - the state of the checkbox
 * Output: none
 */
void EventView::propertyTrackMod(int state)
{
  /* Get the existing */
  bool active, forced, inactive, move, reset, respawn, speed, track, visible;
  EventSet::dataEnumProperties(event->getPropertyMods(), active, forced,
                               inactive, move, reset, respawn, speed,
                               track, visible);

  /* Modify and set */
  track = (state == Qt::Checked);
  ThingProperty prop_mods = EventSet::createEnumProperties(active, forced,
                        inactive, move, reset, respawn, speed, track, visible);
  event->setEventPropMod(event->getPropertyType(), event->getPropertyID(),
                         prop_mods, event->getPropertyBools(),
                         event->getPropertyRespawn(), event->getPropertySpeed(),
                         event->getPropertyTrack(),
                         event->getPropertyInactive(), event->getSoundID());

  /* Enable */
  if(prop_track_mod->isEnabled())
  {
    prop_track_opt->setEnabled(track);
  }
}

/*
 * Description: Slot which triggers when the selected thing type to be modified
 *              in the combo box is changed.
 *
 * Inputs: const QString& text - the new thing type text set
 * Output: none
 */
void EventView::propertyTypeChange(const QString& text)
{
  ThingBase new_type = Helpers::typeFromStr(text.toStdString());

  /* Update the event */
  event->setEventPropMod(new_type, event->getPropertyID(),
                         event->getPropertyMods(), event->getPropertyBools(),
                         event->getPropertyRespawn(), event->getPropertySpeed(),
                         event->getPropertyTrack(),
                         event->getPropertyInactive(), event->getSoundID());

  /* Update the ID data */
  updateListProperty(new_type);
  propertyIDChange(prop_id->currentText());

  /* Enable the data accordingly */
  /* -- thing -- */
  prop_active_mod->setEnabled(true);
  prop_active_val->setEnabled(prop_active_mod->isEnabled() &&
                              prop_active_mod->isChecked());
  prop_respawn_mod->setEnabled(true);
  prop_respawn_dis->setEnabled(prop_respawn_mod->isEnabled() &&
                               prop_respawn_mod->isChecked());
  prop_respawn_val->setEnabled(prop_respawn_mod->isEnabled() &&
                               prop_respawn_mod->isChecked());
  prop_visible_mod->setEnabled(true);
  prop_visible_val->setEnabled(prop_visible_mod->isEnabled() &&
                               prop_visible_mod->isChecked());
  /* -- person -- */
  prop_freeze_mod->setEnabled(new_type == ThingBase::PERSON ||
                              new_type == ThingBase::NPC);
  prop_freeze_val->setEnabled(prop_freeze_mod->isEnabled() &&
                              prop_freeze_mod->isChecked());
  prop_reset_mod->setEnabled(new_type == ThingBase::PERSON ||
                             new_type == ThingBase::NPC);
  prop_reset_val->setEnabled(prop_reset_mod->isEnabled() &&
                             prop_reset_mod->isChecked());
  prop_speed_mod->setEnabled(new_type == ThingBase::PERSON ||
                             new_type == ThingBase::NPC);
  prop_speed_desc->setEnabled(prop_speed_mod->isEnabled() &&
                              prop_speed_mod->isChecked());
  prop_speed_val->setEnabled(prop_speed_mod->isEnabled() &&
                             prop_speed_mod->isChecked());
  /* -- npc -- */
  prop_forced_mod->setEnabled(new_type == ThingBase::NPC);
  prop_forced_val->setEnabled(prop_forced_mod->isEnabled() &&
                              prop_forced_mod->isChecked());
  prop_track_mod->setEnabled(new_type == ThingBase::NPC);
  prop_track_opt->setEnabled(prop_track_mod->isEnabled() &&
                             prop_track_mod->isChecked());
  /* -- io -- */
  prop_inactive_mod->setEnabled(new_type == ThingBase::INTERACTIVE);
  prop_inactive_dis->setEnabled(prop_inactive_mod->isEnabled() &&
                                prop_inactive_mod->isChecked());
  prop_inactive_val->setEnabled(prop_inactive_mod->isEnabled() &&
                                prop_inactive_mod->isChecked());
}

/*
 * Description: Slot which triggers when the visible modifier check box is
 *              changed. Enables the visible to be modified.
 *
 * Inputs: int state - the state of the checkbox
 * Output: none
 */
void EventView::propertyVisibleMod(int state)
{
  /* Get the existing */
  bool active, forced, inactive, move, reset, respawn, speed, track, visible;
  EventSet::dataEnumProperties(event->getPropertyMods(), active, forced,
                               inactive, move, reset, respawn, speed,
                               track, visible);

  /* Modify and set */
  visible = (state == Qt::Checked);
  ThingProperty prop_mods = EventSet::createEnumProperties(active, forced,
                        inactive, move, reset, respawn, speed, track, visible);
  event->setEventPropMod(event->getPropertyType(), event->getPropertyID(),
                         prop_mods, event->getPropertyBools(),
                         event->getPropertyRespawn(), event->getPropertySpeed(),
                         event->getPropertyTrack(),
                         event->getPropertyInactive(), event->getSoundID());

  /* Enable */
  if(prop_visible_mod->isEnabled())
  {
    prop_visible_val->setEnabled(visible);
  }
}

/*
 * Description: Slot which triggers when the visible value mod check box is
 *              changed. If checked, the thing visibility will be set
 *
 * Inputs: int state - the state of the checkbox
 * Output: none
 */
void EventView::propertyVisibleVal(int state)
{
  /* Get the existing */
  bool active, forced, inactive, move, reset, respawn, speed, track, visible;
  EventSet::dataEnumProperties(event->getPropertyBools(), active, forced,
                               inactive, move, reset, respawn, speed,
                               track, visible);

  /* Modify and set */
  visible = (state == Qt::Checked);
  ThingProperty prop = EventSet::createEnumProperties(active, forced, inactive,
                                  move, reset, respawn, speed, track, visible);
  event->setEventPropMod(event->getPropertyType(), event->getPropertyID(),
                         event->getPropertyMods(), prop,
                         event->getPropertyRespawn(), event->getPropertySpeed(),
                         event->getPropertyTrack(),
                         event->getPropertyInactive(), event->getSoundID());
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

      editConversation(ref, is_option);
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
 * Description: Triggers the select tile dialog for the separate convo pop-up
 *              of the event view. This hides the event pop-up and passes the
 *              call to the main select tile trigger (selectTileMain).
 *
 * Inputs: none
 * Output: none
 */
void EventView::selectTileConvo()
{
  if(pop_convo != nullptr)
  {
    pop_convo->hide();
    selectTileMain();
    waiting_tile_convo = true;
  }
}

/*
 * Description: Triggers the select tile dialog for the separate event pop-up
 *              of the event view. This hides the event pop-up and passes the
 *              call to the main select tile trigger (selectTileMain).
 *
 * Inputs: none
 * Output: none
 */
void EventView::selectTileEvent()
{
  if(pop_event_edit != nullptr)
  {
    pop_event->hide();
    selectTileMain();
    waiting_tile_event = true;
  }
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
  selectTileMain();
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
  if(index >= 0 && index < tele_thing->count())
  {
    QStringList list = tele_thing->itemText(index).split(":");
    if(list.size() == 2)
      event->setEventTeleport(list.front().toInt(), event->getTeleportSection(),
                              event->getTeleportX(), event->getTeleportY(),
                              event->getSoundID());
  }
}

/*
 * Description: Slot triggered on trigger IO dropdown selection change. Updates
 *              the targetted IO.
 *
 * Inputs: QString text - the selected text in the dialog to parse
 * Output: none
 */
void EventView::triggerIOChanged(const QString & text)
{
  QStringList list = text.split(":");
  if(list.size() == 2)
  {
    event->setEventTriggerIO(list.front().toInt(), event->getSoundID());
  }
}

/*
 * Description: Slot triggered on unlock IO dropdown selection change. Updates
 *              the targetted IO.
 *
 * Inputs: QString text - the selected text in the dialog to parse
 * Output: none
 */
void EventView::unlockIOChanged(const QString & text)
{
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
    if(index >= 0 && index <= list_map_ios.size())
    {
      unio_state->addItem("-1: All States");
      if(index > 0)
      {
        QStringList split_set = list_map_ios[index - 1].second.split(',');
        for(int i = 0; i < split_set.size(); i++)
          unio_state->addItem(split_set[i] + ": State");
      }
    }
  }
}

/*
 * Description: Slot triggered on unlock IO lock mode check box change. Updates
 *              if the main lock is targetted within the IO for the unlock.
 *
 * Inputs: int state - the new state of the check box
 * Output: none
 */
void EventView::unlockIOModeLock(int state)
{
  bool lock, events;
  UnlockIOMode mode = event->getUnlockIOMode();
  EventSet::dataEnumIOMode(mode, lock, events);
  lock = (state == Qt::Checked);
  mode = EventSet::createEnumIOMode(lock, events);
  event->setEventUnlockIO(event->getUnlockIOID(), mode,
                          event->getUnlockIOState(),
                          event->getUnlockIOEventMode(),
                          event->getUnlockViewMode(),
                          event->getUnlockViewTime(), event->getSoundID());
}

/*
 * Description: Slot triggered on unlock IO state check box change. Updates if
 *              the states are targetted within the IO for the unlock.
 *
 * Inputs: int state - the new state of the check box
 * Output: none
 */
void EventView::unlockIOModeStates(int state)
{
  bool lock, events;
  UnlockIOMode mode = event->getUnlockIOMode();
  EventSet::dataEnumIOMode(mode, lock, events);
  events = (state == Qt::Checked);
  mode = EventSet::createEnumIOMode(lock, events);
  event->setEventUnlockIO(event->getUnlockIOID(), mode,
                          event->getUnlockIOState(),
                          event->getUnlockIOEventMode(),
                          event->getUnlockViewMode(),
                          event->getUnlockViewTime(), event->getSoundID());

  /* Enable or disable state dropdown depending on current state */
  unio_state->setEnabled(events);
  unio_event->setEnabled(events);
}

/*
 * Description: Slot triggered when the selected target state drop down
 *              selection is changed. Updates the target state (all or specific
 *              one).
 *
 * Inputs: int index - the index within the state dropdown
 * Output: none
 */
void EventView::unlockIOStateChanged(int index)
{
  int new_state = -1;
  QStringList str_list = unio_state->itemText(index).split(':');
  if(str_list.size() >= 2)
    new_state = str_list.front().toInt();

  /* Set data */
  event->setEventUnlockIO(event->getUnlockIOID(), event->getUnlockIOMode(),
                          new_state, event->getUnlockIOEventMode(),
                          event->getUnlockViewMode(),
                          event->getUnlockViewTime(), event->getSoundID());
}

/*
 * Description: Slot triggered on unlock IO state enter event check box change.
 *              Updates if the enter event of the targetted state is unlocked.
 *
 * Inputs: int state - the new state of the check box
 * Output: none
 */
void EventView::unlockIOStateEnter(int state)
{
  bool enter, exit, use, walkover;
  UnlockIOEvent event_mode = event->getUnlockIOEventMode();
  EventSet::dataEnumIOEvent(event_mode, enter, exit, use, walkover);
  enter = (state == Qt::Checked);
  event_mode = EventSet::createEnumIOEvent(enter, exit, use, walkover);
  event->setEventUnlockIO(event->getUnlockIOID(), event->getUnlockIOMode(),
                          event->getUnlockIOState(), event_mode,
                          event->getUnlockViewMode(),
                          event->getUnlockViewTime(), event->getSoundID());
}

/*
 * Description: Slot triggered on unlock IO state exit event check box change.
 *              Updates if the exit event of the targetted state is unlocked.
 *
 * Inputs: int state - the new state of the check box
 * Output: none
 */
void EventView::unlockIOStateExit(int state)
{
  bool enter, exit, use, walkover;
  UnlockIOEvent event_mode = event->getUnlockIOEventMode();
  EventSet::dataEnumIOEvent(event_mode, enter, exit, use, walkover);
  exit = (state == Qt::Checked);
  event_mode = EventSet::createEnumIOEvent(enter, exit, use, walkover);
  event->setEventUnlockIO(event->getUnlockIOID(), event->getUnlockIOMode(),
                          event->getUnlockIOState(), event_mode,
                          event->getUnlockViewMode(),
                          event->getUnlockViewTime(), event->getSoundID());
}

/*
 * Description: Slot triggered on unlock IO state use event check box change.
 *              Updates if the use event of the targetted state is unlocked.
 *
 * Inputs: int state - the new state of the check box
 * Output: none
 */
void EventView::unlockIOStateUse(int state)
{
  bool enter, exit, use, walkover;
  UnlockIOEvent event_mode = event->getUnlockIOEventMode();
  EventSet::dataEnumIOEvent(event_mode, enter, exit, use, walkover);
  use = (state == Qt::Checked);
  event_mode = EventSet::createEnumIOEvent(enter, exit, use, walkover);
  event->setEventUnlockIO(event->getUnlockIOID(), event->getUnlockIOMode(),
                          event->getUnlockIOState(), event_mode,
                          event->getUnlockViewMode(),
                          event->getUnlockViewTime(), event->getSoundID());
}

/*
 * Description: Slot triggered on unlock IO state walkover event check box
 *              change. Updates if the walkover event of the targetted state
 *              is unlocked.
 *
 * Inputs: int state - the new state of the check box
 * Output: none
 */
void EventView::unlockIOStateWalk(int state)
{
  bool enter, exit, use, walkover;
  UnlockIOEvent event_mode = event->getUnlockIOEventMode();
  EventSet::dataEnumIOEvent(event_mode, enter, exit, use, walkover);
  walkover = (state == Qt::Checked);
  event_mode = EventSet::createEnumIOEvent(enter, exit, use, walkover);
  event->setEventUnlockIO(event->getUnlockIOID(), event->getUnlockIOMode(),
                          event->getUnlockIOState(), event_mode,
                          event->getUnlockViewMode(),
                          event->getUnlockViewTime(), event->getSoundID());
}

/*
 * Description: Slot triggered when the unlock IO view check box is changed.
 *              Updates if the unlocked IO is to be viewed by the player.
 *
 * Inputs: int state - the new state of the check box
 * Output: none
 */
void EventView::unlockIOView(int state)
{
  /* Save the data to the event */
  bool scroll, view;
  UnlockView view_mode = event->getUnlockViewMode();
  EventSet::dataEnumView(view_mode, view, scroll);
  view = (state == Qt::Checked);
  view_mode = EventSet::createEnumView(view, scroll);
  event->setEventUnlockIO(event->getUnlockIOID(), event->getUnlockIOMode(),
                          event->getUnlockIOState(),
                          event->getUnlockIOEventMode(), view_mode,
                          event->getUnlockViewTime(), event->getSoundID());

  /* Proceed to enable/disable widget */
  unio_view_scroll->setEnabled(view);
  unio_view_time->setEnabled(view);
}

/*
 * Description: Slot triggered when the unlock IO view check box is changed.
 *              Updates if the unlocked IO is to be scrolled to as opposed to
 *              fade out/in viewed by the player.
 *
 * Inputs: int state - the new state of the check box
 * Output: none
 */
void EventView::unlockIOViewScroll(int state)
{
  bool scroll, view;
  UnlockView view_mode = event->getUnlockViewMode();
  EventSet::dataEnumView(view_mode, view, scroll);
  scroll = (state == Qt::Checked);
  view_mode = EventSet::createEnumView(view, scroll);
  event->setEventUnlockIO(event->getUnlockIOID(), event->getUnlockIOMode(),
                          event->getUnlockIOState(),
                          event->getUnlockIOEventMode(), view_mode,
                          event->getUnlockViewTime(), event->getSoundID());
}

/*
 * Description: Slot triggered when the unlock IO view time (ms) is changed.
 *              Updates how long the respective IO is viewed when unlocked by
 *              the player.
 *
 * Inputs: int time - the time to view the unlocked object in milliseconds
 * Output: none
 */
void EventView::unlockIOViewTime(int time)
{
  if(time >= 0)
    event->setEventUnlockIO(event->getUnlockIOID(), event->getUnlockIOMode(),
                            event->getUnlockIOState(),
                            event->getUnlockIOEventMode(),
                            event->getUnlockViewMode(),
                            time, event->getSoundID());
}

/*
 * Description: Slot triggered on unlock thing dropdown selection change.
 *              Updates the targetted thing.
 *
 * Inputs: QString text - the selected text in the dialog to parse
 * Output: none
 */
void EventView::unlockThingChanged(const QString & text)
{
  QStringList list = text.split(":");
  if(list.size() == 2)
    event->setEventUnlockThing(list.front().toInt(), event->getUnlockViewMode(),
                               event->getUnlockViewTime(), event->getSoundID());
}

/*
 * Description: Slot triggered when the unlock thing view check box is changed.
 *              Updates if the unlocked thing is to be viewed by the player.
 *
 * Inputs: int state - the new state of the check box
 * Output: none
 */
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

/*
 * Description: Slot triggered when the unlock thing view check box is changed.
 *              Updates if the unlocked thing is to be scrolled to as opposed to
 *              fade out/in viewed by the player.
 *
 * Inputs: int state - the new state of the check box
 * Output: none
 */
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

/*
 * Description: Slot triggered when the unlock thing view time (ms) is changed.
 *              Updates how long the respective thing is viewed when unlocked by
 *              the player.
 *
 * Inputs: int time - the time to view the unlocked object in milliseconds
 * Output: none
 */
void EventView::unlockThingViewTime(int time)
{
  if(time >= 0)
    event->setEventUnlockThing(event->getUnlockThingID(),
                               event->getUnlockViewMode(),
                               time, event->getSoundID());
}

/*
 * Description: Slot triggered on unlock tile enter event check box change.
 *              Updates if the enter event of the tile is to be unlocked.
 *
 * Inputs: int state - the new state of the check box
 * Output: none
 */
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

/*
 * Description: Slot triggered on unlock tile exit event check box change.
 *              Updates if the exit event of the tile is to be unlocked.
 *
 * Inputs: int state - the new state of the check box
 * Output: none
 */
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

/*
 * Description: Slot which triggers when the select tile in unlock tile event is
 *              pressed. Triggers selectTile() which is picked up by the parent
 *              and triggers a hide and then select tile before re-showing.
 *
 * Inputs: none
 * Output: none
 */
void EventView::unlockTilePressed()
{
  selectTileMain();
}

/*
 * Description: Slot triggered when the unlock tile view check box is changed.
 *              Updates if the unlocked tile is to be viewed by the player.
 *
 * Inputs: int state - the new state of the check box
 * Output: none
 */
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

/*
 * Description: Slot triggered when the unlock tile view check box is changed.
 *              Updates if the unlocked tile is to be scrolled to as opposed to
 *              fade out/in viewed by the player.
 *
 * Inputs: int state - the new state of the check box
 * Output: none
 */
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

/*
 * Description: Slot triggered when the unlock tile view time (ms) is changed.
 *              Updates how long the respective tile is viewed when unlocked by
 *              the player.
 *
 * Inputs: int time - the time to view the unlocked object in milliseconds
 * Output: none
 */
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
 * Description: Updates the battle information after an edit. Attempts to
 *              maintain the same index after execution.
 *
 * Inputs: none
 * Output: none
 */
void EventView::updateBattle()
{
  if(event->getEventType() == EventClassifier::BATTLESTART)
  {
    setLayoutData();
  }
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
  if(event->getEventType() == EventClassifier::CONVERSATION)
  {
    QString index = getConvoIndex(convo_tree->currentItem());
    setLayoutData();
    convo_tree->setCurrentItem(getConvo(index));
  }
}

/*
 * Description: Updates the multiple after an edit. Attempts to maintain the
 *              same index after execution.
 *
 * Inputs: none
 * Output: none
 */
void EventView::updateMultiple()
{
  if(event->getEventType() == EventClassifier::MULTIPLE)
  {
    /* Grab the old index and update the data */
    int index = mult_list->currentRow();
    setLayoutData();

    /* Update the index */
    if(mult_list->count() > 0)
    {
      if(mult_list->count() > index)
        mult_list->setCurrentRow(index);
      else
        mult_list->setCurrentRow(mult_list->count() - 1);
    }
  }
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Closes all pop-ups and cleans up edits within the view.
 *
 * Inputs: none
 * Output: none
 */
void EventView::closeAllPopups()
{
  editConversation(nullptr, false);
  editEvent(nullptr);
}

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

/*
 * Description: Returns the list of items, used for event creation.
 *
 * Inputs: none
 * Output: QList<QString> - list of all items (for give item event)
 */
QList<QString> EventView::getListItems()
{
  return list_items;
}

/*
 * Description: Returns the list of maps, used for event creation.
 *
 * Inputs: none
 * Output: QList<QString> - list of all maps (for change map event)
 */
QList<QString> EventView::getListMaps()
{
  return list_maps;
}

/*
 * Description: Returns the list of map IOs, used for event creation.
 *
 * Inputs: none
 * Output: QList<QPair<QString,QString> - list of all IOs (for unlock event)
 */
QList<QPair<QString,QString>> EventView::getListMapIOs()
{
  return list_map_ios;
}

/*
 * Description: Returns the list of map items, used for event creation.
 *
 * Inputs: none
 * Output: QList<QString> - list of all items (for events)
 */
QList<QString> EventView::getListMapItems()
{
  return list_map_items;
}

/*
 * Description: Returns the list of map npcs, used for event creation.
 *
 * Inputs: none
 * Output: QList<QString> - list of all npcs (for events)
 */
QList<QString> EventView::getListMapNPCs()
{
  return list_map_npcs;
}

/*
 * Description: Returns the list of map persons, used for event creation.
 *
 * Inputs: none
 * Output: QList<QString> - list of all persons (for events)
 */
QList<QString> EventView::getListMapPersons()
{
  return list_map_persons;
}

/*
 * Description: Returns the list of map things, used for event creation.
 *
 * Inputs: none
 * Output: QList<QString> - list of all things (for events)
 */
QList<QString> EventView::getListMapThings()
{
  return list_map_things;
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
 * Output: QList<QString> - list of all sub-maps (for teleport event)
 */
QList<QString> EventView::getListSubmaps()
{
  return list_submaps;
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
  if(this->event != nullptr)
    this->event = nullptr;

  /* Set the event */
  this->event = event;

  /* After-math processing */
  setLayoutData();
}

/*
 * Description: Sets the list of items, used for event creation
 *
 * Inputs: QList<QString> - list of all items (for give item event)
 * Output: none
 */
void EventView::setListItems(QList<QString> items)
{
  /* Set new data */
  list_items = items;

  /* Block signals */
  item_name->blockSignals(true);
  take_name->blockSignals(true);

  /* Give Items */
  item_name->clear();
  item_name->addItems(list_items);

  /* Take Items */
  take_name->clear();
  take_name->addItems(list_items);

  /* Update and unblock */
  setLayoutData();
  item_name->blockSignals(false);
  take_name->blockSignals(false);
}

/*
 * Description: Sets the list of maps, used for event creation
 *
 * Inputs: QList<QString> - list of all maps (for change map event)
 * Output: none
 */
void EventView::setListMaps(QList<QString> maps)
{
  list_maps = maps;
  map_name->blockSignals(true);
  map_name->clear();
  map_name->addItems(list_maps);
  setLayoutData();
  map_name->blockSignals(false);
}

/*
 * Description: Sets the list of map IOs, used for event creation
 *
 * Inputs: QList<QPair<QString,QString>> - list of all IOs (for event lists)
 * Output: none
 */
void EventView::setListMapIOs(QList<QPair<QString,QString>> ios)
{
  list_map_ios = ios;
  updateLists(false, true, false, false, false);
}

/*
 * Description: Sets the list of map items, used for event creation.
 *
 * Inputs: QList<QString> - list of all map items (for event lists)
 * Output: none
 */
void EventView::setListMapItems(QList<QString> items)
{
  list_map_items = items;
  updateLists(false, false, true, false, false);
}

/*
 * Description: Sets the list of map npcs, used for event creation.
 *
 * Inputs: QList<QString> - list of all map npcs (for event lists)
 * Output: none
 */
void EventView::setListMapNPCs(QList<QString> npcs)
{
  list_map_npcs = npcs;
  updateLists(false, false, false, false, true);
}

/*
 * Description: Sets the list of map persons, used for event creation.
 *
 * Inputs: QList<QString> - list of all map persons (for event lists)
 * Output: none
 */
void EventView::setListMapPersons(QList<QString> persons)
{
  list_map_persons = persons;
  updateLists(false, false, false, true, false);
}

/*
 * Description: Sets the list of map things, used for event creation.
 *
 * Inputs: QList<QString> - list of all map things (for event lists)
 * Output: none
 */
void EventView::setListMapThings(QList<QString> things)
{
  list_map_things = things;
  updateLists(true, false, false, false, false);
}

/*
 * Description: Sets the list of map things, items, ios, persons, npcs, used
 *              for event creation.
 *
 * Inputs: QList<QString> things - list of all map things
 *         QList<QPair<QString,QString>> ios - list of all map ios
 *         QList<QString> items - list of all map items
 *         QList<QString> persons - list of all map persons
 *         QList<QString npcs - list of all map npcs
 * Output: none
 */
void EventView::setListMapThings(QList<QString> things,
                                 QList<QPair<QString,QString>> ios,
                                 QList<QString> items,
                                 QList<QString> persons,
                                 QList<QString> npcs)
{
  list_map_things = things;
  list_map_ios = ios;
  list_map_items = items;
  list_map_persons = persons;
  list_map_npcs = npcs;
  updateLists(true, true, true, true, true);
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
  combo_sound->addItems(list_sounds);
  setLayoutData();
  combo_sound->blockSignals(false);
}

/*
 * Description: Sets the list of sub-maps, used for event creation.
 *
 * Inputs: QList<QString> - list of all sub-maps (for teleport event)
 * Output: none
 */
void EventView::setListSubmaps(QList<QString> sub_maps)
{
  list_submaps = sub_maps;
  setLayoutData();
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
  if(event != nullptr && waiting_tile)
  {
    waiting_tile = false;

    /* -- EVENT DIALOG -- */
    if(waiting_tile_convo)
    {
      pop_convo->getEventView()->updateSelectedTile(id, x, y);
      pop_convo->show();
    }
    else if(waiting_tile_event)
    {
      pop_event_view->updateSelectedTile(id, x, y);
      pop_event->show();
    }
    /* -- EMBEDDED MAIN EVENT -- */
    else
    {
      if(event->getEventType() == EventClassifier::TELEPORTTHING)
        event->setEventTeleport(event->getTeleportThingID(), id, x, y,
                                event->getSoundID());
      else if(event->getEventType() == EventClassifier::UNLOCKTILE)
        event->setEventUnlockTile(id, x, y, event->getUnlockTileMode(),
                                  event->getUnlockViewMode(),
                                  event->getUnlockViewTime(),
                                  event->getSoundID());
      setLayoutData();
    }
  }
}
