/*******************************************************************************
 * Class Name: EditorParty
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Party
 ******************************************************************************/
#include "Database/EditorParty.h"
#include <QDebug>

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
  /* Core data */
  party_base = source.party_base;
  id = source.id;
  name_base = source.name_base;

  /* Sub data */
  item_set_base = source.item_set_base;
  items_all = source.items_all;
  person_set_base = source.person_set_base;
  persons_all = source.persons_all;

  resetWorking();
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
  for(int i = 0; i <= (int)PartyType::FINAL_BOSS; i++)
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
  connect(list_items_all, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
          this, SLOT(listItemAllDClicked(QListWidgetItem*)));
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
  connect(list_persons_all, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
          this, SLOT(listPersonAllDClicked(QListWidgetItem*)));
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

/* Returns calculated mass of inventory */
int EditorParty::getInvMass()
{
  int total = 0;

  QMapIterator<int,int> i(item_set);
  while(i.hasNext())
  {
    i.next();
    EditorItem* item = getItem(i.key());
    if(item != NULL)
      total += item->getMass() * i.value();
  }

  return total;
}

/* Returns calculated mass remaining of inventory */
int EditorParty::getInvMassRemain()
{
  int mass = getInvMass();
  int limit = party_curr.getInventory()->getMassLimit();
  return (limit - mass);
}

/* Returns item by ID - from total list */
EditorItem* EditorParty::getItem(int id)
{
  for(int i = 0; i < items_all.size(); i++)
    if(items_all[i]->getID() == id)
      return items_all[i];
  return NULL;
}

/* Loads working info into UI objects */
void EditorParty::loadWorkingInfo()
{
  /* ID */
  edit_id->setText(QString::number(id));

  /* Name */
  edit_name->setText(getName());

  /* Classification */
  combo_classify->setCurrentIndex((int)party_curr.getPartyType());

  /* Items in Inventory */
  list_items_all->clear();
  for(int i = 0; i < items_all.size(); i++)
    list_items_all->addItem(items_all[i]->getNameList());
  updateItemList();

  /* Persons in Party */
  list_persons_all->clear();
  for(int i = 0; i < persons_all.size(); i++)
    list_persons_all->addItem(persons_all[i]->getNameList());
  updatePersonList();
}

/* Sort Person Set */
void EditorParty::sortPersons()
{
  QMap<int,int> sort_set;

  /* Go through list and add to map */
  for(int i = 0; i < person_set.size(); i++)
    sort_set.insertMulti(person_set[i].first, person_set[i].second);

  /* Go through map and add back to set */
  person_set.clear();
  QList<int> keys = sort_set.uniqueKeys();
  for(int i = 0; i < keys.size(); i++)
  {
    QList<int> vals = sort_set.values(keys[i]);
    qSort(vals);
    for(int j = 0; j < vals.size(); j++)
      person_set.push_back(QPair<int,int>(keys[i],vals[j]));
  }
}

/* Update object lists */
void EditorParty::updateItemList()
{
  int index = list_items_used->currentRow();
  list_items_used->clear();

  /* Loop through items */
  QVector<int> items_remove;
  QMapIterator<int,int> i(item_set);
  while(i.hasNext())
  {
    bool found = false;
    i.next();

    /* Loop through entire set to find */
    for(int j = 0; !found && j < items_all.size(); j++)
    {
      if(items_all[j]->getID() == i.key())
      {
        found = true;
        list_items_used->addItem(items_all[j]->getNameList() + " x " +
                                 QString::number(i.value()));
      }
    }

    /* If not found, add to list */
    if(!found)
      items_remove.push_back(i.key());
  }

  /* If not found, delete IDs */
  for(int i = 0; i < items_remove.size(); i++)
    item_set.remove(items_remove[i]);

  /* If index is valid, select row */
  if(index >= 0)
  {
    if(index >= list_items_used->count())
      list_items_used->setCurrentRow(list_items_used->count() - 1);
    else
      list_items_used->setCurrentRow(index);
  }

  /* Update fill label */
  lbl_inv_details->setText("Count: " + QString::number(getInvMass()) + " / " +
                    QString::number(party_curr.getInventory()->getMassLimit()));
}

/* Update object lists */
void EditorParty::updatePersonList()
{
  int index = list_persons_used->currentRow();
  list_persons_used->clear();
  QVector<bool> person_used;
  for(int i = 0; i < person_set.size(); i++)
    person_used.push_back(false);

  sortPersons();

  /* Loop through persons */
  for(int i = 0; i < person_set.size(); i++)
  {
    for(int j = 0; !person_used[i] && j < persons_all.size(); j++)
    {
      if(persons_all[j]->getID() == person_set[i].first)
      {
        person_used[i] = true;
        list_persons_used->addItem(persons_all[j]->getNameList() + " - LVL " +
                                   QString::number(person_set[i].second));
      }
    }
  }

  /* If not found, delete IDs */
  for(int i = person_set.size() - 1; i >= 0; i--)
    if(!person_used[i])
      person_set.remove(i);

  /* If index is valid, select row */
  if(index >= 0)
  {
    if(index >= list_persons_used->count())
      list_persons_used->setCurrentRow(list_persons_used->count() - 1);
    else
      list_persons_used->setCurrentRow(index);
  }

  /* Update fill label */
  lbl_person_details->setText("Fill: " + QString::number(person_set.size()) +
                              " / " +
                              QString::number(party_curr.getMaxSize()));
}

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/

