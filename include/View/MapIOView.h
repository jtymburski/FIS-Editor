/*******************************************************************************
 * Class Name: MapIOView
 * Date Created: July 16, 2015
 * Inheritance: QWidget
 * Description: The side toolbar in the map database that gives access to the
 *              map interactive objects.
 ******************************************************************************/
#ifndef MAPIOVIEW_H
#define MAPIOVIEW_H

#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "Database/EditorMap.h"
#include "Dialog/IODialog.h"
#include "EditorHelpers.h"
#include "View/MapThingView.h"

class MapIOView : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  MapIOView(QWidget* parent = NULL);

  /* Destructor function */
  ~MapIOView();

private:
  /* The current editing map */
  EditorMap* editor_map;

  /* Thing instance dialog */
  InstanceDialog* instance_dialog;

  /* The IO dialog for editing */
  IODialog* io_dialog;
  
  /* The list of IOs in the view */
  QListWidget* io_instances;
  QListWidget* io_list;
  
  /* Label views for selected thing */
  QLabel* lbl_id;
  QLabel* lbl_image;
  QLabel* lbl_name;
  QLabel* lbl_size;

  /* Right click menu on instance list */
  QMenu* rightclick_menu;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Adds an editor IO to the editing map */
  bool addIO(EditorMapIO* io);

  /* Creates the layout - only called on initial construction */
  void createLayout();

  /* Delete pop-up */
  void deletePopBase();
  void deletePopInstance();

  /* Opens the IO editing dialog */
  void editIO(EditorMapIO* sub_io = NULL);

  /* Refreshes the info in the lower half of the widget */
  void updateInfo();

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Change layer and pen trigger */
  void changeLayer(EditorEnumDb::Layer layer);

  /* Signals map rendering view to ensure the following item is visible */
  void ensureVisible(QGraphicsItem* item);

  /* Triggers the update the data of objects for use in events */
  void fillWithData(EditorEnumDb::MapObjectMode view);

  /* Triggers for the select tile on the given view mode */
  void selectTile(EditorEnumDb::MapObjectMode view);

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* The current row changes in the list widget */
  void currentRowChanged(int index);

  /* Delete IO instance */
  void deleteInstance();

  /* Edit the base IO */
  void editBaseIO(EditorMapThing* base);

  /* Edit the hover instance */
  void editHoverInstance(int index);

  /* Edit IO instance */
  void editInstance();

  /* Instance double clicked */
  void instanceDoubleClicked(QListWidgetItem*);

  /* Instance menu trigger */
  void instanceMenu(const QPoint & pos);

  /* Instance of IO row changed */
  void instanceRowChanged(int index, bool lock_viewport = false);

  /* An item in the list was double clicked */
  void itemDoubleClicked(QListWidgetItem*);

  /* Update IO instances in view */
  void ioInstanceUpdate(QString name_list);

  /* Refreshes the editor IO list in the view */
  void updateList();

  /* Updates the IO sidebar */
  void updateIOs();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Deletes the selected IO */
  bool deleteIO();

  /* Duplicates the selected IO */
  bool duplicateIO();

  /* Get current IO */
  EditorMapIO* getSelected();

  /* Gets the editor map */
  EditorMap* getEditorMap();

  /* Imports a IO from another file */
  void importIO();

  /* Creates a new blank IO */
  void newIO();

  /* Sets the editor map, which contains the data needed */
  void setEditorMap(EditorMap* map);

  /* Updates list in IO dialog, needed for event control */
  void updateListItems(QList<QString> list);
  void updateListMaps(QList<QString> list);
  void updateListMapThings(QList<QString> things,
                           QList<QPair<QString,QString>> ios,
                           QList<QString> items, QList<QString> persons,
                           QList<QString> npcs);
  void updateListParties(QList<QString> list);
  void updateListSounds(QList<QString> list);
  void updateListSubmaps(QList<QString> list);

  /* Update the selected tile for the IO */
  void updateSelectedTile(int id, int x, int y);
};
#endif // MAPIOVIEW_H
