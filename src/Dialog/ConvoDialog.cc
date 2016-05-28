/*******************************************************************************
 * Class Name: ConvoDialog
 * Date Created: February 15, 2015
 * Inheritance: QDialog
 * Description: The conversation dialog, which handles editing a singular 
 *              conversation node. Used primarily by ThingDialog and all
 *              children.
 ******************************************************************************/
#include "Dialog/ConvoDialog.h"
//#include <QDebug>

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

  /* The push buttons */
  QHBoxLayout* btn_layout = new QHBoxLayout();
  btn_brush = new QPushButton(this);
  btn_brush->setIcon(QIcon(":/images/icons/32_brush.png"));
  btn_brush->setIconSize(QSize(24,24));
  btn_brush->setMaximumSize(30, 30);
  connect(btn_brush, SIGNAL(clicked()),
          this, SLOT(textBtnBrush()));
  btn_layout->addWidget(btn_brush);
  btn_bold = new QPushButton(this);
  btn_bold->setIcon(QIcon(":/images/icons/32_font_bold.png"));
  btn_bold->setIconSize(QSize(24,24));
  btn_bold->setMaximumSize(30, 30);
  connect(btn_bold, SIGNAL(clicked()),
          this, SLOT(textBtnBold()));
  btn_layout->addWidget(btn_bold);
  btn_italic = new QPushButton(this);
  btn_italic->setIcon(QIcon(":/images/icons/32_font_italic.png"));
  btn_italic->setIconSize(QSize(24,24));
  btn_italic->setMaximumSize(30, 30);
  connect(btn_italic, SIGNAL(clicked()),
          this, SLOT(textBtnItalic()));
  btn_layout->addWidget(btn_italic);
  btn_underline = new QPushButton(this);
  btn_underline->setIcon(QIcon(":/images/icons/32_font_underline.png"));
  btn_underline->setIconSize(QSize(24,24));
  btn_underline->setMaximumSize(30, 30);
  connect(btn_underline, SIGNAL(clicked()),
          this, SLOT(textBtnUnderline()));
  btn_layout->addWidget(btn_underline);
  QPushButton* btn_thing = new QPushButton(this);
  btn_thing->setIcon(QIcon(":/images/icons/32_user.png"));
  btn_thing->setIconSize(QSize(24,24));
  btn_thing->setMaximumSize(30, 30);
  connect(btn_thing, SIGNAL(clicked()),
          this, SLOT(textBtnThing()));
  btn_layout->addWidget(btn_thing);
  QPushButton* btn_preview = new QPushButton(this);
  btn_preview->setIcon(QIcon(":/images/icons/32_export.png"));
  btn_preview->setIconSize(QSize(24,24));
  btn_preview->setMaximumSize(30, 30);
  connect(btn_preview, SIGNAL(clicked()),
          this, SLOT(textBtnPreview()));
  btn_layout->addWidget(btn_preview);
  textSelected(false);
  layout->addLayout(btn_layout, 0, 0, 1, 4);

  /* The text edit widget */
  text_box = new QTextEdit(this);
  connect(text_box, SIGNAL(copyAvailable(bool)),
          this, SLOT(textSelected(bool)));
  layout->addWidget(text_box, 1, 0, 1, 4);

  /* The thing choice widget */
  QLabel* lbl_thing = new QLabel("Interactor", this);
  thing_combo = new QComboBox(this);
  EditorHelpers::comboBoxOptimize(thing_combo);
  if(is_option)
    thing_combo->setDisabled(true);
  layout->addWidget(lbl_thing, 2, 0, 1, 1);
  layout->addWidget(thing_combo, 2, 1, 1, 3);

  /* The event widget */
  int limit_int = ((int)limiter | (int)EventClassifier::CONVERSATION);
  event_view = new EventView(event_ctrl, this, (EventClassifier)limit_int);
  layout->addWidget(event_view, 3, 0, 1, 4);

  /* The push buttons */
  QPushButton* btn_ok = new QPushButton("Ok", this);
  btn_ok->setDefault(true);
  connect(btn_ok, SIGNAL(clicked()), this, SLOT(buttonOk()));
  QPushButton* btn_cancel = new QPushButton("Cancel", this);
  connect(btn_cancel, SIGNAL(clicked()), this, SLOT(buttonCancel()));
  layout->addWidget(btn_ok, 4, 1);
  layout->addWidget(btn_cancel, 4, 2);

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
  /* Set the text in the box */
  text_box->setPlainText(QString::fromStdString(convo_working.text));

  thing_combo->clear();
  int index = -1;
  thing_combo->addItems(list_things);
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

/*
 * Description: Slot which triggers when the text color changing button
 *              is pressed. Opens dialog to select from color selection
 *              (QColorDialog).
 *
 * Inputs: none
 * Output: none
 */
void ConvoDialog::textBtnBrush()
{
  /* Get the color */
  QColorDialog color_dialog;
  if(color_dialog.exec() == QDialog::Accepted)
  {
    QString selected_color = color_dialog.selectedColor().name();
    selected_color.remove(0, 1);

    /* Convert to format and insert in as pseudo html */
    QString selected_text = text_box->textCursor().selectedText();
    selected_text = "[" + selected_color + "]" + selected_text +
                    "[/" + selected_color + "]";
    text_box->textCursor().insertText(selected_text);
  }
}

