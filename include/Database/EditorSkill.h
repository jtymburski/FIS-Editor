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

  QVector<QPair<QString,EditorAction*>* >* total_action_list;
  QVector<QPair<QString,EditorAction*>* >* skill_action_list;
  QVector<QPair<QString,EditorAction*>* > previous_skill_action_list;

  QImage* animation_frame;
  QImage* thumbnail_frame;
  QLabel* animation_label;
  QLabel* thumbnail_label;

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
 // Action::Action base;

  /* Action being worked on */
 // Action::Action working;
  int action_selection;
  int skill_action_selection;

  int running_action_id;

protected:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorSkill &source);

public slots:
  /* Sets the action list */
  void setTotalActionsList(QVector<QPair<QString,EditorAction*>* >* list);

  void setNameAndID(QString str);

  /* Adds an action to the skill */
  void addAction();
  void removeAction();

  /* Alters the index */
  void changeIndex(int);
  void changeSkillActionIndex(int);

  /* Set base skill */
  void setBaseSkill(Skill);

  void setWorkingSkill(Skill);
  void loadWorkingInfo();

  Skill getEditedSkill();
  void resetWorkingSkill();

signals:
  void nameChange(QString);

public:
  /* Clone */
  EditorSkill* clone();

  /* Returns the ID of the skill */
  virtual int getID();

  /* Returns the name of the skill */
  virtual QString getName();

  /* Returns the name of the skill for listing */
  virtual QString getNameList();

  /* Sets the ID of the skill */
  virtual void setID(int id);

  /* Sets the name of the skill */
  virtual void setName(QString name);

/* Operator functions */
public:
  /* The copy operator */
  EditorSkill& operator= (const EditorSkill &source);
};

#endif // EDITORSKILL_H
