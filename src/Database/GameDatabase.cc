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
  /* Set up the top list view and connect it to the changing of the
   * bottom list view */
  top_view = new QListWidget(this);
  top_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
  QStringList items;
  items << "Maps" << "Persons" << "Parties" << "Items" << "Actions"
        << "Races" << "BattleClasses" << "Skill Sets" << "Skills"
        << "Equipment" << "B.U.B.B.I.E's";

  top_view->addItems(items);
  top_view->setCurrentRow(0);
  connect(top_view,SIGNAL(currentRowChanged(int)),
          this,SLOT(modifyBottomList(int)));

  bottom_view = new QListWidget(this);
  bottom_view->setEditTriggers(QAbstractItemView::NoEditTriggers);

  connect(bottom_view,SIGNAL(doubleClicked(QModelIndex)),
         this,SLOT(modifySelection(QModelIndex)));

  connect(bottom_view,SIGNAL(currentRowChanged(int)),
          this,SLOT(modifyIndex(int)));
  /* Sets up temporary data for the bottom list view */
  map_pair.push_back(new QPair<QString,EditorMap*>
                     ("00 : Dynaton",new EditorMap(this)));
  map_pair.push_back(new QPair<QString,EditorMap*>
                     ("01 : Aviation Violation",new EditorMap(this)));
  map_pair.push_back(new QPair<QString,EditorMap*>
                     ("02 : Fish with Frickin' Rockets Attached",
                      new EditorMap(this)));
  map_pair.push_back(new QPair<QString,EditorMap*>
                     ("03 : Bsian Season",new EditorMap(this)));

  person_pair.push_back(new QPair<QString,EditorPerson*>
                        ("00 : Main Character",new EditorPerson(this)));
  person_pair.push_back(new QPair<QString,EditorPerson*>
                        ("01 : Arcadius",new EditorPerson(this)));
  person_pair.push_back(new QPair<QString,EditorPerson*>
                        ("02 : Frosty, A Doopy Snowman",
                                  new EditorPerson(this)));

  party_pair.push_back(new QPair<QString,EditorParty*>
                       ("00 : The Master Sleuth", new EditorParty(this)));
  party_pair.push_back(new QPair<QString,EditorParty*>
                       ("01 : The Doop Troop", new EditorParty(this)));

  item_pair.push_back(new QPair<QString,EditorItem*>
                      ("00 : Fated Oak Saber", new EditorItem(this)));
  item_pair.push_back(new QPair<QString,EditorItem*>
                      ("01 : Bear Arms", new EditorItem(this)));
  current_map_index = 0;
  current_person_index = 0;
  current_party_index = 0;
  current_item_index = 0;
  current_action_index = 0;
  current_race_index = 0;
  current_battleclass_index = 0;
  current_skillset_index = 0;
  current_skill_index = 0;
  current_equipment_index = 0;
  current_bubby_index = 0;

  current_map_selection = -1;
  current_person_selection = -1;
  current_party_selection = -1;
  current_item_selection = -1;
  current_action_selection = -1;
  current_race_selection = -1;
  current_battleclass_selection = -1;
  current_skillset_selection = -1;
  current_skill_selection = -1;
  current_equipment_selection = -1;
  current_bubby_selection = -1;

  current_map_id = map_pair.size()-1;
  current_person_id = person_pair.size()-1;
  current_party_id = party_pair.size()-1;
  current_item_id = item_pair.size()-1;
  current_action_id = action_pair.size()-1;
  current_race_id = race_pair.size()-1;
  current_battleclass_id = battleclass_pair.size()-1;
  current_skillset_id = skillset_pair.size()-1;
  current_skill_id = skill_pair.size()-1;
  current_equipment_id = equipment_pair.size()-1;
  current_bubby_id = bubby_pair.size()-1;

  new_button = new QPushButton("New",this);
  del_button = new QPushButton("Delete",this);
  import_button = new QPushButton("Import",this);
  duplicate_button = new QPushButton("Duplicate",this);

  connect(new_button,SIGNAL(clicked()),this,SLOT(createNewResource()));
  connect(del_button,SIGNAL(clicked()),this,SLOT(deleteResource()));
  connect(duplicate_button,SIGNAL(clicked()),this,SLOT(duplicateResource()));
  layout = new QVBoxLayout(this);
  QLabel *label = new QLabel("Game Database", this);
  label->setAlignment(Qt::AlignHCenter);
  QFont font = label->font();
  font.setBold(true);
  label->setFont(font);
  layout->addWidget(label);
  layout->addWidget(top_view);
  layout->addWidget(bottom_view);

  QHBoxLayout* button_layout = new QHBoxLayout();
  button_layout->addWidget(new_button);
  button_layout->addWidget(del_button);
  button_layout->addWidget(import_button);
  button_layout->addWidget(duplicate_button);
  layout->addLayout(button_layout);
  modifyBottomList(top_view->currentRow());
}