/*
 * Description: Slot which triggers when the text bold button is
 *              pressed. Inserts the relevant pseudo xml for making the
 *              selection bold.
 *
 * Inputs: none
 * Output: none
 */
void ConvoDialog::textBtnBold()
{
  QString selected_text = text_box->textCursor().selectedText();
  selected_text = "[b]" + selected_text + "[/b]";
  text_box->textCursor().insertText(selected_text);
}

/*
 * Description: Slot which triggers when the text italic button is
 *              pressed. Inserts the relevant pseudo xml for making the
 *              selection italic.
 *
 * Inputs: none
 * Output: none
 */
void ConvoDialog::textBtnItalic()
{
  QString selected_text = text_box->textCursor().selectedText();
  selected_text = "[i]" + selected_text + "[/i]";
  text_box->textCursor().insertText(selected_text);
}

/*
 * Description: Slot which triggers when the text preview is selected.
 *              This loads the current xml for a quick preview of how it will
 *              render.
 *
 * Inputs: none
 * Output: none
 */
void ConvoDialog::textBtnPreview()
{
  /* Compile list */
  QList<QString> list_vec = event_view->getListMapPersons()
                          + event_view->getListMapNPCs()
                          + event_view->getListMapThings();
  QList<QPair<QString, QString>> list_map_ios = event_view->getListMapIOs();
  for(int i = 0; i < list_map_ios.size(); i++)
    list_vec.push_back(list_map_ios[i].first);
  list_vec += event_view->getListMapItems();

  /* Handle conversion */
  QString render_text = EditorHelpers::convertXml(
                                             text_box->toPlainText(), list_vec);

  /* Insert warning */
  QString info_text = QString("<font color=\"#900\">* Overlap nested color ") +
                      QString("rendering might not be exactly as shown</font>");

  /* Render */
  QMessageBox msg_box;
  msg_box.setText(render_text);
  msg_box.setInformativeText(info_text);
  msg_box.setWindowTitle("Preview");
  msg_box.exec();
}

/*
 * Description: Slot which triggers when the text thing insertion is
 *              pressed. Allows for selection of a thing ID to insert into the
 *              conversation text.
 *
 * Inputs: none
 * Output: none
 */
void ConvoDialog::textBtnThing()
{
  /* Compile list of all things */
  QList<QString> list = event_view->getListMapPersons();
  QString player = list.first();
  list.removeFirst();
  list += event_view->getListMapNPCs() + event_view->getListMapThings();
  QList<QPair<QString, QString>> list_map_ios = event_view->getListMapIOs();
  for(int i = 0; i < list_map_ios.size(); i++)
    list.push_back(list_map_ios[i].first);
  list += event_view->getListMapItems();
  list.push_front(player);

  /* Create input dialog to get selected thing */
  QInputDialog input_dialog;
  input_dialog.setComboBoxItems(list);
  if(input_dialog.exec() == QDialog::Accepted)
  {
    /* Check if the selected is valid and an ID */
    QStringList split = input_dialog.textValue().split(':');
    if(split.size() >= 2)
    {
      /* If valid, insert the number */
      bool ok;
      split.first().toInt(&ok);
      if(ok)
      {
        QString insert_text = "{" + split.first() + "}";
        text_box->textCursor().insertText(insert_text);
      }
    }
  }
}

/*
 * Description: Slot which triggers when the text underline button is
 *              pressed. Inserts the relevant pseudo xml for making the
 *              selection underlined.
 *
 * Inputs: none
 * Output: none
 */
void ConvoDialog::textBtnUnderline()
{
  QString selected_text = text_box->textCursor().selectedText();
  selected_text = "[u]" + selected_text + "[/u]";
  text_box->textCursor().insertText(selected_text);
}

/*
 * Description: Slot triggered when the text selection in the text
 *              changes. This controls if the buttons are enabled.
 *
 * Inputs: bool yes - text is selected
 * Output: none
 */
void ConvoDialog::textSelected(bool yes)
{
  btn_brush->setEnabled(yes);
  btn_bold->setEnabled(yes);
  btn_italic->setEnabled(yes);
  //btn_thing->setEnabled(yes);
  btn_underline->setEnabled(yes);
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
 * Output: QList<QString> - the list of things as string with "ID: NAME"
 */
QList<QString> ConvoDialog::getListMapThings()
{
  return list_things;
}

/*
 * Description: Sets the list of things that are being used in the event view
 *              of the dialog (for teleport events). Updates the event view.
 *
 * Inputs: QList<QString> things - the list of map things
 *         QList<QPair<QString,QString>> ios - the list of map ios
 *         QList<QString> items - the list of map items
 *         QList<QString> persons - the list of map persons
 *         QList<QString> npcs - the list of map npcs
 *         bool is_thing - is the event on a thing? Default true.
 * Output: none
 */
void ConvoDialog::setListMapThings(QList<QString> things,
                                   QList<QPair<QString,QString>> ios,
                                   QList<QString> items,
                                   QList<QString> persons,
                                   QList<QString> npcs,  bool is_thing)
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
