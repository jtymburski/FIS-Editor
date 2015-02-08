/*******************************************************************************
 * Class Name: MatrixDialog
 * Date Created: February 5, 2015
 * Inheritance: QDialog
 * Description: The sprite matrix dialog, which handles loading in a matrix 
 *              frame, as per the standard Univursa naming convention. Used
 *              in ThingDialog and all children.
 ******************************************************************************/
#include "Dialog/MatrixDialog.h"
#include <QDebug>

/* Constant Implementation - see header file for descriptions */
const int MatrixDialog::kSCENE_SIZE = 320;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function
 *
 * Inputs: QString path - the base path initially chosen 
 *         QWidget* parent - the parent class of the dialog
 */
MatrixDialog::MatrixDialog(QString path, QWidget* parent) : QDialog(parent)
{
  if(!path.isEmpty() && path.endsWith(".png"))
  {
    /* Initial variables */
    path = QDir::toNativeSeparators(path);
    QStringList path_set = path.split(QDir::separator());
    initial_filename = path_set.last();
    initial_path = path.remove(initial_filename);
    frame_num = 0;

    /* Matrix config */
    matrix.setVisibilityGrid(false);
    matrix.setVisibilityPass(false);
    matrix.setVisibilityRender(false);

    /* Set up the view objects */
    createDialog();
    checkFilename();

    /* Finally, update the scene */
    updateScene();
  }
}

/*
 * Description: Destructor Function
 */
MatrixDialog::~MatrixDialog()
{

}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: Checks the initial file name from the path and disables 
 *              the control objects that aren't valid (if the sprite doesn't
 *              conform to Univursa naming standards).
 *
 * Inputs: none
 * Output: none
 */
void MatrixDialog::checkFilename()
{
  /* Initial split */
  QString file = initial_filename;
  file.chop(QString(".png").size());
  QStringList file_split = file.split('_');

  /* Start with all elements disabled */
  button_prev->setDisabled(true);
  button_next->setDisabled(true);
  cbox_height_from->setDisabled(true);
  cbox_height_to->setDisabled(true);
  cbox_width_from->setDisabled(true);
  cbox_width_to->setDisabled(true);
  num_spinner->setDisabled(true);

  /* Ensure there are at least enough elements */
  if(file_split.size() >= 3)
  {
    QString last = file_split.last();
    QString second_last = file_split.at(file_split.size() - 2);

    /* Check that the last and second last sections meet the standard */
    if(last.size() == 3 && second_last.size() == 2 && 
       second_last.at(0).isUpper() && second_last.at(1).isUpper() && 
       last.at(0).isUpper() && last.at(1).isDigit() && last.at(2).isDigit())
    {
      /* Enable combo boxes */
      cbox_height_from->setEnabled(true);
      cbox_height_to->setEnabled(true);
      cbox_width_from->setEnabled(true);
      cbox_width_to->setEnabled(true);

      /* Disable signals */
      cbox_height_from->blockSignals(true);
      cbox_height_to->blockSignals(true);
      cbox_width_from->blockSignals(true);
      cbox_width_to->blockSignals(true);

      /* Choose the full range and then update scene and trim */
      cbox_height_from->setCurrentIndex(0);
      cbox_height_to->setCurrentIndex(cbox_height_to->count() - 1);
      cbox_width_from->setCurrentIndex(0);
      cbox_width_to->setCurrentIndex(cbox_width_to->count() - 1);
      updateScene();
      buttonTrim();

      /* Check if both digits are 0 (base frame */
      if(last.at(1).digitValue() == 0 && last.at(2).digitValue() == 0)
      {
        /* If they are, set a slightly higher value and trim */
        num_spinner->setEnabled(true);
        num_spinner->setValue(10);
        buttonTrim();
      }
      else
      {
        updateScene();
      }

      /* Enable signals */
      cbox_height_from->blockSignals(false);
      cbox_height_to->blockSignals(false);
      cbox_width_from->blockSignals(false);
      cbox_width_to->blockSignals(false);
    }
  }
}

/*
 * Description: Creates the dialog and fills it with objects with the correct
 *              configuration. Only called once on initial construction and
 *              necessary for the dialog to work.
 *
 * Inputs: none
 * Output: none
 */
