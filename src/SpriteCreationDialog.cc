#include "SpriteCreationDialog.h"

SpriteCreationDialog::SpriteCreationDialog(QWidget *parent) :
  QDialog(parent)
{
  working_sprite = NULL;
  QGridLayout* layout = new QGridLayout();

  label = new QLabel("Brightness (0.0-1.9)");
  layout->addWidget(label,0,0);

  brightness_input = new QLineEdit(this);
  brightness_input->setInputMask("B.D");

  layout->addWidget(brightness_input,0,1);

  QPushButton* ok = new QPushButton("Ok",this);
  layout->addWidget(ok,1,0);
  connect(ok,SIGNAL(clicked()),this,SLOT(updateWorkingSprite()));

  setLayout(layout);


}

void SpriteCreationDialog::loadWorkingSprite(EditorSprite *s)
{
  working_sprite = s;
}

void SpriteCreationDialog::updateWorkingSprite()
{
  working_sprite->getSprite()
      ->setBrightness(brightness_input->text().toFloat());
  close();
}
