/*******************************************************************************
 * Class Name: MatrixView
 * Date Created: February 8, 2015
 * Inheritance: QFrame
 * Description: The view controller that takes a matrix and allows for control
 *              and viewing of it.
 ******************************************************************************/
#ifndef MATRIXVIEW_H
#define MATRIXVIEW_H

#include <QFileDialog>
#include <QFrame>
#include <QGraphicsView>
#include <QGridLayout>
#include <QInputDialog>
#include <QMenu>
#include <QPushButton>

#include "Database/EditorMatrix.h"
#include "Dialog/MatrixDialog.h"
#include "Dialog/SpriteDialog.h"

class MatrixView : public QFrame
{
  Q_OBJECT
public:
  /* Constructor Function */
  MatrixView(EditorMatrix* matrix = NULL, QWidget* parent = NULL);

  /* Destructor function */
  ~MatrixView();

private:
  /* Depth control action right click */
  QAction* action_depth;

  /* The action control for the right click */
  QAction* action_pass_e;
  QAction* action_pass_n;
  QAction* action_pass_s;
  QAction* action_pass_w;

  /* Frame control for viewing */
  QPushButton* button_frame_next;
  QPushButton* button_frame_prev;

  /* Pen control buttons */
  QPushButton* button_pen_erase;
  QPushButton* button_pen_minus;
  QPushButton* button_pen_pass;
  QPushButton* button_pen_place;
  QPushButton* button_pen_plus;

  /* Sprite edit control */
  QPushButton* button_sprite_edit;

  /* View control buttons */
  QPushButton* button_view_grid;
  QPushButton* button_view_pass;
  QPushButton* button_view_render;

  /* Frame number in matrix */
  QLabel* lbl_frame_num;

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
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Sets the active viewing frame */
  bool setActiveFrame(int index);

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* Active sprite passability control */
  void actionPassabilityEast(bool set);
  void actionPassabilityNorth(bool set);
  void actionPassabilitySouth(bool set);
  void actionPassabilityWest(bool set);

  /* Action render depth editor */
  void actionRenderDepth();

  /* Actions for sprite control and view */
  void actionSpriteEdit();
  void actionSpriteView();

  /* Frame view control */
  void buttonFrameNext();
  void buttonFramePrev();

  /* Height control */
  void buttonHeightDecrease();
  void buttonHeightIncrease();

  /* Pen click trigger */
  void buttonPen(bool checked);

  /* General sprite edit trigger */
  void buttonSpriteEdit();

  /* Trim the matrix */
  void buttonTrim();

  /* View button triggers */
  void buttonViewGrid(bool checked);
  void buttonViewPass(bool checked);
  void buttonViewRender(bool checked);

  /* Width control */
  void buttonWidthDecrease();
  void buttonWidthIncrease();

  /* Place matrix control start */
  void initMatrixPlace();

  /* Matrix was changed */
  void matrixChanged();

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
