/*******************************************************************************
 * Class Name: ThingView
 * Date Created: February 13, 2015
 * Inheritance: QWidget
 * Description: The side toolbar in the map database that gives access to the
 *              map things.
 ******************************************************************************/
#ifndef THINGVIEW_H
#define THINGVIEW_H

#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "Database/EditorMap.h"
#include "Dialog/ThingDialog.h"
#include "EditorHelpers.h"
//#include "View/SpriteViewList.h"

class ThingView : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  ThingView(QWidget* parent = NULL);

  /* Destructor function */
  ~ThingView();

private:
  /* The current editing map */
  EditorMap* editor_map;

  /* Label views for selected thing */
  QLabel* lbl_id;
  QLabel* lbl_image;
  QLabel* lbl_name;
  QLabel* lbl_size;

  /* The thing dialog for editing */
  ThingDialog* thing_dialog;

  /* The list of things in the view */
  QListWidget* thing_list;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Adds an editor thing to the editing map */
  void addThing(EditorThing* thing);

  /* Creates the layout - only called on initial construction */
  void createLayout();

  /* Opens the thing editing dialog */
  void editThing();

  /* Refreshes the info in the lower half of the widget */
  void updateInfo();

  /* Refreshes the editor thing list in the view */
  void updateList();

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Triggers the update the data of objects for use in events */
  void fillWithData(EditorEnumDb::MapViewMode view);

  /* Triggers for the select tile on the given view mode */
  void selectTile(EditorEnumDb::MapViewMode view);

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* The current row changes in the list widget */
  void currentRowChanged(int);

  /* An item in the list was double clicked */
  void itemDoubleClicked(QListWidgetItem*);

  /* Updates the thing sidebar */
  void updateThings();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Deletes the selected thing */
  bool deleteThing();

  /* Duplicates the selected thing */
  bool duplicateThing();

  /* Get current thing */
  EditorThing* getSelected();

  /* Gets the editor map */
  EditorMap* getEditorMap();

  /* Imports a thing from another file */
  void importThing();

  /* Creates a new blank thing */
  void newThing();

  /* Sets the editor map, which contains the data needed */
  void setEditorMap(EditorMap* map);

  /* Updates list in thing dialog, needed for event control */
  void updateListItems(QVector<QString> list);
  void updateListMaps(QVector<QString> list);
  void updateListSubmaps(QVector<QString> list);
  void updateListThings(QVector<QString> list);

  /* Update the selected tile for the thing */
  void updateSelectedTile(int id, int x, int y);
};
#endif // THINGVIEW_H
