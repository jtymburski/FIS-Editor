/*******************************************************************************
 * Class Name: PersonDialog
 * Date Created: March 8, 2015
 * Inheritance: QDialog
 * Description: The dialog that displays the map person and the information
 *              related to it. Allows manipulation of the data.
 ******************************************************************************/
#include "Dialog/PersonDialog.h"
#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function for person dialog. Takes an editing person
 *              and a parent widget. Pop-up constructed using editor person.
 *
 * Inputs: EditorThing* edit_thing - the thing to edit the data for
 *         QWidget* parent - the parent widget
 */
PersonDialog::PersonDialog(EditorMapPerson* edit_person, QWidget* parent)
           : QDialog(parent)
{
  convo_dialog = NULL;
  frame_dialog = NULL;
  waiting_for_submap = false;

  /* Matrix defaults */
  matrix_direction = Direction::NORTH;
  matrix_surface = MapPerson::GROUND;

  /* Set-up the thing set - copied to working for changes */
  person_original = edit_person;
  person_working = new EditorMapPerson();
  if(person_original != NULL)
  {
    *person_working = *person_original;
    event_ctrl = new EditorEvent(
                              EventSet::copyEvent(person_original->getEvent()));
  }
  else
  {
    event_ctrl = new EditorEvent(EventSet::createBlankEvent());
  }

  /* Layout setup */
  createLayout(edit_person->getBasePerson() != NULL);
  updateData();
  updateFrame();
}

/*
 * Description: Destructor function
 */
