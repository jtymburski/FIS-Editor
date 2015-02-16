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

/* Constructor function */
ThingDialog::ThingDialog(EditorThing* edit_thing, QWidget* parent)
           : QDialog(parent)
{
  frame_dialog = NULL;
  waiting_for_submap = false;

  /* Set-up the thing set - copied to working for changes */
  thing_original = edit_thing;
  thing_working = new EditorThing();
  if(thing_original != NULL)
  {
    *thing_working = *thing_original;
    event_ctrl = new EditorEvent(
                           EventHandler::copyEvent(thing_original->getEvent()));
  }
  else
  {
    event_ctrl = new EditorEvent(EventHandler::createEventTemplate());
  }

  /* Layout setup */
  createLayout();
  updateData();
  updateFrame();

  // TODO: TESTING - REMOVE
  Conversation test;
  test.action_event = EventHandler::createEventTemplate();
  test.text = "HI";
  test.thing_id = 1;
  convo_dialog = new ConvoDialog(&test, false, this);
  //convo_dialog->show();
}

/* Destructor function */
ThingDialog::~ThingDialog()
{
  thing_original = NULL;

  /* Delete event controller */
  event_view->setEvent(NULL);
  delete event_ctrl;
  event_ctrl = NULL;

  /* Delete working thing */
  if(thing_working != NULL)
    delete thing_working;
  thing_working = NULL;
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* Creates the layout - only called on initial construction */
void ThingDialog::createLayout()
{
  /* Layout setup */
  QGridLayout* layout = new QGridLayout(this);
  layout->setSizeConstraint(QLayout::SetFixedSize);
  layout->setColumnStretch(2, 1);

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
  layout->addWidget(line_name, 1, 1, 1, 2);

  /* The visibility widget */
  QLabel* lbl_visible = new QLabel("Visible:", this);
  layout->addWidget(lbl_visible, 2, 0);
  box_visible = new QComboBox(this);
  box_visible->addItem("False");
  box_visible->addItem("True");
  connect(box_visible, SIGNAL(currentIndexChanged(int)),
          this, SLOT(visibilityChanged(int)));
  layout->addWidget(box_visible, 2, 1);

  /* The description widget */
  QLabel* lbl_description = new QLabel("Description:", this);
  layout->addWidget(lbl_description, 3, 0);
  line_description = new QLineEdit("", this);
  connect(line_description, SIGNAL(textEdited(QString)),
          this, SLOT(changedDescription(QString)));
  layout->addWidget(line_description, 3, 1, 1, 5);

  /* The sprite view widget */
  QLabel* lbl_frame = new QLabel("Dialog Image:", this);
  layout->addWidget(lbl_frame, 4, 0, 2, 1);
  lbl_frame_img = new QLabel(this);
  lbl_frame_img->setMinimumSize(200, 200);
  lbl_frame_img->setStyleSheet("border: 1px solid black");
  lbl_frame_img->setAlignment(Qt::AlignCenter);
  layout->addWidget(lbl_frame_img, 4, 1, 2, 3);
  QPushButton* btn_frame_click = new QPushButton(this);
  btn_frame_click->setIcon(QIcon(":/images/icons/32_settings.png"));
  btn_frame_click->setIconSize(QSize(24,24));
  btn_frame_click->setMaximumSize(30, 30);
  connect(btn_frame_click, SIGNAL(clicked()), this, SLOT(buttonFrameEdit()));
  layout->addWidget(btn_frame_click, 4, 3, 2, 1, Qt::AlignTop);

  /* Event View */
  event_view = new EventView(event_ctrl, this);
  layout->addWidget(event_view, 6, 0, 2, 4, Qt::AlignBottom);
  connect(event_view, SIGNAL(selectTile()), this, SLOT(selectTile()));

  /* Matrix View */
  matrix_view = new MatrixView(thing_working->getMatrix(), this);
  layout->addWidget(matrix_view, 4, 4, 4, 4);

  /* The button control */
  layout->setRowMinimumHeight(8, 15);
  QPushButton* btn_ok = new QPushButton("Ok", this);
  connect(btn_ok, SIGNAL(clicked()), this, SLOT(buttonOk()));
  layout->addWidget(btn_ok, 9, 6);
  QPushButton* btn_cancel = new QPushButton("Cancel", this);
  connect(btn_cancel, SIGNAL(clicked()), this, SLOT(buttonCancel()));
  layout->addWidget(btn_cancel, 9, 7);

  /* Dialog control */
  setWindowTitle("Thing Edit");
}

/* Updates the objects with the thing data */
void ThingDialog::updateData()
{
  line_description->setText(thing_working->getDescription());
  line_name->setText(thing_working->getName());
  if(thing_working->isVisible())
    box_visible->setCurrentIndex(1);
  else
    box_visible->setCurrentIndex(0);
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
  
/* Custom close event */
void ThingDialog::closeEvent(QCloseEvent* event)
{
  event_ctrl->setEventBlank();
  if(thing_working != NULL)
    delete thing_working;
  thing_working = NULL;

  event->accept();
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

/* Button control triggers */
void ThingDialog::buttonCancel()
{
  close();
}

/* Button control triggers */
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

/* Button control triggers */
void ThingDialog::buttonOk()
{
  if(thing_original != NULL)
  {
    *thing_original = *thing_working;
    thing_original->setEvent(*event_ctrl->getEvent());
    event_ctrl->setEventBlank(false);
    emit ok();
    close();
  }
}

/* Changed text in line edits */
void ThingDialog::changedDescription(QString description)
{
  thing_working->setDescription(description);
}

/* Changed text in line edits */
void ThingDialog::changedName(QString name)
{
  thing_working->setName(name);
}

/* Update the frame for the thing */
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

/* Select tile trigger */
void ThingDialog::selectTile()
{
  waiting_for_submap = true;
  hide();
  emit selectTile(EditorEnumDb::THING_VIEW);
}

/* Visibility status changed */
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

/* Returns the event view widget */
EventView* ThingDialog::getEventView()
{
  return event_view;
}

/* Update the selected tile for the thing */
void ThingDialog::updateSelectedTile(int id, int x, int y)
{
  waiting_for_submap = false;
  show();
  event_view->updateSelectedTile(id, x, y);
}
