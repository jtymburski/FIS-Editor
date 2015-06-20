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
  /* Clear out variables */
  current_action = NULL;
  current_battleclass = NULL;
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

  /* Top view set-up */
  view_top = new QListWidget(this);
  view_top->setEditTriggers(QAbstractItemView::NoEditTriggers);
  QStringList items;
  items << "Maps" << "Persons" << "Parties" << "Items" << "Actions"
        << "Races" << "BattleClasses" << "Skill Sets" << "Skills"
        << "Equipment" << "B.U.B.B.I.E's";
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

  /* Configure the layout */
  layout = new QVBoxLayout(this);
  QLabel *label = new QLabel("Game Database", this);
  label->setAlignment(Qt::AlignHCenter);
  QFont font = label->font();
  font.setBold(true);
  label->setFont(font);
  layout->addWidget(label);
  layout->addWidget(view_top);
  layout->addWidget(view_bottom);
  QHBoxLayout* button_layout = new QHBoxLayout();
  button_layout->addWidget(button_new);
  button_layout->addWidget(button_delete);
  button_layout->addWidget(button_import);
  button_layout->addWidget(button_duplicate);
  layout->addLayout(button_layout);

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
    case 0:
      if(mapsize_dialog != NULL)
        delete mapsize_dialog;
      mapsize_dialog = EditorMap::createMapDialog(this);
      connect(EditorMap::getDialogButton(mapsize_dialog), SIGNAL(clicked()),
              this, SLOT(createNewMap()));
      mapsize_dialog->show();
      break;
    /* -- PERSON -- */
    case 1:
      name = "New Person";
      if(data_person.size() > 0)
        data_person.push_back(
                       new EditorPerson(data_person.last()->getID() + 1, name));
      else
        data_person.push_back(new EditorPerson(0, name));
      break;
    /* -- PARTY -- */
    case 2:
      name = "New Party";
      if(data_party.size() > 0)
        data_party.push_back(
                         new EditorParty(data_party.last()->getID() + 1, name));
      else
        data_party.push_back(new EditorParty(0, name));
      break;
    /* -- ITEM -- */
    case 3:
      name = "New Item";
      if(data_item.size() > 0)
        data_item.push_back(new EditorItem(data_item.last()->getID() + 1,name));
      else
        data_item.push_back(new EditorItem(0, name));
      break;
    /* -- ACTION -- */
    case 4:
      name = "New Action";
      if(data_action.size() > 0)
        data_action.push_back(
                       new EditorAction(data_action.last()->getID() + 1, name));
      else
        data_action.push_back(new EditorAction(0, name));
      break;
    /* -- RACE -- */
    case 5:
      name = "New Race";
      if(data_race.size() > 0)
        data_race.push_back(
                       new EditorCategory(data_race.last()->getID() + 1, name));
      else
        data_race.push_back(new EditorCategory(0, name));
      break;
    /* -- BATTLE CLASS -- */
    case 6:
      name = "New Battle Class";
      if(data_battleclass.size() > 0)
        data_battleclass.push_back(
                new EditorCategory(data_battleclass.last()->getID() + 1, name));
      else
        data_battleclass.push_back(new EditorCategory(0, name));
      break;
    /* -- SKILL SET -- */
    case 7:
      name = "New Skill Set";
      if(data_skillset.size() > 0)
        data_skillset.push_back(
                   new EditorSkillset(data_skillset.last()->getID() + 1, name));
      else
        data_skillset.push_back(new EditorSkillset(0, name));
      break;
    /* -- SKILL -- */
    case 8:
      name = "New Skill";
      if(data_skill.size() > 0)
        data_skill.push_back(
                         new EditorSkill(data_skill.last()->getID() + 1, name));
      else
        data_skill.push_back(new EditorSkill(0, name));
      break;
    /* -- EQUIPMENT -- */
    case 9:
      name = "New Equipment";
      if(data_equipment.size() > 0)
        data_equipment.push_back(
                 new EditorEquipment(data_equipment.last()->getID() + 1, name));
      else
        data_equipment.push_back(new EditorEquipment(0, name));
      break;
    /* -- BUBBY -- */
    case 10:
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
    int index = view_bottom->currentRow();

    /* Check if it's the bottom row */
    bool bottom = false;
    if((index + 1) == view_bottom->count())
      bottom = true;

    /* Switch through what to do */
    switch(view_top->currentRow())
    {
      /* -- MAP -- */
      case 0:
        if(data_map[index] == current_map)
        {
          emit changeMap(NULL);
          current_map = NULL;
        }
        delete data_map[index];
        data_map.remove(index);
        break;
      /* -- PERSON -- */
      case 1:
        if(data_person[index] == current_person)
        {
          emit changePerson(NULL);
          current_person = NULL;
        }
        delete data_person[index];
        data_person.remove(index);
        break;
      /* -- PARTY -- */
      case 2:
        if(data_party[index] == current_party)
        {
          emit changeParty(NULL);
          current_party = NULL;
        }
        delete data_party[index];
        data_party.remove(index);
        break;
      /* -- ITEM -- */
      case 3:
        if(data_item[index] == current_item)
        {
          emit changeItem(NULL);
          current_item = NULL;
        }
        delete data_item[index];
        data_item.remove(index);
        break;
      /* -- ACTION -- */
      case 4:
        if(data_action[index] == current_action)
        {
          emit changeAction(NULL);
          current_action = NULL;
        }
        delete data_action[index];
        data_action.remove(index);
        break;
      /* -- RACE CLASS -- */
      case 5:
        if(data_race[index] == current_race)
        {
          emit changeRace(NULL);
          current_race = NULL;
        }
        delete data_race[index];
        data_race.remove(index);
        break;
      /* -- BATTLE CLASS -- */
      case 6:
        if(data_battleclass[index] == current_battleclass)
        {
          emit changeBattleclass(NULL);
          current_battleclass = NULL;
        }
        delete data_battleclass[index];
        data_battleclass.remove(index);
        break;
      /* -- SKILL SET -- */
      case 7:
        if(data_skillset[index] == current_skillset)
        {
          emit changeSkillset(NULL);
          current_skillset = NULL;
        }
        delete data_skillset[index];
        data_skillset.remove(index);
        break;
      /* -- SKILL -- */
      case 8:
        if(data_skill[index] == current_skill)
        {
          emit changeSkill(NULL);
          current_skill = NULL;
        }
        delete data_skill[index];
        data_skill.remove(index);
        break;
      /* -- EQUIPMENT -- */
      case 9:
        if(data_equipment[index] == current_equipment)
        {
          emit changeEquipment(NULL);
          current_equipment = NULL;
        }
        delete data_equipment[index];
        data_equipment.remove(index);
        break;
      /* -- BUBBY -- */
      case 10:
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
      case 0:
        data_map.push_back(new EditorMap(data_map.last()->getID() + 1,
                                         "TEMP", 0, 0));
        id = data_map.last()->getID();
        *data_map.last() = *data_map[index];
        data_map.last()->setID(id);
        data_map.last()->setTileIcons(data_map[index]->getTileIcons());
        break;
      /* -- PERSON -- */
      case 1:
        id = data_person.last()->getID();
        *data_person.last() = *data_person[index];
        data_person.last()->setID(id);
        break;
      /* -- PARTY -- */
      case 2:
        id = data_party.last()->getID();
        *data_party.last() = *data_party[index];
        data_party.last()->setID(id);
        break;
      /* -- ITEM -- */
      case 3:
        id = data_item.last()->getID();
        *data_item.last() = *data_item[index];
        data_item.last()->setID(id);
        break;
      /* -- ACTION -- */
      case 4:
        id = data_action.last()->getID();
        *data_action.last() = *data_action[index];
        data_action.last()->setID(id);
        break;
      /* -- RACE CLASS -- */
      case 5:
        id = data_race.last()->getID();
        *data_race.last() = *data_race[index];
        data_race.last()->setID(id);
        break;
      /* -- BATTLE CLASS -- */
      case 6:
        id = data_battleclass.last()->getID();
        *data_battleclass.last() = *data_battleclass[index];
        data_battleclass.last()->setID(id);
        break;
      /* -- SKILL SET -- */
      case 7:
        id = data_skillset.last()->getID();
        *data_skillset.last() = *data_skillset[index];
        data_skillset.last()->setID(id);
        break;
      /* -- SKILL -- */
      case 8:
        id = data_skill.last()->getID();
        *data_skill.last() = *data_skill[index];
        data_skill.last()->setID(id);
        break;
      /* -- EQUIPMENT -- */
      case 9:
        id = data_equipment.last()->getID();
        *data_equipment.last() = *data_equipment[index];
        data_equipment.last()->setID(id);
        break;
      /* -- BUBBY -- */
      case 10:
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
  if(view_top->currentRow() == 0)
    rightclick_menu->exec(QCursor::pos());
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
    case 0:
      current_map = data_map[index];
      emit changeMap(current_map);
      break;
    /* -- PERSON -- */
    case 1:
      current_person = data_person[index];
      emit changePerson(current_person);
      break;
    /* -- PARTY -- */
    case 2:
      current_party = data_party[index];
      emit changeParty(current_party);
      break;
    /* -- ITEM -- */
    case 3:
      current_item = data_item[index];
      emit changeItem(current_item);
      break;
    /* -- ACTION -- */
    case 4:
      current_action = data_action[index];
      emit changeAction(current_action);
      break;
    /* -- RACE -- */
    case 5:
      current_race = data_race[index];
      emit changeRace(current_race);
      break;
    /* -- BATTLE CLASS -- */
    case 6:
      current_battleclass = data_battleclass[index];
      emit changeBattleclass(current_battleclass);
      break;
    /* -- SKILL SET -- */
    case 7:
      current_skillset = data_skillset[index];
      emit changeSkillset(current_skillset);
      break;
    /* -- SKILL -- */
    case 8:
      current_skill = data_skill[index];
      emit changeSkill(current_skill);
      break;
    /* -- EQUIPMENT -- */
    case 9:
      current_equipment = data_equipment[index];
      emit changeEquipment(current_equipment);
      break;
    /* -- BUBBY -- */
    case 10:
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
  switch(index)
  {
    /* -- MAP -- */
    case 0:
      emit changeMode(EditorEnumDb::MAPVIEW);
      break;
    /* -- PERSON -- */
    case 1:
      emit changeMode(EditorEnumDb::PERSONVIEW);
      break;
    /* -- PARTY -- */
    case 2:
      emit changeMode(EditorEnumDb::PARTYVIEW);
      break;
    /* -- ITEM -- */
    case 3:
      emit changeMode(EditorEnumDb::ITEMVIEW);
      break;
    /* -- ACTION -- */
    case 4:
      emit changeMode(EditorEnumDb::ACTIONVIEW);
      break;
    /* -- RACE -- */
    case 5:
      emit changeMode(EditorEnumDb::RACEVIEW);
      break;
    /* -- BATTLE CLASS -- */
    case 6:
      emit changeMode(EditorEnumDb::BATTLECLASSVIEW);
      break;
    /* -- SKILL SET -- */
    case 7:
      // TODO: Move to modifySelection()
      emit changeMode(EditorEnumDb::SKILLSETVIEW);
//      if(skillset_pair.size() != 0 && current_skillset_selection != -1)
//      {
//        current_name = &skillset_pair[current_skillset_selection]->first;
//        skillset_pair.at(current_skillset_selection)->second->
//                      setTotalSkillsList(skill_pair);
//      }
      break;
    /* -- SKILL -- */
    case 8:
      // TODO: Move to modifySelection()
      emit changeMode(EditorEnumDb::SKILLVIEW);
//      if(skill_pair.size() != 0 && current_skill_selection != -1)
//      {
//        current_name = &skill_pair[current_skill_selection]->first;
//        skill_pair.at(current_skill_selection)->second->
//                      setTotalActionsList(&action_pair);
//      }
      break;
    /* -- EQUIPMENT -- */
    case 9:
      emit changeMode(EditorEnumDb::EQUIPMENTVIEW);
      break;
    /* -- BUBBY -- */
    case 10:
      emit changeMode(EditorEnumDb::BUBBYVIEW);
      break;
    default:
      break;
  }

  modifyBottomList(index);
}

