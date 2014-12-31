/*******************************************************************************
 * Class Name: MapDatabase
 * Date Created: December 23, 2014
 * Inheritance: QWidget
 * Description: Database on the left hand side that contains all of the sprite
 * creation/selection and editing components.
 ******************************************************************************/
#include "View/MapDatabase.h"

MapDatabase::MapDatabase(QWidget *parent) : QWidget(parent)
{
  /* Top view */
  top_view = new QListWidget(this);
  top_view->setEditTriggers(QAbstractItemView::NoEditTriggers);
  QStringList items;
  items << "Raw Images" << "Sprites";
  top_view->addItems(items);
  top_view->setCurrentRow(0);

  /* Sets up a QListWidget */
  /* Sets up a scroll area with the images tab */
  raw_view = new RawImageView(this);
  //raw_view->hide();

  /* Sets up a scroll area with the sprites tab */
  sprite_view = new SpriteView(this);
  //sprite_view->show();
  //connect(raw_view->getToolbox(),SIGNAL(sendUpEditorSprite(EditorSprite*)),
  //        sprite_view,SLOT(addEditorSprite(EditorSprite*)));

  //addTab(raw_view,"Raw Images");
  //addTab(sprite_view,"Sprites");

  layout = new QVBoxLayout(this);
  //QLabel *label = new QLabel("Game Database", this);
  //label->setAlignment(Qt::AlignHCenter);
  //QFont font = label->font();
  //font.setBold(true);
  //label->setFont(font);
  //layout->addWidget(label);
  layout->addWidget(top_view);
  layout->addWidget(raw_view);
  layout->addWidget(sprite_view);

  //QHBoxLayout* button_layout = new QHBoxLayout();
  //button_layout->addWidget(new_button);
  //button_layout->addWidget(del_button);
  //button_layout->addWidget(import_button);
  //button_layout->addWidget(duplicate_button);
  //layout->addLayout(button_layout);
  //modifyBottomList(top_view->currentRow());

/*
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
*/
}

/*
 * Description: Destructor Function
 */
MapDatabase::~MapDatabase()
{
  //delete raw_view;
  //delete sprite_view;
}

RawImageView* MapDatabase::getRawView()
{
  return raw_view;
}

SpriteView* MapDatabase::getSpriteView()
{
  return sprite_view;
}

