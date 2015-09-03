/*******************************************************************************
 * Class Name: EditorPerson
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Person
 ******************************************************************************/
#include "Database/EditorPerson.h"
#include <QDebug>

/* Constant Implementation - see header file for descriptions */
const int EditorPerson::kELEMENT_COUNT = 8;
const int EditorPerson::kFRAME_SIZE = 200;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

EditorPerson::EditorPerson(QWidget* parent) : QWidget(parent)
{
  class_id = -1;
  class_id_base = -1;
  dialog_sprite = NULL;
  id = 0;
  person_base.setCurves(Element::PHYSICAL, ElementCurve::D,
                        Element::PHYSICAL, ElementCurve::D);
  person_curr = person_base;
  race_id = -1;
  race_id_base = -1;

  /* Layout */
  createLayout();
  loadWorkingInfo();
}

/* Constructor function with id and name */
EditorPerson::EditorPerson(int id, QString name, QWidget* parent)
            : EditorPerson(parent)
{
  setID(id);
  setName(name);
  saveWorking();
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
  connect(edit_name, SIGNAL(textEdited(QString)),
          this, SLOT(changedName(QString)));
  layout->addWidget(edit_name, 1, 1);

  /* String sets for categories */
  QStringList elements;
  for(int i = 1; i < kELEMENT_COUNT; i++)
  {
    /* Base */
    QString base = QString::fromStdString(
                            Helpers::elementToString((Element)(1 << i)));
    base = base.toLower();
    base[0] = base.at(0).toUpper();

    /* Curve */
    for(int j = 0; j <= (int)ElementCurve::D; j++)
    {
      QString curve = QString::fromStdString(
                               Helpers::curveToString((ElementCurve)j, false));
      elements << (base + " - " + curve);
    }
  }

  /* Primary Element */
  QLabel* lbl_pri = new QLabel("Primary", this);
  layout->addWidget(lbl_pri, 2, 0);
  combo_pri = new QComboBox(this);
  combo_pri->addItems(elements);
  connect(combo_pri, SIGNAL(currentIndexChanged(QString)),
          this, SLOT(changedPrimary(QString)));
  layout->addWidget(combo_pri, 2, 1);

  /* Secondary Element */
  QLabel* lbl_sec = new QLabel("Secondary", this);
  layout->addWidget(lbl_sec, 3, 0);
  combo_sec = new QComboBox(this);
  combo_sec->addItems(elements);
  connect(combo_sec, SIGNAL(currentIndexChanged(QString)),
          this, SLOT(changedSecondary(QString)));
  layout->addWidget(combo_sec, 3, 1);

  /* Class */
  QLabel* lbl_class = new QLabel("Class", this);
  layout->addWidget(lbl_class, 4, 0);
  combo_class = new QComboBox(this);
  combo_class->addItem("None");
  connect(combo_class, SIGNAL(currentIndexChanged(int)),
          this, SLOT(changedClass(int)));
  layout->addWidget(combo_class, 4, 1);

  /* Race */
  QLabel* lbl_race = new QLabel("Race", this);
  layout->addWidget(lbl_race, 5, 0);
  combo_race = new QComboBox(this);
  combo_race->addItem("None");
  connect(combo_race, SIGNAL(currentIndexChanged(int)),
          this, SLOT(changedRace(int)));
  layout->addWidget(combo_race);

  /* Flags */
  QGroupBox* box_flags = new QGroupBox("Flags", this);
  QVBoxLayout* layout_flags = new QVBoxLayout(box_flags);
  chk_gain_xp = new QCheckBox("Can Gain XP", this);
  connect(chk_gain_xp, SIGNAL(stateChanged(int)),
          this, SLOT(changedFlags(int)));
  layout_flags->addWidget(chk_gain_xp);
  chk_lvl_up = new QCheckBox("Can Level Up", this);
  connect(chk_lvl_up, SIGNAL(stateChanged(int)),
          this, SLOT(changedFlags(int)));
  layout_flags->addWidget(chk_lvl_up);
  chk_learn_skills = new QCheckBox("Can Learn Skills", this);
  connect(chk_learn_skills, SIGNAL(stateChanged(int)),
          this, SLOT(changedFlags(int)));
  layout_flags->addWidget(chk_learn_skills);
  chk_change_equip = new QCheckBox("Can Change Equipment", this);
  connect(chk_change_equip, SIGNAL(stateChanged(int)),
          this, SLOT(changedFlags(int)));
  layout_flags->addWidget(chk_change_equip);
  layout->addWidget(box_flags, 0, 2, 6, 2);

  /* Loot Headers */
  QLabel* lbl_loot = new QLabel("Loot", this);
  layout->addWidget(lbl_loot, 0, 6, 1, 4, Qt::AlignHCenter);

  /* Loot Credits */
  QLabel* lbl_credits = new QLabel("Credits", this);
  layout->addWidget(lbl_credits, 1, 6);
  spin_credits = new QSpinBox(this);
  spin_credits->setMinimum(0);
  spin_credits->setMaximum(Person::kMAX_CREDIT_DROP);
  connect(spin_credits, SIGNAL(valueChanged(int)),
          this, SLOT(changedCredits(int)));
  layout->addWidget(spin_credits, 1, 7);

  /* Loot Experience */
  QLabel* lbl_xp = new QLabel("Experience", this);
  layout->addWidget(lbl_xp, 1, 8);
  spin_xp = new QSpinBox(this);
  spin_xp->setMinimum(0);
  spin_xp->setMaximum(Person::kMAX_EXP_DROP);
  connect(spin_xp, SIGNAL(valueChanged(int)),
          this, SLOT(changedExperience(int)));
  layout->addWidget(spin_xp, 1, 9);

  /* Loot Items */
  btn_item_add = new QPushButton("Add Item", this);
  btn_item_add->setDisabled(true);
  connect(btn_item_add, SIGNAL(clicked()), this, SLOT(btnItemAdd()));
  layout->addWidget(btn_item_add, 2, 6, 1, 2);
  btn_item_rem = new QPushButton("Remove Item", this);
  btn_item_rem->setDisabled(true);
  connect(btn_item_rem, SIGNAL(clicked()), this, SLOT(btnItemRemove()));
  layout->addWidget(btn_item_rem, 2, 8, 1, 2);
  list_items_all = new QListWidget(this);
  connect(list_items_all, SIGNAL(currentRowChanged(int)),
          this, SLOT(listAllIndexChanged(int)));
  layout->addWidget(list_items_all, 3, 6, 9, 2);
  list_items_used = new QListWidget(this);
  connect(list_items_used, SIGNAL(currentRowChanged(int)),
          this, SLOT(listUsedIndexChanged(int)));
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
  connect(btn_fp, SIGNAL(clicked()), this, SLOT(btnFirstPerson()));
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
  connect(btn_tp, SIGNAL(clicked()), this, SLOT(btnThirdPerson()));
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
  connect(btn_ds, SIGNAL(clicked()), this, SLOT(btnDialogSprite()));
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
  connect(btn_as, SIGNAL(clicked()), this, SLOT(btnActionSprite()));
  layout->addWidget(btn_as, 8, 4, 1, 2,
                    Qt::AlignRight | Qt::AlignTop);

  /* Action X */
  QLabel* lbl_actionx = new QLabel("Action X", this);
  layout->addWidget(lbl_actionx, 9, 4);
  spin_actionx = new QSpinBox(this);
  spin_actionx->setMinimum(0);
  spin_actionx->setMaximum(256);
  connect(spin_actionx, SIGNAL(valueChanged(int)),
          this, SLOT(changedActionX(int)));
  layout->addWidget(spin_actionx, 9, 5);

  /* Action Y */
  QLabel* lbl_actiony = new QLabel("Action Y", this);
  layout->addWidget(lbl_actiony, 10, 4);
  spin_actiony = new QSpinBox(this);
  spin_actiony->setMinimum(0);
  spin_actiony->setMaximum(256);
  connect(spin_actiony, SIGNAL(valueChanged(int)),
          this, SLOT(changedActionY(int)));
  layout->addWidget(spin_actiony, 10, 5);

  /* Save / Reset Buttons */
  QPushButton* btn_reset = new QPushButton("Reset", this);
  connect(btn_reset, SIGNAL(clicked()), this, SLOT(btnReset()));
  layout->addWidget(btn_reset, 13, 8);
  QPushButton* btn_save = new QPushButton("Save", this);
  connect(btn_save, SIGNAL(clicked()), this, SLOT(btnSave()));
  layout->addWidget(btn_save, 13, 9);
}

