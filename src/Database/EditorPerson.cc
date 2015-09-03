/*******************************************************************************
 * Class Name: EditorPerson
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Person
 ******************************************************************************/
#include "Database/EditorPerson.h"
#include <QDebug>

/* Constant Implementation - see header file for descriptions */
const int EditorPerson::kFRAME_SIZE = 200;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

EditorPerson::EditorPerson(QWidget* parent) : QWidget(parent)
{
  class_id = 0;
  class_id_base = 0;
  dialog_sprite = NULL;
  id = 0;
  race_id = 0;
  race_id_base = 0;

  /* Layout */
  createLayout();
}

/* Constructor function with id and name */
EditorPerson::EditorPerson(int id, QString name, QWidget* parent)
            : EditorPerson(parent)
{
  setID(id);
  setName(name);
}

/* Copy constructor */
EditorPerson::EditorPerson(const EditorPerson &source) : EditorPerson()
{
  copySelf(source);
}

EditorPerson::~EditorPerson()
{
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/* Copy function, to be called by a copy or equal operator constructor */
void EditorPerson::copySelf(const EditorPerson &source)
{
  // TODO: Implementation
  //person = source.person;
}

/* Creates interface layout */
void EditorPerson::createLayout()
{
  /* Layout */
  QGridLayout* layout = new QGridLayout(this);
  layout->setColumnStretch(10, 1);
  layout->setRowMinimumHeight(12, 15);
  layout->setRowStretch(11, 1);

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

  /* String sets for categories */
  QStringList elements;
  elements << "Physical" << "Fire" << "Ice" << "Forest" << "Electric" << "Digital"
           << "Nihil";

  /* Primary Element */
  QLabel* lbl_pri = new QLabel("Primary", this);
  layout->addWidget(lbl_pri, 2, 0);
  combo_pri = new QComboBox(this);
  combo_pri->addItems(elements);
  layout->addWidget(combo_pri, 2, 1);

  /* Secondary Element */
  QLabel* lbl_sec = new QLabel("Secondary", this);
  layout->addWidget(lbl_sec, 3, 0);
  combo_sec = new QComboBox(this);
  combo_sec->addItems(elements);
  layout->addWidget(combo_sec, 3, 1);

  /* Class */
  QLabel* lbl_class = new QLabel("Class", this);
  layout->addWidget(lbl_class, 4, 0);
  combo_class = new QComboBox(this);
  combo_class->addItem("None");
  layout->addWidget(combo_class, 4, 1);

  /* Race */
  QLabel* lbl_race = new QLabel("Race", this);
  layout->addWidget(lbl_race, 5, 0);
  combo_race = new QComboBox(this);
  combo_race->addItem("None");
  layout->addWidget(combo_race);

  /* Flags */
  QGroupBox* box_flags = new QGroupBox("Flags", this);
  QVBoxLayout* layout_flags = new QVBoxLayout(box_flags);
  chk_gain_xp = new QCheckBox("Can Gain XP", this);
  layout_flags->addWidget(chk_gain_xp);
  chk_lvl_up = new QCheckBox("Can Level Up", this);
  layout_flags->addWidget(chk_lvl_up);
  chk_learn_skills = new QCheckBox("Can Learn Skills", this);
  layout_flags->addWidget(chk_learn_skills);
  chk_change_equip = new QCheckBox("Can Change Equipment", this);
  layout_flags->addWidget(chk_change_equip);
  layout->addWidget(box_flags, 0, 2, 6, 2);

  /* Loot Headers */
  QLabel* lbl_loot = new QLabel("Loot", this);
  layout->addWidget(lbl_loot, 0, 6, 1, 4, Qt::AlignHCenter);

  /* Loot Credits */
  QLabel* lbl_credits = new QLabel("Credits", this);
  layout->addWidget(lbl_credits, 1, 6);
  spin_credits = new QSpinBox(this);
  layout->addWidget(spin_credits, 1, 7);

  /* Loot Experience */
  QLabel* lbl_xp = new QLabel("Experience", this);
  layout->addWidget(lbl_xp, 1, 8);
  spin_xp = new QSpinBox(this);
  layout->addWidget(spin_xp, 1, 9);

  /* Loot Items */
  btn_item_add = new QPushButton("Add Item", this);
  layout->addWidget(btn_item_add, 2, 6, 1, 2);
  btn_item_rem = new QPushButton("Remove Item", this);
  layout->addWidget(btn_item_rem, 2, 8, 1, 2);
  list_items_all = new QListWidget(this);
  layout->addWidget(list_items_all, 3, 6, 9, 2);
  list_items_used = new QListWidget(this);
  layout->addWidget(list_items_used, 3, 8, 9, 2);

  /* First Person Image */
  QLabel* lbl_fp = new QLabel("First Person", this);
  layout->addWidget(lbl_fp, 7, 0, 1, 2, Qt::AlignHCenter);
  lbl_fp_img = new QLabel(this);
  lbl_fp_img->setMinimumSize(kFRAME_SIZE, kFRAME_SIZE);
  lbl_fp_img->setStyleSheet("border: 1px solid #b9b5b2");
  lbl_fp_img->setAlignment(Qt::AlignCenter);
  layout->addWidget(lbl_fp_img, 8, 0, 1, 2);
  QPushButton* btn_fp = new QPushButton(this);
  btn_fp->setIcon(QIcon(":/images/icons/32_settings.png"));
  btn_fp->setIconSize(QSize(24,24));
  btn_fp->setMaximumSize(30, 30);
  //connect(btn_fp, SIGNAL(clicked()), this, SLOT(buttonAnimEdit()));
  layout->addWidget(btn_fp, 8, 0, 1, 2,
                    Qt::AlignRight | Qt::AlignTop);

  /* Third Person Image */
  QLabel* lbl_tp = new QLabel("Third Person", this);
  layout->addWidget(lbl_tp, 7, 2, 1, 2, Qt::AlignHCenter);
  lbl_tp_img = new QLabel(this);
  lbl_tp_img->setMinimumSize(kFRAME_SIZE, kFRAME_SIZE);
  lbl_tp_img->setStyleSheet("border: 1px solid #b9b5b2");
  lbl_tp_img->setAlignment(Qt::AlignCenter);
  layout->addWidget(lbl_tp_img, 8, 2, 1, 2);
  QPushButton* btn_tp = new QPushButton(this);
  btn_tp->setIcon(QIcon(":/images/icons/32_settings.png"));
  btn_tp->setIconSize(QSize(24,24));
  btn_tp->setMaximumSize(30, 30);
  //connect(btn_tp, SIGNAL(clicked()), this, SLOT(buttonAnimEdit()));
  layout->addWidget(btn_tp, 8, 2, 1, 2,
                    Qt::AlignRight | Qt::AlignTop);

  /* Dialog Image */
  QLabel* lbl_ds = new QLabel("Dialog Person", this);
  layout->addWidget(lbl_ds, 0, 4, 1, 2, Qt::AlignHCenter);
  lbl_ds_img = new QLabel(this);
  lbl_ds_img->setMinimumSize(kFRAME_SIZE, kFRAME_SIZE);
  lbl_ds_img->setStyleSheet("border: 1px solid #b9b5b2");
  lbl_ds_img->setAlignment(Qt::AlignCenter);
  layout->addWidget(lbl_ds_img, 1, 4, 6, 2);
  QPushButton* btn_ds = new QPushButton(this);
  btn_ds->setIcon(QIcon(":/images/icons/32_settings.png"));
  btn_ds->setIconSize(QSize(24,24));
  btn_ds->setMaximumSize(30, 30);
  //connect(btn_ds, SIGNAL(clicked()), this, SLOT(buttonAnimEdit()));
  layout->addWidget(btn_ds, 1, 4, 6, 2,
                    Qt::AlignRight | Qt::AlignTop);

  /* Action Image */
  QLabel* lbl_as = new QLabel("Action Person", this);
  layout->addWidget(lbl_as, 7, 4, 1, 2, Qt::AlignHCenter);
  lbl_as_img = new QLabel(this);
  lbl_as_img->setMinimumSize(kFRAME_SIZE, kFRAME_SIZE);
  lbl_as_img->setStyleSheet("border: 1px solid #b9b5b2");
  lbl_as_img->setAlignment(Qt::AlignCenter);
  layout->addWidget(lbl_as_img, 8, 4, 1, 2);
  QPushButton* btn_as = new QPushButton(this);
  btn_as->setIcon(QIcon(":/images/icons/32_settings.png"));
  btn_as->setIconSize(QSize(24,24));
  btn_as->setMaximumSize(30, 30);
  //connect(btn_as, SIGNAL(clicked()), this, SLOT(buttonAnimEdit()));
  layout->addWidget(btn_as, 8, 4, 1, 2,
                    Qt::AlignRight | Qt::AlignTop);

  /* Action X */
  QLabel* lbl_actionx = new QLabel("Action X", this);
  layout->addWidget(lbl_actionx, 9, 4);
  QSpinBox* spin_actionx = new QSpinBox(this);
  layout->addWidget(spin_actionx, 9, 5);

  /* Action Y */
  QLabel* lbl_actiony = new QLabel("Action Y", this);
  layout->addWidget(lbl_actiony, 10, 4);
  QSpinBox* spin_actiony = new QSpinBox(this);
  layout->addWidget(spin_actiony, 10, 5);

  /* Save / Reset Buttons */
  QPushButton* btn_reset = new QPushButton("Reset", this);
  //connect(btn_reset, SIGNAL(clicked()), this, SLOT(btnReset()));
  layout->addWidget(btn_reset, 13, 8);
  QPushButton* btn_save = new QPushButton("Save", this);
  //connect(btn_save, SIGNAL(clicked()), this, SLOT(btnSave()));
  layout->addWidget(btn_save, 13, 9);
}

/* Loads working info into UI objects */
void EditorPerson::loadWorkingInfo()
{
  // TODO: Implementation
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Returns the ID of the person */
int EditorPerson::getID() const
{
  return id;
}

/* Returns the name of the person */
QString EditorPerson::getName() const
{
  return QString::fromStdString(person_curr.getName());
}

/* Returns the name of the person for listing */
QString EditorPerson::getNameList()
{
  return EditorHelpers::getListString(getID(), getName());
}

/* Loads the object data */
void EditorPerson::load(XmlData data, int index)
{
  // TODO: Implementation
}

/* Resets the working set trigger */
void EditorPerson::resetWorking()
{
  // TODO: Implementation
}

/* Saves the object data */
void EditorPerson::save(FileHandler* fh, bool game_only)
{
  // TODO: Implementation
}

/* Saves the working set trigger */
void EditorPerson::saveWorking()
{
  // TODO: Implementation
}

/* Sets the ID of the person */
void EditorPerson::setID(int id)
{
  this->id = id;
  edit_id->setText(QString::number(id));
}

/* Sets the name of the person */
void EditorPerson::setName(QString name)
{
  person_curr.setName(name.toStdString());
  edit_name->setText(name);
  emit nameChange(name);
}

/* Update Calls for data */
void EditorPerson::updateClasses(QVector<EditorCategory*> classes)
{
  // TODO: Implementation
}

/* Update Calls for data */
void EditorPerson::updateItems(QVector<EditorItem*> items)
{
  // TODO: Implementation
}

/* Update Calls for data */
void EditorPerson::updateRaces(QVector<EditorCategory*> races)
{
  // TODO: Implementation
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/* The copy operator */
EditorPerson& EditorPerson::operator= (const EditorPerson &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
