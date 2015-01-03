#ifndef EDITORSKILLSET_H
#define EDITORSKILLSET_H
/*******************************************************************************
 * Class Name: EditorSkillset
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Skillset
 ******************************************************************************/
#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include "Game/Player/SkillSet.h"
#include "Database/EditorSkill.h"

class EditorSkillset : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorSkillset(QWidget* parent = NULL);

  /* Destructor function */
  ~EditorSkillset();

  /* Clone */
  EditorSkillset* clone();

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

};
#endif // EDITORSKILLSET_H
