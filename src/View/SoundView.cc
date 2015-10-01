/*******************************************************************************
 * Class Name: SoundView
 * Date Created: September 28, 2015
 * Inheritance: QWidget
 * Description: The controller which takes an EditorSound and offers editing,
 *              playing, and saving capacity.
 ******************************************************************************/
#include "View/SoundView.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor Function */
SoundView::SoundView(QWidget* parent) : QWidget(parent)
{
  sound_base = nullptr;
  sound_curr = nullptr;

  createLayout();
  loadWorkingInfo();
}

/* Constructor function with Editor Sound reference */
SoundView::SoundView(EditorSound* sound, QWidget* parent) : SoundView(parent)
{
  // TODO: deal with sound pointer
}

/* Copy constructor */
SoundView::SoundView(const SoundView &source) : SoundView()
{
  copySelf(source);
}

/* Destructor function */
SoundView::~SoundView()
{
  // TODO
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
  
/* Copy function, to be called by a copy or equal operator constructor */
void SoundView::copySelf(const SoundView &source)
{
  // TODO
}

/* Creates interface layout */
void SoundView::createLayout()
{
  /* Layout */
  QGridLayout* layout = new QGridLayout(this);
  layout->setColumnStretch(2, 1);
  //layout->setRowMinimumHeight(7, 15);
  //layout->setRowStretch(5, 1);

  /* Fonts */
  QFont bold_font;
  bold_font.setBold(true);

  /* Horizontal Separator */
  QFrame* frm_separator = new QFrame(this);
  frm_separator->setFrameShape(QFrame::VLine);
  frm_separator->setLineWidth(1);
  layout->addWidget(frm_separator, 0, 0, 6, 1);

  /* ID */
  QLabel* lbl_id = new QLabel(tr("ID"), this);
  layout->addWidget(lbl_id, 0, 1);
  QLabel* lbl_id_num = new QLabel("0000", this);
  layout->addWidget(lbl_id_num, 0, 2);

  /* Name */
  QLabel* lbl_name = new QLabel(tr("Name"), this);
  layout->addWidget(lbl_name, 1, 1);
  QTextEdit* edit_name = new QTextEdit(this);
  layout->addWidget(edit_name, 1, 2, 1, 3);

  /* File Name */
  QLabel* lbl_file = new QLabel(tr("File"), this);
  layout->addWidget(lbl_file, 2, 1);
  QTextEdit* edit_file = new QTextEdit(this);
  edit_file->setDisabled(true);
  layout->addWidget(edit_file, 2, 2, 1, 3);
  QPushButton* btn_file = new QPushButton(tr("Select"), this);
  layout->addWidget(btn_file, 2, 5);

  /* Volume */
  QLabel* lbl_vol = new QLabel(tr("Volume"), this);
  layout->addWidget(lbl_vol, 3, 1);
  QSlider* slid_vol = new QSlider(Qt::Horizontal, this);
  slid_vol->setMinimum(0);
  slid_vol->setMaximum(MIX_MAX_VOLUME);
  layout->addWidget(slid_vol, 3, 2, 1, 3);
  QLabel* lbl_vol_value = new QLabel("000", this);
  layout->addWidget(lbl_vol_value, 3, 5);

  /* Fade Value */
  QLabel* lbl_fade = new QLabel(tr("Fade"), this);
  layout->addWidget(lbl_fade, 4, 1);
  QSpinBox* spin_fade = new QSpinBox(this);
  spin_fade->setMinimum(0);
  spin_fade->setMaximum(10000);
  layout->addWidget(spin_fade, 4, 2);

  /* Horizontal Separator */
  QFrame* frm_separator1 = new QFrame(this);
  frm_separator1->setFrameShape(QFrame::VLine);
  frm_separator1->setLineWidth(1);
  layout->addWidget(frm_separator1, 0, 6, 6, 1);

  /* Player */
  QLabel* lbl_play = new QLabel(tr("Player"), this);
  lbl_play->setFont(bold_font);
  layout->addWidget(lbl_play, 0, 7, Qt::AlignHCenter);
  QPushButton* btn_play = new QPushButton(this);
  QPixmap img_icon(":/images/sound_play.png");
  QIcon btn_icon(img_icon);
  btn_play->setIcon(btn_icon);
  btn_play->setIconSize(img_icon.rect().size());
  layout->addWidget(btn_play, 1, 7, 2, 1, Qt::AlignHCenter);
  QPushButton* btn_repeat = new QPushButton("Repeat", this);
  btn_repeat->setMaximumWidth(img_icon.rect().width() + 20);
  layout->addWidget(btn_repeat, 3, 7, Qt::AlignHCenter);
  QSpinBox* spin_margin = new QSpinBox(this);
  spin_margin->setMinimum(0);
  spin_margin->setMinimum(10000);
  layout->addWidget(spin_margin, 4, 7, Qt::AlignHCenter);

  /* Horizontal Separator */
  QFrame* frm_separator2 = new QFrame(this);
  frm_separator2->setFrameShape(QFrame::VLine);
  frm_separator2->setLineWidth(1);
  layout->addWidget(frm_separator2, 0, 8, 6, 1);

  /* Save and Reset buttons */
  QPushButton* btn_reset = new QPushButton(tr("Reset"), this);
  layout->addWidget(btn_reset, 5, 4);
  QPushButton* btn_save = new QPushButton(tr("Save"), this);
  layout->addWidget(btn_save, 5, 5);

  /* Maximum widget limiters */
  setMaximumHeight(160);
  setMaximumWidth(700);
}

/* Loads working info into UI objects */
void SoundView::loadWorkingInfo()
{
  /* Widget enable / disable */
  if(sound_curr != nullptr)
    setEnabled(true);
  else
    setDisabled(true);
}

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/
  
/* Button Triggers */
//void btnItemAdd();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Loads the object data */
//void SoundView::load(XmlData data, int index)
//{
//
//}

/* Resets the working set trigger */
void SoundView::resetWorking()
{

}

/* Saves the object data */
//void SoundView::save(FileHandler* fh, bool game_only)
//{
//
//}

/* Saves the working set trigger */
void SoundView::saveWorking()
{

}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
  
/* The copy operator */
SoundView& SoundView::operator= (const SoundView &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
