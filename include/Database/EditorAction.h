#ifndef EDITORACTION_H
#define EDITORACTION_H
/*******************************************************************************
 * Class Name: EditorAction
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Action
 ******************************************************************************/
#include <QWidget>
#include <QGroupBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include "Game/Player/Action.h"
class EditorAction : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorAction(QWidget* parent = NULL);

  /* Destructor function */
  ~EditorAction();

  /* Clone */
  EditorAction* clone();

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
#endif // EDITORACTION_H
