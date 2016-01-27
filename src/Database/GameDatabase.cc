/*******************************************************************************
 * Class Name: GameDatabase
 * Date Created: December 23, 2014
 * Inheritance: QWidget
 * Description: Far left view that determines what the game view will become.
 ******************************************************************************/
#include "Database/GameDatabase.h"
#include <QDebug>

GameDatabase::GameDatabase(QWidget *parent) : QWidget(parent)
{
  //setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);

  /* Clear out variables */
  current_action = NULL;
  current_battleclass = NULL;
  current_battlescene = nullptr;
  current_bubby = NULL;
  current_equipment = NULL;
  current_item = NULL;
  current_map = NULL;
  current_party = NULL;
  current_person = NULL;
  current_race = NULL;
  current_skill = NULL;
  current_skillset = NULL;
  mapsize_dialog = NULL;

  /* Create views that will always exist */
  data_sounds = new EditorSoundDb(this);
  connect(data_sounds, SIGNAL(changedMusicList()),
          this, SLOT(listMusicUpdated()));
  connect(data_sounds, SIGNAL(changedSoundList()),
          this, SLOT(listSoundUpdated()));

  /* Top view set-up */
  view_top = new QListWidget(this);
  view_top->setEditTriggers(QAbstractItemView::NoEditTriggers);
  QStringList items;
  items << "Maps" << "--" << "Parties" << "Persons" << "B.U.B.B.I.E's"
        << "Equipments" << "Items" << "Classes" << "Races" << "Skill Sets"
        << "Skills" << "Actions" << "--" << "Music / Sound" << "Battle Scenes";
  view_top->addItems(items);
  view_top->setCurrentRow(0);
  connect(view_top,SIGNAL(currentRowChanged(int)),
          this,SLOT(rowChange(int)));

  /* Bottom view set-up */
  view_bottom = new QListWidget(this);
  view_bottom->setContextMenuPolicy(Qt::CustomContextMenu);
  view_bottom->setEditTriggers(QAbstractItemView::NoEditTriggers);
  connect(view_bottom, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
          this, SLOT(modifySelection(QListWidgetItem*)));
  connect(view_bottom, SIGNAL(customContextMenuRequested(QPoint)),
          this, SLOT(listMenuRequested(QPoint)));

  /* Right click menu control */
  rightclick_menu = new QMenu("Map Edit", this);
  QAction* rename_map = new QAction("Rename", rightclick_menu);
  connect(rename_map, SIGNAL(triggered()), this, SLOT(renameMap()));
  rightclick_menu->addAction(rename_map);
  rightclick_menu->hide();

  /* Set-up buttons at the bottom */
  button_new = new QPushButton("New",this);
  button_delete = new QPushButton("Delete",this);
  button_import = new QPushButton("Import",this);
  button_duplicate = new QPushButton("Duplicate",this);
  connect(button_new, SIGNAL(clicked()), this, SLOT(createNewResource()));
  connect(button_delete, SIGNAL(clicked()), this, SLOT(deleteResource()));
  connect(button_import, SIGNAL(clicked()), this, SLOT(importResource()));
  connect(button_duplicate, SIGNAL(clicked()), this, SLOT(duplicateResource()));

  /* Save all button */
  QPushButton* button_saveall = new QPushButton("Process All", this);
  connect(button_saveall, SIGNAL(clicked()), this, SLOT(saveAll()));

  /* Configure the layout */
  layout = new QVBoxLayout(this);
  QLabel *label = new QLabel("Game Database", this);
  label->setAlignment(Qt::AlignHCenter);
  QFont font = label->font();
  font.setBold(true);
  label->setFont(font);
  layout->addWidget(label);
  layout->addWidget(button_saveall);
  layout->addWidget(view_top);
  layout->addWidget(view_bottom);
  //QHBoxLayout* button_layout = new QHBoxLayout();
  //button_layout->addWidget(button_new);
  //button_layout->addWidget(button_delete);
  //layout->addLayout(button_layout);
  //QHBoxLayout* button_layout2 = new QHBoxLayout();
  //button_layout2->addWidget(button_import);
  //button_layout2->addWidget(button_duplicate);
  //layout->addLayout(button_layout2);
  layout->addWidget(button_new);
  layout->addWidget(button_delete);
  layout->addWidget(button_duplicate);
  layout->addWidget(button_import);
  //layout->setSizeConstraint(QLayout::SetMinimumSize);

  /* Update the bottom list */
  rowChange(view_top->currentRow());

  /* Set-up the tile icons */
  tile_icons.passN = new QPixmap(":/images/pass_N.png");
  tile_icons.passE = new QPixmap(":/images/pass_E.png");
  tile_icons.passS = new QPixmap(":/images/pass_S.png");
  tile_icons.passW = new QPixmap(":/images/pass_W.png");
  tile_icons.nopassN = new QPixmap(":/images/nopass_N.png");
  tile_icons.nopassE = new QPixmap(":/images/nopass_E.png");
  tile_icons.nopassS = new QPixmap(":/images/nopass_S.png");
  tile_icons.nopassW = new QPixmap(":/images/nopass_W.png");

  /* Create start objects */
  createStartObjects();
}

GameDatabase::~GameDatabase()
{
  deleteAll();

  /* UnSet the tile icons */
  delete tile_icons.passN;
  tile_icons.passN = NULL;
  delete tile_icons.passE;
  tile_icons.passE = NULL;
  delete tile_icons.passS;
  tile_icons.passS = NULL;
  delete tile_icons.passW;
  tile_icons.passW = NULL;
  delete tile_icons.nopassN;
  tile_icons.nopassN = NULL;
  delete tile_icons.nopassE;
  tile_icons.nopassE = NULL;
  delete tile_icons.nopassS;
  tile_icons.nopassS = NULL;
  delete tile_icons.nopassW;
  tile_icons.nopassW = NULL;
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* Add object in the correct spot in the array */
void GameDatabase::addAction(EditorAction* action)
{
  bool inserted = false;

  for(int i = 0; (i < data_action.size() && !inserted); i++)
  {
    if(data_action[i]->getID() > action->getID())
    {
      data_action.insert(i, action);
      inserted = true;
    }
  }

  /* If not inserted, insert at tail */
  if(!inserted)
    data_action.push_back(action);
}

/* Add object in the correct spot in the stack */
void GameDatabase::addBattleScene(EditorBattleScene* scene)
{
  bool inserted = false;

  for(int i = 0; (i < data_battlescene.size() && !inserted); i++)
  {
    if(data_battlescene[i]->getID() > scene->getID())
    {
      data_battlescene.insert(i, scene);
      inserted = true;
    }
  }

  /* If not inserted, append */
  if(!inserted)
    data_battlescene.push_back(scene);
}

/* Add object in the correct spot in the array */
void GameDatabase::addClass(EditorCategory* cat_class)
{
  bool inserted = false;

  for(int i = 0; (i < data_battleclass.size() && !inserted); i++)
  {
    if(data_battleclass[i]->getID() > cat_class->getID())
    {
      data_battleclass.insert(i, cat_class);
      inserted = true;
    }
  }

  /* If not inserted, insert at tail */
  if(!inserted)
    data_battleclass.push_back(cat_class);
}

/* Add object in the correct spot in the array */
void GameDatabase::addItem(EditorItem* item)
{
  bool inserted = false;

  for(int i = 0; (i < data_item.size() && !inserted); i++)
  {
    if(data_item[i]->getID() > item->getID())
    {
      data_item.insert(i, item);
      inserted = true;
    }
  }

  /* If not inserted, insert at tail */
  if(!inserted)
    data_item.push_back(item);
}

/* Add object in the correct spot in the array */
void GameDatabase::addParty(EditorParty* party)
{
  bool inserted = false;

  for(int i = 0; (i < data_party.size() && !inserted); i++)
  {
    if(data_party[i]->getID() > party->getID())
    {
      data_party.insert(i, party);
      inserted = true;
    }
  }

  /* If not inserted, insert at tail */
  if(!inserted)
    data_party.push_back(party);
}

/* Add object in the correct spot in the array */
void GameDatabase::addPerson(EditorPerson* person)
{
  bool inserted = false;

  for(int i = 0; (i < data_person.size() && !inserted); i++)
  {
    if(data_person[i]->getID() > person->getID())
    {
      data_person.insert(i, person);
      inserted = true;
    }
  }

  /* If not inserted, insert at tail */
  if(!inserted)
    data_person.push_back(person);
}

/* Add object in the correct spot in the array */
void GameDatabase::addRace(EditorCategory* cat_race)
{
  bool inserted = false;

  for(int i = 0; (i < data_race.size() && !inserted); i++)
  {
    if(data_race[i]->getID() > cat_race->getID())
    {
      data_race.insert(i, cat_race);
      inserted = true;
    }
  }

  /* If not inserted, insert at tail */
  if(!inserted)
    data_race.push_back(cat_race);
}

/* Add object in the correct spot in the array */
void GameDatabase::addSkill(EditorSkill* skill)
{
  bool inserted = false;

  for(int i = 0; (i < data_skill.size() && !inserted); i++)
  {
    if(data_skill[i]->getID() > skill->getID())
    {
      data_skill.insert(i, skill);
      inserted = true;
    }
  }

  /* If not inserted, insert at tail */
  if(!inserted)
    data_skill.push_back(skill);
}

/* Add object in the correct spot in the array */
void GameDatabase::addSkillSet(EditorSkillset* set)
{
  bool inserted = false;

  for(int i = 0; (i < data_skillset.size() && !inserted); i++)
  {
    if(data_skillset[i]->getID() > set->getID())
    {
      data_skillset.insert(i, set);
      inserted = true;
    }
  }

  /* If not inserted, insert at tail */
  if(!inserted)
    data_skillset.push_back(set);
}

/* Change objects trigger call */
void GameDatabase::changeAction(int index, bool forced, bool save)
{
  bool proceed = forced;

  /* If current action is valid, either save or load */
  if(current_action != NULL)
  {
    /* Forced - Do not show dialog */
    if(forced)
    {
      proceed = true;
      if(save)
        current_action->getEditedAction();
      else
        current_action->resetInfo();
    }
    /* Not Forced - Create warning about changing object */
    else
    {
      QMessageBox msg_box;
      msg_box.setText(QString("Changing to another Action. All unsaved ") +
                      QString("changes to the existing will be lost."));
      msg_box.setInformativeText("Save changes to existing Action?");
      msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No |
                                 QMessageBox::Cancel);

      /* Execute and get result */
      int result = msg_box.exec();
      if(result == QMessageBox::Yes || result == QMessageBox::No)
      {
        proceed = true;
        if(result == QMessageBox::Yes)
          current_action->getEditedAction();
        else
          current_action->resetInfo();
      }
    }
  }
  else
  {
    proceed = true;
  }

  /* If proceed, change action */
  if(proceed)
  {
    /* Check index */
    if(index >= 0)
      current_action = data_action[index];
    else
      current_action = NULL;

    /* Change object */
    emit changeAction(current_action);
  }
}

