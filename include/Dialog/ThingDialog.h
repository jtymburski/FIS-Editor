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
#include <QStringList>

#include "Database/EditorMapThing.h"
//#include "Dialog/ConvoDialog.h"
#include "Dialog/EventDialog.h"
#include "View/EventSetView.h"
//#include "View/EventView.h"
#include "View/MatrixView.h"

class ThingDialog : public QDialog
{
  Q_OBJECT
public:
  /* Constructor function */
  ThingDialog(EditorMapThing* edit_thing = NULL, QWidget* parent = NULL);

  /* Destructor function */
  ~ThingDialog();

private:
  /* The visible control box */
  QComboBox* box_visible;

  /* The conversation dialog */
  //ConvoDialog* convo_dialog;

  /* Combo Box sound widget */
  QComboBox* combo_sound;

  /* Event view and control */
  //EditorEvent* event_ctrl;
  EventSetView* event_view;
  //EventView* event_view;

  /* The event set dialog */
  EventDialog* event_dialog;

  /* The frame control dialog */
  FrameDialog* frame_dialog;

  /* The frame image */
  QLabel* lbl_frame_img;

  /* The line edit for thing data */
  QLineEdit* line_description;
  QLineEdit* line_name;

  /* The list of objects used in possible events */
  QVector<QString> list_items;
  QVector<QString> list_maps;
  QList<QString> list_sounds;
  QVector<QString> list_submaps;
  QVector<QString> list_things;

  /* Matrix view and control */
  MatrixView* matrix_view;

  /* Sound information, for dropdown */
  //QList<QString> sound_list;

  /* The working and original thing */
  EditorMapThing* thing_original;
  EditorMapThing* thing_working;

  /* Waiting for sub-map data */
  //bool waiting_convo;
  bool waiting_for_submap;

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

  /* Changed triggers in widgets */
  void changedDescription(QString description);
  void changedName(QString name);
  void changedSound(const QString &text);

  /* Edit conversation trigger */
  //void editConversation(Conversation* convo, bool is_option);

  /* Edit event set trigger */
  void editEventSet(EditorEventSet* set);

  /* Select tile trigger */
  void selectTile();
  //void selectTileConvo();

  /* Update the frame for the thing */
  void updateFrame();

  /* Visibility status changed */
  void visibilityChanged(int index);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the event dialog widget */
  EventDialog* getEventDialog();

  /* Returns the event view widget */
  //EventView* getEventView();

  /* Returns the list of objects, used for dialog and event creation */
  QVector<QString> getListItems();
  QVector<QString> getListMaps();
  QList<QString> getListSounds();
  QVector<QString> getListSubmaps();
  QVector<QString> getListThings();

  /* Returns the list of sounds, being used */
  //QList<QString> getListSounds();

  /* Sets the list of objects, used for dialog and event creation */
  void setListItems(QVector<QString> items);
  void setListMaps(QVector<QString> maps);
  void setListSounds(QList<QString> sounds);
  void setListSubmaps(QVector<QString> sub_maps);
  void setListThings(QVector<QString> things);

  /* Set the list of sounds, used for dropdown and in event */
  //void setListSounds(QList<QString> sounds);

  /* Sets the working thing to the original */
  void updateOriginal();

  /* Update the selected tile for the thing */
  void updateSelectedTile(int id, int x, int y);
};

#endif // THINGDIALOG_H
