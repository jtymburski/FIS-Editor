/*******************************************************************************
 * Class Name: ItemDialog
 * Date Created: June 28, 2015
 * Inheritance: QDialog
 * Description: The dialog that displays the map item and the information
 *              related to it. Allows manipulation of the data.
 ******************************************************************************/
#ifndef ITEMDIALOG_H
#define ITEMDIALOG_H

#include <QDialog>
#include <QGridLayout>

#include "Database/EditorMapItem.h"
#include "View/MatrixView.h"

class ItemDialog : public QDialog
{
  Q_OBJECT
public:
  /* Constructor function */
  ItemDialog(EditorMapItem* edit_item = NULL, QWidget* parent = NULL);

  /* Destructor function */
  ~ItemDialog();

private:
  /* The core item selection box */
  QComboBox* box_core;

  /* The visible control box */
  QComboBox* box_visible;

  /* Is the item a walkover? */
  QComboBox* box_walkover;

  /* Combo Box Widget for Sound selection */
  QComboBox* combo_sound;

  /* The frame control dialog */
  FrameDialog* frame_dialog;

  /* The working and original item */
  EditorMapItem* item_original;
  EditorMapItem* item_working;

  /* The list of objects used in possible events */
  QList<QString> list_items;

  /* The line edit for thing data */
  QLineEdit* line_description;
  QLineEdit* line_name;

  /* Matrix view and control */
  MatrixView* matrix_view;

  /* Sound information, for dropdown */
  QList<QString> sound_list;

public:
  /*------------------- Constants -----------------------*/
  //const static int kMAX_COUNT; /* Maximum number of items */

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Creates the layout - only called on initial construction */
  void createLayout(bool instance = false);

  /* Updates the objects with the thing data */
  void updateData();

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Custom close event */
  void closeEvent(QCloseEvent* event);

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
  /* Button control triggers */
  void buttonCancel();
  void buttonFrameEdit();
  void buttonOk();

  /* Changed triggers in widgets */
  void changedCount(int count);
  void changedDescription(QString description);
  void changedName(QString name);
  void changedSound(const QString &text);

  /* Core item status changed */
  void coreItemChanged(int index);

  /* Visibility status changed */
  void visibilityChanged(int index);

  /* Walkover status changed */
  void walkoverChanged(int index);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the list of sounds, being used */
  QList<QString> getListSounds();

  /* Sets the list of items, used for event creation */
  void setListItems(QList<QString> items);

  /* Set the list of sounds, used for dropdown and in event */
  void setListSounds(QList<QString> sounds);

  /* Sets the working item to the original */
  void updateOriginal();
};

#endif // ITEMDIALOG_H
