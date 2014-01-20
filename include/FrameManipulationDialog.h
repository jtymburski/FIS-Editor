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
  void setVerticalFlip(bool);
  void setAngle(QString);
  void closeNoSave();
protected:
private:
  /* Ok button */
  EditorSprite* sprite;
  QPushButton* ok;
  QPushButton* cancel;
  QPushButton* select_file;
  bool horizontal_backup;
  bool vertical_backup;
  int angle_backup;
  int framenumber;
};

#endif // FRAMEMANIPULATIONDIALOG_H
