/*******************************************************************************
 * Class Name: MapMusicView
 * Date Created: October 15, 2015
 * Inheritance: QWidget
 * Description: The side toolbar in the map database that gives access to the
 *              map music for each sub map. This also includes provisions for
 *              weather manipulation.
 ******************************************************************************/
#ifndef MAPMUSICVIEW_H
#define MAPMUSICVIEW_H

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "Database/EditorMap.h"
//#include "Dialog/InstanceDialog.h"
//#include "Dialog/ThingDialog.h"
//#include "EditorHelpers.h"

class MapMusicView : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  MapMusicView(QWidget* parent = nullptr);

  /* Destructor function */
  ~MapMusicView();

private:
  /* Button widgets */
  QPushButton* btn_add;
  QPushButton* btn_rem;

  /* Combo widgets */
  QComboBox* combo_weather;

  /* The current editing map */
  EditorMap* editor_map;

  /* Thing instance dialog */
  //InstanceDialog* instance_dialog;

  /* List widgets */
  QListWidget* list_avail;
  QListWidget* list_used;

  /* Music information, for dropdown and selection list */
  QList<QString> music_list;

  /* Right click menu on instance list */
  //QMenu* rightclick_menu;

  /* The thing dialog for editing */
  //ThingDialog* thing_dialog;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Adds an editor thing to the editing map */
  //void addThing(EditorMapThing* thing);

  /* Creates the layout - only called on initial construction */
  void createLayout();

  /* Delete pop-up */
  //void deletePopBase();
  //void deletePopInstance();

  /* Opens the thing editing dialog */
  //void editThing(EditorMapThing* sub_thing = NULL);

  /* Refreshes the entire data set within the widget */
  void updateData();

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Change layer and pen trigger */
  //void changeLayer(EditorEnumDb::Layer layer);

  /* Signals map rendering view to ensure the following item is visible */
  //void ensureVisible(QGraphicsItem* item);

  /* Triggers the update the data of objects for use in events */
  //void fillWithData(EditorEnumDb::MapObjectMode view);

  /* Triggers for the select tile on the given view mode */
  //void selectTile(EditorEnumDb::MapObjectMode view);

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* The current row changes in the list widget */
  //void currentRowChanged(int index);

  /* Delete thing instance */
  //void deleteInstance();

  /* Edit the base thing */
  //void editBaseThing(EditorMapThing* base);

  /* Edit the hover instance */
  //void editHoverInstance(int index);

  /* Edit thing instance */
  //void editInstance();

  /* Instance double clicked */
  //void instanceDoubleClicked(QListWidgetItem*);

  /* Instance menu trigger */
  //void instanceMenu(const QPoint & pos);

  /* Instance of thing row changed */
  //void instanceRowChanged(int index, bool lock_viewport = false);

  /* An item in the list was double clicked */
  //void itemDoubleClicked(QListWidgetItem*);

  /* Update thing instances in view */
  //void thingInstanceUpdate(QString name_list);

  /* Refreshes the editor thing list in the view */
  //void updateList();

  /* Updates the thing sidebar */
  //void updateThings();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Deletes the selected thing */
  //bool deleteThing();

  /* Duplicates the selected thing */
  //bool duplicateThing();

  /* Get current thing */
  //EditorMapThing* getSelected();

  /* Gets the editor map */
  EditorMap* getEditorMap();

  /* Imports a thing from another file */
  //void importThing();

  /* Creates a new blank thing */
  //void newThing();

  /* Sets the editor map, which contains the data needed */
  void setEditorMap(EditorMap* map);

  /* Updates list in thing dialog, needed for event control */
  //void updateListItems(QVector<QString> list);
  //void updateListMaps(QVector<QString> list);
  void updateListMusic(QList<QString> list);
  //void updateListParties(QVector<QString> list);
  //void updateListSounds(QList<QString> list);
  //void updateListSubmaps(QVector<QString> list);
  //void updateListThings(QVector<QString> list);

  /* Update the selected tile for the thing */
  //void updateSelectedTile(int id, int x, int y);

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the instance ID */
  //static int getInstanceID(QString text);
};
#endif // MAPMUSICVIEW_H
