/*******************************************************************************
 * Class Name: EditorSoundDb
 * Date Created: October 1, 2015
 * Inheritance: QWidget
 * Description: Sound and music handler within the Editor. Controls the data
 *              and the overall view
 ******************************************************************************/
#include "Database/EditorSoundDb.h"
//#include <QDebug>

/* Constant Implementation - see header file for descriptions */
const int EditorSoundDb::kCUSTOM_ID_START = 1000;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Core constructor function. Offers connection to parent widget.
 *
 * Inputs: QWidget* parent - the parent widget of the widget
 */
EditorSoundDb::EditorSoundDb(QWidget* parent) : QWidget(parent)
{
  music_custom_curr = nullptr;
  music_reserved_curr = nullptr;
  sound_custom_curr = nullptr;
  sound_reserved_curr = nullptr;

  createLayout();
  loadWorkingInfo();
}

/*
 * Description: Copy constructor. Calls the blank constructor and then copies
 *              the data from the source.
 *
 * Inputs: const EditorSoundDb &source - the source object to copy
 */
EditorSoundDb::EditorSoundDb(const EditorSoundDb &source) : EditorSoundDb()
{
  copySelf(source);
}

/*
 * Description: Destructor function
 */
EditorSoundDb::~EditorSoundDb()
{
  clearAll();
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: Clears all class data back to default. This includes reserved
 *              sound and music files as well (called by destructor).
 *
 * Inputs: bool reserved - true to include deleting reserved. Default true
 * Output: none
 */
void EditorSoundDb::clearAll(bool reserved)
{
  /* Clear view */
  view_sound->setEditSound(nullptr);
  music_custom_curr = nullptr;
  music_reserved_curr = nullptr;
  sound_custom_curr = nullptr;
  sound_reserved_curr = nullptr;

  /* Delete custom sounds and music */
  for(int i = 0; i < sound_custom.size(); i++)
    delete sound_custom[i];
  sound_custom.clear();
  for(int i = 0; i < music_custom.size(); i++)
    delete music_custom[i];
  music_custom.clear();

  /* Delete reserved sounds and music */
  if(reserved)
  {
    for(int i = 0; i < sound_reserved.size(); i++)
      delete sound_reserved[i];
    sound_reserved.clear();
    for(int i = 0; i < music_reserved.size(); i++)
      delete music_reserved[i];
    music_reserved.clear();
  }
  /* Just clear reserved data */
  else
  {
    for(int i = 0; i < sound_reserved.size(); i++)
      sound_reserved[i]->clear();
    for(int i = 0; i < music_reserved.size(); i++)
      music_reserved[i]->clear();
  }

  /* Re-load working info */
  loadWorkingInfo(true, true, reserved);
}

/*
 * Description: Copies all data from source editor object to this editor
 *              object.
 *
 * Inputs: EditorSoundDb &source - the source to copy from
 * Output: none
 */
void EditorSoundDb::copySelf(const EditorSoundDb &source)
{
  /* Clear all data */
  clearAll();

  /* Reserved music */
  for(int i = 0; i < source.music_reserved.size(); i++)
  {
    EditorSound* edit_music = new EditorSound();
    *edit_music = *source.music_reserved[i];
    music_reserved.push_back(edit_music);
  }

  /* Custom music */
  for(int i = 0; i < source.music_custom.size(); i++)
  {
    EditorSound* edit_music = new EditorSound();
    *edit_music = *source.music_custom[i];
    music_custom.push_back(edit_music);
  }

  /* Reserved sound */
  for(int i = 0; i < source.sound_reserved.size(); i++)
  {
    EditorSound* edit_sound = new EditorSound();
    *edit_sound = *source.sound_reserved[i];
    sound_reserved.push_back(edit_sound);
  }

  /* Custom sound */
  for(int i = 0; i < source.sound_custom.size(); i++)
  {
    EditorSound* edit_sound = new EditorSound();
    *edit_sound = *source.sound_custom[i];
    sound_custom.push_back(edit_sound);
  }

  /* Update working information */
  loadWorkingInfo();
}

/*
 * Description: Creates the sound database layout with QT functional widgets.
 *
 * Inputs: none
 * Output: none
 */
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
  connect(list_s_reserve, SIGNAL(currentRowChanged(int)),
          this, SLOT(listSoundReserveChanged(int)));
  connect(list_s_reserve, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
          this, SLOT(listSoundReserveDouble(QListWidgetItem*)));
  layout->addWidget(list_s_reserve, 2, 0, 1, 2);
  list_s_custom = new QListWidget(this);
  connect(list_s_custom, SIGNAL(currentRowChanged(int)),
          this, SLOT(listSoundCustomChanged(int)));
  connect(list_s_custom, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
          this, SLOT(listSoundCustomDouble(QListWidgetItem*)));
  layout->addWidget(list_s_custom, 2, 2, 1, 2);

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
  connect(list_m_reserve, SIGNAL(currentRowChanged(int)),
          this, SLOT(listMusicReserveChanged(int)));
  connect(list_m_reserve, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
          this, SLOT(listMusicReserveDouble(QListWidgetItem*)));
  layout->addWidget(list_m_reserve, 2, 5, 1, 2);
  list_m_custom = new QListWidget(this);
  connect(list_m_custom, SIGNAL(currentRowChanged(int)),
          this, SLOT(listMusicCustomChanged(int)));
  connect(list_m_custom, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
          this, SLOT(listMusicCustomDouble(QListWidgetItem*)));
  layout->addWidget(list_m_custom, 2, 7, 1, 2);

  /* Sound Buttons */
  QPushButton* btn_s_custom_add = new QPushButton("Add", this);
  connect(btn_s_custom_add, SIGNAL(clicked()), this, SLOT(btnSoundAdd()));
  layout->addWidget(btn_s_custom_add, 3, 2);
  btn_s_custom_rem = new QPushButton("Remove", this);
  btn_s_custom_rem->setDisabled(true);
  connect(btn_s_custom_rem, SIGNAL(clicked()), this, SLOT(btnSoundRemove()));
  layout->addWidget(btn_s_custom_rem, 3, 3);

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
}

