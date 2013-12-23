#ifndef SPRITECREATIONDIALOG_H
#define SPRITECREATIONDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QSlider>
#include <QPushButton>
#include "EditorSprite.h"

class SpriteCreationDialog : public QDialog
{
  Q_OBJECT
public:
  SpriteCreationDialog(uint16_t next_id = 1, QWidget *parent = 0,
                       EditorSprite *working = new EditorSprite());
  void loadWorkingSprite(EditorSprite*);

signals:

public slots:
  void updateWorkingSprite();
private:
  QSlider* brightness_input;
  QSlider* opacity_input;
  QSlider* red_input;
  QSlider* blue_input;
  QSlider* green_input;
  EditorSprite *working_sprite;

};

#endif // SPRITECREATIONDIALOG_H
