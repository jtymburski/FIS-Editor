/*******************************************************************************
 * Class Name: EditorParty
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Party
 ******************************************************************************/
#include "Database/EditorParty.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Base constructor function */
EditorParty::EditorParty(QWidget *parent) : QWidget(parent)
{
  /* Base data */
  id = 0;
  name_base = "Blank";
  name_curr = name_base;

  /* Layout */
  createLayout();
  loadWorkingInfo();
}

/* Constructor function with id and name */
EditorParty::EditorParty(int id, QString name, QWidget* parent)
           : EditorParty(parent)
{
  setID(id);
  setName(name);
  saveWorking();
}

/* Copy constructor */
EditorParty::EditorParty(const EditorParty &source) : EditorParty()
{
  copySelf(source);
}

/* Destructor function */
EditorParty::~EditorParty()
{
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/* Copy function, to be called by a copy or equal operator constructor */
void EditorParty::copySelf(const EditorParty &source)
{
  // TODO: Implementation
  //name = source.name;
  //party = source.party;
}

/* Creates interface layout */
void EditorParty::createLayout()
{
  /* Layout */
  QGridLayout* layout = new QGridLayout(this);
  layout->setColumnStretch(5, 1);
  layout->setRowMinimumHeight(7, 15);
  layout->setRowStretch(5, 1);

  /* ID */
  QLabel* lbl_id = new QLabel("ID", this);
  layout->addWidget(lbl_id, 0, 0);
  edit_id = new QLineEdit(this);
  edit_id->setDisabled(true);
  layout->addWidget(edit_id, 0, 1);

  /* Name */
  QLabel* lbl_name = new QLabel("Name", this);
  layout->addWidget(lbl_name, 1, 0);
  edit_name = new QLineEdit(this);
  connect(edit_name, SIGNAL(textEdited(QString)),
          this, SLOT(changedName(QString)));
  layout->addWidget(edit_name, 1, 1, 1, 2);

  /* Category */
  QLabel* lbl_classify = new QLabel("Classification", this);
  layout->addWidget(lbl_classify, 2, 0);
  combo_classify = new QComboBox(this);
  for(int i = 0; i < (int)PartyType::FINAL_BOSS; i++)
  {
    QString t = QString::fromStdString(Helpers::partyTypeToStr((PartyType)i));
    t = t.toLower();
    t = t.replace('_', ' ');
    t[0] = t.at(0).toUpper();
    combo_classify->addItem(t);
  }
  connect(combo_classify, SIGNAL(currentIndexChanged(QString)),
          this, SLOT(changedClassify(QString)));
  layout->addWidget(combo_classify, 2, 1, 1, 2);

  /* Inventory */
  QLabel* lbl_inv = new QLabel("Items in Inventory", this);
  layout->addWidget(lbl_inv, 3, 0, 1, 3, Qt::AlignHCenter);
  btn_item_add = new QPushButton("Add Item", this);
  btn_item_add->setDisabled(true);
  connect(btn_item_add, SIGNAL(clicked()), this, SLOT(btnItemAdd()));
  layout->addWidget(btn_item_add, 4, 0, 1, 2);
  btn_item_rem = new QPushButton("Remove Item", this);
  btn_item_rem->setDisabled(true);
  connect(btn_item_rem, SIGNAL(clicked()), this, SLOT(btnItemRemove()));
  layout->addWidget(btn_item_rem, 4, 2);
  list_items_all = new QListWidget(this);
  connect(list_items_all, SIGNAL(currentRowChanged(int)),
          this, SLOT(listItemAllChanged(int)));
  layout->addWidget(list_items_all, 5, 0, 1, 2);
  list_items_used = new QListWidget(this);
  connect(list_items_used, SIGNAL(currentRowChanged(int)),
          this, SLOT(listItemUsedChanged(int)));
  connect(list_items_used, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
          this, SLOT(listItemEdited(QListWidgetItem*)));
  layout->addWidget(list_items_used, 5, 2);
  lbl_inv_details = new QLabel("Fill: 0 / 200", this);
  layout->addWidget(lbl_inv_details, 6, 0, 1, 3, Qt::AlignHCenter);

  /* Persons */
  QLabel* lbl_person = new QLabel("Persons in Party", this);
  layout->addWidget(lbl_person, 0, 3, 1, 2, Qt::AlignHCenter);
  btn_person_add = new QPushButton("Add Person", this);
  btn_person_add->setDisabled(true);
  connect(btn_person_add, SIGNAL(clicked()), this, SLOT(btnPersonAdd()));
  layout->addWidget(btn_person_add, 1, 3);
  btn_person_rem = new QPushButton("Remove Person", this);
  btn_person_rem->setDisabled(true);
  connect(btn_person_rem, SIGNAL(clicked()), this, SLOT(btnPersonRemove()));
  layout->addWidget(btn_person_rem, 1, 4);
  list_persons_all = new QListWidget(this);
  connect(list_persons_all, SIGNAL(currentRowChanged(int)),
          this, SLOT(listPersonAllChanged(int)));
  layout->addWidget(list_persons_all, 2, 3, 4, 1);
  list_persons_used = new QListWidget(this);
  connect(list_persons_used, SIGNAL(currentRowChanged(int)),
          this, SLOT(listPersonUsedChanged(int)));
  connect(list_persons_used, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
          this, SLOT(listPersonEdited(QListWidgetItem*)));
  layout->addWidget(list_persons_used, 2, 4, 4, 1);
  lbl_person_details = new QLabel("Fill: 0 / 5", this);
  layout->addWidget(lbl_person_details, 6, 3, 1, 2, Qt::AlignHCenter);

  /* Reset / Save Buttons */
  QPushButton* btn_reset = new QPushButton("Reset", this);
  connect(btn_reset, SIGNAL(clicked()), this, SLOT(btnReset()));
  layout->addWidget(btn_reset, 8, 3);
  QPushButton* btn_save = new QPushButton("Save", this);
  connect(btn_save, SIGNAL(clicked()), this, SLOT(btnSave()));
  layout->addWidget(btn_save, 8, 4);
}

/* Loads working info into UI objects */
void EditorParty::loadWorkingInfo()
{
  // TODO: Implementation
}

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/

/* Button Triggers */
void EditorParty::btnItemAdd()
{
  // TODO: Implementation
}

/* Button Triggers */
void EditorParty::btnItemRemove()
{
  // TODO: Implementation
}

/* Button Triggers */
void EditorParty::btnPersonAdd()
{
  // TODO: Implementation
}

/* Button Triggers */
void EditorParty::btnPersonRemove()
{
  // TODO: Implementation
}

/* Button Triggers */
void EditorParty::btnReset()
{
  // TODO: Implementation
}

/* Button Triggers */
void EditorParty::btnSave()
{
  // TODO: Implementation
}

/* Widget Change Triggers */
void EditorParty::changedClassify(QString index)
{
  // TODO: Implementation
}

/* Widget Change Triggers */
void EditorParty::changedName(QString name)
{
  // TODO: Implementation
}

/* List Index Widget Changes */
void EditorParty::listItemAllChanged(int index)
{
  // TODO: Implementation
}

/* List Index Widget Changes */
void EditorParty::listItemEdited(QListWidgetItem*)
{
  // TODO: Implementation
}

/* List Index Widget Changes */
void EditorParty::listItemUsedChanged(int index)
{
  // TODO: Implementation
}

/* List Index Widget Changes */
void EditorParty::listPersonAllChanged(int index)
{
  // TODO: Implementation
}

/* List Index Widget Changes */
void EditorParty::listPersonEdited(QListWidgetItem*)
{
  // TODO: Implementation
}

/* List Index Widget Changes */
void EditorParty::listPersonUsedChanged(int index)
{
  // TODO: Implementation
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Returns the ID of the party */
int EditorParty::getID() const
{
  return id;
}

/* Returns the name of the party */
QString EditorParty::getName() const
{
  return name_curr;
}

/* Returns the name of the party for listing */
QString EditorParty::getNameList()
{
  return EditorHelpers::getListString(getID(), getName());
}

/* Loads the object data */
void EditorParty::load(XmlData data, int index)
{
  // TODO: Implementation
}

/* Resets the working set trigger */
void EditorParty::resetWorking()
{
  // TODO: Implementation
}

/* Saves the object data */
void EditorParty::save(FileHandler* fh, bool game_only)
{
  // TODO: Implementation
}

/* Saves the working set trigger */
void EditorParty::saveWorking()
{
  // TODO: Implementation
}

/* Sets the ID of the party */
void EditorParty::setID(int id)
{
  this->id = id;
  edit_id->setText(QString::number(id));
}

/* Sets the name of the party */
void EditorParty::setName(QString name)
{
  name_curr = name;
  edit_name->setText(name);
  emit nameChange(name);
}

/* Update Calls for data */
void EditorParty::updateItems(QVector<EditorItem*> items,
                              bool update_working)
{
  // TODO: Implementation
}

/* Update Calls for data */
void EditorParty::updatePersons(QVector<EditorPerson*> persons,
                                bool update_working)
{
  // TODO: Implementation
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/* The copy operator */
EditorParty& EditorParty::operator= (const EditorParty &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
