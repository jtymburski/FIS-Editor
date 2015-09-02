/*******************************************************************************
 * Class Name: EditorItem
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Item
 ******************************************************************************/
#include "Database/EditorItem.h"
#include <QDebug>

/* Constant Implementation - see header file for descriptions */
const int EditorItem::kFRAME_SIZE = 200;
const int EditorItem::kMAX_PRESETS = 6;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

EditorItem::EditorItem(QWidget *parent) : QWidget(parent)
{
  dialog_anim = NULL;
  dialog_thumb = NULL;
  id = 0;
  skill_id = -1;
  skill_id_base = -1;

  /* Make sure there's one frame in the thumbnail */
  if(sprite_thumb_base.frameCount() == 0)
    sprite_thumb_base.setPath(0, "");

  /* Layout */
  createLayout();
  resetWorking();
  saveWorking();
}

/* Constructor function with id and name */
EditorItem::EditorItem(int id, QString name, QWidget* parent)
          : EditorItem(parent)
{
  setID(id);
  setName(name);
  saveWorking();
}

/* Copy constructor */
EditorItem::EditorItem(const EditorItem &source) : EditorItem()
{
  copySelf(source);
}

EditorItem::~EditorItem()
{
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/* Copy function, to be called by a copy or equal operator constructor */
void EditorItem::copySelf(const EditorItem &source)
{
  item_base = source.item_base;
  sprite_anim_base = source.sprite_anim_base;
  sprite_thumb_base = source.sprite_thumb_base;
  skill_id_base = source.skill_id_base;
  skill_total = source.skill_total;

  loadWorkingInfo();
}

/* Creates interface layout */
void EditorItem::createLayout()
{
  /* Layout */
  QGridLayout* layout = new QGridLayout(this);
  layout->setColumnStretch(7, 1);
  layout->setRowMinimumHeight(11, 15);
  layout->setRowStretch(10, 1);

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
  layout->addWidget(edit_name, 1, 1);

  /* Prefix */
  QLabel* lbl_prefix = new QLabel("Prefix", this);
  layout->addWidget(lbl_prefix, 1, 2);
  edit_prefix = new QLineEdit(this);
  connect(edit_prefix, SIGNAL(textEdited(QString)),
          this, SLOT(changedPrefix(QString)));
  layout->addWidget(edit_prefix, 1, 3);

  /* Brief Description */
  QLabel* lbl_brief_desc = new QLabel("Brief Description", this);
  layout->addWidget(lbl_brief_desc, 2, 0);
  edit_brief_desc = new QLineEdit(this);
  connect(edit_brief_desc, SIGNAL(textEdited(QString)),
          this, SLOT(changedBriefDescription(QString)));
  layout->addWidget(edit_brief_desc, 2, 1, 1, 3);

  /* Description */
  QLabel* lbl_desc = new QLabel("Description", this);
  layout->addWidget(lbl_desc, 3, 0);
  text_desc = new QTextEdit(this);
  connect(text_desc, SIGNAL(textChanged()), this, SLOT(changedDescription()));
  layout->addWidget(text_desc, 3, 1, 1, 3);

  /* Message */
  QLabel* lbl_msg = new QLabel("Use Message", this);
  layout->addWidget(lbl_msg, 4, 0);
  edit_msg = new QLineEdit(this);
  connect(edit_msg, SIGNAL(textEdited(QString)),
          this, SLOT(changedMessage(QString)));
  layout->addWidget(edit_msg, 4, 1, 1, 3);

  /* Tier */
  QLabel* lbl_tier = new QLabel("Tier", this);
  layout->addWidget(lbl_tier, 5, 0);
  combo_tier = new QComboBox(this);
  for(int i = 0; i <= (int)ItemTier::NONE; i++)
  {
    QString tier = QString::fromStdString(Helpers::tierToStr((ItemTier)i));
    tier = tier.toLower();
    if(tier.size() > 0)
      tier[0] = tier.at(0).toUpper();
    else
      tier = "None";
    combo_tier->addItem(tier);
  }
  connect(combo_tier, SIGNAL(currentIndexChanged(int)),
          this, SLOT(changedTier(int)));
  layout->addWidget(combo_tier, 5, 1);

  /* Use */
  QLabel* lbl_use = new QLabel("Use", this);
  layout->addWidget(lbl_use, 5, 2);
  combo_use = new QComboBox(this);
  QStringList use_list;
  use_list << "None" << "Always" << "Battle" << "Menu";
  combo_use->addItems(use_list);
  connect(combo_use, SIGNAL(currentIndexChanged(int)),
          this, SLOT(changedUse(int)));
  layout->addWidget(combo_use, 5, 3);

  /* Mass */
  QLabel* lbl_mass = new QLabel("Mass", this);
  layout->addWidget(lbl_mass, 6, 0);
  spin_mass = new QSpinBox(this);
  spin_mass->setMinimum(Item::kMIN_MASS);
  spin_mass->setMaximum(Item::kMAX_MASS);
  connect(spin_mass, SIGNAL(valueChanged(int)), this, SLOT(changedMass(int)));
  layout->addWidget(spin_mass, 6, 1);

  /* Value */
  QLabel* lbl_value = new QLabel("Value", this);
  layout->addWidget(lbl_value, 6, 2);
  spin_value = new QSpinBox(this);
  spin_value->setMaximum(Item::kMAX_VALUE);
  connect(spin_value, SIGNAL(valueChanged(int)), this, SLOT(changedValue(int)));
  layout->addWidget(spin_value, 6, 3);

  /* Skill */
  QLabel* lbl_skill = new QLabel("Skill", this);
  layout->addWidget(lbl_skill, 7, 0);
  combo_skill = new QComboBox(this);
  connect(combo_skill, SIGNAL(currentIndexChanged(int)),
          this, SLOT(changedSkill(int)));
  layout->addWidget(combo_skill, 7, 1, 1, 3);

  /* Thumbnail Frame */
  QLabel* lbl_thumb = new QLabel("Thumbnail", this);
  layout->addWidget(lbl_thumb, 8, 0, 1, 2, Qt::AlignHCenter);
  lbl_thumb_img = new QLabel(this);
  lbl_thumb_img->setMinimumSize(kFRAME_SIZE, kFRAME_SIZE);
  lbl_thumb_img->setStyleSheet("border: 1px solid #b9b5b2");
  lbl_thumb_img->setAlignment(Qt::AlignCenter);
  layout->addWidget(lbl_thumb_img, 9, 0, 1, 2);//, Qt::AlignHCenter);
  QPushButton* btn_thumb_click = new QPushButton(this);
  btn_thumb_click->setIcon(QIcon(":/images/icons/32_settings.png"));
  btn_thumb_click->setIconSize(QSize(24,24));
  btn_thumb_click->setMaximumSize(30, 30);
  connect(btn_thumb_click, SIGNAL(clicked()), this, SLOT(buttonThumbEdit()));
  layout->addWidget(btn_thumb_click, 9, 0, 1, 2,
                    Qt::AlignRight | Qt::AlignTop);

  /* Animation Frame */
  QLabel* lbl_animation = new QLabel("Animation", this);
  layout->addWidget(lbl_animation, 8, 2, 1, 2, Qt::AlignHCenter);
  lbl_anim_img = new QLabel(this);
  lbl_anim_img->setMinimumSize(kFRAME_SIZE, kFRAME_SIZE);
  lbl_anim_img->setStyleSheet("border: 1px solid #b9b5b2");
  lbl_anim_img->setAlignment(Qt::AlignCenter);
  layout->addWidget(lbl_anim_img, 9, 2, 1, 2);//, Qt::AlignHCenter);
  btn_anim_click = new QPushButton(this);
  btn_anim_click->setIcon(QIcon(":/images/icons/32_settings.png"));
  btn_anim_click->setIconSize(QSize(24,24));
  btn_anim_click->setMaximumSize(30, 30);
  connect(btn_anim_click, SIGNAL(clicked()), this, SLOT(buttonAnimEdit()));
  layout->addWidget(btn_anim_click, 9, 2, 1, 2,
                    Qt::AlignRight | Qt::AlignTop);

  /* Flags GROUP */
  QGroupBox* box_flags = new QGroupBox("Flags", this);
  QVBoxLayout* layout_flags = new QVBoxLayout(box_flags);
  radio_f_nocat = new QRadioButton("No Category", this);
  connect(radio_f_nocat, SIGNAL(clicked()), this, SLOT(changedFlagCat()));
  layout_flags->addWidget(radio_f_nocat);
  radio_f_consumed = new QRadioButton("Consumed", this);
  connect(radio_f_consumed, SIGNAL(clicked()), this, SLOT(changedFlagCat()));
  layout_flags->addWidget(radio_f_consumed);
  radio_f_material = new QRadioButton("Material", this);
  connect(radio_f_material, SIGNAL(clicked()), this, SLOT(changedFlagCat()));
  layout_flags->addWidget(radio_f_material);
  radio_f_key = new QRadioButton("Key Item");
  connect(radio_f_key, SIGNAL(clicked()), this, SLOT(changedFlagCat()));
  layout_flags->addWidget(radio_f_key);
  /* -- */
  QFrame* line1 = new QFrame(this);
  line1->setFrameShape(QFrame::HLine);
  layout_flags->addWidget(line1);
  /* -- */
  chk_f_statalter = new QCheckBox("Stat Altering",this);
  connect(chk_f_statalter, SIGNAL(stateChanged(int)),
          this, SLOT(changedFlags(int)));
  layout_flags->addWidget(chk_f_statalter);
  chk_f_skilllearn = new QCheckBox("Skill Learning",this);
  connect(chk_f_skilllearn, SIGNAL(stateChanged(int)),
          this, SLOT(changedFlags(int)));
  layout_flags->addWidget(chk_f_skilllearn);
  layout->addWidget(box_flags, 0, 4, 5, 1);

  /* Material GROUP */
  box_material = new QGroupBox("Material", this);
  QVBoxLayout* layout_material = new QVBoxLayout(box_material);
  QButtonGroup* group_element = new QButtonGroup(box_material);
  radio_m_physical = new QRadioButton("Physical",this);
  group_element->addButton(radio_m_physical);
  connect(radio_m_physical, SIGNAL(clicked()), this, SLOT(changedElement()));
  layout_material->addWidget(radio_m_physical);
  radio_m_fire = new QRadioButton("Fire", this);
  group_element->addButton(radio_m_fire);
  connect(radio_m_fire, SIGNAL(clicked()), this, SLOT(changedElement()));
  layout_material->addWidget(radio_m_fire);
  radio_m_forest = new QRadioButton("Forest", this);
  group_element->addButton(radio_m_forest);
  connect(radio_m_forest, SIGNAL(clicked()), this, SLOT(changedElement()));
  layout_material->addWidget(radio_m_forest);
  radio_m_ice = new QRadioButton("Ice", this);
  group_element->addButton(radio_m_ice);
  connect(radio_m_ice, SIGNAL(clicked()), this, SLOT(changedElement()));
  layout_material->addWidget(radio_m_ice);
  radio_m_electric = new QRadioButton("Electric", this);
  group_element->addButton(radio_m_electric);
  connect(radio_m_electric, SIGNAL(clicked()), this, SLOT(changedElement()));
  layout_material->addWidget(radio_m_electric);
  radio_m_digital = new QRadioButton("Digital", this);
  group_element->addButton(radio_m_digital);
  connect(radio_m_digital, SIGNAL(clicked()), this, SLOT(changedElement()));
  layout_material->addWidget(radio_m_digital);
  radio_m_nihil = new QRadioButton("Void", this);
  group_element->addButton(radio_m_nihil);
  connect(radio_m_nihil, SIGNAL(clicked()), this, SLOT(changedElement()));
  layout_material->addWidget(radio_m_nihil);
  /* -- */
  QFrame* line2 = new QFrame(this);
  line2->setFrameShape(QFrame::HLine);
  layout_material->addWidget(line2);
  /* -- */
  QButtonGroup* group_material = new QButtonGroup(box_material);
  radio_m_wood = new QRadioButton("Wood",this);
  group_material->addButton(radio_m_wood);
  connect(radio_m_wood, SIGNAL(clicked()), this, SLOT(changedMaterial()));
  layout_material->addWidget(radio_m_wood);
  radio_m_steel = new QRadioButton("Steel",this);
  group_material->addButton(radio_m_steel);
  connect(radio_m_steel, SIGNAL(clicked()), this, SLOT(changedMaterial()));
  layout_material->addWidget(radio_m_steel);
  radio_m_brass = new QRadioButton("Brass",this);
  group_material->addButton(radio_m_brass);
  connect(radio_m_brass, SIGNAL(clicked()), this, SLOT(changedMaterial()));
  layout_material->addWidget(radio_m_brass);
  radio_m_titanium = new QRadioButton("Titanium",this);
  group_material->addButton(radio_m_titanium);
  connect(radio_m_titanium, SIGNAL(clicked()), this, SLOT(changedMaterial()));
  layout_material->addWidget(radio_m_titanium);
  radio_m_graphene = new QRadioButton("Graphene",this);
  group_material->addButton(radio_m_graphene);
  connect(radio_m_graphene, SIGNAL(clicked()), this, SLOT(changedMaterial()));
  layout_material->addWidget(radio_m_graphene);
  layout->addWidget(box_material, 0, 5, 8, 1);

  /* Preset List for Stats */
  QStringList stat_presets;
  stat_presets << "Weak" << "Not As Weak" << "Normal" << "Medium"
               << "Top" << "Boss" << "Custom";

  /* Buff Set GROUP */
  box_buff = new QGroupBox("Buff Set", this);
  QGridLayout* layout_buff = new QGridLayout(box_buff);
  combo_buff_preset = new QComboBox(this);
  combo_buff_preset->addItems(stat_presets);
  connect(combo_buff_preset, SIGNAL(currentIndexChanged(int)),
          this, SLOT(statBuffPreset(int)));
  layout_buff->addWidget(combo_buff_preset, 0, 0, 1, 2);
  for(int i = 0; i < (int)Attribute::NONE; i++)
  {
    QString att = QString::fromStdString(Helpers::attributeToStr((Attribute)i));
    QLabel* lbl = new QLabel(att, this);
    layout_buff->addWidget(lbl, i + 1, 0);
    spin_buffs.push_back(new QSpinBox(this));
    spin_buffs.last()->setMinimum(AttributeSet::kMIN_P_VALUE);
    spin_buffs.last()->setMaximum(AttributeSet::kMAX_VALUE);
    connect(spin_buffs.last(), SIGNAL(valueChanged(int)),
            this, SLOT(changedBuffSet(int)));
    layout_buff->addWidget(spin_buffs.last(), i + 1, 1);
  }
  layout->addWidget(box_buff, 0, 6, 11, 1);

  /* Buttons */
  QPushButton* btn_reset = new QPushButton("Reset", this);
  connect(btn_reset, SIGNAL(clicked()), this, SLOT(btnReset()));
  layout->addWidget(btn_reset, 12, 5);
  QPushButton* btn_save = new QPushButton("Save", this);
  connect(btn_save, SIGNAL(clicked()), this, SLOT(btnSave()));
  layout->addWidget(btn_save, 12, 6);
}

/* Loads working info into UI objects */
void EditorItem::loadWorkingInfo()
{
  /* ID */
  edit_id->setText(QString::number(id));

  /* Name */
  edit_name->setText(QString::fromStdString(item_curr.getName()));

  /* Prefix */
  edit_prefix->setText(QString::fromStdString(item_curr.getPrefix()));

  /* Brief Description */
  edit_brief_desc->setText(QString::fromStdString(
                                          item_curr.getBriefDescription()));

  /* Description */
  text_desc->setText(QString::fromStdString(item_curr.getDescription()));

  /* Message */
  edit_msg->setText(QString::fromStdString(item_curr.getUseMessage()));

  /* Tier */
  combo_tier->setCurrentIndex((int)item_curr.getItemTier());

  /* Use */
  combo_use->setCurrentIndex((int)item_curr.getOccasion());

  /* Mass */
  spin_mass->setValue(item_curr.getMass());

  /* Value */
  spin_value->setValue(item_curr.getValue());

  /* Skills */
  combo_skill->blockSignals(true);
  combo_skill->clear();
  combo_skill->addItem("None");
  int skill_index = -1;
  for(int i = 0; i < skill_total.size(); i++)
  {
    combo_skill->addItem(skill_total[i]->getNameList());
    if(skill_total[i]->getID() == skill_id)
      skill_index = i + 1;
  }
  if(skill_index > 0)
    combo_skill->setCurrentIndex(skill_index);
  else
    skill_id = -1;
  combo_skill->blockSignals(false);

  /* Animation and Thumbnail */
  updateAnimation();
  updateThumb();

  /* Flags */
  if(item_curr.getFlag(ItemFlags::CONSUMED))
    radio_f_consumed->setChecked(true);
  else if(item_curr.getFlag(ItemFlags::MATERIAL))
    radio_f_material->setChecked(true);
  else if(item_curr.getFlag(ItemFlags::KEY_ITEM))
    radio_f_key->setChecked(true);
  else
    radio_f_nocat->setChecked(true);
  if(radio_f_consumed->isChecked() || radio_f_nocat->isChecked())
  {
    bool statalter = item_curr.getFlag(ItemFlags::STAT_ALTERING);
    bool skilllearn = item_curr.getFlag(ItemFlags::SKILL_LEARNING);

    chk_f_statalter->setChecked(statalter);
    chk_f_skilllearn->setChecked(skilllearn);
  }
  else
  {
    chk_f_statalter->setChecked(false);
    chk_f_skilllearn->setChecked(false);
  }

  /* Material - Element */
  if(item_curr.getMaterial(Material::FIRE))
    radio_m_fire->setChecked(true);
  else if(item_curr.getMaterial(Material::FOREST))
    radio_m_forest->setChecked(true);
  else if(item_curr.getMaterial(Material::ICE))
    radio_m_ice->setChecked(true);
  else if(item_curr.getMaterial(Material::ELECTRIC))
    radio_m_electric->setChecked(true);
  else if(item_curr.getMaterial(Material::DIGITAL))
    radio_m_digital->setChecked(true);
  else if(item_curr.getMaterial(Material::NIHIL))
    radio_m_nihil->setChecked(true);
  else
    radio_m_physical->setChecked(true);
  /* Material - Physical */
  if(item_curr.getMaterial(Material::WOODEN))
    radio_m_wood->setChecked(true);
  else if(item_curr.getMaterial(Material::STEEL))
    radio_m_steel->setChecked(true);
  else if(item_curr.getMaterial(Material::BRASS))
    radio_m_brass->setChecked(true);
  else if(item_curr.getMaterial(Material::TITANIUM))
    radio_m_titanium->setChecked(true);
  else if(item_curr.getMaterial(Material::GRAPHENE))
    radio_m_graphene->setChecked(true);

  /* Buff Set */
  for(int i = 0; i < (int)Attribute::NONE; i++)
  {
    spin_buffs[i]->blockSignals(true);
    spin_buffs[i]->setValue(item_curr.getStats().getStat(i));
    spin_buffs[i]->blockSignals(false);
  }

  /* Update Enables/Disables */
  updateConnected();
}

/* Updates connected widgets with enabled and disabled statuses */
void EditorItem::updateConnected()
{
  /* ---- Consumable or No Category set ---- */
  if(radio_f_consumed->isChecked() || radio_f_nocat->isChecked())
  {
    chk_f_skilllearn->setEnabled(true);
    combo_use->setEnabled(true);
    if(chk_f_skilllearn->isChecked())
    {
      combo_use->setCurrentIndex((int)ActionOccasion::MENU);
      combo_use->setDisabled(true);
    }

    /* Check use */
    if(item_curr.getOccasion() == ActionOccasion::ALWAYS ||
       item_curr.getOccasion() == ActionOccasion::BATTLE ||
       item_curr.getOccasion() == ActionOccasion::MENU)
    {
      combo_skill->setEnabled(true);
      edit_msg->setEnabled(true);
    }
    else
    {
      combo_skill->setDisabled(true);
      edit_msg->setDisabled(true);
    }

    /* Animation */
    btn_anim_click->setEnabled(true);
    lbl_anim_img->setEnabled(true);
  }
  else
  {
    chk_f_skilllearn->setChecked(false);
    chk_f_skilllearn->setDisabled(true);
    combo_skill->setDisabled(true);
    combo_use->setDisabled(true);
    edit_msg->setDisabled(true);

    /* Animation */
    buttonAnimEdit(true);
    btn_anim_click->setDisabled(true);
    lbl_anim_img->setDisabled(true);
  }

  /* ---- Just Consumable ---- */
  if(radio_f_consumed->isChecked())
  {
    chk_f_statalter->setEnabled(true);
    box_buff->setEnabled(chk_f_statalter->isChecked());
  }
  else
  {
    chk_f_statalter->setChecked(false);
    box_buff->setEnabled(false);
    chk_f_statalter->setDisabled(true);
  }

  /* ---- Just Materials ---- */
  if(radio_f_material->isChecked())
  {
    box_material->setEnabled(true);
  }
  else
  {
    box_material->setDisabled(true);
  }

  /* ---- Just Key Items ---- */
  if(radio_f_key->isChecked())
  {
    spin_mass->setValue(0);
    spin_mass->setDisabled(true);
    spin_value->setValue(0);
    spin_value->setDisabled(true);
  }
  else
  {
    spin_mass->setEnabled(true);
    spin_value->setEnabled(true);
  }
}

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/

/* Button Triggers */
void EditorItem::buttonAnimEdit(bool clean_only)
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

/* Button Triggers */
void EditorItem::btnReset()
{
  resetWorking();
}

/* Button Triggers */
void EditorItem::btnSave()
{
  saveWorking();
}

/* Button Triggers */
void EditorItem::buttonThumbEdit(bool clean_only)
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

/* Widget Change Triggers */
void EditorItem::changedBriefDescription(QString brief)
{
  item_curr.setBriefDescription(brief.toStdString());
}

/* Widget Change Triggers */
void EditorItem::changedBuffSet(int)
{
  combo_buff_preset->setCurrentIndex(kMAX_PRESETS);
  AttributeSet set = item_curr.getStats();
  for(int i = 0; i < spin_buffs.size(); i++)
    set.setStat((Attribute)i, spin_buffs[i]->value());
  item_curr.setBuffSet(set);
}

/* Widget Change Triggers */
void EditorItem::changedDescription()
{
  item_curr.setDescription(text_desc->toPlainText().simplified().toStdString());
}

/* Widget Change Triggers */
void EditorItem::changedElement()
{
  /* Unset the old material elements */
  item_curr.setMaterial(Material::PHYSICAL, false);
  item_curr.setMaterial(Material::FIRE, false);
  item_curr.setMaterial(Material::FOREST, false);
  item_curr.setMaterial(Material::ICE, false);
  item_curr.setMaterial(Material::ELECTRIC, false);
  item_curr.setMaterial(Material::DIGITAL, false);
  item_curr.setMaterial(Material::NIHIL, false);

  /* Set the new material element */
  if(radio_m_physical->isChecked())
    item_curr.setMaterial(Material::PHYSICAL, true);
  else if(radio_m_fire->isChecked())
    item_curr.setMaterial(Material::FIRE, true);
  else if(radio_m_forest->isChecked())
    item_curr.setMaterial(Material::FOREST, true);
  else if(radio_m_ice->isChecked())
    item_curr.setMaterial(Material::ICE, true);
  else if(radio_m_electric->isChecked())
    item_curr.setMaterial(Material::ELECTRIC, true);
  else if(radio_m_digital->isChecked())
    item_curr.setMaterial(Material::DIGITAL, true);
  else if(radio_m_nihil->isChecked())
    item_curr.setMaterial(Material::NIHIL, true);
}

/* Widget Change Triggers */
void EditorItem::changedFlagCat()
{
  /* Unset the old flag category */
  item_curr.setFlag(ItemFlags::CONSUMED, false);
  item_curr.setFlag(ItemFlags::MATERIAL, false);
  item_curr.setFlag(ItemFlags::KEY_ITEM, false);
  item_curr.setFlag(ItemFlags::NO_CATEGORY, false);

  /* Set the new flag category */
  if(radio_f_consumed->isChecked())
    item_curr.setFlag(ItemFlags::CONSUMED, true);
  else if(radio_f_material->isChecked())
    item_curr.setFlag(ItemFlags::MATERIAL, true);
  else if(radio_f_key->isChecked())
    item_curr.setFlag(ItemFlags::KEY_ITEM, true);
  else
    item_curr.setFlag(ItemFlags::NO_CATEGORY, true);

  /* Update Enables/Disables */
  updateConnected();
}

/* Widget Change Triggers */
void EditorItem::changedFlags(int)
{
  bool previous_stat = item_curr.getFlag(ItemFlags::STAT_ALTERING);

  /* Set current item data */
  item_curr.setFlag(ItemFlags::STAT_ALTERING, chk_f_statalter->isChecked());
  item_curr.setFlag(ItemFlags::SKILL_LEARNING, chk_f_skilllearn->isChecked());

  /* If both are set, unset one */
  if(item_curr.getFlag(ItemFlags::STAT_ALTERING) &&
     item_curr.getFlag(ItemFlags::SKILL_LEARNING))
  {
    if(previous_stat)
    {
      chk_f_statalter->blockSignals(true);
      chk_f_statalter->setChecked(false);
      item_curr.setFlag(ItemFlags::STAT_ALTERING, false);
      chk_f_statalter->blockSignals(false);
    }
    else
    {
      chk_f_skilllearn->blockSignals(true);
      chk_f_skilllearn->setChecked(false);
      item_curr.setFlag(ItemFlags::SKILL_LEARNING, false);
      chk_f_skilllearn->blockSignals(false);
    }
  }

  /* Update Enables/Disables */
  updateConnected();
}

/* Widget Change Triggers */
void EditorItem::changedMass(int mass)
{
  if(mass >= 0)
    item_curr.setMass(mass);
}

/* Widget Change Triggers */
void EditorItem::changedMaterial()
{
  /* Unset the old material */
  item_curr.setMaterial(Material::WOODEN, false);
  item_curr.setMaterial(Material::STEEL, false);
  item_curr.setMaterial(Material::BRASS, false);
  item_curr.setMaterial(Material::TITANIUM, false);
  item_curr.setMaterial(Material::GRAPHENE, false);

  /* Sets the new material */
  if(radio_m_wood->isChecked())
    item_curr.setMaterial(Material::WOODEN, true);
  else if(radio_m_steel->isChecked())
    item_curr.setMaterial(Material::STEEL, true);
  else if(radio_m_brass->isChecked())
    item_curr.setMaterial(Material::BRASS, true);
  else if(radio_m_titanium->isChecked())
    item_curr.setMaterial(Material::TITANIUM, true);
  else if(radio_m_graphene->isChecked())
    item_curr.setMaterial(Material::GRAPHENE, true);
}

/* Widget Change Triggers */
void EditorItem::changedMessage(QString message)
{
  item_curr.setUseMessage(message.toStdString());
}

/* Widget Change Triggers */
void EditorItem::changedName(QString name)
{
  setName(name);
}

/* Widget Change Triggers */
void EditorItem::changedPrefix(QString prefix)
{
  item_curr.setPrefix(prefix.toStdString());
}

/* Widget Change Triggers */
void EditorItem::changedSkill(int index)
{
  if(index > 0)
    skill_id = skill_total[index-1]->getID();
  else
    skill_id = -1;
}

/* Widget Change Triggers */
void EditorItem::changedTier(int index)
{
  item_curr.setItemTier((ItemTier)index);
}

/* Widget Change Triggers */
void EditorItem::changedUse(int index)
{
  item_curr.setOccasion((ActionOccasion)index);

  /* Update Enables/Disables */
  updateConnected();
}

/* Widget Change Triggers */
void EditorItem::changedValue(int value)
{
  if(value >= 0)
    item_curr.setValue(value);
}

/* Stat Preset Trigger */
void EditorItem::statBuffPreset(int index)
{
  if(index >= 0 && index < kMAX_PRESETS)
  {
    AttributeSet set = item_curr.getStats();
    set.buildAsPreset(index);
    item_curr.setBuffSet(set);
    loadWorkingInfo();
  }
}

/* Update calls for frames and sprites */
void EditorItem::updateAnimation()
{
  int img_size = kFRAME_SIZE - 5;
  QImage original = sprite_anim.getImage(0);

  if(original.width() > img_size || original.height() > img_size)
  {
    QPixmap scaled_image = sprite_anim.getPixmap(0, img_size, img_size);
    lbl_anim_img->setPixmap(scaled_image);
  }
  else
  {
    QPixmap orig_image =
                 sprite_anim.getPixmap(0, original.width(), original.height());
    lbl_anim_img->setPixmap(orig_image);
  }
}

/* Update calls for frames and sprites */
void EditorItem::updateThumb()
{
  int img_size = kFRAME_SIZE - 5;
  QImage original = sprite_thumb.getImage(0);

  if(original.width() > img_size || original.height() > img_size)
  {
    QImage scaled_image = original.scaled(img_size, img_size,
                                          Qt::KeepAspectRatio);
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

/* Returns the ID of the item */
int EditorItem::getID() const
{
  return id;
}

/* Returns the name of the item */
QString EditorItem::getName() const
{
  return QString::fromStdString(item_curr.getName());
}

/* Returns the name of the item for listing */
QString EditorItem::getNameList()
{
  return EditorHelpers::getListString(getID(), getName());
}

/* Loads the object data */
void EditorItem::load(XmlData data, int index)
{
  // TODO: Implementation
}

/* Resets the working set trigger */
void EditorItem::resetWorking()
{
  buttonAnimEdit(true);
  buttonThumbEdit(true);

  /* Fix the data */
  item_curr = item_base;
  sprite_anim = sprite_anim_base;
  sprite_thumb = sprite_thumb_base;
  skill_id = skill_id_base;

  /* Properly load the information */
  setName(QString::fromStdString(item_curr.getName()));
  combo_buff_preset->setCurrentIndex(kMAX_PRESETS);
  loadWorkingInfo();
}

/* Saves the object data */
void EditorItem::save(FileHandler* fh, bool game_only)
{
  // TODO: Implementation
}

/* Saves the working set trigger */
void EditorItem::saveWorking()
{
  buttonAnimEdit(true);
  buttonThumbEdit(true);

  /* Save the data */
  item_base = item_curr;
  sprite_anim_base = sprite_anim;
  sprite_thumb_base = sprite_thumb;
  skill_id_base = skill_id;
}

/* Sets the ID of the item */
void EditorItem::setID(int id)
{
  this->id = id;
  edit_id->setText(QString::number(id));
}

/* Sets the name of the item */
void EditorItem::setName(QString name)
{
  item_curr.setName(name.toStdString());
  edit_name->setText(name);
  emit nameChange(name);
}

/* Update skills */
void EditorItem::updateSkills(QVector<EditorSkill*> skills)
{
  skill_total = skills;
  loadWorkingInfo();
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/* The copy operator */
EditorItem& EditorItem::operator= (const EditorItem &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
