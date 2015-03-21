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
  void addPerson(EditorMapPerson* person);

  /* Creates the layout - only called on initial construction */
  void createLayout();

  /* Opens the person editing dialog */
  void editPerson(EditorMapPerson* sub_person = NULL);

  /* Refreshes the info in the lower half of the widget */
  void updateInfo();

  /* Refreshes the editor person list in the view */
  void updateList();

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Signals map rendering view to ensure the following rect is visible */
  void ensureVisible(QRect rect);

  /* Triggers the update the data of objects for use in events */
  void fillWithData(EditorEnumDb::MapViewMode view);

  /* Triggers for the select tile on the given view mode */
  void selectTile(EditorEnumDb::MapViewMode view);

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* The current row changes in the list widget */
  void currentRowChanged(int index);

  /* Delete person instance */
  void deleteInstance();

  /* Edit person instance */
  void editInstance();

  /* Instance double clicked */
  void instanceDoubleClicked(QListWidgetItem*);

  /* Instance menu trigger */
  void instanceMenu(const QPoint & pos);

  /* Instance of person row changed */
  void instanceRowChanged(int index);

  /* An item in the list was double clicked */
  void itemDoubleClicked(QListWidgetItem*);

  /* Update person instances in view */
  void personInstanceUpdate();

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
  void updateListItems(QVector<QString> list);
  void updateListMaps(QVector<QString> list);
  void updateListSubmaps(QVector<QString> list);
  void updateListThings(QVector<QString> list);

  /* Update the selected tile for the person */
  void updateSelectedTile(int id, int x, int y);
};
#endif // MAPPERSONVIEW_H
