/*******************************************************************************
 * Class Name: EditorBubby
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Bubby inherited from item display and modifier.
 ******************************************************************************/
#ifndef EDITORBUBBY_H
#define EDITORBUBBY_H

#include <QWidget>

#include "Database/EditorTemplate.h"
#include "EditorHelpers.h"
#include "Game/Player/Bubby.h"

class EditorBubby : public QWidget, public EditorTemplate
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorBubby(QWidget* parent = NULL);

  /* Constructor function with id and name */
  EditorBubby(int id, QString name, QWidget* parent = NULL);

  /* Copy constructor */
  EditorBubby(const EditorBubby &source);

  /* Destructor function */
  virtual ~EditorBubby();

private:
  /* The reference bubby for data */
  Bubby bubby;

protected:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorBubby &source);

public slots:
signals:

public:
  /* Clone */
  EditorBubby* clone();

  /* Returns the ID of the bubby */
  virtual int getID() const;

  /* Returns the name of the bubby */
  virtual QString getName() const;

  /* Returns the name of the bubby for listing */
  virtual QString getNameList();

  /* Sets the ID of the bubby */
  virtual void setID(int id);

  /* Sets the name of the bubby */
  virtual void setName(QString name, bool update = true);

/* Operator functions */
public:
  /* The copy operator */
  EditorBubby& operator= (const EditorBubby &source);
};

#endif // EDITORBUBBY_H
