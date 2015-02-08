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
  /* Initialize variables */
  thing_original = NULL;
  thing_working = NULL;

  /* Layout setup */
  QGridLayout* layout = new QGridLayout(this);
  layout->setSizeConstraint(QLayout::SetFixedSize);

  /* Try and set-up view with edit thing matrix */
  // TODO: Fix - more like SpriteDialog
  if(edit_thing != NULL)
    matrix_view = new MatrixView(edit_thing->getMatrix(), this);
  else
    matrix_view = new MatrixView(NULL, this);
  layout->addWidget(matrix_view, 0, 0);
}

/* Destructor function */
ThingDialog::~ThingDialog()
{
  thing_original = NULL;
  thing_working = NULL;
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
  
/* Custom close event */
void ThingDialog::closeEvent(QCloseEvent *)
{

}
