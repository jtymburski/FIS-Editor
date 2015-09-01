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

/*
 * Description: Main constructor function. All parameters have null defaults.
 *
 * Inputs: QWidget* parent - the parent widget. Default to NULL
 */
EditorCategory::EditorCategory(QWidget *parent) : QWidget(parent)
{
  /* Properties of base data */
  cat_base.triggerEditMode(true);
  cat_curr.triggerEditMode(true);
  chk_no_signals = false;
  id = 0;

  /* Layouts and info loading */
  createLayout();
  loadWorkingInfo();
}

/*
 * Description: Second constructor function, with ID and name parameters.
 *
 * Inputs: int id - the id of the category set
 *         QString name - the name of the category set
 *         QWidget* parent - the parent widget. Default to NULL
 */
EditorCategory::EditorCategory(int id, QString name, QWidget* parent)
              : EditorCategory(parent)
{
  setID(id);
  setName(name);
  saveWorking();
}

/*
 * Description: Copy constructor. Calls the blank constructor and then copies
 *              the data from the source.
 *
 * Inputs: const EditorCategory &source - the source object to copy
 */
EditorCategory::EditorCategory(const EditorCategory &source) : EditorCategory()
{
  copySelf(source);
}

/*
 * Description: Destructor function
 */
EditorCategory::~EditorCategory()
{
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: Copies all data from source editor object to this editor
 *              object.
 *
 * Inputs: EditorCategory &source - the source to copy from
 * Output: none
 */
void EditorCategory::copySelf(const EditorCategory &source)
{
  cat_base = source.cat_base;
  id = source.id;
  resetWorking();
}

/*
 * Description: Creates the category layout with QT functional widgets.
 *
 * Inputs: none
 * Output: none
 */
void EditorCategory::createLayout()
{
  /* Layout */
  QGridLayout* layout = new QGridLayout(this);
  layout->setColumnStretch(7, 1);
  layout->setRowMinimumHeight(7, 15);
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
  combo_stats_base->setCurrentIndex(kMAX_PRESETS);
  connect(combo_stats_base, SIGNAL(currentIndexChanged(int)),
          this, SLOT(statBasePreset(int)));
  layout_stats_base->addWidget(combo_stats_base, 0, 0, 1, 2);
  for(int i = 0; i < (int)Attribute::NONE; i++)
  {
    QString att = QString::fromStdString(Helpers::attributeToStr((Attribute)i));
    QLabel* lbl = new QLabel(att, this);
    layout_stats_base->addWidget(lbl, i + 1, 0);
    spin_atts_base.push_back(new QSpinBox(this));
    spin_atts_base.last()->setMinimum(AttributeSet::kMIN_P_VALUE);
    spin_atts_base.last()->setMaximum(AttributeSet::kMAX_VALUE);
    connect(spin_atts_base.last(), SIGNAL(valueChanged(QString)),
            this, SLOT(changedStatsBase(QString)));
    layout_stats_base->addWidget(spin_atts_base.last(), i + 1, 1);
  }
  layout->addWidget(box_stats_base, 0, 5, 7, 1);

  /* Max Stats Group */
  QGroupBox* box_stats_max = new QGroupBox("Max Stats", this);
  QGridLayout* layout_stats_max = new QGridLayout(box_stats_max);
  combo_stats_max = new QComboBox(this);
  combo_stats_max->addItems(stat_presets);
  combo_stats_max->setCurrentIndex(kMAX_PRESETS);
  connect(combo_stats_max, SIGNAL(currentIndexChanged(int)),
          this, SLOT(statMaxPreset(int)));
  layout_stats_max->addWidget(combo_stats_max, 0, 0, 1, 2);
  for(int i = 0; i < (int)Attribute::NONE; i++)
  {
    QString att = QString::fromStdString(Helpers::attributeToStr((Attribute)i));
    QLabel* lbl = new QLabel(att, this);
    layout_stats_max->addWidget(lbl, i + 1, 0);
    spin_atts_max.push_back(new QSpinBox(this));
    spin_atts_max.last()->setMinimum(AttributeSet::kMIN_P_VALUE);
    spin_atts_max.last()->setMaximum(AttributeSet::kMAX_VALUE);
    connect(spin_atts_max.last(), SIGNAL(valueChanged(QString)),
            this, SLOT(changedStatsMax(QString)));
    layout_stats_max->addWidget(spin_atts_max.back(), i + 1, 1);
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

/*
 * Description: Loads all the UI elements with the contents from the working
 *              Category information.
 *
 * Inputs: none
 * Output: none
 */
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
  chk_no_signals = true;
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
  chk_no_signals = false;

  /* Immunities */
  for(int i = 0; i < (int)Infliction::INVALID; i++)
  {
    chk_immunities[i]->blockSignals(true);
    chk_immunities[i]->setChecked(cat_curr.isImmune((Infliction)i));
    chk_immunities[i]->blockSignals(false);
  }

  /* Base Stats */
  for(int i = 0; i < (int)Attribute::NONE; i++)
  {
    spin_atts_base[i]->blockSignals(true);
    spin_atts_base[i]->setValue(cat_curr.getBaseSet().getStat(i));
    spin_atts_base[i]->blockSignals(false);
  }

  /* Max Stats */
  for(int i = 0; i < (int)Attribute::NONE; i++)
  {
    spin_atts_max[i]->blockSignals(true);
    spin_atts_max[i]->setValue(cat_curr.getTopSet().getStat(i));
    spin_atts_max[i]->blockSignals(false);
  }
}

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/

/*
 * Description: Slot triggered by reset button. Resets data in Category.
 *
 * Inputs: none
 * Output: none
 */
void EditorCategory::btnReset()
{
  resetWorking();
}

/*
 * Description: Slot triggered by reset button. Saves data in Category.
 *
 * Inputs: none
 * Output: none
 */
void EditorCategory::btnSave()
{
  saveWorking();
}

/*
 * Description: Slot triggered when the denonym line edit text is modified.
 *              Updates the working data with the new denonym.
 *
 * Inputs: QString denonym - the new text
 * Output: none
 */
void EditorCategory::changedDenonym(QString denonym)
{
  cat_curr.setDenonym(denonym.toStdString());
}

/*
 * Description: Slot triggered when the description text edit contents are
 *              modified. Updates the working data with the new description.
 *
 * Inputs: none
 * Output: none
 */
void EditorCategory::changedDescription()
{
  cat_curr.setDescription(text_desc->toPlainText().simplified().toStdString());
}

/*
 * Description: Slot triggered when any flag check box is within the flag
 *              group is modified. Internal private variable 'chk_no_signals'
 *              inidicates if signals will be fired by checks. Default to false.
 *
 * Inputs: int - not used
 * Output: none
 */
void EditorCategory::changedFlags(int)
{
  if(!chk_no_signals)
  {
    cat_curr.setFlag(CategoryState::DEF_ENABLED, chk_def->isChecked());
    cat_curr.setFlag(CategoryState::E_BIG_ARMS, chk_e_big_arms->isChecked());
    cat_curr.setFlag(CategoryState::E_CLAWS, chk_e_claws->isChecked());
    cat_curr.setFlag(CategoryState::E_HEAVY_ARMOR,
                     chk_e_heavy_armor->isChecked());
    cat_curr.setFlag(CategoryState::E_LIGHT_ARMOR,
                     chk_e_light_armor->isChecked());
    cat_curr.setFlag(CategoryState::E_LONG_ARMS, chk_e_long_arms->isChecked());
    cat_curr.setFlag(CategoryState::E_MID_ARMOR, chk_e_mid_armor->isChecked());
    cat_curr.setFlag(CategoryState::E_SMALL_ARMS, chk_e_small_arms->isChecked());
    cat_curr.setFlag(CategoryState::E_STAFF, chk_e_staff->isChecked());
    cat_curr.setFlag(CategoryState::E_SWORD, chk_e_sword->isChecked());
    cat_curr.setFlag(CategoryState::GRD_ENABLED, chk_grd->isChecked());
    cat_curr.setFlag(CategoryState::IMP_ENABLED, chk_imp->isChecked());
    cat_curr.setFlag(CategoryState::POWER_DEFENDER, chk_power_def->isChecked());
    cat_curr.setFlag(CategoryState::POWER_GUARDER, chk_power_grd->isChecked());
  }
}

/*
 * Description: Slot triggered when any immunity check boxes are modified.
 *
 * Inputs: int - not used
 * Output: none
 */
void EditorCategory::changedImmunities(int)
{
  for(int i = 0; i < chk_immunities.size(); i++)
    cat_curr.setImmunity((Infliction)i, chk_immunities[i]->isChecked());
}

/*
 * Description: Slot triggered when the name line edit text is modified.
 *              Updates the working data with the new name.
 *
 * Inputs: QString name - the new text
 * Output: none
 */
void EditorCategory::changedName(QString name)
{
  setName(name);
}

/*
 * Description: Slot triggered when the QD regen rate drop down changes the
 *              active index. Updates the working data with the new qd rate.
 *
 * Inputs: QString qd - the new selected combo box text
 * Output: none
 */
void EditorCategory::changedRegenQD(QString qd)
{
  cat_curr.setQDRegenRate(Helpers::regenRateFromStr(qd.toStdString()));
}

/*
 * Description: Slot triggered when the vitality regen rate drop down changes
 *              the active index. Updates the working data with the new
 *              vitality rate.
 *
 * Inputs: QString vita - the new selected combo box text
 * Output: none
 */
void EditorCategory::changedRegenVita(QString vita)
{
  cat_curr.setVitaRegenRate(Helpers::regenRateFromStr(vita.toStdString()));
}

/*
 * Description: Slot triggered when any one spin box of the base stat group set
 *              is modified. Updates the working data with the base attribute
 *              set.
 *
 * Inputs: QString - not used
 * Output: none
 */
void EditorCategory::changedStatsBase(QString)
{
  combo_stats_base->setCurrentIndex(kMAX_PRESETS);
  for(int i = 0; i < spin_atts_base.size(); i++)
    cat_curr.getBaseSet().setStat((Attribute)i, spin_atts_base[i]->value());
}

/*
 * Description: Slot triggered when any one spin box of the max stat group set
 *              is modified. Updates the working data with the max attribute
 *              set.
 *
 * Inputs: QString - not used
 * Output: none
 */
void EditorCategory::changedStatsMax(QString)
{
  combo_stats_max->setCurrentIndex(kMAX_PRESETS);
  for(int i = 0; i < spin_atts_max.size(); i++)
    cat_curr.getTopSet().setStat((Attribute)i, spin_atts_max[i]->value());
}

/*
 * Description: Slot triggered when the base preset drop down is modified. This
 *              loads a preset attribute set into the base, which can then be
 *              further modified.
 *
 * Inputs: int index - the index in the dropdown
 * Output: none
 */
void EditorCategory::statBasePreset(int index)
{
  if(index >= 0 && index < kMAX_PRESETS)
  {
    cat_curr.getBaseSet().buildAsPreset(index);
    loadWorkingInfo();
  }
}

/*
 * Description: Slot triggered when the max preset drop down is modified. This
 *              loads a preset attribute set into the base, which can then be
 *              further modified.
 *
 * Inputs: int index - the index in the dropdown
 * Output: none
 */
void EditorCategory::statMaxPreset(int index)
{
  if(index >= 0 && index < kMAX_PRESETS)
  {
    cat_curr.getTopSet().buildAsPreset(index);
    loadWorkingInfo();
  }
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Returns the ID of the category
 *
 * Inputs: none
 * Output: int - the ID
 */
int EditorCategory::getID() const
{
  return id;
}

/*
 * Description: Returns the name of the category
 *
 * Inputs: none
 * Output: QString - the name
 */
QString EditorCategory::getName() const
{
  return QString::fromStdString(cat_curr.getName());
}

/*
 * Description: Get the listing name string which includes the ID and name
 *
 * Inputs: none
 * Output: QString - the name list
 */
QString EditorCategory::getNameList()
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
void EditorCategory::load(XmlData data, int index)
{
  /* Parse elements */
  cat_base.loadData(data, index, NULL);
}

/*
 * Description: Resets the working information back to the base (last saved)
 *              data. This will also automatically update all UI widgets with
 *              the new data.
 *
 * Inputs: none
 * Output: none
 */
void EditorCategory::resetWorking()
{
  cat_curr = cat_base;
  setName(QString::fromStdString(cat_curr.getName()));
  combo_stats_base->setCurrentIndex(kMAX_PRESETS);
  combo_stats_max->setCurrentIndex(kMAX_PRESETS);
  loadWorkingInfo();
}

/*
 * Description: Saves the object data to the file handling pointer.
 *
 * Inputs: FileHandler* fh - the file handling pointer
 *         bool game_only - true if the data should include game only relevant
 * Output: none
 */
void EditorCategory::save(FileHandler* fh, bool game_only, QString wrapper)
{
  (void)game_only;

  if(fh != NULL)
  {
    /* Processing */
    cat_base.triggerEditMode(false);
    Category blank;

    /* Wrapper */
    fh->writeXmlElement(wrapper.toStdString(), "id", getID());

    /* Name */
    fh->writeXmlData("name", getName().toStdString());

    /* Denonym */
    if(cat_base.getDenonym() != blank.getDenonym())
      fh->writeXmlData("denonym", cat_base.getDenonym());

    /* Description */
    if(cat_base.getDescription() != blank.getDescription())
      fh->writeXmlData("description", cat_base.getDescription());

    /* Vita Regen */
    if(cat_base.getVitaRegenRate() != blank.getVitaRegenRate())
      fh->writeXmlData("vita_regen",
                       Helpers::regenRateToStr(cat_base.getVitaRegenRate()));

    /* QD Regen */
    if(cat_base.getQDRegenRate() != blank.getQDRegenRate())
      fh->writeXmlData("qd_regen",
                       Helpers::regenRateToStr(cat_base.getQDRegenRate()));

    /* Flags */
    fh->writeXmlElement("flags");
    if(cat_base.getFlag(CategoryState::DEF_ENABLED) !=
       blank.getFlag(CategoryState::DEF_ENABLED))
      fh->writeXmlData("def_enabled",
                       cat_base.getFlag(CategoryState::DEF_ENABLED));
    if(cat_base.getFlag(CategoryState::E_BIG_ARMS) !=
       blank.getFlag(CategoryState::E_BIG_ARMS))
      fh->writeXmlData("e_big_arms",
                       cat_base.getFlag(CategoryState::E_BIG_ARMS));
    if(cat_base.getFlag(CategoryState::E_CLAWS) !=
       blank.getFlag(CategoryState::E_CLAWS))
      fh->writeXmlData("e_claws",
                       cat_base.getFlag(CategoryState::E_CLAWS));
    if(cat_base.getFlag(CategoryState::E_HEAVY_ARMOR) !=
       blank.getFlag(CategoryState::E_HEAVY_ARMOR))
      fh->writeXmlData("e_heavy_armor",
                       cat_base.getFlag(CategoryState::E_HEAVY_ARMOR));
    if(cat_base.getFlag(CategoryState::E_LIGHT_ARMOR) !=
       blank.getFlag(CategoryState::E_LIGHT_ARMOR))
      fh->writeXmlData("e_light_armor",
                       cat_base.getFlag(CategoryState::E_LIGHT_ARMOR));
    if(cat_base.getFlag(CategoryState::E_LONG_ARMS) !=
       blank.getFlag(CategoryState::E_LONG_ARMS))
      fh->writeXmlData("e_long_arms",
                       cat_base.getFlag(CategoryState::E_LONG_ARMS));
    if(cat_base.getFlag(CategoryState::E_MID_ARMOR) !=
       blank.getFlag(CategoryState::E_MID_ARMOR))
      fh->writeXmlData("e_mid_armor",
                       cat_base.getFlag(CategoryState::E_MID_ARMOR));
    if(cat_base.getFlag(CategoryState::E_SMALL_ARMS) !=
       blank.getFlag(CategoryState::E_SMALL_ARMS))
      fh->writeXmlData("e_small_arms",
                       cat_base.getFlag(CategoryState::E_SMALL_ARMS));
    if(cat_base.getFlag(CategoryState::E_STAFF) !=
       blank.getFlag(CategoryState::E_STAFF))
      fh->writeXmlData("e_staff",
                       cat_base.getFlag(CategoryState::E_STAFF));
    if(cat_base.getFlag(CategoryState::E_SWORD) !=
       blank.getFlag(CategoryState::E_SWORD))
      fh->writeXmlData("e_sword",
                       cat_base.getFlag(CategoryState::E_SWORD));
    if(cat_base.getFlag(CategoryState::GRD_ENABLED) !=
       blank.getFlag(CategoryState::GRD_ENABLED))
      fh->writeXmlData("grd_enabled",
                       cat_base.getFlag(CategoryState::GRD_ENABLED));
    if(cat_base.getFlag(CategoryState::IMP_ENABLED) !=
       blank.getFlag(CategoryState::IMP_ENABLED))
      fh->writeXmlData("imp_enabled",
                       cat_base.getFlag(CategoryState::IMP_ENABLED));
    if(cat_base.getFlag(CategoryState::POWER_DEFENDER) !=
       blank.getFlag(CategoryState::POWER_DEFENDER))
      fh->writeXmlData("power_defender",
                       cat_base.getFlag(CategoryState::POWER_DEFENDER));
    if(cat_base.getFlag(CategoryState::POWER_GUARDER) !=
       blank.getFlag(CategoryState::POWER_GUARDER))
      fh->writeXmlData("power_guarder",
                       cat_base.getFlag(CategoryState::POWER_GUARDER));
    fh->writeXmlElementEnd();

    /* Immunities */
    fh->writeXmlElement("immunities");
    for(int i = 0; i < (int)Infliction::INVALID; i++)
    {
      Infliction curr = (Infliction)i;
      if(cat_base.isImmune(curr) != blank.isImmune(curr))
      {
        QString ele = QString::fromStdString(Helpers::ailmentToStr(curr));
        ele = ele.toLower();
        fh->writeXmlData(ele.toStdString(), cat_base.isImmune(curr));
      }
    }
    fh->writeXmlElementEnd();

    /* Base Stats */
    fh->writeXmlData("stats_base",
                     AttributeSet::setToStr(cat_base.getBaseSet()));

    /* Top Stats */
    fh->writeXmlData("stats_max", AttributeSet::setToStr(cat_base.getTopSet()));

    /* End Wrapper */
    fh->writeXmlElementEnd();
    cat_base.triggerEditMode();
  }
}

/*
 * Description: Saves the working information back to the base (last saved)
 *              data.
 *
 * Inputs: none
 * Output: none
 */
void EditorCategory::saveWorking()
{
  cat_base = cat_curr;
}

/*
 * Description: Sets the category ID in the class.
 *
 * Inputs: int id - the new id
 * Output: none
 */
void EditorCategory::setID(int id)
{
  this->id = id;
  edit_id->setText(QString::number(id));
}

/*
 * Description: Sets the name of the category.
 *
 * Inputs: QString name - the name text
 * Output: none
 */
void EditorCategory::setName(QString name)
{
  cat_curr.setName(name.toStdString());
  edit_name->setText(QString::fromStdString(cat_curr.getName()));
  emit nameChange(name);
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/*
 * Description: Copy operator construction. This is called when the variable
 *              already exists and equal operator used with another
 *              EditorCategory.
 *
 * Inputs: const EditorCategory &source - the source class constructor
 * Output: EditorCategory& - pointer to the copied class
 */
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
