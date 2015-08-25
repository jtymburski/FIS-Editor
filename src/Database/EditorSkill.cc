/*******************************************************************************
 * Class Name: EditorSkill
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Skill which contains the connections to edit a skill.
 ******************************************************************************/
#include "Database/EditorSkill.h"
#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

EditorSkill::EditorSkill(QWidget *parent) : QWidget(parent)
{
  /* Dialogs for animation and thumbs */
  dialog_anim = NULL;
  dialog_thumb = NULL;

  /* Make sure there's one frame in the thumbnail */
  if(sprite_thumb_base.frameCount() == 0)
    sprite_thumb_base.setPath(0, "");

  top_horizontal = new QHBoxLayout();
  bottom_horizontal = new QHBoxLayout();

  main_layout = new QVBoxLayout();

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

  QVBoxLayout* total_right = new QVBoxLayout();
  QGridLayout* skill_layout = new QGridLayout();
  total_actions = new QListWidget();
  skill_layout->addWidget(total_actions, 3, 0, 1, 2);
  skill_actions = new QListWidget();
  skill_layout->addWidget(skill_actions, 3, 2, 1, 2);
  add_action_to_skill = new QPushButton("Add Action",this);
  skill_layout->addWidget(add_action_to_skill, 4, 0, 1, 2);
  remove_action_from_skill = new QPushButton("Remove Action",this);
  skill_layout->addWidget(remove_action_from_skill, 4, 2, 1, 2);

  /* Animation Frame */
  QLabel* lbl_animation = new QLabel("Animation", this);
  skill_layout->addWidget(lbl_animation, 0, 0, 1, 2, Qt::AlignHCenter);
  lbl_anim_img = new QLabel(this);
  lbl_anim_img->setMinimumSize(200, 200);
  lbl_anim_img->setStyleSheet("border: 1px solid #b9b5b2");
  lbl_anim_img->setAlignment(Qt::AlignCenter);
  skill_layout->addWidget(lbl_anim_img, 1, 0, 2, 2, Qt::AlignHCenter);
  QPushButton* btn_anim_click = new QPushButton(this);
  btn_anim_click->setIcon(QIcon(":/images/icons/32_settings.png"));
  btn_anim_click->setIconSize(QSize(24,24));
  btn_anim_click->setMaximumSize(30, 30);
  connect(btn_anim_click, SIGNAL(clicked()), this, SLOT(buttonAnimEdit()));
  skill_layout->addWidget(btn_anim_click, 2, 0, 1, 2, Qt::AlignHCenter);

  /* Thumbnail Frame */
  QLabel* lbl_thumb = new QLabel("Thumbnail", this);
  skill_layout->addWidget(lbl_thumb, 0, 2, 1, 2, Qt::AlignHCenter);
  lbl_thumb_img = new QLabel(this);
  lbl_thumb_img->setMinimumSize(200, 200);
  lbl_thumb_img->setStyleSheet("border: 1px solid #b9b5b2");
  lbl_thumb_img->setAlignment(Qt::AlignCenter);
  skill_layout->addWidget(lbl_thumb_img, 1, 2, 2, 2, Qt::AlignHCenter);
  QPushButton* btn_thumb_click = new QPushButton(this);
  btn_thumb_click->setIcon(QIcon(":/images/icons/32_settings.png"));
  btn_thumb_click->setIconSize(QSize(24,24));
  btn_thumb_click->setMaximumSize(30, 30);
  connect(btn_thumb_click, SIGNAL(clicked()), this, SLOT(buttonThumbEdit()));
  skill_layout->addWidget(btn_thumb_click, 2, 2, 1, 2, Qt::AlignHCenter);

  QHBoxLayout* save_reset = new QHBoxLayout();
  save_skill = new QPushButton("Save Skill",this);
  save_reset->addWidget(save_skill);
  reset_skill = new QPushButton("Reset Skill",this);
  save_reset->addWidget(reset_skill);

  connect(save_skill,SIGNAL(clicked()),this,SLOT(getEditedSkill()));
  connect(reset_skill,SIGNAL(clicked()),this,SLOT(resetWorkingSkill()));
  total_right->addLayout(skill_layout);
  total_right->addSpacing(24);
  total_right->addLayout(save_reset);

  total_action_list = new QVector<QPair<QString,EditorAction*>* >(0);
  skill_action_list = new QVector<QPair<QString,EditorAction*>* >(0);

  layout = new QHBoxLayout(this);
  main_layout->addLayout(top_horizontal);
  main_layout->addLayout(bottom_horizontal);
  main_layout->addStretch(1);

  layout->addLayout(main_layout);
  layout->addLayout(total_right);

  action_selection = -1;
  skill_action_selection = -1;
  running_action_id = 0;
  connect(total_actions,SIGNAL(currentRowChanged(int)),
          this,SLOT(changeIndex(int)));

  connect(add_action_to_skill,SIGNAL(clicked()),this,SLOT(addAction()));
  connect(remove_action_from_skill,SIGNAL(clicked()),this,SLOT(removeAction()));

  connect(skill_actions,SIGNAL(currentRowChanged(int)),
          this,SLOT(changeSkillActionIndex(int)));

  setBaseSkill(Skill());
  getEditedSkill();
}

