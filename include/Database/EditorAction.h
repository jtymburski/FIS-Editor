/*******************************************************************************
 * Class Name: EditorAction
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Action to interface with game action.
 ******************************************************************************/
#ifndef EDITORACTION_H
#define EDITORACTION_H

#include <QWidget>
#include <QGroupBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#include "Database/EditorTemplate.h"
#include "EditorHelpers.h"
#include "Game/Player/Action.h"

class EditorAction : public QWidget, public EditorTemplate
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorAction(QWidget* parent = NULL);

  /* Constructor function with id and name */
  EditorAction(int id, QString name, QWidget* parent = NULL);

  /* Copy constructor */
  EditorAction(const EditorAction &source);

  /* Destructor function */
  virtual ~EditorAction();

private:
  /* Editor name */
  QString name;
  QString full_name;

  /* Testing string (May be removed) */
  QString test_string;

  /* Original Action */
  Action base;

  /* Action being worked on */
  Action working;

  /* Display elements */
  /* Main Layout */
  QHBoxLayout* main_layout;
  QGridLayout* text_layout;
  QLabel* name_label;
  QLabel* value_label;
  QLabel* chance_label;
  QLabel* variance_label;
  QLabel* minduration_label;
  QLabel* maxduration_label;

  QLineEdit* name_edit;
  QLineEdit* value_edit;
  QLineEdit* chance_edit;
  QLineEdit* variance_edit;
  QLineEdit* minduration_edit;
  QLineEdit* maxduration_edit;

  /* Left Layout */
  QVBoxLayout* left_layout;
  /* Right Layout */
  QVBoxLayout* right_layout;

  /* Boxes Layout */
  QHBoxLayout* boxes_layout;
  /* Groupbox for the action flags */
  QGroupBox* action_flags_box;
  /* Action flags */
  QRadioButton* action_flags_damage;
  QRadioButton* action_flags_alter;
  QRadioButton* action_flags_alterflip;
  QRadioButton* action_flags_inflict;
  QRadioButton* action_flags_relieve;
  QRadioButton* action_flags_assign;
  QRadioButton* action_flags_assignflip;
  QRadioButton* action_flags_revive;

  QCheckBox* action_flags_base_pc;
  QCheckBox* action_flags_vari_pc;
  //QRadioButton* action_flags_valid;

  /* Action flags layout */
  QVBoxLayout* action_flags_layout;

  /* Groupbox for the ignore attack flags */
  QGroupBox* ignoreatk_flags_box;
  /* Ignore Attack flags */
  QCheckBox* ignoreatk_flags_physical;
  QCheckBox* ignoreatk_flags_thermal;
  QCheckBox* ignoreatk_flags_polar;
  QCheckBox* ignoreatk_flags_primal;
  QCheckBox* ignoreatk_flags_charged;
  QCheckBox* ignoreatk_flags_nihil;
  QCheckBox* ignoreatk_flags_luck;
  QCheckBox* ignoreatk_flags_cybernetic;
  QCheckBox* ignoreatk_flags_armor;
  /* Action flags layout */
  QVBoxLayout* ignoreatk_flags_layout;

  /* Groupbox for the ignore attack flags */
  QGroupBox* ignoredef_flags_box;
  /* Ignore Attack flags */
  QCheckBox* ignoredef_flags_physical;
  QCheckBox* ignoredef_flags_thermal;
  QCheckBox* ignoredef_flags_polar;
  QCheckBox* ignoredef_flags_primal;
  QCheckBox* ignoredef_flags_charged;
  QCheckBox* ignoredef_flags_nihil;
  QCheckBox* ignoredef_flags_luck;
  QCheckBox* ignoredef_flags_cybernetic;
  QCheckBox* ignoredef_flags_armor;
  /* Action flags layout */
  QVBoxLayout* ignoredef_flags_layout;

  /* Boxes Layout */
  QHBoxLayout* radio_layout;
  /* Groupbox for the ailment flags */
  QGroupBox* ailment_box;
  /* user flags */
  QRadioButton* ailment_none;
  QRadioButton* ailment_poison;
  QRadioButton* ailment_burn;
  QRadioButton* ailment_scald;
  QRadioButton* ailment_char;
  QRadioButton* ailment_berserk;
  QRadioButton* ailment_confuse;
  QRadioButton* ailment_silence;
  QRadioButton* ailment_bubbify;
  QRadioButton* ailment_deathtimer;
  QRadioButton* ailment_paralysis;
  QRadioButton* ailment_blindness;
  QRadioButton* ailment_dreadstruck;
  QRadioButton* ailment_dreamsnare;
  QRadioButton* ailment_hellbound;
  QRadioButton* ailment_bond;
  QRadioButton* ailment_bonded;
  QRadioButton* ailment_allatkbuff;
  QRadioButton* ailment_alldefbuff;
  QRadioButton* ailment_physicalbuff;
  QRadioButton* ailment_thermalbuff;
  QRadioButton* ailment_polarbuff;
  QRadioButton* ailment_primalbuff;
  QRadioButton* ailment_chargedbuff;
  QRadioButton* ailment_cyberneticbuff;
  QRadioButton* ailment_nihilbuff;
  QRadioButton* ailment_limbertudebuff;
  QRadioButton* ailment_unbearabilitybuff;
  QRadioButton* ailment_vitalitybuff;
  QRadioButton* ailment_qdbuff;
  QRadioButton* ailment_rootbound;
  QRadioButton* ailment_doublecast;
  QRadioButton* ailment_triplecast;
  QRadioButton* ailment_halfcost;
  QRadioButton* ailment_reflect;
  QRadioButton* ailment_hibernation;
  QRadioButton* ailment_curse;
  QRadioButton* ailment_metatether;
  QRadioButton* ailment_stubulate;
  QRadioButton* ailment_modulate;
  QRadioButton* ailment_invalid;
  /* Ailment layout */
  QVBoxLayout* ailment_layout;
  QVBoxLayout* ailment_layout2;
  QHBoxLayout* total_ailment_layout;

  /* Groupbox for the user attribute flags */
  QGroupBox* user_flags_box;
  /* User flags */
  QRadioButton* user_flags_VITA;
  QRadioButton* user_flags_QTDR;
  QRadioButton* user_flags_PHAG;
  QRadioButton* user_flags_PHFD;
  QRadioButton* user_flags_THAG;
  QRadioButton* user_flags_THFD;
  QRadioButton* user_flags_PRAG;
  QRadioButton* user_flags_PRFD;
  QRadioButton* user_flags_POAG;
  QRadioButton* user_flags_POFD;
  QRadioButton* user_flags_CHAG;
  QRadioButton* user_flags_CHFD;
  QRadioButton* user_flags_CYAG;
  QRadioButton* user_flags_CYFD;
  QRadioButton* user_flags_NIAG;
  QRadioButton* user_flags_NIFD;
  QRadioButton* user_flags_MMNT;
  QRadioButton* user_flags_LIMB;
  QRadioButton* user_flags_UNBR;
  QRadioButton* user_flags_MANN;
  QRadioButton* user_flags_NOAT;
  /* User flags layout */
  QVBoxLayout* user_flags_layout;

  /* Groupbox for the user attribute flags */
  QGroupBox* target_flags_box;
  /* Target flags */
  QRadioButton* target_flags_VITA;
  QRadioButton* target_flags_QTDR;
  QRadioButton* target_flags_PHAG;
  QRadioButton* target_flags_PHFD;
  QRadioButton* target_flags_THAG;
  QRadioButton* target_flags_THFD;
  QRadioButton* target_flags_PRAG;
  QRadioButton* target_flags_PRFD;
  QRadioButton* target_flags_POAG;
  QRadioButton* target_flags_POFD;
  QRadioButton* target_flags_CHAG;
  QRadioButton* target_flags_CHFD;
  QRadioButton* target_flags_CYAG;
  QRadioButton* target_flags_CYFD;
  QRadioButton* target_flags_NIAG;
  QRadioButton* target_flags_NIFD;
  QRadioButton* target_flags_MMNT;
  QRadioButton* target_flags_LIMB;
  QRadioButton* target_flags_UNBR;
  QRadioButton* target_flags_MANN;
  QRadioButton* target_flags_NOAT;
  /* Target flags layout */
  QVBoxLayout* target_flags_layout;
  QHBoxLayout* buttons_layout;

