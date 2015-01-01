#include "Database/EditorSkill.h"
#include <QDebug>

EditorSkill::EditorSkill(QWidget *parent) : QWidget(parent)
{
  top_horizontal = new QHBoxLayout();
  bottom_horizontal = new QHBoxLayout();

  main_layout = new QVBoxLayout();

  animation_frame = new QImage(":/Icons/Resources/fbs_logo_transparent.png");
  QImage temp = animation_frame->scaled(128,128);
  animation_frame = &temp;
  animation_label = new QLabel(this);
  animation_label->setPixmap(QPixmap::fromImage(*animation_frame));
  animation_label->setFixedSize(128,128);
  top_horizontal->addWidget(animation_label,Qt::AlignTop);

  QVBoxLayout* combo_layout = new QVBoxLayout();
  QFormLayout* form = new QFormLayout();
  name_label = new QLabel("Skill Name",this);
  name_edit = new QLineEdit(this);
  form->addRow(name_label,name_edit);

  message_label = new QLabel("Hit Message",this);
  message_edit = new QTextEdit(this);
  form->addRow(message_label,message_edit);

  combo_layout->addLayout(form);

  QHBoxLayout* combos = new QHBoxLayout();
  QHBoxLayout* combo_labels = new QHBoxLayout();
  QStringList types;
  types << "Physical" << "Fire" << "Ice" << "Forest" << "Electric" << "Digital"
        << "Nihil" << "None ";
  QStringList scope;
  scope << "User" << "One Target" << "One Enemy" << "Two Enemies" <<
           "All Enemies" << "One Ally" << "One Ally (Not User)" <<
           "Two Allies" << "All Allies" << "One Fallen Ally" <<
           "All Fallen Allies" << "One Party" << "All Targets" << "Not User" <<
           "All Targets (Not User)" << "No Scope";

  QLabel *primary_label = new QLabel("Primary Element",this);
  QLabel *secondary_label = new QLabel("Secondary Element",this);
  QLabel *scope_label = new QLabel("Skill Scope",this);
  combo_labels->addWidget(primary_label);
  combo_labels->addWidget(secondary_label);
  combo_labels->addWidget(scope_label);
  primary_flag = new QComboBox(this);
  primary_flag->addItems(types);
  combos->addWidget(primary_flag);
  secondary_flag = new QComboBox(this);
  secondary_flag->addItems(types);
  combos->addWidget(secondary_flag);
  scope_flag = new QComboBox(this);
  scope_flag->addItems(scope);
  combos->addWidget(scope_flag);
  combo_layout->addLayout(combo_labels);
  combo_layout->addLayout(combos);
  top_horizontal->addLayout(combo_layout);

  thumbnail_frame = new QImage(":/Icons/Resources/fbs_logo_transparent.png");
  QImage temp2 = thumbnail_frame->scaled(64,64);
  thumbnail_frame = &temp2;
  thumbnail_label = new QLabel(this);
  thumbnail_label->setPixmap(QPixmap::fromImage(*thumbnail_frame));
  thumbnail_label->setFixedSize(64,64);
  top_horizontal->addWidget(thumbnail_label);

  skill_flags = new QGroupBox("Skill Flags",this);
  QVBoxLayout* skill_flags_layout = new QVBoxLayout();
  skill_offensive = new QCheckBox("Offensive",this);
  skill_flags_layout->addWidget(skill_offensive);
  skill_defensive = new QCheckBox("Defensive",this);
  skill_flags_layout->addWidget(skill_defensive);
  skill_neutral = new QCheckBox("Neutral",this);
  skill_flags_layout->addWidget(skill_neutral);
  skill_altering = new QCheckBox("Altering",this);
  skill_flags_layout->addWidget(skill_altering);
  skill_damaging = new QCheckBox("Damaging",this);
  skill_flags_layout->addWidget(skill_damaging);
  skill_healing = new QCheckBox("Healing",this);
  skill_flags_layout->addWidget(skill_healing);
  skill_inflicting = new QCheckBox("Inflicting",this);
  skill_flags_layout->addWidget(skill_inflicting);
  skill_relieving = new QCheckBox("Relieveing",this);
  skill_flags_layout->addWidget(skill_relieving);
  skill_reviving = new QCheckBox("Reviving",this);
  skill_flags_layout->addWidget(skill_reviving);
  skill_assigning = new QCheckBox("Assigning",this);
  skill_flags_layout->addWidget(skill_assigning);
  skill_valid = new QCheckBox("Valid",this);
  skill_flags_layout->addWidget(skill_valid);

  skill_flags_layout->addStretch(1);
  skill_flags->setLayout(skill_flags_layout);

  top_horizontal->addWidget(skill_flags);

  QFormLayout* info_layout = new QFormLayout();
  description_label = new QLabel("Written Description",this);
  description = new QTextEdit(this);
  info_layout->addRow(description_label,description);

  cost_label = new QLabel("QD Cost",this);
  cost_edit = new QLineEdit(this);
  info_layout->addRow(cost_label,cost_edit);

  chance_label = new QLabel("Chance",this);
  chance_edit = new QLineEdit(this);
  info_layout->addRow(chance_label,chance_edit);

  cooldown_label = new QLabel("Charge Time",this);
  cooldown_edit = new QLineEdit(this);
  info_layout->addRow(cooldown_label,cooldown_edit);

  value_label = new QLabel("Skill Value (For AI)",this);
  value_edit = new QLineEdit(this);
  info_layout->addRow(value_label,value_edit);

  info_layout->addWidget(new QWidget(this));
  bottom_horizontal->addLayout(info_layout);

  QGridLayout* skill_layout = new QGridLayout();
  total_actions = new QListWidget();
  skill_layout->addWidget(total_actions,0,0);
  skill_actions = new QListWidget();
  skill_layout->addWidget(skill_actions,0,1);
  add_action_to_skill = new QPushButton("Add Action",this);
  skill_layout->addWidget(add_action_to_skill,1,0);
  remove_action_from_skill = new QPushButton("Remove Action",this);
  skill_layout->addWidget(remove_action_from_skill,1,1);

  total_action_list = new QVector<QPair<QString,EditorAction*>* >(0);
  skill_action_list = new QVector<QPair<QString,EditorAction*>* >(0);

  layout = new QHBoxLayout(this);
  main_layout->addLayout(top_horizontal);
  main_layout->addLayout(bottom_horizontal);
  main_layout->addStretch(1);
  layout->addLayout(main_layout);
  layout->addLayout(skill_layout);

  action_selection = -1;
  running_action_id = 0;
  connect(total_actions,SIGNAL(currentRowChanged(int)),
          this,SLOT(changeIndex(int)));
  connect(add_action_to_skill,SIGNAL(clicked()),this,SLOT(addAction()));

  setBaseSkill(Skill());
}