/* Constructor function with id and name */
EditorSkill::EditorSkill(int id, QString name, QWidget* parent)
           : EditorSkill(parent)
{
  setID(id);
  setName(name);
  getEditedSkill();
}

/* Copy constructor */
EditorSkill::EditorSkill(const EditorSkill &source) : EditorSkill()
{
  copySelf(source);
}

EditorSkill::~EditorSkill()
{
  resetWorkingSkill();
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/* Copy function, to be called by a copy or equal operator constructor */
void EditorSkill::copySelf(const EditorSkill &source)
{
  id = source.id;
  name = source.name;
  test_string = source.test_string;

  /* Action lists */
  total_action_list = source.total_action_list;
  skill_action_list = source.skill_action_list;
  previous_skill_action_list = source.previous_skill_action_list;
  running_action_id = source.running_action_id;

  /* Frame connections */
  sprite_anim_base = source.sprite_anim_base;
  sprite_thumb_base = source.sprite_thumb_base;

  setBaseSkill(source.base);
  loadWorkingInfo();
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

void EditorSkill::addAction()
{
  // TODO: PROPERLY IMPLEMENT
  qDebug() << "TODO: ADD ACTION";

//  if(action_selection >= 0)
//  {
//    QString new_string = total_actions->item(action_selection)->text().
//        split(" : ").at(1);
//    new_string.prepend(" : ");
//    new_string.prepend(QString::number(running_action_id++));
//    if(running_action_id < 10)
//      new_string.prepend("0");
//    skill_actions->addItem(new_string);

//    skill_action_list->push_back(new QPair<QString,EditorAction*>
//                  (new_string,
//                   new EditorAction()));
//    skill_action_list->last()->second->setBaseAction(
//       Action(total_action_list->at(action_selection)->second->
//              outputString().toStdString()));
//    skill_action_list->last()->second->setNameAndID(new_string);
//  }
}

/* Animation dialog edit trigger */
void EditorSkill::buttonAnimEdit(bool clean_only)
{
  /* Close and delete the dialog if button pressed */
  if(dialog_anim != NULL)
  {
    dialog_anim->hide();
    disconnect(dialog_anim, SIGNAL(ok()), this, SLOT(updateAnimation()));
    delete dialog_anim;
  }
  dialog_anim = NULL;

  if(!clean_only)
  {
    /* Create the new sprite dialog */
    dialog_anim = new SpriteDialog(this, &sprite_anim, "", 0, false,
                                   EditorEnumDb::SPRITE_ALL);
    connect(dialog_anim, SIGNAL(ok()), this, SLOT(updateAnimation()));
    dialog_anim->show();
  }
}

/* Thumbnail dialog edit trigger */
void EditorSkill::buttonThumbEdit(bool clean_only)
{
  /* Delete the dialog, if it exists */
  if(dialog_thumb != NULL)
  {
    dialog_thumb->hide();
    disconnect(dialog_thumb, SIGNAL(ok()), this, SLOT(updateThumb()));
    delete dialog_thumb;
  }
  dialog_thumb = NULL;

  /* Make sure there's one frame in the thumbnail */
  if(sprite_thumb.frameCount() == 0)
    sprite_thumb.setPath(0, "");

  if(!clean_only)
  {
    /* Create the new frame */
    dialog_thumb = new FrameDialog(this, &sprite_thumb);
    connect(dialog_thumb, SIGNAL(ok()), this, SLOT(updateThumb()));
    dialog_thumb->show();
  }
}

void EditorSkill::changeIndex(int x)
{
  action_selection = x;
}

void EditorSkill::changeSkillActionIndex(int x)
{
  skill_action_selection = x;
}

Skill EditorSkill::getEditedSkill()
{
  /* Clear pop-ups, if relevant */
  buttonAnimEdit(true);
  buttonThumbEdit(true);

  QString full_name = "";
  name = name_edit->text();
  if(working.getID() < 10)
    full_name.append("0");
  full_name.append(QString::number(working.getID()));
  full_name.append(" : ");
  full_name.append(name);
  emit nameChange(full_name);

  working.setName(name_edit->text().toStdString());
  working.setMessage(message_edit->toPlainText().toStdString());
  working.setDescription(description->toPlainText().toStdString());
  working.setCost(cost_edit->text().toInt());
  working.setChance(chance_edit->text().toFloat());
  working.setCooldown(cooldown_edit->text().toInt());
  working.setValue(value_edit->text().toInt());

  working.setFlag(SkillFlags::OFFENSIVE,skill_offensive->isChecked());
  working.setFlag(SkillFlags::DEFENSIVE,skill_defensive->isChecked());
  working.setFlag(SkillFlags::NEUTRAL,skill_neutral->isChecked());
  working.setFlag(SkillFlags::ALTERING,skill_altering->isChecked());
  working.setFlag(SkillFlags::DAMAGING,skill_damaging->isChecked());
  working.setFlag(SkillFlags::HEALING,skill_healing->isChecked());
  working.setFlag(SkillFlags::INFLICTING,skill_inflicting->isChecked());
  working.setFlag(SkillFlags::RELIEVING,skill_relieving->isChecked());
  working.setFlag(SkillFlags::REVIVING,skill_reviving->isChecked());
  working.setFlag(SkillFlags::ASSIGNING,skill_assigning->isChecked());
  working.setFlag(SkillFlags::VALID,skill_valid->isChecked());

  if(primary_flag->currentIndex() == 0)
    working.setPrimary(Element::PHYSICAL);
  else if(primary_flag->currentIndex() == 1)
    working.setPrimary(Element::FIRE);
  else if(primary_flag->currentIndex() == 2)
    working.setPrimary(Element::ICE);
  else if(primary_flag->currentIndex() == 3)
    working.setPrimary(Element::FOREST);
  else if(primary_flag->currentIndex() == 4)
    working.setPrimary(Element::ELECTRIC);
  else if(primary_flag->currentIndex() == 5)
    working.setPrimary(Element::DIGITAL);
  else if(primary_flag->currentIndex() == 6)
    working.setPrimary(Element::NIHIL);
  else if(primary_flag->currentIndex() == 7)
    working.setPrimary(Element::NONE);

  if(secondary_flag->currentIndex() == 0)
    working.setSecondary(Element::PHYSICAL);
  else if(secondary_flag->currentIndex() == 1)
    working.setSecondary(Element::FIRE);
  else if(secondary_flag->currentIndex() == 2)
    working.setSecondary(Element::ICE);
  else if(secondary_flag->currentIndex() == 3)
    working.setSecondary(Element::FOREST);
  else if(secondary_flag->currentIndex() == 4)
    working.setSecondary(Element::ELECTRIC);
  else if(secondary_flag->currentIndex() == 5)
    working.setSecondary(Element::DIGITAL);
  else if(secondary_flag->currentIndex() == 6)
    working.setSecondary(Element::NIHIL);
  else if(secondary_flag->currentIndex() == 7)
    working.setSecondary(Element::NONE);

  if(scope_flag->currentIndex() == 0)
    working.setScope(ActionScope::USER);
  else if(scope_flag->currentIndex() == 1)
    working.setScope(ActionScope::ONE_TARGET);
  else if(scope_flag->currentIndex() == 2)
    working.setScope(ActionScope::ONE_ENEMY);
  else if(scope_flag->currentIndex() == 3)
    working.setScope(ActionScope::TWO_ENEMIES);
  else if(scope_flag->currentIndex() == 4)
    working.setScope(ActionScope::ALL_ENEMIES);
  else if(scope_flag->currentIndex() == 5)
    working.setScope(ActionScope::ONE_ALLY);
  else if(scope_flag->currentIndex() == 6)
    working.setScope(ActionScope::ONE_ALLY_NOT_USER);
  else if(scope_flag->currentIndex() == 7)
    working.setScope(ActionScope::TWO_ALLIES);
  else if(scope_flag->currentIndex() == 8)
    working.setScope(ActionScope::ALL_ALLIES);
  else if(scope_flag->currentIndex() == 9)
    working.setScope(ActionScope::ONE_ALLY_KO);
  else if(scope_flag->currentIndex() == 10)
    working.setScope(ActionScope::ALL_ALLIES_KO);
  else if(scope_flag->currentIndex() == 11)
    working.setScope(ActionScope::ONE_PARTY);
  else if(scope_flag->currentIndex() == 12)
    working.setScope(ActionScope::ALL_TARGETS);
  else if(scope_flag->currentIndex() == 13)
    working.setScope(ActionScope::NOT_USER);
  else if(scope_flag->currentIndex() == 14)
    working.setScope(ActionScope::ALL_NOT_USER);
  else if(scope_flag->currentIndex() == 15)
    working.setScope(ActionScope::NO_SCOPE);

  previous_skill_action_list = *skill_action_list;

  base = working;
  sprite_anim_base = sprite_anim;
  sprite_thumb_base = sprite_thumb;

  return base;
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

  if(working.getPrimary() == Element::PHYSICAL)
    primary_flag->setCurrentIndex(0);
  else if(working.getPrimary() == Element::FIRE)
    primary_flag->setCurrentIndex(1);
  else if(working.getPrimary() == Element::ICE)
    primary_flag->setCurrentIndex(2);
  else if(working.getPrimary() == Element::FOREST)
    primary_flag->setCurrentIndex(3);
  else if(working.getPrimary() == Element::ELECTRIC)
    primary_flag->setCurrentIndex(4);
  else if(working.getPrimary() == Element::DIGITAL)
    primary_flag->setCurrentIndex(5);
  else if(working.getPrimary() == Element::NIHIL)
    primary_flag->setCurrentIndex(6);
  else if(working.getPrimary() == Element::NONE)
    primary_flag->setCurrentIndex(7);

  if(working.getSecondary() == Element::PHYSICAL)
    secondary_flag->setCurrentIndex(0);
  else if(working.getSecondary() == Element::FIRE)
    secondary_flag->setCurrentIndex(1);
  else if(working.getSecondary() == Element::ICE)
    secondary_flag->setCurrentIndex(2);
  else if(working.getSecondary() == Element::FOREST)
    secondary_flag->setCurrentIndex(3);
  else if(working.getSecondary() == Element::ELECTRIC)
    secondary_flag->setCurrentIndex(4);
  else if(working.getSecondary() == Element::DIGITAL)
    secondary_flag->setCurrentIndex(5);
  else if(working.getSecondary() == Element::NIHIL)
    secondary_flag->setCurrentIndex(6);
  else if(working.getSecondary() == Element::NONE)
    secondary_flag->setCurrentIndex(7);

  if(working.getScope() == ActionScope::USER)
    scope_flag->setCurrentIndex(0);
  else if(working.getScope() == ActionScope::ONE_TARGET)
    scope_flag->setCurrentIndex(1);
  else if(working.getScope() == ActionScope::ONE_ENEMY)
    scope_flag->setCurrentIndex(2);
  else if(working.getScope() == ActionScope::TWO_ENEMIES)
    scope_flag->setCurrentIndex(3);
  else if(working.getScope() == ActionScope::ALL_ENEMIES)
    scope_flag->setCurrentIndex(4);
  else if(working.getScope() == ActionScope::ONE_ALLY)
    scope_flag->setCurrentIndex(5);
  else if(working.getScope() == ActionScope::ONE_ALLY_NOT_USER)
    scope_flag->setCurrentIndex(6);
  else if(working.getScope() == ActionScope::TWO_ALLIES)
    scope_flag->setCurrentIndex(7);
  else if(working.getScope() == ActionScope::ALL_ALLIES)
    scope_flag->setCurrentIndex(8);
  else if(working.getScope() == ActionScope::ONE_ALLY_KO)
    scope_flag->setCurrentIndex(9);
  else if(working.getScope() == ActionScope::ALL_ALLIES_KO)
    scope_flag->setCurrentIndex(10);
  else if(working.getScope() == ActionScope::ONE_PARTY)
    scope_flag->setCurrentIndex(11);
  else if(working.getScope() == ActionScope::ALL_TARGETS)
    scope_flag->setCurrentIndex(12);
  else if(working.getScope() == ActionScope::NOT_USER)
    scope_flag->setCurrentIndex(13);
  else if(working.getScope() == ActionScope::ALL_NOT_USER)
    scope_flag->setCurrentIndex(14);
  else if(working.getScope() == ActionScope::NO_SCOPE)
    scope_flag->setCurrentIndex(15);

  /* Update sprite data */
  updateAnimation();
  updateThumb();
}

void EditorSkill::removeAction()
{
  // TODO: PROPERLY IMPLEMENT
  qDebug() << "TODO: REMOVE ACTION";

//  if(skill_action_selection >= 0 &&
//     skill_action_list->size() > 0)
//  {
//    skill_action_list->remove(skill_action_selection);
//    skill_actions->takeItem(skill_action_selection);
//    if(skill_action_selection != 0)
//      skill_action_selection--;
//  }
}

void EditorSkill::resetWorkingSkill()
{
  /* Clean-up pop-ups */
  buttonAnimEdit(true);
  buttonThumbEdit(true);

  /* Fix data */
  working = base;
  sprite_anim = sprite_anim_base;
  sprite_thumb = sprite_thumb_base;
  *skill_action_list = previous_skill_action_list;
  skill_actions->clear();
  for(int i=0; i<skill_action_list->size(); i++)
  {
    skill_actions->addItem(skill_action_list->at(i)->first);
    skill_actions->setCurrentRow(i);
  }

  /* Re-load working info */
  loadWorkingInfo();
}

void EditorSkill::setBaseSkill(Skill s)
{
  base = s;
  setWorkingSkill(base);
}

void EditorSkill::setNameAndID(QString str)
{
  base.setID(str.split(" : ").at(0).toInt());
  name = str.split(" : ").at(1);
  name_edit->setText(name);
  setWorkingSkill(base);
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

void EditorSkill::setWorkingSkill(Skill s)
{
  (void)s;
  working = base;
  sprite_anim = sprite_anim_base;
  sprite_thumb = sprite_thumb_base;

  loadWorkingInfo();
}

/* Update the thumb and animation frame for skill */
void EditorSkill::updateAnimation()
{
  QImage original = sprite_anim.getImage(0);
  if(original.width() > 195 || original.height() > 195)
  {
    QPixmap scaled_image = sprite_anim.getPixmap(0, 195, 195);
    lbl_anim_img->setPixmap(scaled_image);
  }
  else
  {
    QPixmap orig_image =
                 sprite_anim.getPixmap(0, original.width(), original.height());
    lbl_anim_img->setPixmap(orig_image);
  }
}

/* Update the thumb and animation frame for skill */
void EditorSkill::updateThumb()
{
  QImage original = sprite_thumb.getImage(0);
  if(original.width() > 195 || original.height() > 195)
  {
    QImage scaled_image = original.scaled(195, 195, Qt::KeepAspectRatio);
    lbl_thumb_img->setPixmap(QPixmap::fromImage(scaled_image));
  }
  else
  {
    lbl_thumb_img->setPixmap(QPixmap::fromImage(original));
  }
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Returns the ID of the skill */
int EditorSkill::getID() const
{
  return id;
}

/* Returns the name of the skill */
QString EditorSkill::getName() const
{
  return name;
}

/* Returns the name of the skill for listing */
QString EditorSkill::getNameList()
{
  return EditorHelpers::getListString(getID(), getName());
}

/* Sets the ID of the skill */
void EditorSkill::setID(int id)
{
  this->id = id;
}

/* Sets the name of the skill */
void EditorSkill::setName(QString name)
{
  this->name = name;
  name_edit->setText(name);
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/* The copy operator */
EditorSkill& EditorSkill::operator= (const EditorSkill &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