/* Gets element index for combo box */
int EditorPerson::getElementIndex(Element ele, ElementCurve curve)
{
  /* Determine element index */
  int ele_index = 0;
  int working_ele = (int)ele;
  while(working_ele > 1)
  {
    working_ele = working_ele >> 1;
    ele_index++;
  }
  ele_index--;

  /* Confirm element index is valid */
  if(ele_index >= 0)
    return ele_index * ((int)ElementCurve::D + 1) + (int)curve;
  return 0;
}

/* Loads working info into UI objects */
void EditorPerson::loadWorkingInfo()
{
  /* ID */
  edit_id->setText(QString::number(id));

  /* Name */
  edit_name->setText(QString::fromStdString(person_curr.getName()));

  /* Primary Element */
  combo_pri->setCurrentIndex(getElementIndex(person_curr.getPrimary(),
                                             person_curr.getPrimaryCurve()));

  /* Secondary Element */
  combo_sec->setCurrentIndex(getElementIndex(person_curr.getSecondary(),
                                             person_curr.getSecondaryCurve()));

  /* Class */
  combo_class->blockSignals(true);
  combo_class->clear();
  combo_class->addItem("None");
  int class_index = -1;
  for(int i = 0; i < class_total.size(); i++)
  {
    combo_class->addItem(class_total[i]->getNameList());
    if(class_total[i]->getID() == class_id)
      class_index = i + 1;
  }
  if(class_index > 0)
    combo_class->setCurrentIndex(class_index);
  else
    class_id = -1;
  combo_class->blockSignals(false);

  /* Race */
  combo_race->blockSignals(true);
  combo_race->clear();
  combo_race->addItem("None");
  int race_index = -1;
  for(int i = 0; i < race_total.size(); i++)
  {
    combo_race->addItem(race_total[i]->getNameList());
    if(race_total[i]->getID() == race_id)
      race_index = i + 1;
  }
  if(race_index > 0)
    combo_race->setCurrentIndex(race_index);
  else
    race_id = -1;
  combo_race->blockSignals(false);

  /* Flags */
  chk_gain_xp->setChecked(person_curr.getPFlag(PState::CAN_GAIN_EXP));
  chk_lvl_up->setChecked(person_curr.getPFlag(PState::CAN_LEVEL_UP));
  chk_learn_skills->setChecked(person_curr.getPFlag(PState::CAN_LEARN_SKILLS));
  chk_change_equip->setChecked(person_curr.getPFlag(PState::CAN_CHANGE_EQUIP));

  /* First Person Sprite */
  updateFirstPerson();

  /* Third Person Sprite */
  updateThirdPerson();

  /* Dialog Sprite */
  updateDialogSprite();

  /* Action Sprite */
  updateActionSprite();

  /* Action X/Y */
  spin_actionx->setValue(person_curr.getActionX());
  spin_actiony->setValue(person_curr.getActionY());

  /* Loot Credits */
  spin_credits->setValue(person_curr.getCreditDrop());

  /* Loot Exp */
  spin_xp->setValue(person_curr.getExpDrop());

  /* Loot Items */
  list_items_all->clear();
  for(int i = 0; i < item_total.size(); i++)
    list_items_all->addItem(item_total[i]->getNameList());
  updateUsedItems();
}

