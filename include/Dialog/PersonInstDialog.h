/*******************************************************************************
 * Class Name: PersonInstDialog
 * Date Created: April 1, 2015
 * Inheritance: QDialog
 * Description: The dialog that displays the map person instant and the 
 *              information related to it. Allows manipulation of the data. 
 *              It also contains the path information used for map npcs.
 ******************************************************************************/
#ifndef PERSONINSTDIALOG_H
#define PERSONINSTDIALOG_H

#include <QCloseEvent>
#include <QDialog>
#include <QGridLayout>

#include "Database/EditorMapNPC.h"
#include "Database/EditorMapPerson.h"
#include "Dialog/ConvoDialog.h"
#include "View/EventView.h"

class PersonInstDialog : public QDialog
{
  Q_OBJECT
public:
  /* Constructor function */
  PersonInstDialog(EditorMapPerson* edit_person = NULL, QWidget* parent = NULL);
  PersonInstDialog(EditorMapNPC* edit_npc = NULL, QWidget* parent = NULL);

  /* Destructor function */
  ~PersonInstDialog();

private:
  /* The visible control box */
  //QComboBox* box_visible;

  /* The conversation dialog */
  ConvoDialog* convo_dialog;

  /* Event view and control */
  EditorEvent* event_ctrl;
  EventView* event_view;

  /* The frame control dialog */
  //FrameDialog* frame_dialog;

  /* Returns if the thing is an npc */
  bool is_npc;

  /* The frame image */
  //QLabel* lbl_frame_img;

  /* The line edit for thing data */
  QLineEdit* line_description;
  QLineEdit* line_name;

  /* Speed result label */
  //QLabel* lbl_speed_result;

  /* Matrix control */
  //Direction matrix_direction;
  //MapPerson::SurfaceClassifier matrix_surface;

  /* Matrix view and control */
  //MatrixView* matrix_view;

  /* The working and original person */
  EditorMapPerson* person_original;
  EditorMapPerson* person_working;

  /* The speed control value */
  //QSpinBox* spin_speed;

  /* Waiting for sub-map data */
  bool waiting_convo;
  bool waiting_for_submap;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Creates the layout - only called on initial construction */
  void createLayout();

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

  /* Select tile trigger */
  void selectTile(EditorEnumDb::MapViewMode view);

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* Button control triggers */
  void buttonCancel();
  void buttonOk();

  /* Changed text in line edits */
  void changedDescription(QString description);
  void changedName(QString name);

  /* Edit conversation trigger */
  void editConversation(Conversation* convo, bool is_option);

  /* Select tile trigger */
  void selectTile();
  void selectTileConvo();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the event view widget */
  EventView* getEventView();

  /* Sets the working thing to the original */
  void updateOriginal();

  /* Update the selected tile for the thing */
  void updateSelectedTile(int id, int x, int y);
};

#endif // PERSONINSTDIALOG_H
