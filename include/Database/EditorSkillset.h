/*******************************************************************************
 * Class Name: EditorSkillset
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Skillset which contains a set of Editor Skills.
 ******************************************************************************/
#ifndef EDITORSKILLSET_H
#define EDITORSKILLSET_H

#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>

#include "Database/EditorSkill.h"
#include "EditorHelpers.h"
#include "Game/Player/SkillSet.h"

class EditorSkillset : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorSkillset(QWidget* parent = NULL);

  /* Constructor function with id and name */
  EditorSkillset(int id, QString name, QWidget* parent = NULL);

  /* Destructor function */
  ~EditorSkillset();

private:
  /* Editor ID */
  int id;
  /* Editor Name */
  QString name;

  bool deletion;

  QListWidget* available_set_list;
  QListWidget* working_set_list;
  int available_skill_index;
  int working_skill_index;
  int running_skill_id;

  SkillSet base;
  SkillSet working;

  QVector<QPair<QString,EditorSkill*>* > available_set;
  QVector<QPair<QString,EditorSkill*>* > working_set;
  QVector<QPair<QString,EditorSkill*>* > base_set;
  QVector<QPair<QString,EditorSkill*>* > previous_set;
  QVector<int> working_level_set;
  QVector<int> previous_level_set;
  QVector<int> base_level_set;

  QLabel* level_label;
  QLineEdit* level_edit;

  QPushButton* add;
  QPushButton* remove;

  QPushButton* save;
  QPushButton* reset;

protected:
public slots:
  void setTotalSkillsList(QVector<QPair<QString,EditorSkill*>* > s);
  void setBaseSkillset(SkillSet s);
  void setWorkingSkillset(SkillSet s);
  void setNameAndID(QString str);
  void loadWorkingInfo();
  void setLevelBox(int);
  void addSkill();
  void removeSkill();
  void changeIndex(int);
  void setLevel(QString s);

  SkillSet getEditedSkillset();
  void resetWorkingSkillset();

signals:
  void nameChange(QString);

public:
  /* Clone */
  EditorSkillset* clone();

  /* Returns the ID of the skillset */
  int getID();

  /* Returns the name of the skillset */
  QString getName();

  /* Returns the name of the skillset for listing */
  QString getNameList();

  /* Sets the ID of the skillset */
  void setID(int id);

  /* Sets the name of the skillset */
  void setName(QString name);
};

#endif // EDITORSKILLSET_H