/* Button Triggers */
void EditorParty::btnItemAdd()
{
  if(list_items_all->currentRow() >= 0)
  {
    QString list_item = list_items_all->currentItem()->text();
    QStringList list_split = list_item.split(':');
    if(list_split.size() == 2)
    {
      /* Try to find item first */
      int item_id = list_split.front().toInt();
      bool exists = item_set.contains(item_id);
      EditorItem* ref = getItem(item_id);

      /* Check reference item first */
      if(ref != NULL)
      {
        /* Limit max */
        int max = Inventory::kMIN_EACH_ITEM;
        if(exists)
          max -= item_set.value(item_id);
        if(ref->getMass() > 0)
        {
          int item_fit = (int)(getInvMassRemain() * 1.0 / ref->getMass());
          if(item_fit < max)
            max = item_fit;
        }

        /* Get new count */
        if(max > 0)
        {
          bool ok;
          int count = QInputDialog::getInt(this, "Count?", "Item Count:", 1, 1,
                                           max, 1, &ok);
          if(ok)
          {
            if(exists)
              item_set.insert(item_id, item_set.value(item_id) + count);
            else
              item_set.insert(item_id, count);
            updateItemList();
          }
        }
        /* Otherwise, no room in stack */
        else
        {
          QMessageBox::information(this, "Add Fail",
                                               "No room in inventory for item");
        }
      }
      /* Reference item invalid */
      else
      {
        QMessageBox::information(this, "Add Fail", "Item doesn't exist. WTF");
      }
    }
  }
}

/* Button Triggers */
void EditorParty::btnItemRemove()
{
  if(list_items_used->currentRow() >= 0)
  {
    QString list_item = list_items_used->currentItem()->text();
    QStringList list_split = list_item.split(':');
    if(list_split.size() == 2)
    {
      /* Find the item and delete */
      int item_id = list_split.front().toInt();
      item_set.remove(item_id);
      updateItemList();
    }
  }
}

/* Button Triggers */
void EditorParty::btnPersonAdd()
{
  if(list_persons_all->currentRow() >= 0)
  {
    if(person_set.size() < (int)party_curr.getMaxSize())
    {
      bool ok;
      int lvl = QInputDialog::getInt(this, "Level?", "Person Level:", 1, 1,
                                     EditorEnumDb::kMAX_PERSON_LVL, 1, &ok);
      if(ok)
      {
        /* Get the person ID */
        QString list_person = list_persons_all->currentItem()->text();
        QStringList list_split = list_person.split(':');
        if(list_split.size() == 2)
        {
          /* Insert new person and reload */
          person_set.push_back(QPair<int,int>(list_split.front().toInt(), lvl));
          updatePersonList();
        }
      }
    }
    else
    {
      QMessageBox::information(this, "Add Fail",
                               tr("Party too full to add another person. ") +
                               tr("Delete some to add more."));
    }
  }
}

/* Button Triggers */
void EditorParty::btnPersonRemove()
{
  if(list_persons_used->currentRow() >= 0)
  {
    person_set.remove(list_persons_used->currentRow());
    updatePersonList();
  }
}

/* Button Triggers */
void EditorParty::btnReset()
{
  resetWorking();
}

/* Button Triggers */
void EditorParty::btnSave()
{
  saveWorking();
}

/* Widget Change Triggers */
void EditorParty::changedClassify(QString index)
{
  party_curr.setPartyType(Helpers::partyTypeFromStr(index.toStdString()));
  updateItemList();
  updatePersonList();
}

/* Widget Change Triggers */
void EditorParty::changedName(QString name)
{
  setName(name);
}

/* List Index Widget Changes */
void EditorParty::listItemAllChanged(int index)
{
  if(index >= 0)
    btn_item_add->setEnabled(true);
  else
    btn_item_add->setDisabled(true);
}

/* List Index Widget Changes */
void EditorParty::listItemAllDClicked(QListWidgetItem*)
{
  btnItemAdd();
}

/* List Index Widget Changes */
void EditorParty::listItemEdited(QListWidgetItem*)
{
  if(list_items_used->currentRow() >= 0)
  {
    QString list_item = list_items_used->currentItem()->text();
    QStringList list_split = list_item.split(':');
    if(list_split.size() == 2)
    {
      /* Try to find item first */
      int item_id = list_split.front().toInt();
      bool exists = item_set.contains(item_id);
      if(exists)
      {
        EditorItem* ref = getItem(item_id);
        int max = Inventory::kMIN_EACH_ITEM - item_set.value(item_id);
        if(ref->getMass() > 0)
        {
          int item_fit = (int)(getInvMassRemain() * 1.0 / ref->getMass());
          if(item_fit < max)
            max = item_fit;
        }
        max += item_set.value(item_id);

        /* Get new count */
        bool ok;
        int count = QInputDialog::getInt(this, "Count?", "Item Count:",
                                         item_set.value(item_id), 1,
                                         max, 1, &ok);
        if(ok)
        {
          item_set.insert(item_id, count);
          updateItemList();
        }
      }
    }
  }
}

