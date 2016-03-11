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
  btn_thing = new QPushButton(this);
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
  // TODO: This is the chunk for how HTML can be used...
  //text_box->setHtml(
  //  QString("<b>This text is bold</b> <i>This text is italic</i> ") +
  //  QString("<u>This text is underline</u> <font color=\"red\">Test</font>"));
  text_box->setPlainText(QString::fromStdString(convo_working.text));
  //text_box->setTextColor(QColor(255,0,0));
  //qDebug() << text_box->toHtml(); // TODO: Outputs crap...see below

  // TODO: PARSING INFORMATION FOR TEXT
//  QTextDocument* doc = text_box->document();
//  for(int i = 0; i < doc->blockCount(); i++)
//  {
//    QTextBlock block = doc->findBlockByNumber(i);
//    QTextBlock::iterator it = block.begin();
//    while(!it.atEnd())
//    {
//      QTextFragment fragment = it.fragment();
//      QTextCharFormat format = fragment.charFormat();
//      qDebug() << fragment.text();
//      qDebug() << "  - Bold: " << format.font().bold();
//      qDebug() << "  - Italic: " << format.font().italic();
//      qDebug() << "  - Underline: " << format.font().underline();
//      qDebug() << "  - Color: " << format.foreground().color().red() << ","
//                                << format.foreground().color().green() << ","
//                                << format.foreground().color().blue();

//      it++;
//    }
//  }

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
  qDebug() << "TODO: Color Brush - QColorDialog";
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
  qDebug() << "TODO: Bold";
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
  qDebug() << "TODO: Italic";
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
  qDebug() << "TODO: Preview";
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
  qDebug() << "TODO: Thing Insertion";
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
  qDebug() << "TODO: Underline";
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
  btn_thing->setEnabled(yes);
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
