/*******************************************************************************
 * Class Name: EditorItem
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Item
 ******************************************************************************/
#include "Database/EditorItem.h"

EditorItem::EditorItem(QWidget *parent) : QWidget(parent)
{
  createLayout();
}

/* Constructor function with id and name */
EditorItem::EditorItem(int id, QString name, QWidget* parent)
          : EditorItem(parent)
{
  setID(id);
  setName(name);
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
  // TODO: Implementation
  //item = source.item;
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
  layout->addWidget(edit_name, 1, 1);

  /* Prefix */
  QLabel* lbl_prefix = new QLabel("Prefix", this);
  layout->addWidget(lbl_prefix, 1, 2);
  edit_prefix = new QLineEdit(this);
  layout->addWidget(edit_prefix, 1, 3);

  /* Brief Description */
  QLabel* lbl_brief_desc = new QLabel("Brief Description", this);
  layout->addWidget(lbl_brief_desc, 2, 0);
  edit_brief_desc = new QLineEdit(this);
  layout->addWidget(edit_brief_desc, 2, 1, 1, 3);

  /* Description */
  QLabel* lbl_desc = new QLabel("Description", this);
  layout->addWidget(lbl_desc, 3, 0);
  text_desc = new QTextEdit(this);
  layout->addWidget(text_desc, 3, 1, 1, 3);

  /* Message */
  QLabel* lbl_msg = new QLabel("Message", this);
  layout->addWidget(lbl_msg, 4, 0);
  edit_msg = new QLineEdit(this);
  layout->addWidget(edit_msg, 4, 1, 1, 3);

  /* Tier */
  QLabel* lbl_tier = new QLabel("Tier", this);
  layout->addWidget(lbl_tier, 5, 0);
  combo_tier = new QComboBox(this);
  layout->addWidget(combo_tier, 5, 1);

  /* Use */
  QLabel* lbl_use = new QLabel("Use", this);
  layout->addWidget(lbl_use, 5, 2);
  combo_use = new QComboBox(this);
  layout->addWidget(combo_use, 5, 3);

  /* Durability */
  QLabel* lbl_dura = new QLabel("Durability", this);
  layout->addWidget(lbl_dura, 6, 0);
  spin_dura = new QSpinBox(this);
  layout->addWidget(spin_dura, 6, 1);

  /* Mass */
  QLabel* lbl_mass = new QLabel("Mass", this);
  layout->addWidget(lbl_mass, 6, 2);
  spin_mass = new QSpinBox(this);
  layout->addWidget(spin_mass, 6, 3);

  /* Value */
  QLabel* lbl_value = new QLabel("Value", this);
  layout->addWidget(lbl_value, 7, 0);
  spin_value = new QSpinBox(this);
  layout->addWidget(spin_value, 7, 1);

  /* Thumbnail Frame */
  QLabel* lbl_thumb = new QLabel("Thumbnail", this);
  layout->addWidget(lbl_thumb, 8, 0, 1, 2, Qt::AlignHCenter);
  lbl_thumb_img = new QLabel(this);
  lbl_thumb_img->setMinimumSize(200, 200);
  lbl_thumb_img->setStyleSheet("border: 1px solid #b9b5b2");
  lbl_thumb_img->setAlignment(Qt::AlignCenter);
  layout->addWidget(lbl_thumb_img, 9, 0, 1, 2);//, Qt::AlignHCenter);
  QPushButton* btn_thumb_click = new QPushButton(this);
  btn_thumb_click->setIcon(QIcon(":/images/icons/32_settings.png"));
  btn_thumb_click->setIconSize(QSize(24,24));
  btn_thumb_click->setMaximumSize(30, 30);
  //connect(btn_thumb_click, SIGNAL(clicked()), this, SLOT(buttonThumbEdit()));
  layout->addWidget(btn_thumb_click, 9, 0, 1, 2,
                    Qt::AlignRight | Qt::AlignTop);

  /* Animation Frame */
  QLabel* lbl_animation = new QLabel("Animation", this);
  layout->addWidget(lbl_animation, 8, 2, 1, 2, Qt::AlignHCenter);
  lbl_anim_img = new QLabel(this);
  lbl_anim_img->setMinimumSize(200, 200);
  lbl_anim_img->setStyleSheet("border: 1px solid #b9b5b2");
  lbl_anim_img->setAlignment(Qt::AlignCenter);
  layout->addWidget(lbl_anim_img, 9, 2, 1, 2);//, Qt::AlignHCenter);
  QPushButton* btn_anim_click = new QPushButton(this);
  btn_anim_click->setIcon(QIcon(":/images/icons/32_settings.png"));
  btn_anim_click->setIconSize(QSize(24,24));
  btn_anim_click->setMaximumSize(30, 30);
  //connect(btn_anim_click, SIGNAL(clicked()), this, SLOT(buttonAnimEdit()));
  layout->addWidget(btn_anim_click, 9, 2, 1, 2,
                    Qt::AlignRight | Qt::AlignTop);

  /* Material GROUP */
  QGroupBox* box_material = new QGroupBox("Material", this);
  QVBoxLayout* layout_material = new QVBoxLayout(box_material);
  radio_wood = new QRadioButton("Wood",this);
  layout_material->addWidget(radio_wood);
  radio_steel = new QRadioButton("Steel",this);
  layout_material->addWidget(radio_steel);
  radio_brass = new QRadioButton("Brass",this);
  layout_material->addWidget(radio_brass);
  radio_titanium = new QRadioButton("Titanium",this);
  layout_material->addWidget(radio_titanium);
  radio_graphene = new QRadioButton("Graphene",this);
  layout_material->addWidget(radio_graphene);
  radio_physical = new QRadioButton("Physical",this);
  layout_material->addWidget(radio_physical);
  radio_nonphysical = new QRadioButton("Non-Physical",this);
  layout_material->addWidget(radio_nonphysical);
  chk_m_fire = new QCheckBox("Fire",this);
  layout_material->addWidget(chk_m_fire);
  chk_m_forest = new QCheckBox("Forest",this);
  layout_material->addWidget(chk_m_forest);
  chk_m_ice = new QCheckBox("Ice",this);
  layout_material->addWidget(chk_m_ice);
  chk_m_electric = new QCheckBox("Electric",this);
  layout_material->addWidget(chk_m_electric);
  chk_m_digital = new QCheckBox("Digital",this);
  layout_material->addWidget(chk_m_digital);
  chk_m_nihil = new QCheckBox("Nihil",this);
  layout_material->addWidget(chk_m_nihil);
  layout->addWidget(box_material, 0, 4, 8, 1);

  /* Flags GROUP */
  QGroupBox* box_flags = new QGroupBox("Flags", this);
  QVBoxLayout* layout_flags = new QVBoxLayout(box_flags);
  chk_f_consumed = new QCheckBox("Consumed",this);
  layout_flags->addWidget(chk_f_consumed);
  chk_f_offensive = new QCheckBox("Offensive",this);
  layout_flags->addWidget(chk_f_offensive);
  chk_f_defensive = new QCheckBox("Defensive",this);
  layout_flags->addWidget(chk_f_defensive);
  chk_f_equip = new QCheckBox("Equipment",this);
  layout_flags->addWidget(chk_f_equip);
  chk_f_bubby = new QCheckBox("B.U.B.B.I.E.",this);
  layout_flags->addWidget(chk_f_bubby);
  chk_f_key = new QCheckBox("Key Item",this);
  layout_flags->addWidget(chk_f_key);
  chk_f_material = new QCheckBox("Material",this);
  layout_flags->addWidget(chk_f_material);
  chk_f_gene = new QCheckBox("Gene Composition",this);
  layout_flags->addWidget(chk_f_gene);
  chk_f_nocat = new QCheckBox("No Category",this);
  layout_flags->addWidget(chk_f_nocat);
  chk_f_statalter = new QCheckBox("Stat Altering",this);
  layout_flags->addWidget(chk_f_statalter);
  chk_f_skilllearn = new QCheckBox("Skill Learning",this);
  layout_flags->addWidget(chk_f_skilllearn);
  chk_f_healing = new QCheckBox("Healing Item",this);
  layout_flags->addWidget(chk_f_healing);
  chk_f_relieving = new QCheckBox("Relieving Item",this);
  layout_flags->addWidget(chk_f_relieving);
  chk_f_money = new QCheckBox("Money",this);
  layout_flags->addWidget(chk_f_money);
  layout->addWidget(box_flags, 0, 5, 8, 1);

  /* Preset List for Stats */
  QStringList stat_presets;
  stat_presets << "Weak" << "Not As Weak" << "Normal" << "Medium"
               << "Top" << "Boss" << "Custom";

  /* Buff Set GROUP */
  QGroupBox* box_buff = new QGroupBox("Buff Set", this);
  QGridLayout* layout_buff = new QGridLayout(box_buff);
  combo_buff_preset = new QComboBox(this);
  combo_buff_preset->addItems(stat_presets);
  layout_buff->addWidget(combo_buff_preset, 0, 0, 1, 2);
  for(int i = 0; i < (int)Attribute::NONE; i++)
  {
    QString att = QString::fromStdString(Helpers::attributeToStr((Attribute)i));
    QLabel* lbl = new QLabel(att, this);
    layout_buff->addWidget(lbl, i + 1, 0);
    spin_buffs.push_back(new QSpinBox(this));
    spin_buffs.last()->setMinimum(AttributeSet::kMIN_P_VALUE);
    spin_buffs.last()->setMaximum(AttributeSet::kMAX_VALUE);
    layout_buff->addWidget(spin_buffs.last(), i + 1, 1);
  }
  layout->addWidget(box_buff, 0, 6, 11, 1);

  /* Buttons */
  QPushButton* btn_reset = new QPushButton("Reset", this);
  layout->addWidget(btn_reset, 12, 5);
  QPushButton* btn_save = new QPushButton("Save", this);
  layout->addWidget(btn_save, 12, 6);
}

/* Loads working info into UI objects */
void EditorItem::loadWorkingInfo()
{
  // TODO: Implementation
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
  // TODO: Implementation
}

/* Saves the object data */
void EditorItem::save(FileHandler* fh, bool game_only)
{
  // TODO: Implementation
}

/* Saves the working set trigger */
void EditorItem::saveWorking()
{
  // TODO: Implementation
}

/* Sets the ID of the item */
void EditorItem::setID(int id)
{
  this->id = id;
}

/* Sets the name of the item */
void EditorItem::setName(QString name)
{
  item_curr.setName(name.toStdString());
  emit nameChange(name);
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
