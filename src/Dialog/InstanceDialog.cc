/*******************************************************************************
 * Class Name: InstanceDialog
 * Date Created: April 1, 2015
 * Inheritance: QDialog
 * Description: The dialog that displays the map thing instant and the
 *              information related to it. Allows manipulation of the data. 
 *              It also contains the path information used for map npcs.
 ******************************************************************************/
#include "Dialog/InstanceDialog.h"
#include <QDebug>

/* Constants */
const int InstanceDialog::kALGO_COUNT = 5;
const std::string InstanceDialog::kALGO_STATES[] = {"None", "Path Circular",
                                                    "Path Back and Forth",
                                                    "Random In Range",
                                                    "Random"};
const int InstanceDialog::kTRACK_COUNT = 3;
const std::string InstanceDialog::kTRACK_STATES[] = {"None", "Away From Player",
                                                     "To The Player"};

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

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
  if(thing_original != NULL)
    *thing_working = *thing_original;

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
  if(thing_original != NULL)
    *((EditorMapPerson*)thing_working) = *((EditorMapPerson*)thing_original);

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
  if(thing_original != NULL)
    *((EditorMapNPC*)thing_working) = *((EditorMapNPC*)thing_original);

  setup();
}

/*
 * Description: Destructor function
 */
InstanceDialog::~InstanceDialog()
{
  thing_original = NULL;

  /* Delete event controller */
  event_view->setEvent(NULL);
  event_ctrl->setEventBlank();
  delete event_ctrl;
  event_ctrl = NULL;

  /* Delete working thing */
  if(thing_working != NULL)
    delete thing_working;
  thing_working = NULL;
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
  QGridLayout* layout = new QGridLayout(this);
  layout->setSizeConstraint(QLayout::SetFixedSize);
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
  QLabel* lbl_description = new QLabel("Description", this);
  layout->addWidget(lbl_description, 2, 0);
  edit_description = new QTextEdit("", this);
  edit_description->setMaximumHeight(60);
  connect(edit_description, SIGNAL(textChanged()),
          this, SLOT(changedDescription()));
  layout->addWidget(edit_description, 2, 1, 2, 3);

  /* Use base event widget */
  box_base_event = new QCheckBox("Use Base Event", this);
  connect(box_base_event, SIGNAL(stateChanged(int)),
          this, SLOT(checkBaseChange(int)));
  layout->addWidget(box_base_event, 4, 1, 1, 2);

  /* Event View */
  event_view = new EventView(event_ctrl, this);
  connect(event_view, SIGNAL(editConversation(Conversation*,bool)),
          this, SLOT(editConversation(Conversation*,bool)));
  connect(event_view, SIGNAL(selectTile()), this, SLOT(selectTile()));
  layout->addWidget(event_view, 5, 0, 1, 4);

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
    for(int i = 0; i < kTRACK_COUNT; i++)
      combo_tracking->addItem(QString::fromStdString(kTRACK_STATES[i]));
    connect(combo_tracking, SIGNAL(currentIndexChanged(int)),
            this, SLOT(comboTrackingChange(int)));
    layout->addWidget(combo_tracking, 2, 6, 1, 3);

    /* Force interaction */
    box_interaction = new QCheckBox("Force Interaction", this);
    connect(box_interaction, SIGNAL(stateChanged(int)),
            this, SLOT(checkInteractionChange(int)));
    layout->addWidget(box_interaction, 3, 6, 1, 2);

    /* Node control */
    QLabel* lbl_nodes = new QLabel("Nodes", this);
    layout->addWidget(lbl_nodes, 4, 5);
    int icon_size = 24;
    int button_size = icon_size + 6;
    btn_edit_nodes = new QPushButton(this);
    btn_edit_nodes->setIcon(QIcon(":/images/icons/32_settings.png"));
    btn_edit_nodes->setIconSize(QSize(icon_size,icon_size));
    btn_edit_nodes->setMaximumSize(button_size, button_size);
    connect(btn_edit_nodes, SIGNAL(clicked()), this, SLOT(buttonEditNodes()));
    layout->addWidget(btn_edit_nodes, 4, 8, Qt::AlignRight);

    /* Node list */
    list_nodes = new QListWidget(this);
    connect(list_nodes, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(editNode(QListWidgetItem*)));
    layout->addWidget(list_nodes, 5, 5, 1, 4);

    btn_offset = 5;
  }

  /* The button control */
  layout->setRowMinimumHeight(6, 15);
  QPushButton* btn_ok = new QPushButton("Ok", this);
  btn_ok->setMaximumWidth(75);
  connect(btn_ok, SIGNAL(clicked()), this, SLOT(buttonOk()));
  layout->addWidget(btn_ok, 7, 2 + btn_offset);
  QPushButton* btn_cancel = new QPushButton("Cancel", this);
  btn_cancel->setMaximumWidth(75);
  connect(btn_cancel, SIGNAL(clicked()), this, SLOT(buttonCancel()));
  layout->addWidget(btn_cancel, 7, 3 + btn_offset);

  /* Dialog control */
  if(thing_type == EditorEnumDb::THING)
    setWindowTitle("Thing Instance Edit");
  else if(thing_type == EditorEnumDb::PERSON)
    setWindowTitle("Person Instance Edit");
  else if(thing_type == EditorEnumDb::NPC)
    setWindowTitle("NPC Instance Edit");
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
  convo_dialog = NULL;
  waiting_convo = false;
  waiting_for_submap = false;
  waiting_path = false;

  /* Set-up the event */
  if(thing_original != NULL)
    event_ctrl = new EditorEvent(
                           EventHandler::copyEvent(thing_original->getEvent()));
  else
    event_ctrl = new EditorEvent(EventHandler::createEventTemplate());

  /* Layout setup */
  createLayout();

  /* Name and descrip */
  line_name->setText(thing_working->getName());
  edit_description->setPlainText(thing_working->getDescription());

  /* Event control */
  box_base_event->blockSignals(true);
  box_base_event->setChecked(thing_working->isBaseEvent());
  event_view->setEnabled(!thing_working->isBaseEvent());
  box_base_event->blockSignals(false);

  /* If NPC, update movement data */
  if(thing_type == EditorEnumDb::NPC)
  {
    EditorMapNPC* npc = (EditorMapNPC*)thing_working;
    EditorNPCPath* path = npc->getPath();

    /* Combo box algorithm */
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

    /* Combo box tracking */
    if(path->getTracking() == MapNPC::NOTRACK)
      combo_tracking->setCurrentIndex(0);
    else if(path->getTracking() == MapNPC::AVOIDPLAYER)
      combo_tracking->setCurrentIndex(1);
    else if(path->getTracking() == MapNPC::TOPLAYER)
      combo_tracking->setCurrentIndex(2);

    /* Forced interaction */
    box_interaction->setChecked(path->isForcedInteraction());

    /* Nodes */
    updateNodes();
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
  event_ctrl->setEventBlank();
  if(thing_working != NULL)
    delete thing_working;
  thing_working = NULL;

  event->accept();
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

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
  emit ok();
  event_ctrl->setEventBlank(false);
  close();
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
 * Description: Slot triggered when the state of the check box for the "use
 *              base" event changes. This is used to use a different event for
 *              the thing instance, as opposed to the base event.
 *
 * Inputs: int state - the new state of the check box
 * Output: none
 */
void InstanceDialog::checkBaseChange(int state)
{
  /* Was this an instance event, now a base event? */
  bool was_instance = false;
  if(state == Qt::Checked)
    was_instance = true;

  /* If base is checked, event is disabled. Otherwise, enabled */
  thing_working->setUseBaseEvent(state == Qt::Checked);
  event_view->setEnabled(state == Qt::Unchecked);

  /* Update the event accordingly */
  EditorEvent* old_event = event_ctrl;
  event_ctrl = new EditorEvent(
                            EventHandler::copyEvent(thing_working->getEvent()));
  event_view->setEvent(event_ctrl);

  /* Handle the old event */
  if(was_instance)
  {
    thing_working->setEvent(*old_event->getEvent());
    old_event->setEventBlank(false);
  }
  else
  {
    old_event->setEventBlank();
  }
  delete old_event;
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

    /* Update nodes */
    updateNodes();
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
    if(index == 0)
      npc->getPath()->setTracking(MapNPC::NOTRACK);
    else if(index == 1)
      npc->getPath()->setTracking(MapNPC::AVOIDPLAYER);
    else if(index == 2)
      npc->getPath()->setTracking(MapNPC::TOPLAYER);
  }
}

