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
  QVBoxLayout* main_layout = new QVBoxLayout(this);
  QGridLayout* lists_layout = new QGridLayout();
  QHBoxLayout* save_reset_layout = new QHBoxLayout();

  /* Setup data */
  available_skill_index = -1;
  working_skill_index = -1;
  running_skill_id = 0;

  /* Setup lists */
  available_set_list = new QListWidget(this);
  working_set_list = new QListWidget(this);

  /* Setup labels */
  QHBoxLayout* level_layout = new QHBoxLayout();
  level_label = new QLabel("Level Active On",this);
  level_layout->addWidget(level_label);
  level_edit = new QLineEdit(this);
  level_layout->addWidget(level_edit);

  /* Setup Buttons */
  save = new QPushButton("Save Skill Set",this);
  connect(save,SIGNAL(clicked()),this,SLOT(getEditedSkillset()));
  reset = new QPushButton("Reset Skill Set",this);
  connect(reset,SIGNAL(clicked()),this,SLOT(resetWorkingSkillset()));

  add = new QPushButton("Add To Skillset",this);
  connect(add,SIGNAL(clicked()),this,SLOT(addSkill()));
  remove = new QPushButton("Remove From Skill Set",this);
  connect(remove,SIGNAL(clicked()),this,SLOT(removeSkill()));

  save_reset_layout->addWidget(save);
  save_reset_layout->addWidget(reset);

  lists_layout->addLayout(level_layout,0,1);
  lists_layout->addWidget(available_set_list,1,0);
  lists_layout->addWidget(working_set_list,1,1);
  lists_layout->addWidget(add,2,0);
  lists_layout->addWidget(remove,2,1);

  main_layout->addLayout(lists_layout);
  main_layout->addSpacing(32);
  main_layout->addLayout(save_reset_layout);

  /* Connections */
  connect(working_set_list,SIGNAL(currentRowChanged(int)),
          this,SLOT(setLevelBox(int)));

  connect(level_edit,SIGNAL(textEdited(QString)),this,SLOT(setLevel(QString)));

  connect(available_set_list,SIGNAL(currentRowChanged(int)),
          this,SLOT(changeIndex(int)));
  deletion = false;
  setBaseSkillset(SkillSet());
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
  deletion = source.deletion;

  available_set_list = source.available_set_list;
  working_set_list = source.working_set_list;
  available_skill_index = source.available_skill_index;
  working_skill_index = source.working_skill_index;
  running_skill_id = source.running_skill_id;

  available_set = source.available_set;
  working_set = source.working_set;
  base_set = source.base_set;
  previous_set = source.previous_set;
  working_level_set = source.working_level_set;
  previous_level_set = source.previous_level_set;
  base_level_set = source.base_level_set;

  setBaseSkillset(source.base);
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

void EditorSkillset::setNameAndID(QString str)
{
  id = (str.split(" : ").at(0).toInt());
  name = str.split(" : ").at(1);
  //name_edit->setText(name);
  setWorkingSkillset(base);
}

void EditorSkillset::setTotalSkillsList
(QVector<QPair<QString,EditorSkill *>* > list)
{
  available_set = list;
  available_set_list->clear();
  QStringList skill_list;
  for(int i=0; i<available_set.size(); i++)
    skill_list << available_set.at(i)->first;
  available_set_list->addItems(skill_list);
}

void EditorSkillset::setLevelBox(int row)
{
  if(row >= 0)
  {
    working_skill_index = row;
    if(deletion)
    {
      working_skill_index -=1;
      deletion = false;
    }
    level_edit->setText(QString::number(
                                    working_level_set.at(working_skill_index)));
  }

}

void EditorSkillset::setLevel(QString s)
{
  if(working_skill_index >= 0)
  {
    working_level_set.replace(working_skill_index,s.toInt());
  }
}

void EditorSkillset::setWorkingSkillset(SkillSet s)
{
  working = base;
}

void EditorSkillset::setBaseSkillset(SkillSet s)
{
  base = s;
  setWorkingSkillset(base);
  loadWorkingInfo();
}

void EditorSkillset::loadWorkingInfo()
{
}

void EditorSkillset::addSkill()
{
  if(available_skill_index >= 0)
  {
    deletion = false;
    QListWidgetItem* current = available_set_list->item(available_skill_index);
    working_set_list->addItem(current->text());

    working_set.push_back(new QPair<QString,EditorSkill*>
                  (available_set_list->item(available_skill_index)->text(),
                   new EditorSkill()));
    working_level_set.push_back(0);
  }
}

void EditorSkillset::removeSkill()
{
  if(working_skill_index >= 0 &&
     working_set.size() > 0 &&
     working_level_set.size() > 0)
  {
    deletion = true;
    working_level_set.remove(working_skill_index);
    working_set.remove(working_skill_index);
    working_set_list->takeItem(working_skill_index);
    if(working_skill_index != 0)
      working_skill_index--;
  }
}

void EditorSkillset::changeIndex(int i)
{
  available_skill_index = i;
}

SkillSet EditorSkillset::getEditedSkillset()
{
  previous_set = working_set;
  previous_level_set = working_level_set;
  base_level_set = working_level_set;
  base_set = working_set;

  working.clear();
  for(int i=0; i<working_set.size(); i++)
  {
    //TODO : Add actual skill pointers insetad of blanks
    working.addSkill(new Skill(),working_level_set.at(i));
  }
  base = working;
  return base;
}

void EditorSkillset::resetWorkingSkillset()
{
  deletion = false;
  working = base;
  working_set = previous_set;
  working_level_set = previous_level_set;
  qDebug()<<working_set.size()<<","<<working_level_set.size();
  working_set_list->clear();
  for(int i=0; i<working_set.size(); i++)
  {
    working_set_list->addItem(working_set.at(i)->first);
    working_set_list->setCurrentRow(i);
  }
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