EditorSkill::~EditorSkill()
{
}

void EditorSkill::setBaseSkill(Skill s)
{
  base = s;
  setWorkingSkill(base);
  loadWorkingInfo();
}

void EditorSkill::setWorkingSkill(Skill s)
{
  working = base;
}

void EditorSkill::loadWorkingInfo()
{
  name_edit->setText(QString::fromStdString(working.getName()));
  message_edit->setText((QString::fromStdString(working.getMessage())));
  description->setText(QString::fromStdString(working.getDescription()));
  cost_edit->setText(QString::number(working.getCost()));
  chance_edit->setText(QString::number(working.getChance()));
  cooldown_edit->setText(QString::number(working.getCooldown()));
  value_edit->setText(QString::number(working.getValue()));

  skill_offensive->setChecked(working.getFlag(SkillFlags::OFFENSIVE));
  skill_defensive->setChecked(working.getFlag(SkillFlags::DEFENSIVE));
  skill_neutral->setChecked(working.getFlag(SkillFlags::NEUTRAL));
  skill_altering->setChecked(working.getFlag(SkillFlags::ALTERING));
  skill_damaging->setChecked(working.getFlag(SkillFlags::DAMAGING));
  skill_healing->setChecked(working.getFlag(SkillFlags::HEALING));
  skill_inflicting->setChecked(working.getFlag(SkillFlags::INFLICTING));
  skill_relieving->setChecked(working.getFlag(SkillFlags::RELIEVING));
  skill_reviving->setChecked(working.getFlag(SkillFlags::REVIVING));
  skill_assigning->setChecked(working.getFlag(SkillFlags::ASSIGNING));
  skill_valid->setChecked(working.getFlag(SkillFlags::VALID));



}

EditorSkill* EditorSkill::clone()
{
  return this;
}

void EditorSkill::setTotalActionsList
(QVector<QPair<QString,EditorAction *>* >* list)
{
  total_action_list = list;
  total_actions->clear();
  QStringList action_list;
  for(int i=0; i<total_action_list->size(); i++)
    action_list << total_action_list->at(i)->first;
  total_actions->addItems(action_list);
}

void EditorSkill::changeIndex(int x)
{
  action_selection = x;
  qDebug()<<action_selection;
}

void EditorSkill::addAction()
{
  if(action_selection != -1)
  {
    QString new_string = total_actions->item(action_selection)->text().
        split(" : ").at(1);
    new_string.prepend(" : ");
    new_string.prepend(QString::number(running_action_id++));
    if(running_action_id < 10)
      new_string.prepend("0");
    skill_actions->addItem(new_string);

    skill_action_list->push_back(new QPair<QString,EditorAction*>
                  (new_string,
                   new EditorAction()));
    skill_action_list->last()->second->setBaseAction(
       Action(total_action_list->at(action_selection)->second->
              outputString().toStdString()));
    skill_action_list->last()->second->setNameAndID(new_string);
    qDebug() << skill_action_list->last()->second->outputString();
  }
}
