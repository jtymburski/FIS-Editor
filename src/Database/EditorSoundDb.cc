/*******************************************************************************
 * Class Name: EditorSoundDb
 * Date Created: October 1, 2015
 * Inheritance: QWidget
 * Description: Sound and music handler within the Editor. Controls the data
 *              and the overall view
 ******************************************************************************/
#include "Database/EditorSoundDb.h"
#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor Function */
EditorSoundDb::EditorSoundDb(QWidget* parent) : QWidget(parent)
{
  music_custom_curr = nullptr;
  music_reserved_curr = nullptr;
  sound_custom_curr = nullptr;
  sound_reserved_curr = nullptr;

  createLayout();
  loadWorkingInfo();
}

/* Copy constructor */
EditorSoundDb::EditorSoundDb(const EditorSoundDb &source) : EditorSoundDb()
{
  copySelf(source);
}

/* Destructor function */
EditorSoundDb::~EditorSoundDb()
{
  // TODO
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/* Copy function, to be called by a copy or equal operator constructor */
void EditorSoundDb::copySelf(const EditorSoundDb &source)
{
  qDebug() << "TODO COPY SELF";
}

/* Creates interface layout */
void EditorSoundDb::createLayout()
{
  /* Layout */
  QGridLayout* layout = new QGridLayout(this);
  //layout->setColumnStretch(9, 1);
  //layout->setRowMinimumHeight(7, 15);
  layout->setRowStretch(3, 1);

  /* Fonts */
  QFont bold_font;
  bold_font.setBold(true);

  /* Sound Labels */
  QLabel* lbl_s_title = new QLabel("Sounds", this);
  lbl_s_title->setFont(bold_font);
  layout->addWidget(lbl_s_title, 0, 0, 1, 4, Qt::AlignHCenter);
  QLabel* lbl_s_reserve = new QLabel("Reserved", this);
  layout->addWidget(lbl_s_reserve, 1, 0, 1, 2, Qt::AlignHCenter);
  QLabel* lbl_s_custom = new QLabel("Custom", this);
  layout->addWidget(lbl_s_custom, 1, 2, 1, 2, Qt::AlignHCenter);

  /* Sound Lists */
  list_s_reserve = new QListWidget(this);
  connect(list_s_reserve, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
          this, SLOT(listSoundReserveDouble(QListWidgetItem*)));
  layout->addWidget(list_s_reserve, 2, 0, 1, 2);
  list_s_custom = new QListWidget(this);
  connect(list_s_custom, SIGNAL(currentRowChanged(int)),
          this, SLOT(listSoundChanged(int)));
  connect(list_s_custom, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
          this, SLOT(listSoundCustomDouble(QListWidgetItem*)));
  layout->addWidget(list_s_custom, 2, 2, 1, 2);

  /* Sound Buttons */
  QPushButton* btn_s_custom_add = new QPushButton("Add", this);
  connect(btn_s_custom_add, SIGNAL(clicked()), this, SLOT(btnSoundAdd()));
  layout->addWidget(btn_s_custom_add, 3, 2);
  btn_s_custom_rem = new QPushButton("Remove", this);
  btn_s_custom_rem->setDisabled(true);
  connect(btn_s_custom_rem, SIGNAL(clicked()), this, SLOT(btnSoundRemove()));
  layout->addWidget(btn_s_custom_rem, 3, 3);

  /* Separating Vertical Frame */
  QFrame* frm_vertical = new QFrame(this);
  frm_vertical->setFrameShape(QFrame::VLine);
  frm_vertical->setLineWidth(1);
  layout->addWidget(frm_vertical, 0, 4, 4, 1);

  /* Music Labels */
  QLabel* lbl_m_title = new QLabel("Music", this);
  lbl_m_title->setFont(bold_font);
  layout->addWidget(lbl_m_title, 0, 5, 1, 4, Qt::AlignHCenter);
  QLabel* lbl_m_reserve = new QLabel("Reserved", this);
  layout->addWidget(lbl_m_reserve, 1, 5, 1, 2, Qt::AlignHCenter);
  QLabel* lbl_m_custom = new QLabel("Custom", this);
  layout->addWidget(lbl_m_custom, 1, 7, 1, 2, Qt::AlignHCenter);

  /* Music Lists */
  list_m_reserve = new QListWidget(this);
  connect(list_m_reserve, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
          this, SLOT(listMusicReserveDouble(QListWidgetItem*)));
  layout->addWidget(list_m_reserve, 2, 5, 1, 2);
  list_m_custom = new QListWidget(this);
  connect(list_m_custom, SIGNAL(currentRowChanged(int)),
          this, SLOT(listMusicChanged(int)));
  connect(list_m_custom, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
          this, SLOT(listMusicCustomDouble(QListWidgetItem*)));
  layout->addWidget(list_m_custom, 2, 7, 1, 2);

  /* Music Buttons */
  QPushButton* btn_m_custom_add = new QPushButton("Add", this);
  connect(btn_m_custom_add, SIGNAL(clicked()), this, SLOT(btnMusicAdd()));
  layout->addWidget(btn_m_custom_add, 3, 7);
  btn_m_custom_rem = new QPushButton("Remove", this);
  btn_m_custom_rem->setDisabled(true);
  connect(btn_m_custom_rem, SIGNAL(clicked()), this, SLOT(btnMusicRemove()));
  layout->addWidget(btn_m_custom_rem, 3, 8);

  /* Separating Horizontal Frame */
  QFrame* frm_horizontal = new QFrame(this);
  frm_horizontal->setFrameShape(QFrame::HLine);
  frm_horizontal->setLineWidth(1);
  layout->addWidget(frm_horizontal, 4, 0, 1, 9);

  /* Player */
  view_sound = new SoundView(this);
  connect(view_sound, SIGNAL(nameChange(QString)),
          this, SLOT(changedName(QString)));
  layout->addWidget(view_sound, 5, 0, 1, 9, Qt::AlignHCenter);

  /* Create reserved data */
  createReserved();

  // TODO: Set Sound Properly
  EditorSound* edit_sound = new EditorSound();
  view_sound->setEditSound(edit_sound);
}

/* Create reserved sounds */
void EditorSoundDb::createReserved()
{
  /* Music */
  music_reserved.push_back(new EditorSound(Sound::kID_MUSIC_BATTLE,
                                           "Generic Battle Music"));
  music_reserved.push_back(new EditorSound(Sound::kID_MUSIC_LOADING,
                                           "Loading Music"));
  music_reserved.push_back(new EditorSound(Sound::kID_MUSIC_TITLE,
                                           "Title Screen Music"));
  for(int i = 0; i < music_reserved.size(); i++)
    music_reserved[i]->setNameLock(true);
  qSort(music_reserved.begin(), music_reserved.end(), EditorSound::lessThan);

  /* Sounds */
  sound_reserved.push_back(new EditorSound(Sound::kID_SOUND_BTL_CONFUSE,
                                           "Battle: Confused"));
  sound_reserved.push_back(new EditorSound(Sound::kID_SOUND_BTL_DEATH,
                                           "Battle: Death"));
  sound_reserved.push_back(new EditorSound(Sound::kID_SOUND_BTL_FIRE,
                                           "Battle: On Fire"));
  sound_reserved.push_back(new EditorSound(Sound::kID_SOUND_BTL_HIBERNATE,
                                           "Battle: Hibernating"));
  sound_reserved.push_back(new EditorSound(Sound::kID_SOUND_BTL_LOWER,
                                           "Battle: Stats Lowered"));
  sound_reserved.push_back(new EditorSound(Sound::kID_SOUND_BTL_PARALYSIS,
                                           "Battle: Paralyzed"));
  sound_reserved.push_back(new EditorSound(Sound::kID_SOUND_BTL_PLEP,
                                           "Battle: Generic Plep"));
  sound_reserved.push_back(new EditorSound(Sound::kID_SOUND_BTL_RAISE,
                                           "Battle: Stats Raised"));
  sound_reserved.push_back(new EditorSound(Sound::kID_SOUND_BTL_SILENCE,
                                           "Battle: Silenced"));
  sound_reserved.push_back(new EditorSound(Sound::kID_SOUND_MENU_CHG,
                                           "Menu: Selection Changed"));
  sound_reserved.push_back(new EditorSound(Sound::kID_SOUND_MENU_NEXT,
                                           "Menu: Enter (Select)"));
  sound_reserved.push_back(new EditorSound(Sound::kID_SOUND_MENU_PREV,
                                           "Menu: Backspace (Unselect)"));
  sound_reserved.push_back(new EditorSound(Sound::kID_SOUND_PICK_COIN,
                                           "Pick Up: Generic Coin"));
  sound_reserved.push_back(new EditorSound(Sound::kID_SOUND_PICK_ITEM,
                                           "Pick Up: Generic Item"));
  for(int i = 0; i < sound_reserved.size(); i++)
    sound_reserved[i]->setNameLock(true);
  qSort(sound_reserved.begin(), sound_reserved.end(), EditorSound::lessThan);
}

/* Loads list with applicable information */
void EditorSoundDb::loadList(QListWidget* list, QVector<EditorSound*>* chunks,
                             EditorSound* current)
{
  int index = list->currentRow();

  /* Clear list */
  list->clear();

  /* Load new data */
  for(int i = 0; i < chunks->size(); i++)
  {
    list->addItem(chunks->at(i)->getNameList());
    if(chunks->at(i) == current)
    {
      QFont bold_font;
      bold_font.setBold(true);
      list->item(list->count() - 1)->setFont(bold_font);
    }
  }

  /* Re-select row, if relevant */
  if(index >= 0 && list->count() > 0)
  {
    if(list->count() > index)
      list->setCurrentRow(index);
    else
      list->setCurrentRow(list->count() - 1);
  }
}

/* Loads working info into UI objects */
void EditorSoundDb::loadWorkingInfo()
{
  loadList(list_m_custom, &music_custom, music_custom_curr);
  loadList(list_m_reserve, &music_reserved, music_reserved_curr);
  loadList(list_s_custom, &sound_custom, sound_custom_curr);
  loadList(list_s_reserve, &sound_reserved, sound_reserved_curr);
}

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/

/* Button Triggers */
void EditorSoundDb::btnMusicAdd()
{
  qDebug() << "TODO MUSIC ADD";
}

/* Button Triggers */
void EditorSoundDb::btnMusicRemove()
{
  qDebug() << "TODO MUSIC REMOVE";
}

/* Button Triggers */
void EditorSoundDb::btnSoundAdd()
{
  qDebug() << "TODO SOUND ADD";
}

/* Button Triggers */
void EditorSoundDb::btnSoundRemove()
{
  qDebug() << "TODO SOUND REMOVE";
}

/* Widget Change Triggers */
void EditorSoundDb::changedName(QString name)
{
  qDebug() << "TODO CHANGED NAME IN SOUND VIEW";
}

/* List Index Widget Changes */
void EditorSoundDb::listMusicChanged(int index)
{
  btn_m_custom_rem->setEnabled(index >= 0);
}

/* List Index Widget Changes */
void EditorSoundDb::listMusicCustomDouble(QListWidgetItem*)
{
  /* Get current index */

  qDebug() << "TODO CUSTOM MUSIC ITEM DOUBLE CLICKED";
}

/* List Index Widget Changes */
void EditorSoundDb::listMusicReserveDouble(QListWidgetItem*)
{
  qDebug() << "TODO RESERVE MUSIC ITEM DOUBLE CLICKED";
}

/* List Index Widget Changes */
void EditorSoundDb::listSoundChanged(int index)
{
  btn_s_custom_rem->setEnabled(index >= 0);
}

/* List Index Widget Changes */
void EditorSoundDb::listSoundCustomDouble(QListWidgetItem*)
{
  qDebug() << "TODO CUSTOM SOUND ITEM DOUBLE CLICKED";
}

/* List Index Widget Changes */
void EditorSoundDb::listSoundReserveDouble(QListWidgetItem*)
{
  qDebug() << "TODO RESERVE SOUND ITEM DOUBLE CLICKED";
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Loads the object data */
void EditorSoundDb::load(XmlData data, int index)
{

}

/* Saves the object data */
void EditorSoundDb::save(FileHandler* fh, bool game_only)
{

}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/* The copy operator */
EditorSoundDb& EditorSoundDb::operator= (const EditorSoundDb &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