/* Update used working item list */
void EditorPerson::updateUsedItems()
{
  int index = list_items_used->currentRow();
  list_items_used->clear();
  QVector<bool> item_used;
  for(int i = 0; i < item_ids.size(); i++)
    item_used.push_back(false);

  /* Loop through items */
  for(int i = 0; i < item_ids.size(); i++)
  {
    for(int j = 0; !item_used[i] && j < item_total.size(); j++)
    {
      if(item_total[j]->getID() == item_ids[i])
      {
        item_used[i] = true;
        list_items_used->addItem(item_total[j]->getNameList());
      }
    }
  }

  /* If not found, delete IDs */
  for(int i = item_ids.size() - 1; i >= 0; i--)
  {
    if(!item_used[i])
      item_ids.remove(i);
  }

  /* If index is valid, select row */
  if(index >= 0)
  {
    if(index >= list_items_used->count())
      list_items_used->setCurrentRow(list_items_used->count() - 1);
    else
      list_items_used->setCurrentRow(index);
  }
}

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/

/* Button Triggers */
void EditorPerson::btnActionSprite(bool clean_only)
{
  /* Close and delete the dialog if button is pressed */
  if(dialog_sprite != NULL)
  {
    dialog_sprite->hide();
    dialog_sprite->deleteLater();
  }
  dialog_sprite = NULL;

  /* Create new dialog */
  if(!clean_only)
  {
    dialog_sprite = new SpriteDialog(this, &sprite_as, "", 0, false,
                                     EditorEnumDb::SPRITE_ALL);
    connect(dialog_sprite, SIGNAL(ok()), this, SLOT(updateActionSprite()));
    dialog_sprite->show();
  }
}

