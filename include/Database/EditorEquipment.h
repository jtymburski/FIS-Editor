/*******************************************************************************
 * Class Name: EditorEquipment
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Equipment for the game.
 ******************************************************************************/
#ifndef EDITOREQUIPMENT_H
#define EDITOREQUIPMENT_H

#include <QWidget>

#include "Database/EditorTemplate.h"
#include "EditorHelpers.h"
#include "Game/Player/Equipment.h"

class EditorEquipment : public QWidget, public EditorTemplate
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorEquipment(QWidget* parent = NULL);

  /* Constructor function with id and name */
  EditorEquipment(int id, QString name, QWidget* parent = NULL);

  /* Copy constructor */
  EditorEquipment(const EditorEquipment &source);

  /* Destructor function */
  virtual ~EditorEquipment();

private:
  /* The reference equipment for data */
  Equipment equipment;

protected:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorEquipment &source);

public slots:
signals:

public:
  /* Clone */
  EditorEquipment* clone();

  /* Returns the ID of the equipment */
  virtual int getID() const;

  /* Returns the name of the equipment */
  virtual QString getName() const;

  /* Returns the name of the equipment for listing */
  virtual QString getNameList();

  /* Sets the ID of the equipment */
  virtual void setID(int id);

  /* Sets the name of the equipment */
  virtual void setName(QString name, bool update = true);

/* Operator functions */
public:
  /* The copy operator */
  EditorEquipment& operator= (const EditorEquipment &source);
};

#endif // EDITOREQUIPMENT_H
