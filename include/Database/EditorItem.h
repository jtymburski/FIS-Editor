/*******************************************************************************
 * Class Name: EditorItem
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Item
 ******************************************************************************/
#ifndef EDITORITEM_H
#define EDITORITEM_H

#include <QWidget>

#include "Database/EditorTemplate.h"
#include "EditorHelpers.h"
#include "Game/Player/Item.h"

class EditorItem : public QWidget, public EditorTemplate
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorItem(QWidget* parent = NULL);

  /* Constructor function with id and name */
  EditorItem(int id, QString name, QWidget* parent = NULL);

  /* Copy constructor */
  EditorItem(const EditorItem &source);

  /* Destructor function */
  virtual ~EditorItem();

private:
  /* The reference item for data */
  Item item;

protected:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorItem &source);

public slots:
signals:

public:
  /* Clone */
  EditorItem* clone();

  /* Returns the ID of the item */
  virtual int getID();

  /* Returns the name of the item */
  virtual QString getName();

  /* Returns the name of the item for listing */
  virtual QString getNameList();

  /* Sets the ID of the item */
  virtual void setID(int id);

  /* Sets the name of the item */
  virtual void setName(QString name);

/* Operator functions */
public:
  /* The copy operator */
  EditorItem& operator= (const EditorItem &source);
};

#endif // EDITORITEM_H