/*
 * Description: Creates the reserved music and sound at start-up.
 *
 * Inputs: none
 * Output: none
 */
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

  /* System Sounds */
  sound_reserved.push_back(new EditorSound(Sound::kID_SOUND_MENU_CHG,
                                           "Menu: Selection Changed"));
  sound_reserved.push_back(new EditorSound(Sound::kID_SOUND_MENU_NEXT,
                                           "Menu: Enter (Select)"));
  sound_reserved.push_back(new EditorSound(Sound::kID_SOUND_MENU_PREV,
                                           "Menu: Backspace (Unselect)"));
  sound_reserved.push_back(new EditorSound(Sound::kID_SOUND_MENU_ERR,
                                           "Menu: Error"));

  /* Map Sounds */
  sound_reserved.push_back(new EditorSound(Sound::kID_SOUND_PICK_COIN,
                                           "Map: Pick Up Generic Coin"));
  sound_reserved.push_back(new EditorSound(Sound::kID_SOUND_PICK_ITEM,
                                           "Map: Pick Up Generic Item"));
  sound_reserved.push_back(new EditorSound(Sound::kID_SOUND_SPOTTED,
                                           "Map: Player Spotted by NPC"));

  /* Battle */
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
  sound_reserved.push_back(new EditorSound(Sound::kID_SOUND_BTL_POISON,
                                           "Battle: Poison"));

  for(int i = 0; i < sound_reserved.size(); i++)
    sound_reserved[i]->setNameLock(true);

  qSort(sound_reserved.begin(), sound_reserved.end(), EditorSound::lessThan);
}

/*
 * Description: Returns a music chunk based on the ID. If create is enabled,
 *              and the widget is greater than or equal to 1000 (custom range),
 *              it will create it. If null, it was not created or found.
 *
 * Inputs: int id - the id to find
 *         bool create - true to create the chunk if not found. Only for custom
 *                       range
 * Output: EditorSound* - the music chunk found (or created)
 */
