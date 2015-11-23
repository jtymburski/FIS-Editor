/*******************************************************************************
 * Class Name: ThingDialog
 * Date Created: February 8, 2015
 * Inheritance: QDialog
 * Description: The dialog that displays the map thing and the information
 *              related to it. Allows manipulation of the data.
 ******************************************************************************/
#include "Dialog/ThingDialog.h"
#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function for thing dialog. Takes an editing thing
 *              and a parent widget. Pop-up constructed using editor thing.
 *
 * Inputs: EditorThing* edit_thing - the thing to edit the data for
 *         QWidget* parent - the parent widget
 */
ThingDialog::ThingDialog(EditorMapThing* edit_thing, QWidget* parent)
           : QDialog(parent)
{
  //convo_dialog = NULL;
  event_dialog = nullptr;
  frame_dialog = NULL;
  waiting_for_submap = false;

  /* Set-up the thing set - copied to working for changes */
  thing_original = edit_thing;
  thing_working = new EditorMapThing();
  if(thing_original != NULL)
  {
    *thing_working = *thing_original;
    //event_ctrl = new EditorEvent(
    //                           EventSet::copyEvent(thing_original->getEvent()));
  }
  //else
  //{
  //  event_ctrl = new EditorEvent(EventSet::createBlankEvent());
  //}

  /* Layout setup */
  createLayout(edit_thing->getBaseThing() != NULL);
  updateData();
  updateFrame();
}

/*
 * Description: Destructor function
 */
ThingDialog::~ThingDialog()
{
  thing_original = NULL;

  /* Delete event controller */
  //event_view->setEvent(NULL);
  //event_ctrl->setEventBlank();
  //delete event_ctrl;
  //event_ctrl = NULL;

  /* Delete working thing */
  if(thing_working != NULL)
    delete thing_working;
  thing_working = NULL;
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: Creates the dialog layout with QT functional widgets.
 *
 * Inputs: bool instance - is the thing an instance (has a base thing)
 * Output: none
 */
void ThingDialog::createLayout(bool instance)
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
  line_id->setText(QString::number(thing_working->getID()));
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

  /* The sprite view widget */
  QLabel* lbl_frame = new QLabel("Dialog Image:", this);
  layout->addWidget(lbl_frame, 4, 0, 2, 1);
  lbl_frame_img = new QLabel(this);
  //lbl_frame_img->setMinimumSize(200, 200);
  lbl_frame_img->setStyleSheet("border: 1px solid black");
  lbl_frame_img->setAlignment(Qt::AlignCenter);
  layout->addWidget(lbl_frame_img, 4, 1, 2, 3);
  QPushButton* btn_frame_click = new QPushButton(this);
  btn_frame_click->setIcon(QIcon(":/images/icons/32_settings.png"));
  btn_frame_click->setIconSize(QSize(24,24));
  btn_frame_click->setMaximumSize(30, 30);
  if(instance)
    btn_frame_click->setDisabled(true);
  connect(btn_frame_click, SIGNAL(clicked()), this, SLOT(buttonFrameEdit()));
  layout->addWidget(btn_frame_click, 4, 3, 2, 1, Qt::AlignTop);

  /* Event View */
  event_view = new EventSetView(nullptr, this);
  connect(event_view, SIGNAL(editSet(EditorEventSet*)),
          this, SLOT(editEventSet(EditorEventSet*)));
  layout->addWidget(event_view, 6, 0, 2, 4, Qt::AlignBottom);

//  QLabel* lbl_event_frame = new QLabel(this);
//  lbl_event_frame->setFrameStyle(QFrame::Panel | QFrame::Plain);
//  lbl_event_frame->setFixedSize(EditorEnumDb::kEVENT_VIEW_W,
//                                EditorEnumDb::kEVENT_VIEW_H);
//  layout->addWidget(lbl_event_frame, 6, 0, 2, 4, Qt::AlignBottom);

  //event_view = new EventView(event_ctrl, this);
  //if(instance)
  //  event_view->setDisabled(true);
  //layout->addWidget(event_view, 6, 0, 2, 4, Qt::AlignBottom);
  //connect(event_view, SIGNAL(editConversation(Conversation*,bool)),
  //        this, SLOT(editConversation(Conversation*,bool)));
  //connect(event_view, SIGNAL(selectTile()), this, SLOT(selectTile()));

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

  /* Matrix View */
  matrix_view = new MatrixView(thing_working->getMatrix(), this);
  if(instance)
    matrix_view->setDisabled(true);
  layout->addWidget(matrix_view, 4, 4, 4, 4);

  /* The button control */
  layout->setRowMinimumHeight(8, 15);
  QPushButton* btn_ok = new QPushButton("Ok", this);
  btn_ok->setDefault(true);
  connect(btn_ok, SIGNAL(clicked()), this, SLOT(buttonOk()));
  layout->addWidget(btn_ok, 9, 6);
  QPushButton* btn_cancel = new QPushButton("Cancel", this);
  connect(btn_cancel, SIGNAL(clicked()), this, SLOT(buttonCancel()));
  layout->addWidget(btn_cancel, 9, 7);

  /* Dialog control */
  setWindowTitle("Thing Edit");
}

