/*******************************************************************************
 * Class Name: EditorSkill
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Skill which contains the connections to edit a skill.
 ******************************************************************************/
#ifndef EDITORSKILL_H
#define EDITORSKILL_H

#include <QWidget>
#include <QListWidget>
#include <QPixmap>
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <QGroupBox>
#include <QCheckBox>
#include <QFormLayout>

#include "Database/EditorAction.h"
#include "Database/EditorTemplate.h"
#include "Dialog/SpriteDialog.h"
#include "EditorHelpers.h"
#include "Game/Player/Skill.h"

class EditorSkill : public QWidget, public EditorTemplate
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorSkill(QWidget* parent = NULL);

  /* Constructor function with id and name */
  EditorSkill(int id, QString name, QWidget* parent = NULL);

  /* Copy constructor */
  EditorSkill(const EditorSkill &source);

  /* Destructor function */
  virtual ~EditorSkill();

private:
  /* Corresponding skill */
  Skill base;
  Skill working;

  /* Editor ID */
  int id;
  /* Editor name */
  QString name;
  /* Testing string (May be removed) */
  QString test_string;

  /* The thumbnail control dialog */
  SpriteDialog* dialog_anim;
  FrameDialog* dialog_thumb;

  /* The thumbnail frame and animation sprite */
  EditorSprite sprite_anim;
  EditorSprite sprite_anim_base;
  EditorSprite sprite_thumb;
  EditorSprite sprite_thumb_base;

  QVector<QPair<QString,EditorAction*>* >* total_action_list;
  QVector<QPair<QString,EditorAction*>* >* skill_action_list;
  QVector<QPair<QString,EditorAction*>* > previous_skill_action_list;

  QLabel* lbl_anim_img;
  QLabel* lbl_thumb_img;

  QLabel* description_label;
  QTextEdit* description;

  QLabel* cost_label;
  QLineEdit* cost_edit;

  QLabel* chance_label;
  QLineEdit* chance_edit;

  QLabel* cooldown_label;
  QLineEdit* cooldown_edit;

  QLabel* value_label;
  QLineEdit* value_edit;

  QLabel* name_label;
  QLineEdit* name_edit;

  QLabel* message_label;
  QTextEdit* message_edit;

  QComboBox* primary_flag;
  QComboBox* secondary_flag;
  QComboBox* scope_flag;

  QListWidget* total_actions;
  QListWidget* skill_actions;

  QPushButton* add_action_to_skill;
  QPushButton* remove_action_from_skill;

  QGroupBox* skill_flags;
  QCheckBox* skill_offensive;
  QCheckBox* skill_defensive;
  QCheckBox* skill_neutral;
  QCheckBox* skill_altering;
  QCheckBox* skill_damaging;
  QCheckBox* skill_healing;
  QCheckBox* skill_inflicting;
  QCheckBox* skill_relieving;
  QCheckBox* skill_reviving;
  QCheckBox* skill_assigning;
  QCheckBox* skill_valid;

  QPushButton* save_skill;
  QPushButton* reset_skill;

  QHBoxLayout* top_horizontal;
  QHBoxLayout* bottom_horizontal;

  QVBoxLayout* main_layout;
  QHBoxLayout* layout;

  /* Original Action */
  //Action::Action base;

  /* Action being worked on */
  //Action::Action working;
  int action_selection;
  int skill_action_selection;

  int running_action_id;

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorSkill &source);

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  void nameChange(QString);

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/
public slots:
  /* Adds an action */
  void addAction();

  /* Animation and thumbnail frame edits */
  void buttonAnimEdit(bool clean_only = false);
  void buttonThumbEdit(bool clean_only = false);

  /* Alters the index */
  void changeIndex(int);
  void changeSkillActionIndex(int);

  /* Returns the edited working skill */
  Skill getEditedSkill();

  /* Loads the working info from skill */
  void loadWorkingInfo();

  /* Removes an action from the skill */
  void removeAction();

  /* Resets the working skill to the base skill */
  void resetWorkingSkill();

  /* Set base skill */
  void setBaseSkill(Skill);

  /* Sets name and ID, from colon delimited string */
  void setNameAndID(QString str);

  /* Sets the action list */
  void setTotalActionsList(QVector<QPair<QString,EditorAction*>* >* list);

  /* Sets the working skill */
  void setWorkingSkill(Skill);

  /* Update the thumb and animation frame for skill */
  void updateAnimation();
  void updateThumb();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the ID of the skill */
  virtual int getID() const;

  /* Returns the name of the skill */
  virtual QString getName() const;

  /* Returns the name of the skill for listing */
  virtual QString getNameList();

  /* Sets the ID of the skill */
  virtual void setID(int id);

  /* Sets the name of the skill */
  virtual void setName(QString name);

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  /* The copy operator */
  EditorSkill& operator= (const EditorSkill &source);
};

#endif // EDITORSKILL_H
