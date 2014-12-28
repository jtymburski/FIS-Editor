#ifndef EDITORPERSON_H
#define EDITORPERSON_H
/*******************************************************************************
 * Class Name: EditorPerson
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Person
 ******************************************************************************/
#include <QWidget>
class EditorPerson : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorPerson(QWidget* parent = NULL);

  /* Destructor function */
  ~EditorPerson();

  /* Clone */
  EditorPerson* clone();

private:
protected:
public slots:
signals:

};
#endif // EDITORPERSON_H
