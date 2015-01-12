/*******************************************************************************
 * Class Name: EditorBubby
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Bubby inherited from item display and modifier.
 ******************************************************************************/
#ifndef EDITORBUBBY_H
#define EDITORBUBBY_H

#include <QWidget>

#include "EditorHelpers.h"
#include "Game/Player/Bubby.h"

class EditorBubby : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorBubby(QWidget* parent = NULL);

  /* Constructor function with id and name */
  EditorBubby(int id, QString name, QWidget* parent = NULL);

  /* Destructor function */
  ~EditorBubby();

private:
  /* The reference bubby for data */
  Bubby bubby;

protected:
public slots:
signals:

public:
  /* Clone */
  EditorBubby* clone();

  /* Returns the ID of the bubby */
  int getID();

  /* Returns the name of the bubby */
  QString getName();

  /* Returns the name of the bubby for listing */
  QString getNameList();

  /* Sets the ID of the bubby */
  void setID(int id);

  /* Sets the name of the bubby */
  void setName(QString name);
};

#endif // EDITORBUBBY_H