// TODO: Comment
void GameDatabase::updateBottomListName(QString str)
{
  (void)str;
  modifyBottomList(view_top->currentRow());
}

/* Updates event objects in the map database class */
// TODO: Comment
void GameDatabase::updateEventObjects()
{
  /* List of Items */
  QVector<QString> item_list;
  for(int i = 0; i < data_item.size(); i++)
    item_list.push_back(data_item[i]->getNameList());
  emit updatedItems(item_list);

  /* List of maps */
  QVector<QString> map_list;
  for(int i = 0; i < data_map.size(); i++)
    map_list.push_back(data_map[i]->getNameList());
  emit updatedMaps(map_list);
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Delete the game */
void GameDatabase::deleteAll()
{
  /* Get the current top view */
  int index = view_top->currentRow();

  /* Action clean-up */
  emit changeAction(NULL);
  for(int i = 0; i < data_action.size(); i++)
    delete data_action[i];
  data_action.clear();

  /* Battle Class clean-up */
  emit changeBattleclass(NULL);
  for(int i = 0; i < data_battleclass.size(); i++)
    delete data_battleclass[i];
  data_battleclass.clear();

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
  emit changeItem(NULL);
  for(int i = 0; i < data_item.size(); i++)
    delete data_item[i];
  data_item.clear();

  /* Map clean-up */
  emit changeMap(NULL);
  for(int i = 0; i < data_map.size(); i++)
    delete data_map[i];
  data_map.clear();

  /* Party clean-up */
  emit changeParty(NULL);
  for(int i = 0; i < data_party.size(); i++)
    delete data_party[i];
  data_party.clear();

  /* Person clean-up */
  emit changePerson(NULL);
  for(int i = 0; i < data_person.size(); i++)
    delete data_person[i];
  data_person.clear();

  /* Race clean-up */
  emit changeRace(NULL);
  for(int i = 0; i < data_race.size(); i++)
    delete data_race[i];
  data_race.clear();

  /* Skill clean-up */
  emit changeSkill(NULL);
  for(int i = 0; i < data_skill.size(); i++)
    delete data_skill[i];
  data_skill.clear();

  /* Skillset clean-up */
  emit changeSkillset(NULL);
  for(int i = 0; i < data_skillset.size(); i++)
    delete data_skillset[i];
  data_skillset.clear();

  /* Reset the view */
  if(index == 0)
    view_top->setCurrentRow(1);
  else
    view_top->setCurrentRow(0);
  view_top->setCurrentRow(index);
}

/* Load the game */
void GameDatabase::load(FileHandler* fh)
{
  if(fh != NULL)
  {
    XmlData data;
    bool done = false;
    bool read_success = true;
    bool success = true;

    /* Initiate the progress dialog for loading */
    QProgressDialog progress("", "", 0, fh->getCount(), this);
    progress.setWindowTitle("Loading");
    progress.setCancelButton(NULL);
    progress.setLabelText("Loading Game...");
    progress.setWindowModality(Qt::WindowModal);
    int value = 0;
    progress.setValue(value);
    progress.setMinimumDuration(0);
    progress.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint |
                            Qt::WindowTitleHint| Qt::WindowSystemMenuHint);
    progress.show();

    /* Loop through all elements */
    do
    {
      /* Read set of XML data */
      data = fh->readXmlData(&done, &read_success);
      success &= read_success;

      /* Only validate if wrapped within game and successful read */
      if(success && data.getElement(0) == "game")
      {
        /* If map element, add new map if it doesn't exist; then send
         * new information to map */
        if(data.getElement(1) == "map" && data.getKey(1) == "id")
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
            data_map.push_back(new EditorMap(map_id,"TEMP",0,0,&tile_icons));
            map_index = data_map.size() - 1;
          }

          /* Pass the XML data to the map */
          data_map[map_index]->load(data, 2);
        }
      }

      progress.setValue(++value);
    } while(!done);
  }

  /* Clean up the maps */
  for(int i = 0; i < data_map.size(); i++)
  {
    data_map[i]->tilesThingAdd(true);
    data_map[i]->tilesPersonAdd(true);
    data_map[i]->tilesNPCAdd(true);
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
    case 0:
      /* Fill the data */
      for(int i = 0; i < data_map.size(); i++)
      {
        bottom_list << data_map[i]->getNameList();
        if(data_map[i] == current_map)
          bold_index = i;
      }
      break;
    /* -- PERSON -- */
    case 1:
      /* Fill the data */
      for(int i = 0; i < data_person.size(); i++)
      {
        bottom_list << data_person[i]->getNameList();
        if(data_person[i] == current_person)
          bold_index = i;
      }
      break;
    /* -- PARTY -- */
    case 2:
      /* Fill the data */
      for(int i = 0; i < data_party.size(); i++)
      {
        bottom_list << data_party[i]->getNameList();
        if(data_party[i] == current_party)
          bold_index = i;
      }
      break;
    /* -- ITEM -- */
    case 3:
      /* Fill the data */
      for(int i = 0; i < data_item.size(); i++)
      {
        bottom_list << data_item[i]->getNameList();
        if(data_item[i] == current_item)
          bold_index = i;
      }
      break;
    /* -- ACTION -- */
    case 4:
      /* Fill the data */
      for(int i = 0; i < data_action.size(); i++)
      {
        bottom_list << data_action[i]->getNameList();
        if(data_action[i] == current_action)
          bold_index = i;
      }
      break;
    /* -- RACE -- */
    case 5:
      /* Fill the data */
      for(int i = 0; i < data_race.size(); i++)
      {
        bottom_list << data_race[i]->getNameList();
        if(data_race[i] == current_race)
          bold_index = i;
      }
      break;
    /* -- BATTLE CLASS -- */
    case 6:
      /* Fill the data */
      for(int i = 0; i < data_battleclass.size(); i++)
      {
        bottom_list << data_battleclass[i]->getNameList();
        if(data_battleclass[i] == current_battleclass)
          bold_index = i;
      }
      break;
    /* -- SKILL SET -- */
    case 7:
      /* Fill the data */
      for(int i = 0; i < data_skillset.size(); i++)
      {
        bottom_list << data_skillset[i]->getNameList();
        if(data_skillset[i] == current_skillset)
          bold_index = i;
      }
      break;
    /* -- SKILL -- */
    case 8:
      /* Fill the data */
      for(int i = 0; i < data_skill.size(); i++)
      {
        bottom_list << data_skill[i]->getNameList();
        if(data_skill[i] == current_skill)
          bold_index = i;
      }
      break;
    /* -- EQUIPMENT -- */
    case 9:
      /* Fill the data */
      for(int i = 0; i < data_equipment.size(); i++)
      {
        bottom_list << data_equipment[i]->getNameList();
        if(data_equipment[i] == current_equipment)
          bold_index = i;
      }
      break;
    /* -- BUBBY -- */
    case 10:
      /* Fill the data */
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
void GameDatabase::save(FileHandler* fh, bool game_only,
                        bool selected_map, int sub_index)
{
  if(fh != NULL)
  {
    /* Determine total number of things to be saved */
    int count = 0;
    if(!selected_map)
    {
      for(int i = 0; i < data_map.size(); i++)
        count += data_map[i]->getSaveCount();
    }
    else if(current_map != NULL)
    {
      count += current_map->getSaveCount(sub_index);
    }

    /* Create the new save dialog */
    QProgressDialog progress("", "", 0, count, this);
    if(game_only)
    {
      progress.setWindowTitle("Exporting");
      progress.setLabelText("Exporting Game...");
    }
    else
    {
      progress.setWindowTitle("Saving");
      progress.setLabelText("Saving Game...");
    }
    progress.setCancelButton(NULL);
    progress.setWindowModality(Qt::WindowModal);
    progress.setMinimumDuration(0);
    progress.setValue(0);
    progress.setWindowFlags(Qt::Window | Qt::CustomizeWindowHint |
                            Qt::WindowTitleHint| Qt::WindowSystemMenuHint);
    progress.show();

    /* Write starting element */
    fh->writeXmlElement("game");

    /* If not the selected map, save all the maps */
    if(!selected_map)
    {
      for(int i = 0; i < data_map.size(); i++)
        data_map[i]->save(fh, &progress, game_only);
    }
    /* Otherwise, save the single map */
    else if(current_map != NULL)
    {
      current_map->save(fh, &progress, game_only, sub_index);
    }

    fh->writeXmlElementEnd();
  }
}

/* Temp start - TODO: REMOVE */
void GameDatabase::tempMake()
{
  data_map.push_back(new EditorMap(0, "TEST", 100, 100, &tile_icons));
  current_map = data_map.last();
  modifyBottomList(view_top->currentRow());
  view_bottom->setCurrentRow(view_bottom->count() - 1);
  emit changeMap(current_map);
}
