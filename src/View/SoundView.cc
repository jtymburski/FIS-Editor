/*******************************************************************************
 * Class Name: SoundView
 * Date Created: September 28, 2015
 * Inheritance: QWidget
 * Description: The controller which takes an EditorSound and offers editing,
 *              playing, and saving capacity.
 ******************************************************************************/
#include "View/SoundView.h"
#include <QDebug>

/* Constant Implementation - see header file for descriptions */
const int SoundView::kFADE_INTERVAL = 25;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor Function */
SoundView::SoundView(QWidget* parent) : QWidget(parent)
{
  sound_base = nullptr;
  sound_curr = new EditorSound();
  timer_fade.setSingleShot(false);
  timer_replay.setSingleShot(true);
  vol_ref = 0.0;

  createLayout();

  // TODO: TESTING - REMOVE
  sound_base = new EditorSound(1200);
  *sound_curr = *sound_base;

  loadWorkingInfo();
}

/* Constructor function with Editor Sound reference */
SoundView::SoundView(EditorSound* sound, QWidget* parent) : SoundView(parent)
{
  setEditSound(sound);
}

/* Copy constructor */
SoundView::SoundView(const SoundView &source) : SoundView()
{
  copySelf(source);
}

/* Destructor function */
SoundView::~SoundView()
{
  sound_base = nullptr;

  delete sound_curr;
  sound_curr = nullptr;
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
  layout->setColumnStretch(3, 1);
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
  lbl_id_num = new QLabel("0000", this);
  lbl_id_num->setStyleSheet("border: 1px solid #afafaf");
  layout->addWidget(lbl_id_num, 0, 2);

  /* Name */
  QLabel* lbl_name = new QLabel(tr("Name"), this);
  layout->addWidget(lbl_name, 1, 1);
  edit_name = new QLineEdit(this);
  connect(edit_name, SIGNAL(textEdited(QString)),
          this, SLOT(changedName(QString)));
  layout->addWidget(edit_name, 1, 2, 1, 2);

  /* File Name */
  QLabel* lbl_file = new QLabel(tr("File"), this);
  layout->addWidget(lbl_file, 2, 1);
  lbl_file_name = new QLabel(this);
  lbl_file_name->setMinimumWidth(280);
  lbl_file_name->setStyleSheet("border: 1px solid #afafaf");
  layout->addWidget(lbl_file_name, 2, 2, 1, 3);
  btn_file = new QPushButton(tr("Select"), this);
  connect(btn_file, SIGNAL(clicked()), this, SLOT(btnFileSelect()));
  layout->addWidget(btn_file, 2, 5);

  /* Volume */
  QLabel* lbl_vol = new QLabel(tr("Volume"), this);
  layout->addWidget(lbl_vol, 3, 1);
  slid_vol = new QSlider(Qt::Horizontal, this);
  slid_vol->setMinimum(0);
  slid_vol->setMaximum(MIX_MAX_VOLUME);
  connect(slid_vol, SIGNAL(valueChanged(int)), this, SLOT(volumeChanged(int)));
  layout->addWidget(slid_vol, 3, 2, 1, 3);
  lbl_vol_value = new QLabel("000", this);
  layout->addWidget(lbl_vol_value, 3, 5);

  /* Fade Value */
  QLabel* lbl_fade = new QLabel(tr("Fade"), this);
  layout->addWidget(lbl_fade, 4, 1);
  spin_fade = new QSpinBox(this);
  spin_fade->setMinimum(0);
  spin_fade->setMaximum(10000);
  connect(spin_fade, SIGNAL(valueChanged(int)),
          this, SLOT(changedFadeTime(int)));
  layout->addWidget(spin_fade, 4, 2);

  /* Horizontal Separator */
  QFrame* frm_separator1 = new QFrame(this);
  frm_separator1->setFrameShape(QFrame::VLine);
  frm_separator1->setLineWidth(1);
  layout->addWidget(frm_separator1, 0, 6, 6, 1);

  /* Player  - Label */
  QLabel* lbl_play = new QLabel(tr("Player"), this);
  lbl_play->setFont(bold_font);
  layout->addWidget(lbl_play, 0, 7, Qt::AlignHCenter);

  /* Player - Play/Stop Button */
  btn_play = new QPushButton(this);
  //btn_play->setCheckable(true);
  QPixmap img_icon(":/images/sound_play.png");
  QIcon btn_icon(img_icon);
  btn_play->setIcon(btn_icon);
  btn_play->setIconSize(img_icon.rect().size());
  connect(btn_play, SIGNAL(clicked()), this, SLOT(btnPlay()));
  layout->addWidget(btn_play, 1, 7, 2, 1, Qt::AlignHCenter);

  /* Player - Repeat Button */
  btn_repeat = new QPushButton("Repeat", this);
  btn_repeat->setCheckable(true);
  btn_repeat->setMaximumWidth(img_icon.rect().width() + 20);
  connect(btn_repeat, SIGNAL(clicked(bool)), this, SLOT(btnRepeat(bool)));
  layout->addWidget(btn_repeat, 3, 7, Qt::AlignHCenter);

  /* Player - Delay Margin */
  spin_margin = new QSpinBox(this);
  spin_margin->setMinimum(0);
  spin_margin->setMaximum(10000);
  spin_margin->setDisabled(true);
  layout->addWidget(spin_margin, 4, 7, Qt::AlignHCenter);

  /* Horizontal Separator */
  QFrame* frm_separator2 = new QFrame(this);
  frm_separator2->setFrameShape(QFrame::VLine);
  frm_separator2->setLineWidth(1);
  layout->addWidget(frm_separator2, 0, 8, 6, 1);

  /* Save and Reset buttons */
  QPushButton* btn_reset = new QPushButton(tr("Reset"), this);
  connect(btn_reset, SIGNAL(clicked()), this, SLOT(btnReset()));
  layout->addWidget(btn_reset, 5, 4);
  QPushButton* btn_save = new QPushButton(tr("Save"), this);
  connect(btn_save, SIGNAL(clicked()), this, SLOT(btnSave()));
  layout->addWidget(btn_save, 5, 5);

  /* Final connections */
  connect(&player, SIGNAL(stateChanged(QMediaPlayer::State)),
          this, SLOT(playerStateChanged(QMediaPlayer::State)));
  connect(&timer_fade, SIGNAL(timeout()), this, SLOT(playerFadeIn()));
  connect(&timer_replay, SIGNAL(timeout()), this, SLOT(playerRepeat()));
}

