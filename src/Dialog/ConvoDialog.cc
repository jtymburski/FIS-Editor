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

/* Constructor Function */
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

/* Destructor Function */
ConvoDialog::~ConvoDialog()
{

}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* Creates the dialog */
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
  layout->addWidget(lbl_thing, 1, 0, 1, 1);
  layout->addWidget(thing_combo, 1, 1, 1, 3);

  /* The event widget */
  event_view = new EventView(event_ctrl, this);
  if(is_option)
    event_view->setDisabled(true);
  layout->addWidget(event_view, 2, 0, 1, 4);

  /* The push buttons */
  QPushButton* btn_ok = new QPushButton("Ok", this);
  QPushButton* btn_cancel = new QPushButton("Cancel", this);
  layout->addWidget(btn_ok, 3, 1);
  layout->addWidget(btn_cancel, 3, 2);

  /* Dialog control */
  setWindowTitle("Conversation Edit");
}

/* Fill with data */
void ConvoDialog::updateData()
{

}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/* Custom close event */
void ConvoDialog::closeEvent(QCloseEvent* event)
{

}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
  
/* Button triggers */
void ConvoDialog::buttonCancel()
{

}

/* Button triggers */
void ConvoDialog::buttonOk()
{

}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/ 
  
/* Returns the event view in the pop-up */
void ConvoDialog::getEventView()
{

}
  
/* Returns the list of objects, used for event creation */
QVector<QString> ConvoDialog::getListThings()
{

}

/* Sets the list of objects, used for event creation */
void ConvoDialog::setListThings(QVector<QString> things)
{

}