EditorSound* EditorSoundDb::getMusic(int id, bool create)
{
  EditorSound* found = nullptr;

  /* Parse the appropriate list - custom music */
  if(id >= kCUSTOM_ID_START)
  {
    for(int i = 0; found == nullptr && i < music_custom.size(); i++)
      if(music_custom[i]->getID() == id)
        found = music_custom[i];

    /* If custom not found and create enabled, create and add it */
    if(found == nullptr && create)
    {
      found = new EditorSound(id, "New Custom Music");
      music_custom.push_back(found);
      qSort(music_custom.begin(), music_custom.end(), EditorSound::lessThan);
      loadWorkingInfo(true, false, false);
    }
  }
  /* Reserved music */
  else
  {
    for(int i = 0; found == nullptr && i < music_reserved.size(); i++)
      if(music_reserved[i]->getID() == id)
        found = music_reserved[i];
  }

  return found;
}

/*
 * Description: Returns a sound chunk based on the ID. If create is enabled,
 *              and the widget is greater than or equal to 1000 (custom range),
 *              it will create it. If null, it was not created or found.
 *
 * Inputs: int id - the id to find
 *         bool create - true to create the chunk if not found. Only for custom
 *                       range
 * Output: EditorSound* - the sound chunk found (or created)
 */
EditorSound* EditorSoundDb::getSound(int id, bool create)
{
  EditorSound* found = nullptr;

  /* Parse the appropriate list - custom music */
  if(id >= kCUSTOM_ID_START)
  {
    for(int i = 0; found == nullptr && i < sound_custom.size(); i++)
      if(sound_custom[i]->getID() == id)
        found = sound_custom[i];

    /* If custom not found and create enabled, create and add it */
    if(found == nullptr && create)
    {
      found = new EditorSound(id, "New Custom Sound");
      sound_custom.push_back(found);
      qSort(sound_custom.begin(), sound_custom.end(), EditorSound::lessThan);
      loadWorkingInfo(false, true, false);
    }
  }
  /* Reserved music */
  else
  {
    for(int i = 0; found == nullptr && i < sound_reserved.size(); i++)
      if(sound_reserved[i]->getID() == id)
        found = sound_reserved[i];
  }

  return found;
}

/*
 * Description: Loads the list widget with a list of sound chunks and a current
 *              pointer. The current pointer text will be bold in the list.
 *
 * Inputs: QListWidget* list - the list to add the chunk name list to
 *         QVector<EditorSound*>* chunks - reference to sound chunks
 *         EditorSound* current - the current of the chunk list that is active
 * Output: none
 */
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

/*
 * Description: Loads all the UI elements with the contents from the working
 *              Editor Sound chunk stacks.
 *
 * Inputs: none
 * Output: none
 */
void EditorSoundDb::loadWorkingInfo(bool custom_music, bool custom_sound,
                                    bool reserved)
{
  if(custom_music)
    loadList(list_m_custom, &music_custom, music_custom_curr);
  if(custom_sound)
    loadList(list_s_custom, &sound_custom, sound_custom_curr);

  if(reserved)
  {
    loadList(list_m_reserve, &music_reserved, music_reserved_curr);
    loadList(list_s_reserve, &sound_reserved, sound_reserved_curr);
  }
}

/*
 * Description: Triggers a sound edit warning if the sound view has been edited
 *              and the user is triggering a view change. Will either save or
 *              reset the working chunk and then unset it, or cancel will not
 *              unset it.
 *
 * Inputs: none
 * Output: bool - true if proceed is authorized with unset
 */