GameDatabase::~GameDatabase()
{

}

void GameDatabase::modifySelection(QModelIndex index)
{
  switch(top_view->currentRow())
  {
    case 0:
      current_name = &map_pair[index.row()]->first;
      current_map = map_pair[index.row()];
      current_map_selection = index.row();
      current_map_index = index.row();
      modifyBottomList(top_view->currentRow());
      emit changeMap(current_map);
      break;
    case 1:
      current_name = &person_pair[index.row()]->first;
      current_person = person_pair[index.row()];
      current_person_selection = index.row();
      current_person_index = index.row();
      modifyBottomList(top_view->currentRow());
      emit changePerson(current_person);
      break;
    case 2:
      current_name = &party_pair[index.row()]->first;
      current_party = party_pair[index.row()];
      current_party_selection = index.row();
      current_party_index = index.row();
      modifyBottomList(top_view->currentRow());
      emit changeParty(current_party);
      break;
    case 3:
      current_name = &item_pair[index.row()]->first;
      current_item = item_pair[index.row()];
      current_item_selection = index.row();
      current_item_index = index.row();
      modifyBottomList(top_view->currentRow());
      emit changeItem(current_item);
      break;
    case 4:
      current_name = &action_pair[index.row()]->first;
      current_action = action_pair[index.row()];
      current_action_selection = index.row();
      current_action_index = index.row();
      modifyBottomList(top_view->currentRow());
      emit changeAction(current_action);
      break;
    case 5:
      current_name = &race_pair[index.row()]->first;
      current_race = race_pair[index.row()];
      current_race_selection = index.row();
      current_race_index = index.row();
      modifyBottomList(top_view->currentRow());
      emit changeRace(current_race);
      break;
    case 6:
      current_name = &battleclass_pair[index.row()]->first;
      current_battleclass = battleclass_pair[index.row()];
      current_battleclass_selection = index.row();
      current_battleclass_index = index.row();
      modifyBottomList(top_view->currentRow());
      emit changeBattleclass(current_battleclass);
      break;
    case 7:
      current_name = &skillset_pair[index.row()]->first;
      current_skillset = skillset_pair[index.row()];
      current_skillset_selection = index.row();
      current_skillset_index = index.row();
      modifyBottomList(top_view->currentRow());
      emit changeSkillset(current_skillset);
      break;
    case 8:
      current_name = &skill_pair[index.row()]->first;
      current_skill = skill_pair[index.row()];
      current_skill_selection = index.row();
      current_skill_index = index.row();
      modifyBottomList(top_view->currentRow());
      emit changeSkill(current_skill);
      break;
    case 9:
      current_name = &equipment_pair[index.row()]->first;
      current_equipment = equipment_pair[index.row()];
      current_equipment_selection = index.row();
      current_equipment_index = index.row();
      modifyBottomList(top_view->currentRow());
      emit changeEquipment(current_equipment);
      break;
    case 10:
      current_name = &bubby_pair[index.row()]->first;
      current_bubby = bubby_pair[index.row()];
      current_bubby_selection = index.row();
      current_bubby_index = index.row();
      modifyBottomList(top_view->currentRow());
      emit changeBubby(current_bubby);
      break;
    default:
      break;
  }
}


