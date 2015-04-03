/*******************************************************************************
 * Class Name: InstanceDialog
 * Date Created: April 1, 2015
 * Inheritance: QDialog
 * Description: The dialog that displays the map thing instant and the
 *              information related to it. Allows manipulation of the data. 
 *              It also contains the path information used for map npcs.
 ******************************************************************************/
#ifndef INSTANCEDIALOG_H
#define INSTANCEDIALOG_H

#include <QCloseEvent>
#include <QDialog>
#include <QGridLayout>

#include "Database/EditorMapNPC.h"
#include "Dialog/ConvoDialog.h"
#include "View/EventView.h"

class InstanceDialog : public QDialog
{
  Q_OBJECT
public:
  /* Constructor function */
  InstanceDialog(EditorMapThing* edit_thing = NULL, QWidget* parent = NULL);
  InstanceDialog(EditorMapPerson* edit_person, QWidget* parent = NULL);
  InstanceDialog(EditorMapNPC* edit_npc, QWidget* parent = NULL);

  /* Destructor function */
  ~InstanceDialog();

private:
  /* Check boxes for base event and interaction control */
  QCheckBox* box_base_event;
  QCheckBox* box_interaction;

  /* Button for triggering node edit */
  QPushButton* btn_edit_nodes;

  /* Combo box control for algorithms and tracking */
  QComboBox* combo_algorithm;
  QComboBox* combo_tracking;

  /* The conversation dialog */
  ConvoDialog* convo_dialog;

  /* Description text box */
  QTextEdit* edit_description;

  /* Event view and control */
  EditorEvent* event_ctrl;
  EventView* event_view;

  /* The line edit for thing data */
  QLineEdit* line_name;

  /* The node list */
  QListWidget* list_nodes;

  /* The working and original thing */
  EditorEnumDb::Layer thing_type;
  EditorMapThing* thing_original;
  EditorMapThing* thing_working;

  /* Waiting for sub-map data */
  bool waiting_convo;
  bool waiting_for_submap;

  /* Constants */
  static const int kALGO_COUNT;
  static const std::string kALGO_STATES[];
  static const int kTRACK_COUNT;
  static const std::string kTRACK_STATES[];

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Creates the layout - only called on initial construction */
  void createLayout();

  /* Called on initial set-up of the class */
  void setup();

  /* Updates the list of nodes */
  void updateNodes();

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
  void buttonEditNodes();
  void buttonOk();

  /* Changed text in line edits */
  void changedDescription();
  void changedName(QString name);

  /* Check box triggers */
  void checkBaseChange(int state);
  void checkInteractionChange(int state);

  /* Combo box triggers */
  void comboAlgorithmChange(int index); // TODO
  void comboTrackingChange(int index); // TODO

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

#endif // INSTANCEDIALOG_H
