#ifndef EDITORSKILL_H
#define EDITORSKILL_H
/*******************************************************************************
 * Class Name: EditorAction
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Action
 ******************************************************************************/
#include <QWidget>
#include "Game/Player/Skill.h"
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
#endif // EDITORSKILL_H
