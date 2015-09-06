/*******************************************************************************
 * Class Name: EditorSkillset
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Skillset which contains a set of Editor Skills.
 ******************************************************************************/
#include "Database/EditorSkillset.h"
#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Main constructor function. All parameters have null defaults.
 *
 * Inputs: QWidget* parent - the parent widget. Default to NULL
 */
EditorSkillset::EditorSkillset(QWidget *parent) : QWidget(parent)
{
  /* Setup layout */
  QGridLayout* main_layout = new QGridLayout(this);
  main_layout->setColumnStretch(0, 1);
  main_layout->setColumnStretch(3, 2);

  /* Name */
  edit_name = new QLineEdit(this);
  edit_name->setPlaceholderText("Name");
  edit_name->setAlignment(Qt::AlignHCenter);
  connect(edit_name, SIGNAL(textEdited(QString)),
          this, SLOT(nameEdited(QString)));
  main_layout->addWidget(edit_name, 1, 1, 1, 2);

  /* Available List */
  QLabel* lbl_avail = new QLabel("Available Skills", this);
  main_layout->addWidget(lbl_avail, 2, 1, Qt::AlignHCenter);
  list_available = new QListWidget(this);
  list_available->setMaximumWidth(350);
  main_layout->addWidget(list_available, 3, 1);

  /* Used set list */
  QLabel* lbl_used = new QLabel("Used Skills", this);
  main_layout->addWidget(lbl_used, 2, 2, Qt::AlignHCenter);
  list_used = new QListWidget(this);
  list_used->setMaximumWidth(350);
  main_layout->addWidget(list_used, 3, 2);

  /* Add Remove Buttons of skills*/
  btn_add = new QPushButton("Add To Skillset",this);
  connect(btn_add, SIGNAL(clicked()), this, SLOT(addSkill()));
  main_layout->addWidget(btn_add, 4, 1);
  btn_remove = new QPushButton("Remove From Skill Set",this);
  connect(btn_remove, SIGNAL(clicked()), this, SLOT(removeSkill()));
  main_layout->addWidget(btn_remove, 4, 2);

  /* 4th row spacer */
  main_layout->setRowMinimumHeight(5, 25);

  /* Setup Buttons */
  QPushButton* btn_reset = new QPushButton("Reset",this);
  connect(btn_reset, SIGNAL(clicked()), this, SLOT(resetWorking()));
  main_layout->addWidget(btn_reset, 6, 1);
  QPushButton* btn_save = new QPushButton("Save",this);
  connect(btn_save, SIGNAL(clicked()), this, SLOT(saveWorking()));
  main_layout->addWidget(btn_save, 6, 2);
}

/*
 * Description: Second constructor function, with ID and name parameters.
 *
 * Inputs: int id - the id of the skill set
 *         QString name - the name of the skill set
 *         QWidget* parent - the parent widget. Default to NULL
 */
EditorSkillset::EditorSkillset(int id, QString name, QWidget* parent)
              : EditorSkillset(parent)
{
  setID(id);
  setName(name);
}

/*
 * Description: Copy constructor. Calls the blank constructor and then copies
 *              the data from the source.
 *
 * Inputs: const EditorSkillset &source - the source object to copy
 */
EditorSkillset::EditorSkillset(const EditorSkillset &source) : EditorSkillset()
{
  copySelf(source);
}

/*
 * Description: Destructor function
 */
