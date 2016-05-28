/*******************************************************************************
 * Class Name: InstanceDialog
 * Date Created: April 1, 2015
 * Inheritance: QDialog
 * Description: The dialog that displays the map thing instant and the
 *              information related to it. Allows manipulation of the data. 
 *              It also contains the path information used for map npcs.
 ******************************************************************************/
#include "Dialog/InstanceDialog.h"
//#include <QDebug>

/* Constant Implementation - see header file for descriptions */
const int InstanceDialog::kALGO_COUNT = 5;
const std::string InstanceDialog::kALGO_STATES[] = {"None", "Path Circular",
                                                    "Path Back and Forth",
                                                    "Random In Range",
                                                    "Random"};
const int InstanceDialog::kMAX_ITEM_COUNT = 2000000000;
const int InstanceDialog::kTRACK_MAX = 100;
const int InstanceDialog::kTRACK_MIN = 1;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function for instant dialog. Takes an
 *              editing IO and a parent widget. Pop-up constructed using
 *              data from the IO.
 *
 * Inputs: EditorMapIO* edit_io - the IO to edit the data for
 *         QWidget* parent - the parent widget
 */
InstanceDialog::InstanceDialog(EditorMapIO* edit_io, QWidget* parent)
           : QDialog(parent)
{
  /* Set-up the IO set - copied to working for changes */
  thing_type = EditorEnumDb::IO;
  thing_original = edit_io;
  thing_working = new EditorMapIO();
  if(thing_original != nullptr)
    *((EditorMapIO*)thing_working) = *((EditorMapIO*)thing_original);

  setup();
}

/*
 * Description: Constructor function for instant dialog. Takes an
 *              editing item and a parent widget. Pop-up constructed using
 *              data from the item.
 *
 * Inputs: EditorMapItem* edit_item - the item to edit the data for
 *         QWidget* parent - the parent widget
 */
InstanceDialog::InstanceDialog(EditorMapItem* edit_item, QWidget* parent)
              : QDialog(parent)
{
  /* Set-up the IO set - copied to working for changes */
  thing_type = EditorEnumDb::ITEM;
  thing_original = edit_item;
  thing_working = new EditorMapItem();
  if(thing_original != nullptr)
    *((EditorMapItem*)thing_working) = *((EditorMapItem*)thing_original);

  setup();
}

/*
 * Description: Constructor function for instant dialog. Takes an
 *              editing npc and a parent widget. Pop-up constructed using
 *              data from the npc.
 *
 * Inputs: EditorMapNPC* edit_npc - the npc to edit the data for
 *         QWidget* parent - the parent widget
 */
InstanceDialog::InstanceDialog(EditorMapNPC* edit_npc, QWidget* parent)
              : QDialog(parent)
{
  /* Set-up the npc set - copied to working for changes */
  thing_type = EditorEnumDb::NPC;
  thing_original = edit_npc;
  thing_working = new EditorMapNPC();
  if(thing_original != nullptr)
    *((EditorMapNPC*)thing_working) = *((EditorMapNPC*)thing_original);

  setup();
}

/*
 * Description: Constructor function for instant dialog. Takes an
 *              editing person and a parent widget. Pop-up constructed using
 *              data from the person.
 *
 * Inputs: EditorMapPerson* edit_person - the person to edit the data for
 *         QWidget* parent - the parent widget
 */
InstanceDialog::InstanceDialog(EditorMapPerson* edit_person, QWidget* parent)
              : QDialog(parent)
{
  /* Set-up the person set - copied to working for changes */
  thing_type = EditorEnumDb::PERSON;
  thing_original = edit_person;
  thing_working = new EditorMapPerson();
  if(thing_original != nullptr)
    *((EditorMapPerson*)thing_working) = *((EditorMapPerson*)thing_original);

  setup();
}

/*
 * Description: Constructor function for instant dialog. Takes an
 *              editing thing and a parent widget. Pop-up constructed using
 *              data from the thing.
 *
 * Inputs: EditorMapThing* edit_thing - the thing to edit the data for
 *         QWidget* parent - the parent widget
 */
InstanceDialog::InstanceDialog(EditorMapThing* edit_thing, QWidget* parent)
           : QDialog(parent)
{
  /* Set-up the thing set - copied to working for changes */
  thing_type = EditorEnumDb::THING;
  thing_original = edit_thing;
  thing_working = new EditorMapThing();
  if(thing_original != nullptr)
    *thing_working = *thing_original;

  setup();
}

/*
 * Description: Destructor function
 */
