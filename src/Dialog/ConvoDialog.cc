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
  event_view->setEvent(NULL);
  delete event_ctrl;
  event_ctrl = NULL;
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

/* Fill with data */
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

/* Custom close event */
void ConvoDialog::closeEvent(QCloseEvent*)
{
  convo_original = NULL;
  convo_working = EventHandler::createEmptyConversation();
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
  
/* Button triggers */
void ConvoDialog::buttonCancel()
{
  close();
}

/* Button triggers */
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
  
/* Returns the event view in the pop-up */
EventView* ConvoDialog::getEventView()
{
  return event_view;
}
  
/* Returns the list of objects, used for event creation */
QVector<QString> ConvoDialog::getListThings()
{
  return list_things;
}

/* Sets the list of objects, used for event creation */
void ConvoDialog::setListThings(QVector<QString> things)
{
  list_things = things;
  event_view->setListThings(things);

  /* Modify the list and append -1 entry to the very front */
  list_things.push_front("-1: This thing");

  updateData();
}
