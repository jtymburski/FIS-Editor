/*******************************************************************************
 * Class Name: EditorParty
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Party
 ******************************************************************************/
#ifndef EDITORPARTY_H
#define EDITORPARTY_H

#include <QWidget>

#include "Database/EditorTemplate.h"
#include "EditorHelpers.h"
#include "Game/Player/Party.h"

class EditorParty : public QWidget, public EditorTemplate
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorParty(QWidget* parent = NULL);

  /* Constructor function with id and name */
  EditorParty(int id, QString name, QWidget* parent = NULL);

  /* Copy constructor */
  EditorParty(const EditorParty &source);

  /* Destructor function */
  virtual ~EditorParty();

private:
  /* The name of the map set */
  QString name;

  /* The reference party for data */
  Party party;

protected:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorParty &source);

public slots:
signals:
public:
  /* Clone */
  EditorParty* clone();

  /* Returns the ID of the party */
  virtual int getID();

  /* Returns the name of the party */
  virtual QString getName();

  /* Returns the name of the party for listing */
  virtual QString getNameList();

  /* Sets the ID of the party */
  virtual void setID(int id);

  /* Sets the name of the party */
  virtual void setName(QString name);

/* Operator functions */
public:
  /* The copy operator */
  EditorParty& operator= (const EditorParty &source);
};

#endif // EDITORPARTY_H
