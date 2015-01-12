/*******************************************************************************
 * Class Name: EditorEquipment
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Equipment for the game.
 ******************************************************************************/
#ifndef EDITOREQUIPMENT_H
#define EDITOREQUIPMENT_H

#include <QWidget>

#include "EditorHelpers.h"
#include "Game/Player/Equipment.h"

class EditorEquipment : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorEquipment(QWidget* parent = NULL);

  /* Constructor function with id and name */
  EditorEquipment(int id, QString name, QWidget* parent = NULL);

  /* Destructor function */
  ~EditorEquipment();

private:
  /* The reference equipment for data */
  Equipment equipment;

protected:
public slots:
signals:

public:
  /* Clone */
  EditorEquipment* clone();

  /* Returns the ID of the equipment */
  int getID();

  /* Returns the name of the equipment */
  QString getName();

  /* Returns the name of the equipment for listing */
  QString getNameList();

  /* Sets the ID of the equipment */
  void setID(int id);

  /* Sets the name of the equipment */
  void setName(QString name);
};

#endif // EDITOREQUIPMENT_H