/* Button Triggers */
void EditorPerson::btnDialogSprite(bool clean_only)
{
  /* Close and delete the dialog if button is pressed */
  if(dialog_sprite != NULL)
  {
    dialog_sprite->hide();
    dialog_sprite->deleteLater();
  }
  dialog_sprite = NULL;

  /* Create new dialog */
  if(!clean_only)
  {
    dialog_sprite = new SpriteDialog(this, &sprite_ds, "", 0, false,
                                     EditorEnumDb::SPRITE_ALL);
    connect(dialog_sprite, SIGNAL(ok()), this, SLOT(updateDialogSprite()));
    dialog_sprite->show();
  }
}

/* Button Triggers */
void EditorPerson::btnFirstPerson(bool clean_only)
{
  /* Close and delete the dialog if button is pressed */
  if(dialog_sprite != NULL)
  {
    dialog_sprite->hide();
    dialog_sprite->deleteLater();
  }
  dialog_sprite = NULL;

  /* Create new dialog */
  if(!clean_only)
  {
    dialog_sprite = new SpriteDialog(this, &sprite_fp, "", 0, false,
                                     EditorEnumDb::SPRITE_ALL);
    connect(dialog_sprite, SIGNAL(ok()), this, SLOT(updateFirstPerson()));
    dialog_sprite->show();
  }
}

/* Button Triggers */
void EditorPerson::btnItemAdd()
{
  if(list_items_all->currentRow() >= 0)
  {
    item_ids.push_back(item_total[list_items_all->currentRow()]->getID());
    qSort(item_ids);
    updateUsedItems();
  }
}

/* Button Triggers */
void EditorPerson::btnItemRemove()
{
  if(list_items_used->currentRow() >= 0)
  {
    item_ids.remove(list_items_used->currentRow());
    updateUsedItems();
  }
}

/* Button Triggers */
void EditorPerson::btnReset()
{
  resetWorking();
}