/* Change objects trigger call */
void GameDatabase::changeBattleScene(int index, bool forced, bool save)
{
  bool proceed = forced;
  bool save_object = false;

  /* If current battle class is valid, either save or load */
  if(current_battlescene != nullptr)
  {
    /* Forced - Do not show dialog */
    if(forced || !current_battlescene->isChanged())
    {
      proceed = true;
      if(save)
        save_object = true;
    }
    /* Not Forced - Create warning about changing object */
    else
    {
      QMessageBox msg_box;
      msg_box.setText(QString("Changing to another BattleScene. All unsaved ") +
                      QString("changes to the existing will be lost."));
      msg_box.setInformativeText("Save changes to existing BattleScene?");
      msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No |
                                 QMessageBox::Cancel);

      /* Execute and get result */
      int result = msg_box.exec();
      if(result == QMessageBox::Yes || result == QMessageBox::No)
      {
        proceed = true;
        if(result == QMessageBox::Yes)
          save_object = true;
      }
    }
  }
  else
  {
    proceed = true;
  }

  /* If proceed, change battle class */
  if(proceed)
  {
    /* Check index */
    if(index >= 0)
      current_battlescene = data_battlescene[index];
    else
      current_battlescene = nullptr;

    /* Change object */
    emit changeBattleScene(current_battlescene, save_object);
  }
}

/* Change objects trigger call */
void GameDatabase::changeClass(int index, bool forced, bool save)
{
  bool proceed = forced;

  /* If current battle class is valid, either save or load */
  if(current_battleclass != NULL)
  {
    /* Forced - Do not show dialog */
    if(forced)
    {
      proceed = true;
      if(save)
        current_battleclass->saveWorking();
      else
        current_battleclass->resetWorking();
    }
    /* Not Forced - Create warning about changing object */
    else
    {
      QMessageBox msg_box;
      msg_box.setText(QString("Changing to another BattleClass. All unsaved ") +
                      QString("changes to the existing will be lost."));
      msg_box.setInformativeText("Save changes to existing BattleClass?");
      msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No |
                                 QMessageBox::Cancel);

      /* Execute and get result */
      int result = msg_box.exec();
      if(result == QMessageBox::Yes || result == QMessageBox::No)
      {
        proceed = true;
        if(result == QMessageBox::Yes)
          current_battleclass->saveWorking();
        else
          current_battleclass->resetWorking();
      }
    }
  }
  else
  {
    proceed = true;
  }

  /* If proceed, change battle class */
  if(proceed)
  {
    /* Check index */
    if(index >= 0)
      current_battleclass = data_battleclass[index];
    else
      current_battleclass = NULL;

    /* Change object */
    emit changeBattleclass(current_battleclass);
  }
}

/* Change objects trigger call */
void GameDatabase::changeItem(int index, bool forced, bool save)
{
  bool proceed = forced;

  /* If current item is valid, either save or load */
  if(current_item != NULL)
  {
    /* Forced - Do not show dialog */
    if(forced)
    {
      proceed = true;
      if(save)
        current_item->saveWorking();
      else
        current_item->resetWorking();
    }
    /* Not Forced - Create warning about changing object */
    else
    {
      QMessageBox msg_box;
      msg_box.setText(QString("Changing to another Item. All unsaved ") +
                      QString("changes to the existing will be lost."));
      msg_box.setInformativeText("Save changes to existing Item?");
      msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No |
                                 QMessageBox::Cancel);

      /* Execute and get result */
      int result = msg_box.exec();
      if(result == QMessageBox::Yes || result == QMessageBox::No)
      {
        proceed = true;
        if(result == QMessageBox::Yes)
          current_item->saveWorking();
        else
          current_item->resetWorking();
      }
    }
  }
  else
  {
    proceed = true;
  }

  /* If proceed, change item */
  if(proceed)
  {
    /* Check index */
    if(index >= 0)
      current_item = data_item[index];
    else
      current_item = NULL;

    /* Change object */
    emit changeItem(current_item);
  }
}

/* Change objects trigger call */
void GameDatabase::changeParty(int index, bool forced, bool save)
{
  bool proceed = forced;

  /* If current party is valid, either save or load */
  if(current_party != NULL)
  {
    /* Forced - Do not show dialog */
    if(forced)
    {
      proceed = true;
      if(save)
        current_party->saveWorking();
      else
        current_party->resetWorking();
    }
    /* Not Forced - Create warning about changing object */
    else
    {
      QMessageBox msg_box;
      msg_box.setText(QString("Changing to another Party. All unsaved ") +
                      QString("changes to the existing will be lost."));
      msg_box.setInformativeText("Save changes to existing Party?");
      msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No |
                                 QMessageBox::Cancel);

      /* Execute and get result */
      int result = msg_box.exec();
      if(result == QMessageBox::Yes || result == QMessageBox::No)
      {
        proceed = true;
        if(result == QMessageBox::Yes)
          current_party->saveWorking();
        else
          current_party->resetWorking();
      }
    }
  }
  else
  {
    proceed = true;
  }

  /* If proceed, change party */
  if(proceed)
  {
    /* Check index */
    if(index >= 0)
      current_party = data_party[index];
    else
      current_party = NULL;

    /* Change object */
    emit changeParty(current_party);
  }
}

/* Change objects trigger call */
void GameDatabase::changePerson(int index, bool forced, bool save)
{
  bool proceed = forced;

  /* If current person is valid, either save or load */
  if(current_person != NULL)
  {
    /* Forced - Do not show dialog */
    if(forced)
    {
      proceed = true;
      if(save)
        current_person->saveWorking();
      else
        current_person->resetWorking();
    }
    /* Not Forced - Create warning about changing object */
    else
    {
      QMessageBox msg_box;
      msg_box.setText(QString("Changing to another Person. All unsaved ") +
                      QString("changes to the existing will be lost."));
      msg_box.setInformativeText("Save changes to existing Person?");
      msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No |
                                 QMessageBox::Cancel);

      /* Execute and get result */
      int result = msg_box.exec();
      if(result == QMessageBox::Yes || result == QMessageBox::No)
      {
        proceed = true;
        if(result == QMessageBox::Yes)
          current_person->saveWorking();
        else
          current_person->resetWorking();
      }
    }
  }
  else
  {
    proceed = true;
  }

  /* If proceed, change person */
  if(proceed)
  {
    /* Check index */
    if(index >= 0)
      current_person = data_person[index];
    else
      current_person = NULL;

    /* Change object */
    emit changePerson(current_person);
  }
}

