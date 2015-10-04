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
//#include <QInputDialog>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QWidget>

//#include "Database/EditorPerson.h"
//#include "Database/EditorSound.h"
//#include "Database/EditorTemplate.h"
//#include "EditorHelpers.h"
#include "FileHandler.h"
//#include "Game/Player/Party.h"
#include "View/SoundView.h"

class EditorSoundDb : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorSoundDb(QWidget* parent = NULL);

  /* Constructor function with id and name */
  //EditorParty(int id, QString name, QWidget* parent = NULL);

  /* Copy constructor */
  EditorSoundDb(const EditorSoundDb &source);

  /* Destructor function */
  ~EditorSoundDb();

private:
  /* Button Widgets */
  QPushButton* btn_m_custom_rem;
  QPushButton* btn_s_custom_rem;

  /* Combo Box Widgets */
  //QComboBox* combo_classify;

  /* Line Edit Widgets */
  //QLineEdit* edit_id;
  //QLineEdit* edit_name;

  /* Editor ID */
  //int id;

  /* Item Information */
  //QMap<int,int> item_set;
  //QMap<int,int> item_set_base;
  //QVector<EditorItem*> items_all;

  /* Label Widgets - just for displaying text */
  //QLabel* lbl_inv_details;
  //QLabel* lbl_person_details;

  /* List Widgets */
  QListWidget* list_m_custom;
  QListWidget* list_m_reserve;
  QListWidget* list_s_custom;
  QListWidget* list_s_reserve;

  /* Editor name */
  //QString name_base;
  //QString name_curr;

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

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorSoundDb &source);

  /* Creates interface layout */
  void createLayout();

  /* Create reserved sounds */
  void createReserved();

  /* Loads list with applicable information */
  void loadList(QListWidget* list, QVector<EditorSound*>* chunks,
                EditorSound* current = nullptr);

  /* Loads working info into UI objects */
  void loadWorkingInfo();

  /* Update object lists */
  //void updateItemList();
  //void updatePersonList();

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
  void listMusicChanged(int index);
  void listMusicCustomDouble(QListWidgetItem*);
  void listMusicReserveDouble(QListWidgetItem*);
  void listSoundChanged(int index);
  void listSoundCustomDouble(QListWidgetItem*);
  void listSoundReserveDouble(QListWidgetItem*);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the ID of the party */
  //virtual int getID() const;

  /* Returns the name of the party */
  //virtual QString getName() const;

  /* Returns the name of the party for listing */
  //virtual QString getNameList();

  /* Loads the object data */
  void load(XmlData data, int index);

  /* Resets the working set trigger */
  //void resetWorking();

  /* Saves the object data */
  void save(FileHandler* fh, bool game_only = false);

  /* Saves the working set trigger */
  //void saveWorking();

  /* Sets the ID of the party */
  //virtual void setID(int id);

  /* Sets the name of the party */
  //virtual void setName(QString name);

  /* Update Calls for data */
  //void updateItems(QVector<EditorItem*> items,
  //                 bool update_working = true);
  //void updatePersons(QVector<EditorPerson*> persons,
  //                   bool update_working = true);

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  /* The copy operator */
  EditorSoundDb& operator= (const EditorSoundDb &source);
};

#endif // EDITORSOUNDDB_H
