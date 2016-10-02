/*******************************************************************************
 * Class Name: PersonDialog
 * Date Created: March 8, 2015
 * Inheritance: QDialog
 * Description: The dialog that displays the map person and the information
 *              related to it. Allows manipulation of the data.
 ******************************************************************************/
#ifndef PERSONDIALOG_H
#define PERSONDIALOG_H

#include <QDialog>
#include <QGridLayout>

#include "Database/EditorMapPerson.h"
#include "Dialog/EventDialog.h"
#include "View/EventSetView.h"
#include "View/MatrixView.h"

class PersonDialog : public QDialog
{
  Q_OBJECT
public:
  /* Constructor function */
  PersonDialog(EditorMapPerson* edit_person = NULL, QWidget* parent = NULL);

  /* Destructor function */
  ~PersonDialog();

private:
  /* The visible control box */
  QComboBox* box_visible;

  /* Combo Box Widget for Sound */
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

  /* Speed result label */
  QLabel* lbl_speed_result;

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

  /* Matrix control */
  Direction matrix_direction;
  MapPerson::SurfaceClassifier matrix_surface;

  /* Matrix view and control */
  MatrixView* matrix_view;

  /* The working and original person */
  EditorMapPerson* person_original;
  EditorMapPerson* person_working;

  /* The speed control value */
  QSpinBox* spin_speed;

  /* Waiting for sub-map data */
  bool waiting_for_submap;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Consolidate all the matrixes together */
  void consolidateAll();

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

  /* Direction change for matrix */
  void directionChange(QString text);

  /* Edit event set trigger */
  void editEventSet(EditorEventSet* set);

  /* Select tile trigger */
  void selectTile();

  /* Speed changed */
  void speedChanged(int value);

  /* Surface change for matrix */
  void surfaceChange(QString text);

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

#endif // PERSONDIALOG_H
