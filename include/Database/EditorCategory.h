/*******************************************************************************
 * Class Name: EditorCategory
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: The category that defines persons and what they are capable of.
 ******************************************************************************/
#ifndef EDITORCATEGORY_H
#define EDITORCATEGORY_H

#include <QWidget>

#include "Database/EditorTemplate.h"
#include "EditorHelpers.h"
#include "Game/Player/Category.h"

class EditorCategory : public QWidget, public EditorTemplate
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorCategory(QWidget* parent = NULL);

  /* Constructor function with id and name */
  EditorCategory(int id, QString name, QWidget* parent = NULL);

  /* Destructor function */
  virtual ~EditorCategory();

private:
  /* The reference category for data */
  Category category;

protected:
public slots:
signals:

public:
  /* Clone */
  EditorCategory* clone();

  /* Returns the ID of the category */
  virtual int getID();

  /* Returns the name of the category */
  virtual QString getName();

  /* Returns the name of the category for listing */
  virtual QString getNameList();

  /* Sets the ID of the category */
  virtual void setID(int id);

  /* Sets the name of the category */
  virtual void setName(QString name);
};

#endif // EDITORCATEGORY_H
