#ifndef SPRITECREATIONDIALOG_H
#define SPRITECREATIONDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QSlider>
#include <QAction>
#include <QScrollArea>
#include <QPushButton>
#include <QComboBox>
#include "FrameManipulator.h"
#include "EditorSprite.h"

class SpriteCreationDialog : public QDialog
{
  Q_OBJECT
public:
  /* Constructor function */
  SpriteCreationDialog(QWidget *parent = 0,
                       EditorSprite *working = new EditorSprite(),
                       QString path = "", int subsequent = 0,
                       bool creation = true);
private:
  /* Sliders that correspond to the various parameters of the sprite */
  QSlider* brightness_input;
  QSlider* opacity_input;
  QSlider* red_input;
  QSlider* blue_input;
  QSlider* green_input;
  QCheckBox* rotate0;
  QCheckBox* rotate90;
  QCheckBox* rotate180;
  QCheckBox* rotate270;
  FrameManipulator* framemanipulator;
  QScrollArea* frame_scrollwrapper;

  /* The current sprite that is being altered/created by the dialog */
  EditorSprite *working_sprite;

  bool creation_mode;

  QString name_backup;
  int brightness_backup;
  int opacity_backup;
  int red_backup,blue_backup,green_backup;
  QString time_backup;
  bool direction_backup;
  QString rotation_backup;

  bool rotate0_backup;
  bool rotate90_backup;
  bool rotate180_backup;
  bool rotate270_backup;


signals:
  /* Sends up the Editor Sprite */
  void sendUpEditorSprite(EditorSprite* sprite);

  void ok();
public slots:
  /* Loads the given working Editor Sprite */
  void loadWorkingSprite(EditorSprite*);

  /* Updates the current working sprite */
  void updateWorkingSprite();

  /* destroys the current working sprite */
  void destroyWorkingSprite();

};

#endif // SPRITECREATIONDIALOG_H
