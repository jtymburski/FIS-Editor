/*******************************************************************************
 * Class Name: FrameDialog
 * Date Created: January 19, 2014
 * Inheritance: QWidget
 * Description: The frame dialog in the sprite creation dialog
 ******************************************************************************/
#include "Dialog/FrameDialog.h"
#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function
 *
 * Input: Parent, editor sprite to work on, image path
 */
FrameDialog::FrameDialog(QWidget *parent, EditorSprite* s, int framenum)
           : QDialog(parent)
{
  framenumber = framenum;
  sprite_original = s;
  sprite_working = new EditorSprite();

  /* Only proceed if sprite isn't NULL */
  if(s != NULL)
  {
    *sprite_working = *s;
    framelabel = new FrameView(this, EditorEnumDb::VIEWONLY, sprite_working,
                               framenumber, 0, 0);

    /* Checkboxes for flipping */
    QCheckBox* horizontal_flip = new QCheckBox("Horizontal Flip", this);
    horizontal_flip->setChecked(sprite_working->getHorizontalFlip(framenumber));
    QCheckBox* vertical_flip = new QCheckBox("Vertical Flip", this);
    vertical_flip->setChecked(sprite_working->getVerticalFlip(framenumber));
    connect(horizontal_flip, SIGNAL(toggled(bool)),
            this, SLOT(setHorizontalFlip(bool)));
    connect(vertical_flip, SIGNAL(toggled(bool)),
            this, SLOT(setVerticalFlip(bool)));

    /* Checkboxes for rotation */
    QCheckBox* rotate0 = new QCheckBox("0", this);
    rotate0->setChecked(sprite_working->getFrameAngle(framenumber) == 0);
    rotate0->setAutoExclusive(true);
    QCheckBox* rotate90 = new QCheckBox("90", this);
    rotate90->setChecked(sprite_working->getFrameAngle(framenumber) == 90);
    rotate90->setAutoExclusive(true);
    QCheckBox* rotate180 = new QCheckBox("180", this);
    rotate180->setChecked(sprite_working->getFrameAngle(framenumber) == 180);
    rotate180->setAutoExclusive(true);
    QCheckBox* rotate270 = new QCheckBox("270", this);
    rotate270->setChecked(sprite_working->getFrameAngle(framenumber) == 270);
    rotate270->setAutoExclusive(true);
    connect(rotate0, SIGNAL(clicked()), this, SLOT(set0()));
    connect(rotate90, SIGNAL(clicked()), this, SLOT(set90()));
    connect(rotate180, SIGNAL(clicked()), this, SLOT(set180()));
    connect(rotate270, SIGNAL(clicked()), this, SLOT(set270()));

    /* Ok button setup */
    QPushButton* ok = new QPushButton("Ok", this);
    connect(ok,SIGNAL(pressed()),this,SLOT(finishSave()));
    connect(ok,SIGNAL(pressed()),this,SLOT(close()));

    /* Cancel button setup */
    QPushButton* cancel = new QPushButton("Cancel", this);
    connect(cancel,SIGNAL(pressed()),this,SLOT(close()));

    /* Replace button setup */
    QPushButton* replace = new QPushButton("Replace Frame", this);
    connect(replace,SIGNAL(pressed()),this,SLOT(replaceFrame()));

    /* Delete button setup */
    QPushButton* deleteframe = new QPushButton("Delete This Frame", this);
    connect(deleteframe,SIGNAL(pressed()),this,SLOT(deleteFrame()));
    if(s->frameCount() <= 1)
      deleteframe->setEnabled(false);

    /* Set up the layout */
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

    layout->addWidget(framelabel, 0, Qt::AlignCenter);
    layout->addWidget(replace);
    layout->addWidget(deleteframe);
    layout->addLayout(checkbox_layout);
    layout->addLayout(buttonlayout);

    setLayout(layout);
  }
}

/*
 * Description: Destructor Function
 */
FrameDialog::~FrameDialog()
{
  delete sprite_working;
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

/*
 * Description : Deletes this current frame from the sequence
 */
void FrameDialog::deleteFrame()
{
  QMessageBox::StandardButton ret;
  ret = QMessageBox::warning(this, tr("Application"),
                             tr("Are you sure you want to delete this frame?"),
                             QMessageBox::Yes | QMessageBox::Cancel);
  if(ret == QMessageBox::Yes)
  {
    sprite_working->deleteFrame(framenumber);
    *sprite_original = *sprite_working;
    finishSave();
    close();
  }
}

/* Slot for finishing the saving of changes to the sequence */
void FrameDialog::finishSave()
{
  *sprite_original = *sprite_working;
}

/*
 * Description : Replaces the current frame with a new path
 */
void FrameDialog::replaceFrame()
{
  QString filename = QFileDialog::getOpenFileName(this,
                                   tr("Select A Frame To Replace This"),
                                   sprite_working->getPath(framenumber),
                                   tr("Image Files (*.png)"));
  if(filename != "")
  {
    sprite_working->setFramePath(framenumber,filename);
    framelabel->update();
  }
}

/*
 * Description: Sets the horizontal flip
 *
 * Input: Flip toggle
 */
void FrameDialog::setHorizontalFlip(bool toggle)
{
  sprite_working->setHorizontalFlip(framenumber, toggle);
  framelabel->update();
}

/*
 * Description: Sets the vertical flip
 *
 * Input: Flip toggle
 */
void FrameDialog::setVerticalFlip(bool toggle)
{
  sprite_working->setVerticalFlip(framenumber, toggle);
  framelabel->update();
}

/*
 * Description: Sets the angle
 *
 * Input: Angle string
 */
void FrameDialog::set0()
{
  sprite_working->setFrameAngle(framenumber, 0);
  framelabel->update();
}

void FrameDialog::set90()
{
  sprite_working->setFrameAngle(framenumber, 90);
  framelabel->update();
}

void FrameDialog::set180()
{
  sprite_working->setFrameAngle(framenumber, 180);
  framelabel->update();
}

void FrameDialog::set270()
{
  sprite_working->setFrameAngle(framenumber, 270);
  framelabel->update();
}
