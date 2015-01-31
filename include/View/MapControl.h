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

  /* The current sub-map being edited */
  SubMapInfo* current_map;

  /* The editing map */
  EditorMap* editing_map;

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

protected:
  /* Select the sub-map, by index */
  void selectSubMap(int index);

  /* Updates list at the bottom right for maps selected */
  void updateMapList(int selected = -1);

public slots:
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

  /* Toggles the grid */
  void toggleGrid(bool visible);

  /* Toggles the passability */
  void togglePassability(bool visible);

  /* Update map row on bottom list */
  void updateBottomRow(int current_row);

signals:
  /* Signal for setting the sub-map in the rendering space */
  void addMap(SubMapInfo*);

public:
  /* Get the current sub-map */
  SubMapInfo* getCurrentMap();
  int getCurrentMapIndex();

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
};

#endif // MAPCONTROL_H
