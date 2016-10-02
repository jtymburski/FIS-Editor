/*******************************************************************************
 * Class Name: NodeDialog
 * Date Created: April 12, 2015
 * Inheritance: QDialog
 * Description: The NPC node dialog, which handles editing a singular node in
 *              the NPC set. Used by MapDatabase with NPC node view controller.
 ******************************************************************************/
#include "Dialog/NodeDialog.h"
#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function for NPC node dialog. Takes an editing path,
 *              the reference node number in the path, and a parent widget.
 *              Pop-up constructed using EditorNPCPath.
 *
 * Inputs: EditorNPCPath* path - the node path to edit the node data for
 *         int node_num - the node number in the path (0+)
 *         QWidget* parent - the parent widget
 */
NodeDialog::NodeDialog(EditorNPCPath* path, int node_num, QWidget* parent)
          : QDialog(parent)
{
  /* Initialize class variables */
  path_node = node_num;
  path_original = path;
  path_working = new EditorNPCPath();
  if(path_original != NULL)
  {
    *path_working = *path_original;
  }

  /* Create the dialog and fill with data */
  createDialog();
  updateData();
}

/*
 * Description: Destructor function
 */
NodeDialog::~NodeDialog()
{
  path_original = NULL;

  /* Delete working path */
  if(path_working != NULL)
    delete path_working;
  path_working = NULL;
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: Creates the dialog layout with QT functional widgets.
 *
 * Inputs: none
 * Output: none
 */
void NodeDialog::createDialog()
{
  /* Layout setup */
  QGridLayout* layout = new QGridLayout(this);

  /* Node rest delay */
  QLabel* lbl_delay = new QLabel("Delay", this);
  layout->addWidget(lbl_delay, 0, 0, 2, 1, Qt::AlignRight);
  spin_delay = new QSpinBox(this);
  spin_delay->setMinimum(0);
  spin_delay->setMaximum(600000);
  spin_delay->setSuffix(" ms");
  layout->addWidget(spin_delay, 0, 1, 2, 1);

  /* X-Y parse direction flip */
  radio_xy = new QRadioButton("X-Y Flip", this);
  layout->addWidget(radio_xy, 0, 2, 1, 2, Qt::AlignHCenter);
  radio_yx = new QRadioButton("Y-X Flip", this);
  layout->addWidget(radio_yx, 1, 2, 1, 2, Qt::AlignHCenter);

  /* The event */
  event_view = new EventView(NULL, this);
  event_view->setDisabled(true);
  layout->addWidget(event_view, 3, 0, 1, 4);

  /* The button control */
  QPushButton* btn_ok = new QPushButton("Ok", this);
  btn_ok->setDefault(true);
  connect(btn_ok, SIGNAL(clicked()), this, SLOT(buttonOk()));
  layout->addWidget(btn_ok, 4, 1);
  QPushButton* btn_cancel = new QPushButton("Cancel", this);
  connect(btn_cancel, SIGNAL(clicked()), this, SLOT(buttonCancel()));
  layout->addWidget(btn_cancel, 4, 2);
}

/*
 * Description: Updates the data in the widgets. CreateLayout() must be called
 *              prior.
 *
 * Inputs: none
 * Output: none
 */
void NodeDialog::updateData()
{
  /* Add the data if the node is in valid range */
  if(path_node >= -1 && path_node < path_working->getNodes().size())
  {
    /* Set the window title */
    setWindowTitle("Node #" + QString::number(path_node) + " - X:" +
                   QString::number(path_working->getNodeX(path_node)) + " Y:" +
                   QString::number(path_working->getNodeY(path_node)));

    /* Set the delay */
    spin_delay->setValue(path_working->getNodeDelay(path_node));

    /* Set the X-Y radio buttons */
    if(path_working->getNodeXYFlip(path_node))
      radio_yx->setChecked(true);
    else
      radio_xy->setChecked(true);
  }
  /* Otherwise, disable all editable values */
  else
  {
    /* Set the window title */
    setWindowTitle("Invalid Node #" + QString::number(path_node));

    /* Disable widgets */
    spin_delay->setDisabled(true);
    radio_xy->setDisabled(true);
    radio_yx->setDisabled(true);
  }
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: The re-implementation of the close event for the dialog. Not
 *              used currently.
 *
 * Inputs: QCloseEvent* - not used
 * Output: none
 */
void NodeDialog::closeEvent(QCloseEvent*)
{
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
void NodeDialog::buttonCancel()
{
  reject();
}

/*
 * Description: Button slot on the ok button. Emits success prior to closing the
 *              dialog. The ok gets handled by the widget parent, which
 *              updates the visible path on map prior to accepting changes.
 *
 * Inputs: none
 * Output: none
 */
void NodeDialog::buttonOk()
{
  /* Sets the data if the node is in valid range */
  if(path_node >= 0 && path_node < path_working->getNodes().size())
  {
    path_original->editNode(path_node, path_original->getNodeX(path_node),
                            path_original->getNodeY(path_node),
                            spin_delay->value(), radio_yx->isChecked());
    emit success();
  }
  else if(path_node == -1)
  {
    path_original->editStartNode(spin_delay->value(), radio_yx->isChecked());
    emit success();
  }

  accept();
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/ 
