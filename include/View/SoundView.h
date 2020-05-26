/*******************************************************************************
 * Class Name: SoundView
 * Date Created: September 28, 2015
 * Inheritance: QWidget
 * Description: The controller which takes an EditorSound and offers editing,
 *              playing, and saving capacity.
 ******************************************************************************/
#ifndef SOUNDVIEW_H
#define SOUNDVIEW_H

#include <QFileDialog>
#include <QGridLayout>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMediaPlayer>
#include <QMessageBox>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QTimer>
#include <QWidget>

#include "Database/EditorSound.h"

class SoundView : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  SoundView(QWidget* parent = nullptr);

  /* Constructor function with Editor Sound reference */
  SoundView(EditorSound* sound, QWidget* parent = nullptr);

  /* Copy constructor */
  SoundView(const SoundView &source);

  /* Destructor function */
  ~SoundView();

private:
  /* Button Widgets */
  QPushButton* btn_file;
  QPushButton* btn_play;
  QPushButton* btn_repeat;

  /* Changed sensor */
  bool changed;

  /* Line Edit Widgets */
  QLineEdit* edit_name;

  /* Label Widgets - just for displaying text */
  QLabel* lbl_file_name;
  QLabel* lbl_id_num;
  QLabel* lbl_vol_value;

  /* Player Widget */
  QMediaPlayer player;

  /* Slider Widget */
  QSlider* slid_vol;

  /* The reference sound for data */
  EditorSound* sound_base;
  EditorSound* sound_curr;

  /* Spin Box Widget */
  QSpinBox* spin_fade;
  QSpinBox* spin_margin;

  /* Timers */
  QTimer timer_fade;
  QTimer timer_replay;

  /* Volume reference */
  float vol_ref;

  /*------------------- Constants -----------------------*/
  const static int kFADE_INTERVAL; /* The fade interval, in milliseconds */

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const SoundView &source);

  /* Creates interface layout */
  void createLayout();

  /* Enable edit sound information widget portion */
  void enableEditWidgets(bool enable);

  /* Loads working info into UI objects */
  void loadWorkingInfo();

  /* Stops playing audio, if relevant */
  void stop();

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Name changed within sound widget signal */
  void nameChange(QString);

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/
public slots:
  /* Button Triggers */
  void btnFileSelect();
  void btnPlay();
  void btnRepeat(bool checked);
  void btnReset();
  void btnSave();

  /* Fade Time Changed */
  void changedFadeTime(int time);

  /* Name Text Changed */
  void changedName(QString);

  /* Player triggers */
  void playerFadeIn();
  void playerRepeat();
  void playerStateChanged(QMediaPlayer::State state);

  /* Volume Value Changed */
  void volumeChanged(int volume);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns if the sound has been edited but not saved */
  bool isChanged();

  /* Returns if the sound is playing */
  bool isPlaying();

  /* Resets the working set trigger */
  void resetWorking();

  /* Saves the working set trigger */
  void saveWorking();

  /* Sets the edit sound */
  void setEditSound(EditorSound* sound);

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  /* The copy operator */
  SoundView& operator= (const SoundView &source);
};

#endif // SOUNDVIEW_H