/* Change objects trigger call */
void GameDatabase::changeRace(int index, bool forced, bool save)
{
  bool proceed = forced;

  /* If current race is valid, either save or load */
  if(current_race != NULL)
  {
    /* Forced - Do not show dialog */
    if(forced)
    {
      proceed = true;
      if(save)
        current_race->saveWorking();
      else
        current_race->resetWorking();
    }
    /* Not Forced - Create warning about changing object */
    else
    {
      QMessageBox msg_box;
      msg_box.setText(QString("Changing to another Race. All unsaved ") +
                      QString("changes to the existing will be lost."));
      msg_box.setInformativeText("Save changes to existing Race?");
      msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No |
                                 QMessageBox::Cancel);

      /* Execute and get result */
      int result = msg_box.exec();
      if(result == QMessageBox::Yes || result == QMessageBox::No)
      {
        proceed = true;
        if(result == QMessageBox::Yes)
          current_race->saveWorking();
        else
          current_race->resetWorking();
      }
    }
  }
  else
  {
    proceed = true;
  }

  /* If proceed, change race */
  if(proceed)
  {
    /* Check index */
    if(index >= 0)
      current_race = data_race[index];
    else
      current_race = NULL;

    /* Change object */
    emit changeRace(current_race);
  }
}

/* Change objects trigger call */
void GameDatabase::changeSkill(int index, bool forced, bool save)
{
  bool proceed = forced;

  /* If current skill is valid, either save or load */
  if(current_skill != NULL)
  {
    /* Forced - Do not show dialog */
    if(forced)
    {
      proceed = true;
      if(save)
        current_skill->getEditedSkill();
      else
        current_skill->resetWorkingSkill();
    }
    /* Not Forced - Create warning about changing object */
    else
    {
      QMessageBox msg_box;
      msg_box.setText(QString("Changing to another Skill. All unsaved ") +
                      QString("changes to the existing will be lost."));
      msg_box.setInformativeText("Save changes to existing Skill?");
      msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No |
                                 QMessageBox::Cancel);

      /* Execute and get result */
      int result = msg_box.exec();
      if(result == QMessageBox::Yes || result == QMessageBox::No)
      {
        proceed = true;
        if(result == QMessageBox::Yes)
          current_skill->getEditedSkill();
        else
          current_skill->resetWorkingSkill();
      }
    }
  }
  else
  {
    proceed = true;
  }

  /* If proceed, change skill */
  if(proceed)
  {
    /* Check index */
    if(index >= 0)
      current_skill = data_skill[index];
    else
      current_skill = NULL;

    /* Change object */
    emit changeSkill(current_skill);
  }
}

/* Change objects trigger call */
void GameDatabase::changeSkillSet(int index, bool forced, bool save)
{
  bool proceed = forced;

  /* If current skill set is valid, either save or load */
  if(current_skillset != NULL)
  {
    /* Forced - Do not show dialog */
    if(forced)
    {
      proceed = true;
      if(save)
        current_skillset->saveWorking();
      else
        current_skillset->resetWorking();
    }
    /* Not Forced - Create warning about changing object */
    else
    {
      QMessageBox msg_box;
      msg_box.setText(QString("Changing to another Skill Set. All unsaved ") +
                      QString("changes to the existing will be lost."));
      msg_box.setInformativeText("Save changes to existing Skill Set?");
      msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No |
                                 QMessageBox::Cancel);

      /* Execute and get result */
      int result = msg_box.exec();
      if(result == QMessageBox::Yes || result == QMessageBox::No)
      {
        proceed = true;
        if(result == QMessageBox::Yes)
          current_skillset->saveWorking();
        else
          current_skillset->resetWorking();
      }
    }
  }
  else
  {
    proceed = true;
  }

  /* If proceed, change skill set */
  if(proceed)
  {
    /* Check index */
    if(index >= 0)
      current_skillset = data_skillset[index];
    else
      current_skillset = NULL;

    /* Change object */
    emit changeSkillset(current_skillset);
  }
}

/* Get object, based on ID */
EditorAction* GameDatabase::getAction(int id)
{
  for(int i = 0; i < data_action.size(); i++)
    if(data_action[i]->getID() == id)
      return data_action[i];
  return NULL;
}

/* Get object, based on ID */
EditorBattleScene* GameDatabase::getBattleScene(int id)
{
  for(int i = 0; i < data_battlescene.size(); i++)
    if(data_battlescene[i]->getID() == id)
      return data_battlescene[i];
  return nullptr;
}

/* Get object, based on ID */
EditorCategory* GameDatabase::getClass(int id)
{
  for(int i = 0; i < data_battleclass.size(); i++)
    if(data_battleclass[i]->getID() == id)
      return data_battleclass[i];
  return NULL;
}

/* Get object, based on ID */
EditorItem* GameDatabase::getItem(int id)
{
  for(int i = 0; i < data_item.size(); i++)
    if(data_item[i]->getID() == id)
      return data_item[i];
  return NULL;
}

/* Get object, based on ID */
EditorParty* GameDatabase::getParty(int id)
{
  for(int i = 0; i < data_party.size(); i++)
    if(data_party[i]->getID() == id)
      return data_party[i];
  return NULL;
}

/* Get object, based on ID */
EditorPerson* GameDatabase::getPerson(int id)
{
  for(int i = 0; i < data_person.size(); i++)
    if(data_person[i]->getID() == id)
      return data_person[i];
  return NULL;
}

/* Get object, based on ID */
EditorCategory* GameDatabase::getRace(int id)
{
  for(int i = 0; i < data_race.size(); i++)
    if(data_race[i]->getID() == id)
      return data_race[i];
  return NULL;
}

/* Get object, based on ID */
EditorSkill* GameDatabase::getSkill(int id)
{
  for(int i = 0; i < data_skill.size(); i++)
    if(data_skill[i]->getID() == id)
      return data_skill[i];
  return NULL;
}

/* Get object, based on ID */
EditorSkillset* GameDatabase::getSkillSet(int id)
{
  for(int i = 0; i < data_skillset.size(); i++)
    if(data_skillset[i]->getID() == id)
      return data_skillset[i];
  return NULL;
}

/* Check if the core object is protected */
bool GameDatabase::isObjectProtected()
{
  int index = view_bottom->currentRow();
  bool is_protected = false;

  /* Check current object */
  if(view_top->currentRow() == EditorEnumDb::SKILLSETVIEW)
  {
    if(data_skillset[index]->getID() == (int)SkillSet::kID_BUBBIFIED)
      is_protected = true;
  }
  else if(view_top->currentRow() == EditorEnumDb::ITEMVIEW)
  {
    if(data_item[index]->getID() == (int)Item::kID_MONEY)
      is_protected = true;
  }
  else if(view_top->currentRow() == EditorEnumDb::PERSONVIEW)
  {
    if(data_person[index]->getID() == (int)Person::kID_PLAYER)
      is_protected = true;
  }
  else if(view_top->currentRow() == EditorEnumDb::PARTYVIEW)
  {
    if(data_party[index]->getID() == (int)Party::kID_SLEUTH ||
       data_party[index]->getID() == (int)Party::kID_BEARACKS)
      is_protected = true;
  }

  return is_protected;
}

/* Called to load action data */
void GameDatabase::loadAction(XmlData data, int index)
{
  /* Parse depending on the data */
  int id = -1;
  if(data.getTailElements(index).size() == 1)
  {
    EditorAction* new_action = new EditorAction();
    new_action->load(data, index);
    id = new_action->getID();
    delete new_action;
  }
  else if(data.getKeyValue(index) != "")
  {
    id = std::stoi(data.getKeyValue(index));
  }

  /* If ID is valid, try and add/create */
  if(id >= 0)
  {
    EditorAction* found_action = getAction(id);
    if(found_action == NULL)
    {
      found_action = new EditorAction(id, "New Action");
      addAction(found_action);
    }
    found_action->load(data, index);
  }
}

/* Called to load object data */
void GameDatabase::loadBattleScene(XmlData data, int index)
{
  int id = -1;
  if(!data.getKeyValue(index).empty())
    id = std::stoi(data.getKeyValue(index));

  /* If the ID is valid, try and add/create */
  if(id >= 0)
  {
    EditorBattleScene* found_scene = getBattleScene(id);
    if(found_scene == nullptr)
    {
      found_scene = new EditorBattleScene(id);
      addBattleScene(found_scene);
    }
    found_scene->load(data, index + 1);
  }
}

