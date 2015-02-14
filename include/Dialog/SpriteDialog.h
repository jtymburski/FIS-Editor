/*******************************************************************************
 * Class Name: SpriteDialog
 * Date Created: January 22, 2014
 * Inheritance: QDialog
 * Description: The dialog that displays the sprites and the information
 *              related to it. Allows manipulation of the data.
 ******************************************************************************/
#ifndef SPRITEDIALOG_H
#define SPRITEDIALOG_H

#include <QAction>
#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QScrollArea>
#include <QSlider>
#include <QVBoxLayout>

#include "Dialog/FrameList.h"
#include "Database/EditorSprite.h"

class SpriteDialog : public QDialog
{
  Q_OBJECT
public:
  /* Constructor function */
  SpriteDialog(QWidget *parent = NULL, EditorSprite *working = NULL,
               QString path = "", int subsequent = 0, bool creation = true,
               EditorEnumDb::SpriteEditMode mode = EditorEnumDb::SPRITE_ALL);

  /* Destructor function */
  ~SpriteDialog();

private:
  /* Sliders that correspond to the various parameters of the sprite */
  QSlider* brightness_input;
  QSlider* opacity_input;
  QSlider* red_input;
  QSlider* blue_input;
  QSlider* green_input;
  FrameList* frame_list;
  QScrollArea* frame_scrollwrapper;

  /* Direction combo box */
  QComboBox* direction_input;

  /* Time input Line edit */
  QLineEdit* rotation_input;
  QLineEdit* time_input;

  /* The current sprite that is being altered/created by the dialog */
  EditorSprite *original_sprite;
  EditorSprite *working_sprite;
  bool creation_mode;
  EditorEnumDb::SpriteEditMode mode;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Sets the data sliders and frames in the dialog, based on the working
   * sprite */
  void setToWorking();

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Custom close event */
  void closeEvent(QCloseEvent *);

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* destroys the current working sprite */
  void destroyWorkingSprite();

  /* Set sprite values to default */
  void setToDefault();

  /* Updates the current working sprite */
  void updateWorkingSprite();

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Sends up the Editor Sprite */
  void sendUpEditorSprite(EditorSprite* sprite);

  /* Ok pressed */
  void ok();
};

#endif // SPRITEDIALOG_H
