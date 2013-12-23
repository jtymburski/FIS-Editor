#include "SpriteCreationDialog.h"
#include <QDebug>

SpriteCreationDialog::SpriteCreationDialog(uint16_t next_id,
                                           QWidget *parent,
                                           EditorSprite *working) :
  QDialog(parent)
{
  qDebug()<<next_id;
  working_sprite = working;
  working_sprite->getSprite()->setId(next_id);

  QGridLayout* layout = new QGridLayout();
  layout->setSpacing(2);
  layout->setAlignment(Qt::AlignRight);

  QString id_label("ID: ");
  id_label.append(QString::number(working_sprite->getSprite()->getId()));
  QLabel* id_number = new QLabel(id_label);
  layout->addWidget(id_number,0,0);

  QWidget* frametest = new QWidget();
  frametest->setFixedSize(256,32);
  frametest->setBackgroundRole(QPalette::Base);
  layout->addWidget(frametest,1,0);

  QLabel* brightness_label = new QLabel("Brightness");
  layout->addWidget(brightness_label,2,0);
  QLabel* brightness_min = new QLabel("0.0");
  layout->addWidget(brightness_min,2,1);
  brightness_input = new QSlider(Qt::Horizontal);
  brightness_input->setTickInterval(1);
  brightness_input->setMinimum(0);
  brightness_input->setMaximum(200);
  brightness_input->setFixedWidth(192);
  brightness_input->setSliderPosition(
        static_cast<int>(working_sprite->getSprite()->getBrightness()*100));
  connect(brightness_input,SIGNAL(valueChanged(int)),
          working_sprite,SLOT(setBrightness(int)));
  layout->addWidget(brightness_input,2,2);
  QLabel* brightness_max = new QLabel("2.0");
  layout->addWidget(brightness_max,2,3);

  QLabel* opacity_label = new QLabel("Opacity");
  layout->addWidget(opacity_label,3,0);
  QLabel* opacity_min = new QLabel("0");
  layout->addWidget(opacity_min,3,1);
  opacity_input = new QSlider(Qt::Horizontal);
  opacity_input->setTickInterval(1);
  opacity_input->setMinimum(0);
  opacity_input->setMaximum(255);
  opacity_input->setSliderPosition(working_sprite->getSprite()->getOpacity());
  connect(opacity_input,SIGNAL(valueChanged(int)),
          working_sprite,SLOT(setOpacity(int)));
  layout->addWidget(opacity_input,3,2);
  QLabel* opacity_max = new QLabel("255");
  layout->addWidget(opacity_max,3,3);

  QLabel* red_label = new QLabel("Red Balance");
  layout->addWidget(red_label,4,0);
  QLabel* red_min = new QLabel("0");
  layout->addWidget(red_min,4,1);
  red_input = new QSlider(Qt::Horizontal);
  red_input->setTickInterval(1);
  red_input->setMinimum(0);
  red_input->setMaximum(255);
  red_input->setSliderPosition(working_sprite->getSprite()->getColorRed());
  connect(red_input,SIGNAL(valueChanged(int)),
          working_sprite,SLOT(setColorRed(int)));
  layout->addWidget(red_input,4,2);
  QLabel* red_max = new QLabel("255");
  layout->addWidget(red_max,4,3);

  QLabel* blue_label = new QLabel("Blue Balance");
  layout->addWidget(blue_label,5,0);
  QLabel* blue_min = new QLabel("0");
  layout->addWidget(blue_min,5,1);
  blue_input = new QSlider(Qt::Horizontal);
  blue_input->setTickInterval(1);
  blue_input->setMinimum(0);
  blue_input->setMaximum(255);
  blue_input->setSliderPosition(working_sprite->getSprite()->getColorBlue());
  connect(blue_input,SIGNAL(valueChanged(int)),
          working_sprite,SLOT(setColorBlue(int)));
  layout->addWidget(blue_input,5,2);
  QLabel* blue_max = new QLabel("255");
  layout->addWidget(blue_max,5,3);

  QLabel* green_label = new QLabel("Green Balance");
  layout->addWidget(green_label,6,0);
  QLabel* green_min = new QLabel("0");
  layout->addWidget(green_min,6,1);
  green_input = new QSlider(Qt::Horizontal);
  green_input->setTickInterval(1);
  green_input->setMinimum(0);
  green_input->setMaximum(255);
  green_input->setSliderPosition(working_sprite->getSprite()->getColorGreen());
  connect(green_input,SIGNAL(valueChanged(int)),
          working_sprite,SLOT(setColorGreen(int)));
  layout->addWidget(green_input,6,2);
  QLabel* green_max = new QLabel("255");
  layout->addWidget(green_max,6,3);

  layout->addWidget(new QWidget(),7,0);
  layout->addWidget(new QWidget(),8,0);
  layout->addWidget(new QWidget(),9,0);

  QPushButton* ok = new QPushButton("Ok",this);
  ok->setFixedWidth(64);
  layout->addWidget(ok,10,0);
  connect(ok,SIGNAL(clicked()),this,SLOT(updateWorkingSprite()));

  setLayout(layout);


}

void SpriteCreationDialog::loadWorkingSprite(EditorSprite *s)
{
  working_sprite = s;
}

void SpriteCreationDialog::updateWorkingSprite()
{
  delete working_sprite;
  close();
}