void GameDatabase::modifyIndex(int index)
{
  switch(top_view->currentRow())
  {
    case 0:
      current_map_index = index;
      break;
    case 1:
      current_person_index = index;
      break;
    case 2:
      current_party_index = index;
      break;
    case 3:
      current_item_index = index;
      break;
    case 4:
      current_action_index = index;
      break;
    case 5:
      current_race_index = index;
      break;
    case 6:
      current_battleclass_index = index;
      break;
    case 7:
      current_skillset_index = index;
      break;
    case 8:
      current_skill_index = index;
      break;
    case 9:
      current_equipment_index = index;
      break;
    case 10:
      current_bubby_index = index;
      break;
    default:
      break;
  }
/*
  qDebug()<<"Map Index " <<current_map_index;
  qDebug()<<"Person Index " <<current_person_index;
  qDebug()<<"Party Index " <<current_party_index;
  qDebug()<<"Item Index " <<current_item_index;
  qDebug()<<" ";
*/
}

void GameDatabase::modifyBottomList(int index)
{
  QStringList map_list;
  QStringList person_list;
  QStringList party_list;
  QStringList item_list;
  QStringList action_list;
  QStringList race_list;
  QStringList battleclass_list;
  QStringList skillset_list;
  QStringList skill_list;
  QStringList equipment_list;
  QStringList bubby_list;

  QFont font;

  disconnect(bottom_view,SIGNAL(currentRowChanged(int)),
          this,SLOT(modifyIndex(int)));
  bottom_view->clear();

  connect(bottom_view,SIGNAL(currentRowChanged(int)),
          this,SLOT(modifyIndex(int)));
  switch(index)
  {
    case 0:
      for(int i=0; i<map_pair.size(); i++)
        map_list << map_pair[i]->first;
      bottom_view->addItems(map_list);

      font.setBold(false);
      for(int i=0; i<bottom_view->count(); i++)
        bottom_view->item(i)->setFont(font);
      if(current_map_selection != -1)
      {
        font.setBold(true);
        bottom_view->item(current_map_selection)->setFont(font);
      }
      bottom_view->setCurrentRow(current_map_index);
      emit changeMode(EditorEnumDb::MAPVIEW);
      break;
    case 1:
      for(int i=0; i<person_pair.size(); i++)
        person_list << person_pair[i]->first;
      bottom_view->addItems(person_list);

      font.setBold(false);
      for(int i=0; i<bottom_view->count(); i++)
        bottom_view->item(i)->setFont(font);
      if(current_person_selection != -1)
      {
        font.setBold(true);
        bottom_view->item(current_person_selection)->setFont(font);
      }
      bottom_view->setCurrentRow(current_person_index);
      emit changeMode(EditorEnumDb::PERSONVIEW);
      break;
    case 2:
      for(int i=0; i<party_pair.size(); i++)
        party_list << party_pair[i]->first;
      bottom_view->addItems(party_list);

      font.setBold(false);
      for(int i=0; i<bottom_view->count(); i++)
        bottom_view->item(i)->setFont(font);
      if(current_party_selection != -1)
      {
        font.setBold(true);
        bottom_view->item(current_party_selection)->setFont(font);
      }

      bottom_view->setCurrentRow(current_party_index);
      emit changeMode(EditorEnumDb::PARTYVIEW);
      break;
    case 3:
      for(int i=0; i<item_pair.size(); i++)
        item_list << item_pair[i]->first;
      bottom_view->addItems(item_list);

      font.setBold(false);
      for(int i=0; i<bottom_view->count(); i++)
        bottom_view->item(i)->setFont(font);
      if(current_item_selection != -1)
      {
        font.setBold(true);
        bottom_view->item(current_item_selection)->setFont(font);
      }
      bottom_view->setCurrentRow(current_item_index);
      emit changeMode(EditorEnumDb::ITEMVIEW);
      break;
    case 4:
      for(int i=0; i<action_pair.size(); i++)
        action_list << action_pair[i]->first;
      bottom_view->addItems(action_list);

      font.setBold(false);
      for(int i=0; i<bottom_view->count(); i++)
        bottom_view->item(i)->setFont(font);
      if(current_action_selection != -1)
      {
        font.setBold(true);
        bottom_view->item(current_action_selection)->setFont(font);
      }
      bottom_view->setCurrentRow(current_action_index);
      emit changeMode(EditorEnumDb::ACTIONVIEW);
      break;
    case 5:
      for(int i=0; i<race_pair.size(); i++)
        race_list << race_pair[i]->first;
      bottom_view->addItems(race_list);

      font.setBold(false);
      for(int i=0; i<bottom_view->count(); i++)
        bottom_view->item(i)->setFont(font);
      if(current_race_selection != -1)
      {
        font.setBold(true);
        bottom_view->item(current_race_selection)->setFont(font);
      }
      bottom_view->setCurrentRow(current_race_index);
      emit changeMode(EditorEnumDb::RACEVIEW);
      break;
    case 6:
      for(int i=0; i<battleclass_pair.size(); i++)
        battleclass_list << battleclass_pair[i]->first;
      bottom_view->addItems(battleclass_list);

      font.setBold(false);
      for(int i=0; i<bottom_view->count(); i++)
        bottom_view->item(i)->setFont(font);
      if(current_battleclass_selection != -1)
      {
        font.setBold(true);
        bottom_view->item(current_battleclass_selection)->setFont(font);
      }
      bottom_view->setCurrentRow(current_battleclass_index);
      emit changeMode(EditorEnumDb::BATTLECLASSVIEW);
      break;
    case 7:
      for(int i=0; i<skillset_pair.size(); i++)
        skillset_list << skillset_pair[i]->first;
      bottom_view->addItems(skillset_list);

      font.setBold(false);
      for(int i=0; i<bottom_view->count(); i++)
        bottom_view->item(i)->setFont(font);
      if(current_skillset_selection != -1)
      {
        font.setBold(true);
        bottom_view->item(current_skillset_selection)->setFont(font);
      }
      bottom_view->setCurrentRow(current_skillset_index);
      emit changeMode(EditorEnumDb::SKILLSETVIEW);
      break;
    case 8:
      for(int i=0; i<skill_pair.size(); i++)
        skill_list << skill_pair[i]->first;
      bottom_view->addItems(skill_list);

      font.setBold(false);
      for(int i=0; i<bottom_view->count(); i++)
        bottom_view->item(i)->setFont(font);
      if(current_skill_selection != -1)
      {
        font.setBold(true);
        bottom_view->item(current_skill_selection)->setFont(font);
      }
      bottom_view->setCurrentRow(current_skill_index);
      emit changeMode(EditorEnumDb::SKILLVIEW);
      break;
    case 9:
      for(int i=0; i<equipment_pair.size(); i++)
        equipment_list << equipment_pair[i]->first;
      bottom_view->addItems(equipment_list);

      font.setBold(false);
      for(int i=0; i<bottom_view->count(); i++)
        bottom_view->item(i)->setFont(font);
      if(current_equipment_selection != -1)
      {
        font.setBold(true);
        bottom_view->item(current_equipment_selection)->setFont(font);
      }
      bottom_view->setCurrentRow(current_equipment_index);
      emit changeMode(EditorEnumDb::EQUIPMENTVIEW);
      break;
    case 10:
      for(int i=0; i<bubby_pair.size(); i++)
        bubby_list << bubby_pair[i]->first;
      bottom_view->addItems(bubby_list);

      font.setBold(false);
      for(int i=0; i<bottom_view->count(); i++)
        bottom_view->item(i)->setFont(font);
      if(current_bubby_selection != -1)
      {
        font.setBold(true);
        bottom_view->item(current_bubby_selection)->setFont(font);
      }
      bottom_view->setCurrentRow(current_bubby_index);
      emit changeMode(EditorEnumDb::BUBBYVIEW);
      break;
    default:
      break;
  }

}

