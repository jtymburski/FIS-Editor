/*******************************************************************************
 * Class Name: IODialog
 * Date Created: July 15, 2015
 * Inheritance: QDialog
 * Description: The dialog that displays the map IO and the information
 *              related to it. Allows manipulation of the data.
 ******************************************************************************/
#include "Dialog/IODialog.h"
#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function for IO dialog. Takes an editing IO
 *              and a parent widget. Pop-up constructed using editor IO.
 *
 * Inputs: EditorMapIO* edit_io - the IO to edit the data for
 *         QWidget* parent - the parent widget
 */
IODialog::IODialog(EditorMapIO* edit_io, QWidget* parent) : QDialog(parent)
{
  convo_dialog = NULL;
  editing_event = NOEVENT;
  frame_dialog = NULL;
  state_index = 0;
  waiting_for_submap = false;

  /* Set-up the IO set - copied to working for changes */
  io_original = edit_io;
  io_working = new EditorMapIO();
  if(io_original != NULL)
  {
    *io_working = *io_original;
  }
  event_ctrl = new EditorEvent(EventHandler::createEventTemplate());

  /* Layout setup */
  createLayout(edit_io->getBaseIO() != NULL);
  updateData();
  updateFrame();
}

/*
 * Description: Destructor function
 */
IODialog::~IODialog()
{
  io_original = NULL;

  /* Delete event controller */
  buttonEventCancel();
  event_view->setEvent(NULL);
  delete event_ctrl;
  event_ctrl = NULL;

  /* Delete working IO */
  if(io_working != NULL)
    delete io_working;
  io_working = NULL;
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: Creates the dialog layout with QT functional widgets.
 *
 * Inputs: bool instance - is the IO an instance (has a base IO)
 * Output: none
 */
void IODialog::createLayout(bool instance)
{
  /* Layout setup */
  QGridLayout* layout = new QGridLayout(this);
  layout->setSizeConstraint(QLayout::SetFixedSize);
  layout->setColumnStretch(2, 1);
  layout->setColumnStretch(5, 1);
  layout->setRowStretch(6, 1);

  /* The ID widget */
  QLabel* lbl_id = new QLabel("ID:", this);
  layout->addWidget(lbl_id, 0, 0);
  QLineEdit* line_id = new QLineEdit("", this);
  line_id->setText(QString::number(io_working->getID()));
  line_id->setDisabled(true);
  layout->addWidget(line_id, 0, 1);

  /* The name widget */
  QLabel* lbl_name = new QLabel("Name:", this);
  layout->addWidget(lbl_name, 1, 0);
  line_name = new QLineEdit("", this);
  connect(line_name, SIGNAL(textEdited(QString)),
          this, SLOT(changedName(QString)));
  layout->addWidget(line_name, 1, 1, 1, 3);

  /* The visibility widget */
  QLabel* lbl_visible = new QLabel("Visible:", this);
  layout->addWidget(lbl_visible, 2, 0);
  box_visible = new QComboBox(this);
  box_visible->addItem("False");
  box_visible->addItem("True");
  if(instance)
    box_visible->setDisabled(true);
  connect(box_visible, SIGNAL(currentIndexChanged(int)),
          this, SLOT(visibilityChanged(int)));
  layout->addWidget(box_visible, 2, 1);

  /* Inactive time widget */
  QLabel* lbl_time = new QLabel("Inactive Time:", this);
  layout->addWidget(lbl_time, 3, 0);
  spin_inactive = new QSpinBox(this);
  spin_inactive->setMaximum(99999999);
  spin_inactive->setSuffix(" ms");
  connect(spin_inactive, SIGNAL(valueChanged(int)),
          this, SLOT(changedInactive(int)));
  layout->addWidget(spin_inactive, 3, 1);
  box_inactive_disable = new QCheckBox("Disable", this);
  connect(box_inactive_disable, SIGNAL(stateChanged(int)),
          this, SLOT(checkInactive(int)));
  layout->addWidget(box_inactive_disable, 3, 2);

  /* The description widget */
  QLabel* lbl_description = new QLabel("Description:", this);
  layout->addWidget(lbl_description, 4, 0);
  text_description = new QTextEdit("", this);
  text_description->setAcceptRichText(false);
  text_description->setMaximumHeight(120);
  connect(text_description, SIGNAL(textChanged()),
          this, SLOT(changedDescription()));
  layout->addWidget(text_description, 4, 1, 1, 3);

  /* State selection */
  QLabel* lbl_state = new QLabel("State", this);
  layout->addWidget(lbl_state, 0, 4, 1, 1);
  combo_state = new QComboBox(this);
  connect(combo_state, SIGNAL(currentIndexChanged(int)),
          this, SLOT(changedComboState(int)));
  layout->addWidget(combo_state, 0, 5, 1, 1);
  QHBoxLayout* layout_button = new QHBoxLayout();
  layout_button->setSpacing(0);
  QPushButton* btn_plus = new QPushButton("+", this);
  QFont font = btn_plus->font();
  font.setBold(true);
  font.setPixelSize(14);
  btn_plus->setFont(font);
  btn_plus->setMaximumSize(30, 30);
  connect(btn_plus, SIGNAL(clicked()), this, SLOT(buttonStateAdd()));
  layout_button->addWidget(btn_plus);
  btn_minus = new QPushButton("-", this);
  btn_minus->setFont(font);
  btn_minus->setMaximumSize(30, 30);
  connect(btn_minus, SIGNAL(clicked()), this, SLOT(buttonStateRemove()));
  layout_button->addWidget(btn_minus);
  layout->addLayout(layout_button, 0, 6, 1, 1);
  layout_button->addStretch();
  QPushButton* btn_overview = new QPushButton("Overview", this);
  connect(btn_overview, SIGNAL(clicked()), this, SLOT(buttonStateOverview()));
  layout->addWidget(btn_overview, 0, 7);

  /* Interaction selection */
  QLabel* lbl_interaction = new QLabel("Interaction", this);
  layout->addWidget(lbl_interaction, 1, 4, 1, 1);
  combo_interaction = new QComboBox(this);
  combo_interaction->addItem("None");
  combo_interaction->addItem("Walk On");
  combo_interaction->addItem("Walk Off");
  combo_interaction->addItem("Use");
  connect(combo_interaction, SIGNAL(currentIndexChanged(int)),
          this, SLOT(changedComboInteract(int)));
  layout->addWidget(combo_interaction, 1, 5, 1, 3);

  /* The sprite view widget */
  QLabel* lbl_frame = new QLabel("Dialog Image:", this);
  layout->addWidget(lbl_frame, 5, 0, 3, 1);
  lbl_frame_img = new QLabel(this);
  lbl_frame_img->setMinimumSize(200, 200);
  lbl_frame_img->setStyleSheet("border: 1px solid black");
  lbl_frame_img->setAlignment(Qt::AlignCenter);
  layout->addWidget(lbl_frame_img, 5, 1, 3, 3);
  QPushButton* btn_frame_click = new QPushButton(this);
  btn_frame_click->setIcon(QIcon(":/images/icons/32_settings.png"));
  btn_frame_click->setIconSize(QSize(24,24));
  btn_frame_click->setMaximumSize(30, 30);
  if(instance)
    btn_frame_click->setDisabled(true);
  connect(btn_frame_click, SIGNAL(clicked()), this, SLOT(buttonFrameEdit()));
  layout->addWidget(btn_frame_click, 5, 3, 3, 1, Qt::AlignTop);

  /* Matrix View */
  matrix_view = new MatrixView(NULL, this);
  //matrix_view = new MatrixView(io_working->getMatrix(), this);
  if(instance)
    matrix_view->setDisabled(true);
  layout->addWidget(matrix_view, 2, 4, 5, 4);

  /* Event Control */
  group_events = new QGroupBox("Events", this);
  btn_enter = new QPushButton("Enter", this);
  connect(btn_enter, SIGNAL(clicked()), this, SLOT(buttonEventEnter()));
  btn_exit = new QPushButton("Exit", this);
  connect(btn_exit, SIGNAL(clicked()), this, SLOT(buttonEventExit()));
  btn_use = new QPushButton("Use", this);
  connect(btn_use, SIGNAL(clicked()), this, SLOT(buttonEventUse()));
  btn_walkover = new QPushButton("Walkover", this);
  connect(btn_walkover, SIGNAL(clicked()), this, SLOT(buttonEventWalkover()));
  QHBoxLayout* layout_events = new QHBoxLayout();
  layout_events->addWidget(btn_enter);
  layout_events->addWidget(btn_exit);
  layout_events->addWidget(btn_use);
  layout_events->addWidget(btn_walkover);
  group_events->setLayout(layout_events);
  layout->addWidget(group_events, 7, 4, 1, 4);

  /* Event View */
  pop_event = new QDialog(this);
  pop_event->setWindowTitle("Event Edit");
  QGridLayout* e_layout = new QGridLayout(pop_event);
  event_view = new EventView(event_ctrl, pop_event);
  connect(event_view, SIGNAL(editConversation(Conversation*,bool)),
          this, SLOT(editConversation(Conversation*,bool)));
  connect(event_view, SIGNAL(selectTile()), this, SLOT(selectTile()));
  e_layout->addWidget(event_view, 0, 0, 1, 4);
  QPushButton* btn_event_ok = new QPushButton("Ok", pop_event);
  btn_event_ok->setDefault(true);
  connect(btn_event_ok, SIGNAL(clicked()), this, SLOT(buttonEventOk()));
  e_layout->addWidget(btn_event_ok, 1, 2);
  QPushButton* btn_event_cancel = new QPushButton("Cancel", pop_event);
  connect(btn_event_cancel, SIGNAL(clicked()), this, SLOT(buttonEventCancel()));
  e_layout->addWidget(btn_event_cancel, 1, 3);
  pop_event->setLayout(e_layout);
  pop_event->hide();

  /* The button control */
  layout->setRowMinimumHeight(8, 25);
  QPushButton* btn_ok = new QPushButton("Ok", this);
  btn_ok->setDefault(true);
  connect(btn_ok, SIGNAL(clicked()), this, SLOT(buttonOk()));
  layout->addWidget(btn_ok, 9, 6);
  QPushButton* btn_cancel = new QPushButton("Cancel", this);
  connect(btn_cancel, SIGNAL(clicked()), this, SLOT(buttonCancel()));
  layout->addWidget(btn_cancel, 9, 7);

  /* Dialog control */
  setWindowTitle("Interactive Object Edit");
}

/*
 * Description: Updates the data in the widgets. CreateLayout() must be called
 *              prior.
 *
 * Inputs: none
 * Output: none
 */
void IODialog::updateData()
{
  /* Name */
  line_name->setText(io_working->getName());

  /* Description */
  text_description->setPlainText(io_working->getDescription());


  /* Visibility */
  if(io_working->isVisible())
    box_visible->setCurrentIndex(1);
  else
    box_visible->setCurrentIndex(0);

  /* Inactive */
  if(io_working->getInactiveTime() >= 0)
  {
    box_inactive_disable->setChecked(false);
    spin_inactive->setValue(io_working->getInactiveTime());
  }
  else
  {
    box_inactive_disable->setChecked(true);
  }

  /* States */
  for(int i = 0; i < io_working->getStates().size(); i++)
    combo_state->addItem(io_working->getStateName(i));
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
  
/*
 * Description: The re-implementation of the close event for the dialog. Cleans
 *              up the existing IO references and deletes memory.
 *
 * Inputs: QCloseEvent* event - the close event (accepted after clean-up)
 * Output: none
 */
void IODialog::closeEvent(QCloseEvent* event)
{
  /* Close pop-ups */
  buttonEventCancel();

  /* Clean up events */
  event_ctrl->setEventBlank();
  if(io_working != NULL)
    delete io_working;
  io_working = NULL;

  /* Finalize close */
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
void IODialog::buttonCancel()
{
  close();
}

/*
 * Description: Button slot on the cancel button of the event pop-up. Just
 *              closes the event dialog and doesn't save the changes.
 *
 * Inputs: none
 * Output: none
 */
void IODialog::buttonEventCancel()
{
  if(editing_event != NOEVENT)
  {
    /* If conversation dialog is visible, hide it first */
    if(convo_dialog != NULL && convo_dialog->isVisible())
      convo_dialog->buttonCancel();

    /* Then the event pop-up */
    pop_event->hide();
    event_ctrl->setEventBlank();
    event_view->update();
    editing_event = NOEVENT;
  }
}

/*
 * Description: Button slot on the enter event button in the dialog. Triggers
 *              the event pop-up to be visible with the enter event for the
 *              state
 *
 * Inputs: none
 * Output: none
 */
void IODialog::buttonEventEnter()
{
  /* Close the pop-up if open */
  if(editing_event != NOEVENT)
    buttonEventCancel();

  /* Set the event and open the new pop-up */
  EditorState* state = io_working->getState(combo_state->currentIndex());
  if(state != NULL && state->type == EditorEnumDb::IO_STATE)
  {
    event_ctrl->setEvent(EventHandler::copyEvent(state->event_enter));
    event_view->updateEvent();
    pop_event->show();
    editing_event = ENTER;
  }
}

/*
 * Description: Button slot on the exit event button in the dialog. Triggers
 *              the event pop-up to be visible with the exit event for the
 *              state
 *
 * Inputs: none
 * Output: none
 */
void IODialog::buttonEventExit()
{
  /* Close the pop-up if open */
  if(editing_event != NOEVENT)
    buttonEventCancel();

  /* Set the event and open the new pop-up */
  EditorState* state = io_working->getState(combo_state->currentIndex());
  if(state != NULL && state->type == EditorEnumDb::IO_STATE)
  {
    event_ctrl->setEvent(EventHandler::copyEvent(state->event_exit));
    event_view->updateEvent();
    pop_event->show();
    editing_event = EXIT;
  }
}

/*
 * Description: Button slot on the ok button of the event pop-up. Closes and
 *              savesz the event dialog to the IO working pointer in the IO
 *              dialog.
 *
 * Inputs: none
 * Output: none
 */
void IODialog::buttonEventOk()
{
  if(editing_event != NOEVENT)
  {
    /* If conversation dialog is visible, hide it first */
    if(convo_dialog != NULL && convo_dialog->isVisible())
      convo_dialog->buttonCancel();

    /* Hide pop-up */
    pop_event->hide();

    /* Set the event */
    int index = combo_state->currentIndex();
    bool success = false;
    if(editing_event == ENTER)
      success = io_working->setEventEnter(index, *event_ctrl->getEvent());
    else if(editing_event == EXIT)
      success = io_working->setEventExit(index, *event_ctrl->getEvent());
    else if(editing_event == USE)
      success = io_working->setEventUse(index, *event_ctrl->getEvent());
    else if(editing_event == WALKOVER)
      success = io_working->setEventWalkover(index, *event_ctrl->getEvent());

    /* Update IO Dialog */
    changedComboState(index);

    /* Clear out the event control and view class */
    event_ctrl->setEventBlank(!success);
    event_view->update();
    editing_event = NOEVENT;
  }
}

/*
 * Description: Button slot on the use event button in the dialog. Triggers
 *              the event pop-up to be visible with the use event for the
 *              state
 *
 * Inputs: none
 * Output: none
 */
void IODialog::buttonEventUse()
{
  /* Close the pop-up if open */
  if(editing_event != NOEVENT)
    buttonEventCancel();

  /* Set the event and open the new pop-up */
  EditorState* state = io_working->getState(combo_state->currentIndex());
  if(state != NULL && state->type == EditorEnumDb::IO_STATE)
  {
    event_ctrl->setEvent(EventHandler::copyEvent(state->event_use));
    event_view->updateEvent();
    pop_event->show();
    editing_event = USE;
  }
}

/*
 * Description: Button slot on the walkover event button in the dialog. Triggers
 *              the event pop-up to be visible with the walkover event for the
 *              state
 *
 * Inputs: none
 * Output: none
 */
void IODialog::buttonEventWalkover()
{
  /* Close the pop-up if open */
  if(editing_event != NOEVENT)
    buttonEventCancel();

  /* Set the event and open the new pop-up */
  EditorState* state = io_working->getState(combo_state->currentIndex());
  if(state != NULL && state->type == EditorEnumDb::IO_STATE)
  {
    event_ctrl->setEvent(EventHandler::copyEvent(state->event_walkover));
    event_view->updateEvent();
    pop_event->show();
    editing_event = WALKOVER;
  }
}

/*
 * Description: Button slot on the frame edit button. This is triggered for
 *              the IO dialog. Opens the FrameDialog class.
 *
 * Inputs: none
 * Output: none
 */
void IODialog::buttonFrameEdit()
{
  /* Delete the dialog, if it exists */
  if(frame_dialog != NULL)
  {
    disconnect(frame_dialog, SIGNAL(ok()), this, SLOT(updateFrame()));
    delete frame_dialog;
  }
  frame_dialog = NULL;

  /* Create the new frame */
  frame_dialog = new FrameDialog(this, io_working->getDialogImage());
  connect(frame_dialog, SIGNAL(ok()), this, SLOT(updateFrame()));
  frame_dialog->show();
}

/*
 * Description: Button slot on the ok button. Emits ok prior to closing the
 *              dialog. The ok gets handled by the widget parent, which
 *              updates the visible IOs on map prior to accepting changes.
 *
 * Inputs: none
 * Output: none
 */
void IODialog::buttonOk()
{
  /* Consolidate before closing */
  io_working->consolidate(state_index);

  emit ok();
  event_ctrl->setEventBlank(false);
  close();
}

/*
 * Description: Button slot on the '+' button beside the node drop down which
 *              allows for appending another state or transition node to the end
 *              of the stack.
 *
 * Inputs: none
 * Output: none
 */
void IODialog::buttonStateAdd()
{
  QMessageBox msg_box;
  msg_box.setWindowTitle("Adding...");
  msg_box.setText("Select Node Type:");
  QPushButton* state = msg_box.addButton("State", QMessageBox::YesRole);
  msg_box.addButton("Transition", QMessageBox::NoRole);
  msg_box.exec();
  if(msg_box.clickedButton() == state)
  { /* STATE NODE */
    io_working->appendState(EditorEnumDb::IO_STATE);
  }
  else
  { /* TRANSITION NODE */
    io_working->appendState(EditorEnumDb::IO_TRANSITION);
  }

  /* Add to combo box */
  int index = io_working->getStates().size() - 1;
  combo_state->addItem(io_working->getStateName(index));
  combo_state->setCurrentIndex(index);
}

/*
 * Description: Button slot on the node state overview in the IO pop-up. This
 *              gives a summary of the state/transition sequencing for the
 *              designer.
 *
 * Inputs: none
 * Output: none
 */
void IODialog::buttonStateOverview()
{
  // TODO: Implementation
  QMessageBox::information(this, "Notification",
                           "Coming soon to a production near you!");
}

/*
 * Description: Button slot on the '-' button beside the node drop down which
 *              allows for removing the selected state or transition node in the
 *              stack.
 *
 * Inputs: none
 * Output: none
 */
void IODialog::buttonStateRemove()
{
  int index = combo_state->currentIndex();

  /* Create warning about deleting */
  QMessageBox msg_box;
  msg_box.setWindowTitle("Deleting...");
  msg_box.setText("Deleting node " + QString::number(index));
  msg_box.setInformativeText("Are you sure?");
  msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  if(msg_box.exec() == QMessageBox::Yes)
  {
    /* Set to base index and remove data from class */
    combo_state->setCurrentIndex(0);
    io_working->unsetState(index);

    /* Redo list */
    while(combo_state->count() > 1)
      combo_state->removeItem(1);
    for(int i = 1; i < io_working->getStates().size(); i++)
      combo_state->addItem(io_working->getStateName(i));

    /* Reset index */
    if(index >= combo_state->count())
      combo_state->setCurrentIndex(index - 1);
    else
      combo_state->setCurrentIndex(index);
  }
}

/*
 * Description: Connected to the combo box for the state interaction drop down.
 *              Triggers the update for the state interaction.
 *
 * Inputs: int index - the index in the dropdown.
 * Output: none
 */
void IODialog::changedComboInteract(int index)
{
  EditorState* state = io_working->getState(combo_state->currentIndex());
  if(state != NULL)
    state->interact = (MapState::InteractionState)index;
}

/*
 * Description: Connected to the combo box for the node drop down. Changes the
 *              matrix and information viewed below depending on the node.
 *              If the node is a state, interact drop down and event buttons are
 *              also enabled.
 *
 * Inputs: int index - the index in the dropdown.
 * Output: none
 */
void IODialog::changedComboState(int index)
{
  EditorState* state = io_working->getState(index);

  /* Consolidate */
  if(index != state_index)
    io_working->consolidate(state_index);

  /* Set the matrix */
  matrix_view->setMatrix(state->matrix);

  /* Depending on state mode, enable / disable some control */
  if(state->type == EditorEnumDb::IO_STATE)
  {
    combo_interaction->setEnabled(true);
    group_events->setEnabled(true);

    /* Interaction combo box */
    combo_interaction->setCurrentIndex((int)state->interact);

    /* Events */
    QFont bold = btn_enter->font();
    bold.setBold(true);
    QFont not_bold = bold;
    not_bold.setBold(false);
    if(state->event_enter.classification != EventClassifier::NOEVENT)
      btn_enter->setFont(bold);
    else
      btn_enter->setFont(not_bold);
    if(state->event_exit.classification != EventClassifier::NOEVENT)
      btn_exit->setFont(bold);
    else
      btn_exit->setFont(not_bold);
    if(state->event_use.classification != EventClassifier::NOEVENT)
      btn_use->setFont(bold);
    else
      btn_use->setFont(not_bold);
    if(state->event_walkover.classification != EventClassifier::NOEVENT)
      btn_walkover->setFont(bold);
    else
      btn_walkover->setFont(not_bold);
  }
  else if(state->type == EditorEnumDb::IO_TRANSITION)
  {
    combo_interaction->setDisabled(true);
    group_events->setDisabled(true);
  }

  /* If base index is 0, it cannot be deleted */
  if(index == 0)
    btn_minus->setDisabled(true);
  else
    btn_minus->setEnabled(true);

  /* Reset state index */
  state_index = index;
}

/*
 * Description: Slot triggered on the text edit field being changed, which
 *              updates the description.
 *
 * Inputs: none
 * Output: none
 */
void IODialog::changedDescription()
{
  QString simple = text_description->toPlainText().simplified();
  io_working->setDescription(simple);
}

/*
 * Description: Slot triggered on the spin box being changed, which updates the
 *              inactive time. Range is 0 to 99.9 million milliseconds.
 *
 * Inputs: int i - the new inactive time
 * Output: none
 */
void IODialog::changedInactive(int i)
{
  io_working->setInactiveTime(i);
}

/*
 * Description: Slot triggered on the text edit field being changed, which
 *              updates the name.
 *
 * Inputs: QString name - the new IO name
 * Output: none
 */
void IODialog::changedName(QString name)
{
  io_working->setName(name);
}

/*
 * Description: Slot triggered when the inactive disable/enable check box
 *              changes state.
 *
 * Inputs: int state - the new check box state
 * Output: none
 */
void IODialog::checkInactive(int state)
{
  if(state == Qt::Checked)
  {
    spin_inactive->setDisabled(true);
    io_working->setInactiveTime(-1);
  }
  else
  {
    spin_inactive->setEnabled(true);
    io_working->setInactiveTime(spin_inactive->value());
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
void IODialog::editConversation(Conversation* convo, bool is_option)
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
  convo_dialog->getEventView()->setListSubmaps(getEventView()->getListSubmaps());
  connect(convo_dialog->getEventView(), SIGNAL(selectTile()),
          this, SLOT(selectTileConvo()));
  connect(convo_dialog, SIGNAL(success()),
          event_view, SLOT(updateConversation()));
  convo_dialog->show();
}

/*
 * Description: Updates the frame dialog image in the pop-up. Scales the image
 *              to fit inside a fixed label.
 *
 * Inputs: none
 * Output: none
 */
void IODialog::updateFrame()
{
  QImage original = io_working->getDialogImage()->getImage(0);
  if(original.width() > 200 || original.height() > 200)
  {
    QImage scaled_image = original.scaled(200, 200, Qt::KeepAspectRatio);
    lbl_frame_img->setPixmap(QPixmap::fromImage(scaled_image));
  }
  else
  {
    lbl_frame_img->setPixmap(QPixmap::fromImage(original));
  }
}

/*
 * Description: Triggers the select tile dialog for the pop-up. This hides the
 *              pop-up and waits for a tile to be selected on the map render.
 *
 * Inputs: none
 * Output: none
 */
void IODialog::selectTile()
{
  waiting_convo = false;
  waiting_for_submap = true;
  hide();
  pop_event->hide();
  emit selectTile(EditorEnumDb::IO_VIEW);
}

/*
 * Description: Triggers the select tile dialog for the conversation in the
 *              event view in the pop-up. This hides the pop-up and waits for a
 *              tile to be selected on the map render.
 *
 * Inputs: none
 * Output: none
 */
void IODialog::selectTileConvo()
{
  if(convo_dialog != NULL)
  {
    selectTile();
    waiting_convo = true;
    convo_dialog->hide();
  }
}

/*
 * Description: The drop down for visibility of the IO changed slot. This
 *              updates the visibility in the working IO.
 *
 * Inputs: int index - index in the dropdown (only two, true or false)
 * Output: none
 */
void IODialog::visibilityChanged(int index)
{
  if(index == 1)
    io_working->setVisibility(true);
  else if(index == 0)
    io_working->setVisibility(false);
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Returns the event view within the IO dialog.
 *
 * Inputs: none
 * Output: EventView* - the event view widget for the current IO
 */
EventView* IODialog::getEventView()
{
  return event_view;
}

/*
 * Description: Updates the original IO with the working data. Called after
 *              the emit ok() is triggered. Will not change anything if there
 *              is no original IO set (or it's NULL).
 *
 * Inputs: none
 * Output: none
 */
void IODialog::updateOriginal()
{
  if(io_original != NULL)
  {
    *io_original = *io_working;
    //io_original->setEvent(*event_ctrl->getEvent());
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
void IODialog::updateSelectedTile(int id, int x, int y)
{
  if(waiting_for_submap)
  {
    waiting_for_submap = false;
    show();
    pop_event->show();
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
