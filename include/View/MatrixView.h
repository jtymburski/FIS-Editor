/*******************************************************************************
 * Class Name: MatrixView
 * Date Created: February 8, 2015
 * Inheritance: QWidget
 * Description: The view controller that takes a matrix and allows for control
 *              and viewing of it.
 ******************************************************************************/
#ifndef MATRIXVIEW_H
#define MATRIXVIEW_H

#include <QFileDialog>
#include <QGraphicsView>
#include <QGridLayout>
#include <QMenu>
#include <QPushButton>
#include <QWidget>

#include "Database/EditorMatrix.h"
#include "Dialog/MatrixDialog.h"
#include "Dialog/SpriteDialog.h"

class MatrixView : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  MatrixView(EditorMatrix* matrix = NULL, QWidget* parent = NULL);

  /* Destructor function */
  ~MatrixView();

private:
  /* Pen control buttons */
  QPushButton* button_pen_erase;
  QPushButton* button_pen_minus;
  QPushButton* button_pen_place;
  QPushButton* button_pen_plus;

  /* Sprite edit control */
  QPushButton* button_sprite_edit;

  /* View control buttons */
  QPushButton* button_view_grid;
  QPushButton* button_view_pass;
  QPushButton* button_view_render;

  /* The matrix being displayed */
  EditorMatrix* matrix;

  /* The matrix initiation dialog */
  MatrixDialog* matrix_dialog;

  /* The viewport for the matrix */
  QGraphicsView* matrix_view;

  /* Right click menu on matrix */
  QMenu* rightclick_menu;

  /* The sprite dialog for editing */
  SpriteDialog* sprite_dialog;

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* Height control */
  void buttonHeightDecrease();
  void buttonHeightIncrease();

  /* Pen click trigger */
  void buttonPen(bool checked);

  /* General sprite edit trigger */
  void buttonSpriteEdit();

  /* View button triggers */
  void buttonViewGrid(bool checked);
  void buttonViewPass(bool checked);
  void buttonViewRender(bool checked);

  /* Width control */
  void buttonWidthDecrease();
  void buttonWidthIncrease();

  /* Place matrix control start */
  void initMatrixPlace();

  /* Right click on sprite trigger */
  void rightClickTrigger(EditorTileSprite* sprite);

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
