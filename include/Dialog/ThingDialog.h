/*******************************************************************************
 * Class Name: ThingDialog
 * Date Created: February 8, 2015
 * Inheritance: QDialog
 * Description: The dialog that displays the map thing and the information
 *              related to it. Allows manipulation of the data.
 ******************************************************************************/
#ifndef THINGDIALOG_H
#define THINGDIALOG_H

#include <QDialog>
#include <QGridLayout>

#include "Database/EditorThing.h"
#include "View/MatrixView.h"

class ThingDialog : public QDialog
{
  Q_OBJECT
public:
  /* Constructor function */
  ThingDialog(EditorThing* edit_thing = NULL, QWidget* parent = NULL);

  /* Destructor function */
  ~ThingDialog();

private:
  /* Matrix view and control */
  MatrixView* matrix_view;

  /* The working and original thing */
  EditorThing* thing_original;
  EditorThing* thing_working;

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Custom close event */
  void closeEvent(QCloseEvent *);

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Ok pressed */
  void ok();

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* Destroys the current working sprite */
  //void destroyWorkingSprite();

  /* Updates the current working sprite */
  //void updateWorkingSprite();
};

#endif // THINGDIALOG_H
