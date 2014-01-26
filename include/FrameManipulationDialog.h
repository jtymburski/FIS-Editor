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
#include "EditorSprite.h"

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
  void setAngle(QString);

  /* Closes with no saving */
  void closeNoSave();

signals:
  /* Signal for finishing and saving the changes to the sequence */
  void finishedSave();
protected:
private:
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
  int angle_backup;

  /* Frame number */
  int framenumber;
};

#endif // FRAMEMANIPULATIONDIALOG_H
