/*******************************************************************************
 * Class Name: MapLayView
 * Date Created: March 20, 2016
 * Inheritance: QWidget
 * Description: The side toolbar in the map database that gives access to the
 *              lay overs for each sub map.
 ******************************************************************************/
#ifndef MAPLAYVIEW_H
#define MAPLAYVIEW_H

#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QToolButton>
#include <QWidget>

#include "Database/EditorMap.h"

class MapLayView : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  MapLayView(QWidget* parent = nullptr);

  /* Destructor function */
  ~MapLayView();

  /* Enum for lay types */
  enum LayType {
    UNDERLAYS = 0,
    OVERLAYS = 1
  };

private:
  /* Button widgets */
  QPushButton* btn_del;
  QPushButton* btn_down;
  QToolButton* btn_path;
  QPushButton* btn_up;

  /* Check Box widgets */
  QCheckBox* chk_player;

  /* Combo Box widgets */
  QComboBox* combo_laytype;

  /* The current editing map */
  EditorMap* editor_map;

  /* Label widgets */
  QLabel* lbl_path_list;

  /* List widgets */
  QListWidget* list_lays;

  /* Spin Box widgets */
  QSpinBox* spin_anim;
  QSpinBox* spin_velx;
  QSpinBox* spin_vely;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Creates the layout - only called on initial construction */
  void createLayout();

  /* Returns the selected lay over in the list */
  LayOver* getSelectedLay();

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Triggers the update the data of objects for use in events */
  //void fillWithData(EditorEnumDb::MapObjectMode view);

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* Button control triggers */
  void buttonAdd();
  void buttonDelete();
  void buttonDown();
  void buttonPath();
  void buttonUp();

  /* Changed triggers in widgets */
  void changedLayType(QString);
  void changedListLay(int row);

  /* Check box change triggers */
  void checkPlayerChanged(int state);

  /* Spin box change triggers */
  void spinAnimChanged(int value);
  void spinVelXChanged(int value);
  void spinVelYChanged(int value);

  /* Refreshes the entire data set within the widget */
  void updateData();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Gets the editor map */
  EditorMap* getEditorMap();

  /* Sets the editor map, which contains the data needed */
  void setEditorMap(EditorMap* map);
};
#endif // MAPLAYVIEW_H