void MatrixDialog::createDialog()
{
  /* Layout setup */
  QGridLayout* layout = new QGridLayout(this);
  layout->setSizeConstraint(QLayout::SetFixedSize);

  /* Image label */
  lbl_scene = new QLabel(this);
  QImage scene_img(kSCENE_SIZE, kSCENE_SIZE, QImage::Format_ARGB32);
  lbl_scene->setPixmap(QPixmap::fromImage(scene_img));
  lbl_scene->setStyleSheet("border: 2px solid black");
  lbl_scene->setMinimumWidth(kSCENE_SIZE);
  lbl_scene->setMinimumHeight(kSCENE_SIZE);
  lbl_scene->setAlignment(Qt::AlignCenter);
  layout->addWidget(lbl_scene, 0, 0, 6, 3);

  /* Scene control */
  button_prev = new QPushButton("<", this);
  button_prev->setMaximumWidth(kSCENE_SIZE / 10);
  connect(button_prev, SIGNAL(clicked()), this, SLOT(buttonPreviousFrame()));
  layout->addWidget(button_prev, 6, 0, Qt::AlignRight);
  lbl_frame_num = new QLabel(QString::number(frame_num), this);
  layout->addWidget(lbl_frame_num, 6, 1, Qt::AlignCenter);
  button_next = new QPushButton(">", this);
  button_next->setMaximumWidth(kSCENE_SIZE / 10);
  connect(button_next, SIGNAL(clicked()), this, SLOT(buttonNextFrame()));
  layout->addWidget(button_next, 6, 2, Qt::AlignLeft);

  /* Path label */
  QLabel* lbl_initial = new QLabel("Filename: " + initial_filename, this);
  lbl_initial->setWordWrap(true);
  layout->addWidget(lbl_initial, 0, 3, 1, 4);

  /* Horizontal width control */
  QLabel* width_lbl_1 = new QLabel("Horizontal", this);
  layout->addWidget(width_lbl_1, 1, 3);
  cbox_width_from = new QComboBox(this);
  fillDropDown(cbox_width_from);
  connect(cbox_width_from, SIGNAL(currentIndexChanged(int)), 
          this, SLOT(comboWidthChanged(int)));
  layout->addWidget(cbox_width_from, 1, 4);
  QLabel* width_lbl_2 = new QLabel("TO", this);
  layout->addWidget(width_lbl_2, 1, 5, Qt::AlignHCenter);
  cbox_width_to = new QComboBox(this);
  fillDropDown(cbox_width_to, cbox_width_from->currentText());
  connect(cbox_width_to, SIGNAL(currentIndexChanged(int)),
          this, SLOT(editObjectChanged(int)));
  layout->addWidget(cbox_width_to, 1, 6);

  /* Vertical height control */
  QLabel* height_lbl_1 = new QLabel("Vertical", this);
  layout->addWidget(height_lbl_1, 2, 3);
  cbox_height_from = new QComboBox(this);
  fillDropDown(cbox_height_from);
  connect(cbox_height_from, SIGNAL(currentIndexChanged(int)), 
          this, SLOT(comboHeightChanged(int)));
  layout->addWidget(cbox_height_from, 2, 4);
  QLabel* height_lbl_2 = new QLabel("TO", this);
  layout->addWidget(height_lbl_2, 2, 5, Qt::AlignHCenter);
  cbox_height_to = new QComboBox(this);
  fillDropDown(cbox_height_to, cbox_height_from->currentText());
  connect(cbox_height_to, SIGNAL(currentIndexChanged(int)), 
          this, SLOT(editObjectChanged(int)));
  layout->addWidget(cbox_height_to, 2, 6);

  /* Scene flipping control */
  button_flip_h = new QPushButton("Horizontal Flip", this);
  button_flip_h->setCheckable(true);
  connect(button_flip_h, SIGNAL(clicked()), this, SLOT(buttonFlip()));
  layout->addWidget(button_flip_h, 3, 3, 1, 2);
  button_flip_v = new QPushButton("Vertical Flip", this);
  button_flip_v->setCheckable(true);
  connect(button_flip_v, SIGNAL(clicked()), this, SLOT(buttonFlip()));
  layout->addWidget(button_flip_v, 3, 5, 1, 2);

  /* Number of frames */
  QLabel* lbl_num = new QLabel("Number of Frames", this);
  layout->addWidget(lbl_num, 4, 3, 1, 2);
  num_spinner = new QSpinBox(this);
  num_spinner->setMinimum(1);
  num_spinner->setMaximum(99);
  connect(num_spinner, SIGNAL(valueChanged(int)),
          this, SLOT(editObjectChanged(int)));
  layout->addWidget(num_spinner, 4, 5, 1, 2);

  /* Result label */
  lbl_result = new QLabel("Result:", this);
  layout->addWidget(lbl_result, 5, 3, 1, 4);

  /* Buttons at bottom */
  QPushButton* button_trim = new QPushButton("Trim", this);
  connect(button_trim, SIGNAL(clicked()), this, SLOT(buttonTrim()));
  layout->addWidget(button_trim, 6, 4);
  QPushButton* button_ok = new QPushButton("Ok", this);
  connect(button_ok, SIGNAL(clicked()), this, SLOT(buttonOk()));
  layout->addWidget(button_ok, 6, 5);
  QPushButton* button_cancel = new QPushButton("Cancel", this);
  connect(button_cancel, SIGNAL(clicked()), this, SLOT(buttonCancel()));
  layout->addWidget(button_cancel, 6, 6);

  /* Dialog control */
  setLayout(layout);
  setWindowTitle("Sprite Matrix Edit");
}

