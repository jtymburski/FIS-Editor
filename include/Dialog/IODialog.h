/*******************************************************************************
 * Class Name: IODialog
 * Date Created: July 15, 2015
 * Inheritance: QDialog
 * Description: The dialog that displays the map IO and the information
 *              related to it. Allows manipulation of the data.
 ******************************************************************************/
#ifndef IODIALOG_H
#define IODIALOG_H

#include <QDialog>
#include <QGridLayout>

#include "Database/EditorMapIO.h"
#include "Dialog/ConvoDialog.h"
#include "View/EventView.h"
#include "View/MatrixView.h"

class IODialog : public QDialog
{
  Q_OBJECT
public:
  /* Constructor function */
  IODialog(EditorMapIO* edit_io = NULL, QWidget* parent = NULL);

  /* Destructor function */
  ~IODialog();

private:
  /* The visible control box */
  QComboBox* box_visible;

  /* The conversation dialog */
  ConvoDialog* convo_dialog;

  /* Event view and control */
  EditorEvent* event_ctrl;
  EventView* event_view;

  /* The frame control dialog */
  FrameDialog* frame_dialog;

  /* The working and original IO */
  EditorMapIO* io_original;
  EditorMapIO* io_working;
  
  /* The frame image */
  QLabel* lbl_frame_img;

  /* The line edit for io data */
  QLineEdit* line_description;
  QLineEdit* line_name;

  /* Matrix view and control */
  MatrixView* matrix_view;

  /* Waiting for sub-map data */
  bool waiting_convo;
  bool waiting_for_submap;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Creates the layout - only called on initial construction */
  void createLayout(bool instance = false);

  /* Updates the objects with the io data */
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

  /* Select tile trigger */
  void selectTile(EditorEnumDb::MapObjectMode view);

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* Button control triggers */
  void buttonCancel();
  void buttonFrameEdit();
  void buttonOk();

  /* Changed text in line edits */
  void changedDescription(QString description);
  void changedName(QString name);

  /* Edit conversation trigger */
  void editConversation(Conversation* convo, bool is_option);

  /* Select tile trigger */
  void selectTile();
  void selectTileConvo();

  /* Update the frame for the IO */
  void updateFrame();

  /* Visibility status changed */
  void visibilityChanged(int index);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the event view widget */
  EventView* getEventView();

  /* Sets the working IO to the original */
  void updateOriginal();

  /* Update the selected tile for the IO */
  void updateSelectedTile(int id, int x, int y);
};

#endif // IODIALOG_H