PersonDialog::~PersonDialog()
{
  person_original = NULL;

  /* Delete event controller */
  event_view->setEvent(NULL);
  event_ctrl->setEventBlank();
  delete event_ctrl;
  event_ctrl = NULL;

  /* Delete working thing */
  if(person_working != NULL)
    delete person_working;
  person_working = NULL;
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: Makes all matrixes have common sizes, render depths, and
 *              passability. Passes call to rebase() in matrix.
 *
 * Inputs: none
 * Output: none
 */
void PersonDialog::consolidateAll()
{
  EditorMatrix* matrix = matrix_view->getMatrix();
  if(matrix != NULL)
  {
    QList<QList<EditorMatrix*>> matrix_set = person_working->getStates();
    for(int i = 0; i < matrix_set.size(); i++)
      for(int j = 0; j < matrix_set[i].size(); j++)
        if(matrix_set[i][j] != matrix)
          matrix_set[i][j]->rebase(matrix);
  }
}

/*
 * Description: Creates the dialog layout with QT functional widgets.
 *
 * Inputs: bool instance - is the person an instance (has a base person)
 * Output: none
 */
void PersonDialog::createLayout(bool instance)
{
  /* Layout setup */
  QGridLayout* layout = new QGridLayout(this);
  layout->setSizeConstraint(QLayout::SetFixedSize);
  layout->setColumnStretch(2, 1);
  layout->setColumnStretch(5, 1);

  /* The ID widget */
  QLabel* lbl_id = new QLabel("ID:", this);
  layout->addWidget(lbl_id, 0, 0);
  QLineEdit* line_id = new QLineEdit("", this);
  line_id->setText(QString::number(person_working->getID()));
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

  /* The speed widget */
  QLabel* lbl_speed = new QLabel("Speed:", this);
  layout->addWidget(lbl_speed, 4, 0);
  spin_speed = new QSpinBox(this);
  spin_speed->setMinimum(0);
  spin_speed->setMaximum(256);
  if(instance)
    spin_speed->setDisabled(true);
  connect(spin_speed, SIGNAL(valueChanged(int)), this, SLOT(speedChanged(int)));
  layout->addWidget(spin_speed, 4, 1);
  lbl_speed_result = new QLabel("X ms/tile");
  layout->addWidget(lbl_speed_result, 4, 2);

  /* The sprite view widget */
  QLabel* lbl_frame = new QLabel("Dialog Image:", this);
  layout->addWidget(lbl_frame, 5, 0, 2, 1);
  lbl_frame_img = new QLabel(this);
  lbl_frame_img->setMinimumSize(200, 200);
  lbl_frame_img->setStyleSheet("border: 1px solid black");
  lbl_frame_img->setAlignment(Qt::AlignCenter);
  layout->addWidget(lbl_frame_img, 5, 1, 2, 3);
  QPushButton* btn_frame_click = new QPushButton(this);
  btn_frame_click->setIcon(QIcon(":/images/icons/32_settings.png"));
  btn_frame_click->setIconSize(QSize(24,24));
  btn_frame_click->setMaximumSize(30, 30);
  if(instance)
    btn_frame_click->setDisabled(true);
  connect(btn_frame_click, SIGNAL(clicked()), this, SLOT(buttonFrameEdit()));
  layout->addWidget(btn_frame_click, 5, 3, 2, 1, Qt::AlignTop);

  /* Event View */
  event_view = new EventView(event_ctrl, this);
  if(instance)
    event_view->setDisabled(true);
  layout->addWidget(event_view, 7, 0, 2, 4, Qt::AlignBottom);
  connect(event_view, SIGNAL(editConversation(Conversation*,bool)),
          this, SLOT(editConversation(Conversation*,bool)));
  connect(event_view, SIGNAL(selectTile()), this, SLOT(selectTile()));

  /* The description widget */
  QLabel* lbl_description = new QLabel("Description:", this);
  layout->addWidget(lbl_description, 1, 4);
  line_description = new QLineEdit("", this);
  connect(line_description, SIGNAL(textEdited(QString)),
          this, SLOT(changedDescription(QString)));
  layout->addWidget(line_description, 1, 5, 1, 3);

  /* The sound widget */
  QLabel* lbl_sound = new QLabel("Sound:", this);
  layout->addWidget(lbl_sound, 2, 4);
  combo_sound = new QComboBox(this);
  connect(combo_sound, SIGNAL(currentIndexChanged(QString)),
          this, SLOT(changedSound(QString)));
  layout->addWidget(combo_sound, 2, 5, 1, 2);

  /* Matrix selection views */
  QComboBox* box_ground = new QComboBox(this);
  box_ground->addItem("Ground");
  connect(box_ground, SIGNAL(currentIndexChanged(QString)),
          this, SLOT(surfaceChange(QString)));
  layout->addWidget(box_ground, 4, 4, 1, 2);
  QComboBox* box_direction = new QComboBox(this);
  box_direction->addItem("North");
  box_direction->addItem("East");
  box_direction->addItem("South");
  box_direction->addItem("West");
  connect(box_direction, SIGNAL(currentIndexChanged(QString)),
          this, SLOT(directionChange(QString)));
  layout->addWidget(box_direction, 4, 6, 1, 2);

  /* Matrix View */
  matrix_view = new MatrixView(person_working->getMatrix(), this);
  if(instance)
    matrix_view->setDisabled(true);
  layout->addWidget(matrix_view, 5, 4, 4, 4);

  /* The button control */
  layout->setRowMinimumHeight(9, 15);
  QPushButton* btn_ok = new QPushButton("Ok", this);
  btn_ok->setDefault(true);
  connect(btn_ok, SIGNAL(clicked()), this, SLOT(buttonOk()));
  layout->addWidget(btn_ok, 10, 6);
  QPushButton* btn_cancel = new QPushButton("Cancel", this);
  connect(btn_cancel, SIGNAL(clicked()), this, SLOT(buttonCancel()));
  layout->addWidget(btn_cancel, 10, 7);

  /* Dialog control */
  setWindowTitle("Person Edit");
}

/*
 * Description: Updates the data in the widgets. CreateLayout() must be called
 *              prior.
 *
 * Inputs: none
 * Output: none
 */
void PersonDialog::updateData()
{
  /* Core data */
  line_description->setText(person_working->getDescription());
  line_name->setText(person_working->getName());
  if(person_working->isVisible())
    box_visible->setCurrentIndex(1);
  else
    box_visible->setCurrentIndex(0);
  spin_speed->setValue(person_working->getSpeed());

  /* Update all the matrixes */
  consolidateAll();
  bool enabled = matrix_view->isEnabled();
  matrix_view->setMatrix(person_working->getState(matrix_surface,
                                                  matrix_direction));
  matrix_view->setEnabled(enabled);

  /* Sound data - find index */
  int index = -1;
  for(int i = 0; i < sound_list.size(); i++)
  {
    QStringList str_split = sound_list[i].split(':');
    if(str_split.size() >= 2)
      if(str_split.front().toInt() == person_working->getSoundID())
        index = i;
  }

  /* Sound data - load into combo */
  combo_sound->blockSignals(true);
  combo_sound->clear();
  combo_sound->addItems(QStringList(sound_list));
  if(index >= 0)
    combo_sound->setCurrentIndex(index);
  combo_sound->blockSignals(false);
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
void PersonDialog::closeEvent(QCloseEvent* event)
{
  matrix_view->setMatrix(NULL);
  event_ctrl->setEventBlank();
  if(person_working != NULL)
    delete person_working;
  person_working = NULL;

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
void PersonDialog::buttonCancel()
{
  close();
}

/*
 * Description: Button slot on the frame edit button. This is triggered for
 *              the person dialog. Opens the FrameDialog class.
 *
 * Inputs: none
 * Output: none
 */
void PersonDialog::buttonFrameEdit()
{
  /* Delete the dialog, if it exists */
  if(frame_dialog != NULL)
  {
    disconnect(frame_dialog, SIGNAL(ok()), this, SLOT(updateFrame()));
    delete frame_dialog;
  }
  frame_dialog = NULL;

  /* Create the new frame */
  frame_dialog = new FrameDialog(this, person_working->getDialogImage());
  connect(frame_dialog, SIGNAL(ok()), this, SLOT(updateFrame()));
  frame_dialog->show();
}

/*
 * Description: Button slot on the ok button. Emits ok prior to closing the
 *              dialog. The ok gets handled by the widget parent, which
 *              updates the visible persons on map prior to accepting changes.
 *
 * Inputs: none
 * Output: none
 */
void PersonDialog::buttonOk()
{
  /* Trim the matrix before accepting the sprites */
  EditorMatrix* matrix = matrix_view->getMatrix();
  matrix->trim();
  consolidateAll();

  /* Proceed to ok() */
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
void PersonDialog::changedDescription(QString description)
{
  person_working->setDescription(description);
}

/*
 * Description: Slot triggered on the text edit field being changed, which
 *              updates the name.
 *
 * Inputs: QString name - the new person name
 * Output: none
 */
void PersonDialog::changedName(QString name)
{
  person_working->setName(name);
}

/*
 * Description: Slot triggered on the combo box selection changed, which updates
 *              the stored sound ID within the working object.
 *
 * Inputs: QString text - the new sound row selection
 * Output: none
 */
void PersonDialog::changedSound(const QString &text)
{
  QStringList str_list = text.split(':');

  /* If the list is two long, it is proper format - 001: Sound Example */
  if(str_list.size() >= 2)
  {
    person_working->setSoundID(str_list.front().toInt());
  }
  /* Otherwise, unset the sound ID */
  else
  {
    person_working->setSoundID(-1);
  }
}

/*
 * Description: Change the direction in the drop down of the matrix being
 *              viewed / edited.
 *
 * Inputs: QString text - text from direction drop down
 * Output: none
 */
void PersonDialog::directionChange(QString text)
{
  if(text == "North")
    matrix_direction = Direction::NORTH;
  else if(text == "East")
    matrix_direction = Direction::EAST;
  else if(text == "South")
    matrix_direction = Direction::SOUTH;
  else if(text == "West")
    matrix_direction = Direction::WEST;

  updateData();
}

/*
 * Description: Edits the current conversation instance trigger. Triggered
 *              from EventView. Required to lock out the pop-up.
 *
 * Inputs: Conversation* convo - the conversation segment to edit
 *         bool is_option - true if the segment is an option
 * Output: none
 */
void PersonDialog::editConversation(Conversation* convo, bool is_option)
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
  convo_dialog->getEventView()->setListSounds(getListSounds());
  convo_dialog->getEventView()->setListSubmaps(
                                           getEventView()->getListSubmaps());
  connect(convo_dialog->getEventView(), SIGNAL(selectTile()),
          this, SLOT(selectTileConvo()));
  connect(convo_dialog, SIGNAL(success()),
          event_view, SLOT(updateConversation()));
  convo_dialog->show();
}

/*
 * Description: Triggers the select tile dialog for the pop-up. This hides the
 *              pop-up and waits for a tile to be selected on the map render.
 *
 * Inputs: none
 * Output: none
 */
void PersonDialog::selectTile()
{
  waiting_convo = false;
  waiting_for_submap = true;
  hide();
  emit selectTile(EditorEnumDb::PERSON_VIEW);
}

/*
 * Description: Triggers the select tile dialog for the conversation in the
 *              event view in the pop-up. This hides the pop-up and waits for a
 *              tile to be selected on the map render.
 *
 * Inputs: none
 * Output: none
 */
void PersonDialog::selectTileConvo()
{
  if(convo_dialog != NULL)
  {
    selectTile();
    waiting_convo = true;
    convo_dialog->hide();
  }
}

/*
 * Description: Triggered when the speed changes in the data scroller. Updates
 *              the information in the working person.
 *
 * Inputs: int value - the new speed ratio
 * Output: none
 */
void PersonDialog::speedChanged(int value)
{
  person_working->setSpeed(value);

  if(value > 0)
    lbl_speed_result->setText(QString::number(4096 / value) + " ms/tile");
  else
    lbl_speed_result->setText("0 ms/tile");
}

/*
 * Description: Change the surface in the drop down of the matrix being
 *              viewed / edited.
 *
 * Inputs: QString text - text from surface drop down
 * Output: none
 */
void PersonDialog::surfaceChange(QString text)
{
  if(text == "Ground")
    matrix_surface = MapPerson::GROUND;

  updateData();
}

/*
 * Description: Updates the frame dialog image in the pop-up. Scales the image
 *              to fit inside a fixed label.
 *
 * Inputs: none
 * Output: none
 */
void PersonDialog::updateFrame()
{
  QImage original = person_working->getDialogImage()->getImage(0);
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
 * Description: The drop down for visibility of the person changed slot. This
 *              updates the visibility in the working person.
 *
 * Inputs: int index - index in the dropdown (only two, true or false)
 * Output: none
 */
void PersonDialog::visibilityChanged(int index)
{
  if(index == 1)
    person_working->setVisibility(true);
  else if(index == 0)
    person_working->setVisibility(false);
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
EventView* PersonDialog::getEventView()
{
  return event_view;
}

/*
 * Description: Returns a list of sounds being used in the person presently.
 *              Used when required for neighboring calls.
 *
 * Inputs: none
 * Output: QList<QString> - the sound string list
 */
QList<QString> PersonDialog::getListSounds()
{
  return sound_list;
}

/*
 * Description: Sets the list of sounds, to be used within the person dialog for
 *              the drop down selection and within the event.
 *
 * Inputs: QList<QString> sounds - the sound string list
 * Output: none
 */
void PersonDialog::setListSounds(QList<QString> sounds)
{
  /* Base data */
  sound_list = sounds;
  updateData();

  /* Event view data */
  if(event_view != nullptr)
    event_view->setListSounds(sounds);
}

/*
 * Description: Updates the original person with the working data. Called after
 *              the emit ok() is triggered. Will not change anything if there
 *              is no original person set (or it's NULL).
 *
 * Inputs: none
 * Output: none
 */
void PersonDialog::updateOriginal()
{
  if(person_original != NULL)
  {
    *person_original = *person_working;
    person_original->setEvent(*event_ctrl->getEvent());
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
void PersonDialog::updateSelectedTile(int id, int x, int y)
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
