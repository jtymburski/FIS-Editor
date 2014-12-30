#ifndef EDITORCATEGORY_H
#define EDITORCATEGORY_H
/*******************************************************************************
 * Class Name: EditorAction
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Action
 ******************************************************************************/
#include <QWidget>
#include "Game/Player/Category.h"
class EditorCategory : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorCategory(QWidget* parent = NULL);

  /* Destructor function */
  ~EditorCategory();

  /* Clone */
  EditorCategory* clone();

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
#endif // EDITORCATEGORY_H
