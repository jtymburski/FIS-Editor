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
  setMinimumWidth(8);
  setMaximumWidth(8);

  /* Set up the top list view and connect it to the changing of the
   * bottom list view */
  top_view = new QListWidget(this);
  top_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
  QStringList items;
  items << "Maps" << "Persons" << "Parties" << "Items";
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

  current_map_selection = -1;
  current_person_selection = -1;
  current_party_selection = -1;
  current_item_selection = -1;

  new_button = new QPushButton("New",this);
  del_button = new QPushButton("Delete",this);
  import_button = new QPushButton("Import",this);
  duplicate_button = new QPushButton("Duplicate",this);

  connect(new_button,SIGNAL(clicked()),this,SLOT(createNewResource()));
  connect(del_button,SIGNAL(clicked()),this,SLOT(deleteResource()));
  connect(duplicate_button,SIGNAL(clicked()),this,SLOT(duplicateResource()));
  layout = new QVBoxLayout(this);
  layout->addSpacing(32);
  layout->addWidget(top_view);
  layout->addWidget(bottom_view);

  QHBoxLayout* button_layout = new QHBoxLayout();
  button_layout->addWidget(new_button);
  button_layout->addWidget(del_button);
  button_layout->addWidget(import_button);
  button_layout->addWidget(duplicate_button);
  layout->addLayout(button_layout);


}

GameDatabase::~GameDatabase()
{

}

void GameDatabase::modifySelection(QModelIndex index)
{
  switch(top_view->currentRow())
  {
    case 0:
      current_map = map_pair[index.row()];
      current_map_selection = index.row();
      current_map_index = index.row();
      modifyBottomList(top_view->currentRow());
      emit changeMap(current_map);
      break;
    case 1:
      current_person = person_pair[index.row()];
      current_person_selection = index.row();
      current_person_index = index.row();
      modifyBottomList(top_view->currentRow());
      emit changePerson(current_person);
      break;
    case 2:
      current_party = party_pair[index.row()];
      current_party_selection = index.row();
      current_party_index = index.row();
      modifyBottomList(top_view->currentRow());
      emit changeParty(current_party);
      break;
    case 3:
      current_item = item_pair[index.row()];
      current_item_selection = index.row();
      current_item_index = index.row();
      modifyBottomList(top_view->currentRow());
      emit changeItem(current_item);
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

void GameDatabase::resize()
{
  if(width()< 128)
  {
      setMinimumWidth(256);
      setMaximumWidth(256);
  }
  else
  {
      setMinimumWidth(8);
      setMaximumWidth(8);
  }
}

void GameDatabase::modifyBottomList(int index)
{
  QStringList map_list;
  QStringList person_list;
  QStringList party_list;
  QStringList item_list;
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
      if(map_pair.size()<10)
        name.append(QString::number(0));
      name.append(QString::number(map_pair.size()));
      name.append(" : Some Land");
      map_pair.push_back(new QPair<QString,EditorMap*>
                         (name,new EditorMap(this)));
      current_map_index = map_pair.size()-1;
      modifyBottomList(top_view->currentRow());
      break;
    case 1:
      if(person_pair.size()<10)
        name.append(QString::number(0));
      name.append(QString::number(person_pair.size()));
      name.append(" : Some Guy");
      person_pair.push_back(new QPair<QString,EditorPerson*>
                         (name,new EditorPerson(this)));
      current_person_index = person_pair.size()-1;
      modifyBottomList(top_view->currentRow());
      break;
    case 2:
      if(party_pair.size()<10)
        name.append(QString::number(0));
      name.append(QString::number(party_pair.size()));
      name.append(" : Some Sleuth");
      party_pair.push_back(new QPair<QString,EditorParty*>
                         (name,new EditorParty(this)));
      current_party_index = party_pair.size()-1;
      modifyBottomList(top_view->currentRow());
      break;
    case 3:
      if(item_pair.size()<10)
        name.append(QString::number(0));
      name.append(QString::number(item_pair.size()));
      name.append(" : Some Doohickey");
      item_pair.push_back(new QPair<QString,EditorItem*>
                         (name,new EditorItem(this)));
      current_party_index = item_pair.size()-1;
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
      if(map_pair.size() > 0)
      {
        QString maptemp = map_pair[current_map_index]->first;
        if(map_pair.size()<10)
          name.append(QString::number(0));
        name.append(QString::number(map_pair.size()));
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
      if(person_pair.size() > 0)
      {
        QString persontemp = person_pair[current_person_index]->first;
        if(person_pair.size()<10)
          name.append(QString::number(0));
        name.append(QString::number(person_pair.size()));
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
      if(party_pair.size() > 0)
      {
        QString partytemp = party_pair[current_party_index]->first;
        if(party_pair.size()<10)
          name.append(QString::number(0));
        name.append(QString::number(party_pair.size()));
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
      if(item_pair.size() > 0)
      {
        QString itemtemp = item_pair[current_item_index]->first;
        if(item_pair.size()<10)
          name.append(QString::number(0));
        name.append(QString::number(item_pair.size()));
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
    default:
      break;
  }
}