/*
 * Description: Takes a combo box and fills it with all the letters from the
 *              passed in starting letter to 'Z'. It will clear the combo box
 *              prior to the add.
 *
 * Inputs: QComboBox* box - the combo box to edit. Can't be NULL
 *         QString starting_letter - the first letter. Default to 'A'.
 * Output: none
 */
void MatrixDialog::fillDropDown(QComboBox* box, QString starting_letter)
{
  if(starting_letter.size() == 1 && starting_letter.at(0).isUpper())
  {
    box->blockSignals(true);

    /* Empty the existing combo box */
    QString previous_selected = box->currentText();
    while(box->count() > 0)
      box->removeItem(0);

    /* Get the range */
    int start = starting_letter.at(0).unicode();
    int end = QChar('Z').unicode();
    
    /* Loop through them all */
    for(int i = start; i <= end; i++)
      box->addItem(QString(QChar(i)));

    /* Try and set the item back to where it was */
    int index = box->findText(previous_selected);
    if(index >= 0)
      box->setCurrentIndex(index);

    box->blockSignals(false);
  }
}
  
/*
 * Description: Updates the scene matrix and prints an image of it for 
 *              rendering in the dialog.
 *
 * Inputs: none
 * Output: none
 */
void MatrixDialog::updateScene()
{
  /* Update the scene with the new path */
  QString result_file = getResultFile();
  matrix.addPath(initial_path + result_file, 0, 0, 
                 button_flip_h->isChecked(), button_flip_v->isChecked(), true);

  /* Paint the image */
  matrix.clearSelection();
  matrix.setSceneRect(matrix.itemsBoundingRect());
  QImage image(matrix.sceneRect().size().toSize(), QImage::Format_ARGB32);
  image.fill(Qt::transparent);
  if(!image.isNull())
  {
    QPainter painter(&image);
    matrix.render(&painter);
    if(image.width() > kSCENE_SIZE || image.height() > kSCENE_SIZE)
    {
      QImage scaled_image = image.scaled(kSCENE_SIZE, kSCENE_SIZE, 
                                         Qt::KeepAspectRatio);
      lbl_scene->setPixmap(QPixmap::fromImage(scaled_image));
    }
    else
    {
      lbl_scene->setPixmap(QPixmap::fromImage(image));
    }
  }

  /* Update result string */
  lbl_result->setText("Result: " + result_file);

  /* Update dialog */
  update();
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
  
/*
 * Description: Button trigger called on cancel click. Only closes the dialog.
 *
 * Inputs: none
 * Output: none
 */
void MatrixDialog::buttonCancel()
{
  close();
}

/* 
 * Description: Button trigger called on a flip click (horizontal or vertical).
 *              Updates the scene for the new flip.
 *
 * Inputs: none
 * Output: none
 */
void MatrixDialog::buttonFlip()
{
  updateScene();
}

/*
 * Description: Button trigger called on next frame click. Shifts the viewing
 *              frame of the matrix to the next one, if within range of the
 *              spin box.
 *
 * Inputs: none
 * Output: none
 */
void MatrixDialog::buttonNextFrame()
{
  if(frame_num < (num_spinner->value() - 1))
  {
    frame_num++;
    matrix.setActiveFrame(frame_num);
    button_prev->setEnabled(true);

    if(frame_num == (num_spinner->value() - 1))
      button_next->setDisabled(true);

    /* Update label and scene */
    lbl_frame_num->setText(QString::number(frame_num));
    updateScene();
  }
}

/*
 * Description: Button trigger called on ok click. Emits matrixSuccess signal 
 *              (necessary for determining if the data is good) and then closes
 *              the dialog.
 *
 * Inputs: none
 * Output: none
 */
void MatrixDialog::buttonOk()
{
  //buttonTrim();
  emit matrixPlace(getResultPath(), isFlipHorizontal(), isFlipVertical());
  close();
}

/*
 * Description: Button trigger called on prev frame click. Shifts the viewing
 *              frame of the matrix to the previous one, if greater than 0.
 *
 * Inputs: none
 * Output: none
 */
void MatrixDialog::buttonPreviousFrame()
{
  if(frame_num > 0)
  {
    frame_num--;
    matrix.setActiveFrame(frame_num);
    button_next->setEnabled(true);

    if(frame_num == 0)
      button_prev->setDisabled(true);

    /* Update label and scene */
    lbl_frame_num->setText(QString::number(frame_num));
    updateScene();
  }
}

/*
 * Description: Button trigger called on trim click. This cleans up the matrix
 *              and removes all frames on all sides that completely blank and
 *              also removes frames that have no data in them. Prepares for 'ok'
 *
 * Inputs: none
 * Output: none
 */
void MatrixDialog::buttonTrim()
{
  /* Get trimmed range of sprites */
  QRect range = matrix.getTrimRect();

  /* Update width */
  int diff_x1 = range.x();
  int diff_x2 = matrix.getWidth() - range.x() - range.width();
  if(diff_x1 > 0)
  {
    if(isFlipHorizontal())
      cbox_width_to->setCurrentIndex(cbox_width_to->currentIndex() - diff_x1);
    else
      cbox_width_from->setCurrentIndex(cbox_width_from->currentIndex() + 
                                       diff_x1);
  }
  if(diff_x2 > 0)
  {
    if(isFlipHorizontal())
      cbox_width_from->setCurrentIndex(cbox_width_from->currentIndex() + 
                                       diff_x2);
    else
      cbox_width_to->setCurrentIndex(cbox_width_to->currentIndex() - diff_x2);
  }

  /* Update height */
  int diff_y1 = range.y();
  int diff_y2 = matrix.getHeight() - range.y() - range.height();
  if(diff_y1 > 0)
  {
    if(isFlipVertical())
      cbox_height_to->setCurrentIndex(cbox_height_to->currentIndex() - diff_y1);
    else
      cbox_height_from->setCurrentIndex(cbox_height_from->currentIndex() + 
                                        diff_y1);
  }
  if(diff_y2 > 0)
  {
    if(isFlipVertical())
      cbox_height_from->setCurrentIndex(cbox_height_from->currentIndex() + 
                                        diff_y1);
    else
      cbox_height_to->setCurrentIndex(cbox_height_to->currentIndex() - diff_y2);
  }

  /* Update the spinbox with number of frames */
  int num_frames = matrix.getTrimFrames();
  if(num_spinner->value() > (num_frames + 1))
    num_spinner->setValue(num_frames + 1);
}

/*
 * Description: Triggered if the combo height 'from' box is changed. Updates
 *              the height 'to' box to only show applicable letters greater
 *              than or equal to the 'from' box.
 *
 * Inputs: int - not used
 * Output: none
 */
void MatrixDialog::comboHeightChanged(int)
{
  fillDropDown(cbox_height_to, cbox_height_from->currentText());
  updateScene();
}

/*
 * Description: Triggered if the combo width 'from' box is changed. Updates
 *              the width 'to' box to only show applicable letters greater
 *              than or equal to the 'from' box.
 *
 * Inputs: int - not used
 * Output: none
 */
void MatrixDialog::comboWidthChanged(int)
{
  fillDropDown(cbox_width_to, cbox_width_from->currentText());
  updateScene();
}

/*
 * Description: Fires if any other edit object changes (the other two combo 
 *              boxes or the spinner). Updates the scene and frames for spinner.
 *
 * Inputs: int - not used
 * Output: none
 */
void MatrixDialog::editObjectChanged(int)
{
  /* Update frame buttons if spinner has changed */
  if(num_spinner->value() == 1)
  {
    frame_num = 0;
    button_prev->setDisabled(true);
    button_next->setDisabled(true);

    /* Update label */
    lbl_frame_num->setText(QString::number(frame_num));
  }
  else
  {
    if(frame_num >= num_spinner->value())
      frame_num = num_spinner->value() - 1;

    /* Previous button */
    if(frame_num > 0)
      button_prev->setEnabled(true);
    else
      button_prev->setDisabled(true);

    /* Next button */
    if(frame_num < (num_spinner->value() - 1))
      button_next->setEnabled(true);
    else
      button_next->setDisabled(true);

    /* Update label */
    lbl_frame_num->setText(QString::number(frame_num));
  }

  /* Finally, update the scene with changes */
  matrix.setActiveFrame(frame_num);
  updateScene();
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/ 
  
/*
 * Description: Is the matrix flipped horizontally?
 *
 * Inputs: none
 * Output: bool - true if the matrix is flipped horizontally
 */
bool MatrixDialog::isFlipHorizontal()
{
  return button_flip_h->isChecked();
}

/*
 * Description: Is the matrix flipped vertically?
 *
 * Inputs: none
 * Output: bool - true if the matrix is flipped vertically
 */
bool MatrixDialog::isFlipVertical()
{
  return button_flip_v->isChecked();
}

/*
 * Description: Returns the result file of the matrix in the format of 
 *              Univursa. This can be passed into EditorMatrix to load in the
 *              frames. Eg. archy_[A-C][B-D]_D|2|.png
 *
 * Inputs: none
 * Output: QString - the result file name
 */
QString MatrixDialog::getResultFile()
{
  QString file = initial_filename;

  if(cbox_height_from->isEnabled())
  {
    /* Initial string computation */
    file.chop(QString("XX_D00.png").size());
    QStringList file_set = initial_filename.split('_');

    /* Get the width */
    if(cbox_width_from->currentText() != cbox_width_to->currentText())
    {
      file += "[";
      if(button_flip_h->isChecked())
        file += cbox_width_to->currentText() + "-" + 
                cbox_width_from->currentText();
      else
        file += cbox_width_from->currentText() + "-" + 
                cbox_width_to->currentText();
      file += "]";
    }
    else
    {
      file += cbox_width_from->currentText();
    }

    /* Get the height */
    if(cbox_height_from->currentText() != cbox_height_to->currentText())
    {
      file += "[";
      if(button_flip_v->isChecked())
        file += cbox_height_to->currentText() + "-" + 
                cbox_height_from->currentText();
      else
        file += cbox_height_from->currentText() + "-" + 
                cbox_height_to->currentText();
      file += "]";
    }
    else
    {
      file += cbox_height_from->currentText();
    }
    file += "_";

    /* Get the frame count */
    if(num_spinner->value() > 1)
      file += QString(file_set.last().at(0)) + "|" + 
              QString::number(num_spinner->value()) + "|.png";
    else
      file += file_set.last();
  }

  return file;
}

/*
 * Description: Returns the result path and file appended together, in the 
 *              format of Univursa. This can be passed into EditorMatrix to load
 *              in the frames. See getResultFile() for more info.
 *
 * Inputs: none
 * Output: QString - the result path with file name
 */
QString MatrixDialog::getResultPath()
{
  return initial_path + getResultFile();
}
