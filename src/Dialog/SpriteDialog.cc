/*******************************************************************************
 * Class Name: SpriteDialog
 * Date Created: January 22, 2014
 * Inheritance: QDialog
 * Description: The dialog that displays the sprites and the information
 *              related to it. Allows manipulation of the data.
 ******************************************************************************/
#include "Dialog/SpriteDialog.h"
#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function
 *
 * Input: QWidget* parent - parent class widget in QT
 *        EditorSprite* working - the working sprite to edit
 *        QString p - the path string
 *        int subsequent - number of subsequent frames
 *        bool creation - true if it's a new sprite
 *        EditorEnumDb::SpriteEditMode mode - the editing mode. default all
 */
SpriteDialog::SpriteDialog(QWidget *parent, EditorSprite *working, QString p,
                           int subsequent, bool creation, 
                           EditorEnumDb::SpriteEditMode mode)
            : QDialog(parent)
{
  /* Sets the working sprite and appropriate paths */
  original_sprite = working;
  working_sprite = new EditorSprite();
  if(!creation)
    *working_sprite = *working;
  creation_mode = creation;

  if(creation_mode)
  {
    if(subsequent > 0)
    {
      /* Sets the frames of the sequence */
      for(int i=0; i<subsequent; i++)
      {
        int dot_ref = p.lastIndexOf('.');
        QString tail_end = p.right(p.size() - dot_ref);
        QString frame_temp = p.left(dot_ref - 2);

        if(i<=9)
          frame_temp.append('0');
        frame_temp.append(QString::number(i));
        frame_temp.append(tail_end);
        working_sprite->setPath(working_sprite->frameCount(), frame_temp);
      }
    }
    else
    {
      working_sprite->setPath(working_sprite->frameCount(), p);
    }
  }

  /* Sets up a grid layout for the sliders and such */
  QGridLayout* layout = new QGridLayout();
  layout->setSpacing(5);
  layout->setAlignment(Qt::AlignRight);

  /* ID display */
  QString id_label("ID: ");
  id_label.append(QString::number(working_sprite->getID()));
  QLabel* id_number = new QLabel(id_label);
  layout->addWidget(id_number,0,0);

  /* Name input */
  QLabel* name = new QLabel("Name: ");
  layout->addWidget(name,0,1, Qt::AlignRight);
  QLineEdit* name_input = new QLineEdit(working_sprite->getName());
  name_input->setInputMask("xxxxxxxxxxxxxxxx");
  connect(name_input,SIGNAL(textChanged(QString)),
          working_sprite,SLOT(setName(QString)));
  layout->addWidget(name_input,0,2,1,2);

  /* Frame Display */
  frame_scrollwrapper = new QScrollArea();
  frame_list = new FrameList(this,working_sprite);
  frame_scrollwrapper->setWidget(frame_list);
  frame_scrollwrapper->setAlignment(Qt::AlignCenter);
  frame_scrollwrapper->setFixedHeight(105);
  frame_scrollwrapper->setMaximumWidth(446);
  frame_scrollwrapper->setBackgroundRole(QPalette::Base);
  layout->addWidget(frame_scrollwrapper,1,0,1,4);

  /* Brightness input */
  QLabel* brightness_label = new QLabel("Brightness");
  layout->addWidget(brightness_label,2,0);
  brightness_input = new QSlider(Qt::Horizontal);
  brightness_input->setTickInterval(1);
  brightness_input->setMinimum(0);
  brightness_input->setMaximum(510);
  connect(brightness_input,SIGNAL(valueChanged(int)),
          working_sprite,SLOT(setBrightness(int)));
  layout->addWidget(brightness_input,2,1,1,2);
  QLabel* brightness = new QLabel(QString::number(brightness_input->value()));
  connect(brightness_input,SIGNAL(valueChanged(int)),
          brightness,SLOT(setNum(int)));
  layout->addWidget(brightness,2,3);

  /* Opacity input */
  QLabel* opacity_label = new QLabel("Opacity");
  layout->addWidget(opacity_label,3,0);
  opacity_input = new QSlider(Qt::Horizontal);
  opacity_input->setTickInterval(1);
  opacity_input->setMinimum(0);
  opacity_input->setMaximum(100);
  connect(opacity_input,SIGNAL(valueChanged(int)),
          working_sprite,SLOT(setOpacity(int)));
  layout->addWidget(opacity_input,3,1,1,2);
  QLabel* opacity = new QLabel(QString::number(opacity_input->value()));
  connect(opacity_input,SIGNAL(valueChanged(int)),
          opacity,SLOT(setNum(int)));
  layout->addWidget(opacity,3,3);

  /* Red input */
  QLabel* red_label = new QLabel("Red Balance");
  layout->addWidget(red_label,4,0);
  red_input = new QSlider(Qt::Horizontal);
  red_input->setTickInterval(1);
  red_input->setMinimum(0);
  red_input->setMaximum(255);
  connect(red_input,SIGNAL(valueChanged(int)),
          working_sprite,SLOT(setColorRed(int)));
  layout->addWidget(red_input,4,1,1,2);
  QLabel* red = new QLabel(QString::number(red_input->value()));
  connect(red_input,SIGNAL(valueChanged(int)),
          red,SLOT(setNum(int)));
  layout->addWidget(red,4,3);

  /* Green input */
  QLabel* green_label = new QLabel("Green Balance");
  layout->addWidget(green_label,5,0);
  green_input = new QSlider(Qt::Horizontal);
  green_input->setTickInterval(1);
  green_input->setMinimum(0);
  green_input->setMaximum(255);
  connect(green_input,SIGNAL(valueChanged(int)),
          working_sprite,SLOT(setColorGreen(int)));
  layout->addWidget(green_input,5,1,1,2);
  QLabel* green = new QLabel(QString::number(green_input->value()));
  connect(green_input,SIGNAL(valueChanged(int)),
          green,SLOT(setNum(int)));
  layout->addWidget(green,5,3);

  /* Blue input */
  QLabel* blue_label = new QLabel("Blue Balance");
  layout->addWidget(blue_label,6,0);
  blue_input = new QSlider(Qt::Horizontal);
  blue_input->setTickInterval(1);
  blue_input->setMinimum(0);
  blue_input->setMaximum(255);
  connect(blue_input,SIGNAL(valueChanged(int)),
          working_sprite,SLOT(setColorBlue(int)));
  layout->addWidget(blue_input,6,1,1,2);
  QLabel* blue = new QLabel(QString::number(blue_input->value()));
  connect(blue_input,SIGNAL(valueChanged(int)),
          blue,SLOT(setNum(int)));
  layout->addWidget(blue,6,3);

  /* Animation time input */
  QLabel* time_label = new QLabel("Animation Time:");
  layout->addWidget(time_label,7,0);
  time_input = new QLineEdit(QString::number(working_sprite->getSprite()->
                                                           getAnimationTime()));
  time_input->setInputMask("99999");
  time_input->setMaximumWidth(99);
  connect(time_input,SIGNAL(textChanged(QString)),
          working_sprite,SLOT(setAnimationTime(QString)));
  layout->addWidget(time_input,7,1);
  QLabel* time_input_lbl = new QLabel(" ms");
  layout->addWidget(time_input_lbl,7,2);

  /* Directional input */
  QLabel* direction_label = new QLabel("Direction:");
  layout->addWidget(direction_label,8,0);
  direction_input = new QComboBox();
  direction_input->addItem("Forward");
  direction_input->addItem("Reverse");
  connect(direction_input,SIGNAL(currentIndexChanged(int)),
          working_sprite,SLOT(setDirection(int)));
  layout->addWidget(direction_input,8,1);

  /* Rotation input */
  QLabel* rotation_label = new QLabel("Rotation:");
  layout->addWidget(rotation_label,9,0);
  rotation_input = new QLineEdit(QString::number(working_sprite->getSprite()->
                                                                getRotation()));
  rotation_input->setInputMask("999");
  rotation_input->setMaximumWidth(99);
  connect(rotation_input,SIGNAL(textChanged(QString)),
          working_sprite,SLOT(setRotation(QString)));
  layout->addWidget(rotation_input,9,1);
  QLabel* degree_lbl = new QLabel(" degrees");
  layout->addWidget(degree_lbl,9,2);

  /* Spacing */
  QSpacerItem* item = new QSpacerItem(5, 15);
  layout->addItem(item, 10,0,1,4);

  /* Frame rotation add */
  QGroupBox *box = new QGroupBox("Frame Mods", this);
  QHBoxLayout *hlayout = new QHBoxLayout();
  QHBoxLayout *hlayout2 = new QHBoxLayout();
  QVBoxLayout *vlayout = new QVBoxLayout();
  QPushButton *rotate0 = new QPushButton("Rotate 0", this);
  QPushButton *rotate90 = new QPushButton("Rotate 90", this);
  QPushButton *rotate180 = new QPushButton("Rotate 180", this);
  QPushButton *rotate270 = new QPushButton("Rotate 270", this);
  QPushButton *flipH = new QPushButton("Flip Horizontal", this);
  QPushButton *flipV = new QPushButton("Flip Vertical", this);
  QPushButton *resetFlip = new QPushButton("Reset Flips", this);
  hlayout->addWidget(rotate0);
  hlayout->addWidget(rotate90);
  hlayout->addWidget(rotate180);
  hlayout->addWidget(rotate270);
  hlayout2->addWidget(flipH);
  hlayout2->addWidget(flipV);
  hlayout2->addWidget(resetFlip);
  vlayout->addLayout(hlayout);
  vlayout->addLayout(hlayout2);
  connect(rotate0,SIGNAL(clicked()),working_sprite,SLOT(set0()));
  connect(rotate90,SIGNAL(clicked()),working_sprite,SLOT(set90()));
  connect(rotate180,SIGNAL(clicked()),working_sprite,SLOT(set180()));
  connect(rotate270,SIGNAL(clicked()),working_sprite,SLOT(set270()));
  connect(flipH, SIGNAL(clicked()), working_sprite, SLOT(setHorizontalFlips()));
  connect(flipV, SIGNAL(clicked()), working_sprite, SLOT(setVerticalFlips()));
  connect(resetFlip, SIGNAL(clicked()), working_sprite, SLOT(resetFlips()));
  box->setLayout(vlayout);
  layout->addWidget(box, 11, 0, 1, 4);

  /* Spacing */
  QSpacerItem* item2 = new QSpacerItem(5, 30);
  layout->addItem(item2, 12,0,1,4);

  /* Ok button */
  QHBoxLayout* button_layout = new QHBoxLayout();
  QPushButton* ok = new QPushButton("Ok",this);
  ok->setDefault(true);
  ok->setFixedWidth(100);
  button_layout->addWidget(ok);
  connect(ok,SIGNAL(clicked()),this,SLOT(updateWorkingSprite()));

  /* Cancel button */
  QPushButton* cancel = new QPushButton("Cancel",this);
  cancel->setFixedWidth(100);
  button_layout->addWidget(cancel);
  connect(cancel,SIGNAL(clicked()),this,SLOT(destroyWorkingSprite()));

  /* Default button */
  QPushButton* default_btn = new QPushButton("Default",this);
  default_btn->setFixedWidth(100);
  button_layout->addWidget(default_btn);
  connect(default_btn, SIGNAL(clicked()), this, SLOT(setToDefault()));
  layout->addLayout(button_layout, 13, 0, 1, 4, Qt::AlignCenter);

  /* Set Layout */
  layout->setVerticalSpacing(10);
  layout->setSizeConstraint(QLayout::SetFixedSize);
  setLayout(layout);

  if(creation_mode)
  {
    connect(this,SIGNAL(sendUpEditorSprite(EditorSprite*)),parent,
            SIGNAL(sendUpEditorSprite(EditorSprite*)));
  }
  connect(working_sprite,SIGNAL(spriteChanged()),frame_list,SLOT(update()));

  /* Set up the sliders and inputs */
  setToWorking();

  /* Set disabled, based on mode */
  if(mode == EditorEnumDb::SPRITE_FRAMES)
  {
    brightness_input->setDisabled(true);
    opacity_input->setDisabled(true);
    red_input->setDisabled(true);
    blue_input->setDisabled(true);
    green_input->setDisabled(true);
    time_input->setDisabled(true);
    direction_input->setDisabled(true);
    rotation_input->setDisabled(true);
  }
  else if(mode == EditorEnumDb::SPRITE_DATA)
  {
    rotate0->setDisabled(true);
    rotate90->setDisabled(true);
    rotate180->setDisabled(true);
    rotate270->setDisabled(true);
    flipH->setDisabled(true);
    flipV->setDisabled(true);
    resetFlip->setDisabled(true);
   
    frame_list->setDisabled(true);
  }
}
  
