#ifndef EDITORBUBBY_H
#define EDITORBUBBY_H
/*******************************************************************************
 * Class Name: EditorAction
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Action
 ******************************************************************************/
#include <QWidget>
#include "Game/Player/Bubby.h"
class EditorBubby : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorBubby(QWidget* parent = NULL);

  /* Destructor function */
  ~EditorBubby();

  /* Clone */
  EditorBubby* clone();

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
#endif // EDITORBUBBY_H