/* Button Triggers */
void EditorPerson::btnSave()
{
  saveWorking();
}

/* Button Triggers */
void EditorPerson::btnThirdPerson(bool clean_only)
{
  /* Close and delete the dialog if button is pressed */
  if(dialog_sprite != NULL)
  {
    dialog_sprite->hide();
    dialog_sprite->deleteLater();
  }
  dialog_sprite = NULL;

  /* Create new dialog */
  if(!clean_only)
  {
    dialog_sprite = new SpriteDialog(this, &sprite_tp, "", 0, false,
                                     EditorEnumDb::SPRITE_ALL);
    connect(dialog_sprite, SIGNAL(ok()), this, SLOT(updateThirdPerson()));
    dialog_sprite->show();
  }
}

/* Widget Change Triggers */
void EditorPerson::changedActionX(int value)
{
  person_curr.setActionXY(value, person_curr.getActionY());
}

/* Widget Change Triggers */
void EditorPerson::changedActionY(int value)
{
  person_curr.setActionXY(person_curr.getActionX(), value);
}

/* Widget Change Triggers */
void EditorPerson::changedClass(int index)
{
  if(index > 0)
    class_id = class_total[index-1]->getID();
  else
    class_id = -1;
}

/* Widget Change Triggers */
void EditorPerson::changedCredits(int value)
{
  person_curr.setLoot(value, person_curr.getExpDrop(),
                      person_curr.getItemDrops());
}

/* Widget Change Triggers */
void EditorPerson::changedExperience(int value)
{
  person_curr.setLoot(person_curr.getCreditDrop(), value,
                      person_curr.getItemDrops());
}

/* Widget Change Triggers */
void EditorPerson::changedFlags(int)
{
  person_curr.setPFlag(PState::CAN_CHANGE_EQUIP, chk_change_equip->isChecked());
  person_curr.setPFlag(PState::CAN_GAIN_EXP, chk_gain_xp->isChecked());
  person_curr.setPFlag(PState::CAN_LEARN_SKILLS, chk_learn_skills->isChecked());
  person_curr.setPFlag(PState::CAN_LEVEL_UP, chk_lvl_up->isChecked());
}

/* Widget Change Triggers */
void EditorPerson::changedName(QString name)
{
  setName(name);
}

/* Widget Change Triggers */
void EditorPerson::changedPrimary(QString text)
{
  QStringList split = text.split('-');
  if(split.size() == 2)
  {
    Element ele = Helpers::elementFromString(
                                     split.first().trimmed().toStdString());
    ElementCurve curve = Helpers::curveFromString(
                                     split.last().trimmed().toStdString());
    person_curr.setCurves(ele, curve, person_curr.getSecondary(),
                          person_curr.getSecondaryCurve());
  }
}

/* Widget Change Triggers */
void EditorPerson::changedRace(int index)
{
  if(index > 0)
    race_id = race_total[index-1]->getID();
  else
    race_id = -1;
}

/* Widget Change Triggers */
void EditorPerson::changedSecondary(QString text)
{
  QStringList split = text.split('-');
  if(split.size() == 2)
  {
    Element ele = Helpers::elementFromString(
                                     split.first().trimmed().toStdString());
    ElementCurve curve = Helpers::curveFromString(
                                     split.last().trimmed().toStdString());
    person_curr.setCurves(person_curr.getPrimary(),
                          person_curr.getPrimaryCurve(), ele, curve);
  }
}

/* List Index Widget Changes */
void EditorPerson::listAllIndexChanged(int index)
{
  if(index >= 0)
    btn_item_add->setEnabled(true);
  else
    btn_item_add->setDisabled(true);
}

/* List Index Widget Changes */
void EditorPerson::listUsedIndexChanged(int index)
{
  if(index >= 0)
    btn_item_rem->setEnabled(true);
  else
    btn_item_rem->setDisabled(true);
}