/*
 * Description: Destructor function
 */
SpriteDialog::~SpriteDialog()
{
  if(working_sprite != NULL)
    delete working_sprite;
  working_sprite = NULL;
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: Set the working control for base sprite values to the working
 *              sprite values. Used on initial construction and updating.
 *
 * Inputs: none
 * Output: none
 */
void SpriteDialog::setToWorking()
{
  /* Brightness setup */
  brightness_input->setSliderPosition(
        static_cast<int>(working_sprite->getSprite()->getBrightness()*255));

  /* Opacity setup */
  opacity_input->setSliderPosition(working_sprite->getSprite()->getOpacity());

  /* Red green blue control setup */
  red_input->setSliderPosition(working_sprite->getSprite()->getColorRed());
  green_input->setSliderPosition(working_sprite->getSprite()->getColorGreen());
  blue_input->setSliderPosition(working_sprite->getSprite()->getColorBlue());

  /* Time input setup */
  time_input->setText(QString::number(working_sprite->getSprite()->
                                      getAnimationTime()));

  /* Direction control setup */
  if(working_sprite->getSprite()->isDirectionForward())
    direction_input->setCurrentIndex(0);
  else
    direction_input->setCurrentIndex(1);

  /* Rotation input setup */
  rotation_input->setText(QString::number(working_sprite->getSprite()->
                                                                getRotation()));
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: The custom close event for saving changes
 *
 * Inputs: The event passed
 * Output: none
 */
void SpriteDialog::closeEvent(QCloseEvent *event)
{
  if(working_sprite != NULL)
    delete working_sprite;
  working_sprite = NULL;

  event->accept();
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

/*
 * Description: Destroys the working sprite
 *
 * Inputs: none
 * Output: none
 */
void SpriteDialog::destroyWorkingSprite()
{
  delete working_sprite;
  working_sprite = NULL;

  close();
}

/*
 * Description: Sets the sprite back to default settings for the base values.
 *
 * Inputs: none
 * Output: none
 */
void SpriteDialog::setToDefault()
{
  EditorSprite* default_sprite = new EditorSprite();
  working_sprite->copyBaseSprite(*default_sprite);
  setToWorking();
  update();
  delete default_sprite;
}

/*
 * Description: Emits the sprite with the new changes and closes the dialog
 *
 * Inputs: none
 * Output: none
 */
void SpriteDialog::updateWorkingSprite()
{
  if(creation_mode)
  {
    emit sendUpEditorSprite(working_sprite);
  }
  else
  {
    *original_sprite = *working_sprite;
    delete working_sprite;
    emit ok();
  }

  working_sprite = NULL;
  close();
}
