/*******************************************************************************
 * Class Name: EditorParty
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Party
 ******************************************************************************/
#ifndef EDITORPARTY_H
#define EDITORPARTY_H

#include <QWidget>

#include "EditorHelpers.h"
#include "Game/Player/Party.h"

class EditorParty : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorParty(QWidget* parent = NULL);

  /* Constructor function with id and name */
  EditorParty(int id, QString name, QWidget* parent = NULL);

  /* Destructor function */
  ~EditorParty();

private:
  /* The name of the map set */
  QString name;

  /* The reference party for data */
  Party party;

protected:
public slots:
signals:
public:
  /* Clone */
  EditorParty* clone();

  /* Returns the ID of the party */
  int getID();

  /* Returns the name of the party */
  QString getName();

  /* Returns the name of the party for listing */
  QString getNameList();

  /* Sets the ID of the party */
  void setID(int id);

  /* Sets the name of the party */
  void setName(QString name);
};

#endif // EDITORPARTY_H