/* Called to load object data */
void GameDatabase::loadClass(XmlData data, int index)
{
  int id = -1;
  if(!data.getKeyValue(index).empty())
    id = std::stoi(data.getKeyValue(index));

  /* If the ID is valid, try and add/create */
  if(id >= 0)
  {
    EditorCategory* found_class = getClass(id);
    if(found_class == NULL)
    {
      found_class = new EditorCategory(id, "New Class");
      addClass(found_class);
    }
    found_class->load(data, index + 1);
  }
}

/* Called to load object data */
void GameDatabase::loadItem(XmlData data, int index)
{
  int id = -1;
  if(!data.getKeyValue(index).empty())
    id = std::stoi(data.getKeyValue(index));

  /* If the ID is valid, try and add/create */
  if(id >= 0)
  {
    EditorItem* found_item = getItem(id);
    if(found_item == NULL)
    {
      found_item = new EditorItem(id, "New Item");
      addItem(found_item);
    }
    found_item->load(data, index + 1);
  }
}

/* Called to load object data */
void GameDatabase::loadParty(XmlData data, int index)
{
  int id = -1;
  if(!data.getKeyValue(index).empty())
    id = std::stoi(data.getKeyValue(index));

  /* If the ID is valid, try and add/create */
  if(id >= 0)
  {
    EditorParty* found_party = getParty(id);
    if(found_party == NULL)
    {
      found_party = new EditorParty(id, "New Party");
      addParty(found_party);
    }
    found_party->load(data, index + 1);
  }
}

/* Called to load object data */
void GameDatabase::loadPerson(XmlData data, int index)
{
  int id = -1;
  if(!data.getKeyValue(index).empty())
    id = std::stoi(data.getKeyValue(index));

  /* If the ID is valid, try and add/create */
  if(id >= 0)
  {
    EditorPerson* found_person = getPerson(id);
    if(found_person == NULL)
    {
      found_person = new EditorPerson(id, "New Person");
      addPerson(found_person);
    }
    found_person->load(data, index + 1);
  }
}

/* Called to load object data */
void GameDatabase::loadRace(XmlData data, int index)
{
  int id = -1;
  if(!data.getKeyValue(index).empty())
    id = std::stoi(data.getKeyValue(index));

  /* If the ID is valid, try and add/create */
  if(id >= 0)
  {
    EditorCategory* found_race = getRace(id);
    if(found_race == NULL)
    {
      found_race = new EditorCategory(id, "New Race");
      addRace(found_race);
    }
    found_race->load(data, index + 1);
  }
}

/* Called to load object data */
void GameDatabase::loadSkill(XmlData data, int index)
{
  int id = -1;
  if(!data.getKeyValue(index).empty())
    id = std::stoi(data.getKeyValue(index));

  /* If the ID is valid, try and add/create */
  if(id >= 0)
  {
    EditorSkill* found_skill = getSkill(id);
    if(found_skill == NULL)
    {
      found_skill = new EditorSkill(id, "New Skill");
      addSkill(found_skill);
    }
    found_skill->load(data, index + 1);
  }
}

/* Called to load object data */
void GameDatabase::loadSkillSet(XmlData data, int index)
{
  int id = -1;
  if(!data.getKeyValue(index).empty())
    id = std::stoi(data.getKeyValue(index));

  /* If the ID is valid, try and add/create */
  if(id >= 0)
  {
    EditorSkillset* found_set = getSkillSet(id);
    if(found_set == NULL)
    {
      found_set = new EditorSkillset(id, "New Skill Set");
      addSkillSet(found_set);
    }
    found_set->load(data, index + 1);
  }
}

/* Called upon load finish - for clean up */
void GameDatabase::loadFinish()
{
  /* Actions finish */
  for(int i = 0; i < data_action.size(); i++)
    data_action[i]->setBaseAction(data_action[i]->getBaseAction());

  /* Skills finish */
  for(int i = 0; i < data_skill.size(); i++)
  {
    data_skill[i]->updateActions(data_action);
    data_skill[i]->setBaseSkill(data_skill[i]->getBaseSkill());
    data_skill[i]->checkFlags();
  }

  /* Skill Sets finish */
  for(int i = 0; i < data_skillset.size(); i++)
  {
    data_skillset[i]->updateSkills(data_skill);
    data_skillset[i]->resetWorking();
  }

  /* Classes finish */
  for(int i = 0; i < data_battleclass.size(); i++)
  {
    data_battleclass[i]->updateSkillSets(data_skillset);
    data_battleclass[i]->resetWorking();
  }

  /* Races finish */
  for(int i = 0; i < data_race.size(); i++)
  {
    data_race[i]->updateSkillSets(data_skillset);
    data_race[i]->resetWorking();
  }

  /* Item finish */
  for(int i = 0; i < data_item.size(); i++)
  {
    data_item[i]->updateSkills(data_skill);
    data_item[i]->resetWorking();
  }

  /* Persons finish */
  for(int i = 0; i < data_person.size(); i++)
  {
    data_person[i]->updateClasses(data_battleclass, false);
    data_person[i]->updateItems(data_item, false);
    data_person[i]->updateRaces(data_race, false);
    data_person[i]->resetWorking();
  }

  /* Parties finish */
  for(int i = 0; i < data_party.size(); i++)
  {
    data_party[i]->updateItems(data_item, false);
    data_party[i]->updatePersons(data_person, false);
    data_party[i]->resetWorking();
  }

  /* Update music */
  emit updatedMusic(data_sounds->getListMusic());

  /* Update battle scenes */
  updateBattleSceneObjects();
}

/* Update calls for objects (to fill in information required from others) */
void GameDatabase::updateClasses()
{
  for(int i = 0; i < data_battleclass.size(); i++)
    data_battleclass[i]->updateSkillSets(data_skillset);
  for(int i = 0; i < data_race.size(); i++)
    data_race[i]->updateSkillSets(data_skillset);
}

/* Update calls for objects (to fill in information required from others) */
void GameDatabase::updateItems()
{
  for(int i = 0; i < data_item.size(); i++)
    data_item[i]->updateSkills(data_skill);
}

/* Update calls for objects (to fill in information required from others) */
void GameDatabase::updateParties()
{
  for(int i = 0; i < data_party.size(); i++)
  {
    data_party[i]->updateItems(data_item, false);
    data_party[i]->updatePersons(data_person);
  }
}

/* Update calls for objects (to fill in information required from others) */
void GameDatabase::updatePersons()
{
  for(int i = 0; i < data_person.size(); i++)
  {
    data_person[i]->updateClasses(data_battleclass, false);
    data_person[i]->updateItems(data_item, false);
    data_person[i]->updateRaces(data_race);
  }
}

/* Update calls for objects (to fill in information required from others) */
void GameDatabase::updateSkills()
{
  for(int i = 0; i < data_skill.size(); i++)
    data_skill[i]->updateActions(data_action);
}

