/*******************************************************************************
 * Class Name: MatrixView
 * Date Created: February 8, 2015
 * Inheritance: QWidget
 * Description: The view controller that takes a matrix and allows for control
 *              and viewing of it.
 ******************************************************************************/
#ifndef MATRIXVIEW_H
#define MATRIXVIEW_H

#include <QGraphicsView>
#include <QGridLayout>
#include <QWidget>

#include "Database/EditorMatrix.h"

class MatrixView : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  MatrixView(EditorMatrix* matrix = NULL, QWidget* parent = NULL);

  /* Destructor function */
  ~MatrixView();

private:
  /* The matrix being displayed */
  EditorMatrix* matrix;

  /* The viewport for the matrix */
  QGraphicsView* matrix_view;

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the matrix being viewed */
  EditorMatrix* getMatrix();

  /* Sets a different matrix to the active view */
  void setMatrix(EditorMatrix* matrix);
};
#endif // SpriteView_H
