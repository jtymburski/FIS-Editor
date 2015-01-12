/*******************************************************************************
 * Class Name: EditorItem
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Item
 ******************************************************************************/
#ifndef EDITORITEM_H
#define EDITORITEM_H

#include <QWidget>

#include "EditorHelpers.h"
#include "Game/Player/Item.h"

class EditorItem : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorItem(QWidget* parent = NULL);

  /* Constructor function with id and name */
  EditorItem(int id, QString name, QWidget* parent = NULL);

  /* Destructor function */
  ~EditorItem();

private:
  /* The reference item for data */
  Item item;

protected:
public slots:
signals:

public:
  /* Clone */
  EditorItem* clone();

  /* Returns the ID of the item */
  int getID();

  /* Returns the name of the item */
  QString getName();

  /* Returns the name of the item for listing */
  QString getNameList();

  /* Sets the ID of the item */
  void setID(int id);

  /* Sets the name of the item */
  void setName(QString name);
};

#endif // EDITORITEM_H