bool EditorSoundDb::soundEditWarning()
{
  bool proceed = true;

  /* Ask the user permission if the existing has changed but not saved */
  if(view_sound->isChanged())
  {
    /* Open message box for user, if changed */
    QMessageBox msg_box;
    msg_box.setText("The editing sound has been modified.");
    msg_box.setInformativeText("Do you want to save the changes?");
    msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No |
                               QMessageBox::Cancel);
    msg_box.setDefaultButton(QMessageBox::Yes);
    int ret_value = msg_box.exec();

    /* Parse message box info */
    switch(ret_value)
    {
      case QMessageBox::Yes:
        view_sound->saveWorking();
        break;
      case QMessageBox::No:
        view_sound->resetWorking();
        break;
      case QMessageBox::Cancel:
      default:
        proceed = false;
        break;
    }
  }

  return proceed;
}

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/

/*
 * Description: Slot trigger when the custom music list add button is hit.
 *              Will add a music chunk with the next available ID.
 *
 * Inputs: none
 * Output: none
 */
void EditorSoundDb::btnMusicAdd()
{
  /* Determine next ID */
  bool found = false;
  int id = kCUSTOM_ID_START;
  for(int i = 0; !found && i < music_custom.size(); i++)
  {
    if(music_custom[i]->getID() != (id + i))
    {
      id += i;
      found = true;
    }
  }
  if(!found && music_custom.size() > 0)
    id = music_custom.last()->getID() + 1;

  /* Create and add to list */
  int index = id - kCUSTOM_ID_START;
  EditorSound* new_music = new EditorSound(id,
                                           "New Custom Music");
  music_custom.insert(index, new_music);
  loadWorkingInfo(true, false, false);
  list_m_custom->setCurrentRow(index);

  /* Trigger on music update */
  emit changedMusicList();
}

/*
 * Description: Slot trigger when the custom music list remove button is hit.
 *              Will remove the selected music chunk and reload the list.
 *
 * Inputs: none
 * Output: none
 */
void EditorSoundDb::btnMusicRemove()
{
  /* Get the index */
  int index = list_m_custom->currentRow();
  if(index >= 0)
  {
    /* Deleting warning */
    int ret_value = QMessageBox::question(this, "Deleting?",
                            "The selected music will be deleted. Are you sure?",
                                          QMessageBox::Yes, QMessageBox::No);
    if(ret_value == QMessageBox::Yes)
    {
      EditorSound* delete_music = music_custom[index];

      /* If the sound is being edited, remove it */
      if(music_custom_curr == delete_music)
      {
        view_sound->setEditSound(nullptr);
        music_custom_curr = nullptr;
      }

      /* Proceed to delete */
      delete delete_music;
      music_custom.remove(index);
      loadWorkingInfo(true, false, false);
    }

    /* Trigger on music update */
    emit changedMusicList();
  }
}

/*
 * Description: Slot trigger when the custom sound list add button is hit.
 *              Will add a sound chunk with the next available ID.
 *
 * Inputs: none
 * Output: none
 */
void EditorSoundDb::btnSoundAdd()
{
  /* Determine next ID */
  bool found = false;
  int id = kCUSTOM_ID_START;
  for(int i = 0; !found && i < sound_custom.size(); i++)
  {
    if(sound_custom[i]->getID() != (id + i))
    {
      id += i;
      found = true;
    }
  }
  if(!found && sound_custom.size() > 0)
    id = sound_custom.last()->getID() + 1;

  /* Create and add to list */
  int index = id - kCUSTOM_ID_START;
  EditorSound* new_sound = new EditorSound(id,
                                           "New Custom Sound");
  sound_custom.insert(index, new_sound);
  loadWorkingInfo(false, true, false);
  list_s_custom->setCurrentRow(index);

  /* Trigger on sound update */
  emit changedSoundList();
}

/*
 * Description: Slot trigger when the custom sound list remove button is hit.
 *              Will remove the selected sound chunk and reload the list.
 *
 * Inputs: none
 * Output: none
 */
