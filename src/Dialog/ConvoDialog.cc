/*******************************************************************************
 * Class Name: ConvoDialog
 * Date Created: February 15, 2015
 * Inheritance: QDialog
 * Description: The conversation dialog, which handles editing a singular 
 *              conversation node. Used primarily by ThingDialog and all
 *              children.
 ******************************************************************************/
#include "Dialog/ConvoDialog.h"
#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function for the conversation dialog. Takes a
 *              conversation pointer, a bool if it's an option, and the parent.
 *              Only allows editing one instance of a conversation (one entry).
 *
 * Inputs: Conversation* edit_convo - the reference conversation segment
 *         bool is_option - is the segment an option in a list?
 *         QWidget* parent - the parent widget
 */
ConvoDialog::ConvoDialog(Conversation* edit_convo, bool is_option, 
                         QWidget* parent)
           : QDialog(parent)
{
  /* Initialize variables */
  convo_original = edit_convo;
  convo_working.action_event = EventHandler::createEventTemplate();
  convo_working.category = DialogCategory::TEXT;
  convo_working.text = "";
  convo_working.thing_id = 0;
  if(convo_original != NULL)
  {
    convo_working.action_event = convo_original->action_event;
    convo_working.category = convo_original->category;
    convo_working.text = convo_original->text;
    convo_working.thing_id = convo_original->thing_id;
  }
  event_ctrl = new EditorEvent(convo_working.action_event);

  /* Create the dialog */
  createDialog(is_option);

  /* Fill with data */
  updateData();
}

/*
 * Description: Destructor function
 */
ConvoDialog::~ConvoDialog()
{
  event_view->setEvent(NULL);
  delete event_ctrl;
  event_ctrl = NULL;
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: Creates the dialog layout with QT functional widgets.
 *
 * Inputs: bool is_option - is the segment conversation an option?
 * Output: none
 */
void ConvoDialog::createDialog(bool is_option)
{
  /* Layout */
  QGridLayout* layout = new QGridLayout(this);
  layout->setSizeConstraint(QLayout::SetFixedSize);

  /* The text edit widget */
  text_box = new QTextEdit(this);
  layout->addWidget(text_box, 0, 0, 1, 4);

  /* The thing choice widget */
  QLabel* lbl_thing = new QLabel("Interactor", this);
  thing_combo = new QComboBox(this);
  if(is_option)
    thing_combo->setDisabled(true);
  layout->addWidget(lbl_thing, 1, 0, 1, 1);
  layout->addWidget(thing_combo, 1, 1, 1, 3);

  /* The event widget */
  event_view = new EventView(event_ctrl, this, false);
  layout->addWidget(event_view, 2, 0, 1, 4);

  /* The push buttons */
  QPushButton* btn_ok = new QPushButton("Ok", this);
  connect(btn_ok, SIGNAL(clicked()), this, SLOT(buttonOk()));
  QPushButton* btn_cancel = new QPushButton("Cancel", this);
  connect(btn_cancel, SIGNAL(clicked()), this, SLOT(buttonCancel()));
  layout->addWidget(btn_ok, 3, 1);
  layout->addWidget(btn_cancel, 3, 2);

  /* Dialog control */
  setWindowTitle("Conversation Edit");
}

/*
 * Description: Updates the data in the widgets. CreateLayout() must be called
 *              prior.
 *
 * Inputs: none
 * Output: none
 */
void ConvoDialog::updateData()
{
  text_box->setPlainText(QString::fromStdString(convo_working.text));

  thing_combo->clear();
  int index = -1;
  for(int i = 0; i < list_things.size(); i++)
    thing_combo->addItem(list_things[i]);
  for(int i = 0; (index < 0) && (i < list_things.size()); i++)
  {
    QStringList list = list_things[i].split(":");
    if(list.size() == 2 && convo_working.thing_id == list.front().toInt())
      index = i;
  }
  if(index >= 0)
    thing_combo->setCurrentIndex(index);
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: The re-implementation of the close event for the dialog. Cleans
 *              up the existing conversation references.
 *
 * Inputs: QCloseEvent* - not used
 * Output: none
 */
void ConvoDialog::closeEvent(QCloseEvent*)
{
  convo_original = NULL;
  convo_working = EventHandler::createEmptyConversation();
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
void ConvoDialog::buttonCancel()
{
  close();
}

/*
 * Description: Button slot on the ok button. Copies the data into the original
 *              conversation reference and updates the list; then closes the
 *              dialog.
 *
 * Inputs: none
 * Output: none
 */
void ConvoDialog::buttonOk()
{
  if(convo_original != NULL)
  {
    /* Copy the data across */
    convo_original->text = text_box->toPlainText().toStdString();
    convo_original->action_event = *event_ctrl->getEvent();
    QString selected_thing = thing_combo->currentText();
    QStringList list = selected_thing.split(":");
    if(list.size() == 2 && list.front().toInt() >= -1)
      convo_original->thing_id = list.front().toInt();
  }

  emit success();
  close();
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/ 

/*
 * Description: Returns the event view within the conversation dialog.
 *
 * Inputs: none
 * Output: EventView* - the event view widget for the current convo segment
 */
EventView* ConvoDialog::getEventView()
{
  return event_view;
}

/*
 * Description: Returns the list of things that are being used in the event view
 *              of the dialog (for teleport events).
 *
 * Inputs: none
 * Output: QVector<QString> - the list of things as string with "ID: NAME"
 */
QVector<QString> ConvoDialog::getListThings()
{
  return list_things;
}

/*
 * Description: Sets the list of things that are being used in the event view
 *              of the dialog (for teleport events). Updates the event view.
 *
 * Inputs: QVector<QString> things - the list of things
 * Output: none
 */
void ConvoDialog::setListThings(QVector<QString> things)
{
  list_things = things;
  event_view->setListThings(things);

  /* Modify the list and append -1 entry to the very front */
  list_things.push_front("-1: This thing");

  updateData();
}
