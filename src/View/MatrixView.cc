/*******************************************************************************
 * Class Name: MatrixView
 * Date Created: February 8, 2015
 * Inheritance: QFrame
 * Description: The view controller that takes a matrix and allows for control
 *              and viewing of it.
 ******************************************************************************/
#include "View/MatrixView.h"
#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Primary constructor function. Sets up the layout, with a given
 *              matrix.
 *
 * Inputs: EditorMatrix* matrix - the matrix to display in the view
 *         QWidget* parent - parent widget
 */
MatrixView::MatrixView(EditorMatrix* matrix, QWidget* parent) : QFrame(parent)
{
  /* Initialize variables */
  this->matrix = NULL;
  last_path = EditorHelpers::getSpriteDir();
  matrix_dialog = NULL;
  playing = false;
  sprite_dialog = NULL;
  int icon_size = 24;
  int button_size = icon_size + 6;

  /* Set up the timer */
  playing_timer = new QTimer(this);
  playing_timer->setSingleShot(false);
  connect(playing_timer, SIGNAL(timeout()), this, SLOT(animateNext()));

  /* Layout setup */
  QGridLayout* layout = new QGridLayout(this);
  layout->setSizeConstraint(QLayout::SetFixedSize);

  /* Set up the view */
  matrix_view = new QGraphicsView(this);
  int size = EditorHelpers::getTileSize() * 5 +
             EditorHelpers::getTileSize() / 2;
  matrix_view->setMinimumSize(size, size);
  matrix_view->setMaximumSize(size, size);
  layout->addWidget(matrix_view, 1, 1, 10, 10);

  /* Set up view push buttons */
  button_view_grid = new QPushButton(this);
  button_view_grid->setCheckable(true);
  button_view_grid->setChecked(true);
  button_view_grid->setIcon(QIcon(":/images/icons/32_grid.png"));
  button_view_grid->setIconSize(QSize(icon_size,icon_size));
  button_view_grid->setMaximumSize(button_size, button_size);
  connect(button_view_grid, SIGNAL(toggled(bool)),
          this, SLOT(buttonViewGrid(bool)));
  layout->addWidget(button_view_grid, 0, 1);
  button_view_pass = new QPushButton(this);
  button_view_pass->setCheckable(true);
  button_view_pass->setIcon(QIcon(":/images/icons/32_pass_vis.png"));
  button_view_pass->setIconSize(QSize(icon_size,icon_size));
  button_view_pass->setMaximumSize(button_size, button_size);
  connect(button_view_pass, SIGNAL(toggled(bool)),
          this, SLOT(buttonViewPass(bool)));
  layout->addWidget(button_view_pass, 0, 2);
  button_view_render = new QPushButton(this);
  button_view_render->setCheckable(true);
  button_view_render->setIcon(QIcon(":/images/icons/32_render_vis.png"));
  button_view_render->setIconSize(QSize(icon_size,icon_size));
  button_view_render->setMaximumSize(button_size, button_size);
  connect(button_view_render, SIGNAL(toggled(bool)),
          this, SLOT(buttonViewRender(bool)));
  layout->addWidget(button_view_render, 0, 3);

  /* Set up the pen push buttons */
  button_pen_place = new QPushButton(this);
  button_pen_place->setCheckable(true);
  button_pen_place->setChecked(true);
  button_pen_place->setAutoExclusive(true);
  button_pen_place->setIcon(QIcon(":/images/icons/32_pencil.png"));
  button_pen_place->setIconSize(QSize(icon_size,icon_size));
  button_pen_place->setMaximumSize(button_size, button_size);
  connect(button_pen_place, SIGNAL(toggled(bool)), this, SLOT(buttonPen(bool)));
  layout->addWidget(button_pen_place, 1, 0);
  button_pen_erase = new QPushButton(this);
  button_pen_erase->setCheckable(true);
  button_pen_erase->setAutoExclusive(true);
  button_pen_erase->setIcon(QIcon(":/images/icons/32_eraser.png"));
  button_pen_erase->setIconSize(QSize(icon_size,icon_size));
  button_pen_erase->setMaximumSize(button_size, button_size);
  connect(button_pen_erase, SIGNAL(toggled(bool)), this, SLOT(buttonPen(bool)));
  layout->addWidget(button_pen_erase, 2, 0);
  button_pen_plus = new QPushButton(this);
  button_pen_plus->setCheckable(true);
  button_pen_plus->setAutoExclusive(true);
  button_pen_plus->setIcon(QIcon(":/images/icons/32_depth_up.png"));
  button_pen_plus->setIconSize(QSize(icon_size,icon_size));
  button_pen_plus->setMaximumSize(button_size, button_size);
  connect(button_pen_plus, SIGNAL(toggled(bool)), this, SLOT(buttonPen(bool)));
  layout->addWidget(button_pen_plus, 3, 0);
  button_pen_minus = new QPushButton(this);
  button_pen_minus->setCheckable(true);
  button_pen_minus->setAutoExclusive(true);
  button_pen_minus->setIcon(QIcon(":/images/icons/32_depth_down.png"));
  button_pen_minus->setIconSize(QSize(icon_size,icon_size));
  button_pen_minus->setMaximumSize(button_size, button_size);
  connect(button_pen_minus, SIGNAL(toggled(bool)), this, SLOT(buttonPen(bool)));
  layout->addWidget(button_pen_minus, 4, 0);
  button_pen_pass = new QPushButton(this);
  button_pen_pass->setCheckable(true);
  button_pen_pass->setAutoExclusive(true);
  button_pen_pass->setIcon(QIcon(":/images/icons/32_passA.png"));
  button_pen_pass->setIconSize(QSize(icon_size,icon_size));
  button_pen_pass->setMaximumSize(button_size, button_size);
  connect(button_pen_pass, SIGNAL(toggled(bool)), this, SLOT(buttonPen(bool)));
  layout->addWidget(button_pen_pass, 5, 0);

  /* Sprite setting button */
  button_sprite_edit = new QPushButton(this);
  button_sprite_edit->setIcon(QIcon(":/images/icons/32_settings.png"));
  button_sprite_edit->setIconSize(QSize(icon_size,icon_size));
  button_sprite_edit->setMaximumSize(button_size, button_size);
  connect(button_sprite_edit, SIGNAL(clicked()),
          this, SLOT(buttonSpriteEdit()));
  layout->addWidget(button_sprite_edit, 0, 10);

  /* Increase/decrease width buttons */
  QPushButton* button_width_minus = new QPushButton(this);
  button_width_minus->setIcon(QIcon(":/images/icons/32_minus.png"));
  button_width_minus->setIconSize(QSize(icon_size,icon_size));
  button_width_minus->setMaximumSize(button_size, button_size);
  connect(button_width_minus, SIGNAL(clicked()),
          this, SLOT(buttonWidthDecrease()));
  layout->addWidget(button_width_minus, 11, 9);
  QPushButton* button_width_plus = new QPushButton(this);
  button_width_plus->setIcon(QIcon(":/images/icons/32_plus.png"));
  button_width_plus->setIconSize(QSize(icon_size,icon_size));
  button_width_plus->setMaximumSize(button_size, button_size);
  connect(button_width_plus, SIGNAL(clicked()),
          this, SLOT(buttonWidthIncrease()));
  layout->addWidget(button_width_plus, 11, 10);

  /* Increase/decrease height buttons */
  QPushButton* button_height_minus = new QPushButton(this);
  button_height_minus->setIcon(QIcon(":/images/icons/32_minus.png"));
  button_height_minus->setIconSize(QSize(icon_size,icon_size));
  button_height_minus->setMaximumSize(button_size, button_size);
  connect(button_height_minus, SIGNAL(clicked()),
          this, SLOT(buttonHeightDecrease()));
  layout->addWidget(button_height_minus, 9, 11);
  QPushButton* button_height_plus = new QPushButton(this);
  button_height_plus->setIcon(QIcon(":/images/icons/32_plus.png"));
  button_height_plus->setIconSize(QSize(icon_size,icon_size));
  button_height_plus->setMaximumSize(button_size, button_size);
  connect(button_height_plus, SIGNAL(clicked()),
          this, SLOT(buttonHeightIncrease()));
  layout->addWidget(button_height_plus, 10, 11);

  /* Trim scene button */
  QPushButton* button_trim = new QPushButton(this);
  button_trim->setIcon(QIcon(":/images/icons/32_collapse.png"));
  button_trim->setIconSize(QSize(icon_size,icon_size));
  button_trim->setMaximumSize(button_size, button_size);
  connect(button_trim, SIGNAL(clicked()),
          this, SLOT(buttonTrim()));
  layout->addWidget(button_trim, 11, 11);

  /* Scene control buttons */
  button_play = new QPushButton(this);
  button_play->setIcon(QIcon(":/images/icons/32_play.png"));
  button_play->setIconSize(QSize(icon_size, icon_size));
  button_play->setMaximumSize(button_size, button_size);
  connect(button_play, SIGNAL(clicked()), this, SLOT(buttonRunAnimation()));
  layout->addWidget(button_play, 11, 0);
  button_frame_prev = new QPushButton("<", this);
  button_frame_prev->setMaximumSize(button_size, button_size);
  connect(button_frame_prev, SIGNAL(clicked()), this, SLOT(buttonFramePrev()));
  layout->addWidget(button_frame_prev, 11, 1);//, Qt::AlignRight);
  lbl_frame_num = new QLabel("0", this);
  layout->addWidget(lbl_frame_num, 11, 2, Qt::AlignCenter);
  button_frame_next = new QPushButton(">", this);
  button_frame_next->setMaximumSize(button_size, button_size);
  connect(button_frame_next, SIGNAL(clicked()), this, SLOT(buttonFrameNext()));
  layout->addWidget(button_frame_next, 11, 3);//, Qt::AlignLeft);

  /* Set up the right click menu */
  rightclick_menu = new QMenu("Sprite Edit", this);
  QAction* action_edit = new QAction("Edit Sprite", rightclick_menu);
  connect(action_edit, SIGNAL(triggered()), this, SLOT(actionSpriteEdit()));
  rightclick_menu->addAction(action_edit);
  QAction* action_view = new QAction("View Sprite", rightclick_menu);
  connect(action_view, SIGNAL(triggered()), this, SLOT(actionSpriteView()));
  rightclick_menu->addAction(action_view);
  action_depth = new QAction("Render Depth", rightclick_menu);
  connect(action_depth, SIGNAL(triggered()), this, SLOT(actionRenderDepth()));
  rightclick_menu->addAction(action_depth);
  action_pass_n = new QAction("Pass N", rightclick_menu);
  action_pass_n->setCheckable(true);
  connect(action_pass_n, SIGNAL(toggled(bool)),
          this, SLOT(actionPassabilityNorth(bool)));
  rightclick_menu->addAction(action_pass_n);
  action_pass_e = new QAction("Pass E", rightclick_menu);
  action_pass_e->setCheckable(true);
  connect(action_pass_e, SIGNAL(toggled(bool)),
          this, SLOT(actionPassabilityEast(bool)));
  rightclick_menu->addAction(action_pass_e);
  action_pass_s = new QAction("Pass S", rightclick_menu);
  action_pass_s->setCheckable(true);
  connect(action_pass_s, SIGNAL(toggled(bool)),
          this, SLOT(actionPassabilitySouth(bool)));
  rightclick_menu->addAction(action_pass_s);
  action_pass_w = new QAction("Pass W", rightclick_menu);
  action_pass_w->setCheckable(true);
  connect(action_pass_w, SIGNAL(toggled(bool)),
          this, SLOT(actionPassabilityWest(bool)));
  rightclick_menu->addAction(action_pass_w);
  rightclick_menu->hide();

  /* Configure parent widget */
  setFrameStyle(QFrame::Panel);
  setLineWidth(1);

  /* Sets the matrix */
  setMatrix(matrix);
}

