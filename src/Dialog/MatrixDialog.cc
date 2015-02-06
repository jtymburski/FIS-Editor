/*******************************************************************************
 * Class Name: MatrixDialog
 * Date Created: February 5, 2015
 * Inheritance: QDialog
 * Description: The sprite matrix dialog, which handles loading in a matrix 
 *              frame, as per the standard Univursa naming convention. Used
 *              in ThingDialog and all children.
 ******************************************************************************/
#include "Dialog/MatrixDialog.h"
#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function
 *
 * Inputs: QString path - the base path initially chosen 
 *         QWidget* parent - the parent class of the dialog
 */
MatrixDialog::MatrixDialog(QString path, QWidget* parent) : QDialog(parent)
{
  /* Layout setup */
  QGridLayout* layout = new QGridLayout(this);
  layout->setSizeConstraint(QLayout::SetFixedSize);

  /* Image label */
  QLabel* scene_lbl = new QLabel(this);
  QImage scene_img(320, 320, QImage::Format_ARGB32);
  scene_lbl->setPixmap(QPixmap::fromImage(scene_img));
  scene_lbl->setStyleSheet("border: 2px solid black");
  scene_lbl->setMinimumWidth(320);
  scene_lbl->setMinimumHeight(320);
  layout->addWidget(scene_lbl, 0, 0, 6, 1);

  /* Path label */
  QLabel* initial_lbl = new QLabel("Filename: " + path, this);
  //initial_lbl->setMinimumWidth(320);
  layout->addWidget(initial_lbl, 0, 1, 1, 4);

  /* Horizontal width control */
  QLabel* width_lbl_1 = new QLabel("Horizontal", this);
  layout->addWidget(width_lbl_1, 1, 1);
  QComboBox* width_from_combo = new QComboBox(this);
  fillDropDown(width_from_combo);
  layout->addWidget(width_from_combo, 1, 2);
  QLabel* width_lbl_2 = new QLabel("TO", this);
  layout->addWidget(width_lbl_2, 1, 3);
  QComboBox* width_to_combo = new QComboBox(this);
  fillDropDown(width_to_combo, width_from_combo->currentText());
  layout->addWidget(width_to_combo, 1, 4);

  /* Vertical height control */
  QLabel* height_lbl_1 = new QLabel("Vertical", this);
  layout->addWidget(height_lbl_1, 2, 1);
  QComboBox* height_from_combo = new QComboBox(this);
  fillDropDown(height_from_combo);
  layout->addWidget(height_from_combo, 2, 2);
  QLabel* height_lbl_2 = new QLabel("TO", this);
  layout->addWidget(height_lbl_2, 2, 3);
  QComboBox* height_to_combo = new QComboBox(this);
  fillDropDown(height_to_combo, height_from_combo->currentText());
  layout->addWidget(height_to_combo, 2, 4);

  /* Number of frames */
  QLabel* num_lbl = new QLabel("Number of Frames", this);
  layout->addWidget(num_lbl, 3, 1, 1, 2);
  QSpinBox* num_spinner = new QSpinBox(this);
  num_spinner->setMinimum(1);
  num_spinner->setMaximum(99);
  layout->addWidget(num_spinner, 3, 3, 1, 2);

  /* Result label */
  QLabel* result_lbl = new QLabel("Result: TODO", this);
  layout->addWidget(result_lbl, 4, 1, 1, 4);

  /* Buttons at bottom */
  QPushButton* button_trim = new QPushButton("Trim", this);
  layout->addWidget(button_trim, 5, 2);
  QPushButton* button_ok = new QPushButton("Ok", this);
  layout->addWidget(button_ok, 5, 3);
  QPushButton* button_cancel = new QPushButton("Cancel", this);
  layout->addWidget(button_cancel, 5, 4);

  /* Dialog control */
  setLayout(layout);
  //setFixedSize(320 * 2 + 5 * 3, 320 + 5 * 2);
}
/*FrameDialog::FrameDialog(QWidget *parent, EditorSprite* s, int framenum)
           : QDialog(parent)
{
  framenumber = framenum;
  sprite_original = s;
  sprite_working = new EditorSprite();

  if(s != NULL)
  {
    *sprite_working = *s;
    framelabel = new FrameView(this, EditorEnumDb::VIEWONLY, sprite_working,
                               framenumber, 0, 0);

    QCheckBox* horizontal_flip = new QCheckBox("Horizontal Flip", this);
    horizontal_flip->setChecked(sprite_working->getHorizontalFlip(framenumber));
    QCheckBox* vertical_flip = new QCheckBox("Vertical Flip", this);
    vertical_flip->setChecked(sprite_working->getVerticalFlip(framenumber));
    connect(horizontal_flip, SIGNAL(toggled(bool)),
            this, SLOT(setHorizontalFlip(bool)));
    connect(vertical_flip, SIGNAL(toggled(bool)),
            this, SLOT(setVerticalFlip(bool)));

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

    QPushButton* ok = new QPushButton("Ok", this);
    connect(ok,SIGNAL(pressed()),this,SLOT(finishSave()));
    connect(ok,SIGNAL(pressed()),this,SLOT(close()));

    QPushButton* cancel = new QPushButton("Cancel", this);
    connect(cancel,SIGNAL(pressed()),this,SLOT(close()));

    QPushButton* replace = new QPushButton("Replace Frame", this);
    connect(replace,SIGNAL(pressed()),this,SLOT(replaceFrame()));

    QPushButton* deleteframe = new QPushButton("Delete This Frame", this);
    connect(deleteframe,SIGNAL(pressed()),this,SLOT(deleteFrame()));
    if(s->frameCount() <= 1)
      deleteframe->setEnabled(false);

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
}*/

/*
 * Description: Destructor Function
 */
MatrixDialog::~MatrixDialog()
{

}
//FrameDialog::~FrameDialog()
//{
//  delete sprite_working;
//}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* Fills the drop down passed in with all the characters of the alphabet */
void MatrixDialog::fillDropDown(QComboBox* box, QString starting_letter)
{
  
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

/*
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

void FrameDialog::finishSave()
{
  *sprite_original = *sprite_working;
}

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

void FrameDialog::setHorizontalFlip(bool toggle)
{
  sprite_working->setHorizontalFlip(framenumber, toggle);
  framelabel->update();
}

void FrameDialog::setVerticalFlip(bool toggle)
{
  sprite_working->setVerticalFlip(framenumber, toggle);
  framelabel->update();
}

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
*/
