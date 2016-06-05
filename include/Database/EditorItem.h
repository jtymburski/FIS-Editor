/*******************************************************************************
 * Class Name: EditorItem
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Item
 ******************************************************************************/
#ifndef EDITORITEM_H
#define EDITORITEM_H

#include <QButtonGroup>
#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

#include "Database/EditorSkill.h"
#include "Database/EditorTemplate.h"
#include "Dialog/SpriteDialog.h"
#include "EditorHelpers.h"
#include "FileHandler.h"
#include "Game/Player/Item.h"

class EditorItem : public QWidget, public EditorTemplate
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorItem(QWidget* parent = NULL);

  /* Constructor function with id and name */
  EditorItem(int id, QString name, QWidget* parent = NULL);

  /* Copy constructor */
  EditorItem(const EditorItem &source);

  /* Destructor function */
  virtual ~EditorItem();

private:
  /* Groupings */
  QGroupBox* box_buff;
  QGroupBox* box_flags;
  QGroupBox* box_material;

  /* Button Widgets */
  QPushButton* btn_anim_click;

  /* Check Box Widgets */
  QCheckBox* chk_f_skilllearn;
  QCheckBox* chk_f_statalter;

  /* Combo Box Widgets */
  QComboBox* combo_buff_preset;
  QComboBox* combo_skill;
  QComboBox* combo_tier;
  QComboBox* combo_use;

  /* The thumbnail and animation frame(s) control dialogs */
  SpriteDialog* dialog_anim;
  FrameDialog* dialog_thumb;

  /* Label Widgets */
  QLabel* lbl_anim_img;
  QLabel* lbl_thumb_img;

  /* Line Edit Widgets */
  QLineEdit* edit_brief_desc;
  QLineEdit* edit_id;
  QLineEdit* edit_msg;
  QLineEdit* edit_name;
  QLineEdit* edit_prefix;

  /* Editor ID */
  int id;

  /* Protected flag */
  bool is_protected;

  /* Item data */
  Item item_base;
  Item item_curr;

  /* Radio Button Widgets */
  QRadioButton* radio_f_consumed;
  QRadioButton* radio_f_key;
  QRadioButton* radio_f_material;
  QRadioButton* radio_f_nocat;
  QRadioButton* radio_m_brass;
  QRadioButton* radio_m_digital;
  QRadioButton* radio_m_electric;
  QRadioButton* radio_m_fire;
  QRadioButton* radio_m_forest;
  QRadioButton* radio_m_graphene;
  QRadioButton* radio_m_ice;
  QRadioButton* radio_m_nihil;
  QRadioButton* radio_m_physical;
  QRadioButton* radio_m_steel;
  QRadioButton* radio_m_titanium;
  QRadioButton* radio_m_wood;

  /* Skill information */
  int skill_id;
  int skill_id_base;
  QVector<EditorSkill*> skill_total;

  /* Spin Box Widgets */
  QVector<QSpinBox*> spin_buffs;
  QSpinBox* spin_mass;
  QSpinBox* spin_value;

  /* Sprite data */
  EditorSprite sprite_anim;
  EditorSprite sprite_anim_base;
  EditorSprite sprite_thumb;
  EditorSprite sprite_thumb_base;

  /* Text Edit Widgets */
  QTextEdit* text_desc;

  /*------------------- Constants -----------------------*/
  const static int kFRAME_SIZE; /* Size of frame containing sprites */
  const static int kMAX_PRESETS; /* Max number of legitimate presets */

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorItem &source);

  /* Creates interface layout */
  void createLayout();

  /* Loads working info into UI objects */
  void loadWorkingInfo();

  /* Updates connected widgets with enabled and disabled statuses */
  void updateConnected();

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Data change trigger within the widget signal */
  void dataChange(ItemData);

  /* Name changed within item widget signal */
  void nameChange(QString);

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/
public slots:
  /* Button Triggers */
  void buttonAnimEdit(bool clean_only = false);
  void btnReset();
  void btnSave();
  void buttonThumbEdit(bool clean_only = false);

  /* Widget Change Triggers */
  void changedBriefDescription(QString brief);
  void changedBuffSet(int);
  void changedDescription();
  void changedElement();
  void changedFlagCat();
  void changedFlags(int);
  void changedMass(int mass);
  void changedMaterial();
  void changedMessage(QString message);
  void changedName(QString name);
  void changedPrefix(QString prefix);
  void changedSkill(int index);
  void changedTier(int index);
  void changedUse(int index);
  void changedValue(int value);

  /* Stat Preset Trigger */
  void statBuffPreset(int index);

  /* Update calls for frames and sprites */
  void updateAnimation();
  void updateThumb();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the associated critical data for the item */
  ItemData getData();

  /* Returns the ID of the item */
  virtual int getID() const;

  /* Returns the mass of the item */
  int getMass();

  /* Returns the name of the item */
  virtual QString getName() const;

  /* Returns the name of the item for listing */
  virtual QString getNameList();

  /* Loads the object data */
  void load(XmlData data, int index);

  /* Resets the working set trigger */
  void resetWorking();

  /* Saves the object data */
  void save(FileHandler* fh, bool game_only = false);

  /* Saves the working set trigger */
  void saveWorking();

  /* Sets the ID of the item */
  virtual void setID(int id);

  /* Sets the name of the item */
  virtual void setName(QString name, bool update = true);

  /* Update skills */
  void updateSkills(QVector<EditorSkill*> skills);

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  /* The copy operator */
  EditorItem& operator= (const EditorItem &source);
};

#endif // EDITORITEM_H