protected:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorAction &source);

public slots:
  /* Sets the name */
  void setNameAndID(QString str);

  /* Update Layouts */
  void updateLayouts();

  /* Get/Set for base action */
  void setBaseAction(Action);
  Action getBaseAction();

  /* Get/Set for working action */
  void setWorkingAction(Action);
  Action getWorkingAction();

  /* Load info from working file */
  void loadWorkingInfo();

  /* Load info from base file */
  void resetInfo();

  /* Save info to base file and return it */
  Action getEditedAction();

  /* Returns the Attribute the action may alter/assign for user and target */
  Attribute getUserAttribute() const;
  Attribute getTargetAttribute() const;

  /* Returns the infliction the action may inflict/relieve */
  Infliction getAilment() const;

  /* Returns the base change */
  int getBase() const;

  /* Return the chance of the action occuring */
  float getChance() const;

  /* Methods for returning the min and max portions of the duration */
  int getMin() const;
  int getMax() const;

  /* Returns the variance of the Action */
  int getVariance() const;

  /* The output string (to store in file) */
  QString outputString();

  /* Sets an action flag */
  void setActionFlag(ActionFlags set_flag, bool set);

  /* Sets the ailment and duration */
  void setAilment(Infliction ailment);
  bool setAilmentDuration(int min, int max);

  /* Sets the attributes */
  void setAttributeTarget(Attribute target);
  void setAttributeUser(Attribute user);

  /* Sets the base value and variance (amount or pc in flag) to change
   * attribute by */
  void setBaseValue(int value, bool percent = false);
  void setBaseVariance(int variance, bool percent = false);

  /* Sets the chance of the action occuring */
  bool setChance(float chance);

  /* Sets the ignore flags for attack and defense */
  void setIgnoreAttack(IgnoreFlags flag, bool set);
  void setIgnoreDefense(IgnoreFlags flag, bool set);

signals:
  void nameChange(QString);

public:
  /* Clone */
  EditorAction* clone();

  /* Returns the ID of the Action */
  virtual int getID();

  /* Returns the name of the item */
  virtual QString getName();

  /* Returns the name of the item for listing */
  virtual QString getNameList();

  /* Sets the ID */
  virtual void setID(int id);

  /* Sets the name */
  virtual void setName(QString name);

/* Operator functions */
public:
  /* The copy operator */
  EditorAction& operator= (const EditorAction &source);
};

#endif // EDITORACTION_H
