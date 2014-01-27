/*******************************************************************************
 * Class Name: FrameManipulationDialog
 * Date Created: January 19, 2014
 * Inheritance: QWidget
 * Description: The frame dialog in the sprite creation dialog
 ******************************************************************************/
#ifndef FRAMEMANIPULATIONDIALOG_H
#define FRAMEMANIPULATIONDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QLineEdit>
#include <QDebug>
#include <QMessageBox>
#include "EditorSprite.h"
#include "Manipulabel.h"

class FrameManipulationDialog : public QDialog
{
  Q_OBJECT
public:
  /* Constructor Function */
  FrameManipulationDialog(QWidget* parent = 0,
                          EditorSprite* s = 0,
                          int framenum = 0);

  /* Destructor Function */
  ~FrameManipulationDialog();
public slots:

  /* Sets the horizontal flip for the given frame */
  void setHorizontalFlip(bool);

  /* Sets the vertical flip for the given frame */
  void setVerticalFlip(bool);

  /* Sets the angle for the given frame */
  void set0();
  void set90();
  void set180();
  void set270();

  /* Closes with no saving */
  void closeNoSave();

  /* Opens a dialog for frame replacement */
  void replaceFrame();

  /* Deletes the current frame */
  void deleteFrame();
signals:
  /* Signal for finishing and saving the changes to the sequence */
  void finishedSave();
protected:
private:

  Manipulabel* framelabel;
  QCheckBox* rotate0;
  QCheckBox* rotate90;
  QCheckBox* rotate180;
  QCheckBox* rotate270;

  /* The sprite being edited */
  EditorSprite* sprite;

  /* Ok button */
  QPushButton* ok;

  /* Cancel button */
  QPushButton* cancel;

  /* File selection button (For changing) */
  QPushButton* select_file;

  /* Backups (For Cancellation) */
  bool horizontal_backup;
  bool vertical_backup;
  bool rotate0_backup;
  bool rotate90_backup;
  bool rotate180_backup;
  bool rotate270_backup;

  /* Frame number */
  int framenumber;
};

#endif // FRAMEMANIPULATIONDIALOG_H
