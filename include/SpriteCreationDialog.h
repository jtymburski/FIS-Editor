#ifndef SPRITECREATIONDIALOG_H
#define SPRITECREATIONDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QSlider>
#include <QPushButton>
#include <QComboBox>
#include "EditorSprite.h"

class SpriteCreationDialog : public QDialog
{
  Q_OBJECT
public:
  /* Constructor function */
  SpriteCreationDialog(QWidget *parent = 0,
                       EditorSprite *working = new EditorSprite(),
                       QString path = "");
private:
  /* Sliders that correspond to the various parameters of the sprite */
  QSlider* brightness_input;
  QSlider* opacity_input;
  QSlider* red_input;
  QSlider* blue_input;
  QSlider* green_input;

  /* The current sprite that is being altered/created by the dialog */
  EditorSprite *working_sprite;

signals:
  /* Sends up the Editor Sprite */
  void sendUpEditorSprite(EditorSprite* sprite);

public slots:
  /* Loads the given working Editor Sprite */
  void loadWorkingSprite(EditorSprite*);

  /* Updates the current working sprite */
  void updateWorkingSprite();

  /* destroys the current working sprite */
  void destroyWorkingSprite();

};

#endif // SPRITECREATIONDIALOG_H
