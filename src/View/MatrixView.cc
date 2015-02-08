/*******************************************************************************
 * Class Name: MatrixView
 * Date Created: February 8, 2015
 * Inheritance: QWidget
 * Description: The view controller that takes a matrix and allows for control
 *              and viewing of it.
 ******************************************************************************/
#include "View/MatrixView.h"
#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor Function */
MatrixView::MatrixView(EditorMatrix* matrix, QWidget* parent) : QWidget(parent)
{
  /* Initialize variables */
  this->matrix = matrix;

  /* Layout setup */
  QGridLayout* layout = new QGridLayout(this);
  layout->setSizeConstraint(QLayout::SetFixedSize);

  /* Set up the view */
  matrix_view = new QGraphicsView(matrix, this);
  layout->addWidget(matrix_view, 0, 0);
}

/* Destructor function */
MatrixView::~MatrixView()
{
  matrix_view->setScene(NULL);
  matrix = NULL;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
  
/* Returns the matrix being viewed */
EditorMatrix* MatrixView::getMatrix()
{
  return matrix;
}

/* Sets a different matrix to the active view */
void MatrixView::setMatrix(EditorMatrix* matrix)
{
  matrix_view->setScene(matrix);
  this->matrix = matrix;
  update();
}