void GameDatabase::createNewResource()
{
  QString name;
  switch(top_view->currentRow())
  {
    case 0:
      if(current_map_id<9)
        name.append(QString::number(0));
      name.append(QString::number(++current_map_id));
      name.append(" : Some Land");
      map_pair.push_back(new QPair<QString,EditorMap*>
                         (name,new EditorMap(this)));
      current_map_index = map_pair.size()-1;
      modifyBottomList(top_view->currentRow());
      break;
    case 1:
      if(current_person_id<9)
        name.append(QString::number(0));
      name.append(QString::number(++current_person_id));
      name.append(" : Some Guy");
      person_pair.push_back(new QPair<QString,EditorPerson*>
                         (name,new EditorPerson(this)));
      current_person_index = person_pair.size()-1;
      modifyBottomList(top_view->currentRow());
      break;
    case 2:
      if(current_party_id<9)
        name.append(QString::number(0));
      name.append(QString::number(++current_party_id));
      name.append(" : Some Sleuth");
      party_pair.push_back(new QPair<QString,EditorParty*>
                         (name,new EditorParty(this)));
      current_party_index = party_pair.size()-1;
      modifyBottomList(top_view->currentRow());
      break;
    case 3:
      if(current_item_id<9)
        name.append(QString::number(0));
      name.append(QString::number(++current_item_id));
      name.append(" : Some Doohickey");
      item_pair.push_back(new QPair<QString,EditorItem*>
                         (name,new EditorItem(this)));
      current_item_index = item_pair.size()-1;
      modifyBottomList(top_view->currentRow());
      break;
    case 4:
      if(current_action_id<9)
        name.append(QString::number(0));
      name.append(QString::number(++current_action_id));
      name.append(" : Some Act");
      action_pair.push_back(new QPair<QString,EditorAction*>
                         (name,new EditorAction(this)));
      //action_pair.at(action_pair.size() - 1)->second->setBaseAction(Action());
      action_pair.at(action_pair.size() - 1)->second->setNameAndID(action_pair.at(action_pair.size() - 1)->first);
      //qDebug() << " : " << action_pair.at(action_pair.size() - 1)->second->getBaseAction().getID();
      current_action_index = action_pair.size()-1;
      modifyBottomList(top_view->currentRow());
      break;
    case 5:
      if(current_race_id<9)
        name.append(QString::number(0));
      name.append(QString::number(++current_race_id));
      name.append(" : Some Race");
      race_pair.push_back(new QPair<QString,EditorCategory*>
                         (name,new EditorCategory(this)));
      current_race_index = race_pair.size()-1;
      modifyBottomList(top_view->currentRow());
      break;
    case 6:
      if(current_battleclass_id<9)
        name.append(QString::number(0));
      name.append(QString::number(++current_battleclass_id));
      name.append(" : Some Class");
      battleclass_pair.push_back(new QPair<QString,EditorCategory*>
                         (name,new EditorCategory(this)));
      current_battleclass_index = battleclass_pair.size()-1;
      modifyBottomList(top_view->currentRow());
      break;
    case 7:
      if(current_skillset_id<9)
        name.append(QString::number(0));
      name.append(QString::number(++current_skillset_id));
      name.append(" : Some Set Of Skillz");
      skillset_pair.push_back(new QPair<QString,EditorSkillset*>
                         (name,new EditorSkillset(this)));
      current_skillset_index = skillset_pair.size()-1;
      modifyBottomList(top_view->currentRow());
      break;
    case 8:
      if(current_skill_id<9)
        name.append(QString::number(0));
      name.append(QString::number(++current_skill_id));
      name.append(" : Some Skill");
      skill_pair.push_back(new QPair<QString,EditorSkill*>
                         (name,new EditorSkill(this)));
      current_skill_index = skill_pair.size()-1;
      modifyBottomList(top_view->currentRow());
      break;
    case 9:
      if(current_equipment_id<9)
        name.append(QString::number(0));
      name.append(QString::number(++current_equipment_id));
      name.append(" : Some Equipment");
      equipment_pair.push_back(new QPair<QString,EditorEquipment*>
                         (name,new EditorEquipment(this)));
      current_equipment_index = equipment_pair.size()-1;
      modifyBottomList(top_view->currentRow());
      break;
    case 10:
      if(current_bubby_id<9)
        name.append(QString::number(0));
      name.append(QString::number(++current_bubby_id));
      name.append(" : Some Bubby");
      bubby_pair.push_back(new QPair<QString,EditorBubby*>
                         (name,new EditorBubby(this)));
      current_bubby_index = bubby_pair.size()-1;
      modifyBottomList(top_view->currentRow());
      break;
    default:
      break;
  }
}
//TODO: Fix the bugs with this
void GameDatabase::duplicateResource()
{
  QString name;

   if(top_view->currentRow() == 0)
   {
      if(current_map_id > 0)
      {
        QString maptemp = map_pair[current_map_index]->first;
        if(map_pair.size()<9)
          name.append(QString::number(0));
        name.append(QString::number(++current_map_id));
        name.append(" ");
        do
        {
          maptemp.remove(0,1);
        }
        while(maptemp.at(0) != ':');
        maptemp.prepend(name);
        map_pair.push_back(new QPair<QString,EditorMap*>
                          (maptemp,map_pair[current_map_index]->second->clone()));
        current_map_index = map_pair.size()-1;
        modifyBottomList(top_view->currentRow());
      }
    }
   else if(top_view->currentRow() == 1)
   {
      if(current_person_id > 0)
      {
        QString persontemp = person_pair[current_person_index]->first;
        if(person_pair.size()<9)
          name.append(QString::number(0));
        name.append(QString::number(++current_person_id));
        name.append(" ");
        do
        {
          persontemp.remove(0,1);
        }
        while(persontemp.at(0) != ':');
        persontemp.prepend(name);
        person_pair.push_back(new QPair<QString,EditorPerson*>
                          (persontemp,person_pair[current_person_index]->
                           second->clone()));
        current_person_index = person_pair.size()-1;
        modifyBottomList(top_view->currentRow());
      }
    }
   else if(top_view->currentRow() == 2)
   {
      if(current_party_id > 0)
      {
        QString partytemp = party_pair[current_party_index]->first;
        if(party_pair.size()<9)
          name.append(QString::number(0));
        name.append(QString::number(++current_party_id));
        name.append(" ");
        do
        {
          partytemp.remove(0,1);
        }
        while(partytemp.at(0) != ':');
        partytemp.prepend(name);
        party_pair.push_back(new QPair<QString,EditorParty*>
                          (partytemp,party_pair[current_party_index]->
                           second->clone()));
        current_party_index = party_pair.size()-1;
        modifyBottomList(top_view->currentRow());
      }
    }
   else if(top_view->currentRow() == 3)
   {
      if(current_item_id > 0)
      {
        QString itemtemp = item_pair[current_item_index]->first;
        if(item_pair.size()<9)
          name.append(QString::number(0));
        name.append(QString::number(++current_item_id));
        name.append(" ");
        do
        {
          itemtemp.remove(0,1);
        }
        while(itemtemp.at(0) != ':');
        itemtemp.prepend(name);
        item_pair.push_back(new QPair<QString,EditorItem*>
                          (itemtemp,item_pair[current_item_index]->
                           second->clone()));
        current_item_index = item_pair.size()-1;
        modifyBottomList(top_view->currentRow());
      }
    }
   else if(top_view->currentRow() == 4)
   {
      if(current_action_id > 0)
      {
        QString actiontemp = action_pair[current_action_index]->first;
        if(action_pair.size()<9)
          name.append(QString::number(0));
        name.append(QString::number(++current_action_id));
        name.append(" ");
        do
        {
          actiontemp.remove(0,1);
        }
        while(actiontemp.at(0) != ':');
        actiontemp.prepend(name);
        action_pair.push_back(new QPair<QString,EditorAction*>
                          (actiontemp,action_pair[current_action_index]->
                           second->clone()));
        current_action_index = action_pair.size()-1;
        modifyBottomList(top_view->currentRow());
      }
    }
   else if(top_view->currentRow() == 5)
   {
      if(current_race_id > 0)
      {
        QString racetemp = race_pair[current_race_index]->first;
        if(race_pair.size()<9)
          name.append(QString::number(0));
        name.append(QString::number(++current_race_id));
        name.append(" ");
        do
        {
          racetemp.remove(0,1);
        }
        while(racetemp.at(0) != ':');
        racetemp.prepend(name);
        race_pair.push_back(new QPair<QString,EditorCategory*>
                          (racetemp,race_pair[current_race_index]->
                           second->clone()));
        current_race_index = race_pair.size()-1;
        modifyBottomList(top_view->currentRow());
      }
    }
   else if(top_view->currentRow() == 6)
   {
      if(current_battleclass_id > 0)
      {
        QString battleclasstemp = battleclass_pair[current_battleclass_index]
            ->first;
        if(battleclass_pair.size()<9)
          name.append(QString::number(0));
        name.append(QString::number(++current_battleclass_id));
        name.append(" ");
        do
        {
          battleclasstemp.remove(0,1);
        }
        while(battleclasstemp.at(0) != ':');
        battleclasstemp.prepend(name);
        battleclass_pair.push_back(new QPair<QString,EditorCategory*>
                          (battleclasstemp,
                           battleclass_pair[current_battleclass_index]->
                           second->clone()));
        current_battleclass_index = battleclass_pair.size()-1;
        modifyBottomList(top_view->currentRow());
      }
    }
   else if(top_view->currentRow() == 7)
   {
      if(current_skillset_id > 0)
      {
        QString skillsettemp = skillset_pair[current_skillset_index]->first;
        if(skillset_pair.size()<9)
          name.append(QString::number(0));
        name.append(QString::number(++current_skillset_id));
        name.append(" ");
        do
        {
          skillsettemp.remove(0,1);
        }
        while(skillsettemp.at(0) != ':');
        skillsettemp.prepend(name);
        skillset_pair.push_back(new QPair<QString,EditorSkillset*>
                          (skillsettemp,skillset_pair[current_skillset_index]->
                           second->clone()));
        current_skillset_index = skillset_pair.size()-1;
        modifyBottomList(top_view->currentRow());
      }
    }
   else if(top_view->currentRow() == 8)
   {
      if(current_skill_id > 0)
      {
        QString skilltemp = skill_pair[current_skill_index]->first;
        if(skill_pair.size()<9)
          name.append(QString::number(0));
        name.append(QString::number(++current_skill_id));
        name.append(" ");
        do
        {
          skilltemp.remove(0,1);
        }
        while(skilltemp.at(0) != ':');
        skilltemp.prepend(name);
        skill_pair.push_back(new QPair<QString,EditorSkill*>
                          (skilltemp,skill_pair[current_skill_index]->
                           second->clone()));
        current_skill_index = skill_pair.size()-1;
        modifyBottomList(top_view->currentRow());
      }
    }
   else if(top_view->currentRow() == 9)
   {
      if(equipment_pair.size() > 0)
      {
        QString equipmenttemp = equipment_pair[current_equipment_index]->first;
        if(current_equipment_id<9)
          name.append(QString::number(0));
        name.append(QString::number(++current_equipment_id));
        name.append(" ");
        do
        {
          equipmenttemp.remove(0,1);
        }
        while(equipmenttemp.at(0) != ':');
        equipmenttemp.prepend(name);
        equipment_pair.push_back(new QPair<QString,EditorEquipment*>
                          (equipmenttemp,equipment_pair[current_equipment_index]->
                           second->clone()));
        current_equipment_index = equipment_pair.size()-1;
        modifyBottomList(top_view->currentRow());
      }
    }
   else if(top_view->currentRow() == 10)
   {
      if(bubby_pair.size() > 0)
      {
        QString bubbytemp = bubby_pair[current_bubby_index]->first;
        if(current_bubby_id<9)
          name.append(QString::number(0));
        name.append(QString::number(++current_bubby_id));
        name.append(" ");
        do
        {
          bubbytemp.remove(0,1);
        }
        while(bubbytemp.at(0) != ':');
        bubbytemp.prepend(name);
        bubby_pair.push_back(new QPair<QString,EditorBubby*>
                          (bubbytemp,bubby_pair[current_bubby_index]->
                           second->clone()));
        current_bubby_index = bubby_pair.size()-1;
        modifyBottomList(top_view->currentRow());
      }
    }

}
void GameDatabase::deleteResource()
{
  switch(top_view->currentRow())
  {
    case 0:
      if(map_pair.size() > 0)
      {
        map_pair.remove(current_map_index);
        if(current_map_index == current_map_selection)
          current_map_selection = -1;
        else if(current_map_selection > current_map_index)
          current_map_selection--;
        if(current_map_index > 0)
          current_map_index--;
        modifyBottomList(top_view->currentRow());
      }
      break;
    case 1:
      if(person_pair.size() > 0)
      {
        person_pair.remove(current_person_index);
        if(current_person_index == current_person_selection)
          current_person_selection = -1;
        else if(current_person_selection > current_person_index)
          current_person_selection--;
        if(current_person_index > 0)
          current_person_index--;
        modifyBottomList(top_view->currentRow());
      }
      break;
    case 2:
      if(party_pair.size() > 0)
      {
        party_pair.remove(current_party_index);
        if(current_party_index == current_party_selection)
          current_party_selection = -1;
        else if(current_party_selection > current_party_index)
          current_party_selection--;
        if(current_party_index > 0)
          current_party_index--;
        modifyBottomList(top_view->currentRow());
      }
      break;
    case 3:
      if(item_pair.size() > 0)
      {
        item_pair.remove(current_item_index);
        if(current_item_index == current_item_selection)
          current_item_selection = -1;
        else if(current_item_selection > current_item_index)
          current_item_selection--;
        if(current_item_index > 0)
          current_item_index--;
        modifyBottomList(top_view->currentRow());
      }
      break;
    case 4:
      if(action_pair.size() > 0)
      {
        action_pair.remove(current_action_index);
        if(current_action_index == current_action_selection)
          current_action_selection = -1;
        else if(current_action_selection > current_action_index)
          current_action_selection--;
        if(current_action_index > 0)
          current_action_index--;
        modifyBottomList(top_view->currentRow());
      }
      break;
    case 5:
      if(race_pair.size() > 0)
      {
        race_pair.remove(current_race_index);
        if(current_race_index == current_race_selection)
          current_race_selection = -1;
        else if(current_race_selection > current_race_index)
          current_race_selection--;
        if(current_race_index > 0)
          current_race_index--;
        modifyBottomList(top_view->currentRow());
      }
      break;
    case 6:
      if(battleclass_pair.size() > 0)
      {
        battleclass_pair.remove(current_battleclass_index);
        if(current_battleclass_index == current_battleclass_selection)
          current_battleclass_selection = -1;
        else if(current_battleclass_selection > current_battleclass_index)
          current_battleclass_selection--;
        if(current_battleclass_index > 0)
          current_battleclass_index--;
        modifyBottomList(top_view->currentRow());
      }
      break;
    case 7:
      if(skillset_pair.size() > 0)
      {
        skillset_pair.remove(current_skillset_index);
        if(current_skillset_index == current_skillset_selection)
          current_skillset_selection = -1;
        else if(current_skillset_selection > current_skillset_index)
          current_skillset_selection--;
        if(current_skillset_index > 0)
          current_skillset_index--;
        modifyBottomList(top_view->currentRow());
      }
      break;
    case 8:
      if(skill_pair.size() > 0)
      {
        skill_pair.remove(current_skill_index);
        if(current_skill_index == current_skill_selection)
          current_skill_selection = -1;
        else if(current_skill_selection > current_skill_index)
          current_skill_selection--;
        if(current_skill_index > 0)
          current_skill_index--;
        modifyBottomList(top_view->currentRow());
      }
      break;
    case 9:
      if(equipment_pair.size() > 0)
      {
        equipment_pair.remove(current_equipment_index);
        if(current_equipment_index == current_equipment_selection)
          current_equipment_selection = -1;
        else if(current_equipment_selection > current_equipment_index)
          current_equipment_selection--;
        if(current_equipment_index > 0)
          current_equipment_index--;
        modifyBottomList(top_view->currentRow());
      }
      break;
    case 10:
      if(bubby_pair.size() > 0)
      {
        bubby_pair.remove(current_bubby_index);
        if(current_bubby_index == current_bubby_selection)
          current_bubby_selection = -1;
        else if(current_bubby_selection > current_bubby_index)
          current_bubby_selection--;
        if(current_bubby_index > 0)
          current_bubby_index--;
        modifyBottomList(top_view->currentRow());
      }
      break;
    default:
      break;
  }
}

void GameDatabase::updateBottomListName(QString str)
{
  *current_name = str;
  modifyBottomList(top_view->currentRow());
}
