/*******************************************************************************
 * Class Name: FrameManipulationDialog
 * Date Created: January 19, 2014
 * Inheritance: QWidget
 * Description: The frame dialog in the sprite creation dialog
 ******************************************************************************/
#include "FrameManipulationDialog.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function
 *
 * Input: Parent, editor sprite to work on, image path
 */
FrameManipulationDialog::FrameManipulationDialog(QWidget *parent,
                                                 EditorSprite* s, int framenum)
  : QDialog(parent)
{
  setFixedSize(256,256);
  sprite = s;
  framenumber = framenum;
  QVBoxLayout* layout = new QVBoxLayout(this);
  QCheckBox* horizontal_flip = new QCheckBox("Horizontal Flip",this);
  QCheckBox* vertical_flip = new QCheckBox("Vertical Flip",this);
  QLineEdit* angleinput = new QLineEdit(this);
  angle_backup = sprite->getFrameAngle(framenumber);
  angleinput->setText(QString::number(sprite->getFrameAngle(framenumber)));

  if(sprite->getHorizontalFlip(framenumber))
  {
    horizontal_backup = true;
    horizontal_flip->setChecked(true);
  }
  else
  {
    horizontal_backup = false;
    horizontal_flip->setChecked(false);
  }

  if(sprite->getVerticalFlip(framenumber))
  {
    vertical_backup = true;
    vertical_flip->setChecked(true);
  }
  else
  {
    vertical_backup = false;
    vertical_flip->setChecked(false);
  }

  connect(horizontal_flip,SIGNAL(toggled(bool)),
          this,SLOT(setHorizontalFlip(bool)));
  connect(vertical_flip,SIGNAL(toggled(bool)),
          this,SLOT(setVerticalFlip(bool)));
  connect(angleinput,SIGNAL(textChanged(QString)),
          this,SLOT(setAngle(QString)));

  ok = new QPushButton("Ok",this);
  connect(ok,SIGNAL(pressed()),this,SIGNAL(finishedSave()));
  connect(ok,SIGNAL(pressed()),this,SLOT(close()));

  cancel = new QPushButton("Cancel",this);
  connect(cancel,SIGNAL(pressed()),this,SLOT(closeNoSave()));
  layout->addWidget(angleinput);
  layout->addWidget(horizontal_flip);
  layout->addWidget(vertical_flip);
  layout->addWidget(ok);
  layout->addWidget(cancel);
  setLayout(layout);

}

/*
 * Description: Destructor Function
 */
FrameManipulationDialog::~FrameManipulationDialog()
{
}

/*
 * Description: Sets the horizontal flip
 *
 * Input: Flip toggle
 */
void FrameManipulationDialog::setHorizontalFlip(bool toggle)
{
  sprite->setHorizontalFlip(framenumber,toggle);
}

/*
 * Description: Sets the vertical flip
 *
 * Input: Flip toggle
 */
void FrameManipulationDialog::setVerticalFlip(bool toggle)
{
  sprite->setVerticalFlip(framenumber,toggle);
}

/*
 * Description: Sets the angle
 *
 * Input: Angle string
 */
void FrameManipulationDialog::setAngle(QString angle)
{
  sprite->setFrameAngle(framenumber,angle.toInt());
}

/*
 * Description: Closes the dialog and reverts all of the input values
 */
void FrameManipulationDialog::closeNoSave()
{
  sprite->setHorizontalFlip(framenumber,horizontal_backup);
  sprite->setVerticalFlip(framenumber,vertical_backup);
  sprite->setFrameAngle(framenumber,angle_backup);
  close();
}
