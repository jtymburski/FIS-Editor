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
#include <QTime>

#include "Database/EditorMapThing.h"
#include "Dialog/EventDialog.h"
#include "View/EventSetView.h"
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

  /* Combo Box sound widget */
  QComboBox* combo_sound;

  /* Event view and control */
  EventSetView* event_view;

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
  QList<QString> list_items;
  QList<QString> list_maps;
  QList<QPair<QString,QString>> list_map_ios;
  QList<QString> list_map_items;
  QList<QString> list_map_npcs;
  QList<QString> list_map_persons;
  QList<QString> list_map_things;
  QList<QString> list_sounds;
  QList<QString> list_submaps;

  /* Matrix view and control */
  MatrixView* matrix_view;

  /* The working and original thing */
  EditorMapThing* thing_original;
  EditorMapThing* thing_working;

  /* Waiting for sub-map data */
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

  /* Edit event set trigger */
  void editEventSet(EditorEventSet* set);

  /* Select tile trigger */
  void selectTile();

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

  /* Returns the list of objects, used for dialog and event creation */
  QList<QString> getListItems();
  QList<QString> getListMaps();
  QList<QPair<QString,QString>> getListMapIOs();
  QList<QString> getListMapItems();
  QList<QString> getListMapNPCs();
  QList<QString> getListMapPersons();
  QList<QString> getListMapThings();
  QList<QString> getListSounds();
  QList<QString> getListSubmaps();

  /* Sets the list of objects, used for dialog and event creation */
  void setListItems(QList<QString> items);
  void setListMaps(QList<QString> maps);
  void setListMapThings(QList<QString> things,
                        QList<QPair<QString,QString>> ios,
                        QList<QString> items, QList<QString> persons,
                        QList<QString> npcs);
  void setListSounds(QList<QString> sounds);
  void setListSubmaps(QList<QString> sub_maps);

  /* Sets the working thing to the original */
  void updateOriginal();

  /* Update the selected tile for the thing */
  void updateSelectedTile(int id, int x, int y);
};

#endif // THINGDIALOG_H
