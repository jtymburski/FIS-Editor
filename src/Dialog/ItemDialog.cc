/*******************************************************************************
 * Class Name: ItemDialog
 * Date Created: June 28, 2015
 * Inheritance: QDialog
 * Description: The dialog that displays the map item and the information
 *              related to it. Allows manipulation of the data.
 ******************************************************************************/
#include "Dialog/ItemDialog.h"
#include <QDebug>

/* Constant Implementation - see header file for descriptions */
//const int ItemDialog::kMAX_COUNT = 2000000000;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function for item dialog. Takes an editing item
 *              and a parent widget. Pop-up constructed using editor item.
 *
 * Inputs: EditorMapItem* edit_item - the item to edit the data for
 *         QWidget* parent - the parent widget
 */
ItemDialog:: ItemDialog(EditorMapItem* edit_item, QWidget* parent)
          : QDialog(parent)
{
  frame_dialog = nullptr;

  /* Set-up the item set - copied to working for changes */
  item_original = edit_item;
  item_working = new EditorMapItem();
  if(item_original != nullptr)
    *item_working = *item_original;

  /* Layout setup */
  createLayout(edit_item->getBaseItem() != nullptr);
  //box_core->blockSignals(true);
  updateData();
  //box_core->blockSignals(false);
  //updateFrame();
}

/*
 * Description: Destructor function
 */
