/*******************************************************************************
 * Class Name: EditorCategory
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: The category that defines persons and what they are capable of.
 ******************************************************************************/
#ifndef EDITORCATEGORY_H
#define EDITORCATEGORY_H

#include <QWidget>

#include "EditorHelpers.h"
#include "Game/Player/Category.h"

class EditorCategory : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorCategory(QWidget* parent = NULL);

  /* Constructor function with id and name */
  EditorCategory(int id, QString name, QWidget* parent = NULL);

  /* Destructor function */
  ~EditorCategory();

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
  int getID();

  /* Returns the name of the category */
  QString getName();

  /* Returns the name of the category for listing */
  QString getNameList();

  /* Sets the ID of the category */
  void setID(int id);

  /* Sets the name of the category */
  void setName(QString name);
};

#endif // EDITORCATEGORY_H
