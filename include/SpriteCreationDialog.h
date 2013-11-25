#ifndef SPRITECREATIONDIALOG_H
#define SPRITECREATIONDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include "EditorSprite.h"

class SpriteCreationDialog : public QDialog
{
  Q_OBJECT
public:
  SpriteCreationDialog(QWidget *parent = 0);
  void loadWorkingSprite(EditorSprite*);

signals:

public slots:
  void updateWorkingSprite();
private:
  QLabel *label;
  QLineEdit* brightness_input;
  EditorSprite *working_sprite;

};

#endif // SPRITECREATIONDIALOG_H
