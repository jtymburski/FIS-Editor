/*******************************************************************************
 * Class Name: PersonInstDialog
 * Date Created: April 1, 2015
 * Inheritance: QDialog
 * Description: The dialog that displays the map person instant and the 
 *              information related to it. Allows manipulation of the data. 
 *              It also contains the path information used for map npcs.
 ******************************************************************************/
#include "Dialog/PersonInstDialog.h"
#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function for person instant dialog. Takes an 
 *              editing person and a parent widget. Pop-up constructed using 
 *              editor person.
 *
 * Inputs: EditorMapPerson* edit_person - the person to edit the data for
 *         QWidget* parent - the parent widget
 */
PersonInstDialog::PersonInstDialog(EditorMapPerson* edit_person, 
                                   QWidget* parent)
           : QDialog(parent)
{
  convo_dialog = NULL;
  waiting_for_submap = false;

  /* Set-up the thing set - copied to working for changes */
  is_npc = false;
  person_original = edit_person;
  person_working = new EditorMapPerson();
  if(person_original != NULL)
  {
    *person_working = *person_original;
    event_ctrl = new EditorEvent(
                          EventHandler::copyEvent(person_original->getEvent()));
  }
  else
  {
    event_ctrl = new EditorEvent(EventHandler::createEventTemplate());
  }

  /* Layout setup */
  createLayout();
  updateData();
}

/*
 * Description: Destructor function
 */
PersonInstDialog::~PersonInstDialog()
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
 * Description: Creates the dialog layout with QT functional widgets.
 *
 * Inputs: bool instance - is the person an instance (has a base person)
 * Output: none
 */
void PersonInstDialog::createLayout()
{
  /* Layout setup */
  QGridLayout* layout = new QGridLayout(this);
  layout->setSizeConstraint(QLayout::SetFixedSize);
  layout->setColumnStretch(2, 1);

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
  layout->addWidget(line_name, 1, 1, 1, 2);

  /* The description widget */
  QLabel* lbl_description = new QLabel("Description:", this);
  layout->addWidget(lbl_description, 3, 0);
  line_description = new QLineEdit("", this);
  connect(line_description, SIGNAL(textEdited(QString)),
          this, SLOT(changedDescription(QString)));
  layout->addWidget(line_description, 3, 1, 1, 5);

  /* Event View */
  event_view = new EventView(event_ctrl, this);
  layout->addWidget(event_view, 7, 0, 2, 4, Qt::AlignBottom);
  connect(event_view, SIGNAL(editConversation(Conversation*,bool)),
          this, SLOT(editConversation(Conversation*,bool)));
  connect(event_view, SIGNAL(selectTile()), this, SLOT(selectTile()));

  /* The button control */
  layout->setRowMinimumHeight(9, 15);
  QPushButton* btn_ok = new QPushButton("Ok", this);
  connect(btn_ok, SIGNAL(clicked()), this, SLOT(buttonOk()));
  layout->addWidget(btn_ok, 10, 6);
  QPushButton* btn_cancel = new QPushButton("Cancel", this);
  connect(btn_cancel, SIGNAL(clicked()), this, SLOT(buttonCancel()));
  layout->addWidget(btn_cancel, 10, 7);

  /* Dialog control */
  if(is_npc)
    setWindowTitle("NPC Instance Edit");
  else
    setWindowTitle("Person Instance Edit");
}

/*
 * Description: Updates the data in the widgets. CreateLayout() must be called
 *              prior.
 *
 * Inputs: none
 * Output: none
 */
void PersonInstDialog::updateData()
{
  line_description->setText(person_working->getDescription());
  line_name->setText(person_working->getName());
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
void PersonInstDialog::closeEvent(QCloseEvent* event)
{
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
void PersonInstDialog::buttonCancel()
{
  close();
}

/*
 * Description: Button slot on the ok button. Emits ok prior to closing the
 *              dialog. The ok gets handled by the widget parent, which
 *              updates the visible persons on map prior to accepting changes.
 *
 * Inputs: none
 * Output: none
 */
void PersonInstDialog::buttonOk()
{
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
void PersonInstDialog::changedDescription(QString description)
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
void PersonInstDialog::changedName(QString name)
{
  person_working->setName(name);
}

/*
 * Description: Edits the current conversation instance trigger. Triggered
 *              from EventView. Required to lock out the pop-up.
 *
 * Inputs: Conversation* convo - the conversation segment to edit
 *         bool is_option - true if the segment is an option
 * Output: none
 */
void PersonInstDialog::editConversation(Conversation* convo, bool is_option)
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
 * Description: Triggers the select tile dialog for the pop-up. This hides the
 *              pop-up and waits for a tile to be selected on the map render.
 *
 * Inputs: none
 * Output: none
 */
void PersonInstDialog::selectTile()
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
void PersonInstDialog::selectTileConvo()
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
EventView* PersonInstDialog::getEventView()
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
void PersonInstDialog::updateOriginal()
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
void PersonInstDialog::updateSelectedTile(int id, int x, int y)
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