/*
 * Description: Destructor function
 */
MatrixView::~MatrixView()
{
  matrix_view->setScene(NULL);
  matrix = NULL;
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: Sets the active frame index for displaying which snapshot of
 *              the thing is visible. Also, controls enabling/disabling
 *              the control buttons at the bottom for going forward and
 *              backward (depending on the number of frames available).
 *
 * Inputs: int index - the new frame index
 * Output: bool - true if the end of the frame list is reached
 */
bool MatrixView::setActiveFrame(int index)
{
  int min = 0;
  int max = matrix->getTrimFrames();
  if(max < 0)
    max = 0;

  /* Make sure the index is in range */
  if(index > max)
    index = max;
  else if(index < min)
    index = min;

  /* Sets the new index for the matrix */
  matrix->setActiveFrame(index, true);
  lbl_frame_num->setText(QString::number(index));

  /* Enable/disable control for previous button */
  if(index == min)
    button_frame_prev->setDisabled(true);
  else
    button_frame_prev->setEnabled(true);

  /* Enable/disable control for next button */
  if(index == max)
    button_frame_next->setDisabled(true);
  else
    button_frame_next->setEnabled(true);

  /* If both buttons are disabled, disable play button */
  if(button_frame_prev->isEnabled() || button_frame_next->isEnabled())
    button_play->setEnabled(true);
  else
    button_play->setEnabled(false);


  /* If it's min or max, return true - indicating end is reached */
  if(index == min || index == max)
    return true;
  return false;
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

/*
 * Description: Slot which sets the passability in the east direction. Connected
 *              to the right click menu east passability toggle.
 *
 * Inputs: bool set - true if passability should be enabled. false otherwise.
 * Output: none
 */
void MatrixView::actionPassabilityEast(bool set)
{
  if(matrix != NULL)
  {
    EditorTileSprite* sprite = matrix->getRightClicked();
    if(sprite != NULL)
      sprite->setPassability(Direction::EAST, set);

    stopAnimation();
  }
}

/*
 * Description: Slot which sets the passability in the north direction.
 *              Connected to the right click menu north passability toggle.
 *
 * Inputs: bool set - true if passability should be enabled. false otherwise.
 * Output: none
 */
void MatrixView::actionPassabilityNorth(bool set)
{
  if(matrix != NULL)
  {
    EditorTileSprite* sprite = matrix->getRightClicked();
    if(sprite != NULL)
      sprite->setPassability(Direction::NORTH, set);

    stopAnimation();
  }
}

/*
 * Description: Slot which sets the passability in the south direction.
 *              Connected to the right click menu south passability toggle.
 *
 * Inputs: bool set - true if passability should be enabled. false otherwise.
 * Output: none
 */
void MatrixView::actionPassabilitySouth(bool set)
{
  if(matrix != NULL)
  {
    EditorTileSprite* sprite = matrix->getRightClicked();
    if(sprite != NULL)
      sprite->setPassability(Direction::SOUTH, set);

    stopAnimation();
  }
}

/*
 * Description: Slot which sets the passability in the west direction.
 *              Connected to the right click menu west passability toggle.
 *
 * Inputs: bool set - true if passability should be enabled. false otherwise.
 * Output: none
 */
void MatrixView::actionPassabilityWest(bool set)
{
  if(matrix != NULL)
  {
    EditorTileSprite* sprite = matrix->getRightClicked();
    if(sprite != NULL)
      sprite->setPassability(Direction::WEST, set);

    stopAnimation();
  }
}

/*
 * Description: Slot which sets the render depth of the matrix tile. Connected
 *              to the right click menu and opens up a QInputDialog to enter
 *              the new render depth.
 *
 * Inputs: none
 * Output: none
 */
void MatrixView::actionRenderDepth()
{
  if(matrix != NULL)
  {
    EditorTileSprite* sprite = matrix->getRightClicked();
    if(sprite != NULL)
    {
      bool ok;
      int result = QInputDialog::getInt(this, tr("Edit Tile Render Depth"),
                                        tr("Render Depth:"),
                                        sprite->getRenderDepth(), 0, 9, 1, &ok);
      if(ok)
        sprite->setRenderDepth(result);

      stopAnimation();
    }
  }
}

/*
 * Description: Slot which opens up the edit sprite dialog for the individual
 *              matrix tile sprite. Connected to the right click menu.
 *
 * Inputs: none
 * Output: none
 */
void MatrixView::actionSpriteEdit()
{
  if(matrix != NULL)
  {
    EditorTileSprite* sprite = matrix->getRightClicked();
    if(sprite != NULL)
    {
      /* Close and delete the dialog if button pressed */
      if(sprite_dialog != NULL)
      {
        sprite_dialog->hide();
        disconnect(sprite_dialog, SIGNAL(ok()), matrix, SLOT(editSpritesOk()));
        disconnect(sprite_dialog, SIGNAL(ok()), this, SLOT(matrixChanged()));
        delete sprite_dialog;
        sprite_dialog = NULL;
      }

      /* Open the dialog to edit */
      sprite_dialog = new SpriteDialog(this, sprite, "", 0, false,
                                       EditorEnumDb::SPRITE_FRAMES);
      connect(sprite_dialog, SIGNAL(ok()), this, SLOT(matrixChanged()));
      sprite_dialog->show();

      stopAnimation();
    }
  }
}

/*
 * Description: Slot which opens up a QDialog with the view of each frame in
 *              the matrix tile. Connected to the right click menu.
 *
 * Inputs: none
 * Output: none
 */
void MatrixView::actionSpriteView()
{
  if(matrix != NULL)
  {
    EditorTileSprite* sprite = matrix->getRightClicked();
    int size = EditorHelpers::getTileSize();

    if(sprite != NULL)
    {
      QDialog* frames = new QDialog(this);
      QHBoxLayout* framelayout = new QHBoxLayout(frames);
      QVector<QLabel*> labels;

      /* Fill with the frames */
      for(int i = 0; i < sprite->frameCount(); i++)
      {
        labels.push_back(new QLabel(frames));
        labels.last()->setFixedSize(size, size);
        labels.last()->setPixmap(sprite->getPixmap(i, size, size));
        framelayout->addWidget(labels.last());
      }

      /* Finalize dialog */
      frames->setLayout(framelayout);
      frames->setModal(false);
      frames->setWindowTitle(tr("Sprite View"));
      frames->exec();
    }

    stopAnimation();
  }
}

/*
 * Description: Animate to the next frame. Loops around when it reaches the
 *              end.
 *
 * Inputs: none
 * Output: none
 */
void MatrixView::animateNext()
{
  if(button_frame_next->isEnabled())
  {
    buttonFrameNext();
  }
  else
  {
    while(button_frame_prev->isEnabled())
      buttonFramePrev();
  }
}

/*
 * Description: The button which shifts the frames of the matrix to the next to
 *              be displayed.
 *
 * Inputs: none
 * Output: none
 */
void MatrixView::buttonFrameNext()
{
  if(matrix != NULL)
    setActiveFrame(matrix->getActiveFrameIndex() + 1);
}

/*
 * Description: The button which shifts the frames of the matrix to the
 *              previous to be displayed.
 *
 * Inputs: none
 * Output: none
 */
void MatrixView::buttonFramePrev()
{
  if(matrix != NULL)
    setActiveFrame(matrix->getActiveFrameIndex() - 1);
}

/*
 * Description: The button which triggers to decrease the height of the matrix
 *              by one tile sprite.
 *
 * Inputs: none
 * Output: none
 */
void MatrixView::buttonHeightDecrease()
{
  if(matrix != NULL)
  {
    matrix->decreaseHeight();
    if(matrix->getWidth() == 0)
      button_sprite_edit->setDisabled(true);

    stopAnimation();
  }
}

/*
 * Description: The button which triggers to increase the height of the matrix
 *              by one tile sprite.
 *
 * Inputs: none
 * Output: none
 */
void MatrixView::buttonHeightIncrease()
{
  if(matrix != NULL)
  {
    matrix->increaseHeight();
    if(matrix->getWidth() > 0)
      button_sprite_edit->setEnabled(true);

    stopAnimation();
  }
}

/*
 * Description: Triggers on any of the 5 pen buttons being pressed and sets
 *              the current cursor mode.
 *
 * Inputs: bool checked - true if the button has been pressed
 * Output: none
 */
void MatrixView::buttonPen(bool checked)
{
  if(checked && matrix != NULL)
  {
    if(button_pen_place->isChecked())
      matrix->setCursorMode(EditorEnumDb::THING_ADD);
    else if(button_pen_erase->isChecked())
      matrix->setCursorMode(EditorEnumDb::THING_REMOVE);
    else if(button_pen_plus->isChecked())
      matrix->setCursorMode(EditorEnumDb::THING_RENDER_PLUS);
    else if(button_pen_minus->isChecked())
      matrix->setCursorMode(EditorEnumDb::THING_RENDER_MINUS);
    else if(button_pen_pass->isChecked())
      matrix->setCursorMode(EditorEnumDb::THING_PASS_ALL);

    stopAnimation();
  }
}

/*
 * Description: Runs the animation for the matrix (runs through each frame)
 *
 * Inputs: none
 * Output: none
 */
void MatrixView::buttonRunAnimation()
{
  if(playing)
    stopAnimation();
  else
    startAnimation();
}

/*
 * Description: The button which triggers to edit the settings for all sprites
 *              in the matrix (such as opacity, brightness, etc).
 *
 * Inputs: none
 * Output: none
 */
void MatrixView::buttonSpriteEdit()
{
  /* Instantiate the edit */
  if(matrix != NULL)
  {
    /* Close and delete the dialog if button pressed */
    if(sprite_dialog != NULL)
    {
      sprite_dialog->hide();
      disconnect(sprite_dialog, SIGNAL(ok()), matrix, SLOT(editSpritesOk()));
      disconnect(sprite_dialog, SIGNAL(ok()), this, SLOT(matrixChanged()));
      delete sprite_dialog;
      sprite_dialog = NULL;
    }

    /* Try to grab the sprite - if not null, open dialog */
    EditorSprite* sprite = matrix->editAllSprites();
    if(sprite != NULL)
    {
      sprite_dialog = new SpriteDialog(this, sprite, "", 0, false,
                                       EditorEnumDb::SPRITE_DATA);
      connect(sprite_dialog, SIGNAL(ok()), matrix, SLOT(editSpritesOk()));
      sprite_dialog->show();
    }

    stopAnimation();
  }
}

/*
 * Description: Button which triggers a matrix trim and cleans up all tile
 *              rows and columns on all edges.
 *
 * Inputs: none
 * Output: none
 */
void MatrixView::buttonTrim()
{
  if(matrix != NULL)
    matrix->trim(false);

  stopAnimation();
}

/*
 * Description: Button to toggle if the grid is visible in the matrix view.
 *
 * Inputs: bool checked - true if the grid is visible
 * Output: none
 */
void MatrixView::buttonViewGrid(bool checked)
{
  if(matrix != NULL)
    matrix->setVisibilityGrid(checked);

  stopAnimation();
}

/*
 * Description: Button to toggle if the passability is visible in the matrix
 *              view. This is only visible if the render depth is 0 on the
 *              tile.
 *
 * Inputs: bool checked - true if the passability is visible
 * Output: none
 */
void MatrixView::buttonViewPass(bool checked)
{
  if(matrix != NULL)
    matrix->setVisibilityPass(checked);

  stopAnimation();
}

/*
 * Description: Button to toggle if the render depth is visible in the matrix
 *              view.
 *
 * Inputs: bool checked - true if the render depth is visible
 * Output: none
 */
void MatrixView::buttonViewRender(bool checked)
{
  if(matrix != NULL)
    matrix->setVisibilityRender(checked);

  stopAnimation();
}

/*
 * Description: The button which triggers to decrease the width of the matrix
 *              by one tile sprite.
 *
 * Inputs: none
 * Output: none
 */
void MatrixView::buttonWidthDecrease()
{
  if(matrix != NULL)
  {
    matrix->decreaseWidth();
    if(matrix->getWidth() == 0)
      button_sprite_edit->setDisabled(true);

    stopAnimation();
  }
}

/*
 * Description: The button which triggers to increase the width of the matrix
 *              by one tile sprite.
 *
 * Inputs: none
 * Output: none
 */
void MatrixView::buttonWidthIncrease()
{
  if(matrix != NULL)
  {
    matrix->increaseWidth();
    if(matrix->getWidth() > 0)
      button_sprite_edit->setEnabled(true);

    stopAnimation();
  }
}

/*
 * Description: Initializes a matrix place dialog at the given selected tile
 *              with the path from the QFileDialog.
 *
 * Inputs: none
 * Output: none
 */
void MatrixView::initMatrixPlace()
{
  stopAnimation();

  QString path = QFileDialog::getOpenFileName(this,
                                              tr("Select a matrix sprite"),
                                              last_path,
                                              tr("Image Files (*.png)"));
  if(path != "" && matrix != NULL)
  {
    last_path = path;

    /* Close and delete the dialog if button pressed */
    if(matrix_dialog != NULL)
    {
      matrix_dialog->hide();
      disconnect(matrix_dialog, SIGNAL(matrixPlace(QString, bool, bool)),
                 matrix, SLOT(matrixPlace(QString, bool, bool)));
      delete matrix_dialog;
      matrix_dialog = NULL;
    }

    /* Open the matrix dialog and connect it */
    matrix_dialog = new MatrixDialog(path, this);
    connect(matrix_dialog, SIGNAL(matrixPlace(QString, bool, bool)),
            matrix, SLOT(matrixPlace(QString, bool, bool)));
    matrix_dialog->show();
  }
}

/*
 * Description: Triggers if the matrix is changed to set the new active frame
 *              index.
 *
 * Inputs: none
 * Output: none
 */
void MatrixView::matrixChanged()
{
  if(matrix != NULL)
    setActiveFrame(matrix->getActiveFrameIndex());
}

/*
 * Description: Right click trigger on the editor tile sprite. Sets up the
 *              menu with the proper action settings and then executes the
 *              menu.
 *
 * Inputs: EditorTileSprite* sprite - the sprite which was right clicked on
 * Output: none
 */
void MatrixView::rightClickTrigger(EditorTileSprite* sprite)
{
  if(sprite != NULL)
  {
    stopAnimation();

    /* If render depth is 0, enable elements and set their checked state */
    if(sprite->getRenderDepth() == 0 && sprite->frameCount() > 0)
    {
      action_pass_e->setEnabled(true);
      action_pass_n->setEnabled(true);
      action_pass_s->setEnabled(true);
      action_pass_w->setEnabled(true);

      action_pass_e->setChecked(sprite->getPassability(Direction::EAST));
      action_pass_n->setChecked(sprite->getPassability(Direction::NORTH));
      action_pass_s->setChecked(sprite->getPassability(Direction::SOUTH));
      action_pass_w->setChecked(sprite->getPassability(Direction::WEST));
    }
    /* Otherwise, disable action elements */
    else
    {
      action_pass_e->setDisabled(true);
      action_pass_n->setDisabled(true);
      action_pass_s->setDisabled(true);
      action_pass_w->setDisabled(true);
    }

    /* Control for if render depth is editable */
    if(sprite->frameCount() > 0)
      action_depth->setEnabled(true);
    else
      action_depth->setDisabled(true);

    /* Execute the right click menu at the cursor */
    rightclick_menu->exec(QCursor::pos());
  }
}

/*
 * Description: Starts the animation of the matrix. Triggered by play button.
 *
 * Inputs: none
 * Output: none
 */
void MatrixView::startAnimation()
{
  if(matrix != NULL && matrix->getSprite(0, 0) != NULL)
  {
    playing = true;
    button_play->setIcon(QIcon(":/images/icons/32_stop.png"));

    /* Set back to base */
    while(button_frame_prev->isEnabled())
      buttonFramePrev();

    playing_timer->start(matrix->getSprite(0, 0)->getAnimationTime().toInt());
  }
}

/*
 * Description: Stops the animation of the matrix. Triggered by all button
 *              triggers.
 *
 * Inputs: none
 * Output: none
 */
void MatrixView::stopAnimation()
{
  if(matrix != NULL)
  {
    playing = false;
    button_play->setIcon(QIcon(":/images/icons/32_play.png"));
    playing_timer->stop();

    /* Set back to base */
    while(button_frame_prev->isEnabled())
      buttonFramePrev();
  }
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Returns the matrix which is being displayed.
 *
 * Inputs: none
 * Output: EditorMatrix* - the editor matrix class
 */
EditorMatrix* MatrixView::getMatrix()
{
  return matrix;
}

/*
 * Description: Sets the matrix in the view to the new editor matrix. It also
 *              sets up all the proper visibility settings.
 *
 * Inputs: EditorMatrix* matrix - the matrix to be used. NULL resets the view.
 * Output: none
 */
void MatrixView::setMatrix(EditorMatrix* matrix)
{
  /* If existing matrix is not NULL, disconnect */
  if(this->matrix != NULL)
  {
    disconnect(this->matrix, SIGNAL(initMatrixPlace()),
               this, SLOT(initMatrixPlace()));
    disconnect(this->matrix, SIGNAL(rightClick(EditorTileSprite*)),
               this, SLOT(rightClickTrigger(EditorTileSprite*)));
    disconnect(this->matrix, SIGNAL(matrixChange()),
               this, SLOT(matrixChanged()));
    disconnect(this->matrix, SIGNAL(click()), this, SLOT(stopAnimation()));
  }

  /* Clean up matrix and set it in the view */
  stopAnimation();
  this->matrix = matrix;
  matrix_view->setScene(matrix);

  /* Configure the widget for the new matrix */
  if(matrix != NULL)
  {
    /* Set up the scene */
    matrix->cleanScene(false);
    setEnabled(true);

    /* Set the pen */
    buttonPen(true);

    /* Set the view control */
    buttonViewGrid(button_view_grid->isChecked());
    buttonViewPass(button_view_pass->isChecked());
    buttonViewRender(button_view_render->isChecked());
    setActiveFrame(0);

    /* Sprite edit initial control */
    if(matrix->getWidth() > 0)
      button_sprite_edit->setEnabled(true);
    else
      button_sprite_edit->setDisabled(true);

    /* Matrix creation by pen connection */
    connect(matrix, SIGNAL(initMatrixPlace()),
            this, SLOT(initMatrixPlace()));

    /* Matrix right click menu on tile */
    connect(matrix, SIGNAL(rightClick(EditorTileSprite*)),
            this, SLOT(rightClickTrigger(EditorTileSprite*)));

    /* Connect the matrix change update */
    connect(matrix, SIGNAL(matrixChange()), this, SLOT(matrixChanged()));

    /* Animation stop */
    connect(matrix, SIGNAL(click()), this, SLOT(stopAnimation()));
  }
  else
  {
    setDisabled(true);
  }
}