ItemDialog::~ItemDialog()
{
  item_original = nullptr;

  /* Delete working thing */
  if(item_working != nullptr)
    delete item_working;
  item_working = nullptr;
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: Creates the dialog layout with QT functional widgets.
 *
 * Inputs: bool instance - is the item an instance (has a base item)
 * Output: none
 */
void ItemDialog::createLayout(bool instance)
{
  /* Layout setup */
  QGridLayout* layout = new QGridLayout(this);
  layout->setSizeConstraint(QLayout::SetFixedSize);
  //layout->setColumnMinimumWidth(3, 30);
  layout->setColumnStretch(3, 1);

  /* The ID widget */
  QLabel* lbl_id = new QLabel("ID:", this);
  layout->addWidget(lbl_id, 0, 0);
  QLineEdit* line_id = new QLineEdit("", this);
  line_id->setText(QString::number(item_working->getID()));
  line_id->setDisabled(true);
  layout->addWidget(line_id, 0, 1);

  /* The name widget */
  QLabel* lbl_name = new QLabel("Name:", this);
  layout->addWidget(lbl_name, 2, 0);
  line_name = new QLineEdit("", this);
  connect(line_name, SIGNAL(textEdited(QString)),
          this, SLOT(changedName(QString)));
  layout->addWidget(line_name, 2, 1, 1, 4);

  /* The visibility widget */
  QLabel* lbl_visible = new QLabel("Visible:", this);
  layout->addWidget(lbl_visible, 1, 0);
  box_visible = new QComboBox(this);
  box_visible->addItem("False");
  box_visible->addItem("True");
  if(instance)
    box_visible->setDisabled(true);
  connect(box_visible, SIGNAL(currentIndexChanged(int)),
          this, SLOT(visibilityChanged(int)));
  layout->addWidget(box_visible, 1, 1);

  /* The description widget */
  QLabel* lbl_description = new QLabel("Description:", this);
  layout->addWidget(lbl_description, 3, 0);
  line_description = new QLineEdit("", this);
  connect(line_description, SIGNAL(textEdited(QString)),
          this, SLOT(changedDescription(QString)));
  layout->addWidget(line_description, 3, 1, 1, 7);

  /* The sound widget */
  QLabel* lbl_sound = new QLabel("Sound:", this);
  layout->addWidget(lbl_sound, 4, 0);
  combo_sound = new QComboBox(this);
  connect(combo_sound, SIGNAL(currentIndexChanged(QString)),
          this, SLOT(changedSound(QString)));
  layout->addWidget(combo_sound, 4, 1, 1, 6);

  /* Core ID - not required for new additions - remove future? */
  //QLabel* lbl_core = new QLabel("Game Item:", this);
  //layout->addWidget(lbl_core, 0, 4, 1, 1);
  //box_core = new QComboBox(this);
  //box_core->addItem("None");
  //connect(box_core, SIGNAL(currentIndexChanged(int)),
  //        this, SLOT(coreItemChanged(int)));
  //layout->addWidget(box_core, 0, 5, 1, 3);

  /* Walkover item */
  QLabel* lbl_walkover = new QLabel("Walkover:", this);
  layout->addWidget(lbl_walkover, 1, 4, 1, 1);
  box_walkover = new QComboBox(this);
  box_walkover->addItem("False");
  box_walkover->addItem("True");
  connect(box_walkover, SIGNAL(currentIndexChanged(int)),
          this, SLOT(walkoverChanged(int)));
  layout->addWidget(box_walkover, 1, 5, 1, 2);

  /* Matrix View */
  matrix_view = new MatrixView(item_working->getMatrix(), this, true);
  if(instance)
    matrix_view->setDisabled(true);
  layout->addWidget(matrix_view, 7, 0, 1, 8, Qt::AlignHCenter);

  /* The button control */
  layout->setRowMinimumHeight(8, 15);
  //QPushButton* btn_ok = new QPushButton("Ok", this);
  //btn_ok->setDefault(true);
  //connect(btn_ok, SIGNAL(clicked()), this, SLOT(buttonOk()));
  //layout->addWidget(btn_ok, 9, 6);
  QPushButton* btn_cancel = new QPushButton("Close", this);
  btn_cancel->setDefault(true);
  connect(btn_cancel, SIGNAL(clicked()), this, SLOT(buttonCancel()));
  layout->addWidget(btn_cancel, 9, 7);

  /* Dialog control */
  setWindowTitle("Item View");

  /* Disable calls - added in association with the core item to map item
   * connection */
  box_visible->setDisabled(true);
  box_walkover->setDisabled(true);
  combo_sound->setDisabled(true);
  line_description->setDisabled(true);
  line_name->setDisabled(true);
  matrix_view->setDisabled(true);
}

/*
 * Description: Updates the data in the widgets. CreateLayout() must be called
 *              prior. Assumes signals have been blocked as needed.
 *
 * Inputs: none
 * Output: none
 */
void ItemDialog::updateData()
{
  /* General */
  line_description->setText(item_working->getDescription());
  line_name->setText(item_working->getName());

  /* Visiblity */
  if(item_working->isVisible())
    box_visible->setCurrentIndex(1);
  else
    box_visible->setCurrentIndex(0);

  /* Walkover */
  if(item_working->isWalkover())
    box_walkover->setCurrentIndex(1);
  else
    box_walkover->setCurrentIndex(0);

  /* Game Item - not required for new system - future remove? */
  //box_core->blockSignals(true);
  //box_core->setCurrentIndex(0);
  //for(int i = 0; i < list_items.size(); i++)
  //{
  //  int id = list_items[i].split(":").front().toInt();
  //  if(id == item_working->getGameID())
  //    box_core->setCurrentIndex(i + 1);
  //}
  //box_core->blockSignals(false);

  /* Sound data - find index */
  int index = -1;
  for(int i = 0; i < sound_list.size(); i++)
  {
    QStringList str_split = sound_list[i].split(':');
    if(str_split.size() >= 2)
      if(str_split.front().toInt() == item_working->getSoundID())
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
 *              up the existing item references and deletes memory.
 *
 * Inputs: QCloseEvent* event - the close event (accepted after clean-up)
 * Output: none
 */
void ItemDialog::closeEvent(QCloseEvent* event)
{
  if(item_working != NULL)
    delete item_working;
  item_working = NULL;

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
void ItemDialog::buttonCancel()
{
  close();
}

/*
 * Description: Button slot on the frame edit button. This is triggered for
 *              the item dialog. Opens the FrameDialog class.
 *
 * Inputs: none
 * Output: none
 */
void ItemDialog::buttonFrameEdit()
{
  /* Delete the dialog, if it exists */
  if(frame_dialog != NULL)
  {
    disconnect(frame_dialog, SIGNAL(ok()), this, SLOT(updateFrame()));
    delete frame_dialog;
  }
  frame_dialog = NULL;

  /* Create the new frame */
  frame_dialog = new FrameDialog(this, item_working->getDialogImage());
  connect(frame_dialog, SIGNAL(ok()), this, SLOT(updateFrame()));
  frame_dialog->show();
}

/*
 * Description: Button slot on the ok button. Emits ok prior to closing the
 *              dialog. The ok gets handled by the widget parent, which
 *              updates the visible items on map prior to accepting changes.
 *
 * Inputs: none
 * Output: none
 */
void ItemDialog::buttonOk()
{
  emit ok();
  close();
}

/*
 * Description: Slot triggered by spinner changing the count of the items on
 *              pickup.
 *
 * Inputs: int count - the number of items on pick up
 * Output: none
 */
void ItemDialog::changedCount(int count)
{
  item_working->setCount(count);
}

/*
 * Description: Slot triggered on the text edit field being changed, which
 *              updates the description.
 *
 * Inputs: QString description - the new description
 * Output: none
 */
void ItemDialog::changedDescription(QString description)
{
  item_working->setDescription(description);
}

/*
 * Description: Slot triggered on the text edit field being changed, which
 *              updates the name.
 *
 * Inputs: QString name - the new thing name
 * Output: none
 */
void ItemDialog::changedName(QString name)
{
  item_working->setName(name);
}

/*
 * Description: Slot triggered on the combo box selection changed, which updates
 *              the stored sound ID within the working object.
 *
 * Inputs: QString text - the new sound row selection
 * Output: none
 */
void ItemDialog::changedSound(const QString &text)
{
  QStringList str_list = text.split(':');

  /* If the list is two long, it is proper format - 001: Sound Example */
  if(str_list.size() >= 2)
  {
    item_working->setSoundID(str_list.front().toInt());
  }
  /* Otherwise, unset the sound ID */
  else
  {
    item_working->setSoundID(-1);
  }
}

/*
 * Description: Slot triggered when the core item drop down changes selection.
 *              Updates the ID in the stored class.
 *
 * Inputs: int index - the index in the list
 * Output: none
 */
// TODO: Remove? Future - not required for new system
void ItemDialog::coreItemChanged(int index)
{
  (void)index;
  //if(index > 0)
  //item_working->setGameID(box_core->currentText().split(":").front().toInt());
  //else
  //item_working->setGameID(-1);
}

/*
 * Description: The drop down for visibility of the thing changed slot. This
 *              updates the visibility in the working item.
 *
 * Inputs: int index - index in the dropdown (only two, true or false)
 * Output: none
 */
void ItemDialog::visibilityChanged(int index)
{
  item_working->setVisibility(index == 1);
}

/*
 * Description: Slot triggers when the walkover drop down changes state. First,
 *              index 0, is false and second is true.
 *
 * Inputs: int index - index in the walkover drop down
 * Output: none
 */
void ItemDialog::walkoverChanged(int index)
{
  item_working->setWalkover(index == 1);
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Returns a list of sounds being used in the item presently.
 *              Used when required for neighboring calls.
 *
 * Inputs: none
 * Output: QList<QString> - the sound string list
 */
QList<QString> ItemDialog::getListSounds()
{
  return sound_list;
}

/*
 * Description: Sets the list of items, used for event creation
 *
 * Inputs: QList<QString> - list of all items (for give item event)
 * Output: none
 */
void ItemDialog::setListItems(QList<QString> items)
{
  list_items = items;
  //box_core->blockSignals(true);
  //box_core->clear();
  //box_core->addItem("None");
  //box_core->addItems(list_items);
  //updateData();
  //box_core->blockSignals(false);
}

/*
 * Description: Sets the list of sounds, to be used within the item dialog for
 *              the drop down selection and within the event.
 *
 * Inputs: QList<QString> sounds - the sound string list
 * Output: none
 */
void ItemDialog::setListSounds(QList<QString> sounds)
{
  /* Replace first element, if "None", with appropriate description for items */
  if(sounds.size() > 0 && sounds.first() == "None")
    sounds[0] = "Game Default";

  /* Set the elements and update the data */
  sound_list = sounds;
  updateData();
}

/*
 * Description: Updates the original item with the working data. Called after
 *              the emit ok() is triggered. Will not change anything if there
 *              is no original item set (or it's NULL).
 *
 * Inputs: none
 * Output: none
 */
void ItemDialog::updateOriginal()
{
  if(item_original != NULL)
    *item_original = *item_working;
}