/* Enable edit sound information widget portion */
void SoundView::enableEditWidgets(bool enable)
{
  /* Sound information */
  edit_name->setEnabled(enable);
  btn_file->setEnabled(enable);
  slid_vol->setEnabled(enable);
  spin_fade->setEnabled(enable);

  /* Play information */
  btn_repeat->setEnabled(enable);
  if(btn_repeat->isChecked())
    spin_margin->setEnabled(enable);
}

/* Loads working info into UI objects */
void SoundView::loadWorkingInfo()
{
  /* Load data */
  if(sound_curr != nullptr)
  {
    /* ID */
    lbl_id_num->setText(QString::number(sound_curr->getID()));

    /* Name */
    edit_name->setText(sound_curr->getName());

    /* File Path */
    lbl_file_name->setText(sound_curr->getFileName());
    if(!sound_curr->getFileName().isEmpty())
      player.setMedia(QUrl::fromLocalFile(EditorHelpers::getProjectDir() +
                                          "/" + sound_curr->getFileName()));

    /* Volume */
    slid_vol->setValue(sound_curr->getVolume());

    /* Fade Time */
    spin_fade->setValue((int)sound_curr->getFadeTime());

    /* Enable widget */
    setEnabled(true);
  }
  else
  {
    /* Disable widget */
    setDisabled(true);
  }
}

/* Stops playing audio, if relevant */
void SoundView::stop()
{
  /* Stop playing */
  timer_fade.stop();
  player.stop();
  timer_replay.stop();
  btn_play->setIcon(QIcon(QPixmap(":/images/sound_play.png")));
  enableEditWidgets(true);
}

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/

