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

/*
 * Description: Main constructor function. All parameters have null defaults.
 *
 * Inputs: QWidget* parent - the parent widget. Default to NULL
 */
EditorSkill::EditorSkill(QWidget *parent) : QWidget(parent)
{
  /* Dialogs for animation and thumbs */
  dialog_anim = NULL;
  dialog_thumb = NULL;

  /* Make sure there's one frame in the thumbnail */
  if(sprite_thumb_base.frameCount() == 0)
    sprite_thumb_base.setPath(0, "");

  /* Layout */
  QGridLayout* layout = new QGridLayout(this);
  layout->setColumnStretch(7, 1);
  layout->setRowStretch(9, 1);

  /* Skill Name */
  QLabel* name_label = new QLabel("Skill Name",this);
  layout->addWidget(name_label, 0, 0);
  name_edit = new QLineEdit(this);
  layout->addWidget(name_edit, 0, 1, 1, 2);

  /* Description */
  QLabel* description_label = new QLabel("Description",this);
  layout->addWidget(description_label, 1, 0);
  description = new QTextEdit(this);
  layout->addWidget(description, 1, 1, 1, 2);

  /* String sets for categories and scopes */
  QStringList types;
  types << "Physical" << "Fire" << "Ice" << "Forest" << "Electric" << "Digital"
        << "Nihil" << "None ";
  QStringList scope;
  scope << "User" << "One Target" << "One Enemy" << "Two Enemies" <<
           "All Enemies" << "One Ally" << "One Ally (Not User)" <<
           "Two Allies" << "All Allies" << "One Fallen Ally" <<
           "All Fallen Allies" << "One Party" << "All Targets" << "Not User" <<
           "All Targets (Not User)" << "No Scope";

  /* Primary, Secondary, and Scope selectors */
  QLabel *primary_label = new QLabel("Primary Element",this);
  layout->addWidget(primary_label, 2, 0);
  QLabel *secondary_label = new QLabel("Secondary Element",this);
  layout->addWidget(secondary_label, 2, 1);
  QLabel *scope_label = new QLabel("Skill Scope",this);
  layout->addWidget(scope_label, 2, 2);
  primary_flag = new QComboBox(this);
  primary_flag->addItems(types);
  layout->addWidget(primary_flag, 3, 0);
  secondary_flag = new QComboBox(this);
  secondary_flag->addItems(types);
  layout->addWidget(secondary_flag, 3, 1);
  scope_flag = new QComboBox(this);
  scope_flag->addItems(scope);
  layout->addWidget(scope_flag, 3, 2);

  /* Skill Flags */
  QGroupBox* skill_flags = new QGroupBox("Skill Flags",this);
  QVBoxLayout* skill_flags_layout = new QVBoxLayout();
  skill_offensive = new QCheckBox("Offensive",this);
  skill_flags_layout->addWidget(skill_offensive);
  skill_defensive = new QCheckBox("Defensive",this);
  skill_flags_layout->addWidget(skill_defensive);
  skill_neutral = new QCheckBox("Neutral",this);
  skill_flags_layout->addWidget(skill_neutral);
  skill_flags_layout->addStretch(1);
  skill_flags->setLayout(skill_flags_layout);
  layout->addWidget(skill_flags, 0, 3, 2, 1);

  /* Hit Message */
  QLabel* message_label = new QLabel("Hit Message",this);
  layout->addWidget(message_label, 4, 0);
  message_edit = new QTextEdit(this);
  layout->addWidget(message_edit, 4, 1, 1, 2);

  /* QD Cost */
  QLabel* cost_label = new QLabel("QD Cost",this);
  layout->addWidget(cost_label, 5, 0);
  cost_edit = new QLineEdit(this);
  layout->addWidget(cost_edit, 5, 1);

  /* Chance */
  QLabel* chance_label = new QLabel("Chance",this);
  layout->addWidget(chance_label, 6, 0);
  chance_edit = new QLineEdit(this);
  layout->addWidget(chance_edit, 6, 1);

  /* Cooldown */
  QLabel* cooldown_label = new QLabel("Charge Time",this);
  layout->addWidget(cooldown_label, 7, 0);
  cooldown_edit = new QLineEdit(this);
  layout->addWidget(cooldown_edit, 7, 1);

  /* Value (for AI) */
  QLabel* value_label = new QLabel("Skill Value (For AI)",this);
  layout->addWidget(value_label, 8, 0);
  value_edit = new QLineEdit(this);
  layout->addWidget(value_edit, 8, 1);

  /* Thumbnail Frame */
  QLabel* lbl_thumb = new QLabel("Thumbnail", this);
  layout->addWidget(lbl_thumb, 0, 5, Qt::AlignHCenter);
  lbl_thumb_img = new QLabel(this);
  lbl_thumb_img->setMinimumSize(200, 200);
  lbl_thumb_img->setStyleSheet("border: 1px solid #b9b5b2");
  lbl_thumb_img->setAlignment(Qt::AlignCenter);
  layout->addWidget(lbl_thumb_img, 1, 5, Qt::AlignHCenter);
  QPushButton* btn_thumb_click = new QPushButton(this);
  btn_thumb_click->setIcon(QIcon(":/images/icons/32_settings.png"));
  btn_thumb_click->setIconSize(QSize(24,24));
  btn_thumb_click->setMaximumSize(30, 30);
  connect(btn_thumb_click, SIGNAL(clicked()), this, SLOT(buttonThumbEdit()));
  layout->addWidget(btn_thumb_click, 1, 5, Qt::AlignHCenter | Qt::AlignBottom);

  /* Animation Frame */
  QLabel* lbl_animation = new QLabel("Animation", this);
  layout->addWidget(lbl_animation, 0, 6, Qt::AlignHCenter);
  lbl_anim_img = new QLabel(this);
  lbl_anim_img->setMinimumSize(200, 200);
  lbl_anim_img->setStyleSheet("border: 1px solid #b9b5b2");
  lbl_anim_img->setAlignment(Qt::AlignCenter);
  layout->addWidget(lbl_anim_img, 1, 6, Qt::AlignHCenter);
  QPushButton* btn_anim_click = new QPushButton(this);
  btn_anim_click->setIcon(QIcon(":/images/icons/32_settings.png"));
  btn_anim_click->setIconSize(QSize(24,24));
  btn_anim_click->setMaximumSize(30, 30);
  connect(btn_anim_click, SIGNAL(clicked()), this, SLOT(buttonAnimEdit()));
  layout->addWidget(btn_anim_click, 1, 6, Qt::AlignHCenter | Qt::AlignBottom);

  /* Action layouts */
  QLabel* lbl_skill_avail = new QLabel("Available Actions", this);
  layout->addWidget(lbl_skill_avail, 2, 5, Qt::AlignHCenter);
  QLabel* lbl_skill_used = new QLabel("Used Actions", this);
  layout->addWidget(lbl_skill_used, 2, 6, Qt::AlignHCenter);
  total_actions = new QListWidget();
  layout->addWidget(total_actions, 3, 5, 7, 1);
  skill_actions = new QListWidget();
  layout->addWidget(skill_actions, 3, 6, 7, 1);
  add_action_to_skill = new QPushButton("Add Action",this);
  layout->addWidget(add_action_to_skill, 10, 5);
  connect(add_action_to_skill,SIGNAL(clicked()),this,SLOT(addAction()));
  remove_action_from_skill = new QPushButton("Remove Action",this);
  layout->addWidget(remove_action_from_skill, 10, 6);
  connect(remove_action_from_skill,SIGNAL(clicked()),this,SLOT(removeAction()));

  /* Spacer */
  layout->setRowMinimumHeight(11, 25);

  /* Save / Reset buttons */
  reset_skill = new QPushButton("Reset",this);
  layout->addWidget(reset_skill, 12, 5);
  connect(reset_skill,SIGNAL(clicked()),this,SLOT(resetWorkingSkill()));
  save_skill = new QPushButton("Save",this);
  layout->addWidget(save_skill, 12, 6);
  connect(save_skill,SIGNAL(clicked()),this,SLOT(getEditedSkill()));

  /* Set-up the skill */
  setBaseSkill(Skill());
  getEditedSkill();
}