/* Update calls for objects (to fill in information required from others) */
void GameDatabase::updateSkillSets()
{
  for(int i = 0; i < data_skillset.size(); i++)
    data_skillset[i]->updateSkills(data_skill);
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

/* Creates a new map, from the dialog */
void GameDatabase::createNewMap()
{
  QString name = EditorMap::getDialogName(mapsize_dialog);
  int width = EditorMap::getDialogWidth(mapsize_dialog);
  int height = EditorMap::getDialogHeight(mapsize_dialog);

  /* Add the new map */
  if(!name.isEmpty() && width > 0 && height > 0)
  {
    if(data_map.size() > 0)
      data_map.push_back(new EditorMap(data_map.last()->getID() + 1, name,
                                       width, height, &tile_icons));
    else
      data_map.push_back(new EditorMap(0, name, width, height, &tile_icons));
  }

  /* Finally, close the dialog */
  mapsize_dialog->close();
  delete mapsize_dialog;
  mapsize_dialog = NULL;

  /* Update bottom view */
  modifyBottomList(view_top->currentRow());
  view_bottom->setCurrentRow(view_bottom->count() - 1);
}

/* Creates a new resource -> new button */
void GameDatabase::createNewResource()
{
  QString name;
  switch(view_top->currentRow())
  {
    /* -- MAP -- */
    case EditorEnumDb::MAPVIEW:
      if(mapsize_dialog != NULL)
        delete mapsize_dialog;
      mapsize_dialog = EditorMap::createMapDialog(this);
      connect(EditorMap::getDialogButton(mapsize_dialog), SIGNAL(clicked()),
              this, SLOT(createNewMap()));
      mapsize_dialog->show();
      break;
    /* -- PARTY -- */
    case EditorEnumDb::PARTYVIEW:
      name = "New Party";
      if(data_party.size() > 0)
        data_party.push_back(
                         new EditorParty(data_party.last()->getID() + 1, name));
      else
        data_party.push_back(new EditorParty(0, name));
      data_party.last()->updateItems(data_item, false);
      data_party.last()->updatePersons(data_person);
      break;
    /* -- PERSON -- */
    case EditorEnumDb::PERSONVIEW:
      name = "New Person";
      if(data_person.size() > 0)
        data_person.push_back(
                       new EditorPerson(data_person.last()->getID() + 1, name));
      else
        data_person.push_back(new EditorPerson(0, name));
      data_person.last()->updateClasses(data_battleclass, false);
      data_person.last()->updateItems(data_item, false);
      data_person.last()->updateRaces(data_race);
      updateParties();
      break;
    /* -- ITEM -- */
    case EditorEnumDb::ITEMVIEW:
      name = "New Item";
      if(data_item.size() > 0)
        data_item.push_back(new EditorItem(data_item.last()->getID() + 1,name));
      else
        data_item.push_back(new EditorItem(0, name));
      data_item.last()->updateSkills(data_skill);
      updateParties();
      updatePersons();
      break;
    /* -- ACTION -- */
    case EditorEnumDb::ACTIONVIEW:
      name = "New Action";
      if(data_action.size() > 0)
        data_action.push_back(
                       new EditorAction(data_action.last()->getID() + 1, name));
      else
        data_action.push_back(new EditorAction(0, name));
      updateSkills();
      break;
    /* -- RACE -- */
    case EditorEnumDb::RACEVIEW:
      name = "New Race";
      if(data_race.size() > 0)
        data_race.push_back(
                       new EditorCategory(data_race.last()->getID() + 1, name));
      else
        data_race.push_back(new EditorCategory(0, name));
      data_race.last()->updateSkillSets(data_skillset);
      updatePersons();
      break;
    /* -- BATTLE CLASS -- */
    case EditorEnumDb::BATTLECLASSVIEW:
      name = "New Battle Class";
      if(data_battleclass.size() > 0)
        data_battleclass.push_back(
                new EditorCategory(data_battleclass.last()->getID() + 1, name));
      else
        data_battleclass.push_back(new EditorCategory(0, name));
      data_battleclass.last()->updateSkillSets(data_skillset);
      updatePersons();
      break;
    /* -- SKILL SET -- */
    case EditorEnumDb::SKILLSETVIEW:
      name = "New Skill Set";
      if(data_skillset.size() > 0)
        data_skillset.push_back(
                   new EditorSkillset(data_skillset.last()->getID() + 1, name));
      else
        data_skillset.push_back(new EditorSkillset(0, name));
      data_skillset.last()->updateSkills(data_skill);
      updateClasses();
      break;
    /* -- SKILL -- */
    case EditorEnumDb::SKILLVIEW:
      name = "New Skill";
      if(data_skill.size() > 0)
        data_skill.push_back(
                         new EditorSkill(data_skill.last()->getID() + 1, name));
      else
        data_skill.push_back(new EditorSkill(0, name));
      data_skill.last()->updateActions(data_action);
      data_skill.last()->checkFlags();
      updateItems();
      updateSkillSets();
      break;
    /* -- BATTLE SCENE -- */
    case EditorEnumDb::BATTLESCENEVIEW:
      if(data_battlescene.size() > 0)
        data_battlescene.push_back(
                   new EditorBattleScene(data_battlescene.last()->getID() + 1));
      else
        data_battlescene.push_back(new EditorBattleScene(0));
      updateBattleSceneObjects();
      break;
    /* -- EQUIPMENT -- */
    case EditorEnumDb::EQUIPMENTVIEW:
      name = "New Equipment";
      if(data_equipment.size() > 0)
        data_equipment.push_back(
                 new EditorEquipment(data_equipment.last()->getID() + 1, name));
      else
        data_equipment.push_back(new EditorEquipment(0, name));
      break;
    /* -- BUBBY -- */
    case EditorEnumDb::BUBBYVIEW:
      name = "New Bubby";
      if(data_bubby.size() > 0)
        data_bubby.push_back(
                         new EditorBubby(data_bubby.last()->getID() + 1, name));
      else
        data_bubby.push_back(new EditorBubby(0, name));
      break;
    default:
      break;
  }

  /* Set selection to the new row and update */
  modifyBottomList(view_top->currentRow());
  if(view_top->currentRow() != 0)
    view_bottom->setCurrentRow(view_bottom->count() - 1);
}

// TODO: Comment
void GameDatabase::deleteResource()
{
  if(view_bottom->currentRow() >= 0)
  {
    if(!isObjectProtected())
    {
      int index = view_bottom-> currentRow();

      /* Get the category */
      QString category = view_top->currentItem()->text();
      QString name = view_bottom->currentItem()->text();

      /* Check if it's the bottom row */
      bool bottom = false;
      if((index + 1) == view_bottom->count())
        bottom = true;

      /* Create warning about deleting */
      QMessageBox msg_box;
      msg_box.setText("Deleting \"" + name + "\" from " + category + ".");
      msg_box.setInformativeText("Are you sure?");
      msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
      if(msg_box.exec() == QMessageBox::Yes)
      {
        /* Switch through what to do */
        switch(view_top->currentRow())
        {
          /* -- MAP -- */
          case EditorEnumDb::MAPVIEW:
            if(data_map[index] == current_map)
            {
              emit changeMap(NULL);
              current_map = NULL;
            }
            delete data_map[index];
            data_map.remove(index);
            break;
          /* -- PARTY -- */
          case EditorEnumDb::PARTYVIEW:
            if(data_party[index] == current_party)
              changeParty(-1, true);
            delete data_party[index];
            data_party.remove(index);
            break;
          /* -- PERSON -- */
          case EditorEnumDb::PERSONVIEW:
            if(data_person[index] == current_person)
              changePerson(-1, true);
            delete data_person[index];
            data_person.remove(index);
            updateParties();
            break;
          /* -- ITEM -- */
          case EditorEnumDb::ITEMVIEW:
            if(data_item[index] == current_item)
              changeItem(-1, true);
            delete data_item[index];
            data_item.remove(index);
            updateParties();
            updatePersons();
            break;
          /* -- ACTION -- */
          case EditorEnumDb::ACTIONVIEW:
            if(data_action[index] == current_action)
              changeAction(-1, true);
            delete data_action[index];
            data_action.remove(index);
            updateSkills();
            break;
          /* -- RACE CLASS -- */
          case EditorEnumDb::RACEVIEW:
            if(data_race[index] == current_race)
              changeRace(-1, true);
            delete data_race[index];
            data_race.remove(index);
            updatePersons();
            break;
          /* -- BATTLE CLASS -- */
          case EditorEnumDb::BATTLECLASSVIEW:
            if(data_battleclass[index] == current_battleclass)
              changeClass(-1, true);
            delete data_battleclass[index];
            data_battleclass.remove(index);
            updatePersons();
            break;
          /* -- SKILL SET -- */
          case EditorEnumDb::SKILLSETVIEW:
            if(data_skillset[index] == current_skillset)
              changeSkillSet(-1, true);
            delete data_skillset[index];
            data_skillset.remove(index);
            updateClasses();
            break;
          /* -- SKILL -- */
          case EditorEnumDb::SKILLVIEW:
            if(data_skill[index] == current_skill)
              changeSkill(-1, true);
            delete data_skill[index];
            data_skill.remove(index);
            updateItems();
            updateSkillSets();
            break;
          /* -- BATTLE SCENE -- */
          case EditorEnumDb::BATTLESCENEVIEW:
            if(data_battlescene[index] == current_battlescene)
              changeBattleScene(-1, true);
            delete data_battlescene[index];
            data_battlescene.remove(index);
            updateBattleSceneObjects();
            break;
          /* -- EQUIPMENT -- */
          case EditorEnumDb::EQUIPMENTVIEW:
            if(data_equipment[index] == current_equipment)
            {
              emit changeEquipment(NULL);
              current_equipment = NULL;
            }
            delete data_equipment[index];
            data_equipment.remove(index);
            break;
          /* -- BUBBY -- */
          case EditorEnumDb::BUBBYVIEW:
            if(data_bubby[index] == current_bubby)
            {
              emit changeBubby(NULL);
              current_bubby = NULL;
            }
            delete data_bubby[index];
            data_bubby.remove(index);
            break;
          default:
            break;
        }

        /* Update list */
        modifyBottomList(view_top->currentRow());
        if(bottom)
          view_bottom->setCurrentRow(view_bottom->count() - 1);
      }
    }
    else
    {
      QMessageBox::information(this, "Delete Failed", tr("Selected core ") +
                               tr("object is protected and cannot be deleted"));
    }
  }
  else
  {
    QMessageBox::information(this, "Delete Failed",
                             "No core object selected to delete");
  }
}

void GameDatabase::duplicateResource()
{
  if(view_bottom->currentRow() >= 0)
  {
    /* Store old index and create a new resource */
    int id = 0;
    int index = view_bottom->currentRow();
    if(view_top->currentRow() != 0)
      createNewResource();

    /* Switch through and copy the data from the selected */
    switch(view_top->currentRow())
    {
      /* -- MAP -- */
      case EditorEnumDb::MAPVIEW:
        data_map.push_back(new EditorMap(data_map.last()->getID() + 1,
                                         "TEMP", 0, 0));
        id = data_map.last()->getID();
        *data_map.last() = *data_map[index];
        data_map.last()->setID(id);
        data_map.last()->setTileIcons(data_map[index]->getTileIcons());
        break;
      /* -- PARTY -- */
      case EditorEnumDb::PARTYVIEW:
        id = data_party.last()->getID();
        *data_party.last() = *data_party[index];
        data_party.last()->setID(id);
        break;
      /* -- PERSON -- */
      case EditorEnumDb::PERSONVIEW:
        id = data_person.last()->getID();
        *data_person.last() = *data_person[index];
        data_person.last()->setID(id);
        break;
      /* -- ITEM -- */
      case EditorEnumDb::ITEMVIEW:
        id = data_item.last()->getID();
        *data_item.last() = *data_item[index];
        data_item.last()->setID(id);
        break;
      /* -- ACTION -- */
      case EditorEnumDb::ACTIONVIEW:
        id = data_action.last()->getID();
        *data_action.last() = *data_action[index];
        data_action.last()->setID(id);
        break;
      /* -- RACE CLASS -- */
      case EditorEnumDb::RACEVIEW:
        id = data_race.last()->getID();
        *data_race.last() = *data_race[index];
        data_race.last()->setID(id);
        break;
      /* -- BATTLE CLASS -- */
      case EditorEnumDb::BATTLECLASSVIEW:
        id = data_battleclass.last()->getID();
        *data_battleclass.last() = *data_battleclass[index];
        data_battleclass.last()->setID(id);
        break;
      /* -- SKILL SET -- */
      case EditorEnumDb::SKILLSETVIEW:
        id = data_skillset.last()->getID();
        *data_skillset.last() = *data_skillset[index];
        data_skillset.last()->setID(id);
        break;
      /* -- SKILL -- */
      case EditorEnumDb::SKILLVIEW:
        id = data_skill.last()->getID();
        *data_skill.last() = *data_skill[index];
        data_skill.last()->setID(id);
        break;
      /* -- BATTLE SCENE -- */
      case EditorEnumDb::BATTLESCENEVIEW:
        id = data_battlescene.last()->getID();
        *data_battlescene.last() = *data_battlescene[index];
        data_battlescene.last()->setID(id);
        updateBattleSceneObjects();
      /* -- EQUIPMENT -- */
      case EditorEnumDb::EQUIPMENTVIEW:
        id = data_equipment.last()->getID();
        *data_equipment.last() = *data_equipment[index];
        data_equipment.last()->setID(id);
        break;
      /* -- BUBBY -- */
      case EditorEnumDb::BUBBYVIEW:
        id = data_bubby.last()->getID();
        *data_bubby.last() = *data_bubby[index];
        data_bubby.last()->setID(id);
        break;
      default:
        break;
    }

    /* Update list */
    modifyBottomList(view_top->currentRow());
    view_bottom->setCurrentRow(view_bottom->count() - 1);
  }
}

/* Imports a resource */
void GameDatabase::importResource()
{
  // TODO: Future
  QMessageBox::information(this, "Notification",
                           "Coming soon to a production near you!");
}

/* Right click list menu on bottom list */
void GameDatabase::listMenuRequested(const QPoint & pos)
{
  (void)pos;
  /* Only proceed if it's the map set */
  if(view_top->currentRow() == EditorEnumDb::MAPVIEW)
    rightclick_menu->exec(QCursor::pos());
}

/* List updates, from widgets */
void GameDatabase::listMusicUpdated()
{
  if(data_sounds != nullptr)
    emit updatedMusic(data_sounds->getListMusic());
}

/* List updates, from widgets */
void GameDatabase::listSoundUpdated()
{
  /* Not used right now */
}

/* Double click on an element */
//void GameDatabase::modifySelection(QModelIndex index)
void GameDatabase::modifySelection(QListWidgetItem* item)
{
  int index = view_bottom->currentRow();
  (void)item;

  switch(view_top->currentRow())
  {
    /* -- MAP -- */
    case EditorEnumDb::MAPVIEW:
      current_map = data_map[index];
      emit changeMap(current_map);
      break;
    /* -- PARTY -- */
    case EditorEnumDb::PARTYVIEW:
      changeParty(index);
      break;
    /* -- PERSON -- */
    case EditorEnumDb::PERSONVIEW:
      changePerson(index);
      break;
    /* -- ITEM -- */
    case EditorEnumDb::ITEMVIEW:
      changeItem(index);
      break;
    /* -- ACTION -- */
    case EditorEnumDb::ACTIONVIEW:
      changeAction(index);
      break;
    /* -- RACE -- */
    case EditorEnumDb::RACEVIEW:
      changeRace(index);
      break;
    /* -- BATTLE CLASS -- */
    case EditorEnumDb::BATTLECLASSVIEW:
      changeClass(index);
      break;
    /* -- SKILL SET -- */
    case EditorEnumDb::SKILLSETVIEW:
      changeSkillSet(index);
      break;
    /* -- SKILL -- */
    case EditorEnumDb::SKILLVIEW:
      changeSkill(index);
      break;
    /* -- BATTLE SCENE -- */
    case EditorEnumDb::BATTLESCENEVIEW:
      changeBattleScene(index);
      break;
    /* -- EQUIPMENT -- */
    case EditorEnumDb::EQUIPMENTVIEW:
      current_equipment = data_equipment[index];
      emit changeEquipment(current_equipment);
      break;
    /* -- BUBBY -- */
    case EditorEnumDb::BUBBYVIEW:
      current_bubby = data_bubby[index];
      emit changeBubby(current_bubby);
      break;
    default:
      break;
  }

  modifyBottomList(view_top->currentRow());
}

/* Renames the selected map */
void GameDatabase::renameMap()
{
  rightclick_menu->hide();

  QListWidgetItem* item = view_bottom->currentItem();
  if(item != NULL)
  {
    int row_index = view_bottom->currentRow();
    bool ok;
    QString text = QInputDialog::getText(this, "Rename Map", "Map Name:",
                        QLineEdit::Normal, data_map[row_index]->getName(), &ok);
    if(ok && !text.isEmpty())
    {
      data_map[row_index]->setName(text);
      modifyBottomList(view_top->currentRow());
    }
  }
}

/* Row change on top list */
void GameDatabase::rowChange(int index)
{
  emit changeMode((EditorEnumDb::ViewMode)index);
  modifyBottomList(index);

  /* Modify enable status depending on index */
  bool is_invalid = (index == (int)EditorEnumDb::BLANKVIEW1 ||
                     index == (int)EditorEnumDb::BLANKVIEW2 ||
                     index == (int)EditorEnumDb::AUDIOVIEW);
  button_new->setEnabled(!is_invalid);
  button_import->setEnabled(!is_invalid);
  view_bottom->setEnabled(!is_invalid);
}

/* Triggered by save all button */
void GameDatabase::saveAll()
{
  /* Create warning about saving all */
  QMessageBox msg_box;
  msg_box.setText("This will select \"Save\" in all game objects below");
  msg_box.setInformativeText("Are you sure?");
  msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  if(msg_box.exec() == QMessageBox::Yes)
  {
    /* Actions */
    changeAction(-1, true, true);
    for(int i = 0; i < data_action.size(); i++)
      data_action[i]->getEditedAction();

    /* Skills */
    changeSkill(-1, true, true);
    for(int i = 0; i < data_skill.size(); i++)
      data_skill[i]->getEditedSkill();

    /* Skill Sets */
    changeSkillSet(-1, true, true);
    for(int i = 0; i < data_skillset.size(); i++)
      data_skillset[i]->saveWorking();

    /* Classes */
    changeClass(-1, true, true);
    for(int i = 0; i < data_battleclass.size(); i++)
      data_battleclass[i]->saveWorking();

    /* Races */
    changeRace(-1, true, true);
    for(int i = 0; i < data_race.size(); i++)
      data_race[i]->saveWorking();

    /* Items */
    changeItem(-1, true, true);
    for(int i = 0; i < data_item.size(); i++)
      data_item[i]->saveWorking();

    /* Persons */
    changePerson(-1, true, true);
    for(int i = 0; i < data_person.size(); i++)
      data_person[i]->saveWorking();

    /* Parties */
    changeParty(-1, true, true);
    for(int i = 0; i < data_party.size(); i++)
      data_party[i]->saveWorking();

    /* Music / Sound */
    data_sounds->saveWorking();

    /* Battle Scenes */
    changeBattleScene(-1, true, true);

    /* Fix back to visible row */
    rowChange(view_top->currentRow());
  }
}

// TODO: Comment
void GameDatabase::updateBottomListName(QString str)
{
  (void)str;
  modifyBottomList(view_top->currentRow());

  /* Update other interconnected objects */
  if(view_top->currentRow() == EditorEnumDb::ACTIONVIEW)
  {
    updateSkills();
  }
  else if(view_top->currentRow() == EditorEnumDb::SKILLVIEW)
  {
    updateItems();
    updateSkillSets();
  }
  else if(view_top->currentRow() == EditorEnumDb::SKILLSETVIEW)
  {
    updateClasses();
  }
  else if(view_top->currentRow() == EditorEnumDb::BATTLECLASSVIEW ||
          view_top->currentRow() == EditorEnumDb::RACEVIEW)
  {
    updatePersons();
  }
  else if(view_top->currentRow() == EditorEnumDb::ITEMVIEW)
  {
    updateParties();
    updatePersons();
  }
  else if(view_top->currentRow() == EditorEnumDb::PERSONVIEW)
  {
    updateParties();
  }
  else if(view_top->currentRow() == EditorEnumDb::BATTLESCENEVIEW)
  {
    updateBattleSceneObjects();
  }
}

/* Updates battle scenes in the map database class */
// TODO: Comment
void GameDatabase::updateBattleSceneObjects()
{
  QList<QPair<int,QString>> list;
  for(int i = 0; i < data_battlescene.size(); i++)
    list.push_back(QPair<int,QString>(data_battlescene[i]->getID(),
                                      data_battlescene[i]->getName()));
  emit updatedBattleScenes(list);
}

/* Updates event objects in the map database class */
// TODO: Comment
void GameDatabase::updateEventObjects()
{
  /* List of items */
  QVector<QString> item_list;
  for(int i = 0; i < data_item.size(); i++)
    item_list.push_back(data_item[i]->getNameList());
  emit updatedItems(item_list);

  /* List of maps */
  QVector<QString> map_list;
  for(int i = 0; i < data_map.size(); i++)
    map_list.push_back(data_map[i]->getNameList());
  emit updatedMaps(map_list);

  /* List of parties */
  QVector<QString> party_list;
  for(int i = 0; i < data_party.size(); i++)
    party_list.push_back(data_party[i]->getNameList());
  emit updatedParties(party_list);
}

/* Trigger handles sending music data to other classes */
// TODO: Comment
void GameDatabase::updateMusicObjects()
{
  if(data_sounds != nullptr)
    emit updatedMusic(data_sounds->getListMusic());
}

/* Trigger handles with sounds */
// TODO: Comment
void GameDatabase::updateSoundObjects()
{
  if(data_sounds != nullptr)
    emit updatedSounds(data_sounds->getListSound());
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Create the starting point */
void GameDatabase::createStartObjects()
{
  addSkillSet(new EditorSkillset(SkillSet::kID_BUBBIFIED, "Bubbified Skills"));
  addItem(new EditorItem(Item::kID_MONEY, "Money"));
  addPerson(new EditorPerson(Person::kID_PLAYER, "Player"));
  addParty(new EditorParty(Party::kID_SLEUTH, "Player Sleuth"));
  addParty(new EditorParty(Party::kID_BEARACKS, "Player Bearacks"));
  rowChange(view_top->currentRow());
}

/* Delete the game */
void GameDatabase::deleteAll()
{
  /* Get the current top view */
  int index = view_top->currentRow();

  /* Map clean-up */
  emit changeMap(NULL);
  for(int i = 0; i < data_map.size(); i++)
    delete data_map[i];
  data_map.clear();

  /* Party clean-up */
  changeParty(-1, true);
  for(int i = 0; i < data_party.size(); i++)
    delete data_party[i];
  data_party.clear();

  /* Person clean-up */
  changePerson(-1, true);
  for(int i = 0; i < data_person.size(); i++)
    delete data_person[i];
  data_person.clear();

  /* Bubby clean-up */
  emit changeBubby(NULL);
  for(int i = 0; i < data_bubby.size(); i++)
    delete data_bubby[i];
  data_bubby.clear();

  /* Equipment clean-up */
  emit changeEquipment(NULL);
  for(int i = 0; i < data_equipment.size(); i++)
    delete data_equipment[i];
  data_equipment.clear();

  /* Item clean-up */
  changeItem(-1, true);
  for(int i = 0; i < data_item.size(); i++)
    delete data_item[i];
  data_item.clear();

  /* Battle Class clean-up */
  changeClass(-1, true);
  for(int i = 0; i < data_battleclass.size(); i++)
    delete data_battleclass[i];
  data_battleclass.clear();

  /* Race clean-up */
  changeRace(-1, true);
  for(int i = 0; i < data_race.size(); i++)
    delete data_race[i];
  data_race.clear();

  /* Skillset clean-up */
  changeSkillSet(-1, true);
  for(int i = 0; i < data_skillset.size(); i++)
    delete data_skillset[i];
  data_skillset.clear();

  /* Skill clean-up */
  changeSkill(-1, true);
  for(int i = 0; i < data_skill.size(); i++)
    delete data_skill[i];
  data_skill.clear();

  /* Action clean-up */
  changeAction(-1, true);
  for(int i = 0; i < data_action.size(); i++)
    delete data_action[i];
  data_action.clear();

  /* Music / Sound clean-up */
  data_sounds->clear();

  /* Battle Scenes */
  changeBattleScene(-1, true);
  for(int i = 0; i < data_battlescene.size(); i++)
    delete data_battlescene[i];
  data_battlescene.clear();
  updateBattleSceneObjects();

  /* Reset the view */
  if(index == 0)
    view_top->setCurrentRow(1);
  else
    view_top->setCurrentRow(0);
  view_top->setCurrentRow(index);
}

/* Get save count */
int GameDatabase::getSaveCount(bool selected_map, int sub_index)
{
  int count = 0;

  /* Map data */
  if(!selected_map)
  {
    for(int i = 0; i < data_map.size(); i++)
      count += data_map[i]->getSaveCount();
  }
  else if(current_map != nullptr)
  {
    count += current_map->getSaveCount(sub_index);
  }

  /* Core data */
  count += data_action.size();
  count += data_skill.size();
  count += data_skillset.size();
  count += data_battleclass.size();
  count += data_race.size();
  count += data_item.size();
  count += data_person.size();
  count += data_party.size();
  count += data_sounds->getSaveCount();
  count += data_battlescene.size();

  return count;
}

/* Get current map */
EditorMap* GameDatabase::getCurrentMap()
{
  return current_map;
}

/* Returns the audio view, for connection */
EditorSoundDb* GameDatabase::getSoundDatabase()
{
  return data_sounds;
}

/* Load the game */
void GameDatabase::load(FileHandler* fh, QProgressDialog* dialog)
{
  if(fh != NULL)
  {
    XmlData data;
    bool done = false;
    bool read_success = true;
    bool success = true;

    /* Loop through all elements */
    do
    {
      /* Read set of XML data */
      data = fh->readXmlData(&done, &read_success);
      success &= read_success;

      /* Only validate if wrapped within game and successful read */
      if(success && data.getElement(0) == "app")
      {
        /* Music and Sound */
        if(data.getElement(1) == "music" ||
           data.getElement(1) == "sound")
        {
          data_sounds->load(data, 1);
        }
      }
      else if(success && data.getElement(0) == "game")
      {
        /* If core game data, parse */
        if(data.getElement(1) == "core")
        {
          /* Action */
          if(data.getElement(2) == "action")
            loadAction(data, 2);
          else if(data.getElement(2) == "battlescene")
            loadBattleScene(data, 2);
          else if(data.getElement(2) == "class")
            loadClass(data, 2);
          else if(data.getElement(2) == "item")
            loadItem(data, 2);
          else if(data.getElement(2) == "party")
            loadParty(data, 2);
          else if(data.getElement(2) == "person")
            loadPerson(data, 2);
          else if(data.getElement(2) == "race")
            loadRace(data, 2);
          else if(data.getElement(2) == "skill")
            loadSkill(data, 2);
          else if(data.getElement(2) == "skillset")
            loadSkillSet(data, 2);
        }
        /* If map element, add new map if it doesn't exist; then send
         * new information to map */
        else if(data.getElement(1) == "map" && data.getKey(1) == "id")
        {
          int map_id = QString::fromStdString(data.getKeyValue(1)).toInt();
          int map_index = -1;

          /* Try and find map index */
          for(int i = 0; i < data_map.size(); i++)
            if(data_map[i]->getID() == map_id)
              map_index = i;

          /* Create the map if it doesn't exist */
          if(map_index == -1)
          {
            data_map.push_back(new EditorMap(map_id, "TEMP", 0, 0,&tile_icons));
            map_index = data_map.size() - 1;
          }

          /* Pass the XML data to the map */
          data_map[map_index]->load(data, 2);
        }
      }

      dialog->setValue(dialog->value() + 1);
    } while(!done);
  }

  /* Clean up core data */
  loadFinish();

  /* Clean up the maps */
  for(int i = 0; i < data_map.size(); i++)
  {
    data_map[i]->setVisibilityRef();

    data_map[i]->tilesThingAdd(true);
    data_map[i]->tilesPersonAdd(true);
    data_map[i]->tilesNPCAdd(true);
    data_map[i]->tilesItemAdd(true);
    data_map[i]->tilesIOAdd(true);
  }

  /* Update the view */
  int index = view_top->currentRow();
  if(index == 0)
    view_top->setCurrentRow(1);
  else
    view_top->setCurrentRow(0);
  view_top->setCurrentRow(index);
}

// TODO: Comment
void GameDatabase::modifyBottomList(int index)
{
  int bold_index = -1;
  QStringList bottom_list;
  int last_row = view_bottom->currentRow();

  /* Clear up the existing list */
  view_bottom->clear();

  switch(index)
  {
    /* -- MAP -- */
    case EditorEnumDb::MAPVIEW:
      for(int i = 0; i < data_map.size(); i++)
      {
        bottom_list << data_map[i]->getNameList();
        if(data_map[i] == current_map)
          bold_index = i;
      }
      break;
    /* -- PARTY -- */
    case EditorEnumDb::PARTYVIEW:
      for(int i = 0; i < data_party.size(); i++)
      {
        bottom_list << data_party[i]->getNameList();
        if(data_party[i] == current_party)
          bold_index = i;
      }
      break;
    /* -- PERSON -- */
    case EditorEnumDb::PERSONVIEW:
      for(int i = 0; i < data_person.size(); i++)
      {
        bottom_list << data_person[i]->getNameList();
        if(data_person[i] == current_person)
          bold_index = i;
      }
      break;
    /* -- ITEM -- */
    case EditorEnumDb::ITEMVIEW:
      for(int i = 0; i < data_item.size(); i++)
      {
        bottom_list << data_item[i]->getNameList();
        if(data_item[i] == current_item)
          bold_index = i;
      }
      break;
    /* -- ACTION -- */
    case EditorEnumDb::ACTIONVIEW:
      for(int i = 0; i < data_action.size(); i++)
      {
        bottom_list << data_action[i]->getNameList();
        if(data_action[i] == current_action)
          bold_index = i;
      }
      break;
    /* -- RACE -- */
    case EditorEnumDb::RACEVIEW:
      for(int i = 0; i < data_race.size(); i++)
      {
        bottom_list << data_race[i]->getNameList();
        if(data_race[i] == current_race)
          bold_index = i;
      }
      break;
    /* -- BATTLE CLASS -- */
    case EditorEnumDb::BATTLECLASSVIEW:
      for(int i = 0; i < data_battleclass.size(); i++)
      {
        bottom_list << data_battleclass[i]->getNameList();
        if(data_battleclass[i] == current_battleclass)
          bold_index = i;
      }
      break;
    /* -- SKILL SET -- */
    case EditorEnumDb::SKILLSETVIEW:
      for(int i = 0; i < data_skillset.size(); i++)
      {
        bottom_list << data_skillset[i]->getNameList();
        if(data_skillset[i] == current_skillset)
          bold_index = i;
      }
      break;
    /* -- SKILL -- */
    case EditorEnumDb::SKILLVIEW:
      for(int i = 0; i < data_skill.size(); i++)
      {
        bottom_list << data_skill[i]->getNameList();
        if(data_skill[i] == current_skill)
          bold_index = i;
      }
      break;
    /* -- BATTLE SCENE -- */
    case EditorEnumDb::BATTLESCENEVIEW:
      for(int i = 0; i < data_battlescene.size(); i++)
      {
        bottom_list << data_battlescene[i]->getNameList();
        if(data_battlescene[i] == current_battlescene)
          bold_index = i;
      }
      break;
    /* -- EQUIPMENT -- */
    case EditorEnumDb::EQUIPMENTVIEW:
      for(int i = 0; i < data_equipment.size(); i++)
      {
        bottom_list << data_equipment[i]->getNameList();
        if(data_equipment[i] == current_equipment)
          bold_index = i;
      }
      break;
    /* -- BUBBY -- */
    case EditorEnumDb::BUBBYVIEW:
      for(int i = 0; i < data_bubby.size(); i++)
      {
        bottom_list << data_bubby[i]->getNameList();
        if(data_bubby[i] == current_bubby)
          bold_index = i;
      }
      break;
    default:
      break;
  }

  /* Set up the bottom view */
  QFont font;
  view_bottom->addItems(bottom_list);
  font.setBold(false);
  for(int i = 0; i < view_bottom->count(); i++)
    view_bottom->item(i)->setFont(font);
  if(bold_index != -1)
  {
    font.setBold(true);
    view_bottom->item(bold_index)->setFont(font);
  }
  view_bottom->setResizeMode(QListView::Adjust);

  /* Replace selected row */
  if(last_row >= 0 && last_row < view_bottom->count())
    view_bottom->setCurrentRow(last_row);

  /* Enable/disable buttons as needed */
  if(view_bottom->count() > 0)
  {
    button_delete->setEnabled(true);
    button_duplicate->setEnabled(true);
  }
  else
  {
    button_delete->setDisabled(true);
    button_duplicate->setDisabled(true);
  }
}

/* Save the game */
void GameDatabase::save(FileHandler* fh, QProgressDialog* dialog,
                        bool game_only, bool selected_map, int sub_index)
{
  if(fh != nullptr && dialog != nullptr)
  {
    /* -- Write application data -- */
    fh->writeXmlElement("app");

    /* Music and Sound */
    data_sounds->save(fh, dialog, game_only);

    /* -- Write end application data -- */
    fh->writeXmlElementEnd();

    /* -- Write game data -- */
    fh->writeXmlElement("game");

    /* Core data */
    fh->writeXmlElement("core");

    /* Battle Scenes */
    for(int i = 0; i < data_battlescene.size(); i++)
    {
      data_battlescene[i]->save(fh, game_only);
      dialog->setValue(dialog->value() + 1);
    }

    /* Actions */
    for(int i = 0; i < data_action.size(); i++)
    {
      data_action[i]->save(fh, game_only);
      dialog->setValue(dialog->value() + 1);
    }

    /* Skills */
    for(int i = 0; i < data_skill.size(); i++)
    {
      data_skill[i]->save(fh, game_only);
      dialog->setValue(dialog->value() + 1);
    }

    /* Skill Sets */
    for(int i = 0; i < data_skillset.size(); i++)
    {
      data_skillset[i]->save(fh, game_only);
      dialog->setValue(dialog->value() + 1);
    }

    /* Classes */
    for(int i = 0; i < data_battleclass.size(); i++)
    {
      data_battleclass[i]->save(fh, game_only, "class");
      dialog->setValue(dialog->value() + 1);
    }

    /* Races */
    for(int i = 0; i < data_race.size(); i++)
    {
      data_race[i]->save(fh, game_only, "race");
      dialog->setValue(dialog->value() + 1);
    }

    /* Items */
    for(int i = 0; i < data_item.size(); i++)
    {
      data_item[i]->save(fh, game_only);
      dialog->setValue(dialog->value() + 1);
    }

    /* Persons */
    for(int i = 0; i < data_person.size(); i++)
    {
      data_person[i]->save(fh, game_only);
      dialog->setValue(dialog->value() + 1);
    }

    /* Parties */
    for(int i = 0; i < data_party.size(); i++)
    {
      data_party[i]->save(fh, game_only);
      dialog->setValue(dialog->value() + 1);
    }

    fh->writeXmlElementEnd();

    /* Maps */
    /* If not the selected map, save all the maps */
    if(!selected_map)
    {
      for(int i = 0; i < data_map.size(); i++)
        data_map[i]->save(fh, dialog, game_only);
    }
    /* Otherwise, save the single map */
    else if(current_map != NULL)
    {
      current_map->save(fh, dialog, game_only, sub_index);
    }

    /* -- Write end game data -- */
    fh->writeXmlElementEnd();
  }
}

/* The widget preferred size - reimplemented */
QSize GameDatabase::sizeHint() const
{
  return minimumSizeHint();
}