EditorSkillset::~EditorSkillset()
{
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: Copies all data from source editor object to this editor
 *              object.
 *
 * Inputs: EditorSkillset &source - the source to copy from
 * Output: none
 */
void EditorSkillset::copySelf(const EditorSkillset &source)
{
  id = source.id;
  name = source.name;

  /* List data */
  set_base = source.set_base;
  set_total = source.set_total;
  set_working = set_base;

  loadWorkingInfo();
}

/*
 * Description: Returns the stored Editor Skill by the given ID. Null if not
 *              found.
 *
 * Inputs: int id - the skill ID to search for
 * Output: EditorSkill* - the found skill pointer
 */
EditorSkill* EditorSkillset::getByID(int id)
{
  for(int i = 0; i < set_total.size(); i++)
    if(set_total[i]->getID() == id)
      return set_total[i];
  return NULL;
}

/*
 * Description: Loads all the UI elements with the contents from the working
 *              Skill Set information.
 *
 * Inputs: none
 * Output: none
 */
void EditorSkillset::loadWorkingInfo()
{
  /* Name */
  edit_name->setText(name);

  /* Get indexes and Clear existing sets */
  int index_avail = list_available->currentRow();
  int index_used = list_used->currentRow();
  list_available->clear();
  list_used->clear();

  /* Checking array stack */
  QVector<bool> working_used;
  for(int i = 0; i < set_working.size(); i++)
    working_used.push_back(false);

  /* Set split info */
  for(int i = 0; i < set_total.size(); i++)
  {
    bool found = false;

    /* Loop through and see if it exists in stored array */
    for(int j = 0; !found && j < set_working.size(); j++)
    {
      if(set_total[i]->getID() == set_working[j].first)
      {
        found = true;
        working_used[j] = true;
        list_used->addItem(skillString(set_total[i], set_working[j].second));
      }
    }

    /* If not found, put in other array */
    if(!found)
      list_available->addItem(skillString(set_total[i]));
  }

  /* Sort lists */
  list_available->sortItems();
  list_used->sortItems();

  /* Re-select list rows */
  if(index_avail >= 0 && list_available->count() > 0)
  {
    if(list_available->count() > index_avail)
      list_available->setCurrentRow(index_avail);
    else
      list_available->setCurrentRow(list_available->count() - 1);
  }
  if(index_used >= 0 && list_used->count() > 0)
  {
    if(list_used->count() > index_used)
      list_used->setCurrentRow(index_used);
    else
      list_used->setCurrentRow(list_used->count() - 1);
  }

  /* Check info to assert it is valid */
  for(int i = working_used.size() - 1; i >= 0; i--)
    if(!working_used[i])
      set_working.remove(i);
}

/*
 * Description: Returns the formatted skill string for skills stored within
 *              the set. If the lvl parameter is set, it will prepend a
 *              "LVL # - " to the front of the string for storing in the list.
 *
 * Inputs: EditorSkill* skill - the skill to list
 *         int lvl - the level before skill is usable. Default to -1 (invalid)
 * Output: QString - the resulting string for listing
 */
QString EditorSkillset::skillString(EditorSkill* skill, int lvl)
{
  QString info = "";

  /* Basic string */
  info += skill->getNameList();

  /* Level */
  if(lvl >= 0)
  {
    /* Get string */
    QString number = QString::number(lvl);
    if(lvl < 100)
      number = "0" + number;
    if(lvl < 10)
      number = "0" + number;

    /* Change string */
    info = "LVL " + number + " - " + info;
  }

  return info;
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

/*
 * Description: Adds a skill to the set. Triggered by push button. The added
 *              skill is based on the selected in the left hand list widget.
 *              Triggers a pop-up to force the user to select an activation
 *              level.
 *
 * Inputs: none
 * Output: none
 */
void EditorSkillset::addSkill()
{
  if(list_available->currentRow() >= 0)
  {
    bool ok;
    int count = QInputDialog::getInt(this, "Level?",
                                     "Skill Level Requirement:", 1, 1,
                                     EditorEnumDb::kMAX_PERSON_LVL, 1, &ok);
    if(ok)
    {
      /* Get the item and find skill */
      QString list_item = list_available->currentItem()->text();
      QStringList list_split = list_item.split(':');
      if(list_split.size() == 2)
      {
        /* Insert new skill */
        int id = list_split.front().toInt();
        set_working.push_back(QPair<int,int>(id, count));

        /* Re-load working info */
        loadWorkingInfo();
      }
    }
  }
}

/*
 * Description: Triggered when the name text field is edited. Updates the class
 *              string and the listed string in the parent widget.
 *
 * Inputs: QString str - the new name string
 * Output: none
 */
void EditorSkillset::nameEdited(QString str)
{
  name = str;
  emit nameChange(str);
}

/*
 * Description: Removes a skill to the set. Triggered by push button. The
 *              removed skill is based on the selected in the right hand list
 *              widget.
 *
 * Inputs: none
 * Output: none
 */
void EditorSkillset::removeSkill()
{
  if(list_used->currentRow() >= 0)
  {
    /* Get the item and find the skill */
    QString list_item = list_used->currentItem()->text();
    QStringList list_split_1 = list_item.split('-');
    if(list_split_1.size() == 2)
    {
      QStringList list_split_2 = list_split_1.back().split(':');
      if(list_split_2.size() == 2)
      {
        int id = list_split_2.front().toInt();

        /* Find it in the working set and delete */
        for(int i = set_working.size() - 1; i >= 0; i--)
          if(set_working[i].first == id)
            set_working.remove(i);

        /* Refresh working info */
        loadWorkingInfo();
      }
    }
  }
}

/*
 * Description: Resets the working information back to the base (last saved)
 *              data. This will also automatically update all UI widgets with
 *              the new data.
 *
 * Inputs: none
 * Output: none
 */
void EditorSkillset::resetWorking()
{
  set_working = set_base;
  loadWorkingInfo();
}

/*
 * Description: Saves the working information back to the base (last saved)
 *              data.
 *
 * Inputs: none
 * Output: none
 */
void EditorSkillset::saveWorking()
{
  set_base = set_working;
}

/*
 * Description: Sets the name and ID of this EditorSkillset class based on the
 *              colon delimited string from the parent list widget.
 *
 * Inputs: QString str - the comma delimited ID: Name
 * Output: none
 */
void EditorSkillset::setNameAndID(QString str)
{
  id = (str.split(" : ").at(0).toInt());
  name = str.split(" : ").at(1);
  loadWorkingInfo();
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Returns the ID of the skillset
 *
 * Inputs: none
 * Output: int - the ID
 */
int EditorSkillset::getID() const
{
  return id;
}

/*
 * Description: Returns the name of the skillset
 *
 * Inputs: none
 * Output: QString - the name
 */
QString EditorSkillset::getName() const
{
  return name;
}

/*
 * Description: Get the listing name string which includes the ID and name
 *
 * Inputs: none
 * Output: QString - the name list
 */
QString EditorSkillset::getNameList()
{
  return EditorHelpers::getListString(getID(), getName());
}

/*
 * Description: Loads the object data from the XML struct and offset index.
 *
 * Inputs: XmlData data - the XML data tree struct
 *         int index - the offset index into the struct
 * Output: none
 */
void EditorSkillset::load(XmlData data, int index)
{
  /* Parse elements */
  if(data.getElement(index) == "name")
  {
    name = QString::fromStdString(data.getDataString());
    emit nameChange(name);
  }
  else if(data.getElement(index) == "skill")
  {
    QString element = QString::fromStdString(data.getDataString());
    QStringList ele_split = element.split(',');
    if(ele_split.size() == 2)
    {
      /* Get ID and level and attempt to add */
      int id = ele_split.front().toInt();
      int lvl = ele_split.back().toInt();
      if(id >= 0 && lvl > 0)
        set_base.push_back(QPair<int, int>(id, lvl));
    }
  }
}

/*
 * Description: Saves the object data to the file handling pointer.
 *
 * Inputs: FileHandler* fh - the file handling pointer
 *         bool game_only - true if the data should include game only relevant
 * Output: none
 */
void EditorSkillset::save(FileHandler* fh, bool game_only)
{
  if(fh != NULL)
  {
    /* Base data */
    fh->writeXmlElement("skillset", "id", getID());
    if(!game_only)
      fh->writeXmlData("name", getName().toStdString());

    /* Skills in the set */
    for(int i = 0; i < set_base.size(); i++)
    {
      /* Create the set string */
      QString set = "";
      set.append(QString::number(set_base[i].first));
      set.append(',');
      set.append(QString::number(set_base[i].second));

      /* Write to file */
      fh->writeXmlData("skill", set.toStdString());
    }

    /* Close element */
    fh->writeXmlElementEnd();
  }
}

/*
 * Description: Sets the skillset ID in the class.
 *
 * Inputs: int id - the new id
 * Output: none
 */
void EditorSkillset::setID(int id)
{
  this->id = id;
}

/*
 * Description: Sets the name of the skillset.
 *
 * Inputs: QString name - the name text
 * Output: none
 */
void EditorSkillset::setName(QString name)
{
  this->name = name;
  loadWorkingInfo();
  emit nameChange(name);
}

/*
 * Description: Updates the list of available skills within the set. If this
 *              list removes skills previously used, they are removed from the
 *              working set (only). If new ones are added, they are placed in
 *              the available skills list.
 *
 * Inputs: QVector<EditorSkill*> skills - all available skills created
 * Output: none
 */
void EditorSkillset::updateSkills(QVector<EditorSkill*> skills)
{
  set_total = skills;
  loadWorkingInfo();
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/*
 * Description: Copy operator construction. This is called when the variable
 *              already exists and equal operator used with another
 *              EditorSkillset.
 *
 * Inputs: const EditorSkillset &source - the source class constructor
 * Output: EditorSkillset& - pointer to the copied class
 */
EditorSkillset& EditorSkillset::operator= (const EditorSkillset &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