/*
 * Description: Second constructor function, with ID and name parameters.
 *
 * Inputs: int id - the id of the skill
 *         QString name - the name of the skill
 *         QWidget* parent - the parent widget. Default to NULL
 */
EditorSkill::EditorSkill(int id, QString name, QWidget* parent)
           : EditorSkill(parent)
{
  setID(id);
  setName(name);
  getEditedSkill();
}

/*
 * Description: Copy constructor. Calls the blank constructor and then copies
 *              the data from the source.
 *
 * Inputs: const EditorSkill &source - the source object to copy
 */
EditorSkill::EditorSkill(const EditorSkill &source) : EditorSkill()
{
  copySelf(source);
}

/*
 * Description: Destructor function
 */
EditorSkill::~EditorSkill()
{
  resetWorkingSkill();
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: Copies all data from source editor object to this editor
 *              object.
 *
 * Inputs: EditorSkill &source - the source to copy from
 * Output: none
 */
void EditorSkill::copySelf(const EditorSkill &source)
{
  id = source.id;
  name = source.name;
  test_string = source.test_string;

  /* Action lists */
  actions_total = source.actions_total;
  actions_sel_base = source.actions_sel_base;

  /* Frame connections */
  sprite_anim_base = source.sprite_anim_base;
  sprite_thumb_base = source.sprite_thumb_base;

  setBaseSkill(source.base);
  loadWorkingInfo();
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

/*
 * Description: Adds an action to the skill. Triggered by push button. The added
 *              action is based on the selected in the left hand list widget.
 *
 * Inputs: none
 * Output: none
 */
void EditorSkill::addAction()
{
  if(total_actions->count() > 0 && total_actions->currentRow() >= 0)
  {
    QString item = total_actions->currentItem()->text();
    QStringList item_set = item.split(":");
    if(item_set.size() == 2)
    {
      int index = item_set.front().toInt();
      actions_sel.push_back(index);
      updateActions(actions_total);
    }
  }
}

/*
 * Description: Button trigger on animation sprite edit. This will open the
 *              sprite dialog for adding and removing frames as well as sprite
 *              properties.
 *
 * Inputs: bool clean_only - true if only close existing dialog. Default false
 * Output: none
 */
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

/*
 * Description: Button trigger on thumbnail frame edit. This will open the
 *              frame dialog for editing the chosen frame.
 *
 * Inputs: bool clean_only - true if only close existing dialog. Default false
 * Output: none
 */
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

/*
 * Description: Returns the base skill, stored in the class.
 *
 * Inputs: none
 * Output: Skill - the base skill class object
 */
Skill EditorSkill::getBaseSkill()
{
  return base;
}

/*
 * Description: Updates the working skill with all the GUI element contents,
 *              sets the working skill to the base skill, and returns the
 *              working skill. This is equivalent to the save call.
 *
 * Inputs: none
 * Output: Skill - the current skill data returned
 */
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

  //working.setName(name_edit->text().toStdString());
  working.setMessage(message_edit->toPlainText().toStdString());
  working.setDescription(description->toPlainText().toStdString());
  working.setCost(cost_edit->text().toInt());
  working.setChance(chance_edit->text().toFloat());
  working.setCooldown(cooldown_edit->text().toInt());
  working.setValue(value_edit->text().toInt());

  working.setFlag(SkillFlags::OFFENSIVE,skill_offensive->isChecked());
  working.setFlag(SkillFlags::DEFENSIVE,skill_defensive->isChecked());
  working.setFlag(SkillFlags::NEUTRAL,skill_neutral->isChecked());

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

  base = working;
  sprite_anim_base = sprite_anim;
  sprite_thumb_base = sprite_thumb;
  actions_sel_base = actions_sel;

  return base;
}

/*
 * Description: Loads all the GUI elements with the contents from the working
 *              Skill class.
 *
 * Inputs: none
 * Output: none
 */
void EditorSkill::loadWorkingInfo()
{
  name_edit->setText(name);
  message_edit->setText((QString::fromStdString(working.getMessage())));
  description->setText(QString::fromStdString(working.getDescription()));
  cost_edit->setText(QString::number(working.getCost()));
  chance_edit->setText(QString::number(working.getChance()));
  cooldown_edit->setText(QString::number(working.getCooldown()));
  value_edit->setText(QString::number(working.getValue()));

  skill_offensive->setChecked(working.getFlag(SkillFlags::OFFENSIVE));
  skill_defensive->setChecked(working.getFlag(SkillFlags::DEFENSIVE));
  skill_neutral->setChecked(working.getFlag(SkillFlags::NEUTRAL));

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
  updateActions(actions_total);
}

/*
 * Description: Removes an action in the skill. Triggered by push button. The
 *              removed action is based on the selected in the right hand
 *              list widget.
 *
 * Inputs: none
 * Output: none
 */
void EditorSkill::removeAction()
{
  if(skill_actions->count() > 0 && skill_actions->currentRow() >= 0)
  {
    actions_sel.remove(skill_actions->currentRow());
    updateActions(actions_total);
  }
}

/*
 * Description: Resets the working skill to the base data. This is equivalent
 *              to selecting the reset button on the UI. The UI elements are
 *              updated automatically on call.
 *
 * Inputs: none
 * Output: none
 */
void EditorSkill::resetWorkingSkill()
{
  /* Clean-up pop-ups */
  buttonAnimEdit(true);
  buttonThumbEdit(true);

  /* Fix data */
  working = base;
  sprite_anim = sprite_anim_base;
  sprite_thumb = sprite_thumb_base;
  actions_sel = actions_sel_base;

  /* Re-load working info */
  loadWorkingInfo();
}

/*
 * Description: Changes the base skill to the passed in skill.
 *
 * Inputs: Skill s - the new skill to use as a base reference
 * Output: none
 */
void EditorSkill::setBaseSkill(Skill s)
{
  base = s;
  setWorkingSkill(base);
}

/*
 * Description: Sets the name and ID of this EditorSkill class based on the
 *              colon delimited string from the parent list widget.
 *
 * Inputs: QString str - the comma delimited ID: Name
 * Output: none
 */
void EditorSkill::setNameAndID(QString str)
{
  base.setID(str.split(" : ").at(0).toInt());
  name = str.split(" : ").at(1);
  name_edit->setText(name);
  setWorkingSkill(base);
}

/*
 * Description: Revises the working skill to the passed in parameter.
 *
 * Inputs: Skill s - the new skill to use as the working copy
 * Output: none
 */
void EditorSkill::setWorkingSkill(Skill s)
{
  (void)s;
  working = base;
  sprite_anim = sprite_anim_base;
  sprite_thumb = sprite_thumb_base;
  actions_sel = actions_sel_base;

  loadWorkingInfo();
}

/*
 * Description: Slot called to trigger an update of the visible animation frame
 *              in the UI. Triggered upon any changes to the animation sprite
 *              object.
 *
 * Inputs: none
 * Output: none
 */
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

/*
 * Description: Slot called to trigger an update of the visible thumbnail frame
 *              in the UI. Triggered upon any changes to the thumbnail sprite
 *              object.
 *
 * Inputs: none
 * Output: none
 */
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

/*
 * Description: Returns the ID of the skill
 *
 * Inputs: none
 * Output: int - the ID
 */
int EditorSkill::getID() const
{
  return id;
}

/*
 * Description: Returns the name of the skill
 *
 * Inputs: none
 * Output: QString - the name
 */
QString EditorSkill::getName() const
{
  return name;
}

/*
 * Description: Get the listing name string which includes the ID and name
 *
 * Inputs: none
 * Output: QString - the name list
 */
QString EditorSkill::getNameList()
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
void EditorSkill::load(XmlData data, int index)
{
  /* Parse elements */
  if(data.getElement(index) == "name")
  {
    setName(QString::fromStdString(data.getDataString()));
  }
  else if(data.getElement(index) == "action")
  {
    actions_sel_base.push_back(data.getDataInteger());
    actions_sel = actions_sel_base;
  }
  else if(data.getElement(index) == "thumb")
  {
    sprite_thumb_base.setFramePath(0, EditorHelpers::getProjectDir() + "/" +
                                  QString::fromStdString(data.getDataString()));
  }
  else if(data.getElement(index) == "animation")
  {
    sprite_anim_base.load(data, index + 1);
  }
  else
  {
    base.loadData(data, index, NULL,
                  EditorHelpers::getProjectDir().toStdString() + "/");
  }
}

/*
 * Description: Saves the object data to the file handling pointer.
 *
 * Inputs: FileHandler* fh - the file handling pointer
 *         bool game_only - true if the data should include game only relevant
 * Output: none
 */
void EditorSkill::save(FileHandler* fh, bool game_only)
{
  (void)game_only;

  if(fh != NULL)
  {
    Skill blank;

    fh->writeXmlElement("skill", "id", getID());
    fh->writeXmlData("name", getName().toStdString());

    /* Save data */
    if(blank.getChance() != base.getChance())
      fh->writeXmlData("chance", base.getChance());
    if(blank.getCooldown() != base.getCooldown())
      fh->writeXmlData("cooldown", (int)base.getCooldown());
    if(blank.getCost() != base.getCost())
      fh->writeXmlData("cost", (int)base.getCost());
    if(blank.getDescription() != base.getDescription())
      fh->writeXmlData("description", base.getDescription());
    if(blank.getMessage() != base.getMessage())
      fh->writeXmlData("message", base.getMessage());
    if(blank.getValue() != base.getValue())
      fh->writeXmlData("value", (int)base.getValue());

    /* Primary Element*/
    QString pri = QString::fromStdString(
                                 Helpers::elementToString(base.getPrimary()));
    if(blank.getPrimary() != base.getPrimary())
      fh->writeXmlData("elem_pri", pri.toLower().toStdString());

    /* Secondary Element */
    QString sec = QString::fromStdString(
                                 Helpers::elementToString(base.getSecondary()));
    if(blank.getSecondary() != base.getSecondary())
      fh->writeXmlData("elem_sec", sec.toLower().toStdString());

    /* Scope */
    QString scope = QString::fromStdString(
                                 Helpers::actionScopeToStr(base.getScope()));
    if(blank.getScope() != base.getScope())
      fh->writeXmlData("scope", scope.toLower().toStdString());

    /* Actions */
    for(int i = 0; i < actions_sel_base.size(); i++)
      fh->writeXmlData("action", actions_sel_base[i]);

    /* Flags */
    if(base.getFlag(SkillFlags::OFFENSIVE) ||
       base.getFlag(SkillFlags::DEFENSIVE) ||
       base.getFlag(SkillFlags::NEUTRAL))
    {
      fh->writeXmlElement("flags");

      if(base.getFlag(SkillFlags::OFFENSIVE))
        fh->writeXmlData("offensive", true);
      if(base.getFlag(SkillFlags::DEFENSIVE))
        fh->writeXmlData("defensive", true);
      if(base.getFlag(SkillFlags::NEUTRAL))
        fh->writeXmlData("neutral", true);

      fh->writeXmlElementEnd();
    }

    /* Save the thumbnail */
    if(!sprite_thumb_base.isAllNull() && sprite_thumb_base.frameCount() == 1)
      fh->writeXmlData("thumb",
           EditorHelpers::trimPath(sprite_thumb_base.getPath(0)).toStdString());

    /* Save the animation data */
    if(!sprite_anim_base.isAllNull())
      sprite_anim_base.save(fh, game_only, false, "animation");

    fh->writeXmlElementEnd();
  }
}

/*
 * Description: Sets the skill ID in the base class.
 *
 * Inputs: int id - the new id
 * Output: none
 */
void EditorSkill::setID(int id)
{
  this->id = id;
}

/*
 * Description: Sets the name of the skill.
 *
 * Inputs: QString name - the name text
 * Output: none
 */
void EditorSkill::setName(QString name)
{
  this->name = name;
  name_edit->setText(name);
}

/*
 * Description: Updates the list of available actions within the skill. If this
 *              list removes actions previously used, they are removed from the
 *              working skill (only). If new ones are added, they are placed in
 *              the available actions list.
 *
 * Inputs: QVector<EditorAction*> actions - all available actions created
 * Output: none
 */
void EditorSkill::updateActions(QVector<EditorAction*> actions)
{
  actions_total = actions;
  int index_skill = skill_actions->currentRow();
  int index_total = total_actions->currentRow();

  /* Clear the lists */
  skill_actions->clear();
  total_actions->clear();

  /* Add the full action list */
  for(int i = 0; i < actions.size(); i++)
    total_actions->addItem(actions[i]->getNameList());

  /* Found stack based on ID of skill */
  QVector<bool> actions_found;
  for(int i = 0; i < actions_sel.size(); i++)
    actions_found.push_back(false);

  /* Loop through all actions in stack and add */
  for(int i = 0; i < actions_sel.size(); i++)
  {
    bool found = false;

    /* Loop through the action set and see if the ID exists */
    for(int j = 0; (j < actions.size() && !found); j++)
    {
      if(actions_sel[i] == actions[j]->getID())
      {
        found = true;
        actions_found[i] = true;
        skill_actions->addItem(actions[j]->getNameList());
      }
    }
  }

  /* Remaining IDs that weren't found, remove */
  for(int i = actions_found.size() - 1; i >= 0; i--)
  {
    if(!actions_found[i])
      actions_sel.remove(i);
  }

  /* Restore index if relevant */
  if(index_skill >= 0)
  {
    if(index_skill < skill_actions->count())
      skill_actions->setCurrentRow(index_skill);
    else
      skill_actions->setCurrentRow(skill_actions->count() - 1);
  }
  if(index_total >= 0)
  {
    if(index_total < total_actions->count())
      total_actions->setCurrentRow(index_total);
    else
      total_actions->setCurrentRow(total_actions->count() - 1);
  }
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/*
 * Description: Copy operator construction. This is called when the variable
 *              already exists and equal operator used with another
 *              EditorSkill.
 *
 * Inputs: const EditorSkill &source - the source class constructor
 * Output: EditorSkill& - pointer to the copied class
 */
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