/* List Index Widget Changes */
void EditorParty::listItemUsedChanged(int index)
{
  if(index >= 0)
    btn_item_rem->setEnabled(true);
  else
    btn_item_rem->setDisabled(true);
}

/* List Index Widget Changes */
void EditorParty::listPersonAllChanged(int index)
{
  if(index >= 0)
    btn_person_add->setEnabled(true);
  else
    btn_person_add->setDisabled(true);
}

/* List Index Widget Changes */
void EditorParty::listPersonAllDClicked(QListWidgetItem*)
{
  btnPersonAdd();
}

/* List Index Widget Changes */
void EditorParty::listPersonEdited(QListWidgetItem*)
{
  int index = list_persons_used->currentRow();

  if(index >= 0 && index < person_set.size())
  {
    /* Get the new level */
    bool ok;
    int lvl = QInputDialog::getInt(this, "Level?", "Person Level:",
                                   person_set[index].second, 1,
                                   EditorEnumDb::kMAX_PERSON_LVL, 1, &ok);
    if(ok)
    {
      person_set[index].second = lvl;
      updatePersonList();
    }
  }
}

/* List Index Widget Changes */
void EditorParty::listPersonUsedChanged(int index)
{
  if(index >= 0)
    btn_person_rem->setEnabled(true);
  else
    btn_person_rem->setDisabled(true);
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
  /* Parse elements */
  if(data.getElement(index) == "name")
  {
    name_base = QString::fromStdString(data.getDataString());
  }
  else if(data.getElement(index) == "person")
  {
    QString set = QString::fromStdString(data.getDataString());
    QStringList split_set = set.split(',');
    if(split_set.size() == 2)
    {
      int person_id = split_set.front().toInt();
      int person_lvl = split_set.back().toInt();
      if(person_lvl > 0)
        person_set_base.push_back(QPair<int,int>(person_id, person_lvl));
    }
  }
  else if(data.getElement(index) == "inventory" &&
          data.getElement(index + 1) == "item")
  {
    QString set = QString::fromStdString(data.getDataString());
    QStringList split_set = set.split(',');
    if(split_set.size() == 2)
    {
      int item_id = split_set.front().toInt();
      int item_count = split_set.back().toInt();
      if(item_count > 0)
        item_set_base.insert(item_id, item_count);
    }
  }
  else
  {
    party_base.loadData(data, index, NULL,
                        EditorHelpers::getProjectDir().toStdString() + "/");
  }
}

/* Resets the working set trigger */
void EditorParty::resetWorking()
{
  /* Reset the data */
  party_curr = party_base;
  item_set = item_set_base;
  person_set = person_set_base;
  name_curr = name_base;

  /* Properly load the information */
  setName(name_curr);
  loadWorkingInfo();
}

/* Saves the object data */
void EditorParty::save(FileHandler* fh, bool game_only)
{
  if(fh != NULL)
  {
    /* Wrapper */
    fh->writeXmlElement("party", "id", getID());

    /* Name */
    if(!game_only)
      fh->writeXmlData("name", name_base.toStdString());

    /* Type */
    QString type = QString::fromStdString(Helpers::partyTypeToStr(
                                                    party_base.getPartyType()));
    type = type.toLower();
    fh->writeXmlData("type", type.toStdString());

    /* Persons */
    for(int i = 0; i < person_set_base.size(); i++)
    {
      QString set = QString::number(person_set_base[i].first) + "," +
                    QString::number(person_set_base[i].second);
      fh->writeXmlData("person", set.toStdString());
    }

    /* Inventory */
    if(item_set_base.size() > 0)
    {
      fh->writeXmlElement("inventory");

      /* Go through all items */
      QMapIterator<int,int> i(item_set_base);
      while(i.hasNext())
      {
        i.next();
        QString set = QString::number(i.key()) + "," +
                      QString::number(i.value());
        fh->writeXmlData("item", set.toStdString());
      }

      fh->writeXmlElementEnd();
    }
  }
}

/* Saves the working set trigger */
void EditorParty::saveWorking()
{
  /* Save the data */
  party_base = party_curr;
  item_set_base = item_set;
  person_set_base = person_set;
  name_base = name_curr;
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
  items_all = items;
  if(update_working)
    loadWorkingInfo();
}

/* Update Calls for data */
void EditorParty::updatePersons(QVector<EditorPerson*> persons,
                                bool update_working)
{
  persons_all = persons;
  if(update_working)
    loadWorkingInfo();
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
