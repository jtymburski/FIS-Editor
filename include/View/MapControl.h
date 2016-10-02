/*******************************************************************************
 * Class Name: MapControl
 * Date Created: December 23, 2014
 * Inheritance: QWidget
 * Description: Database on the right hand side that contains all of the map
 * layers and controls for those layers.
 ******************************************************************************/
#ifndef MAPCONTROL_H
#define MAPCONTROL_H

#include <QHBoxLayout>
#include <QInputDialog>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include "Database/EditorMap.h"

class MapControl : public QWidget
{
  Q_OBJECT

public:
  /* Constructor Function */
  MapControl(QWidget* parent = NULL);

  /* Destructor function */
  ~MapControl();

private:
  /* Bottom delete button */
  QPushButton* button_delete;
  QPushButton* button_duplicate;
  QPushButton* button_import;
  QPushButton* button_new;

  /* The editing map */
  EditorMap* editing_map;

  /* Top event notifier visible button */
  QPushButton* events_toggle;

  /* Top Grid visible Button */
  QPushButton* grid_toggle;

  /* Lists */
  QListWidget* list_bottom;
  QListWidget* list_middle;
  QListWidget* list_top;

  /* The map modifying dialog */
  QDialog* mapsize_dialog;

  /* Top passability visible button */
  QPushButton* passability_toggle;

  /* Right click menu on bottom list */
  QAction* rename_map;
  QMenu* rightclick_menu;

  /* The stored resize index */
  int resize_index;

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Select the sub-map, by index */
  void selectSubMap(int index);

  /* Updates list at the bottom right for maps selected */
  void updateMapList(int selected = -1);

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Signal for layer changing */
  void layerChanged(EditorEnumDb::Layer layer);

  /* Signal for saving the current sub-map location */
  void saveMapLocation();

  /* Signal for updating all lists in map database */
  void updateAllLists();

  /* Signal for setting the sub-map in the rendering space */
  void updateMap();

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* Change layer slot - from map DB */
  void changeLayer(EditorEnumDb::Layer layer);

  /* Creates the new Sub-Map, as instantiated from dialog */
  void createNewSubMap();

  /* Delete Sub-Map */
  void deleteSubMap();

  /* Duplicate Sub-Map */
  void duplicateSubMap();

  /* Import Sub-Map */
  void importSubMap();

  /* Item check change */
  void itemCheckChanged(QListWidgetItem* item);

  /* Selected layer changed */
  void layerChanged(int current_row);

  /* Right click list menu on bottom list */
  void listMenuRequested(const QPoint & pos);

  /* New Sub-Map */
  void newSubMap();

  /* Renames the selected map */
  void renameSubMap();

  /* Resizes the selected map */
  void resizeSubMap();
  void resizeTrigger();

  /* Select the sub-map, by double click */
  void selectSubMap(QListWidgetItem* item);

  /* Toggles the event notifier visibility */
  void toggleEvents(bool visible);

  /* Toggles the grid */
  void toggleGrid(bool visible);

  /* Toggles the passability */
  void togglePassability(bool visible);

  /* Update map row on bottom list */
  void updateBottomRow(int current_row);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Disables control on changing sub for when in editing view with map */
  void disableControl(bool disabled);

  /* Get the current sub-map */
  SubMapInfo* getCurrentMap();
  int getCurrentMapIndex();

  /* Returns event notifier toggle status */
  bool getEventsToggle();

  /* Returns grid toggle status */
  bool getGridToggle();

  /* Returns the map being edited */
  EditorMap* getMapEditor();

  /* Returns passability status */
  bool getPassabilityToggle();

  /* Returns the selected layer in the top list */
  EditorEnumDb::Layer getSelectedLayer();

  /* Returns the top list widget */
  QListWidget* getTopList();

  /* Returns the visibility setting of the checked boxes in the top list */
  bool getVisibility(EditorEnumDb::Layer layer);

  /* Sets the map being edited */
  void setMapEditor(EditorMap* editor);

  /* The widget preferred size */
  QSize sizeHint() const;

};

#endif // MAPCONTROL_H
