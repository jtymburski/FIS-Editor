#include "Dialog/SpriteDialog.h"
#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function
 *
 * Input: Parent, editor sprite to work on, image path
 */
SpriteDialog::SpriteDialog(QWidget *parent,
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
      working_sprite->setPath(working_sprite->frameCount(), frame_temp);
      //working_sprite->setPath(frame_temp);
      //working_sprite->setImage(QImage(frame_temp));
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
  frame_scrollwrapper = new QScrollArea();
  frame_list = new FrameList(this,working_sprite);
  frame_scrollwrapper->setWidget(frame_list);
  frame_scrollwrapper->setBaseSize(360,128);
  frame_scrollwrapper->setFixedHeight(112);
  frame_scrollwrapper->setBackgroundRole(QPalette::Base);
  layout->addWidget(frame_scrollwrapper,1,0,1,3);

  /* Brightness input */
  QLabel* brightness_label = new QLabel("Brightness");
  layout->addWidget(brightness_label,2,0);
  brightness_input = new QSlider(Qt::Horizontal);
  brightness_input->setTickInterval(1);
  brightness_input->setMinimum(0);
  brightness_input->setMaximum(510);
  brightness_backup = static_cast<int>(working_sprite->getSprite()->
                                       getBrightness()*255);
  brightness_input->setSliderPosition(
        static_cast<int>(working_sprite->getSprite()->getBrightness()*255));
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
  opacity_input->setMaximum(100);
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

  /* Green input */
  QLabel* green_label = new QLabel("Green Balance");
  layout->addWidget(green_label,5,0);
  green_input = new QSlider(Qt::Horizontal);
  green_input->setTickInterval(1);
  green_input->setMinimum(0);
  green_input->setMaximum(255);
  green_backup = working_sprite->getSprite()->getColorGreen();
  green_input->setSliderPosition(working_sprite->getSprite()->getColorGreen());
  connect(green_input,SIGNAL(valueChanged(int)),
          working_sprite,SLOT(setColorGreen(int)));
  layout->addWidget(green_input,5,1);
  QLabel* green = new QLabel(QString::number(green_input->value()));
  connect(green_input,SIGNAL(valueChanged(int)),
          green,SLOT(setNum(int)));
  layout->addWidget(green,5,2);

  /* Blue input */
  QLabel* blue_label = new QLabel("Blue Balance");
  layout->addWidget(blue_label,6,0);
  blue_input = new QSlider(Qt::Horizontal);
  blue_input->setTickInterval(1);
  blue_input->setMinimum(0);
  blue_input->setMaximum(255);
  blue_backup = working_sprite->getSprite()->getColorBlue();
  blue_input->setSliderPosition(working_sprite->getSprite()->getColorBlue());
  connect(blue_input,SIGNAL(valueChanged(int)),
          working_sprite,SLOT(setColorBlue(int)));
  layout->addWidget(blue_input,6,1);
  QLabel* blue = new QLabel(QString::number(blue_input->value()));
  connect(blue_input,SIGNAL(valueChanged(int)),
          blue,SLOT(setNum(int)));
  layout->addWidget(blue,6,2);

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

  /* Sets up quick rotation for frames */
  QVBoxLayout* rotation_layout = new QVBoxLayout();

  rotate0 = new QCheckBox("0",this);
  if(working_sprite->getQuickRotation() == 0)
  {
    rotate0_backup = true;
    rotate90_backup = false;
    rotate180_backup = false;
    rotate270_backup = false;
    rotate0->setChecked(true);
  }
  rotate0->setAutoExclusive(true);

  rotate90 = new QCheckBox("90",this);
  if(working_sprite->getQuickRotation() == 90)
  {
    rotate0_backup = false;
    rotate90_backup = true;
    rotate180_backup = false;
    rotate270_backup = false;
    rotate90->setChecked(true);
  }
  rotate90->setAutoExclusive(true);

  rotate180 = new QCheckBox("180",this);
  if(working_sprite->getQuickRotation() == 180)
  {
    rotate0_backup = false;
    rotate90_backup = false;
    rotate180_backup = true;
    rotate270_backup = false;
    rotate180->setChecked(true);
  }
  rotate180->setAutoExclusive(true);

  rotate270 = new QCheckBox("270",this);
  if(working_sprite->getQuickRotation() == 270)
  {
    rotate0_backup = false;
    rotate90_backup = false;
    rotate180_backup = false;
    rotate270_backup = true;
    rotate270->setChecked(true);
  }
  rotate270->setAutoExclusive(true);

  /* Connects to the appropriate setting functions */
  connect(rotate0,SIGNAL(clicked()),working_sprite,SLOT(set0()));
  connect(rotate0,SIGNAL(clicked()),frame_list,SLOT(addFrames()));

  connect(rotate90,SIGNAL(clicked()),working_sprite,SLOT(set90()));
  connect(rotate90,SIGNAL(clicked()),frame_list,SLOT(addFrames()));

  connect(rotate180,SIGNAL(clicked()),working_sprite,SLOT(set180()));
  connect(rotate180,SIGNAL(clicked()),frame_list,SLOT(addFrames()));

  connect(rotate270,SIGNAL(clicked()),working_sprite,SLOT(set270()));
  connect(rotate270,SIGNAL(clicked()),frame_list,SLOT(addFrames()));

  rotation_layout->addWidget(rotate0);
  rotation_layout->addWidget(rotate90);
  rotation_layout->addWidget(rotate180);
  rotation_layout->addWidget(rotate270);

  /* Add a the rotation layout that spans 4 rows */
  layout->addLayout(rotation_layout,9,2,4,1);

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
  connect(working_sprite,SIGNAL(spriteChanged()),frame_list,SLOT(update()));
}

/*
 * Description: Loads a working sprite
 *
 * Input: Editor sprite
 */
void SpriteDialog::loadWorkingSprite(EditorSprite *s)
{
  working_sprite = s;
}

/*
 * Description: Destroys the working sprite
 */
void SpriteDialog::destroyWorkingSprite()
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
    if(rotate0_backup)
      working_sprite->set0();
    else if(rotate90_backup)
      working_sprite->set90();
    else if(rotate180_backup)
      working_sprite->set180();
    else if(rotate270_backup)
      working_sprite->set270();
  }
  close();
}

/*
 * Description: Emits the sprite with the new changes and closes the dialog
 */
void SpriteDialog::updateWorkingSprite()
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