/*
 * Description: Updates the data in the widgets. CreateLayout() must be called
 *              prior.
 *
 * Inputs: none
 * Output: none
 */
void ThingDialog::updateData()
{
  /* Core data */
  line_description->setText(thing_working->getDescription());
  line_name->setText(thing_working->getName());
  if(thing_working->isVisible())
    box_visible->setCurrentIndex(1);
  else
    box_visible->setCurrentIndex(0);

  /* Sound data - find index */
  int index = -1;
  for(int i = 0; i < list_sounds.size(); i++)
  {
    QStringList str_split = list_sounds[i].split(':');
    if(str_split.size() >= 2)
      if(str_split.front().toInt() == thing_working->getSoundID())
        index = i;
  }

  /* Sound data - load into combo */
  combo_sound->blockSignals(true);
  combo_sound->clear();
  combo_sound->addItems(QStringList(list_sounds));
  if(index >= 0)
    combo_sound->setCurrentIndex(index);
  combo_sound->blockSignals(false);

  /* Event view */
  event_view->setEventSet(thing_working->getEventSet());
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
  
/*
 * Description: The re-implementation of the close event for the dialog. Cleans
 *              up the existing thing references and deletes memory.
 *
 * Inputs: QCloseEvent* event - the close event (accepted after clean-up)
 * Output: none
 */
void ThingDialog::closeEvent(QCloseEvent* event)
{
  //event_ctrl->setEventBlank();
  if(thing_working != NULL)
    delete thing_working;
  thing_working = NULL;

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
void ThingDialog::buttonCancel()
{
  close();
}

/*
 * Description: Button slot on the frame edit button. This is triggered for
 *              the thing dialog. Opens the FrameDialog class.
 *
 * Inputs: none
 * Output: none
 */
void ThingDialog::buttonFrameEdit()
{
  /* Delete the dialog, if it exists */
  if(frame_dialog != NULL)
  {
    disconnect(frame_dialog, SIGNAL(ok()), this, SLOT(updateFrame()));
    delete frame_dialog;
  }
  frame_dialog = NULL;

  /* Create the new frame */
  frame_dialog = new FrameDialog(this, thing_working->getDialogImage());
  connect(frame_dialog, SIGNAL(ok()), this, SLOT(updateFrame()));
  frame_dialog->show();
}

/*
 * Description: Button slot on the ok button. Emits ok prior to closing the
 *              dialog. The ok gets handled by the widget parent, which
 *              updates the visible things on map prior to accepting changes.
 *
 * Inputs: none
 * Output: none
 */
void ThingDialog::buttonOk()
{
  /* Trim the matrix before accepting the sprites */
  matrix_view->buttonTrim();

  /* Proceed to ok() */
  emit ok();
  //event_ctrl->setEventBlank(false);
  close();
}

/*
 * Description: Slot triggered on the text edit field being changed, which
 *              updates the description.
 *
 * Inputs: QString description - the new description
 * Output: none
 */
void ThingDialog::changedDescription(QString description)
{
  thing_working->setDescription(description);
}

/*
 * Description: Slot triggered on the text edit field being changed, which
 *              updates the name.
 *
 * Inputs: QString name - the new thing name
 * Output: none
 */
void ThingDialog::changedName(QString name)
{
  thing_working->setName(name);
}

/*
 * Description: Slot triggered on the combo box selection changed, which updates
 *              the stored sound ID within the working object.
 *
 * Inputs: QString text - the new sound row selection
 * Output: none
 */
void ThingDialog::changedSound(const QString &text)
{
  QStringList str_list = text.split(':');

  /* If the list is two long, it is proper format - 001: Sound Example */
  if(str_list.size() >= 2)
  {
    thing_working->setSoundID(str_list.front().toInt());
  }
  /* Otherwise, unset the sound ID */
  else
  {
    thing_working->setSoundID(-1);
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
//void ThingDialog::editConversation(Conversation* convo, bool is_option)
//{
//  if(convo_dialog != NULL)
//  {
//    disconnect(convo_dialog->getEventView(), SIGNAL(selectTile()),
//               this, SLOT(selectTileConvo()));
//    disconnect(convo_dialog, SIGNAL(success()),
//               event_view, SLOT(updateConversation()));
//    delete convo_dialog;
//  }
//  convo_dialog = NULL;
//
//  /* Create the new conversation dialog */
//  convo_dialog = new ConvoDialog(convo, is_option, this);
//  convo_dialog->setListThings(getEventView()->getListThings());
//  convo_dialog->getEventView()->setListItems(getEventView()->getListItems());
//  convo_dialog->getEventView()->setListMaps(getEventView()->getListMaps());
//  convo_dialog->getEventView()->setListSounds(getListSounds());
//  convo_dialog->getEventView()->setListSubmaps(
//                                      getEventView()->getListSubmaps());
//  connect(convo_dialog->getEventView(), SIGNAL(selectTile()),
//          this, SLOT(selectTileConvo()));
//  connect(convo_dialog, SIGNAL(success()),
//          event_view, SLOT(updateConversation()));
//  convo_dialog->show();
//}

/* Edit event set trigger */
// TODO: Comment
void ThingDialog::editEventSet(EditorEventSet* set)
{
  if(event_dialog != nullptr)
  {
    disconnect(event_dialog, SIGNAL(selectTile()),
               this, SLOT(selectTile()));
    disconnect(event_dialog, SIGNAL(ok()),
               event_view, SLOT(eventUpdated()));
    delete event_dialog;
  }
  event_dialog = NULL;

  /* Create the new conversation dialog */
  if(set != nullptr)
  {
    event_dialog = new EventDialog(set, this);
    event_dialog->setListItems(list_items);
    event_dialog->setListMaps(list_maps);
    event_dialog->setListSounds(list_sounds);
    event_dialog->setListSubmaps(list_submaps);
    event_dialog->setListThings(list_things);
    connect(event_dialog, SIGNAL(selectTile()),
            this, SLOT(selectTile()));
    connect(event_dialog, SIGNAL(ok()),
            event_view, SLOT(eventUpdated()));
    event_dialog->show();
  }
}

/*
 * Description: Updates the frame dialog image in the pop-up. Scales the image
 *              to fit inside a fixed label.
 *
 * Inputs: none
 * Output: none
 */
void ThingDialog::updateFrame()
{
  QImage original = thing_working->getDialogImage()->getImage(0);
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
void ThingDialog::selectTile()
{
  //waiting_convo = false;
  waiting_for_submap = true;
  hide();
  emit selectTile(EditorEnumDb::THING_VIEW);
}

/*
 * Description: Triggers the select tile dialog for the conversation in the
 *              event view in the pop-up. This hides the pop-up and waits for a
 *              tile to be selected on the map render.
 *
 * Inputs: none
 * Output: none
 */
//void ThingDialog::selectTileConvo()
//{
//  if(convo_dialog != NULL)
//  {
//    selectTile();
//    waiting_convo = true;
//    convo_dialog->hide();
//  }
//}

/*
 * Description: The drop down for visibility of the thing changed slot. This
 *              updates the visibility in the working thing.
 *
 * Inputs: int index - index in the dropdown (only two, true or false)
 * Output: none
 */
void ThingDialog::visibilityChanged(int index)
{
  if(index == 1)
    thing_working->setVisibility(true);
  else if(index == 0)
    thing_working->setVisibility(false);
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Returns the event dialog widget */
// TODO: Comment
EventDialog* ThingDialog::getEventDialog()
{
  return event_dialog;
}

/*
 * Description: Returns the event view within the thing dialog.
 *
 * Inputs: none
 * Output: EventView* - the event view widget for the current thing
 */
//EventView* ThingDialog::getEventView()
//{
//  return event_view;
//}

/*
 * Description: Returns the list of items, used for event creation.
 *
 * Inputs: none
 * Output: QVector<QString> - list of all items (for give item event)
 */
QVector<QString> ThingDialog::getListItems()
{
  return list_items;
}

/*
 * Description: Returns the list of maps, used for event creation.
 *
 * Inputs: none
 * Output: QVector<QString> - list of all maps (for change map event)
 */
QVector<QString> ThingDialog::getListMaps()
{
  return list_maps;
}

/*
 * Description: Returns the list of sounds, used for dialog and event creation.
 *
 * Inputs: none
 * Output: QList<QString> - list of all sounds (for all valid events, dialog)
 */
QList<QString> ThingDialog::getListSounds()
{
  return list_sounds;
}

/*
 * Description: Returns the list of sub-maps, used for event creation.
 *
 * Inputs: none
 * Output: QVector<QString> - list of all sub-maps (for teleport event)
 */
QVector<QString> ThingDialog::getListSubmaps()
{
  return list_submaps;
}

/*
 * Description: Returns the list of things, used for event creation.
 *
 * Inputs: none
 * Output: QVector<QString> - list of all things (for teleport event)
 */
QVector<QString> ThingDialog::getListThings()
{
  return list_things;
}

/*
 * Description: Returns a list of sounds being used in the thing presently.
 *              Used when required for neighboring calls.
 *
 * Inputs: none
 * Output: QList<QString> - the sound string list
 */
//QList<QString> ThingDialog::getListSounds()
//{
//  return sound_list;
//}

/*
 * Description: Sets the list of items, used for event creation
 *
 * Inputs: QVector<QString> - list of all items (for give item event)
 * Output: none
 */
void ThingDialog::setListItems(QVector<QString> items)
{
  list_items = items;

  /* Event dialog data */
  if(event_dialog != nullptr)
    event_dialog->setListItems(items);
}

/*
 * Description: Sets the list of maps, used for event creation
 *
 * Inputs: QVector<QString> - list of all maps (for change map event)
 * Output: none
 */
void ThingDialog::setListMaps(QVector<QString> maps)
{
  list_maps = maps;

  /* Event dialog data */
  if(event_dialog != nullptr)
    event_dialog->setListMaps(maps);
}

/*
 * Description: Sets the list of sounds, to be used within the thing dialog for
 *              the drop down selection and within the event.
 *
 * Inputs: QList<QString> sounds - the sound string list
 * Output: none
 */
void ThingDialog::setListSounds(QList<QString> sounds)
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
void ThingDialog::setListSubmaps(QVector<QString> sub_maps)
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
void ThingDialog::setListThings(QVector<QString> things)
{
  list_things = things;

  /* Event dialog data */
  if(event_dialog != nullptr)
    event_dialog->setListThings(things);
}

/*
 * Description: Updates the original thing with the working data. Called after
 *              the emit ok() is triggered. Will not change anything if there
 *              is no original thing set (or it's NULL).
 *
 * Inputs: none
 * Output: none
 */
void ThingDialog::updateOriginal()
{
  if(thing_original != NULL)
  {
    *thing_original = *thing_working;
    //thing_original->setEvent(*event_ctrl->getEvent());
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
void ThingDialog::updateSelectedTile(int id, int x, int y)
{
  if(waiting_for_submap)
  {
    waiting_for_submap = false;
    show();
    if(event_dialog != nullptr)
      event_dialog->updateSelectedTile(id, x, y);
    //if(waiting_convo)
    //{
    //  convo_dialog->getEventView()->updateSelectedTile(id, x, y);
    //  convo_dialog->show();
    //}
    //else
    //{
    //  event_view->updateSelectedTile(id, x, y);
    //}
  }
}
