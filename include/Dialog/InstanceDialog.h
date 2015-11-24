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

#include <QCheckBox>
#include <QCloseEvent>
#include <QDialog>
#include <QGridLayout>
#include <QListWidget>
#include <QMessageBox>

#include "Database/EditorMapIO.h"
#include "Database/EditorMapNPC.h"
#include "Dialog/EventDialog.h"
#include "Dialog/NodeDialog.h"
#include "View/EventSetView.h"

class InstanceDialog : public QDialog
{
  Q_OBJECT
public:
  /* Constructor function */
  InstanceDialog(EditorMapIO* edit_io, QWidget* parent = NULL);
  InstanceDialog(EditorMapNPC* edit_npc, QWidget* parent = NULL);
  InstanceDialog(EditorMapPerson* edit_person, QWidget* parent = NULL);
  InstanceDialog(EditorMapThing* edit_thing = NULL, QWidget* parent = NULL);

  /* Destructor function */
  ~InstanceDialog();

private:
  /* Check boxes for base event and interaction control */
  QCheckBox* box_base_event;
  QCheckBox* box_base_speed;
  QCheckBox* box_interaction;

  /* Button for triggering node edit */
  QPushButton* btn_edit_nodes;

  /* Combo box control for algorithms and tracking */
  QComboBox* combo_algorithm;
  QComboBox* combo_party;
  QComboBox* combo_tracking;

  /* The conversation dialog */
  //ConvoDialog* convo_dialog;

  /* Description text box */
  QTextEdit* edit_description;

  /* Event view and control */
  EventSetView* event_view;

  /* The event set dialog */
  EventDialog* event_dialog;

  /* Speed result label */
  QLabel* lbl_speed_result;

  /* The line edit for thing data */
  QLineEdit* line_name;

  /* The node list */
  QListWidget* list_nodes;

  /* The list of objects used in possible events/dialog */
  QVector<QString> list_items;
  QVector<QString> list_maps;
  QVector<QString> list_parties;
  QList<QString> list_sounds;
  QVector<QString> list_submaps;
  QVector<QString> list_things;

  /* The speed control value */
  QSpinBox* spin_speed;

  /* The working and original thing */
  EditorEnumDb::Layer thing_type;
  EditorMapThing* thing_original;
  EditorMapThing* thing_working;

  /* Waiting for sub-map data */
  //bool waiting_convo;
  bool waiting_for_submap;
  bool waiting_path;

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

  /* Update the class data */
  void updateData();

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
  /* Edit base signal */
  void editBase(EditorMapThing* thing);

  /* Ok pressed */
  void ok(QString name_list);

  /* Path edit start trigger */
  void pathEditStart(EditorNPCPath*);

  /* Select tile trigger */
  void selectTile(EditorEnumDb::MapObjectMode view);

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* Button control triggers */
  void buttonBaseEdit();
  void buttonCancel();
  void buttonEditNodes();
  void buttonOk();

  /* Changed text in line edits */
  void changedDescription();
  void changedName(QString name);

  /* Check box triggers */
  void checkBaseChange(int state);
  void checkBaseSpeed(int state);
  void checkInteractionChange(int state);

  /* Combo box triggers */
  void comboAlgorithmChange(int index);
  void comboPartyChange(int index);
  void comboTrackingChange(int index);

  /* Edit event set trigger */
  void editEventSet(EditorEventSet* set);

  /* Edit Single Node trigger */
  void editNode(QListWidgetItem*);

  /* Select tile trigger */
  void selectTile();
  //void selectTileConvo();

  /* Speed changed */
  void speedChanged(int value);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the event dialog widget */
  EventDialog* getEventDialog();

  /* Returns the list of objects, used for dialog and event creation */
  QVector<QString> getListItems();
  QVector<QString> getListMaps();
  QVector<QString> getListParties();
  QList<QString> getListSounds();
  QVector<QString> getListSubmaps();
  QVector<QString> getListThings();

  /* Sets the list of parties, used for dialog and event creation */
  void setListItems(QVector<QString> items);
  void setListMaps(QVector<QString> maps);
  void setListParties(QVector<QString> parties);
  void setListSounds(QList<QString> sounds);
  void setListSubmaps(QVector<QString> sub_maps);
  void setListThings(QVector<QString> things);

  /* Sets the working thing to the original */
  void updateOriginal();

  /* Update path finished */
  void updatePathFinished();

  /* Update the selected tile for the thing */
  void updateSelectedTile(int id, int x, int y);
};

#endif // INSTANCEDIALOG_H
