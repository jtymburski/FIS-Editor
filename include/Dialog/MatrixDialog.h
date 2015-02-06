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

//#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QGridLayout>
#include <QLabel>
//#include <QLineEdit>
//#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
//#include <QVBoxLayout>

#include "Database/EditorMatrix.h"
//#include "Database/EditorSprite.h"
//#include "Dialog/FrameView.h"

class MatrixDialog : public QDialog
{
  Q_OBJECT
public:
  /* Constructor Function */
  MatrixDialog(QString path = "", QWidget* parent = NULL);

  /* Destructor Function */
  ~MatrixDialog();

private:
  /* The frame label in the view */
  //FrameView* framelabel;

  /* Frame number */
  //int framenumber;

  /* The sprite being edited */
  //EditorSprite* sprite_original;
  //EditorSprite* sprite_working;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Fills the drop down passed in with all the characters of the alphabet */
  void fillDropDown(QComboBox* box, QString starting_letter = "A");

protected:
signals:
public slots:
public:
  /* Deletes the current frame */
  //void deleteFrame();

  /* Slot for finishing the saving of changes to the sequence */
  //void finishSave();

  /* Opens a dialog for frame replacement */
  //void replaceFrame();

  /* Sets the horizontal flip for the given frame */
  //void setHorizontalFlip(bool);

  /* Sets the vertical flip for the given frame */
  //void setVerticalFlip(bool);

  /* Sets the angle for the given frame */
  //void set0();
  //void set90();
  //void set180();
  //void set270();
};

#endif // MATRIXDIALOG_H
