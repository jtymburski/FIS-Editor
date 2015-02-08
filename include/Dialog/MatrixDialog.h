/*******************************************************************************
 * Class Name: MatrixDialog
 * Date Created: February 5, 2015
 * Inheritance: QDialog
 * Description: The sprite matrix dialog, which handles loading in a matrix 
 *              frame, as per the standard Univursa naming convention. Used
 *              in ThingDialog and all children.
 ******************************************************************************/
#ifndef MATRIXDIALOG_H
#define MATRIXDIALOG_H

#include <QComboBox>
#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>

#include "Database/EditorMatrix.h"

class MatrixDialog : public QDialog
{
  Q_OBJECT
public:
  /* Constructor Function */
  MatrixDialog(QString path = "", QWidget* parent = NULL);

  /* Destructor Function */
  ~MatrixDialog();

private:
  /* Buttons for frame control */
  QPushButton* button_flip_h;
  QPushButton* button_flip_v;
  QPushButton* button_prev;
  QPushButton* button_next;

  /* Combo box with width and height data */
  QComboBox* cbox_height_from;
  QComboBox* cbox_height_to;
  QComboBox* cbox_width_from;
  QComboBox* cbox_width_to;

  /* The frame number */
  int frame_num;

  /* The initial filename and path that instatiated the dialog */
  QString initial_filename;
  QString initial_path;

  /* Labels used for display */
  QLabel* lbl_frame_num;
  QLabel* lbl_result;
  QLabel* lbl_scene;

  /* The view matrix */
  EditorMatrix matrix;

  /* Spinner with the number of frames */
  QSpinBox* num_spinner;

  /*------------------- Constants -----------------------*/
  const static int kSCENE_SIZE; /* Render size of the scene */

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Checks the initial filename and disables portions of control not needed */
  void checkFilename();

  /* Creates the dialog */
  void createDialog();

  /* Fills the drop down passed in with all the characters of the alphabet */
  void fillDropDown(QComboBox* box, QString starting_letter = "A");

  /* Updates the scene view */
  void updateScene();

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Finished processing signal */
  void matrixPlace(QString result_path, bool hflip, bool vflip);

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* Button triggers */
  void buttonCancel();
  void buttonFlip();
  void buttonNextFrame();
  void buttonOk();
  void buttonPreviousFrame();
  void buttonTrim();

  /* The "from" combo box for width or height change trigger */
  void comboHeightChanged(int);
  void comboWidthChanged(int);
  
  /* One of the other edit objects changed */
  void editObjectChanged(int);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/ 
public:
  /* Is the matrix being flipped horizontally or vertically? */
  bool isFlipHorizontal();
  bool isFlipVertical();

  /* Returns the resulting file or path, in the format used by game */
  QString getResultFile();
  QString getResultPath();
};

#endif // MATRIXDIALOG_H
