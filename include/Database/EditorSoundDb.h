/*******************************************************************************
 * Class Name: EditorSoundDb
 * Date Created: October 1, 2015
 * Inheritance: QWidget
 * Description: Sound and music handler within the Editor. Controls the data
 *              and the overall view
 ******************************************************************************/
#ifndef EDITORSOUNDDB_H
#define EDITORSOUNDDB_H

#include <QGridLayout>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QProgressDialog>
#include <QPushButton>
#include <QWidget>

#include "FileHandler.h"
#include "View/SoundView.h"

class EditorSoundDb : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorSoundDb(QWidget* parent = NULL);

  /* Copy constructor */
  EditorSoundDb(const EditorSoundDb &source);

  /* Destructor function */
  ~EditorSoundDb();

private:
  /* Button Widgets */
  QPushButton* btn_m_custom_rem;
  QPushButton* btn_s_custom_rem;

  /* List Widgets */
  QListWidget* list_m_custom;
  QListWidget* list_m_reserve;
  QListWidget* list_s_custom;
  QListWidget* list_s_reserve;

  /* Music Information */
  QVector<EditorSound*> music_custom;
  EditorSound* music_custom_curr;
  QVector<EditorSound*> music_reserved;
  EditorSound* music_reserved_curr;

  /* Sound Information */
  QVector<EditorSound*> sound_custom;
  EditorSound* sound_custom_curr;
  QVector<EditorSound*> sound_reserved;
  EditorSound* sound_reserved_curr;

  /* Sound view widget / controller */
  SoundView* view_sound;

  /*------------------- Constants -----------------------*/
  const static int kCUSTOM_ID_START; /* Start of all custom IDs */

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Clear all data */
  void clearAll(bool reserved = true);

  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorSoundDb &source);

  /* Creates interface layout */
  void createLayout();

  /* Create reserved sounds */
  void createReserved();

  /* Get calls for data */
  EditorSound* getMusic(int id, bool create = true);
  EditorSound* getSound(int id, bool create = true);

  /* Loads list with applicable information */
  void loadList(QListWidget* list, QVector<EditorSound*>* chunks,
                EditorSound* current = nullptr);

  /* Loads working info into UI objects */
  void loadWorkingInfo(bool custom_music = true, bool custom_sound = true,
                       bool reserved = true);

  /* Sound edit warning, if changed */
  bool soundEditWarning();

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Name changed within player widget signal */
  //void nameChange(QString);

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/
public slots:
  /* Button Triggers */
  void btnMusicAdd();
  void btnMusicRemove();
  void btnSoundAdd();
  void btnSoundRemove();

  /* Widget Change Triggers */
  void changedName(QString name);

  /* List Index Widget Changes */
  void listMusicCustomChanged(int index);
  void listMusicCustomDouble(QListWidgetItem*);
  void listMusicReserveChanged(int index);
  void listMusicReserveDouble(QListWidgetItem*);
  void listSoundCustomChanged(int index);
  void listSoundCustomDouble(QListWidgetItem*);
  void listSoundReserveChanged(int index);
  void listSoundReserveDouble(QListWidgetItem*);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Clears editable data within the sound database */
  void clear();

  /* Returns the list of data; for use elsewhere */
  QVector<QString> getListMusic();
  QVector<QString> getListSound();

  /* Returns total save count resource */
  int getSaveCount();

  /* Loads the object data */
  void load(XmlData data, int index);

  /* Resets the working set trigger */
  void resetWorking();

  /* Saves the object data */
  void save(FileHandler* fh, QProgressDialog* dialog, bool game_only = false);

  /* Saves the working set trigger */
  void saveWorking();

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  /* The copy operator */
  EditorSoundDb& operator= (const EditorSoundDb &source);
};

#endif // EDITORSOUNDDB_H
