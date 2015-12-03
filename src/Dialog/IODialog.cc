/*******************************************************************************
 * Class Name: IODialog
 * Date Created: July 15, 2015
 * Inheritance: QDialog
 * Description: The dialog that displays the map IO and the information
 *              related to it. Allows manipulation of the data.
 ******************************************************************************/
#include "Dialog/IODialog.h"
//#include <QDebug>

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
  editing_event = NOEVENT;
  event_dialog = nullptr;
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
  io_original = nullptr;

  /* Delete the pop-up association */
  buttonLockCancel();
  editEventSet(nullptr);
  delete lock_ctrl;
  lock_ctrl = nullptr;

  /* Delete working IO */
  delete io_working;
  io_working = nullptr;
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
  layout->setRowStretch(7, 1);

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

  /* The lock widget */
  QLabel* lbl_lock = new QLabel("Lock:", this);
  layout->addWidget(lbl_lock, 5, 0);
  lbl_lock_data = new QLabel("ITEM #", this);
  lbl_lock_data->setStyleSheet("border: 1px solid #a8a8a8");
  layout->addWidget(lbl_lock_data, 5, 1, 1, 2);
  QPushButton* btn_lock = new QPushButton("Edit", this);
  connect(btn_lock, SIGNAL(clicked()), this, SLOT(buttonLockEdit()));
  layout->addWidget(btn_lock, 5, 3);

  /* The sound widget */
  QLabel* lbl_sound = new QLabel("Sound:", this);
  layout->addWidget(lbl_sound, 6, 0);
  combo_sound = new QComboBox(this);
  connect(combo_sound, SIGNAL(currentIndexChanged(QString)),
          this, SLOT(changedSound(QString)));
  layout->addWidget(combo_sound, 6, 1, 1, 3);

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
  layout->addWidget(lbl_frame, 7, 0, 2, 1);
  lbl_frame_img = new QLabel(this);
  lbl_frame_img->setMinimumSize(200, 200);
  lbl_frame_img->setStyleSheet("border: 1px solid black");
  lbl_frame_img->setAlignment(Qt::AlignCenter);
  layout->addWidget(lbl_frame_img, 7, 1, 2, 3);
  QPushButton* btn_frame_click = new QPushButton(this);
  btn_frame_click->setIcon(QIcon(":/images/icons/32_settings.png"));
  btn_frame_click->setIconSize(QSize(24,24));
  btn_frame_click->setMaximumSize(30, 30);
  if(instance)
    btn_frame_click->setDisabled(true);
  connect(btn_frame_click, SIGNAL(clicked()), this, SLOT(buttonFrameEdit()));
  layout->addWidget(btn_frame_click, 7, 3, 2, 1, Qt::AlignTop | Qt::AlignRight);

  /* Matrix View */
  matrix_view = new MatrixView(NULL, this);
  if(instance)
    matrix_view->setDisabled(true);
  layout->addWidget(matrix_view, 2, 4, 6, 4);

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
  layout->addWidget(group_events, 8, 4, 1, 4);

  /* The button control */
  layout->setRowMinimumHeight(9, 25);
  QPushButton* btn_ok = new QPushButton("Ok", this);
  btn_ok->setDefault(true);
  connect(btn_ok, SIGNAL(clicked()), this, SLOT(buttonOk()));
  layout->addWidget(btn_ok, 10, 6);
  QPushButton* btn_cancel = new QPushButton("Cancel", this);
  connect(btn_cancel, SIGNAL(clicked()), this, SLOT(buttonCancel()));
  layout->addWidget(btn_cancel, 10, 7);

  /* Title for dialog */
  setWindowTitle("Interactive Object Edit");

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
 * Description: Edits the current event set instance trigger. Triggered
 *              from EventSetView. Required to lock out the pop-up.
 *
 * Inputs: EditorEventSet* set - the editing set
 *         QString window_title - the event dialog window title
 * Output: none
 */
