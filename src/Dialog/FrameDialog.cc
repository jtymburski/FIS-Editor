/*******************************************************************************
 * Class Name: FrameDialog
 * Date Created: January 19, 2014
 * Inheritance: QWidget
 * Description: The frame dialog in the sprite creation dialog
 ******************************************************************************/
#include "Dialog/FrameDialog.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function
 *
 * Input: Parent, editor sprite to work on, image path
 */
FrameDialog::FrameDialog(QWidget *parent,
                                                 EditorSprite* s, int framenum)
  : QDialog(parent)
{
  setFixedSize(256,256);
  framenumber = framenum;
  sprite = s;
  framelabel = new FrameView(0,EditorEnumDb::VIEWONLY,
                               new QPixmap(s->getPath(framenumber)),
                               framenumber,0,0,QDir::current().absolutePath(),
                               s);
  framelabel->setHFlip(sprite->getHorizontalFlip(framenumber));
  framelabel->setVFlip(sprite->getVerticalFlip(framenumber));

  QCheckBox* horizontal_flip = new QCheckBox("Horizontal Flip",this);
  QCheckBox* vertical_flip = new QCheckBox("Vertical Flip",this);


  rotate0 = new QCheckBox("0",this);
  if(sprite->getFrameAngle(framenumber) == 0)
  {
    rotate0_backup = true;
    rotate90_backup = false;
    rotate180_backup = false;
    rotate270_backup = false;
    rotate0->setChecked(true);
  }
  rotate0->setAutoExclusive(true);

  rotate90 = new QCheckBox("90",this);
  if(sprite->getFrameAngle(framenumber) == 90)
  {
    rotate0_backup = false;
    rotate90_backup = true;
    rotate180_backup = false;
    rotate270_backup = false;
    rotate90->setChecked(true);
  }
  rotate90->setAutoExclusive(true);

  rotate180 = new QCheckBox("180",this);
  if(sprite->getFrameAngle(framenumber) == 180)
  {
    rotate0_backup = false;
    rotate90_backup = false;
    rotate180_backup = true;
    rotate270_backup = false;
    rotate180->setChecked(true);
  }
  rotate180->setAutoExclusive(true);

  rotate270 = new QCheckBox("270",this);
  if(sprite->getFrameAngle(framenumber) == 270)
  {
    rotate0_backup = false;
    rotate90_backup = false;
    rotate180_backup = false;
    rotate270_backup = true;
    rotate270->setChecked(true);
  }
  rotate270->setAutoExclusive(true);
  connect(rotate0,SIGNAL(clicked()),this,SLOT(set0()));
  connect(rotate90,SIGNAL(clicked()),this,SLOT(set90()));
  connect(rotate180,SIGNAL(clicked()),this,SLOT(set180()));
  connect(rotate270,SIGNAL(clicked()),this,SLOT(set270()));


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

  ok = new QPushButton("Ok",this);
  connect(ok,SIGNAL(pressed()),this,SIGNAL(finishedSave()));
  connect(ok,SIGNAL(pressed()),this,SLOT(close()));

  cancel = new QPushButton("Cancel",this);
  connect(cancel,SIGNAL(pressed()),this,SLOT(closeNoSave()));

  QPushButton* replace = new QPushButton("Replace Frame",this);
  connect(replace,SIGNAL(pressed()),this,SLOT(replaceFrame()));

  QPushButton* deleteframe = new QPushButton("Delete This Frame",this);

  /* If the framecount is only one, then grey out the delete functionality */
  if(s->frameCount() > 1)
    connect(deleteframe,SIGNAL(pressed()),this,SLOT(deleteFrame()));
  else
    deleteframe->setEnabled(false);

  /* Sets up layouts */
  QVBoxLayout* layout = new QVBoxLayout(this);

  QVBoxLayout* rotation_layout = new QVBoxLayout();
  rotation_layout->addWidget(rotate0);
  rotation_layout->addWidget(rotate90);
  rotation_layout->addWidget(rotate180);
  rotation_layout->addWidget(rotate270);

  QVBoxLayout* flip_layout = new QVBoxLayout();
  flip_layout->addWidget(horizontal_flip);
  flip_layout->addWidget(vertical_flip);


  QHBoxLayout* checkbox_layout = new QHBoxLayout();
  checkbox_layout->addLayout(rotation_layout);
  checkbox_layout->addLayout(flip_layout);

  QHBoxLayout* buttonlayout = new QHBoxLayout();
  buttonlayout->addWidget(ok);
  buttonlayout->addWidget(cancel);

  layout->addWidget(framelabel);
  layout->addWidget(replace);
  layout->addWidget(deleteframe);
  layout->addLayout(checkbox_layout);
  layout->addLayout(buttonlayout);

  setLayout(layout);

}

/*
 * Description: Destructor Function
 */
FrameDialog::~FrameDialog()
{
}

/*
 * Description: Sets the horizontal flip
 *
 * Input: Flip toggle
 */
void FrameDialog::setHorizontalFlip(bool toggle)
{
  sprite->setHorizontalFlip(framenumber,toggle);
  framelabel->setHFlip(toggle);
}

/*
 * Description: Sets the vertical flip
 *
 * Input: Flip toggle
 */
void FrameDialog::setVerticalFlip(bool toggle)
{
  sprite->setVerticalFlip(framenumber,toggle);
  framelabel->setVFlip(toggle);
}

/*
 * Description: Sets the angle
 *
 * Input: Angle string
 */
void FrameDialog::set0()
{
  sprite->setFrameAngle(framenumber,0);
  framelabel->update();
}
void FrameDialog::set90()
{
  sprite->setFrameAngle(framenumber,90);
  framelabel->update();
}
void FrameDialog::set180()
{
  sprite->setFrameAngle(framenumber,180);
  framelabel->update();
}
void FrameDialog::set270()
{
  sprite->setFrameAngle(framenumber,270);
  framelabel->update();
}

/*
 * Description: Closes the dialog and reverts all of the input values
 */
void FrameDialog::closeNoSave()
{
  sprite->setHorizontalFlip(framenumber,horizontal_backup);
  sprite->setVerticalFlip(framenumber,vertical_backup);
  if(rotate0_backup)
    sprite->setFrameAngle(framenumber,0);
  if(rotate90_backup)
    sprite->setFrameAngle(framenumber,90);
  if(rotate180_backup)
    sprite->setFrameAngle(framenumber,180);
  if(rotate270_backup)
    sprite->setFrameAngle(framenumber,270);
  close();
}

/*
 * Description : Replaces the current frame with a new path
 */
void FrameDialog::replaceFrame()
{
  QString filename = QFileDialog::getOpenFileName(this,
                                   tr("Select A Frame To Replace This"),
                                   sprite->getPath(framenumber)
                                   ,tr("Image Files (*.png)"));
  sprite->setFramePath(framenumber,filename);
  framelabel->reloadFrame();
}

/*
 * Description : Deletes this current frame from the sequence
 */
void FrameDialog::deleteFrame()
{
  QMessageBox::StandardButton ret;
  ret = QMessageBox::warning(this, tr("Application"),
                             tr("Are you sure you want to delete this frame?"),
                             QMessageBox::Yes | QMessageBox::Cancel);
  qDebug()<<"Delete?";
  if(ret == QMessageBox::Yes)
  {
    sprite->deleteFrame(framenumber);
    emit finishedSave();
    close();
  }
}
