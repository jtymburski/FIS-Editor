/*******************************************************************************
 * Class Name: EditorAction
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Action to interface with game action.
 ******************************************************************************/
#include "Database/EditorAction.h"
#include <QDebug>

EditorAction::EditorAction(QWidget *parent) : QWidget(parent)
{
  /* Main Layout */
  main_layout = new QHBoxLayout(this);
  text_layout = new QGridLayout();

  name_label = new QLabel("Name",this);
  text_layout->addWidget(name_label,0,0,Qt::AlignLeft);
  value_label = new QLabel("Value",this);
  text_layout->addWidget(value_label,1,0,Qt::AlignLeft);
  chance_label = new QLabel("Chance",this);
  text_layout->addWidget(chance_label,2,0,Qt::AlignLeft);
  variance_label = new QLabel("Variance",this);
  text_layout->addWidget(variance_label,3,0,Qt::AlignLeft);
  minduration_label = new QLabel("Minimum Duration",this);
  text_layout->addWidget(minduration_label,4,0,Qt::AlignLeft);
  maxduration_label = new QLabel("Maximum Duration",this);
  text_layout->addWidget(maxduration_label,5,0,Qt::AlignLeft);

  name_edit = new QLineEdit(this);
  name_edit->setMaximumWidth(350);
  text_layout->addWidget(name_edit,0,1,1,2);
  value_edit = new QLineEdit(this);
  text_layout->addWidget(value_edit,1,1,Qt::AlignLeft);
  chance_edit = new QLineEdit(this);
  text_layout->addWidget(chance_edit,2,1,Qt::AlignLeft);
  variance_edit = new QLineEdit(this);
  text_layout->addWidget(variance_edit,3,1,Qt::AlignLeft);
  minduration_edit = new QLineEdit(this);
  text_layout->addWidget(minduration_edit,4,1,Qt::AlignLeft);
  maxduration_edit = new QLineEdit(this);
  text_layout->addWidget(maxduration_edit,5,1,Qt::AlignLeft);
  text_layout->setColumnStretch(2, 1);

  action_flags_base_pc = new QCheckBox("Value is Percent",this);
  action_flags_vari_pc = new QCheckBox("Variance is Percent",this);
  text_layout->addWidget(action_flags_base_pc,1,2,Qt::AlignLeft);
  text_layout->addWidget(action_flags_vari_pc,3,2,Qt::AlignLeft);
  QHBoxLayout* names_layout = new QHBoxLayout();
  names_layout->addLayout(text_layout);

  /* Left Layout */
  left_layout = new QVBoxLayout();
  left_layout->addLayout(names_layout);
  /* Right Layout */
  right_layout = new QVBoxLayout();
  /* Create main layout */
  main_layout->addLayout(left_layout);
  main_layout->addLayout(right_layout);

  /* Boxes Layout */
  boxes_layout = new QHBoxLayout();
  /* Groupbox for the action flags */
  action_flags_box = new QGroupBox("Action Flags",this);
  /* Action flags */
  action_flags_damage = new QRadioButton("Damage",this);
  connect(action_flags_damage,SIGNAL(clicked()),this,SLOT(updateLayouts()));
  action_flags_alter = new QRadioButton("Alter",this);
  connect(action_flags_alter,SIGNAL(clicked()),this,SLOT(updateLayouts()));
  action_flags_alterflip = new QRadioButton("Alter (Flipped)",this);
  connect(action_flags_alterflip,SIGNAL(clicked()),this,SLOT(updateLayouts()));
  action_flags_inflict = new QRadioButton("Inflict",this);
  connect(action_flags_inflict,SIGNAL(clicked()),this,SLOT(updateLayouts()));
  action_flags_relieve = new QRadioButton("Relieve",this);
  connect(action_flags_relieve,SIGNAL(clicked()),this,SLOT(updateLayouts()));
  action_flags_assign = new QRadioButton("Assign",this);
  connect(action_flags_assign,SIGNAL(clicked()),this,SLOT(updateLayouts()));
  action_flags_assignflip = new QRadioButton("Assign (Flipped)",this);
  connect(action_flags_assignflip,SIGNAL(clicked()),this,SLOT(updateLayouts()));
  action_flags_revive = new QRadioButton("Revive",this);
  connect(action_flags_revive,SIGNAL(clicked()),this,SLOT(updateLayouts()));

  /* Action flags layout */
  action_flags_layout = new QVBoxLayout();
  action_flags_layout->addWidget(action_flags_damage);
  action_flags_layout->addWidget(action_flags_alter);
  action_flags_layout->addWidget(action_flags_alterflip);
  action_flags_layout->addWidget(action_flags_inflict);
  action_flags_layout->addWidget(action_flags_relieve);
  action_flags_layout->addWidget(action_flags_assign);
  action_flags_layout->addWidget(action_flags_assignflip);
  action_flags_layout->addWidget(action_flags_revive);

  //action_flags_layout->addWidget(action_flags_valid);
  /* Add the layout to the groupbox */
  action_flags_box->setLayout(action_flags_layout);
  /* Add the action flags box to the boxes layout */
  boxes_layout->addWidget(action_flags_box);

  /* Groupbox for the ignore attack flags */
  ignoreatk_flags_box = new QGroupBox("Ignore Attack Flags",this);
  /* Ignore Attack flags */
  ignoreatk_flags_physical = new QCheckBox("Physical",this);
  ignoreatk_flags_thermal = new QCheckBox("Thermal",this);
  ignoreatk_flags_polar = new QCheckBox("Polar",this);
  ignoreatk_flags_primal = new QCheckBox("Primal",this);
  ignoreatk_flags_charged = new QCheckBox("Charged",this);
  ignoreatk_flags_nihil = new QCheckBox("Nihil",this);
  ignoreatk_flags_luck = new QCheckBox("Luck",this);
  ignoreatk_flags_cybernetic = new QCheckBox("Cybernetic",this);
  ignoreatk_flags_armor = new QCheckBox("Armor",this);
  /* Action flags layout */
  ignoreatk_flags_layout = new QVBoxLayout();
  ignoreatk_flags_layout->addWidget(ignoreatk_flags_physical);
  ignoreatk_flags_layout->addWidget(ignoreatk_flags_thermal);
  ignoreatk_flags_layout->addWidget(ignoreatk_flags_polar);
  ignoreatk_flags_layout->addWidget(ignoreatk_flags_primal);
  ignoreatk_flags_layout->addWidget(ignoreatk_flags_charged);
  ignoreatk_flags_layout->addWidget(ignoreatk_flags_nihil);
  ignoreatk_flags_layout->addWidget(ignoreatk_flags_luck);
  ignoreatk_flags_layout->addWidget(ignoreatk_flags_cybernetic);
  ignoreatk_flags_layout->addWidget(ignoreatk_flags_armor);
  /* Add the layout to the groupbox */
  ignoreatk_flags_box->setLayout(ignoreatk_flags_layout);
  /* Add the action flags box to the boxes layout */
  boxes_layout->addWidget(ignoreatk_flags_box);

  /* Groupbox for the ignore attack flags */
  ignoredef_flags_box = new QGroupBox("Ignore Defense Flags",this);
  /* Ignore Attack flags */
  ignoredef_flags_physical = new QCheckBox("Physical",this);
  ignoredef_flags_thermal = new QCheckBox("Thermal",this);
  ignoredef_flags_polar = new QCheckBox("Polar",this);
  ignoredef_flags_primal = new QCheckBox("Primal",this);
  ignoredef_flags_charged = new QCheckBox("Charged",this);
  ignoredef_flags_nihil = new QCheckBox("Nihil",this);
  ignoredef_flags_luck = new QCheckBox("Luck",this);
  ignoredef_flags_cybernetic = new QCheckBox("Cybernetic",this);
  ignoredef_flags_armor = new QCheckBox("Armor",this);
  /* Action flags layout */
  ignoredef_flags_layout = new QVBoxLayout();
  ignoredef_flags_layout->addWidget(ignoredef_flags_physical);
  ignoredef_flags_layout->addWidget(ignoredef_flags_thermal);
  ignoredef_flags_layout->addWidget(ignoredef_flags_polar);
  ignoredef_flags_layout->addWidget(ignoredef_flags_primal);
  ignoredef_flags_layout->addWidget(ignoredef_flags_charged);
  ignoredef_flags_layout->addWidget(ignoredef_flags_nihil);
  ignoredef_flags_layout->addWidget(ignoredef_flags_luck);
  ignoredef_flags_layout->addWidget(ignoredef_flags_cybernetic);
  ignoredef_flags_layout->addWidget(ignoredef_flags_armor);
  /* Add the layout to the groupbox */
  ignoredef_flags_box->setLayout(ignoredef_flags_layout);
  /* Add the action flags box to the boxes layout */
  boxes_layout->addWidget(ignoredef_flags_box);


  /* Boxes Layout */
  radio_layout = new QHBoxLayout();
  /* Groupbox for the ailment flags */
  ailment_box = new QGroupBox("Ailment Infliction",this);
  /* user flags */
  ailment_none = new QRadioButton("None",this);
  ailment_poison = new QRadioButton("Poison",this);
  ailment_burn = new QRadioButton("Burn",this);
  ailment_scald = new QRadioButton("Scald",this);
  ailment_char = new QRadioButton("Char",this);
  ailment_berserk = new QRadioButton("Berserk",this);
  ailment_confuse = new QRadioButton("Confuse",this);
  ailment_silence = new QRadioButton("Silence",this);
  ailment_bubbify = new QRadioButton("Bubbify",this);
  ailment_deathtimer = new QRadioButton("Death Timer",this);
  ailment_paralysis = new QRadioButton("Paralysis",this);
  ailment_blindness = new QRadioButton("Blindness",this);
  ailment_dreadstruck = new QRadioButton("Dreadstruck",this);
  ailment_dreamsnare = new QRadioButton("Dreamsnare",this);
  ailment_hellbound = new QRadioButton("Hellbound",this);
  ailment_bond = new QRadioButton("Bond",this);
  ailment_bonded = new QRadioButton("Bonded",this);
  ailment_allatkbuff = new QRadioButton("All Attack Buff",this);
  ailment_alldefbuff = new QRadioButton("All Defense Buff",this);
  ailment_physicalbuff = new QRadioButton("Physical Buff",this);
  ailment_thermalbuff = new QRadioButton("Thermal Buff",this);
  ailment_polarbuff = new QRadioButton("Polar Buff",this);
  ailment_primalbuff = new QRadioButton("Primal Buff",this);
  ailment_chargedbuff = new QRadioButton("Charged Buff",this);
  ailment_cyberneticbuff = new QRadioButton("Cybernetic Buff",this);
  ailment_nihilbuff = new QRadioButton("Nihil Buff",this);
  ailment_limbertudebuff = new QRadioButton("Limbertude Buff",this);
  ailment_unbearabilitybuff = new QRadioButton("Unbearability Buff",this);
  ailment_vitalitybuff = new QRadioButton("Vitality Buff",this);
  ailment_qdbuff = new QRadioButton("QD Buff",this);
  ailment_rootbound = new QRadioButton("Rootbound",this);
  ailment_doublecast = new QRadioButton("Doublecast",this);
  ailment_triplecast = new QRadioButton("Triplecast",this);
  ailment_halfcost = new QRadioButton("Half Cost",this);
  ailment_reflect = new QRadioButton("Reflect",this);
  ailment_hibernation = new QRadioButton("Hibernation",this);
  ailment_curse = new QRadioButton("Curse",this);
  ailment_metatether = new QRadioButton("Meta-Tether",this);
  ailment_stubulate = new QRadioButton("Stubulate",this);
  ailment_modulate = new QRadioButton("Modulate",this);
  ailment_invalid = new QRadioButton("Invalid",this);
  /* Ailment layout */
  ailment_layout = new QVBoxLayout();
  ailment_layout2 = new QVBoxLayout();
  total_ailment_layout = new QHBoxLayout();
  ailment_layout->addWidget(ailment_none);
  ailment_layout->addWidget(ailment_poison);
  ailment_layout->addWidget(ailment_burn);
  ailment_layout->addWidget(ailment_scald);
  ailment_layout->addWidget(ailment_char);
  ailment_layout->addWidget(ailment_berserk);
  ailment_layout->addWidget(ailment_confuse);
  ailment_layout->addWidget(ailment_silence);
  ailment_layout->addWidget(ailment_bubbify);
  ailment_layout->addWidget(ailment_deathtimer);
  ailment_layout->addWidget(ailment_paralysis);
  ailment_layout->addWidget(ailment_blindness);
  ailment_layout->addWidget(ailment_dreadstruck);
  ailment_layout->addWidget(ailment_dreamsnare);
  ailment_layout->addWidget(ailment_hellbound);
  ailment_layout->addWidget(ailment_bond);
  ailment_layout->addWidget(ailment_bonded);
  ailment_layout->addWidget(ailment_allatkbuff);
  ailment_layout->addWidget(ailment_alldefbuff);
  ailment_layout->addWidget(ailment_physicalbuff);
  ailment_layout2->addWidget(ailment_thermalbuff);
  ailment_layout2->addWidget(ailment_polarbuff);
  ailment_layout2->addWidget(ailment_primalbuff);
  ailment_layout2->addWidget(ailment_chargedbuff);
  ailment_layout2->addWidget(ailment_cyberneticbuff);
  ailment_layout2->addWidget(ailment_nihilbuff);
  ailment_layout2->addWidget(ailment_limbertudebuff);
  ailment_layout2->addWidget(ailment_unbearabilitybuff);
  ailment_layout2->addWidget(ailment_vitalitybuff);
  ailment_layout2->addWidget(ailment_qdbuff);
  ailment_layout2->addWidget(ailment_rootbound);
  ailment_layout2->addWidget(ailment_doublecast);
  ailment_layout2->addWidget(ailment_triplecast);
  ailment_layout2->addWidget(ailment_halfcost);
  ailment_layout2->addWidget(ailment_reflect);
  ailment_layout2->addWidget(ailment_hibernation);
  ailment_layout2->addWidget(ailment_curse);
  ailment_layout2->addWidget(ailment_metatether);
  ailment_layout2->addWidget(ailment_stubulate);
  ailment_layout2->addWidget(ailment_modulate);
  ailment_layout2->addWidget(ailment_invalid);
  /* Add the layout to the groupbox */
  total_ailment_layout->addLayout(ailment_layout);
  total_ailment_layout->addLayout(ailment_layout2);
  ailment_box->setLayout(total_ailment_layout);
  /* Add the user flags box to the left layout */
  left_layout->addWidget(ailment_box);

  /* Groupbox for the user attribute flags */
  user_flags_box = new QGroupBox("User Attribute Flags",this);
  /* User flags */
  user_flags_VITA = new QRadioButton("Vitality",this);
  user_flags_QTDR = new QRadioButton("Quantum Drive (QD)",this);
  user_flags_PHAG = new QRadioButton("Physical Aggression",this);
  user_flags_PHFD = new QRadioButton("Physical Fortitude",this);
  user_flags_THAG = new QRadioButton("Thermal Aggression",this);
  user_flags_THFD = new QRadioButton("Thermal Fortitude",this);
  user_flags_PRAG = new QRadioButton("Primal Aggression",this);
  user_flags_PRFD = new QRadioButton("Primal Fortitude",this);
  user_flags_POAG = new QRadioButton("Polar Aggression",this);
  user_flags_POFD = new QRadioButton("Polar Fortitude",this);
  user_flags_CHAG = new QRadioButton("Charged Aggression",this);
  user_flags_CHFD = new QRadioButton("Charged Fortitude",this);
  user_flags_CYAG = new QRadioButton("Cybernetic Aggression",this);
  user_flags_CYFD = new QRadioButton("Cybernetic Fortitude",this);
  user_flags_NIAG = new QRadioButton("Nihil Aggression",this);
  user_flags_NIFD = new QRadioButton("Nihil Fortitude",this);
  user_flags_MMNT = new QRadioButton("Momentum",this);
  user_flags_LIMB = new QRadioButton("Limbertude",this);
  user_flags_UNBR = new QRadioButton("Unbearability",this);
  user_flags_MANN = new QRadioButton("Manna",this);
  user_flags_NOAT = new QRadioButton("No Attribute",this);
  /* User flags layout */
  user_flags_layout = new QVBoxLayout();
  user_flags_layout->addWidget(user_flags_VITA);
  user_flags_layout->addWidget(user_flags_QTDR);
  user_flags_layout->addWidget(user_flags_PHAG);
  user_flags_layout->addWidget(user_flags_PHFD);
  user_flags_layout->addWidget(user_flags_THAG);
  user_flags_layout->addWidget(user_flags_THFD);
  user_flags_layout->addWidget(user_flags_PRAG);
  user_flags_layout->addWidget(user_flags_PRFD);
  user_flags_layout->addWidget(user_flags_POAG);
  user_flags_layout->addWidget(user_flags_POFD);
  user_flags_layout->addWidget(user_flags_CHAG);
  user_flags_layout->addWidget(user_flags_CHFD);
  user_flags_layout->addWidget(user_flags_CYAG);
  user_flags_layout->addWidget(user_flags_CYFD);
  user_flags_layout->addWidget(user_flags_NIAG);
  user_flags_layout->addWidget(user_flags_NIFD);
  user_flags_layout->addWidget(user_flags_MMNT);
  user_flags_layout->addWidget(user_flags_LIMB);
  user_flags_layout->addWidget(user_flags_UNBR);
  user_flags_layout->addWidget(user_flags_MANN);
  user_flags_layout->addWidget(user_flags_NOAT);
  /* Add the layout to the groupbox */
  user_flags_box->setLayout(user_flags_layout);
  /* Add the user flags box to the radio layout */
  radio_layout->addWidget(user_flags_box);

  /* Groupbox for the user attribute flags */
  target_flags_box = new QGroupBox("Target Attribute Flags",this);
  /* Target flags */
  target_flags_VITA = new QRadioButton("Vitality",this);
  target_flags_QTDR = new QRadioButton("Quantum Drive (QD)",this);
  target_flags_PHAG = new QRadioButton("Physical Aggression",this);
  target_flags_PHFD = new QRadioButton("Physical Fortitude",this);
  target_flags_THAG = new QRadioButton("Thermal Aggression",this);
  target_flags_THFD = new QRadioButton("Thermal Fortitude",this);
  target_flags_PRAG = new QRadioButton("Primal Aggression",this);
  target_flags_PRFD = new QRadioButton("Primal Fortitude",this);
  target_flags_POAG = new QRadioButton("Polar Aggression",this);
  target_flags_POFD = new QRadioButton("Polar Fortitude",this);
  target_flags_CHAG = new QRadioButton("Charged Aggression",this);
  target_flags_CHFD = new QRadioButton("Charged Fortitude",this);
  target_flags_CYAG = new QRadioButton("Cybernetic Aggression",this);
  target_flags_CYFD = new QRadioButton("Cybernetic Fortitude",this);
  target_flags_NIAG = new QRadioButton("Nihil Aggression",this);
  target_flags_NIFD = new QRadioButton("Nihil Fortitude",this);
  target_flags_MMNT = new QRadioButton("Momentum",this);
  target_flags_LIMB = new QRadioButton("Limbertude",this);
  target_flags_UNBR = new QRadioButton("Unbearability",this);
  target_flags_MANN = new QRadioButton("Manna",this);
  target_flags_NOAT = new QRadioButton("No Attribute",this);
  /* Target flags layout */
  target_flags_layout = new QVBoxLayout();
  target_flags_layout->addWidget(target_flags_VITA);
  target_flags_layout->addWidget(target_flags_QTDR);
  target_flags_layout->addWidget(target_flags_PHAG);
  target_flags_layout->addWidget(target_flags_PHFD);
  target_flags_layout->addWidget(target_flags_THAG);
  target_flags_layout->addWidget(target_flags_THFD);
  target_flags_layout->addWidget(target_flags_PRAG);
  target_flags_layout->addWidget(target_flags_PRFD);
  target_flags_layout->addWidget(target_flags_POAG);
  target_flags_layout->addWidget(target_flags_POFD);
  target_flags_layout->addWidget(target_flags_CHAG);
  target_flags_layout->addWidget(target_flags_CHFD);
  target_flags_layout->addWidget(target_flags_CYAG);
  target_flags_layout->addWidget(target_flags_CYFD);
  target_flags_layout->addWidget(target_flags_NIAG);
  target_flags_layout->addWidget(target_flags_NIFD);
  target_flags_layout->addWidget(target_flags_MMNT);
  target_flags_layout->addWidget(target_flags_LIMB);
  target_flags_layout->addWidget(target_flags_UNBR);
  target_flags_layout->addWidget(target_flags_MANN);
  target_flags_layout->addWidget(target_flags_NOAT);
  /* Add the layout to the groupbox */
  target_flags_box->setLayout(target_flags_layout);

  /* Create buttons for loading */
  QPushButton* reset_button = new QPushButton("Reset");
  connect(reset_button,SIGNAL(pressed()),this,SLOT(resetInfo()));
  QPushButton* save_button = new QPushButton("Save Changes");
  connect(save_button,SIGNAL(pressed()),this,SLOT(getEditedAction()));
  /* Add the target flags box to the right layout */
  radio_layout->addWidget(target_flags_box);
  right_layout->addLayout(boxes_layout);
  right_layout->addLayout(radio_layout);
  buttons_layout = new QHBoxLayout();
  buttons_layout->addWidget(reset_button);
  buttons_layout->addWidget(save_button);
  right_layout->addLayout(buttons_layout);

  /* Set up the base action with just a damage call */
  setBaseAction(Action("0,DAMAGE,,,,,AMOUNT.0,AMOUNT.0,,100"));

  /* Set name */
  name = "";
}