void IODialog::editEventSet(EditorEventSet* set, QString window_title)
{
  if(event_dialog != nullptr)
  {
    disconnect(event_dialog, SIGNAL(selectTile()),
               this, SLOT(selectTile()));
    disconnect(event_dialog, SIGNAL(ok()),
               this, SLOT(buttonEventOk()));
    delete event_dialog;
  }
  event_dialog = nullptr;
  editing_event = NOEVENT;

  /* Create the new conversation dialog */
  if(set != nullptr)
  {
    event_dialog = new EventDialog(set, this, window_title);
    event_dialog->setListIOs(list_ios);
    event_dialog->setListItems(list_items);
    event_dialog->setListMaps(list_maps);
    event_dialog->setListSounds(list_sounds);
    event_dialog->setListSubmaps(list_submaps);
    event_dialog->setListThings(list_things);
    connect(event_dialog, SIGNAL(selectTile()),
            this, SLOT(selectTile()));
    connect(event_dialog, SIGNAL(ok()),
            this, SLOT(buttonEventOk()));
    event_dialog->show();
  }
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
  combo_state->blockSignals(true);
  combo_state->clear();
  for(int i = 0; i < io_working->getStates().size(); i++)
    combo_state->addItem(io_working->getStateName(i));
  changedComboState(0);
  combo_state->blockSignals(false);

  /* Sound data - find index */
  int index = -1;
  for(int i = 0; i < list_sounds.size(); i++)
  {
    QStringList str_split = list_sounds[i].split(':');
    if(str_split.size() >= 2)
      if(str_split.front().toInt() == io_working->getSoundID())
        index = i;
  }

  /* Lock data */
  lbl_lock_data->setText(io_working->getLock()->getTextSummary(""));

  /* Sound data - load into combo */
  combo_sound->blockSignals(true);
  combo_sound->clear();
  combo_sound->addItems(QStringList(list_sounds));
  if(index >= 0)
    combo_sound->setCurrentIndex(index);
  combo_sound->blockSignals(false);
}

/*
 * Description: Updates the selected state visible information on the dialog to
 *              the current selected drop down index
 *
 * Inputs: none
 * Output: none
 */
