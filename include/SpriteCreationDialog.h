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
  SpriteCreationDialog(QWidget *parent = 0,
                       EditorSprite *working = new EditorSprite(),
                       QString path = "");
  void loadWorkingSprite(EditorSprite*);

signals:
  void sendUpEditorSprite(EditorSprite* sprite);
public slots:
  void updateWorkingSprite();
  void destroyWorkingSprite();
private:
  QSlider* brightness_input;
  QSlider* opacity_input;
  QSlider* red_input;
  QSlider* blue_input;
  QSlider* green_input;
  EditorSprite *working_sprite;

};

#endif // SPRITECREATIONDIALOG_H
