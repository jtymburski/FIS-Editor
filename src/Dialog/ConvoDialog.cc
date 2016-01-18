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
 *         EventClassifier limiter - the restrictions on the event view
 *         QWidget* parent - the parent widget
 */
ConvoDialog::ConvoDialog(Conversation* edit_convo, bool is_option, 
                         EventClassifier limiter, QWidget* parent)
           : QDialog(parent)
{
  /* Initialize variables */
  convo_original = edit_convo;
  convo_working.action_event = EventSet::createBlankEvent();
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
  createDialog(is_option, limiter);

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
 *         EventClassifier limiter - the restrictions on the event view
 * Output: none
 */
void ConvoDialog::createDialog(bool is_option, EventClassifier limiter)
{
  /* Layout */
  QGridLayout* layout = new QGridLayout(this);
  //layout->setSizeConstraint(QLayout::SetFixedSize);

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
  int limit_int = ((int)limiter | (int)EventClassifier::CONVERSATION);
  event_view = new EventView(event_ctrl, this, (EventClassifier)limit_int);
  layout->addWidget(event_view, 2, 0, 1, 4);

  /* The push buttons */
  QPushButton* btn_ok = new QPushButton("Ok", this);
  btn_ok->setDefault(true);
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
  for(auto& list_thing : list_things)
    thing_combo->addItem(list_thing);
  //for(int i = 0; i < list_things.size(); i++)
  //  thing_combo->addItem(list_things[i]);
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
  convo_working = EventSet::createBlankConversation();
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
    convo_original->action_event =
                           EventSet::deleteEvent(convo_original->action_event);
    convo_original->action_event = EventSet::copyEvent(*event_ctrl->getEvent());
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
QVector<QString> ConvoDialog::getListMapThings()
{
  return list_things;
}

/*
 * Description: Sets the list of things that are being used in the event view
 *              of the dialog (for teleport events). Updates the event view.
 *
 * Inputs: QVector<QString> things - the list of map things
 *         QVector<QPair<QString,QString>> ios - the list of map ios
 *         QVector<QString> items - the list of map items
 *         QVector<QString> persons - the list of map persons
 *         QVector<QString> npcs - the list of map npcs
 *         bool is_thing - is the event on a thing? Default true.
 * Output: none
 */
void ConvoDialog::setListMapThings(QVector<QString> things,
                                   QVector<QPair<QString,QString>> ios,
                                   QVector<QString> items,
                                   QVector<QString> persons,
                                   QVector<QString> npcs,  bool is_thing)
{
  list_things = (persons + npcs + things);
  for(int i = 0; i < ios.size(); i++)
    list_things.push_back(ios[i].first);
  event_view->setListMapThings(things, ios, items, persons, npcs);

  /* Modify the list and append -1 entry to the very front */
  if(is_thing)
    list_things.push_front("-1: This thing");

  updateData();
}