InstanceDialog::~InstanceDialog()
{
  thing_original = nullptr;

  /* Delete working thing */
  if(thing_working != nullptr)
    delete thing_working;
  thing_working = nullptr;
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: Creates the dialog layout with QT functional widgets.
 *
 * Inputs: bool instance - is the person an instance (has a base person)
 * Output: none
 */
void InstanceDialog::createLayout()
{
  /* Layout setup */
  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  QGridLayout* layout = new QGridLayout(this);
  //layout->setSizeConstraint(QLayout::SetFixedSize);
  layout->setColumnStretch(3, 1);

  /* The ID widget */
  QLabel* lbl_id = new QLabel("ID", this);
  layout->addWidget(lbl_id, 0, 0);
  QLineEdit* line_id = new QLineEdit("", this);
  line_id->setMaximumWidth(75);
  line_id->setText(QString::number(thing_working->getID()));
  line_id->setDisabled(true);
  layout->addWidget(line_id, 0, 1);

  /* The name widget */
  QLabel* lbl_name = new QLabel("Name", this);
  layout->addWidget(lbl_name, 1, 0);
  line_name = new QLineEdit("", this);
  line_name->setMinimumWidth(200);
  connect(line_name, SIGNAL(textEdited(QString)),
          this, SLOT(changedName(QString)));
  layout->addWidget(line_name, 1, 1, 1, 2);

  /* The description widget */
  if(thing_type != EditorEnumDb::ITEM)
  {
    QLabel* lbl_description = new QLabel("Description", this);
    layout->addWidget(lbl_description, 2, 0);
    edit_description = new QTextEdit("", this);
    edit_description->setMaximumHeight(60);
    connect(edit_description, SIGNAL(textChanged()),
            this, SLOT(changedDescription()));
    layout->addWidget(edit_description, 2, 1, 2, 3);
  }
  /* The item count widget - for item things only */
  else
  {
    QLabel* lbl_count = new QLabel("Count", this);
    layout->addWidget(lbl_count, 2, 0);
    spin_count = new QSpinBox(this);
    spin_count->setMinimum(1);
    spin_count->setMaximum(InstanceDialog::kMAX_ITEM_COUNT);
    connect(spin_count, SIGNAL(valueChanged(int)),
            this, SLOT(countChanged(int)));
    layout->addWidget(spin_count, 2, 1, 1, 1);
  }

  /* Active enable widget */
  box_active = new QCheckBox("Spawned on Start", this);
  connect(box_active, SIGNAL(stateChanged(int)),
          this, SLOT(checkActiveChange(int)));
  layout->addWidget(box_active, 4, 1, 1, 2);

  /* Respawn time widget */
  QLabel* lbl_respawn = new QLabel("Respawn Time", this);
  layout->addWidget(lbl_respawn, 5, 0);
  spin_respawn = new QSpinBox(this);
  spin_respawn->setSuffix(" ms");
  spin_respawn->setMinimum(1);
  spin_respawn->setMaximum(1800000);
  spin_respawn->setValue(5000);
  connect(spin_respawn, SIGNAL(valueChanged(int)),
          this, SLOT(respawnChanged(int)));
  layout->addWidget(spin_respawn, 5, 1);
  box_respawn = new QCheckBox("Disabled", this);
  connect(box_respawn, SIGNAL(stateChanged(int)),
          this, SLOT(checkRespawnChange(int)));
  layout->addWidget(box_respawn, 5, 2, 1, 2);

  /* Upper Game Party Connection */
  if(thing_type != EditorEnumDb::ITEM)
  {
    QLabel* lbl_party = new QLabel("Party", this);
    layout->addWidget(lbl_party, 6, 0);
    combo_party = new QComboBox(this);
    combo_party->addItem("None");
    connect(combo_party, SIGNAL(currentIndexChanged(int)),
            this, SLOT(comboPartyChange(int)));
    layout->addWidget(combo_party, 6, 1, 1, 3);
  }

  /* Person/NPC exclusive settings */
  if(thing_type == EditorEnumDb::PERSON || thing_type == EditorEnumDb::NPC)
  {
    /* Speed */
    QLabel* lbl_speed = new QLabel("Speed", this);
    layout->addWidget(lbl_speed, 7, 0);
    spin_speed = new QSpinBox(this);
    spin_speed->setMinimum(0);
    spin_speed->setMaximum(256);
    connect(spin_speed, SIGNAL(valueChanged(int)),
            this, SLOT(speedChanged(int)));
    layout->addWidget(spin_speed, 7, 1);
    lbl_speed_result = new QLabel("X ms/tile");
    layout->addWidget(lbl_speed_result, 7, 2);
    box_base_speed = new QCheckBox("Use Base", this);
    connect(box_base_speed, SIGNAL(stateChanged(int)),
            this, SLOT(checkBaseSpeed(int)));
    layout->addWidget(box_base_speed, 7, 3);

    /* Default direction */
    QLabel* lbl_dir = new QLabel("Direction", this);
    layout->addWidget(lbl_dir, 8, 0);
    combo_dir = new QComboBox(this);
    combo_dir->addItem("North");
    combo_dir->addItem("East");
    combo_dir->addItem("South");
    combo_dir->addItem("West");
    connect(combo_dir, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(comboDirectionChange(QString)));
    layout->addWidget(combo_dir, 8, 1, 1, 2);
  }

  /* Events that are relevant if not IO */
  if(thing_type != EditorEnumDb::IO && thing_type != EditorEnumDb::ITEM)
  {
    /* Use base event widget */
    box_base_event = new QCheckBox("Use Base Event", this);
    connect(box_base_event, SIGNAL(stateChanged(int)),
            this, SLOT(checkBaseChange(int)));
    //if(thing_type == EditorEnumDb::IO)
    //  box_base_event->setDisabled(true);
    layout->addWidget(box_base_event, 9, 1, 1, 2, Qt::AlignBottom);

    /* Event View */
    event_view = new EventSetView(nullptr, this);
    connect(event_view, SIGNAL(editSet(EditorEventSet*)),
            this, SLOT(editEventSet(EditorEventSet*)));
    layout->addWidget(event_view, 10, 0, 8, 4);//,
                      //Qt::AlignHCenter | Qt::AlignBottom);
  }
  /* Events that are relevant if IO - different layout */
  else if(thing_type == EditorEnumDb::IO)
  {
    /* Lock data */
    layout->setRowMinimumHeight(9, 9);
    QLabel* lbl_lock = new QLabel("Lock", this);
    layout->addWidget(lbl_lock, 10, 0);
    box_lock_base = new QCheckBox("Use Base", this);
    connect(box_lock_base, SIGNAL(stateChanged(int)),
            this, SLOT(checkLockBase(int)));
    layout->addWidget(box_lock_base, 10, 1);
    lbl_lock_data = new QLabel("UNCONNECTED", this);
    lbl_lock_data->setStyleSheet("border: 1px solid #a8a8a8");
    layout->addWidget(lbl_lock_data, 11, 1, 1, 2);
    btn_lock = new QPushButton("View", this);
    connect(btn_lock, SIGNAL(clicked()), this, SLOT(buttonLockEdit()));
    layout->addWidget(btn_lock, 11, 3);
    layout->setRowMinimumHeight(12, 9);

    /* Overall state data */
    QLabel* lbl_states = new QLabel("States", this);
    layout->addWidget(lbl_states, 13, 0);
    combo_states = new QComboBox(this);
    connect(combo_states, SIGNAL(currentIndexChanged(int)),
            this, SLOT(comboStateChange(int)));
    layout->addWidget(combo_states, 13, 1, 1, 3);

    /* State enter */
    QLabel* lbl_states_enter = new QLabel("Enter", this);
    layout->addWidget(lbl_states_enter, 14, 1, Qt::AlignRight);
    btn_states_enter = new QPushButton("View", this);
    connect(btn_states_enter, SIGNAL(clicked()), this, SLOT(buttonStateEnter()));
    layout->addWidget(btn_states_enter, 14, 2);
    box_states_enter = new QCheckBox("Use Base", this);
    connect(box_states_enter, SIGNAL(stateChanged(int)),
            this, SLOT(checkStateEnter(int)));
    layout->addWidget(box_states_enter, 14, 3);

    /* State exit */
    QLabel* lbl_states_exit = new QLabel("Exit", this);
    layout->addWidget(lbl_states_exit, 15, 1, Qt::AlignRight);
    btn_states_exit = new QPushButton("View", this);
    connect(btn_states_exit, SIGNAL(clicked()), this, SLOT(buttonStateExit()));
    layout->addWidget(btn_states_exit, 15, 2);
    box_states_exit = new QCheckBox("Use Base", this);
    connect(box_states_exit, SIGNAL(stateChanged(int)),
            this, SLOT(checkStateExit(int)));
    layout->addWidget(box_states_exit, 15, 3);

    /* State use */
    QLabel* lbl_states_use = new QLabel("Use", this);
    layout->addWidget(lbl_states_use, 16, 1, Qt::AlignRight);
    btn_states_use = new QPushButton("View", this);
    connect(btn_states_use, SIGNAL(clicked()), this, SLOT(buttonStateUse()));
    layout->addWidget(btn_states_use, 16, 2);
    box_states_use = new QCheckBox("Use Base", this);
    connect(box_states_use, SIGNAL(stateChanged(int)),
            this, SLOT(checkStateUse(int)));
    layout->addWidget(box_states_use, 16, 3);

    /* State walkover */
    QLabel* lbl_states_walk = new QLabel("Walkover", this);
    layout->addWidget(lbl_states_walk, 17, 1, Qt::AlignRight);
    btn_states_walk = new QPushButton("View", this);
    connect(btn_states_walk, SIGNAL(clicked()), this, SLOT(buttonStateWalk()));
    layout->addWidget(btn_states_walk, 17, 2);
    box_states_walk = new QCheckBox("Use Base", this);
    connect(box_states_walk, SIGNAL(stateChanged(int)),
            this, SLOT(checkStateWalk(int)));
    layout->addWidget(box_states_walk, 17, 3);
  }

  /* Movement section only relevant if npc */
  int btn_offset = 0;
  if(thing_type == EditorEnumDb::NPC)
  {
    /* Title of movement section */
    QLabel* lbl_movement = new QLabel("Movement", this);
    QFont font = lbl_movement->font();
    font.setUnderline(true);
    lbl_movement->setFont(font);
    layout->addWidget(lbl_movement, 0, 5);
    layout->setColumnMinimumWidth(4, 25);

    /* Algorithm */
    QLabel* lbl_algorithm = new QLabel("Algorithm", this);
    layout->addWidget(lbl_algorithm, 1, 5);
    combo_algorithm = new QComboBox(this);
    for(int i = 0; i < kALGO_COUNT; i++)
      combo_algorithm->addItem(QString::fromStdString(kALGO_STATES[i]));
    connect(combo_algorithm, SIGNAL(currentIndexChanged(int)),
            this, SLOT(comboAlgorithmChange(int)));
    layout->addWidget(combo_algorithm, 1, 6, 1, 3);

    /* Tracking */
    QLabel* lbl_tracking = new QLabel("Tracking", this);
    layout->addWidget(lbl_tracking, 2, 5);
    combo_tracking = new QComboBox(this);
    for(int i = 0; i <= static_cast<int>(TrackingState::TOPLAYER); i++)
      combo_tracking->addItem(QString::fromStdString(
                        Helpers::trackingToStr(static_cast<TrackingState>(i))));
    connect(combo_tracking, SIGNAL(currentIndexChanged(int)),
            this, SLOT(comboTrackingChange(int)));
    layout->addWidget(combo_tracking, 2, 6, 1, 3);

    /* Force interaction */
    box_interaction = new QCheckBox("Force Interaction", this);
    connect(box_interaction, SIGNAL(stateChanged(int)),
            this, SLOT(checkInteractionChange(int)));
    layout->addWidget(box_interaction, 3, 6, 1, 2);

    /* Tracking setpoints */
    QLabel* lbl_track_start = new QLabel("Track Start", this);
    layout->addWidget(lbl_track_start, 4, 5);
    spin_track_start = new QSpinBox(this);
    spin_track_start->setSuffix(" tiles");
    spin_track_start->setMinimum(kTRACK_MIN);
    spin_track_start->setMaximum(kTRACK_MAX);
    spin_track_start->setDisabled(true);
    connect(spin_track_start, SIGNAL(valueChanged(int)),
            this, SLOT(trackStartChanged(int)));
    layout->addWidget(spin_track_start, 4, 6, 1, 2);
    QLabel* lbl_track_release = new QLabel("Track Stop", this);
    layout->addWidget(lbl_track_release, 5, 5);
    spin_track_release = new QSpinBox(this);
    spin_track_release->setSuffix(" tiles");
    spin_track_release->setMinimum(kTRACK_MIN + 2);
    spin_track_release->setMaximum(kTRACK_MAX + 2);
    spin_track_release->setDisabled(true);
    connect(spin_track_release, SIGNAL(valueChanged(int)),
            this, SLOT(trackReleaseChanged(int)));
    layout->addWidget(spin_track_release, 5, 6, 1, 2);
    QLabel* lbl_track_maintain = new QLabel("Track Maintain", this);
    layout->addWidget(lbl_track_maintain, 6, 5);
    spin_track_maintain = new QSpinBox(this);
    spin_track_maintain->setSuffix(" tiles");
    spin_track_maintain->setMinimum(kTRACK_MIN + 1);
    spin_track_maintain->setMaximum(kTRACK_MAX + 1);
    spin_track_maintain->setDisabled(true);
    connect(spin_track_maintain, SIGNAL(valueChanged(int)),
            this, SLOT(trackMaintainChanged(int)));
    layout->addWidget(spin_track_maintain, 6, 6, 1, 2);

    /* Node control */
    QLabel* lbl_nodes = new QLabel("Nodes", this);
    layout->addWidget(lbl_nodes, 7, 5);
    int icon_size = 24;
    int button_size = icon_size + 6;
    btn_edit_nodes = new QPushButton(this);
    btn_edit_nodes->setIcon(QIcon(":/images/icons/32_settings.png"));
    btn_edit_nodes->setIconSize(QSize(icon_size,icon_size));
    btn_edit_nodes->setMaximumSize(button_size, button_size);
    connect(btn_edit_nodes, SIGNAL(clicked()), this, SLOT(buttonEditNodes()));
    layout->addWidget(btn_edit_nodes, 7, 8, Qt::AlignRight);

    /* Node list */
    list_nodes = new QListWidget(this);
    connect(list_nodes, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(editNode(QListWidgetItem*)));
    layout->addWidget(list_nodes, 8, 5, 10, 4);

    btn_offset = 5;
  }

  /* The button control */
  layout->setRowMinimumHeight(18, 15);
  QPushButton* btn_base_edit = new QPushButton("Edit Base", this);
  btn_base_edit->setMaximumWidth(75);
  connect(btn_base_edit, SIGNAL(clicked()), this, SLOT(buttonBaseEdit()));
  layout->addWidget(btn_base_edit, 19, 0);
  QPushButton* btn_ok = new QPushButton("Ok", this);
  btn_ok->setMaximumWidth(75);
  btn_ok->setDefault(true);
  connect(btn_ok, SIGNAL(clicked()), this, SLOT(buttonOk()));
  layout->addWidget(btn_ok, 19, 2 + btn_offset);
  QPushButton* btn_cancel = new QPushButton("Cancel", this);
  btn_cancel->setMaximumWidth(75);
  connect(btn_cancel, SIGNAL(clicked()), this, SLOT(buttonCancel()));
  layout->addWidget(btn_cancel, 19, 3 + btn_offset);

  /* Dialog control */
  if(thing_type == EditorEnumDb::THING)
    setWindowTitle("Thing Instance Edit");
  else if(thing_type == EditorEnumDb::PERSON)
    setWindowTitle("Person Instance Edit");
  else if(thing_type == EditorEnumDb::NPC)
    setWindowTitle("NPC Instance Edit");
  else if(thing_type == EditorEnumDb::IO)
    setWindowTitle("IO Instance Edit");
  else if(thing_type == EditorEnumDb::ITEM)
    setWindowTitle("Item Instance Edit");

  /* Finally resize and lock size at minimum */
  updateGeometry();
  setFixedSize(minimumSizeHint());

  /* Pop-up construction for locks */
  pop_lock = new QDialog(this);
  pop_lock->setWindowTitle("Lock Edit");
  QGridLayout* l_layout = new QGridLayout(pop_lock);
  lock_ctrl = new EditorLock();
  lock_view = new LockView(nullptr, pop_lock);
  l_layout->addWidget(lock_view, 0, 0, 1, 4);
  QPushButton* btn_lock_ok = new QPushButton("Ok", pop_lock);
  connect(btn_lock_ok, SIGNAL(clicked()), this, SLOT(buttonLockOk()));
  l_layout->addWidget(btn_lock_ok, 1, 2);
  QPushButton* btn_lock_cancel = new QPushButton("Cancel", pop_lock);
  connect(btn_lock_cancel, SIGNAL(clicked()), this, SLOT(buttonLockCancel()));
  l_layout->addWidget(btn_lock_cancel);
  pop_lock->hide();
}

/*
 * Description: Called on initial construction to set-up the various required
 *              functionality of the class.
 *
 * Inputs: none
 * Output: none
 */
void InstanceDialog::setup()
{
  /* Variable clean-up */
  event_dialog = nullptr;
  waiting_for_submap = false;
  waiting_path = false;

  /* Layout setup */
  createLayout();
  updateData();
}

/*
 * Description: Updates the data in the widgets. setup() must be called
 *              prior. Assumes signals have been blocked as needed.
 *
 * Inputs: none
 * Output: none
 */
void InstanceDialog::updateData()
{
  /* Name */
  line_name->setText(thing_working->getName());

  /* Active and respawn status */
  box_active->setChecked(thing_working->isActive());
  box_respawn->setChecked(thing_working->getActiveRespawn() < 0);
  if(thing_working->getActiveRespawn() >= 0)
    spin_respawn->setValue(thing_working->getActiveRespawn());

  /* Non-Item specific settings */
  if(thing_type != EditorEnumDb::ITEM)
  {
    /* Description */
    edit_description->setPlainText(thing_working->getDescription());

    /* Game ID Party */
    combo_party->setCurrentIndex(0);
    for(int i = 0; i < list_parties.size(); i++)
    {
      int id = list_parties[i].split(":").front().toInt();
      if(id == thing_working->getGameID())
        combo_party->setCurrentIndex(i + 1);
    }
  }
  /* Item specific settings */
  else
  {
    spin_count->setValue(
                     static_cast<EditorMapItem*>(thing_working)->getCount());
  }

  /* Person/NPC specific settings */
  if(thing_type == EditorEnumDb::PERSON || thing_type == EditorEnumDb::NPC)
  {
    EditorMapPerson* ref = (EditorMapPerson*)thing_working;
    /* Speed */
    spin_speed->setValue(ref->getSpeed());
    box_base_speed->setChecked(ref->isBaseSpeed());

    /* Starting direction */
    if(ref->getStartingDirection() == Direction::NORTH)
      combo_dir->setCurrentIndex(0);
    else if(ref->getStartingDirection() == Direction::EAST)
      combo_dir->setCurrentIndex(1);
    else if(ref->getStartingDirection() == Direction::SOUTH)
      combo_dir->setCurrentIndex(2);
    else if(ref->getStartingDirection() == Direction::WEST)
      combo_dir->setCurrentIndex(3);
  }

  /* Event control */
  if(thing_type != EditorEnumDb::IO && thing_type != EditorEnumDb::ITEM)
  {
    box_base_event->blockSignals(true);
    box_base_event->setChecked(thing_working->isBaseEvent());
    event_view->setEventSet(thing_working->getEventSet());
    event_view->setEnabled(!thing_working->isBaseEvent());
    box_base_event->blockSignals(false);
  }
  else if(thing_type == EditorEnumDb::IO)
  {
    EditorMapIO* io = (EditorMapIO*)thing_working;

    /* Lock data */
    box_lock_base->setChecked(io->isBaseLock());
    lbl_lock_data->setText(io->getLock()->getTextSummary(""));
    if(io->isBaseLock())
      checkLockBase(Qt::Checked);
    else
      checkLockBase(Qt::Unchecked);

    /* State data */
    combo_states->blockSignals(true);
    combo_states->clear();
    for(int i = 0; i < io->getStates().size(); i++)
      combo_states->addItem(io->getStateName(i));
    combo_states->blockSignals(false);

    /* Individual state event update */
    comboStateChange(combo_states->currentIndex());
  }

  /* If NPC, update movement data */
  if(thing_type == EditorEnumDb::NPC)
  {
    EditorMapNPC* npc = (EditorMapNPC*)thing_working;
    EditorNPCPath* path = npc->getPath();

    /* Combo box algorithm */
    combo_algorithm->blockSignals(true);
    if(path->getState() == MapNPC::LOCKED)
      combo_algorithm->setCurrentIndex(0);
    else if(path->getState() == MapNPC::LOOPED)
      combo_algorithm->setCurrentIndex(1);
    else if(path->getState() == MapNPC::BACKANDFORTH)
      combo_algorithm->setCurrentIndex(2);
    else if(path->getState() == MapNPC::RANDOMRANGE)
      combo_algorithm->setCurrentIndex(3);
    else if(path->getState() == MapNPC::RANDOM)
      combo_algorithm->setCurrentIndex(4);
    combo_algorithm->blockSignals(false);

    /* Combo box tracking */
    combo_tracking->blockSignals(true);
    combo_tracking->setCurrentText(QString::fromStdString(
                                 Helpers::trackingToStr(path->getTracking())));
    combo_tracking->blockSignals(false);

    /* Tracking setpoints */
    spin_track_maintain->blockSignals(true);
    spin_track_release->blockSignals(true);
    spin_track_start->blockSignals(true);
    spin_track_maintain->setValue(path->getTrackDistRun());
    spin_track_release->setValue(path->getTrackDistMax());
    spin_track_start->setValue(path->getTrackDistMin());
    spin_track_maintain->blockSignals(false);
    spin_track_release->blockSignals(false);
    spin_track_start->blockSignals(false);

    /* Forced interaction */
    box_interaction->setChecked(path->isForcedInteraction());

    /* Nodes and updates */
    updateNodes();
    comboAlgorithmChange(combo_algorithm->currentIndex());
    comboTrackingChange(combo_tracking->currentIndex());
  }
}

/*
 * Description: Updates the list of nodes in the view.
 *
 * Inputs: none
 * Output: none
 */
void InstanceDialog::updateNodes()
{
  /* Clear the existing list */
  list_nodes->clear();

  /* Fill the new list */
  EditorMapNPC* npc = (EditorMapNPC*)thing_working;
  for(int i = -1; i < npc->getPath()->getNodes().size(); i++)
    list_nodes->addItem(npc->getPath()->getNodeStr(i));
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: The re-implementation of the close event for the dialog. Cleans
 *              up the existing person references and deletes memory.
 *
 * Inputs: QCloseEvent* event - the close event (accepted after clean-up)
 * Output: none
 */
void InstanceDialog::closeEvent(QCloseEvent* event)
{
  editEventSet(nullptr);
  thing_original = nullptr;

  event->accept();
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

/*
 * Description: Button slot on the edit base button. Opens up the edit dialog
 *              and locks the instance dialog until that edit is complete.
 *
 * Inputs: none
 * Output: none
 */
void InstanceDialog::buttonBaseEdit()
{
  if(thing_original != nullptr)
  {
    /* Create pop-up to decide what to do with edit */
    QMessageBox msg_box;
    msg_box.setWindowTitle("Save?");
    msg_box.setText("Save instance before editing base?");
    msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No |
                               QMessageBox::Cancel);
    int status = msg_box.exec();
    if(status == QMessageBox::Yes)
    {
      EditorMapThing* base_thing = thing_original->getBaseThing();
      buttonOk();
      emit editBase(base_thing);
    }
    else if(status == QMessageBox::No)
    {
      EditorMapThing* base_thing = thing_original->getBaseThing();
      buttonCancel();
      emit editBase(base_thing);
    }
  }
}

/*
 * Description: Button slot on the cancel button. Just closes the dialog and
 *              doesn't save the changes.
 *
 * Inputs: none
 * Output: none
 */
void InstanceDialog::buttonCancel()
{
  close();
}

/*
 * Description: Button slot on the edit nodes button. Hides the dialog while
 *              the user edits the node on the map. Once completed, proceeds
 *              to update the class information.
 *
 * Inputs: none
 * Output: none
 */
void InstanceDialog::buttonEditNodes()
{
  if(thing_type == EditorEnumDb::NPC && !waiting_path)
  {
    emit pathEditStart(((EditorMapNPC*)thing_working)->getPath());
    waiting_path = true;
    hide();
  }
}

/*
 * Description: Button slot on lock dialog cancel button. Hides the dialog and
 *              cancels all changes.
 *
 * Inputs: none
 * Output: none
 */
void InstanceDialog::buttonLockCancel()
{
  if(thing_type == EditorEnumDb::IO)
  {
    pop_lock->hide();
    lock_view->setLock(nullptr);
  }
}

/*
 * Description: Button slot on lock edit within IO version of instance. Shows
 *              the lock dialog with the lock information from the present IO
 *
 * Inputs: none
 * Output: none
 */
void InstanceDialog::buttonLockEdit()
{
  if(thing_type == EditorEnumDb::IO)
  {
    EditorMapIO* io = (EditorMapIO*)thing_working;

    /* Close event set dialog (and correspondingly the lock dialog as well) */
    editEventSet(nullptr);

    /* Set the new information for the lock and open */
    *lock_ctrl = *io->getLock();
    lock_view->setLock(lock_ctrl);
    lock_view->setDisabled(io->isBaseLock());
    pop_lock->show();
  }
}

/*
 * Description: Button slot on lock dialog ok button. Hides the dialog but
 *              saves changes made to the lock to the current working IO.
 *
 * Inputs: none
 * Output: none
 */
void InstanceDialog::buttonLockOk()
{
  if(thing_type == EditorEnumDb::IO)
  {
    EditorMapIO* io = (EditorMapIO*)thing_working;
    if(!io->isBaseLock())
    {
      io->setLock(*lock_ctrl);
      checkLockBase(Qt::Unchecked);
    }
    buttonLockCancel();
  }
}

/*
 * Description: Button slot on the ok button. Emits ok prior to closing the
 *              dialog. The ok gets handled by the widget parent, which
 *              updates the visible persons on map prior to accepting changes.
 *
 * Inputs: none
 * Output: none
 */
void InstanceDialog::buttonOk()
{
  /* Proceed to ok() */
  updateOriginal();
  emit ok(thing_original->getNameList());
  close();
}

/*
 * Description: Button slot on the enter event edit/view button for the given
 *              IO state. If the state is an instance, this trigger will do
 *              nothing.
 *
 * Inputs: none
 * Output: none
 */
void InstanceDialog::buttonStateEnter()
{
  if(thing_type == EditorEnumDb::IO)
  {
    /* Acquire the state */
    EditorMapIO* io = (EditorMapIO*)thing_working;
    EditorState* state_base = io->getState(combo_states->currentIndex());
    EditorState* state_inst = io->getState(combo_states->currentIndex(), true);

    /* If state is valid, proceed to open pop-up */
    if(state_base != nullptr && state_inst != nullptr &&
       state_base->type == EditorEnumDb::IO_STATE)
    {
      if(state_inst->base_enter)
        editEventSet(&state_base->set_enter, "Enter Event (Base) View", true);
      else
        editEventSet(&state_inst->set_enter, "Enter Event Edit");
    }
  }
}

/*
 * Description: Button slot on the exit event edit/view button for the given
 *              IO state. If the state is an instance, this trigger will do
 *              nothing.
 *
 * Inputs: none
 * Output: none
 */
void InstanceDialog::buttonStateExit()
{
  if(thing_type == EditorEnumDb::IO)
  {
    /* Acquire the state */
    EditorMapIO* io = (EditorMapIO*)thing_working;
    EditorState* state_base = io->getState(combo_states->currentIndex());
    EditorState* state_inst = io->getState(combo_states->currentIndex(), true);

    /* If state is valid, proceed to open pop-up */
    if(state_base != nullptr && state_inst != nullptr &&
       state_base->type == EditorEnumDb::IO_STATE)
    {
      if(state_inst->base_exit)
        editEventSet(&state_base->set_exit, "Exit Event (Base) View", true);
      else
        editEventSet(&state_inst->set_exit, "Exit Event Edit");
    }
  }
}

/*
 * Description: Button slot on the use event edit/view button for the given
 *              IO state. If the state is an instance, this trigger will do
 *              nothing.
 *
 * Inputs: none
 * Output: none
 */
void InstanceDialog::buttonStateUse()
{
  if(thing_type == EditorEnumDb::IO)
  {
    /* Acquire the state */
    EditorMapIO* io = (EditorMapIO*)thing_working;
    EditorState* state_base = io->getState(combo_states->currentIndex());
    EditorState* state_inst = io->getState(combo_states->currentIndex(), true);

    /* If state is valid, proceed to open pop-up */
    if(state_base != nullptr && state_inst != nullptr &&
       state_base->type == EditorEnumDb::IO_STATE)
    {
      if(state_inst->base_use)
        editEventSet(&state_base->set_use, "Use Event (Base) View", true);
      else
        editEventSet(&state_inst->set_use, "Use Event Edit");
    }
  }
}

/*
 * Description: Button slot on the walkover event edit/view button for the given
 *              IO state. If the state is an instance, this trigger will do
 *              nothing.
 *
 * Inputs: none
 * Output: none
 */
void InstanceDialog::buttonStateWalk()
{
  if(thing_type == EditorEnumDb::IO)
  {
    /* Acquire the state */
    EditorMapIO* io = (EditorMapIO*)thing_working;
    EditorState* state_base = io->getState(combo_states->currentIndex());
    EditorState* state_inst = io->getState(combo_states->currentIndex(), true);

    /* If state is valid, proceed to open pop-up */
    if(state_base != nullptr && state_inst != nullptr &&
       state_base->type == EditorEnumDb::IO_STATE)
    {
      if(state_inst->base_walkover)
        editEventSet(&state_base->set_walkover,
                     "Walkover Event (Base) View", true);
      else
        editEventSet(&state_inst->set_walkover, "Walkover Event Edit");
    }
  }
}

/*
 * Description: Slot triggered on the text edit field being changed, which
 *              updates the description.
 *
 * Inputs: QString description - the new description
 * Output: none
 */
void InstanceDialog::changedDescription()
{
  thing_working->setDescription(edit_description->toPlainText());
}

/*
 * Description: Slot triggered on the text edit field being changed, which
 *              updates the name.
 *
 * Inputs: QString name - the new person name
 * Output: none
 */
void InstanceDialog::changedName(QString name)
{
  thing_working->setName(name);
}

/*
 * Description: Slot triggered when the state of the check box for the make
 *              thing active changes. This is used to make the thing active on
 *              the map on start-up of the game.
 *
 * Inputs: int state - the new state of the check box
 * Output: none
 */
void InstanceDialog::checkActiveChange(int state)
{
  thing_working->setActive(state == Qt::Checked);
}

/*
 * Description: Slot triggered when the state of the check box for the "use
 *              base" event changes. This is used to use a different event for
 *              the thing instance, as opposed to the base event.
 *
 * Inputs: int state - the new state of the check box
 * Output: none
 */
void InstanceDialog::checkBaseChange(int state)
{
  /* If base is checked, event is disabled. Otherwise, enabled */
  thing_working->setUseBaseEvent(state == Qt::Checked);

  /* Set the event */
  if(state == Qt::Unchecked)
    thing_working->setEventSet(*thing_working->getBaseThing()->getEventSet());

  /* Update the view */
  event_view->setEventSet(thing_working->getEventSet());
  event_view->setEnabled(state == Qt::Unchecked);
}

/*
 * Description: Slot triggered when the check box to use the base speed changes
 *              state. Updates the enabled status of the spinner plus spinner
 *              value.
 *
 * Inputs: int state - the state of the checkbox
 * Output: none
 */
void InstanceDialog::checkBaseSpeed(int state)
{
  /* Disable/enable the speed if base is chosen */
  spin_speed->setDisabled(state == Qt::Checked);
  ((EditorMapPerson*)thing_working)->setUseBaseSpeed(state == Qt::Checked);

  /* Update the value of the spin speed box */
  spin_speed->setValue(((EditorMapPerson*)thing_working)->getSpeed());
}

/*
 * Description: Slot triggered when the state of the check box for the "force
 *              instance" event changes. This is used to force the npc to
 *              interact with a person, if within vicinity.
 *
 * Inputs: int state - the new state of the check box
 * Output: none
 */
void InstanceDialog::checkInteractionChange(int state)
{
  if(thing_type == EditorEnumDb::NPC)
  {
    EditorMapNPC* npc = (EditorMapNPC*)thing_working;
    npc->getPath()->setForcedInteraction(state == Qt::Checked);
  }
}

/*
 * Description: Slot triggered when the check box for the IO use base for lock
 *              changes state. Updates if the lock uses the base IO or the
 *              instant IO.
 *
 * Inputs: int state - the new state of the check box
 * Output: none
 */
void InstanceDialog::checkLockBase(int state)
{
  if(thing_type == EditorEnumDb::IO)
  {
    /* Change the use base status */
    EditorMapIO* io = (EditorMapIO*)thing_working;
    io->setUseBaseLock(state == Qt::Checked);

    /* Font */
    QFont bold = btn_lock->font();
    bold.setBold(true);
    QFont not_bold = bold;
    not_bold.setBold(false);

    /* Update the lock data and view */
    lbl_lock_data->setText(io->getLock()->getTextSummary(""));
    if(state == Qt::Checked)
      btn_lock->setText("View");
    else
      btn_lock->setText("Edit");
    if(io->getLock()->getLockType() == LockedState::NONE)
      btn_lock->setFont(not_bold);
    else
      btn_lock->setFont(bold);
  }
}

/*
 * Description: Slot triggered when the check box for the respawn enabled or
 *              disabled is changed. Updates if the respawn time should be used
 *              or if inactive it never appears again.
 *
 * Inputs: int state - the new state of the check box
 * Output: none
 */
void InstanceDialog::checkRespawnChange(int state)
{
  bool respawn_disabled = (state == Qt::Checked);
  spin_respawn->setDisabled(respawn_disabled);
  if(respawn_disabled)
    thing_working->setActiveRespawn(-1);
  else
    thing_working->setActiveRespawn(spin_respawn->value());
}

/*
 * Description: Slot triggered when the check box for the IO use base for state
 *              enter event. Updates if the lock uses the base IO or the
 *              instant IO to access the event.
 *
 * Inputs: int state - the new state of the check box
 * Output: none
 */
void InstanceDialog::checkStateEnter(int state)
{
  if(thing_type == EditorEnumDb::IO)
  {
    /* Change the use base status */
    EditorMapIO* io = (EditorMapIO*)thing_working;
    io->setUseBaseEnter(combo_states->currentIndex(), state == Qt::Checked);
    comboStateChange(combo_states->currentIndex());
  }
}

/*
 * Description: Slot triggered when the check box for the IO use base for state
 *              exit event. Updates if the lock uses the base IO or the
 *              instant IO to access the event.
 *
 * Inputs: int state - the new state of the check box
 * Output: none
 */
void InstanceDialog::checkStateExit(int state)
{
  if(thing_type == EditorEnumDb::IO)
  {
    /* Change the use base status */
    EditorMapIO* io = (EditorMapIO*)thing_working;
    io->setUseBaseExit(combo_states->currentIndex(), state == Qt::Checked);
    comboStateChange(combo_states->currentIndex());
  }
}

/*
 * Description: Slot triggered when the check box for the IO use base for state
 *              use event. Updates if the lock uses the base IO or the
 *              instant IO to access the event.
 *
 * Inputs: int state - the new state of the check box
 * Output: none
 */
void InstanceDialog::checkStateUse(int state)
{
  if(thing_type == EditorEnumDb::IO)
  {
    /* Change the use base status */
    EditorMapIO* io = (EditorMapIO*)thing_working;
    io->setUseBaseUse(combo_states->currentIndex(), state == Qt::Checked);
    comboStateChange(combo_states->currentIndex());
  }
}

/*
 * Description: Slot triggered when the check box for the IO use base for state
 *              walkover event. Updates if the lock uses the base IO or the
 *              instant IO to access the event.
 *
 * Inputs: int state - the new state of the check box
 * Output: none
 */
void InstanceDialog::checkStateWalk(int state)
{
  if(thing_type == EditorEnumDb::IO)
  {
    /* Change the use base status */
    EditorMapIO* io = (EditorMapIO*)thing_working;
    io->setUseBaseWalkover(combo_states->currentIndex(), state == Qt::Checked);
    comboStateChange(combo_states->currentIndex());
  }
}

/*
 * Description: Slot triggered when the combo box for the algorithm type of
 *              the movement of the npc is changed. This changes the modes of
 *              the paths and how they are handled.
 *
 * Inputs: int index - the new index in the combo box
 * Output: none
 */
void InstanceDialog::comboAlgorithmChange(int index)
{
  if(thing_type == EditorEnumDb::NPC)
  {
    EditorMapNPC* npc = (EditorMapNPC*)thing_working;

    /* Update state */
    if(index == 0)
      npc->getPath()->setState(MapNPC::LOCKED);
    else if(index == 1)
      npc->getPath()->setState(MapNPC::LOOPED);
    else if(index == 2)
      npc->getPath()->setState(MapNPC::BACKANDFORTH);
    else if(index == 3)
      npc->getPath()->setState(MapNPC::RANDOMRANGE);
    else if(index == 4)
      npc->getPath()->setState(MapNPC::RANDOM);

    /* Disable edit button if relevant */
    if(npc->getPath()->getState() == MapNPC::LOCKED ||
       npc->getPath()->getState() == MapNPC::RANDOM)
      btn_edit_nodes->setEnabled(false);
    else
      btn_edit_nodes->setEnabled(true);

    /* Disable tracking for locked and update child widgets */
    comboTrackingChange(combo_tracking->currentIndex());
    combo_tracking->setDisabled(npc->getPath()->getState() == MapNPC::LOCKED);

    /* Update nodes */
    updateNodes();
  }
}

/*
 * Description: Slot triggered when the combo box for the initial facing
 *              direction of the person/npc is changed. This changes it within
 *              the class.
 *
 * Inputs: QString item - the new selected item in the combo box
 * Output: none
 */
void InstanceDialog::comboDirectionChange(QString item)
{
  if(thing_type == EditorEnumDb::PERSON || thing_type == EditorEnumDb::NPC)
  {
    EditorMapPerson* ref = (EditorMapPerson*)thing_working;
    Direction dir = Helpers::directionFromString(item.toStdString());
    ref->setStartingDirection(dir);
  }
}

/*
 * Description: Slot triggered when the combo box for the selected party of
 *              the thing is changed. This changes the party index.
 *
 * Inputs: int index - the new index in the combo box
 * Output: none
 */
void InstanceDialog::comboPartyChange(int index)
{
  if(index > 0)
    thing_working->setGameID(combo_party->currentText().split(":")
                                                       .front().toInt());
  else
    thing_working->setGameID(-1);
}

/*
 * Description: Slot triggered when the combo box for the selected state of
 *              the IO is changed. This changes what events are viewed/editable.
 *
 * Inputs: int index - the new index in the combo box
 * Output: none
 */
void InstanceDialog::comboStateChange(int index)
{
  if(thing_type == EditorEnumDb::IO)
  {
    EditorMapIO* io = (EditorMapIO*)thing_working;

    /* Close event set dialog, if open */
    editEventSet(nullptr);

    /* Fonts */
    QFont bold = btn_states_enter->font();
    bold.setBold(true);
    QFont not_bold = bold;
    not_bold.setBold(false);

    /* Proceed to access state to update - only if IO_STATE */
    EditorState* state = io->getState(index, true);
    EditorState* state_base = io->getState(index);
    if(state != nullptr && state_base != nullptr &&
       state_base->type == EditorEnumDb::IO_STATE)
    {
      /* Enter */
      box_states_enter->setEnabled(true);
      box_states_enter->setChecked(state->base_enter);
      btn_states_enter->setEnabled(true);
      if(state->base_enter)
      {
        btn_states_enter->setText("View");
        if(state_base->set_enter.isEmpty())
          btn_states_enter->setFont(not_bold);
        else
          btn_states_enter->setFont(bold);
      }
      else
      {
        btn_states_enter->setText("Edit");
        if(state->set_enter.isEmpty())
          btn_states_enter->setFont(not_bold);
        else
          btn_states_enter->setFont(bold);
      }

      /* Exit */
      box_states_exit->setEnabled(true);
      box_states_exit->setChecked(state->base_exit);
      btn_states_exit->setEnabled(true);
      if(state->base_exit)
      {
        btn_states_exit->setText("View");
        if(state_base->set_exit.isEmpty())
          btn_states_exit->setFont(not_bold);
        else
          btn_states_exit->setFont(bold);
      }
      else
      {
        btn_states_exit->setText("Edit");
        if(state->set_exit.isEmpty())
          btn_states_exit->setFont(not_bold);
        else
          btn_states_exit->setFont(bold);
      }

      /* Use */
      box_states_use->setEnabled(true);
      box_states_use->setChecked(state->base_use);
      btn_states_use->setEnabled(true);
      if(state->base_use)
      {
        btn_states_use->setText("View");
        if(state_base->set_use.isEmpty())
          btn_states_use->setFont(not_bold);
        else
          btn_states_use->setFont(bold);
      }
      else
      {
        btn_states_use->setText("Edit");
        if(state->set_use.isEmpty())
          btn_states_use->setFont(not_bold);
        else
          btn_states_use->setFont(bold);
      }

      /* Walkover */
      box_states_walk->setEnabled(true);
      box_states_walk->setChecked(state->base_walkover);
      btn_states_walk->setEnabled(true);
      if(state->base_walkover)
      {
        btn_states_walk->setText("View");
        if(state_base->set_walkover.isEmpty())
          btn_states_walk->setFont(not_bold);
        else
          btn_states_walk->setFont(bold);
      }
      else
      {
        btn_states_walk->setText("Edit");
        if(state->set_walkover.isEmpty())
          btn_states_walk->setFont(not_bold);
        else
          btn_states_walk->setFont(bold);
      }
    }
    /* Not a valid state (transition or otherwise */
    else
    {
      box_states_enter->setDisabled(true);
      box_states_exit->setDisabled(true);
      box_states_use->setDisabled(true);
      box_states_walk->setDisabled(true);

      btn_states_enter->setDisabled(true);
      btn_states_enter->setFont(not_bold);

      btn_states_exit->setDisabled(true);
      btn_states_exit->setFont(not_bold);

      btn_states_use->setDisabled(true);
      btn_states_use->setFont(not_bold);

      btn_states_walk->setDisabled(true);
      btn_states_walk->setFont(not_bold);
    }
  }
}

/*
 * Description: Slot triggered when the combo box for the tracking type of the
 *              movement of the npc is changed. This changes how the npc reacts
 *              when a player comes in the vicinity of the npc.
 *
 * Inputs: int index - the new index in the combo box
 * Output: none
 */
void InstanceDialog::comboTrackingChange(int index)
{
  if(thing_type == EditorEnumDb::NPC)
  {
    EditorMapNPC* npc = (EditorMapNPC*)thing_working;

    /* Update tracking */
    npc->getPath()->setTracking(static_cast<TrackingState>(index));

    /* Enable/Disable of setpoint widgets */
    MapNPC::NodeState node_state = npc->getPath()->getState();
    TrackingState state = npc->getPath()->getTracking();
    spin_track_maintain->setEnabled(state == TrackingState::AVOIDPLAYER &&
                                    node_state != MapNPC::LOCKED);
    spin_track_release->setEnabled(state != TrackingState::NOTRACK &&
                                   node_state != MapNPC::RANDOM &&
                                   node_state != MapNPC::LOCKED);
    spin_track_start->setEnabled(state != TrackingState::NOTRACK &&
                                 node_state != MapNPC::LOCKED);
  }
}

/*
 * Description: Slot triggered when the item count is changed within
 *              the spin box. This is only relevant for the map item.
 *
 * Inputs: int value - the changed value of the spin box
 * Output: none
 */
void InstanceDialog::countChanged(int value)
{
  if(thing_type == EditorEnumDb::ITEM)
  {
    if(value > 0)
      static_cast<EditorMapItem*>(thing_working)->setCount(value);
  }
}

/*
 * Description: Edits the current event set instance trigger. Triggered
 *              from EventSetView. Required to lock out the pop-up.
 *
 * Inputs: EditorEventSet* set - the editing set
 *         QString window_title - title for edit event pop-up window
 *         bool view_only - true if the pop-up is for viewing data only
 * Output: none
 */
void InstanceDialog::editEventSet(EditorEventSet* set,
                                  QString window_title, bool view_only)
{
  /* Close lock dialog */
  buttonLockCancel();

  /* Close event dialog, if open */
  if(event_dialog != nullptr)
  {
    disconnect(event_dialog, SIGNAL(selectTile()),
               this, SLOT(selectTile()));
    disconnect(event_dialog, SIGNAL(ok()),
               this, SLOT(eventUpdated()));
    delete event_dialog;
  }
  event_dialog = nullptr;

  /* Create the new conversation dialog */
  if(set != nullptr)
  {
    event_dialog = new EventDialog(set, this, window_title,
                                   EventClassifier::NOEVENT, view_only);
    event_dialog->setListItems(list_items);
    event_dialog->setListMaps(list_maps);
    event_dialog->setListMapThings(list_map_things, list_map_ios,
                                   list_map_items, list_map_persons,
                                   list_map_npcs);
    event_dialog->setListSounds(list_sounds);
    event_dialog->setListSubmaps(list_submaps);
    connect(event_dialog, SIGNAL(selectTile()),
            this, SLOT(selectTile()));
    connect(event_dialog, SIGNAL(ok()),
            this, SLOT(eventUpdated()));
    event_dialog->show();
  }
}

/*
 * Description: Edits a single node in the instance widget, with the NodeDialog.
 *
 * Inputs: QListWidgetItem* - not used
 * Output: none
 */
void InstanceDialog::editNode(QListWidgetItem*)
{
  if(thing_type == EditorEnumDb::NPC)
  {
    EditorMapNPC* npc = (EditorMapNPC*)thing_working;
    int index = list_nodes->currentRow();

    if(npc != nullptr && npc->getPath()->getState() != MapNPC::LOCKED &&
       (npc->getPath()->getState() == MapNPC::LOOPED ||
        npc->getPath()->getState() == MapNPC::BACKANDFORTH || index == 0))
    {
      NodeDialog* dialog = new NodeDialog(npc->getPath(), index - 1, this);
      if(dialog->exec() == QDialog::Accepted)
      {
        updateNodes();
        list_nodes->setCurrentRow(index);
      }

      delete dialog;
      dialog = nullptr;
    }
  }
}

/*
 * Description: Button slot triggers when the edit event set ok is selected
 *              and the event is to be updated. If IO, it just updates the
 *              enter/exit/use/walkover event view. For other cases, updates
 *              the event set view
 *
 * Inputs: none
 * Output: none
 */
void InstanceDialog::eventUpdated()
{
  /* IO event */
  if(thing_type == EditorEnumDb::IO)
  {
    comboStateChange(combo_states->currentIndex());
  }
  /* All other things */
  else
  {
    event_view->eventUpdated();
  }
}

/*
 * Description: Slot triggered when the respawn time delay is changed within
 *              the spin box.
 *
 * Inputs: int value - the changed value of the spin box
 * Output: none
 */
void InstanceDialog::respawnChanged(int value)
{
  if(value > 0)
    thing_working->setActiveRespawn(value);
}

/*
 * Description: Triggers the select tile dialog for the pop-up. This hides the
 *              pop-up and waits for a tile to be selected on the map render.
 *
 * Inputs: none
 * Output: none
 */
void InstanceDialog::selectTile()
{
  if(thing_type != EditorEnumDb::ITEM)
  {
    waiting_for_submap = true;
    hide();

    /* Emit the proper signal */
    if(thing_type == EditorEnumDb::THING)
      emit selectTile(EditorEnumDb::THING_VIEW);
    else if(thing_type == EditorEnumDb::IO)
      emit selectTile(EditorEnumDb::IO_VIEW);
    else if(thing_type == EditorEnumDb::PERSON)
      emit selectTile(EditorEnumDb::PERSON_VIEW);
    else if(thing_type == EditorEnumDb::NPC)
      emit selectTile(EditorEnumDb::NPC_VIEW);
  }
}

/*
 * Description: Slot triggered when the speed spinner value changes. Updates the
 *              instance class with the new speed value (working instance).
 *
 * Inputs: int value - the new value for the spinner
 * Output: none
 */
void InstanceDialog::speedChanged(int value)
{
  /* This updates even when locked - if not wanted, add check for base status */
  ((EditorMapPerson*)thing_working)->setSpeed(value);

  if(value > 0)
    lbl_speed_result->setText(QString::number(4096 / value) + " ms/tile");
  else
    lbl_speed_result->setText("0 ms/tile");
}

/*
 * Description: Slot triggered when the spin box changes and sets the tracking
 *              distance run in tile count when the
 *              tracking NPC which is set to keep away from the player attempts
 *              to maintain this distance at all times.
 *
 * Inputs: none
 * Output: int - the distance in tiles for the keep away
 */
void InstanceDialog::trackMaintainChanged(int value)
{
  if(thing_type == EditorEnumDb::NPC && value > 0)
  {
    EditorMapNPC* npc = (EditorMapNPC*)thing_working;
    npc->getPath()->setTrackDistRun(value);

    /* Ensure associated values are within range */
    if(value <= spin_track_start->value())
      spin_track_start->setValue(value - 1);
    else if(value >= spin_track_release->value())
      spin_track_release->setValue(value + 1);
  }
}

/*
 * Description: Slot triggered when the spin box changes and sets the tracking
 *              distance max in tile count when the tracking NPC releases its
 *              hold on the player.
 *
 * Inputs: none
 * Output: int - the distance in tiles for the max
 */
void InstanceDialog::trackReleaseChanged(int value)
{
  if(thing_type == EditorEnumDb::NPC && value > 0)
  {
    EditorMapNPC* npc = (EditorMapNPC*)thing_working;
    npc->getPath()->setTrackDistMax(value);

    /* Ensure associated values are within range */
    if(value <= spin_track_maintain->value())
      spin_track_maintain->setValue(value - 1);
  }
}

/*
 * Description: Slot triggered when the spin box changes and sets the tracking
 *              distance min in tile count when the tracking NPC initiates its
 *              hold on the player and follows or runs away.
 *
 * Inputs: none
 * Output: int - the distance in tiles for the min
 */
void InstanceDialog::trackStartChanged(int value)
{
  if(thing_type == EditorEnumDb::NPC && value > 0)
  {
    EditorMapNPC* npc = (EditorMapNPC*)thing_working;
    npc->getPath()->setTrackDistMin(value);

    /* Ensure associated values are within range */
    if(value >= spin_track_maintain->value())
      spin_track_maintain->setValue(value + 1);
  }
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Returns the event dialog used within the instance dialog.
 *
 * Inputs: none
 * Output: EventDialog* - the event dialog widget for the current instance
 */
EventDialog* InstanceDialog::getEventDialog()
{
  return event_dialog;
}

/*
 * Description: Returns the list of items, used for event creation.
 *
 * Inputs: none
 * Output: QList<QString> - list of all items (for give item event)
 */
QList<QString> InstanceDialog::getListItems()
{
  return list_items;
}

/*
 * Description: Returns the list of maps, used for event creation.
 *
 * Inputs: none
 * Output: QList<QString> - list of all maps (for change map event)
 */
QList<QString> InstanceDialog::getListMaps()
{
  return list_maps;
}

/*
 * Description: Returns the list of map IOs, used for event creation.
 *
 * Inputs: none
 * Output: QList<QPair<QString,QString>> - list of all map IOs
 */
QList<QPair<QString,QString>> InstanceDialog::getListMapIOs()
{
  return list_map_ios;
}

/*
 * Description: Returns the list of map items, used for event creation.
 *
 * Inputs: none
 * Output: QList<QPair<QString,QString>> - list of all map items
 */
QList<QString> InstanceDialog::getListMapItems()
{
  return list_map_items;
}

/*
 * Description: Returns the list of map npcs, used for event creation.
 *
 * Inputs: none
 * Output: QList<QPair<QString,QString>> - list of all map npcs
 */
QList<QString> InstanceDialog::getListMapNPCs()
{
  return list_map_npcs;
}

/*
 * Description: Returns the list of map persons, used for event creation.
 *
 * Inputs: none
 * Output: QList<QPair<QString,QString>> - list of all map persons
 */
QList<QString> InstanceDialog::getListMapPersons()
{
  return list_map_persons;
}

/*
 * Description: Returns the list of map things, used for event creation.
 *
 * Inputs: none
 * Output: QList<QString> - list of all map things
 */
QList<QString> InstanceDialog::getListMapThings()
{
  return list_map_things;
}

/*
 * Description: Returns the list of parties, used for the dialog.
 *
 * Inputs: none
 * Output: QList<QString> - list of all parties for instance connection
 */
QList<QString> InstanceDialog::getListParties()
{
  return list_parties;
}

/*
 * Description: Returns the list of sounds, used for dialog and event creation.
 *
 * Inputs: none
 * Output: QList<QString> - list of all sounds (for all valid events, dialog)
 */
QList<QString> InstanceDialog::getListSounds()
{
  return list_sounds;
}

/*
 * Description: Returns the list of sub-maps, used for event creation.
 *
 * Inputs: none
 * Output: QList<QString> - list of all sub-maps (for teleport event)
 */
QList<QString> InstanceDialog::getListSubmaps()
{
  return list_submaps;
}

/*
 * Description: Sets the list of items, used for event creation
 *
 * Inputs: QList<QString> - list of all items (for give item event)
 * Output: none
 */
void InstanceDialog::setListItems(QList<QString> items)
{
  list_items = items;

  /* Event dialog data */
  if(event_dialog != nullptr)
    event_dialog->setListItems(items);
  if(lock_view != nullptr)
    lock_view->setListItems(items);
}

/*
 * Description: Sets the list of maps, used for event creation
 *
 * Inputs: QList<QString> - list of all maps (for change map event)
 * Output: none
 */
void InstanceDialog::setListMaps(QList<QString> maps)
{
  list_maps = maps;

  /* Event dialog data */
  if(event_dialog != nullptr)
    event_dialog->setListMaps(maps);
}

/*
 * Description: Sets the list of things, ios, items, persons, npcs, used for
 *              event creation.
 *
 * Inputs: QList<QString> things - list of all map things
 *         QList<QPair<QString,QString>> ios - list of all map ios
 *         QList<QString> items - list of all map items
 *         QList<QString> persons - list of all map persons
 *         QList<QString> npcs - list of all map npcs
 * Output: none
 */
void InstanceDialog::setListMapThings(QList<QString> things,
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

  /* Event dialog data */
  if(event_dialog != nullptr)
    event_dialog->setListMapThings(things, ios, items, persons, npcs);
}

/*
 * Description: Sets the list of parties, used for selecting linked game objs
 *
 * Inputs: QList<QString> - list of all parties
 * Output: none
 */
void InstanceDialog::setListParties(QList<QString> parties)
{
  list_parties = parties;
  combo_party->blockSignals(true);
  combo_party->clear();
  combo_party->addItem("None");
  combo_party->addItems(list_parties);
  updateData();
  combo_party->blockSignals(false);
}

/*
 * Description: Sets the list of sounds, to be used within the event.
 *
 * Inputs: QList<QString> sounds - the sound string list
 * Output: none
 */
void InstanceDialog::setListSounds(QList<QString> sounds)
{
  /* Base data */
  list_sounds = sounds;

  /* Event dialog data */
  if(event_dialog != nullptr)
    event_dialog->setListSounds(sounds);
}

/*
 * Description: Sets the list of sub-maps, used for event creation.
 *
 * Inputs: QList<QString> - list of all sub-maps (for teleport event)
 * Output: none
 */
void InstanceDialog::setListSubmaps(QList<QString> sub_maps)
{
  list_submaps = sub_maps;

  /* Event dialog data */
  if(event_dialog != nullptr)
    event_dialog->setListSubmaps(sub_maps);
}

/*
 * Description: Updates the original person with the working data. Called after
 *              the emit ok() is triggered. Will not change anything if there
 *              is no original person set (or it's NULL).
 *
 * Inputs: none
 * Output: none
 */
void InstanceDialog::updateOriginal()
{
  if(thing_working != nullptr)
  {
    /* Copy the data for the thing */
    if(thing_type == EditorEnumDb::THING)
    {
      *thing_original = *thing_working;
    }
    /* Copy the data for the person */
    else if(thing_type == EditorEnumDb::PERSON)
    {
      EditorMapPerson* ref_orig = (EditorMapPerson*)thing_original;
      EditorMapPerson* ref_work = (EditorMapPerson*)thing_working;
      bool dir_changed = (ref_orig->getStartingDirection() !=
                          ref_work->getStartingDirection());

      /* Update references */
      *ref_orig = *ref_work;

      /* If dir changed, emit accordingly */
      if(dir_changed)
        emit personChanged(ref_orig);
    }
    /* Copy the data for the npc */
    else if(thing_type == EditorEnumDb::NPC)
    {
      EditorMapNPC* ref_orig = (EditorMapNPC*)thing_original;
      EditorMapNPC* ref_work = (EditorMapNPC*)thing_working;
      bool dir_changed = (ref_orig->getStartingDirection() !=
                          ref_work->getStartingDirection());

      /* Update references */
      *ref_orig = *ref_work;

      /* If dir changed, emit accordingly */
      if(dir_changed)
        emit personChanged(ref_orig);
    }
    /* Copy the data for the interactive object */
    else if(thing_type == EditorEnumDb::IO)
    {
      *((EditorMapIO*)thing_original) = *((EditorMapIO*)thing_working);
    }
    /* Copy the data for the item */
    else if(thing_type == EditorEnumDb::ITEM)
    {
      *((EditorMapItem*)thing_original) = *((EditorMapItem*)thing_working);
    }
  }
}

/*
 * Description: When the update path has been completed, this function
 *              is called to re-show the dialog and clean-up.
 *
 * Inputs: none
 * Output: none
 */
void InstanceDialog::updatePathFinished()
{
  if(waiting_path)
  {
    waiting_path = false;
    updateNodes();
    show();
  }
}

/*
 * Description: Updates the dialog with the tile which was selected on the
 *              sub-map. This shows the pop-up and updates the event with the
 *              new location. If it was triggered from the embedded event view,
 *              it passes the updated location to it.
 *
 * Inputs: int id - the ID of the sub-map
 *         int x - the x tile location in the sub-map
 *         int y - the y tile location in the sub-map
 * Output: none
 */
void InstanceDialog::updateSelectedTile(int id, int x, int y)
{
  if(waiting_for_submap)
  {
    waiting_for_submap = false;
    show();
    if(event_dialog != nullptr)
      event_dialog->updateSelectedTile(id, x, y);
  }
}
