/*******************************************************************************
 * Class Name: MapPersonView
 * Date Created: March 6, 2015
 * Inheritance: QWidget
 * Description: The side toolbar in the map database that gives access to the
 *              map persons.
 ******************************************************************************/
#ifndef MAPPERSONVIEW_H
#define MAPPERSONVIEW_H

#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "Database/EditorMap.h"
#include "Dialog/PersonDialog.h"
#include "EditorHelpers.h"
#include "View/MapThingView.h"

class MapPersonView : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  MapPersonView(QWidget* parent = NULL);

  /* Destructor function */
  ~MapPersonView();

private:
  /* The current editing map */
  EditorMap* editor_map;

  /* Person instance dialog */
  InstanceDialog* instance_dialog;

  /* Label views for selected thing */
  QLabel* lbl_id;
  QLabel* lbl_image;
  QLabel* lbl_name;
  QLabel* lbl_size;

  /* The person dialog for editing */
  PersonDialog* person_dialog;

  /* The list of person in the view */
  QListWidget* person_instances;
  QListWidget* person_list;

  /* Right click menu on instance list */
  QMenu* rightclick_menu;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Adds an editor person to the editing map */
  bool addPerson(EditorMapPerson* person);

  /* Creates the layout - only called on initial construction */
  void createLayout();

  /* Delete pop-up */
  void deletePopBase();
  void deletePopInstance();

  /* Opens the person editing dialog */
  void editPerson(EditorMapPerson* sub_person = NULL);

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

  /* Delete person instance */
  void deleteInstance();

  /* Edit the base person */
  void editBasePerson(EditorMapThing* base);

  /* Edit the hover instance */
  void editHoverInstance(int index);

  /* Edit person instance */
  void editInstance();

  /* Instance double clicked */
  void instanceDoubleClicked(QListWidgetItem*);

  /* Instance menu trigger */
  void instanceMenu(const QPoint & pos);

  /* Instance of person row changed */
  void instanceRowChanged(int index, bool lock_viewport = false);

  /* An item in the list was double clicked */
  void itemDoubleClicked(QListWidgetItem*);

  /* Update person instances in view */
  void personInstanceUpdate(QString name_list);

  /* Refreshes the editor person list in the view */
  void updateList();

  /* Updates the person sidebar */
  void updatePersons();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Deletes the selected person */
  bool deletePerson();

  /* Duplicates the selected person */
  bool duplicatePerson();

  /* Get current person */
  EditorMapPerson* getSelected();

  /* Gets the editor map */
  EditorMap* getEditorMap();

  /* Imports a person from another file */
  void importPerson();

  /* Creates a new blank person */
  void newPerson();

  /* Sets the editor map, which contains the data needed */
  void setEditorMap(EditorMap* map);

  /* Updates list in person dialog, needed for event control */
  void updateListItems(QList<QString> list);
  void updateListMaps(QList<QString> list);
  void updateListMapThings(QList<QString> things,
                           QList<QPair<QString,QString>> ios,
                           QList<QString> items, QList<QString> persons,
                           QList<QString> npcs);
  void updateListParties(QList<QString> list);
  void updateListSounds(QList<QString> list);
  void updateListSubmaps(QList<QString> list);

  /* Update the selected tile for the person */
  void updateSelectedTile(int id, int x, int y);
};
#endif // MAPPERSONVIEW_H
