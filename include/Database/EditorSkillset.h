#ifndef EDITORSKILLSET_H
#define EDITORSKILLSET_H
/*******************************************************************************
 * Class Name: EditorAction
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Action
 ******************************************************************************/
#include <QWidget>
#include "Game/Player/SkillSet.h"
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
  /* Editor name */
  QString name;
  /* Testing string (May be removed) */
  QString test_string;

  /* Original Action */
 // Action::Action base;

  /* Action being worked on */
 // Action::Action working;


protected:
public slots:
signals:

};
#endif // EDITORSKILLSET_H
