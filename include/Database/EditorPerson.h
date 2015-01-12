/*******************************************************************************
 * Class Name: EditorPerson
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Person
 ******************************************************************************/
#ifndef EDITORPERSON_H
#define EDITORPERSON_H

#include <QWidget>

#include "Database/EditorTemplate.h"
#include "EditorHelpers.h"
#include "Game/Player/Person.h"

class EditorPerson : public QWidget, public EditorTemplate
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorPerson(QWidget* parent = NULL);

  /* Constructor function with id and name */
  EditorPerson(int id, QString name, QWidget* parent = NULL);

  /* Destructor function */
  virtual ~EditorPerson();

private:
  /* The reference person for data */
  Person person;

protected:
public slots:
signals:

public:
  /* Clone */
  EditorPerson* clone();

  /* Returns the ID of the person */
  virtual int getID();

  /* Returns the name of the person */
  virtual QString getName();

  /* Returns the name of the person for listing */
  virtual QString getNameList();

  /* Sets the ID of the person */
  virtual void setID(int id);

  /* Sets the name of the person */
  virtual void setName(QString name);
};

#endif // EDITORPERSON_H