void EditorSoundDb::btnSoundRemove()
{
  /* Get the index */
  int index = list_s_custom->currentRow();
  if(index >= 0)
  {
    /* Deleting warning */
    int ret_value = QMessageBox::question(this, "Deleting?",
                            "The selected sound will be deleted. Are you sure?",
                                          QMessageBox::Yes, QMessageBox::No);
    if(ret_value == QMessageBox::Yes)
    {
      EditorSound* delete_sound = sound_custom[index];

      /* If the sound is being edited, remove it */
      if(sound_custom_curr == delete_sound)
      {
        view_sound->setEditSound(nullptr);
        sound_custom_curr = nullptr;
      }

      /* Proceed to delete */
      delete delete_sound;
      sound_custom.remove(index);
      loadWorkingInfo(false, true, false);
    }

    /* Trigger on sound update */
    emit changedSoundList();
  }
}

/*
 * Description: Triggered when the name in the base chunk has been updated in
 *              the edit view. Will update the working info list.
 *
 * Inputs: QString - not used
 * Output: none
 */
void EditorSoundDb::changedName(QString)
{
  loadWorkingInfo();

  /* Trigger on both lists */
  emit changedMusicList();
  emit changedSoundList();
}

/*
 * Description: Slot triggered when the index in the custom music list is
 *              changed. Updates the remove button enabled/disabled state and
 *              unselects the other lists.
 *
 * Inputs: int index - the new index in the list
 * Output: none
 */
void EditorSoundDb::listMusicCustomChanged(int index)
{
  btn_m_custom_rem->setEnabled(index >= 0);

  /* Reset other views */
  if(index >= 0)
  {
    list_m_reserve->setCurrentRow(-1);
    list_s_custom->setCurrentRow(-1);
    list_s_reserve->setCurrentRow(-1);
  }
}

/*
 * Description: Slot triggered when an index in the custom music list is double
 *              clicked. Triggers the chunk to be edited with the sound view.
 *
 * Inputs: QListWidgetItem* - not used
 * Output: none
 */
void EditorSoundDb::listMusicCustomDouble(QListWidgetItem*)
{
  /* Get current index */
  int index = list_m_custom->currentRow();
  if(index >= 0)
  {
    EditorSound* edit_sound = music_custom.at(index);

    /* If proceed, load */
    if(soundEditWarning())
    {
      view_sound->setEditSound(edit_sound);

      music_custom_curr = edit_sound;
      music_reserved_curr = nullptr;
      sound_custom_curr = nullptr;
      sound_reserved_curr = nullptr;

      loadWorkingInfo();
    }
  }
}

/*
 * Description: Slot triggered when the index in the reserved music list is
 *              changed. Updates the remove button enabled/disabled state and
 *              unselects the other lists.
 *
 * Inputs: int index - the new index in the list
 * Output: none
 */
void EditorSoundDb::listMusicReserveChanged(int index)
{
  /* Reset other views */
  if(index >= 0)
  {
    list_m_custom->setCurrentRow(-1);
    list_s_custom->setCurrentRow(-1);
    list_s_reserve->setCurrentRow(-1);
  }
}

/*
 * Description: Slot triggered when an index in the reserved music list is
 *              double clicked. Triggers the chunk to be edited with the sound
 *              view.
 *
 * Inputs: QListWidgetItem* - not used
 * Output: none
 */
void EditorSoundDb::listMusicReserveDouble(QListWidgetItem*)
{
  /* Get current index */
  int index = list_m_reserve->currentRow();
  if(index >= 0)
  {
    EditorSound* edit_sound = music_reserved.at(index);

    /* If proceed, load */
    if(soundEditWarning())
    {
      view_sound->setEditSound(edit_sound);

      music_custom_curr = nullptr;
      music_reserved_curr = edit_sound;
      sound_custom_curr = nullptr;
      sound_reserved_curr = nullptr;

      loadWorkingInfo();
    }
  }
}

/*
 * Description: Slot triggered when the index in the custom sound list is
 *              changed. Updates the remove button enabled/disabled state and
 *              unselects the other lists.
 *
 * Inputs: int index - the new index in the list
 * Output: none
 */
