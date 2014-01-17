#include "SpriteCreationDialog.h"
#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function
 *
 * Input: Parent, editor sprite to work on, image path
 */
SpriteCreationDialog::SpriteCreationDialog(QWidget *parent,
                                           EditorSprite *working,QString p,
                                           int subsequent, bool creation)
                                           : QDialog(parent)
{
  /* Sets the working sprite and appropriate paths */
  working_sprite = working;
  creation_mode = creation;


  if(creation_mode)
  {
    /* Sets the subsequent frames of the sequence (The first is set on
     * construction, thus we start adding the frames at i+1) */
    QString frame_temp = p;
    for(int i=0; i<subsequent; i++)
    {
      frame_temp.chop(6);
      if(i+1<=9)
        frame_temp.append('0');
      frame_temp.append(QString::number(i+1));
      frame_temp.append(".png");
      working_sprite->setPath(frame_temp);
      working_sprite->setImage(QImage(frame_temp));
    }
  }

  /* Sets up a grid layout for the sliders and such */
  QGridLayout* layout = new QGridLayout();
  layout->setSpacing(2);
  layout->setAlignment(Qt::AlignRight);

  /* ID display */
  QString id_label("ID: ");
  id_label.append(QString::number(working_sprite->getSprite()->getId()));
  QLabel* id_number = new QLabel(id_label);
  layout->addWidget(id_number,0,0);

  /* Name input */
  QLabel* name = new QLabel("Name:");
  layout->addWidget(name,0,1);
  name_backup = working_sprite->getName();
  QLineEdit* name_input = new QLineEdit(working_sprite->getName());
  name_input->setInputMask("xxxxxxxxxxxxxxxx");
  name_input->setFixedWidth(128);
  connect(name_input,SIGNAL(textChanged(QString)),
          working_sprite,SLOT(setName(QString)));
  layout->addWidget(name_input,0,2);

  /* Frame Display */
  // TODO: Make this do something
  QWidget* frametest = new QWidget();
  frametest->setFixedSize(256,32);
  frametest->setBackgroundRole(QPalette::Base);
  layout->addWidget(frametest,1,1);

  /* Brightness input */
  QLabel* brightness_label = new QLabel("Brightness");
  layout->addWidget(brightness_label,2,0);
  brightness_input = new QSlider(Qt::Horizontal);
  brightness_input->setTickInterval(1);
  brightness_input->setMinimum(0);
  brightness_input->setMaximum(200);
  brightness_backup = static_cast<int>(working_sprite->getSprite()->
                                       getBrightness()*100);
  brightness_input->setSliderPosition(
        static_cast<int>(working_sprite->getSprite()->getBrightness()*100));
  connect(brightness_input,SIGNAL(valueChanged(int)),
          working_sprite,SLOT(setBrightness(int)));
  layout->addWidget(brightness_input,2,1);
  QLabel* brightness = new QLabel(QString::number(brightness_input->value()));
  connect(brightness_input,SIGNAL(valueChanged(int)),
          brightness,SLOT(setNum(int)));
  layout->addWidget(brightness,2,2);

  /* Opacity input */
  QLabel* opacity_label = new QLabel("Opacity");
  layout->addWidget(opacity_label,3,0);
  opacity_input = new QSlider(Qt::Horizontal);
  opacity_input->setTickInterval(1);
  opacity_input->setMinimum(0);
  opacity_input->setMaximum(255);
  opacity_backup = working_sprite->getSprite()->getOpacity();
  opacity_input->setSliderPosition(working_sprite->getSprite()->getOpacity());
  connect(opacity_input,SIGNAL(valueChanged(int)),
          working_sprite,SLOT(setOpacity(int)));
  layout->addWidget(opacity_input,3,1);
  QLabel* opacity = new QLabel(QString::number(opacity_input->value()));
  connect(opacity_input,SIGNAL(valueChanged(int)),
          opacity,SLOT(setNum(int)));
  layout->addWidget(opacity,3,2);

  /* Red input */
  QLabel* red_label = new QLabel("Red Balance");
  layout->addWidget(red_label,4,0);
  red_input = new QSlider(Qt::Horizontal);
  red_input->setTickInterval(1);
  red_input->setMinimum(0);
  red_input->setMaximum(255);
  red_backup = working_sprite->getSprite()->getColorRed();
  red_input->setSliderPosition(working_sprite->getSprite()->getColorRed());
  connect(red_input,SIGNAL(valueChanged(int)),
          working_sprite,SLOT(setColorRed(int)));
  layout->addWidget(red_input,4,1);
  QLabel* red = new QLabel(QString::number(red_input->value()));
  connect(red_input,SIGNAL(valueChanged(int)),
          red,SLOT(setNum(int)));
  layout->addWidget(red,4,2);

  /* Blue input */
  QLabel* blue_label = new QLabel("Blue Balance");
  layout->addWidget(blue_label,5,0);
  blue_input = new QSlider(Qt::Horizontal);
  blue_input->setTickInterval(1);
  blue_input->setMinimum(0);
  blue_input->setMaximum(255);
  blue_backup = working_sprite->getSprite()->getColorBlue();
  blue_input->setSliderPosition(working_sprite->getSprite()->getColorBlue());
  connect(blue_input,SIGNAL(valueChanged(int)),
          working_sprite,SLOT(setColorBlue(int)));
  layout->addWidget(blue_input,5,1);
  QLabel* blue = new QLabel(QString::number(blue_input->value()));
  connect(blue_input,SIGNAL(valueChanged(int)),
          blue,SLOT(setNum(int)));
  layout->addWidget(blue,5,2);

  /* Green input */
  QLabel* green_label = new QLabel("Green Balance");
  layout->addWidget(green_label,6,0);
  green_input = new QSlider(Qt::Horizontal);
  green_input->setTickInterval(1);
  green_input->setMinimum(0);
  green_input->setMaximum(255);
  green_backup = working_sprite->getSprite()->getColorGreen();
  green_input->setSliderPosition(working_sprite->getSprite()->getColorGreen());
  connect(green_input,SIGNAL(valueChanged(int)),
          working_sprite,SLOT(setColorGreen(int)));
  layout->addWidget(green_input,6,1);
  QLabel* green = new QLabel(QString::number(green_input->value()));
  connect(green_input,SIGNAL(valueChanged(int)),
          green,SLOT(setNum(int)));
  layout->addWidget(green,6,2);

  /* Animation time input */
  QLabel* time_label = new QLabel("Animation Time:");
  layout->addWidget(time_label,7,0);
  time_backup =QString::number(working_sprite->getSprite()->getAnimationTime());
  QLineEdit* time_input = new QLineEdit(QString::number(working_sprite->
                                                        getSprite()->
                                                        getAnimationTime()));
  time_input->setInputMask("99999");
  time_input->setFixedWidth(48);
  connect(time_input,SIGNAL(textChanged(QString)),
          working_sprite,SLOT(setAnimationTime(QString)));
  layout->addWidget(time_input,7,1);

  /* Directional input */
  QLabel* direction_label = new QLabel("Direction:");
  layout->addWidget(direction_label,8,0);
  QComboBox* direction_input = new QComboBox();
  direction_input->addItem("Forward");
  direction_input->addItem("Reverse");
  direction_backup = !working_sprite->getSprite()->isDirectionForward();
  if(working_sprite->getSprite()->isDirectionForward())
    direction_input->setCurrentIndex(0);
  else
    direction_input->setCurrentIndex(1);
  connect(direction_input,SIGNAL(currentIndexChanged(int)),
          working_sprite,SLOT(setDirection(int)));
  layout->addWidget(direction_input,8,1);

  /* Rotation input */
  QLabel* rotation_label = new QLabel("Rotation:");
  layout->addWidget(rotation_label,9,0);
  rotation_backup = QString::number(working_sprite->getSprite()->getRotation());
  QLineEdit* rotation_input = new QLineEdit(QString::number(working_sprite->
                                                            getSprite()->
                                                            getRotation()));
  rotation_input->setInputMask("999");
  rotation_input->setFixedWidth(48);
  connect(rotation_input,SIGNAL(textChanged(QString)),
          working_sprite,SLOT(setRotation(QString)));
  layout->addWidget(rotation_input,9,1);

  /* Ok button */
  QPushButton* ok = new QPushButton("Ok",this);
  ok->setFixedWidth(64);
  layout->addWidget(ok,10,0);
  connect(ok,SIGNAL(clicked()),this,SLOT(updateWorkingSprite()));

  /* Cancel button */
  QPushButton* cancel = new QPushButton("Cancel",this);
  cancel->setFixedWidth(64);
  layout->addWidget(cancel,10,1);
  connect(cancel,SIGNAL(clicked()),this,SLOT(destroyWorkingSprite()));

  setLayout(layout);

  if(creation_mode)
  {
    connect(this,SIGNAL(sendUpEditorSprite(EditorSprite*)),parent,
            SIGNAL(sendUpEditorSprite(EditorSprite*)));
  }
}

/*
 * Description: Loads a working sprite
 *
 * Input: Editor sprite
 */
void SpriteCreationDialog::loadWorkingSprite(EditorSprite *s)
{
  working_sprite = s;
}

/*
 * Description: Destroys the working sprite
 */
void SpriteCreationDialog::destroyWorkingSprite()
{
  if(creation_mode)
    delete working_sprite;
  else
  {
    working_sprite->setName(name_backup);
    working_sprite->setBrightness(brightness_backup);
    working_sprite->setOpacity(opacity_backup);
    working_sprite->setColorRed(red_backup);
    working_sprite->setColorBlue(blue_backup);
    working_sprite->setColorGreen(green_backup);
    working_sprite->setAnimationTime(time_backup);
    working_sprite->setDirection(direction_backup);
    working_sprite->setRotation(rotation_backup);
  }
  close();
}

/*
 * Description: Emits the sprite with the new changes and closes the dialog
 */
void SpriteCreationDialog::updateWorkingSprite()
{
  if(creation_mode)
    emit sendUpEditorSprite(working_sprite);
  else
  {
    emit ok();
  }
  //delete working_sprite;
  close();
}