/* Constructor function with id and name */
EditorAction::EditorAction(int id, QString name, QWidget* parent)
            : EditorAction(parent)
{
  setID(id);
  setName(name);
}

/* Copy constructor */
EditorAction::EditorAction(const EditorAction &source) : EditorAction()
{
  copySelf(source);
}

EditorAction::~EditorAction()
{
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/* Copy function, to be called by a copy or equal operator constructor */
void EditorAction::copySelf(const EditorAction &source)
{
  name = source.name;
  full_name = source.full_name;
  setBaseAction(source.base);
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

Infliction EditorAction::getAilment() const
{
  return working.getAilment();
}

int EditorAction::getBase() const
{
  return working.getBase();
}

Action EditorAction::getBaseAction()
{
  return base;
}

float EditorAction::getChance() const
{
  return working.getChance();
}

Action EditorAction::getEditedAction()
{
  QString full_name = "";
  name = name_edit->text();
  if(working.getID() < 10)
    full_name.append("0");
  full_name.append(QString::number(working.getID()));
  full_name.append(" : ");
  full_name.append(name);
  emit nameChange(full_name);

  working.setBaseValue(value_edit->text().toInt(),
                       action_flags_base_pc->isChecked());
  working.setChance(chance_edit->text().toFloat());
  working.setBaseVariance(variance_edit->text().toInt(),
                          action_flags_vari_pc->isChecked());
  working.setAilmentDuration(minduration_edit->text().toInt(),
                             maxduration_edit->text().toInt());
  working.setIgnoreAttack(IgnoreFlags::PHYSICAL,
                          ignoreatk_flags_physical->isChecked());
  working.setIgnoreAttack(IgnoreFlags::THERMAL,
                          ignoreatk_flags_thermal->isChecked());
  working.setIgnoreAttack(IgnoreFlags::POLAR,
                          ignoreatk_flags_polar->isChecked());
  working.setIgnoreAttack(IgnoreFlags::PRIMAL,
                          ignoreatk_flags_primal->isChecked());
  working.setIgnoreAttack(IgnoreFlags::CHARGED,
                          ignoreatk_flags_charged->isChecked());
  working.setIgnoreAttack(IgnoreFlags::NIHIL,
                          ignoreatk_flags_nihil->isChecked());
  working.setIgnoreAttack(IgnoreFlags::LUCK,
                          ignoreatk_flags_luck->isChecked());
  working.setIgnoreAttack(IgnoreFlags::CYBERNETIC,
                          ignoreatk_flags_cybernetic->isChecked());
  working.setIgnoreAttack(IgnoreFlags::ARMOR,
                          ignoreatk_flags_armor->isChecked());
  working.setIgnoreDefense(IgnoreFlags::PHYSICAL,
                          ignoredef_flags_physical->isChecked());
  working.setIgnoreDefense(IgnoreFlags::THERMAL,
                          ignoredef_flags_thermal->isChecked());
  working.setIgnoreDefense(IgnoreFlags::POLAR,
                          ignoredef_flags_polar->isChecked());
  working.setIgnoreDefense(IgnoreFlags::PRIMAL,
                          ignoredef_flags_primal->isChecked());
  working.setIgnoreDefense(IgnoreFlags::CHARGED,
                          ignoredef_flags_charged->isChecked());
  working.setIgnoreDefense(IgnoreFlags::NIHIL,
                          ignoredef_flags_nihil->isChecked());
  working.setIgnoreDefense(IgnoreFlags::LUCK,
                          ignoredef_flags_luck->isChecked());
  working.setIgnoreDefense(IgnoreFlags::CYBERNETIC,
                          ignoredef_flags_cybernetic->isChecked());
  working.setIgnoreDefense(IgnoreFlags::ARMOR,
                          ignoredef_flags_armor->isChecked());

  working.setActionFlag(ActionFlags::DAMAGE,action_flags_damage->isChecked());
  working.setActionFlag(ActionFlags::ALTER,action_flags_alter->isChecked() ||
                        action_flags_alterflip->isChecked());
  working.setActionFlag(ActionFlags::INFLICT,action_flags_inflict->isChecked());
  working.setActionFlag(ActionFlags::RELIEVE,action_flags_relieve->isChecked());
  working.setActionFlag(ActionFlags::ASSIGN,action_flags_assign->isChecked() ||
                        action_flags_assignflip->isChecked());
  working.setActionFlag(ActionFlags::REVIVE,action_flags_revive->isChecked());
  working.setActionFlag(ActionFlags::FLIP_ATTR,
                        action_flags_alterflip->isChecked() ||
                        action_flags_assignflip->isChecked());
  working.setActionFlag(ActionFlags::VALID, true);

  if(ailment_poison->isChecked())
    working.setAilment(Infliction::POISON);
  else if(ailment_burn->isChecked())
    working.setAilment(Infliction::BURN);
  else if(ailment_scald->isChecked())
    working.setAilment(Infliction::SCALD);
  else if(ailment_char->isChecked())
    working.setAilment(Infliction::CHARR);
  else if(ailment_berserk->isChecked())
    working.setAilment(Infliction::BERSERK);
  else if(ailment_confuse->isChecked())
    working.setAilment(Infliction::CONFUSE);
  else if(ailment_silence->isChecked())
    working.setAilment(Infliction::SILENCE);
  else if(ailment_bubbify->isChecked())
    working.setAilment(Infliction::BUBBIFY);
  else if(ailment_deathtimer->isChecked())
    working.setAilment(Infliction::DEATHTIMER);
  else if(ailment_paralysis->isChecked())
    working.setAilment(Infliction::PARALYSIS);
  else if(ailment_blindness->isChecked())
    working.setAilment(Infliction::BLINDNESS);
  else if(ailment_dreadstruck->isChecked())
    working.setAilment(Infliction::DREADSTRUCK);
  else if(ailment_dreamsnare->isChecked())
    working.setAilment(Infliction::DREAMSNARE);
  else if(ailment_hellbound->isChecked())
    working.setAilment(Infliction::HELLBOUND);
  else if(ailment_bond->isChecked())
    working.setAilment(Infliction::BOND);
  else if(ailment_bonded->isChecked())
    working.setAilment(Infliction::BONDED);
  else if(ailment_allatkbuff->isChecked())
    working.setAilment(Infliction::ALLATKBUFF);
  else if(ailment_alldefbuff->isChecked())
    working.setAilment(Infliction::ALLDEFBUFF);
  else if(ailment_physicalbuff->isChecked())
    working.setAilment(Infliction::PHYBUFF);
  else if(ailment_thermalbuff->isChecked())
    working.setAilment(Infliction::THRBUFF);
  else if(ailment_polarbuff->isChecked())
    working.setAilment(Infliction::POLBUFF);
  else if(ailment_primalbuff->isChecked())
    working.setAilment(Infliction::PRIBUFF);
  else if(ailment_chargedbuff->isChecked())
    working.setAilment(Infliction::CHGBUFF);
  else if(ailment_cyberneticbuff->isChecked())
    working.setAilment(Infliction::CYBBUFF);
  else if(ailment_nihilbuff->isChecked())
    working.setAilment(Infliction::NIHBUFF);
  else if(ailment_limbertudebuff->isChecked())
    working.setAilment(Infliction::LIMBUFF);
  else if(ailment_unbearabilitybuff->isChecked())
    working.setAilment(Infliction::UNBBUFF);
  else if(ailment_vitalitybuff->isChecked())
    working.setAilment(Infliction::VITBUFF);
  else if(ailment_qdbuff->isChecked())
    working.setAilment(Infliction::QDBUFF);
  else if(ailment_rootbound->isChecked())
    working.setAilment(Infliction::ROOTBOUND);
  else if(ailment_doublecast->isChecked())
    working.setAilment(Infliction::DOUBLECAST);
  else if(ailment_triplecast->isChecked())
    working.setAilment(Infliction::TRIPLECAST);
  else if(ailment_halfcost->isChecked())
    working.setAilment(Infliction::HALFCOST);
  else if(ailment_reflect->isChecked())
    working.setAilment(Infliction::REFLECT);
  else if(ailment_hibernation->isChecked())
    working.setAilment(Infliction::HIBERNATION);
  else if(ailment_curse->isChecked())
    working.setAilment(Infliction::CURSE);
  else if(ailment_metatether->isChecked())
    working.setAilment(Infliction::METATETHER);
  else if(ailment_modulate->isChecked())
    working.setAilment(Infliction::MODULATE);
  else if(ailment_invalid->isChecked())
    working.setAilment(Infliction::INVALID);

  if(user_flags_VITA->isChecked())
    working.setAttributeUser(Attribute::VITA);
  else if(user_flags_QTDR->isChecked())
    working.setAttributeUser(Attribute::QTDR);
  else if(user_flags_PHAG->isChecked())
    working.setAttributeUser(Attribute::PHAG);
  else if(user_flags_PHFD->isChecked())
    working.setAttributeUser(Attribute::PHFD);
  else if(user_flags_THAG->isChecked())
    working.setAttributeUser(Attribute::THAG);
  else if(user_flags_THFD->isChecked())
    working.setAttributeUser(Attribute::THFD);
  else if(user_flags_PRAG->isChecked())
    working.setAttributeUser(Attribute::PRAG);
  else if(user_flags_PRFD->isChecked())
    working.setAttributeUser(Attribute::PRFD);
  else if(user_flags_POAG->isChecked())
    working.setAttributeUser(Attribute::POAG);
  else if(user_flags_POFD->isChecked())
    working.setAttributeUser(Attribute::POFD);
  else if(user_flags_CHAG->isChecked())
    working.setAttributeUser(Attribute::CHAG);
  else if(user_flags_CHFD->isChecked())
    working.setAttributeUser(Attribute::CHFD);
  else if(user_flags_CYAG->isChecked())
    working.setAttributeUser(Attribute::CYAG);
  else if(user_flags_CYFD->isChecked())
    working.setAttributeUser(Attribute::CYFD);
  else if(user_flags_NIAG->isChecked())
    working.setAttributeUser(Attribute::NIAG);
  else if(user_flags_NIFD->isChecked())
    working.setAttributeUser(Attribute::NIFD);
  else if(user_flags_MMNT->isChecked())
    working.setAttributeUser(Attribute::MMNT);
  else if(user_flags_LIMB->isChecked())
    working.setAttributeUser(Attribute::LIMB);
  else if(user_flags_UNBR->isChecked())
    working.setAttributeUser(Attribute::UNBR);
  else if(user_flags_MANN->isChecked())
    working.setAttributeUser(Attribute::MANN);
  else if(user_flags_NOAT->isChecked())
    working.setAttributeUser(Attribute::NONE);

  if(target_flags_VITA->isChecked())
    working.setAttributeTarget(Attribute::VITA);
  else if(target_flags_QTDR->isChecked())
    working.setAttributeTarget(Attribute::QTDR);
  else if(target_flags_PHAG->isChecked())
    working.setAttributeTarget(Attribute::PHAG);
  else if(target_flags_PHFD->isChecked())
    working.setAttributeTarget(Attribute::PHFD);
  else if(target_flags_THAG->isChecked())
    working.setAttributeTarget(Attribute::THAG);
  else if(target_flags_THFD->isChecked())
    working.setAttributeTarget(Attribute::THFD);
  else if(target_flags_PRAG->isChecked())
    working.setAttributeTarget(Attribute::PRAG);
  else if(target_flags_PRFD->isChecked())
    working.setAttributeTarget(Attribute::PRFD);
  else if(target_flags_POAG->isChecked())
    working.setAttributeTarget(Attribute::POAG);
  else if(target_flags_POFD->isChecked())
    working.setAttributeTarget(Attribute::POFD);
  else if(target_flags_CHAG->isChecked())
    working.setAttributeTarget(Attribute::CHAG);
  else if(target_flags_CHFD->isChecked())
    working.setAttributeTarget(Attribute::CHFD);
  else if(target_flags_CYAG->isChecked())
    working.setAttributeTarget(Attribute::CYAG);
  else if(target_flags_CYFD->isChecked())
    working.setAttributeTarget(Attribute::CYFD);
  else if(target_flags_NIAG->isChecked())
    working.setAttributeTarget(Attribute::NIAG);
  else if(target_flags_NIFD->isChecked())
    working.setAttributeTarget(Attribute::NIFD);
  else if(target_flags_MMNT->isChecked())
    working.setAttributeTarget(Attribute::MMNT);
  else if(target_flags_LIMB->isChecked())
    working.setAttributeTarget(Attribute::LIMB);
  else if(target_flags_UNBR->isChecked())
    working.setAttributeTarget(Attribute::UNBR);
  else if(target_flags_MANN->isChecked())
    working.setAttributeTarget(Attribute::MANN);
  else if(target_flags_NOAT->isChecked())
    working.setAttributeTarget(Attribute::NONE);

  base = working;
  outputString();
  loadWorkingInfo();
  return base;
}

int EditorAction::getMin() const
{
  return working.getMax();
}

int EditorAction::getMax() const
{
  return working.getMax();
}

Attribute EditorAction::getTargetAttribute() const
{
  return working.getTargetAttribute();
}

Attribute EditorAction::getUserAttribute() const
{
  return working.getUserAttribute();
}

int EditorAction::getVariance() const
{
  return working.getVariance();
}

Action EditorAction::getWorkingAction()
{
  return working;
}

void EditorAction::loadWorkingInfo()
{
  name_edit->setText(name);
  value_edit->setText(QString::number(working.getBase()));
  chance_edit->setText(QString::number(working.getChance()));
  variance_edit->setText(QString::number(working.getVariance()));
  minduration_edit->setText(QString::number(working.getMin()));
  maxduration_edit->setText(QString::number(working.getMax()));

  if(working.atkFlag(IgnoreFlags::PHYSICAL))
    ignoreatk_flags_physical->setChecked(true);
  if(working.atkFlag(IgnoreFlags::THERMAL))
    ignoreatk_flags_thermal->setChecked(true);
  if(working.atkFlag(IgnoreFlags::POLAR))
    ignoreatk_flags_polar->setChecked(true);
  if(working.atkFlag(IgnoreFlags::PRIMAL))
    ignoreatk_flags_primal->setChecked(true);
  if(working.atkFlag(IgnoreFlags::CHARGED))
    ignoreatk_flags_charged->setChecked(true);
  if(working.atkFlag(IgnoreFlags::NIHIL))
    ignoreatk_flags_nihil->setChecked(true);
  if(working.atkFlag(IgnoreFlags::LUCK))
    ignoreatk_flags_luck->setChecked(true);
  if(working.atkFlag(IgnoreFlags::CYBERNETIC))
    ignoreatk_flags_cybernetic->setChecked(true);
  if(working.atkFlag(IgnoreFlags::ARMOR))
    ignoreatk_flags_armor->setChecked(true);

  if(working.defFlag(IgnoreFlags::PHYSICAL))
    ignoredef_flags_physical->setChecked(true);
  if(working.defFlag(IgnoreFlags::THERMAL))
    ignoredef_flags_thermal->setChecked(true);
  if(working.defFlag(IgnoreFlags::POLAR))
    ignoredef_flags_polar->setChecked(true);
  if(working.defFlag(IgnoreFlags::PRIMAL))
    ignoredef_flags_primal->setChecked(true);
  if(working.defFlag(IgnoreFlags::CHARGED))
    ignoredef_flags_charged->setChecked(true);
  if(working.defFlag(IgnoreFlags::NIHIL))
    ignoredef_flags_nihil->setChecked(true);
  if(working.defFlag(IgnoreFlags::LUCK))
    ignoredef_flags_luck->setChecked(true);
  if(working.defFlag(IgnoreFlags::CYBERNETIC))
    ignoredef_flags_cybernetic->setChecked(true);
  if(working.defFlag(IgnoreFlags::ARMOR))
    ignoredef_flags_armor->setChecked(true);

  if(working.actionFlag(ActionFlags::DAMAGE))
    action_flags_damage->setChecked(true);
  if(working.actionFlag(ActionFlags::ALTER))
    action_flags_alter->setChecked(true);
  if(working.actionFlag(ActionFlags::FLIP_ATTR) &&
     working.actionFlag(ActionFlags::ALTER))
    action_flags_alterflip->setChecked(true);
  if(working.actionFlag(ActionFlags::INFLICT))
    action_flags_inflict->setChecked(true);
  if(working.actionFlag(ActionFlags::RELIEVE))
    action_flags_relieve->setChecked(true);
  if(working.actionFlag(ActionFlags::ASSIGN))
    action_flags_assign->setChecked(true);
  if(working.actionFlag(ActionFlags::FLIP_ATTR) &&
     working.actionFlag(ActionFlags::ASSIGN))
    action_flags_assignflip->setChecked(true);
  if(working.actionFlag(ActionFlags::REVIVE))
    action_flags_revive->setChecked(true);
  if(working.actionFlag(ActionFlags::BASE_PC))
    action_flags_base_pc->setChecked(true);
  if(working.actionFlag(ActionFlags::VARI_PC))
    action_flags_vari_pc->setChecked(true);

  switch(working.getAilment())
  {
    case Infliction::POISON:
      ailment_poison->setChecked(true);
      break;
    case Infliction::BURN:
      ailment_burn->setChecked(true);
      break;
    case Infliction::SCALD:
      ailment_scald->setChecked(true);
      break;
    case Infliction::CHARR:
      ailment_char->setChecked(true);
      break;
    case Infliction::BERSERK:
      ailment_berserk->setChecked(true);
      break;
    case Infliction::CONFUSE:
      ailment_confuse->setChecked(true);
      break;
    case Infliction::SILENCE:
      ailment_silence->setChecked(true);
      break;
    case Infliction::BUBBIFY:
      ailment_bubbify->setChecked(true);
      break;
    case Infliction::DEATHTIMER:
      ailment_deathtimer->setChecked(true);
      break;
    case Infliction::PARALYSIS:
      ailment_paralysis->setChecked(true);
      break;
    case Infliction::BLINDNESS:
      ailment_blindness->setChecked(true);
      break;
    case Infliction::DREADSTRUCK:
      ailment_dreadstruck->setChecked(true);
      break;
    case Infliction::DREAMSNARE:
      ailment_dreamsnare->setChecked(true);
      break;
    case Infliction::HELLBOUND:
      ailment_hellbound->setChecked(true);
      break;
    case Infliction::BOND:
      ailment_bond->setChecked(true);
      break;
    case Infliction::BONDED:
      ailment_bonded->setChecked(true);
      break;
    case Infliction::ALLATKBUFF:
      ailment_allatkbuff->setChecked(true);
      break;
    case Infliction::ALLDEFBUFF:
      ailment_alldefbuff->setChecked(true);
      break;
    case Infliction::PHYBUFF:
      ailment_physicalbuff->setChecked(true);
      break;
    case Infliction::THRBUFF:
      ailment_thermalbuff->setChecked(true);
      break;
    case Infliction::POLBUFF:
      ailment_polarbuff->setChecked(true);
      break;
    case Infliction::PRIBUFF:
      ailment_primalbuff->setChecked(true);
      break;
    case Infliction::CHGBUFF:
      ailment_chargedbuff->setChecked(true);
      break;
    case Infliction::CYBBUFF:
      ailment_cyberneticbuff->setChecked(true);
      break;
    case Infliction::NIHBUFF:
      ailment_nihilbuff->setChecked(true);
      break;
    case Infliction::LIMBUFF:
      ailment_limbertudebuff->setChecked(true);
      break;
    case Infliction::UNBBUFF:
      ailment_unbearabilitybuff->setChecked(true);
      break;
    case Infliction::VITBUFF:
      ailment_vitalitybuff->setChecked(true);
      break;
    case Infliction::QDBUFF:
      ailment_qdbuff->setChecked(true);
      break;
    case Infliction::ROOTBOUND:
      ailment_rootbound->setChecked(true);
      break;
    case Infliction::DOUBLECAST:
      ailment_doublecast->setChecked(true);
      break;
    case Infliction::TRIPLECAST:
      ailment_triplecast->setChecked(true);
      break;
    case Infliction::HALFCOST:
      ailment_halfcost->setChecked(true);
      break;
    case Infliction::REFLECT:
      ailment_reflect->setChecked(true);
      break;
    case Infliction::HIBERNATION:
      ailment_hibernation->setChecked(true);
      break;
    case Infliction::CURSE:
      ailment_curse->setChecked(true);
      break;
    case Infliction::METATETHER:
      ailment_metatether->setChecked(true);
      break;
    case Infliction::MODULATE:
      ailment_modulate->setChecked(true);
      break;
    case Infliction::INVALID:
      ailment_invalid->setChecked(true);
      break;
    default:
      ailment_none->setChecked(true);
      break;
  }

  switch(working.getUserAttribute())
  {
    case Attribute::VITA:
      user_flags_VITA->setChecked(true);
      break;
    case Attribute::QTDR:
      user_flags_QTDR->setChecked(true);
      break;
    case Attribute::PHAG:
      user_flags_PHAG->setChecked(true);
      break;
    case Attribute::PHFD:
      user_flags_PHFD->setChecked(true);
      break;
    case Attribute::THAG:
      user_flags_THAG->setChecked(true);
      break;
    case Attribute::THFD:
      user_flags_THFD->setChecked(true);
      break;
    case Attribute::PRAG:
      user_flags_PRAG->setChecked(true);
      break;
    case Attribute::PRFD:
      user_flags_PRFD->setChecked(true);
      break;
    case Attribute::POAG:
      user_flags_POAG->setChecked(true);
      break;
    case Attribute::POFD:
      user_flags_POFD->setChecked(true);
      break;
    case Attribute::CHAG:
      user_flags_CHAG->setChecked(true);
      break;
    case Attribute::CHFD:
      user_flags_CHFD->setChecked(true);
      break;
    case Attribute::CYAG:
      user_flags_CYAG->setChecked(true);
      break;
    case Attribute::CYFD:
      user_flags_CYFD->setChecked(true);
      break;
    case Attribute::NIAG:
      user_flags_NIAG->setChecked(true);
      break;
    case Attribute::NIFD:
      user_flags_NIFD->setChecked(true);
      break;
    case Attribute::MMNT:
      user_flags_MMNT->setChecked(true);
      break;
    case Attribute::LIMB:
      user_flags_LIMB->setChecked(true);
      break;
    case Attribute::UNBR:
      user_flags_UNBR->setChecked(true);
      break;
    case Attribute::MANN:
      user_flags_MANN->setChecked(true);
      break;
    case Attribute::NONE:
      user_flags_NOAT->setChecked(true);
      break;
    default:
      break;
  }
  switch(working.getTargetAttribute())
  {
    case Attribute::VITA:
      target_flags_VITA->setChecked(true);
      break;
    case Attribute::QTDR:
      target_flags_QTDR->setChecked(true);
      break;
    case Attribute::PHAG:
      target_flags_PHAG->setChecked(true);
      break;
    case Attribute::PHFD:
      target_flags_PHFD->setChecked(true);
      break;
    case Attribute::THAG:
      target_flags_THAG->setChecked(true);
      break;
    case Attribute::THFD:
      target_flags_THFD->setChecked(true);
      break;
    case Attribute::PRAG:
      target_flags_PRAG->setChecked(true);
      break;
    case Attribute::PRFD:
      target_flags_PRFD->setChecked(true);
      break;
    case Attribute::POAG:
      target_flags_POAG->setChecked(true);
      break;
    case Attribute::POFD:
      target_flags_POFD->setChecked(true);
      break;
    case Attribute::CHAG:
      target_flags_CHAG->setChecked(true);
      break;
    case Attribute::CHFD:
      target_flags_CHFD->setChecked(true);
      break;
    case Attribute::CYAG:
      target_flags_CYAG->setChecked(true);
      break;
    case Attribute::CYFD:
      target_flags_CYFD->setChecked(true);
      break;
    case Attribute::NIAG:
      target_flags_NIAG->setChecked(true);
      break;
    case Attribute::NIFD:
      target_flags_NIFD->setChecked(true);
      break;
    case Attribute::MMNT:
      target_flags_MMNT->setChecked(true);
      break;
    case Attribute::LIMB:
      target_flags_LIMB->setChecked(true);
      break;
    case Attribute::UNBR:
      target_flags_UNBR->setChecked(true);
      break;
    case Attribute::MANN:
      target_flags_MANN->setChecked(true);
      break;
    case Attribute::NONE:
      target_flags_NOAT->setChecked(true);
      break;
    default:
      break;
  }
}

QString EditorAction::outputString()
{
  //qDebug() << QString::fromStdString(base.outputString());
  return QString::fromStdString(base.outputString());
}

void EditorAction::resetInfo()
{
  working = base;
  loadWorkingInfo();
}

void EditorAction::setActionFlag(ActionFlags set_flag, bool set)
{
  working.setActionFlag(set_flag,set);
}

void EditorAction::setAilment(Infliction ailment)
{
  working.setAilment(ailment);
}

bool EditorAction::setAilmentDuration(int min, int max)
{
  working.setAilmentDuration(min,max);
  return true;
}

void EditorAction::setAttributeTarget(Attribute target)
{
  working.setAttributeTarget(target);
}

void EditorAction::setAttributeUser(Attribute user)
{
  working.setAttributeUser(user);
}

void EditorAction::setBaseAction(Action a)
{
  base = a;
  setWorkingAction(base);
  loadWorkingInfo();
  updateLayouts();
}

void EditorAction::setBaseValue(int value, bool percent)
{
  working.setBaseValue(value,percent);
}

void EditorAction::setBaseVariance(int variance, bool percent)
{
  working.setBaseVariance(variance,percent);
}

bool EditorAction::setChance(float chance)
{
  return working.setChance(chance);
}

void EditorAction::setIgnoreAttack(IgnoreFlags flag, bool set)
{
  working.setIgnoreAttack(flag,set);
}

void EditorAction::setIgnoreDefense(IgnoreFlags flag, bool set)
{
  working.setIgnoreDefense(flag,set);
}

void EditorAction::setNameAndID(QString str)
{
  base.setID(str.split(" : ").at(0).toInt());
  name = str.split(" : ").at(1);
  name_edit->setText(name);
  setWorkingAction(base);
}

void EditorAction::setWorkingAction(Action a)
{
  (void)a;
  working  = base;//a;
  updateLayouts();
}

void EditorAction::updateLayouts()
{
  if(action_flags_inflict->isChecked())
  {
    minduration_edit->setDisabled(false);
    minduration_label->setDisabled(false);
    maxduration_edit->setDisabled(false);
    maxduration_label->setDisabled(false);
  }
  else
  {
    minduration_edit->setDisabled(true);
    minduration_label->setDisabled(true);
    maxduration_edit->setDisabled(true);
    maxduration_label->setDisabled(true);
  }

  if(action_flags_inflict->isChecked() || action_flags_relieve->isChecked())
    ailment_box->setDisabled(false);
  else
    ailment_box->setDisabled(true);

  if(action_flags_damage->isChecked())
  {
    ignoreatk_flags_box->setDisabled(false);
    ignoredef_flags_box->setDisabled(false);
  }
  else
  {
    ignoreatk_flags_box->setDisabled(true);
    ignoredef_flags_box->setDisabled(true);
  }

  if(action_flags_assign->isChecked() || action_flags_assignflip->isChecked() ||
     action_flags_alter->isChecked() || action_flags_alterflip->isChecked())
  {
    user_flags_box->setDisabled(false);
    target_flags_box->setDisabled(false);
  }
  else
  {
    user_flags_box->setDisabled(true);
    target_flags_box->setDisabled(true);
  }

  if(action_flags_inflict->isChecked() || action_flags_relieve->isChecked())
  {
    value_label->setDisabled(true);
    value_edit->setDisabled(true);
    variance_label->setDisabled(true);
    variance_edit->setDisabled(true);
    action_flags_base_pc->setDisabled(true);
    action_flags_vari_pc->setDisabled(true);
  }
  else
  {
    value_label->setDisabled(false);
    value_edit->setDisabled(false);
    variance_label->setDisabled(false);
    variance_edit->setDisabled(false);
    action_flags_base_pc->setDisabled(false);
    action_flags_vari_pc->setDisabled(false);
  }

}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Clone */
EditorAction* EditorAction::clone()
{
  return this;
}

/* Returns the ID of the Action */
int EditorAction::getID() const
{
  return working.getID();
}

/* Returns the name of the item */
QString EditorAction::getName() const
{
  return name;
}

/* Returns the name of the item for listing */
QString EditorAction::getNameList()
{
  return EditorHelpers::getListString(getID(), getName());
}

/* Loads the thing data */
void EditorAction::load(XmlData data, int index)
{
  /* Parse elements */
  if(data.getElement(index) == "action")
  {
    /* If only the action element - like directly from game */
    if(data.getTailElements(index).size() == 1)
    {
      base.parse(data.getDataString());
    }
    /* Otherwise, normal expected editor save */
    else
    {
      /* Name */
      if(data.getElement(index + 1) == "name")
      {
        setName(QString::fromStdString(data.getDataString()));
      }
      /* Action string */
      else if(data.getElement(index + 1) == "str")
      {
        base.parse(data.getDataString());
      }
    }
  }
}

/* Saves the thing data */
void EditorAction::save(FileHandler* fh, bool game_only)
{
  if(fh != NULL)
  {
    /* Game only: is just one string for the action */
    if(game_only)
    {
      fh->writeXmlData("action", outputString().toStdString());
    }
    /* Editor: Also, contains name and separate string */
    else
    {
      fh->writeXmlElement("action", "id", getID());

      fh->writeXmlData("name", getName().toStdString());
      fh->writeXmlData("str", outputString().toStdString());

      fh->writeXmlElementEnd();
    }
  }
}

/* Sets the ID */
void EditorAction::setID(int id)
{
  base.setID(id);
  setWorkingAction(base);
}

/* Sets the name */
void EditorAction::setName(QString name)
{
  this->name = name;
  name_edit->setText(name);
  setWorkingAction(base);
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/* The copy operator */
EditorAction& EditorAction::operator= (const EditorAction &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