void EditorSoundDb::listSoundCustomChanged(int index)
{
  btn_s_custom_rem->setEnabled(index >= 0);

  /* Reset other views */
  if(index >= 0)
  {
    list_m_custom->setCurrentRow(-1);
    list_m_reserve->setCurrentRow(-1);
    list_s_reserve->setCurrentRow(-1);
  }
}

/*
 * Description: Slot triggered when an index in the custom sound list is
 *              double clicked. Triggers the chunk to be edited with the sound
 *              view.
 *
 * Inputs: QListWidgetItem* - not used
 * Output: none
 */
void EditorSoundDb::listSoundCustomDouble(QListWidgetItem*)
{
  /* Get current index */
  int index = list_s_custom->currentRow();
  if(index >= 0)
  {
    EditorSound* edit_sound = sound_custom.at(index);

    /* If proceed, load */
    if(soundEditWarning())
    {
      view_sound->setEditSound(edit_sound);

      music_custom_curr = nullptr;
      music_reserved_curr = nullptr;
      sound_custom_curr = edit_sound;
      sound_reserved_curr = nullptr;

      loadWorkingInfo();
    }
  }
}

/*
 * Description: Slot triggered when the index in the reserved sound list is
 *              changed. Updates the remove button enabled/disabled state and
 *              unselects the other lists.
 *
 * Inputs: int index - the new index in the list
 * Output: none
 */
void EditorSoundDb::listSoundReserveChanged(int index)
{
  /* Reset other views */
  if(index >= 0)
  {
    list_m_custom->setCurrentRow(-1);
    list_m_reserve->setCurrentRow(-1);
    list_s_custom->setCurrentRow(-1);
  }
}

/*
 * Description: Slot triggered when an index in the reserved sound list is
 *              double clicked. Triggers the chunk to be edited with the sound
 *              view.
 *
 * Inputs: QListWidgetItem* - not used
 * Output: none
 */
