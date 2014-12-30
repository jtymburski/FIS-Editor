#include "Database/EditorAction.h"
#include <QDebug>

EditorAction::EditorAction(QWidget *parent) : QWidget(parent)
{
  /* Main Layout */
  QHBoxLayout* main_layout = new QHBoxLayout(this);
  QGridLayout* text_layout = new QGridLayout();
  QLabel* name_label = new QLabel("Name",this);
  text_layout->addWidget(name_label,0,0);
  QLabel* value_label = new QLabel("Value",this);
  text_layout->addWidget(value_label,1,0);
  QLabel* chance_label = new QLabel("Chance",this);
  text_layout->addWidget(chance_label,2,0);
  QLabel* varience_label = new QLabel("Varience",this);
  text_layout->addWidget(varience_label,3,0);
  QLabel* minduration_label = new QLabel("Minimum Duration",this);
  text_layout->addWidget(minduration_label,4,0);
  QLabel* maxduration_label = new QLabel("Maximum Duration",this);
  text_layout->addWidget(maxduration_label,5,0);

  QLineEdit* name_edit = new QLineEdit(this);
  text_layout->addWidget(name_edit,0,1);
  QLineEdit* value_edit = new QLineEdit(this);
  text_layout->addWidget(value_edit,1,1);
  QLineEdit* chance_edit = new QLineEdit(this);
  text_layout->addWidget(chance_edit,2,1);
  QLineEdit* varience_edit = new QLineEdit(this);
  text_layout->addWidget(varience_edit,3,1);
  QLineEdit* minduration_edit = new QLineEdit(this);
  text_layout->addWidget(minduration_edit,4,1);
  QLineEdit* maxduration_edit = new QLineEdit(this);
  text_layout->addWidget(maxduration_edit,5,1);
  main_layout->addLayout(text_layout);

  /* Left Layout */
  QVBoxLayout* left_layout = new QVBoxLayout();
  /* Right Layout */
  QVBoxLayout* right_layout = new QVBoxLayout();
  /* Create main layout */
  main_layout->addLayout(left_layout);
  main_layout->addLayout(right_layout);

  /* Boxes Layout */
  QHBoxLayout* boxes_layout = new QHBoxLayout();
  /* Groupbox for the action flags */
  QGroupBox* action_flags_box = new QGroupBox("Action Flags",this);
  /* Action flags */
  QRadioButton* action_flags_damage = new QRadioButton("Damage",this);
  QRadioButton* action_flags_alter = new QRadioButton("Alter",this);
  QRadioButton* action_flags_inflict = new QRadioButton("Inflict",this);
  QRadioButton* action_flags_relieve = new QRadioButton("Relieve",this);
  QRadioButton* action_flags_assign = new QRadioButton("Assign",this);
  QRadioButton* action_flags_revive = new QRadioButton("Revive",this);
  QRadioButton* action_flags_base_pc = new QRadioButton("Base is Percent",this);
  QRadioButton* action_flags_vari_pc = new QRadioButton("Varience is Percent",this);
  QRadioButton* action_flags_flip_attr = new QRadioButton("Flip Attributes",this);
  //QRadioButton* action_flags_valid = new QRadioButton("Valid Action",this);
  /* Action flags layout */
  QVBoxLayout* action_flags_layout = new QVBoxLayout();
  action_flags_layout->addWidget(action_flags_damage);
  action_flags_layout->addWidget(action_flags_alter);
  action_flags_layout->addWidget(action_flags_inflict);
  action_flags_layout->addWidget(action_flags_relieve);
  action_flags_layout->addWidget(action_flags_assign);
  action_flags_layout->addWidget(action_flags_revive);
  action_flags_layout->addWidget(action_flags_base_pc);
  action_flags_layout->addWidget(action_flags_vari_pc);
  action_flags_layout->addWidget(action_flags_flip_attr);
  //action_flags_layout->addWidget(action_flags_valid);
  /* Add the layout to the groupbox */
  action_flags_box->setLayout(action_flags_layout);
  /* Add the action flags box to the boxes layout */
  boxes_layout->addWidget(action_flags_box);

  /* Groupbox for the ignore attack flags */
  QGroupBox* ignoreatk_flags_box = new QGroupBox("Ignore Attack Flags",this);
  /* Ignore Attack flags */
  QCheckBox* ignoreatk_flags_physical = new QCheckBox("Physical",this);
  QCheckBox* ignoreatk_flags_thermal = new QCheckBox("Thermal",this);
  QCheckBox* ignoreatk_flags_polar = new QCheckBox("Polar",this);
  QCheckBox* ignoreatk_flags_primal = new QCheckBox("Primal",this);
  QCheckBox* ignoreatk_flags_charged = new QCheckBox("Charged",this);
  QCheckBox* ignoreatk_flags_nihil = new QCheckBox("Nihil",this);
  QCheckBox* ignoreatk_flags_luck = new QCheckBox("Luck",this);
  QCheckBox* ignoreatk_flags_cybernetic = new QCheckBox("Cybernetic",this);
  QCheckBox* ignoreatk_flags_armor = new QCheckBox("Armor",this);
  /* Action flags layout */
  QVBoxLayout* ignoreatk_flags_layout = new QVBoxLayout();
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
  QGroupBox* ignoredef_flags_box = new QGroupBox("Ignore Defense Flags",this);
  /* Ignore Attack flags */
  QCheckBox* ignoredef_flags_physical = new QCheckBox("Physical",this);
  QCheckBox* ignoredef_flags_thermal = new QCheckBox("Thermal",this);
  QCheckBox* ignoredef_flags_polar = new QCheckBox("Polar",this);
  QCheckBox* ignoredef_flags_primal = new QCheckBox("Primal",this);
  QCheckBox* ignoredef_flags_charged = new QCheckBox("Charged",this);
  QCheckBox* ignoredef_flags_nihil = new QCheckBox("Nihil",this);
  QCheckBox* ignoredef_flags_luck = new QCheckBox("Luck",this);
  QCheckBox* ignoredef_flags_cybernetic = new QCheckBox("Cybernetic",this);
  QCheckBox* ignoredef_flags_armor = new QCheckBox("Armor",this);
  /* Action flags layout */
  QVBoxLayout* ignoredef_flags_layout = new QVBoxLayout();
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
  QHBoxLayout* radio_layout = new QHBoxLayout();
  /* Groupbox for the ailment flags */
  QGroupBox* ailment_box = new QGroupBox("Ailment Infliction",this);
  /* user flags */
  QRadioButton* ailment_none = new QRadioButton("None",this);
  QRadioButton* ailment_poison = new QRadioButton("Poison",this);
  QRadioButton* ailment_burn = new QRadioButton("Burn",this);
  QRadioButton* ailment_scald = new QRadioButton("Scald",this);
  QRadioButton* ailment_char = new QRadioButton("Char",this);
  QRadioButton* ailment_berserk = new QRadioButton("Berserk",this);
  QRadioButton* ailment_confuse = new QRadioButton("Confuse",this);
  QRadioButton* ailment_silence = new QRadioButton("Silence",this);
  QRadioButton* ailment_bubbify = new QRadioButton("Bubbify",this);
  QRadioButton* ailment_deathtimer = new QRadioButton("Death Timer",this);
  QRadioButton* ailment_paralysis = new QRadioButton("Paralysis",this);
  QRadioButton* ailment_blindness = new QRadioButton("Blindness",this);
  QRadioButton* ailment_dreadstruck = new QRadioButton("Dreadstruck",this);
  QRadioButton* ailment_dreamsnare = new QRadioButton("Dreamsnare",this);
  QRadioButton* ailment_hellbound = new QRadioButton("Hellbound",this);
  QRadioButton* ailment_bond = new QRadioButton("Bond",this);
  QRadioButton* ailment_bonded = new QRadioButton("Bonded",this);
  QRadioButton* ailment_allatkbuff = new QRadioButton("All Attack Buff",this);
  QRadioButton* ailment_alldefbuff = new QRadioButton("All Defense Buff",this);
  QRadioButton* ailment_physicalbuff = new QRadioButton("Physical Buff",this);
  QRadioButton* ailment_thermalbuff = new QRadioButton("Thermal Buff",this);
  QRadioButton* ailment_polarbuff = new QRadioButton("Polar Buff",this);
  QRadioButton* ailment_primalbuff = new QRadioButton("Primal Buff",this);
  QRadioButton* ailment_chargedbuff = new QRadioButton("Charged Buff",this);
  QRadioButton* ailment_cyberneticbuff = new QRadioButton("Cybernetic Buff",this);
  QRadioButton* ailment_nihilbuff = new QRadioButton("Nihil Buff",this);
  QRadioButton* ailment_limbertudebuff = new QRadioButton("Limbertude Buff",this);
  QRadioButton* ailment_unbearabilitybuff = new QRadioButton("Unbearability Buff",this);
  QRadioButton* ailment_vitalitybuff = new QRadioButton("Vitality Buff",this);
  QRadioButton* ailment_qdbuff = new QRadioButton("QD Buff",this);
  QRadioButton* ailment_rootbound = new QRadioButton("Rootbound",this);
  QRadioButton* ailment_doublecast = new QRadioButton("Doublecast",this);
  QRadioButton* ailment_triplecast = new QRadioButton("Triplecast",this);
  QRadioButton* ailment_halfcost = new QRadioButton("Half Cost",this);
  QRadioButton* ailment_reflect = new QRadioButton("Reflect",this);
  QRadioButton* ailment_hibernation = new QRadioButton("Hibernation",this);
  QRadioButton* ailment_curse = new QRadioButton("Curse",this);
  QRadioButton* ailment_metatether = new QRadioButton("Meta-Tether",this);
  QRadioButton* ailment_stubulate = new QRadioButton("Stubulate",this);
  QRadioButton* ailment_invalid = new QRadioButton("Invalid",this);
  /* Ailment layout */
  QVBoxLayout* ailment_layout = new QVBoxLayout();
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
  ailment_layout->addWidget(ailment_thermalbuff);
  ailment_layout->addWidget(ailment_polarbuff);
  ailment_layout->addWidget(ailment_primalbuff);
  ailment_layout->addWidget(ailment_chargedbuff);
  ailment_layout->addWidget(ailment_cyberneticbuff);
  ailment_layout->addWidget(ailment_nihilbuff);
  ailment_layout->addWidget(ailment_limbertudebuff);
  ailment_layout->addWidget(ailment_unbearabilitybuff);
  ailment_layout->addWidget(ailment_vitalitybuff);
  ailment_layout->addWidget(ailment_qdbuff);
  ailment_layout->addWidget(ailment_rootbound);
  ailment_layout->addWidget(ailment_doublecast);
  ailment_layout->addWidget(ailment_triplecast);
  ailment_layout->addWidget(ailment_halfcost);
  ailment_layout->addWidget(ailment_reflect);
  ailment_layout->addWidget(ailment_hibernation);
  ailment_layout->addWidget(ailment_curse);
  ailment_layout->addWidget(ailment_metatether);
  ailment_layout->addWidget(ailment_stubulate);
  ailment_layout->addWidget(ailment_invalid);
  /* Add the layout to the groupbox */
  ailment_box->setLayout(ailment_layout);
  /* Add the user flags box to the left layout */
  left_layout->addWidget(ailment_box);

  /* Groupbox for the user attribute flags */
  QGroupBox* user_flags_box = new QGroupBox("User Attribute Flags",this);
  /* User flags */
  QRadioButton* user_flags_VITA = new QRadioButton("Vitality",this);
  QRadioButton* user_flags_QTDR = new QRadioButton("Quantum Drive (QD)",this);
  QRadioButton* user_flags_PHAG = new QRadioButton("Physical Aggression",this);
  QRadioButton* user_flags_PHFD = new QRadioButton("Physical Fortitude",this);
  QRadioButton* user_flags_THAG = new QRadioButton("Thermal Aggression",this);
  QRadioButton* user_flags_THFD = new QRadioButton("Thermal Fortitude",this);
  QRadioButton* user_flags_PRAG = new QRadioButton("Primal Aggression",this);
  QRadioButton* user_flags_PRFD = new QRadioButton("Primal Fortitude",this);
  QRadioButton* user_flags_POAG = new QRadioButton("Polar Aggression",this);
  QRadioButton* user_flags_POFD = new QRadioButton("Polar Fortitude",this);
  QRadioButton* user_flags_CHAG = new QRadioButton("Charged Aggression",this);
  QRadioButton* user_flags_CHFD = new QRadioButton("Charged Fortitude",this);
  QRadioButton* user_flags_CYAG = new QRadioButton("Cybernetic Aggression",this);
  QRadioButton* user_flags_CYFD = new QRadioButton("Cybernetic Fortitude",this);
  QRadioButton* user_flags_NIAG = new QRadioButton("Nihil Aggression",this);
  QRadioButton* user_flags_NIFD = new QRadioButton("Nihil Fortitude",this);
  QRadioButton* user_flags_MMNT = new QRadioButton("Momentum",this);
  QRadioButton* user_flags_LIMB = new QRadioButton("Limbertude",this);
  QRadioButton* user_flags_UNBR = new QRadioButton("Unbearability",this);
  QRadioButton* user_flags_MANN = new QRadioButton("Manna",this);
  QRadioButton* user_flags_NOAT = new QRadioButton("No Attribute",this);
  /* User flags layout */
  QVBoxLayout* user_flags_layout = new QVBoxLayout();
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
  QGroupBox* target_flags_box = new QGroupBox("Target Attribute Flags",this);
  /* Target flags */
  QRadioButton* target_flags_VITA = new QRadioButton("Vitality",this);
  QRadioButton* target_flags_QTDR = new QRadioButton("Quantum Drive (QD)",this);
  QRadioButton* target_flags_PHAG = new QRadioButton("Physical Aggression",this);
  QRadioButton* target_flags_PHFD = new QRadioButton("Physical Fortitude",this);
  QRadioButton* target_flags_THAG = new QRadioButton("Thermal Aggression",this);
  QRadioButton* target_flags_THFD = new QRadioButton("Thermal Fortitude",this);
  QRadioButton* target_flags_PRAG = new QRadioButton("Primal Aggression",this);
  QRadioButton* target_flags_PRFD = new QRadioButton("Primal Fortitude",this);
  QRadioButton* target_flags_POAG = new QRadioButton("Polar Aggression",this);
  QRadioButton* target_flags_POFD = new QRadioButton("Polar Fortitude",this);
  QRadioButton* target_flags_CHAG = new QRadioButton("Charged Aggression",this);
  QRadioButton* target_flags_CHFD = new QRadioButton("Charged Fortitude",this);
  QRadioButton* target_flags_CYAG = new QRadioButton("Cybernetic Aggression",this);
  QRadioButton* target_flags_CYFD = new QRadioButton("Cybernetic Fortitude",this);
  QRadioButton* target_flags_NIAG = new QRadioButton("Nihil Aggression",this);
  QRadioButton* target_flags_NIFD = new QRadioButton("Nihil Fortitude",this);
  QRadioButton* target_flags_MMNT = new QRadioButton("Momentum",this);
  QRadioButton* target_flags_LIMB = new QRadioButton("Limbertude",this);
  QRadioButton* target_flags_UNBR = new QRadioButton("Unbearability",this);
  QRadioButton* target_flags_MANN = new QRadioButton("Manna",this);
  QRadioButton* target_flags_NOAT = new QRadioButton("No Attribute",this);
  /* Target flags layout */
  QVBoxLayout* target_flags_layout = new QVBoxLayout();
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
  /* Add the target flags box to the right layout */
  radio_layout->addWidget(target_flags_box);
  right_layout->addLayout(boxes_layout);
  right_layout->addLayout(radio_layout);
}

EditorAction::~EditorAction()
{
}

EditorAction* EditorAction::clone()
{
  return this;
}

