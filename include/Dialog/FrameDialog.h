/*******************************************************************************
 * Class Name: FrameDialog
 * Date Created: January 19, 2014
 * Inheritance: QWidget
 * Description: The frame dialog in the sprite creation dialog
 ******************************************************************************/
#ifndef FRAMEDIALOG_H
#define FRAMEDIALOG_H

#include <QCheckBox>
#include <QDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

#include "Database/EditorSprite.h"
#include "Dialog/FrameView.h"

class FrameDialog : public QDialog
{
  Q_OBJECT
public:
  /* Constructor Function */
  FrameDialog(QWidget* parent = NULL, EditorSprite* s = NULL, int framenum = 0);

  /* Destructor Function */
  ~FrameDialog();

private:
  /* The frame label in the view */
  FrameView* framelabel;

  /* Frame number */
  int framenumber;

  /* The sprite being edited */
  EditorSprite* sprite_original;
  EditorSprite* sprite_working;

protected:

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Ok triggered on save */
  void ok();

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* Deletes the current frame */
  void deleteFrame();

  /* Slot for finishing the saving of changes to the sequence */
  void finishSave();

  /* Opens a dialog for frame replacement */
  void replaceFrame();

  /* Sets the horizontal flip for the given frame */
  void setHorizontalFlip(bool);

  /* Sets the vertical flip for the given frame */
  void setVerticalFlip(bool);

  /* Sets the angle for the given frame */
  void set0();
  void set90();
  void set180();
  void set270();
};

#endif // FRAMEDIALOG_H