void IODialog::updateState()
{
  EditorState* state = io_working->getState(state_index);

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
    if(!state->set_enter.isEmpty())
      btn_enter->setFont(bold);
    else
      btn_enter->setFont(not_bold);
    if(!state->set_exit.isEmpty())
      btn_exit->setFont(bold);
    else
      btn_exit->setFont(not_bold);
    if(!state->set_use.isEmpty())
      btn_use->setFont(bold);
    else
      btn_use->setFont(not_bold);
    if(!state->set_walkover.isEmpty())
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
  if(state_index == 0)
    btn_minus->setDisabled(true);
  else
    btn_minus->setEnabled(true);
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
  /* Close and remove all references */
  buttonLockCancel();
  editEventSet(nullptr);
  io_original = nullptr;

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
  buttonLockCancel();
  editEventSet(nullptr);

  /* Set the event and open the new pop-up */
  EditorState* state = io_working->getState(combo_state->currentIndex());
  if(state != nullptr && state->type == EditorEnumDb::IO_STATE)
  {
    editEventSet(&state->set_enter, "Enter Set Edit");
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
  buttonLockCancel();
  editEventSet(nullptr);

  /* Set the event and open the new pop-up */
  EditorState* state = io_working->getState(combo_state->currentIndex());
  if(state != nullptr && state->type == EditorEnumDb::IO_STATE)
  {
    editEventSet(&state->set_exit, "Exit Set Edit");
    editing_event = EXIT;
  }
}

/*
 * Description: Button slot on the ok button of the event pop-up. Closes and
 *              saves the event dialog to the IO working pointer in the IO
 *              dialog.
 *
 * Inputs: none
 * Output: none
 */
void IODialog::buttonEventOk()
{
  if(editing_event != NOEVENT)
  {
    /* Update IO Dialog */
    updateState();

    /* Clear out the event control and view class */
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
  buttonLockCancel();
  editEventSet(nullptr);

  /* Set the event and open the new pop-up */
  EditorState* state = io_working->getState(combo_state->currentIndex());
  if(state != nullptr && state->type == EditorEnumDb::IO_STATE)
  {
    editEventSet(&state->set_use, "Use Set Edit");
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
  buttonLockCancel();
  editEventSet(nullptr);

  /* Set the event and open the new pop-up */
  EditorState* state = io_working->getState(combo_state->currentIndex());
  if(state != nullptr && state->type == EditorEnumDb::IO_STATE)
  {
    editEventSet(&state->set_walkover, "Walkover Set Edit");
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
  /* Close the event pop-up if open */
  buttonLockCancel();
  editEventSet(nullptr);

  /* Delete the dialog, if it exists */
  if(frame_dialog != nullptr)
  {
    disconnect(frame_dialog, SIGNAL(ok()), this, SLOT(updateFrame()));
    delete frame_dialog;
  }
  frame_dialog = nullptr;

  /* Create the new frame */
  frame_dialog = new FrameDialog(this, io_working->getDialogImage());
  connect(frame_dialog, SIGNAL(ok()), this, SLOT(updateFrame()));
  frame_dialog->show();
}

/*
 * Description: Button slot on lock cancel button within lock edit pop-up
 *              created within the class. When selected, the pop-up is cleaned
 *              and the pop-up is hidden.
 *
 * Inputs: none
 * Output: none
 */
void IODialog::buttonLockCancel()
{
  pop_lock->hide();
  lock_view->setLock(nullptr);
}

/*
 * Description: Button slot on the lock edit button triggered from the IO
 *              dialog. This displays the lock edit widget to modify the lock
 *              data.
 *
 * Inputs: none
 * Output: none
 */
void IODialog::buttonLockEdit()
{
  /* Close the event pop-up if open */
  editEventSet(nullptr);

  *lock_ctrl = *io_working->getLock();
  lock_view->setLock(lock_ctrl);
  pop_lock->show();
}

/*
 * Description: Button slot on lock ok button within lock edit pop-up
 *              created within the class. When selected, the lock data is saved
 *              within the working IO, the pop-up is cleaned and the pop-up is
 *              hidden.
 *
 * Inputs: none
 * Output: none
 */
void IODialog::buttonLockOk()
{
  *io_working->getLock() = *lock_ctrl;
  lbl_lock_data->setText(io_working->getLock()->getTextSummary(""));
  buttonLockCancel();
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
  buttonLockCancel();
  editEventSet(nullptr);
  io_working->consolidate(state_index);

  emit ok();
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
  /* Close the event pop-up if open */
  buttonLockCancel();
  editEventSet(nullptr);

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
  /* Close the event pop-up if open */
  buttonLockCancel();
  editEventSet(nullptr);

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

  /* Close the event pop-up if open */
  buttonLockCancel();
  editEventSet(nullptr);

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
  /* Close the event pop-up if open */
  buttonLockCancel();
  editEventSet(nullptr);

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
  /* Close the event pop-up if open */
  buttonLockCancel();
  editEventSet(nullptr);

  /* Consolidate */
  if(index != state_index)
    io_working->consolidate(state_index);
  state_index = index;

  /* Update view */
  updateState();
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
  /* Close the event pop-up if open */
  buttonLockCancel();
  editEventSet(nullptr);

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
  /* Close the event pop-up if open */
  buttonLockCancel();
  editEventSet(nullptr);

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
  /* Close the event pop-up if open */
  buttonLockCancel();
  editEventSet(nullptr);

  io_working->setName(name);
}

/*
 * Description: Slot triggered on the combo box selection changed, which updates
 *              the stored sound ID within the working object.
 *
 * Inputs: QString text - the new sound row selection
 * Output: none
 */
void IODialog::changedSound(const QString &text)
{
  QStringList str_list = text.split(':');

  /* Close the event pop-up if open */
  buttonLockCancel();
  editEventSet(nullptr);

  /* If the list is two long, it is proper format - 001: Sound Example */
  if(str_list.size() >= 2)
  {
    io_working->setSoundID(str_list.front().toInt());
  }
  /* Otherwise, unset the sound ID */
  else
  {
    io_working->setSoundID(-1);
  }
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
  /* Close the event pop-up if open */
  buttonLockCancel();
  editEventSet(nullptr);

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
  waiting_for_submap = true;
  hide();
  emit selectTile(EditorEnumDb::IO_VIEW);
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
  /* Close the event pop-up if open */

  editEventSet(nullptr);
  buttonLockCancel();
  if(index == 1)
    io_working->setVisibility(true);
  else if(index == 0)
    io_working->setVisibility(false);
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Returns the event dialog used within the IO dialog.
 *
 * Inputs: none
 * Output: EventDialog* - the event dialog widget for the current IO
 */
EventDialog* IODialog::getEventDialog()
{
  return event_dialog;
}

/* Returns the list of objects, used for dialog and event creation */
// TODO: Comment
QVector<QPair<QString,QString>> IODialog::getListIOs()
{
  return list_ios;
}

/*
 * Description: Returns the list of items, used for event creation.
 *
 * Inputs: none
 * Output: QVector<QString> - list of all items (for give item event)
 */
QVector<QString> IODialog::getListItems()
{
  return list_items;
}

/*
 * Description: Returns the list of maps, used for event creation.
 *
 * Inputs: none
 * Output: QVector<QString> - list of all maps (for change map event)
 */
QVector<QString> IODialog::getListMaps()
{
  return list_maps;
}

/*
 * Description: Returns a list of sounds being used in the IO presently.
 *              Used when required for neighboring calls.
 *
 * Inputs: none
 * Output: QList<QString> - the sound string list
 */
QList<QString> IODialog::getListSounds()
{
  return list_sounds;
}

/*
 * Description: Returns the list of sub-maps, used for event creation.
 *
 * Inputs: none
 * Output: QVector<QString> - list of all sub-maps (for teleport event)
 */
QVector<QString> IODialog::getListSubmaps()
{
  return list_submaps;
}

/*
 * Description: Returns the list of things, used for event creation.
 *
 * Inputs: none
 * Output: QVector<QString> - list of all things (for teleport event)
 */
QVector<QString> IODialog::getListThings()
{
  return list_things;
}

/* Sets the list of objects, used for dialog and event creation */
void IODialog::setListIOs(QVector<QPair<QString,QString>> ios)
{
  list_ios = ios;

  /* Event dialog data */
  if(event_dialog != nullptr)
    event_dialog->setListIOs(ios);
}

/*
 * Description: Sets the list of items, used for event creation
 *
 * Inputs: QVector<QString> - list of all items (for give item event)
 * Output: none
 */
void IODialog::setListItems(QVector<QString> items)
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
 * Inputs: QVector<QString> - list of all maps (for change map event)
 * Output: none
 */
void IODialog::setListMaps(QVector<QString> maps)
{
  list_maps = maps;

  /* Event dialog data */
  if(event_dialog != nullptr)
    event_dialog->setListMaps(maps);
}

/*
 * Description: Sets the list of sounds, to be used within the IO dialog for
 *              the drop down selection and within the event.
 *
 * Inputs: QList<QString> sounds - the sound string list
 * Output: none
 */
void IODialog::setListSounds(QList<QString> sounds)
{
  /* Base data */
  list_sounds = sounds;
  updateData();

  /* Event dialog data */
  if(event_dialog != nullptr)
    event_dialog->setListSounds(sounds);
}

/*
 * Description: Sets the list of sub-maps, used for event creation.
 *
 * Inputs: QVector<QString> - list of all sub-maps (for teleport event)
 * Output: none
 */
void IODialog::setListSubmaps(QVector<QString> sub_maps)
{
  list_submaps = sub_maps;

  /* Event dialog data */
  if(event_dialog != nullptr)
    event_dialog->setListSubmaps(sub_maps);
}

/*
 * Description: Sets the list of things, used for event creation.
 *
 * Inputs: QVector<QString> - list of all things (for teleport event)
 * Output: none
 */
void IODialog::setListThings(QVector<QString> things)
{
  list_things = things;

  /* Event dialog data */
  if(event_dialog != nullptr)
    event_dialog->setListThings(things);
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
    if(event_dialog != nullptr)
      event_dialog->updateSelectedTile(id, x, y);
  }
}