/*
 * Description: Edits the current conversation instance trigger. Triggered
 *              from EventView. Required to lock out the pop-up.
 *
 * Inputs: Conversation* convo - the conversation segment to edit
 *         bool is_option - true if the segment is an option
 * Output: none
 */
void InstanceDialog::editConversation(Conversation* convo, bool is_option)
{
  if(convo_dialog != NULL)
  {
    disconnect(convo_dialog->getEventView(), SIGNAL(selectTile()),
               this, SLOT(selectTileConvo()));
    disconnect(convo_dialog, SIGNAL(success()),
               event_view, SLOT(updateConversation()));
    delete convo_dialog;
  }
  convo_dialog = NULL;

  /* Create the new conversation dialog */
  convo_dialog = new ConvoDialog(convo, is_option, this);
  convo_dialog->setListThings(getEventView()->getListThings());
  convo_dialog->getEventView()->setListItems(getEventView()->getListItems());
  convo_dialog->getEventView()->setListMaps(getEventView()->getListMaps());
  convo_dialog->getEventView()->setListSubmaps(
                                           getEventView()->getListSubmaps());
  connect(convo_dialog->getEventView(), SIGNAL(selectTile()),
          this, SLOT(selectTileConvo()));
  connect(convo_dialog, SIGNAL(success()),
          event_view, SLOT(updateConversation()));
  convo_dialog->show();
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

    if(npc != NULL && npc->getPath()->getState() != MapNPC::LOCKED &&
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
      dialog = NULL;
    }
  }
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
  waiting_convo = false;
  waiting_for_submap = true;
  hide();

  /* Emit the proper signal */
  if(thing_type == EditorEnumDb::THING)
    emit selectTile(EditorEnumDb::THING_VIEW);
  else if(thing_type == EditorEnumDb::PERSON)
    emit selectTile(EditorEnumDb::PERSON_VIEW);
  else if(thing_type == EditorEnumDb::NPC)
    emit selectTile(EditorEnumDb::NPC_VIEW);
}

