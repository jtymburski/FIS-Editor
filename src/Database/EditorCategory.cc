/*******************************************************************************
 * Class Name: EditorCategory
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: The category that defines persons and what they are capable of.
 ******************************************************************************/
#include "Database/EditorCategory.h"
#include <QDebug>

/* Constant Implementation - see header file for descriptions */
const int EditorCategory::kMAX_PRESETS = 6;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

EditorCategory::EditorCategory(QWidget *parent) : QWidget(parent)
{
  createLayout();
  loadWorkingInfo();
}

/* Constructor function with id and name */
EditorCategory::EditorCategory(int id, QString name, QWidget* parent)
              : EditorCategory(parent)
{
  setID(id);
  setName(name);
}

/* Copy constructor */
EditorCategory::EditorCategory(const EditorCategory &source) : EditorCategory()
{
  copySelf(source);
}

EditorCategory::~EditorCategory()
{
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/* Copy function, to be called by a copy or equal operator constructor */
void EditorCategory::copySelf(const EditorCategory &source)
{
  //category = source.category;
}

/* Creates interface layout */
void EditorCategory::createLayout()
{
  /* Layout */
  QGridLayout* layout = new QGridLayout(this);
  layout->setColumnStretch(7, 1);
  layout->setRowMinimumHeight(7, 25);
  layout->setRowStretch(6, 1);

  /* ID */
  QLabel* lbl_id = new QLabel("ID", this);
  layout->addWidget(lbl_id, 0, 0);
  edit_id = new QLineEdit(this);
  edit_id->setDisabled(true);
  layout->addWidget(edit_id, 0, 1, 1, 2);

  /* Name */
  QLabel* lbl_name = new QLabel("Name", this);
  layout->addWidget(lbl_name, 1, 0);
  edit_name = new QLineEdit(this);
  connect(edit_name, SIGNAL(textEdited(QString)),
          this, SLOT(changedName(QString)));
  layout->addWidget(edit_name, 1, 1, 1, 2);

  /* Denonym */
  QLabel* lbl_denonym = new QLabel("Denonym", this);
  layout->addWidget(lbl_denonym, 2, 0);
  edit_denonym = new QLineEdit(this);
  connect(edit_denonym, SIGNAL(textEdited(QString)),
          this, SLOT(changedDenonym(QString)));
  layout->addWidget(edit_denonym, 2, 1, 1, 2);

  /* Description */
  QLabel* lbl_desc = new QLabel("Description", this);
  layout->addWidget(lbl_desc, 3, 0);
  text_desc = new QTextEdit(this);
  connect(text_desc, SIGNAL(textChanged()), this, SLOT(changedDescription()));
  layout->addWidget(text_desc, 3, 1, 1, 2);

  /* Vita Regen */
  QLabel* lbl_vita = new QLabel("Vita Regen", this);
  layout->addWidget(lbl_vita, 4, 0);
  combo_vita = new QComboBox(this);
  for(int i = 0; i <= (int)RegenRate::GRAND; i++)
    combo_vita->addItem(QString::fromStdString(
                                      Helpers::regenRateToStr((RegenRate)i)));
  connect(combo_vita, SIGNAL(currentIndexChanged(QString)),
          this, SLOT(changedRegenVita(QString)));
  layout->addWidget(combo_vita, 4, 1, 1, 2);

  /* QD Regen */
  QLabel* lbl_qd = new QLabel("QD Regen", this);
  layout->addWidget(lbl_qd, 5, 0);
  combo_qd = new QComboBox(this);
  for(int i = 0; i <= (int)RegenRate::GRAND; i++)
    combo_qd->addItem(QString::fromStdString(
                                      Helpers::regenRateToStr((RegenRate)i)));
  connect(combo_qd, SIGNAL(currentIndexChanged(QString)),
          this, SLOT(changedRegenQD(QString)));
  layout->addWidget(combo_qd, 5, 1, 1, 2);

  /* Flag Group */
  QGroupBox* box_flags = new QGroupBox("Flags", this);
  QVBoxLayout* box_layout = new QVBoxLayout(box_flags);
  chk_def = new QCheckBox("Defense Enabled", this);
  connect(chk_def, SIGNAL(stateChanged(int)), this, SLOT(changedFlags(int)));
  box_layout->addWidget(chk_def);
  chk_grd = new QCheckBox("Guard Enabled", this);
  connect(chk_grd, SIGNAL(stateChanged(int)), this, SLOT(changedFlags(int)));
  box_layout->addWidget(chk_grd);
  chk_imp = new QCheckBox("Impervious", this);
  connect(chk_imp, SIGNAL(stateChanged(int)), this, SLOT(changedFlags(int)));
  box_layout->addWidget(chk_imp);
  chk_power_def = new QCheckBox("Power Defender", this);
  connect(chk_power_def, SIGNAL(stateChanged(int)),
          this, SLOT(changedFlags(int)));
  box_layout->addWidget(chk_power_def);
  chk_power_grd = new QCheckBox("Power Guarder", this);
  connect(chk_power_grd, SIGNAL(stateChanged(int)),
          this, SLOT(changedFlags(int)));
  box_layout->addWidget(chk_power_grd);
  QFrame* line1 = new QFrame(this);
  line1->setFrameShape(QFrame::HLine);
  box_layout->addWidget(line1);
  chk_e_claws = new QCheckBox("Equip: Claws", this);
  connect(chk_e_claws, SIGNAL(stateChanged(int)),
          this, SLOT(changedFlags(int)));
  box_layout->addWidget(chk_e_claws);
  chk_e_staff = new QCheckBox("Equip: Staff", this);
  connect(chk_e_staff, SIGNAL(stateChanged(int)),
          this, SLOT(changedFlags(int)));
  box_layout->addWidget(chk_e_staff);
  chk_e_sword = new QCheckBox("Equip: Sword", this);
  connect(chk_e_sword, SIGNAL(stateChanged(int)),
          this, SLOT(changedFlags(int)));
  box_layout->addWidget(chk_e_sword);
  QFrame* line2 = new QFrame(this);
  line2->setFrameShape(QFrame::HLine);
  box_layout->addWidget(line2);
  chk_e_light_armor = new QCheckBox("Equip: Light Armor", this);
  connect(chk_e_light_armor, SIGNAL(stateChanged(int)),
          this, SLOT(changedFlags(int)));
  box_layout->addWidget(chk_e_light_armor);
  chk_e_mid_armor = new QCheckBox("Equip: Mid Armor", this);
  connect(chk_e_mid_armor, SIGNAL(stateChanged(int)),
          this, SLOT(changedFlags(int)));
  box_layout->addWidget(chk_e_mid_armor);
  chk_e_heavy_armor = new QCheckBox("Equip: Heavy Armor", this);
  connect(chk_e_heavy_armor, SIGNAL(stateChanged(int)),
          this, SLOT(changedFlags(int)));
  box_layout->addWidget(chk_e_heavy_armor);
  QFrame* line3 = new QFrame(this);
  line3->setFrameShape(QFrame::HLine);
  box_layout->addWidget(line3);
  chk_e_small_arms = new QCheckBox("Equip: Small Arms", this);
  connect(chk_e_small_arms, SIGNAL(stateChanged(int)),
          this, SLOT(changedFlags(int)));
  box_layout->addWidget(chk_e_small_arms);
  chk_e_big_arms = new QCheckBox("Equip: Big Arms", this);
  connect(chk_e_big_arms, SIGNAL(stateChanged(int)),
          this, SLOT(changedFlags(int)));
  box_layout->addWidget(chk_e_big_arms);
  chk_e_long_arms = new QCheckBox("Equip: Long Arms", this);
  connect(chk_e_long_arms, SIGNAL(stateChanged(int)),
          this, SLOT(changedFlags(int)));
  box_layout->addWidget(chk_e_long_arms);
  layout->addWidget(box_flags, 0, 3, 6, 1);

  /* Immunities Group */
  QGroupBox* box_immunities = new QGroupBox("Immunities", this);
  QVBoxLayout* layout_immunities = new QVBoxLayout(box_immunities);
  for(int i = 0; i < (int)Infliction::INVALID; i++)
  {
    QString ail = QString::fromStdString(Helpers::ailmentToStr((Infliction)i));
    ail = ail.toLower();
    ail[0] = ail.at(0).toUpper();
    chk_immunities.push_back(new QCheckBox(ail, this));
    connect(chk_immunities.back(), SIGNAL(stateChanged(int)),
            this, SLOT(changedImmunities(int)));
    layout_immunities->addWidget(chk_immunities.back());
  }
  layout->addWidget(box_immunities, 0, 4, 7, 1);

  /* Preset List for Stats */
  QStringList stat_presets;
  stat_presets << "Weak" << "Not As Weak" << "Normal" << "Medium"
               << "Top" << "Boss" << "Custom";

  /* Base Stats Group */
  QGroupBox* box_stats_base = new QGroupBox("Base Stats", this);
  QGridLayout* layout_stats_base = new QGridLayout(box_stats_base);
  combo_stats_base = new QComboBox(this);
  combo_stats_base->addItems(stat_presets);
  connect(combo_stats_base, SIGNAL(currentIndexChanged(int)),
          this, SLOT(statBasePreset(int)));
  layout_stats_base->addWidget(combo_stats_base, 0, 0, 1, 2);
  for(int i = 0; i < (int)Attribute::NONE; i++)
  {
    QString att = QString::fromStdString(Helpers::attributeToStr((Attribute)i));
    QLabel* lbl = new QLabel(att, this);
    layout_stats_base->addWidget(lbl, i + 1, 0);
    edit_atts_base.push_back(new QLineEdit(this));
    connect(edit_atts_base.back(), SIGNAL(textEdited(QString)),
            this, SLOT(changedStatsBase(QString)));
    layout_stats_base->addWidget(edit_atts_base.back(), i + 1, 1);
  }
  layout->addWidget(box_stats_base, 0, 5, 7, 1);

  /* Max Stats Group */
  QGroupBox* box_stats_max = new QGroupBox("Max Stats", this);
  QGridLayout* layout_stats_max = new QGridLayout(box_stats_max);
  combo_stats_max = new QComboBox(this);
  combo_stats_max->addItems(stat_presets);
  connect(combo_stats_max, SIGNAL(currentIndexChanged(int)),
          this, SLOT(statMaxPreset(int)));
  layout_stats_max->addWidget(combo_stats_max, 0, 0, 1, 2);
  for(int i = 0; i < (int)Attribute::NONE; i++)
  {
    QString att = QString::fromStdString(Helpers::attributeToStr((Attribute)i));
    QLabel* lbl = new QLabel(att, this);
    layout_stats_max->addWidget(lbl, i + 1, 0);
    edit_atts_max.push_back(new QLineEdit(this));
    connect(edit_atts_max.back(), SIGNAL(textEdited(QString)),
            this, SLOT(changedStatsMax(QString)));
    layout_stats_max->addWidget(edit_atts_max.back(), i + 1, 1);
  }
  layout->addWidget(box_stats_max, 0, 6, 7, 1);

  /* Push Buttons - for reset and save */
  QPushButton* btn_reset = new QPushButton("Reset", this);
  connect(btn_reset, SIGNAL(clicked()), this, SLOT(btnReset()));
  layout->addWidget(btn_reset, 8, 5);
  QPushButton* btn_save = new QPushButton("Save", this);
  connect(btn_save, SIGNAL(clicked()), this, SLOT(btnSave()));
  layout->addWidget(btn_save, 8, 6);
}

/* Loads working info into UI objects */
void EditorCategory::loadWorkingInfo()
{
  /* ID */
  edit_id->setText(QString::number(id));

  /* Name */
  edit_name->setText(QString::fromStdString(cat_curr.getName()));

  /* Denonym */
  edit_denonym->setText(QString::fromStdString(cat_curr.getDenonym()));

  /* Description */
  text_desc->setText(QString::fromStdString(cat_curr.getDescription()));

  /* Vita and QD Regen */
  combo_vita->setCurrentIndex((int)cat_curr.getVitaRegenRate());
  combo_qd->setCurrentIndex((int)cat_curr.getQDRegenRate());

  /* Flags */
  chk_def->setChecked(cat_curr.getFlag(CategoryState::DEF_ENABLED));
  chk_e_big_arms->setChecked(cat_curr.getFlag(CategoryState::E_BIG_ARMS));
  chk_e_claws->setChecked(cat_curr.getFlag(CategoryState::E_CLAWS));
  chk_e_heavy_armor->setChecked(cat_curr.getFlag(CategoryState::E_HEAVY_ARMOR));
  chk_e_light_armor->setChecked(cat_curr.getFlag(CategoryState::E_LIGHT_ARMOR));
  chk_e_long_arms->setChecked(cat_curr.getFlag(CategoryState::E_LONG_ARMS));
  chk_e_mid_armor->setChecked(cat_curr.getFlag(CategoryState::E_MID_ARMOR));
  chk_e_small_arms->setChecked(cat_curr.getFlag(CategoryState::E_SMALL_ARMS));
  chk_e_staff->setChecked(cat_curr.getFlag(CategoryState::E_STAFF));
  chk_e_sword->setChecked(cat_curr.getFlag(CategoryState::E_SWORD));
  chk_grd->setChecked(cat_curr.getFlag(CategoryState::GRD_ENABLED));
  chk_imp->setChecked(cat_curr.getFlag(CategoryState::IMP_ENABLED));
  chk_power_def->setChecked(cat_curr.getFlag(CategoryState::POWER_DEFENDER));
  chk_power_grd->setChecked(cat_curr.getFlag(CategoryState::POWER_GUARDER));

  /* Immunities */
  for(int i = 0; i < (int)Infliction::INVALID; i++)
    chk_immunities[i]->setChecked(cat_curr.isImmune((Infliction)i));

  /* Base Stats */
  combo_stats_base->setCurrentIndex(kMAX_PRESETS);
  for(int i = 0; i < (int)Attribute::NONE; i++)
    edit_atts_base[i]->setText(QString::number(
                                           cat_curr.getMinSet().getStat(i)));

  /* Max Stats */
  combo_stats_max->setCurrentIndex(kMAX_PRESETS);
  for(int i = 0; i < (int)Attribute::NONE; i++)
    edit_atts_max[i]->setText(QString::number(cat_curr.getMaxSet().getStat(i)));
}

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/

/* Button Triggers */
void EditorCategory::btnReset()
{
  qDebug() << "TODO RESET";
}

/* Button Triggers */
void EditorCategory::btnSave()
{
  qDebug() << "TODO SAVE";
}

/* Widget Change Triggers */
void EditorCategory::changedDenonym(QString denonym)
{
  qDebug() << "TODO CHANGED DENONYM: " << denonym;
}

/* Widget Change Triggers */
void EditorCategory::changedDescription()
{
  qDebug() << "TODO CHANGED DESCRIPTION";
}

/* Widget Change Triggers */
void EditorCategory::changedFlags(int)
{
  qDebug() << "TODO CHANGED FLAGS";
}

/* Widget Change Triggers */
void EditorCategory::changedImmunities(int)
{
  qDebug() << "TODO CHANGED IMMUNITIES";
}

/* Widget Change Triggers */
void EditorCategory::changedName(QString name)
{
  qDebug() << "TODO CHANGED NAME: " << name;
}

/* Widget Change Triggers */
void EditorCategory::changedRegenQD(QString qd)
{
  qDebug() << "TODO CHANGED REGEN QD: " << qd;
}

/* Widget Change Triggers */
void EditorCategory::changedRegenVita(QString vita)
{
  qDebug() << "TODO CHANGED REGEN VITA: " << vita;
}

/* Widget Change Triggers */
void EditorCategory::changedStatsBase(QString)
{
  qDebug() << "CHANGED BASE STATS";
}

/* Widget Change Triggers */
void EditorCategory::changedStatsMax(QString)
{
  qDebug() << "CHANGED MAX STATS";
}

/* Resets the working set trigger */
void EditorCategory::resetWorking()
{
  // TODO: Implementation
}

/* Saves the working set trigger */
void EditorCategory::saveWorking()
{
  // TODO: Implementation
}

/* Stat Preset Triggers */
void EditorCategory::statBasePreset(int index)
{
  qDebug() << "STAT BASE PRESET: " << index;
}

/* Stat Preset Triggers */
void EditorCategory::statMaxPreset(int index)
{
  qDebug() << "STAT MAX PRESET: " << index;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Returns the ID of the category */
int EditorCategory::getID() const
{
  return id;
}

/* Returns the name of the category */
QString EditorCategory::getName() const
{
  return QString::fromStdString(cat_curr.getName());
}

/* Returns the name of the category for listing */
QString EditorCategory::getNameList()
{
  return EditorHelpers::getListString(getID(), getName());
}

/* Loads the object data */
void EditorCategory::load(XmlData data, int index)
{
  // TODO: Implementation
}

/* Saves the object data */
void EditorCategory::save(FileHandler* fh, bool game_only)
{
  // TODO: Implementation
}

/* Sets the ID of the category */
void EditorCategory::setID(int id)
{
  this->id = id;
  loadWorkingInfo();
}

/* Sets the name of the category */
void EditorCategory::setName(QString name)
{
  cat_curr.setName(name.toStdString());
  loadWorkingInfo();
  emit nameChange(name);
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/* The copy operator */
EditorCategory& EditorCategory::operator= (const EditorCategory &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
