/*******************************************************************************
 * Class Name: MapItemView
 * Date Created: June 24, 2015
 * Inheritance: QWidget
 * Description: The side toolbar in the map database that gives access to the
 *              map items.
 ******************************************************************************/
#ifndef MAPITEMVIEW_H
#define MAPITEMVIEW_H

#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "Database/EditorMap.h"
#include "Dialog/ItemDialog.h"
#include "EditorHelpers.h"
#include "View/MapThingView.h"

class MapItemView : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  MapItemView(QWidget* parent = NULL);

  /* Destructor function */
  ~MapItemView();

private:
  /* The current editing map */
  EditorMap* editor_map;

  /* Label views for selected item */
  QLabel* lbl_id;
  QLabel* lbl_image;
  QLabel* lbl_name;
  QLabel* lbl_walkover;

  /* Right click menu on instance list */
  QMenu* rightclick_menu;

  /* The item dialog for editing */
  ItemDialog* item_dialog;

  /* The list of items in the view */
  QListWidget* item_instances;
  QListWidget* item_list;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Adds an editor item to the editing map */
  void addItem(EditorMapItem* item);

  /* Creates the layout - only called on initial construction */
  void createLayout();

  /* Opens the item editing dialog */
  void editItem(EditorMapItem* sub_item = NULL);

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
  /* Signals map rendering view to ensure the following item is visible */
  void ensureVisible(QGraphicsItem* item);
  //void ensureVisible(QRect rect);

  /* Triggers the update the data of objects for use in events */
  void fillWithData(EditorEnumDb::MapObjectMode view);

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* The current row changes in the list widget */
  void currentRowChanged(int index);

  /* Delete item instance */
  void deleteInstance();

  /* Edit the base item */
  void editBaseItem(EditorMapThing* base);

  /* Edit the hover instance */
  void editHoverInstance(int index);

  /* Edit item instance */
  void editInstance();

  /* Instance double clicked */
  void instanceDoubleClicked(QListWidgetItem*);

  /* Instance menu trigger */
  void instanceMenu(const QPoint & pos);

  /* Instance of item row changed */
  void instanceRowChanged(int index, bool lock_viewport = false);

  /* An item in the list was double clicked */
  void itemDoubleClicked(QListWidgetItem*);

  /* Update item instances in view */
  void itemInstanceUpdate(QString name_list);

  /* Refreshes the editor item list in the view */
  void updateList();

  /* Updates the item sidebar */
  void updateItems();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Deletes the selected item */
  bool deleteItem();

  /* Duplicates the selected item */
  bool duplicateItem();

  /* Get current item */
  EditorMapItem* getSelected();

  /* Gets the editor map */
  EditorMap* getEditorMap();

  /* Imports a item from another file */
  void importItem();

  /* Creates a new blank item */
  void newItem();

  /* Sets the editor map, which contains the data needed */
  void setEditorMap(EditorMap* map);

  /* Updates list in item dialog, needed for event control */
  void updateListItems(QVector<QString> list);
};
#endif // MAPITEMVIEW_H
