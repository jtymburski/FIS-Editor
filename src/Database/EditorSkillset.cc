/*******************************************************************************
 * Class Name: EditorSkillset
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Skillset which contains a set of Editor Skills.
 ******************************************************************************/
#include "Database/EditorSkillset.h"
#include <QDebug>

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

/* Constructor function with id and name */
EditorSkillset::EditorSkillset(int id, QString name, QWidget* parent)
              : EditorSkillset(parent)
{
  setID(id);
  setName(name);
}

/* Copy constructor */
EditorSkillset::EditorSkillset(const EditorSkillset &source) : EditorSkillset()
{
  copySelf(source);
}

EditorSkillset::~EditorSkillset()
{
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/* Copy function, to be called by a copy or equal operator constructor */
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

/* Get skill pointer, based on ID */
EditorSkill* EditorSkillset::getID(int id)
{
  for(int i = 0; i < set_total.size(); i++)
    if(set_total[i]->getID() == id)
      return set_total[i];
  return NULL;
}

/* Loads working info into UI objects */
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

/* Get Skill string */
QString EditorSkillset::skillString(EditorSkill* skill, int lvl)
{
  QString info = "";

  /* Basic string */
  info += skill->getNameList();

  /* Level */
  if(lvl >= 0)
    info = "LVL " + QString::number(lvl) + " - " + info;

  return info;
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

void EditorSkillset::addSkill()
{
  if(list_available->currentRow() >= 0)
  {
    bool ok;
    int count = QInputDialog::getInt(this, "Level?",
                                      "Skill Level Requirement:", 1, 1, 128,
                                      1, &ok);
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

/* Name changed trigger */
void EditorSkillset::nameEdited(QString str)
{
  name = str;
  emit nameChange(str);
}

void EditorSkillset::removeSkill()
{
  if(list_used->currentRow() >= 0)
  {
    /* Remove index */
    set_working.remove(list_used->currentRow());

    /* Load info */
    loadWorkingInfo();
  }
}

void EditorSkillset::resetWorking()
{
  set_working = set_base;
  loadWorkingInfo();
}

void EditorSkillset::saveWorking()
{
  set_base = set_working;
  loadWorkingInfo();
}

void EditorSkillset::setNameAndID(QString str)
{
  id = (str.split(" : ").at(0).toInt());
  name = str.split(" : ").at(1);
  loadWorkingInfo();
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

EditorSkillset* EditorSkillset::clone()
{
  return this;
}

/* Returns the ID of the skillset */
int EditorSkillset::getID() const
{
  return id;
}

/* Returns the name of the skillset */
QString EditorSkillset::getName() const
{
  return name;
}

/* Returns the name of the skillset for listing */
QString EditorSkillset::getNameList()
{
  return EditorHelpers::getListString(getID(), getName());
}

/* Sets the ID of the skillset */
void EditorSkillset::setID(int id)
{
  this->id = id;
}

/* Sets the name of the skillset */
void EditorSkillset::setName(QString name)
{
  this->name = name;
  loadWorkingInfo();
  emit nameChange(name);
}

/* Update skills */
void EditorSkillset::updateSkills(QVector<EditorSkill*> skills)
{
  set_total = skills;
  loadWorkingInfo();
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/* The copy operator */
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