void EditorSoundDb::listSoundReserveDouble(QListWidgetItem*)
{
  /* Get current index */
  int index = list_s_reserve->currentRow();
  if(index >= 0)
  {
    EditorSound* edit_sound = sound_reserved.at(index);

    /* If proceed, load */
    if(soundEditWarning())
    {
      view_sound->setEditSound(edit_sound);

      music_custom_curr = nullptr;
      music_reserved_curr = nullptr;
      sound_custom_curr = nullptr;
      sound_reserved_curr = edit_sound;

      loadWorkingInfo();
    }
  }
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Deletes all custom sounds and music chunks and resets the views.
 *
 * Inputs: none
 * Output: none
 */
void EditorSoundDb::clear()
{
  clearAll(false);
}

/*
 * Description: Gets the combined reserved and custom music list in a single
 *              stack. Used throughout the program in other widgets for music
 *              selection.
 *
 * Inputs: bool include_none - true to include a "None" option at front of list
 * Output: QList<QString> - the name list of the music chunks
 */
QList<QString> EditorSoundDb::getListMusic(bool include_none)
{
  QList<QString> list;

  /* Reserved music first */
  for(int i = 0; i < music_reserved.size(); i++)
    list.push_back(music_reserved[i]->getNameList());

  /* Custom music second */
  for(int i = 0; i < music_custom.size(); i++)
    list.push_back(music_custom[i]->getNameList());

  /* None option */
  if(include_none)
    list.push_front("None");

  return list;
}

/*
 * Description: Gets the combined reserved and custom sound list in a single
 *              stack. Used throughout the program in other widgets for sound
 *              selection.
 *
 * Inputs: bool include_none - true to include a "None" option at front of list
 * Output: QList<QString> - the name list of the music chunks
 */
QList<QString> EditorSoundDb::getListSound(bool include_none)
{
  QList<QString> list;

  /* Reserved sound first */
  for(int i = 0; i < sound_reserved.size(); i++)
    list.push_back(sound_reserved[i]->getNameList());

  /* Custom sound second */
  for(int i = 0; i < sound_custom.size(); i++)
    list.push_back(sound_custom[i]->getNameList());

  /* None option */
  if(include_none)
    list.push_front("None");

  return list;
}

/*
 * Description: Returns the total number of save objects that will be processed
 *              when the save is triggered with a progress dialog.
 *
 * Inputs: none
 * Output: int - the total number of save objects
 */
int EditorSoundDb::getSaveCount()
{
  int count = 0;

  /* Tally lists */
  count += music_reserved.size();
  count += music_custom.size();
  count += sound_reserved.size();
  count += sound_custom.size();

  return count;
}

/*
 * Description: Loads the object data from the XML struct and offset index.
 *
 * Inputs: XmlData data - the XML data tree struct
 *         int index - the offset index into the struct
 * Output: none
 */
void EditorSoundDb::load(XmlData data, int index)
{
  /* Parse elements */
  if(data.getElement(index) == "music")
  {
    QString id = QString::fromStdString(data.getKeyValue(index));
    int id_int = id.toInt();
    if(id_int >= 0)
    {
      /* Get/Create the music chunk and then update */
      EditorSound* edit_music = getMusic(id_int);
      if(edit_music != nullptr)
        if(edit_music->load(data, index + 1))
          loadWorkingInfo(true, false, true);
    }
  }
  else if(data.getElement(index) == "sound")
  {
    QString id = QString::fromStdString(data.getKeyValue(index));
    int id_int = id.toInt();
    if(id_int >= 0)
    {
      /* Get/Create the music chunk and then update */
      EditorSound* edit_sound = getSound(id_int);
      if(edit_sound != nullptr)
        if(edit_sound->load(data, index + 1))
          loadWorkingInfo(false, true, true);
    }
  }
}

/*
 * Description: Slot triggered by reset button. Resets data in the active sound
 *              view controller.
 *
 * Inputs: none
 * Output: none
 */
void EditorSoundDb::resetWorking()
{
  view_sound->resetWorking();
  view_sound->setEditSound(nullptr);
  loadWorkingInfo();
}

/*
 * Description: Saves the object data to the file handling pointer.
 *
 * Inputs: FileHandler* fh - the file handling pointer
 *         QProgressDialog* dialog - the progressing dialog
 *         bool game_only - true if the data should include game only relevant
 * Output: none
 */
void EditorSoundDb::save(FileHandler* fh, QProgressDialog* dialog,
                         bool game_only)
{
  if(fh != nullptr && dialog != nullptr)
  {
    /* Reserved Music */
    for(int i = 0; i < music_reserved.size(); i++)
    {
      music_reserved[i]->save(fh, game_only, "music");
      dialog->setValue(dialog->value() + 1);
    }

    /* Custom Music */
    for(int i = 0; i < music_custom.size(); i++)
    {
      music_custom[i]->save(fh, game_only, "music");
      dialog->setValue(dialog->value() + 1);
    }

    /* Reserved Sounds */
    for(int i = 0; i < sound_reserved.size(); i++)
    {
      sound_reserved[i]->save(fh, game_only, "sound");
      dialog->setValue(dialog->value() + 1);
    }

    /* Custom Sounds */
    for(int i = 0; i < sound_custom.size(); i++)
    {
      sound_custom[i]->save(fh, game_only, "sound");
      dialog->setValue(dialog->value() + 1);
    }
  }
}

/*
 * Description: Slot triggered by save button. Save data in the active sound
 *              view controller back to the base Editor Sound chunk.
 *
 * Inputs: none
 * Output: none
 */
void EditorSoundDb::saveWorking()
{
  view_sound->saveWorking();
  view_sound->setEditSound(nullptr);

  music_custom_curr = nullptr;
  music_reserved_curr = nullptr;
  sound_custom_curr = nullptr;
  sound_reserved_curr = nullptr;

  loadWorkingInfo();
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/*
 * Description: Copy operator construction. This is called when the variable
 *              already exists and equal operator used with another
 *              EditorSoundDb.
 *
 * Inputs: const EditorSoundDb &source - the source class constructor
 * Output: EditorSoundDb& - pointer to the copied class
 */
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
