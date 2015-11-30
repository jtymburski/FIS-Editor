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
#include "Dialog/EventDialog.h"
//#include "View/EventSetView.h"
#include "View/MatrixView.h"

class IODialog : public QDialog
{
  Q_OBJECT
public:
  /* Constructor function */
  IODialog(EditorMapIO* edit_io = NULL, QWidget* parent = NULL);

  /* Destructor function */
  ~IODialog();

  /* ---- ENUMERATORS ---- */
  enum EditEvent { NOEVENT, ENTER, EXIT, USE, WALKOVER };

private:
  /* The visible control box */
  QComboBox* box_visible;

  /* Check boxes */
  QCheckBox* box_inactive_disable;

  /* Buttons that require external control */
  QPushButton* btn_enter;
  QPushButton* btn_exit;
  QPushButton* btn_minus;
  QPushButton* btn_use;
  QPushButton* btn_walkover;

  /* Combo boxes */
  QComboBox* combo_interaction;
  QComboBox* combo_sound;
  QComboBox* combo_state;

  /* Editing event */
  EditEvent editing_event;

  /* The event set dialog */
  EventDialog* event_dialog;

  /* The frame control dialog */
  FrameDialog* frame_dialog;

  /* Group box for events */
  QGroupBox* group_events;

  /* The working and original IO */
  EditorMapIO* io_original;
  EditorMapIO* io_working;
  
  /* The labels for displaying data */
  QLabel* lbl_frame_img;
  QLabel* lbl_lock_data;

  /* The line edit for io data */
  QLineEdit* line_name;

  /* The list of objects used in possible events */
  QVector<QString> list_items;
  QVector<QString> list_maps;
  QList<QString> list_sounds;
  QVector<QString> list_submaps;
  QVector<QString> list_things;

  /* Lock view and control */
  EditorLock* lock_ctrl;
  LockView* lock_view;

  /* Matrix view and control */
  MatrixView* matrix_view;

  /* Pop-ups within the dialog */
  QDialog* pop_lock;

  /* Spin Boxes */
  QSpinBox* spin_inactive;

  /* State index */
  int state_index;

  /* Description text edit */
  QTextEdit* text_description;

  /* Waiting for sub-map data */
  bool waiting_for_submap;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Creates the layout - only called on initial construction */
  void createLayout(bool instance = false);

  /* Edit event set trigger */
  void editEventSet(EditorEventSet* set, QString window_title = "");

  /* Updates the objects with the io data */
  void updateData();

  /* Update state information */
  void updateState();

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
  void buttonEventEnter();
  void buttonEventExit();
  void buttonEventOk();
  void buttonEventUse();
  void buttonEventWalkover();
  void buttonFrameEdit();
  void buttonLockCancel();
  void buttonLockEdit();
  void buttonLockOk();
  void buttonOk();
  void buttonStateAdd();
  void buttonStateOverview();
  void buttonStateRemove();

  /* Combo changed */
  void changedComboInteract(int index);
  void changedComboState(int index);

  /* Changed triggers in widgets */
  void changedDescription();
  void changedInactive(int i);
  void changedName(QString name);
  void changedSound(const QString &text);

  /* Check inactive time changed */
  void checkInactive(int state);

  /* Select tile trigger */
  void selectTile();

  /* Update the frame for the IO */
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
  QVector<QString> getListItems();
  QVector<QString> getListMaps();
  QList<QString> getListSounds();
  QVector<QString> getListSubmaps();
  QVector<QString> getListThings();

  /* Sets the list of objects, used for dialog and event creation */
  void setListItems(QVector<QString> items);
  void setListMaps(QVector<QString> maps);
  void setListSounds(QList<QString> sounds);
  void setListSubmaps(QVector<QString> sub_maps);
  void setListThings(QVector<QString> things);

  /* Sets the working IO to the original */
  void updateOriginal();

  /* Update the selected tile for the IO */
  void updateSelectedTile(int id, int x, int y);
};

#endif // IODIALOG_H
