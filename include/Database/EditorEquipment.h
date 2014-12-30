#ifndef EDITOREQUIPMENT_H
#define EDITOREQUIPMENT_H
/*******************************************************************************
 * Class Name: EditorAction
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Action
 ******************************************************************************/
#include <QWidget>
#include "Game/Player/Equipment.h"
class EditorEquipment : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorEquipment(QWidget* parent = NULL);

  /* Destructor function */
  ~EditorEquipment();

  /* Clone */
  EditorEquipment* clone();

private:
  /* Editor ID */
  int id;
  /* Editor name */
  QString name;
  /* Testing string (May be removed) */
  QString test_string;

  /* Original Action */
 // Action::Action base;

  /* Action being worked on */
 // Action::Action working;


protected:
public slots:
signals:

};
#endif // EDITOREQUIPMENT_H