/*
 * Description: Triggers the select tile dialog for the conversation in the
 *              event view in the pop-up. This hides the pop-up and waits for a
 *              tile to be selected on the map render.
 *
 * Inputs: none
 * Output: none
 */
void InstanceDialog::selectTileConvo()
{
  if(convo_dialog != NULL)
  {
    selectTile();
    waiting_convo = true;
    convo_dialog->hide();
  }
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Returns the event view within the person dialog.
 *
 * Inputs: none
 * Output: EventView* - the event view widget for the current person
 */
EventView* InstanceDialog::getEventView()
{
  return event_view;
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
  if(thing_working != NULL)
  {
    /* Copy the data for the thing */
    if(thing_type == EditorEnumDb::THING)
      *thing_original = *thing_working;
    else if(thing_type == EditorEnumDb::PERSON)
      *((EditorMapPerson*)thing_original) = *((EditorMapPerson*)thing_working);
    else if(thing_type == EditorEnumDb::NPC)
      *((EditorMapNPC*)thing_original) = *((EditorMapNPC*)thing_working);

    /* Fix the event */
    if(!box_base_event->isChecked())
      thing_original->setEvent(*event_ctrl->getEvent());
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
    if(waiting_convo)
    {
      convo_dialog->getEventView()->updateSelectedTile(id, x, y);
      convo_dialog->show();
    }
    else
    {
      event_view->updateSelectedTile(id, x, y);
    }
  }
}