/* Update calls for sprites */
void EditorPerson::updateActionSprite()
{
  int img_size = kFRAME_SIZE - 5;
  QImage original = sprite_as.getImage(0);

  if(original.width() > img_size || original.height() > img_size)
  {
    QPixmap scaled_image = sprite_as.getPixmap(0, img_size, img_size);
    lbl_as_img->setPixmap(scaled_image);
  }
  else
  {
    QPixmap orig_image =
                 sprite_as.getPixmap(0, original.width(), original.height());
    lbl_as_img->setPixmap(orig_image);
  }
}

/* Update calls for sprites */
void EditorPerson::updateDialogSprite()
{
  int img_size = kFRAME_SIZE - 5;
  QImage original = sprite_ds.getImage(0);

  if(original.width() > img_size || original.height() > img_size)
  {
    QPixmap scaled_image = sprite_ds.getPixmap(0, img_size, img_size);
    lbl_ds_img->setPixmap(scaled_image);
  }
  else
  {
    QPixmap orig_image =
                 sprite_ds.getPixmap(0, original.width(), original.height());
    lbl_ds_img->setPixmap(orig_image);
  }
}

/* Update calls for sprites */
void EditorPerson::updateFirstPerson()
{
  int img_size = kFRAME_SIZE - 5;
  QImage original = sprite_fp.getImage(0);

  if(original.width() > img_size || original.height() > img_size)
  {
    QPixmap scaled_image = sprite_fp.getPixmap(0, img_size, img_size);
    lbl_fp_img->setPixmap(scaled_image);
  }
  else
  {
    QPixmap orig_image =
                 sprite_fp.getPixmap(0, original.width(), original.height());
    lbl_fp_img->setPixmap(orig_image);
  }
}

/* Update calls for sprites */
void EditorPerson::updateThirdPerson()
{
  int img_size = kFRAME_SIZE - 5;
  QImage original = sprite_tp.getImage(0);

  if(original.width() > img_size || original.height() > img_size)
  {
    QPixmap scaled_image = sprite_tp.getPixmap(0, img_size, img_size);
    lbl_tp_img->setPixmap(scaled_image);
  }
  else
  {
    QPixmap orig_image =
                 sprite_tp.getPixmap(0, original.width(), original.height());
    lbl_tp_img->setPixmap(orig_image);
  }
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
  btnActionSprite(true);

  /* Fix the data */
  person_curr = person_base;
  sprite_as = sprite_as_base;
  sprite_ds = sprite_ds_base;
  sprite_fp = sprite_fp_base;
  sprite_tp = sprite_tp_base;
  class_id = class_id_base;
  item_ids = item_ids_base;
  race_id = race_id_base;

  /* Properly load the information */
  setName(QString::fromStdString(person_curr.getName()));
  loadWorkingInfo();
}

/* Saves the object data */
void EditorPerson::save(FileHandler* fh, bool game_only)
{
  // TODO: Implementation
}

/* Saves the working set trigger */
void EditorPerson::saveWorking()
{
  btnActionSprite(true);

  /* Save the data */
  person_base = person_curr;
  sprite_as_base = sprite_as;
  sprite_ds_base = sprite_ds;
  sprite_fp_base = sprite_fp;
  sprite_tp_base = sprite_tp;
  class_id_base = class_id;
  item_ids_base = item_ids;
  race_id_base = race_id;
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
void EditorPerson::updateClasses(QVector<EditorCategory*> classes,
                                 bool update_working)
{
  class_total = classes;
  if(update_working)
    loadWorkingInfo();
}

/* Update Calls for data */
void EditorPerson::updateItems(QVector<EditorItem*> items, bool update_working)
{
  item_total = items;
  if(update_working)
    loadWorkingInfo();
}

/* Update Calls for data */
void EditorPerson::updateRaces(QVector<EditorCategory*> races,
                               bool update_working)
{
  race_total = races;
  if(update_working)
    loadWorkingInfo();
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