/* Button Triggers */
void SoundView::btnFileSelect()
{
  if(sound_curr != nullptr)
  {
    /* Get Path */
    QString file = QFileDialog::getOpenFileName(this, "Open",
                                      EditorHelpers::getProjectDir() + "/sound",
                                      tr("Sound Files (*.wav *.ogg)"));
    if(!file.isEmpty())
    {
      sound_curr->setFileNameFull(file);
      lbl_file_name->setText(sound_curr->getFileName());
      player.setMedia(QUrl::fromLocalFile(EditorHelpers::getProjectDir() +
                                          "/" + sound_curr->getFileName()));
    }
  }
}

/* Button Triggers */
void SoundView::btnPlay()
{
  /* Is Stopped - Can trigger a play */
  if(player.state() == QMediaPlayer::StoppedState && !timer_replay.isActive())
  {
    /* If the file name is set, trigger sound */
    if(!sound_curr->getFileName().isEmpty())
    {
      /* Set volume */
      if(spin_fade->value() == 0)
        player.setVolume(sound_curr->getVolumePercent());
      else
        player.setVolume(0);

      /* Trigger play */
      player.play();

      /* If volume is at 0, start fading timer */
      if(player.volume() == 0)
      {
        vol_ref = 0.0;
        timer_fade.start(kFADE_INTERVAL);
      }
    }
    /* Otherwise, pop up warning */
    else
    {
      QMessageBox::information(this, "Failed to Play",
                               "No sound file path was set");
    }
  }
  /* Is playing or paused - stop it */
  else
  {
    stop();
  }
}

/* Button Triggers */
void SoundView::btnRepeat(bool checked)
{
  spin_margin->setEnabled(checked);
}

/* Button Triggers */
void SoundView::btnReset()
{
  if(sound_curr != nullptr && sound_base != nullptr)
  {
    stop();
    *sound_curr = *sound_base;
    loadWorkingInfo();
  }
}

/* Button Triggers */
void SoundView::btnSave()
{
  if(sound_curr != nullptr && sound_base != nullptr)
  {
    *sound_base = *sound_curr;
  }
}

/* Fade Time Changed */
void SoundView::changedFadeTime(int time)
{
  if(sound_curr != nullptr && time >= 0)
  {
    sound_curr->setFadeTime(time);
  }
}

/* Name Text Changed */
void SoundView::changedName(QString name)
{
  if(sound_curr != nullptr)
  {
    sound_curr->setName(name);
    emit nameChange(name);
  }
}

/* Player triggers */
void SoundView::playerFadeIn()
{
  /* Initial calculations */
  bool finished = false;
  float time_split = spin_fade->value() * 1.0 / kFADE_INTERVAL;
  float vol_interval = sound_curr->getVolumePercent() / time_split;
  vol_ref += vol_interval;
  if((int)vol_ref > sound_curr->getVolumePercent())
  {
    finished = true;
    vol_ref = (int)sound_curr->getVolumePercent();
  }

  /* Change volume */
  player.setVolume((int)vol_ref);

  /* Stop timer, if relevant */
  if(finished)
    timer_fade.stop();
}

/* Player triggers */
void SoundView::playerRepeat()
{
  player.play();
}

/* Player triggers */
void SoundView::playerStateChanged(QMediaPlayer::State state)
{
  if(state == QMediaPlayer::PlayingState)
  {
    btn_play->setIcon(QIcon(QPixmap(":/images/sound_stop.png")));
    enableEditWidgets(false);
  }
  else
  {
    /* If repeat is enabled, repeat */
    if(btn_repeat->isChecked())
    {
      timer_replay.start(spin_margin->value());
    }
    /* Otherwise, no repeat */
    else
    {
      stop();
    }
  }
}

/* Volume Value Changed */
void SoundView::volumeChanged(int volume)
{
  /* Update label */
  lbl_vol_value->setText(QString::number(volume));

  /* Update sound reference */
  if(sound_curr != nullptr)
  {
    sound_curr->setVolume(volume);
  }
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Returns if the sound is playing */
bool SoundView::isPlaying()
{
  return (player.state() == QMediaPlayer::PlayingState ||
          timer_replay.isActive());
}

/* Resets the working set trigger */
void SoundView::resetWorking()
{
  btnReset();
}

/* Saves the working set trigger */
void SoundView::saveWorking()
{
  btnSave();
}

/* Sets the edit sound */
void SoundView::setEditSound(EditorSound* sound)
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
