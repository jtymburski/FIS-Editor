/*******************************************************************************
 * Class Name: EditorAction
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Action
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

#include "Game/Player/Skill.h"
#include "Database/EditorAction.h"

class EditorSkill : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorSkill(QWidget* parent = NULL);

  /* Destructor function */
  ~EditorSkill();

  /* Clone */
  EditorSkill* clone();

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

};
#endif // EDITORSKILL_H
