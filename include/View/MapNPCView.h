/*******************************************************************************
 * Class Name: MapNPCView
 * Date Created: March 31, 2015
 * Inheritance: QWidget
 * Description: The side toolbar in the map database that gives access to the
 *              map npcs.
 ******************************************************************************/
#ifndef MAPNPCVIEW_H
#define MAPNPCVIEW_H

#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "Database/EditorMap.h"
#include "Dialog/PersonDialog.h"
#include "EditorHelpers.h"
#include "View/MapThingView.h"

class MapNPCView : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  MapNPCView(QWidget* parent = NULL);

  /* Destructor function */
  ~MapNPCView();

private:
  /* The current editing map */
  EditorMap* editor_map;

  /* NPC instance dialog */
  InstanceDialog* instance_dialog;

  /* Label views for selected base npc */
  QLabel* lbl_id;
  QLabel* lbl_image;
  QLabel* lbl_name;
  QLabel* lbl_size;

  /* The person dialog for editing the npcs */
  PersonDialog* npc_dialog;

  /* The list of npc in the view */
  QListWidget* npc_instances;
  QListWidget* npc_list;

  /* Right click menu on instance list */
  QMenu* rightclick_menu;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Adds an editor npc to the editing map */
  void addNPC(EditorMapNPC* npc);

  /* Creates the layout - only called on initial construction */
  void createLayout();

  /* Opens the npc editing dialog */
  void editNPC(EditorMapNPC* sub_npc = NULL);

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

  /* Triggers the update the data of objects for use in events */
  void fillWithData(EditorEnumDb::MapObjectMode view);

  /* Path edit start trigger */
  void pathEditStart(EditorNPCPath*);

  /* Triggers for the select tile on the given view mode */
  void selectTile(EditorEnumDb::MapObjectMode view);

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* The current row changes in the list widget */
  void currentRowChanged(int index);

  /* Delete npc instance */
  void deleteInstance();

  /* Edit the base npc */
  void editBaseNPC(EditorMapThing* base);

  /* Edit the hover instance */
  void editHoverInstance(int index);

  /* Edit npc instance */
  void editInstance();

  /* Instance double clicked */
  void instanceDoubleClicked(QListWidgetItem*);

  /* Instance menu trigger */
  void instanceMenu(const QPoint & pos);

  /* Instance of npc row changed */
  void instanceRowChanged(int index, bool lock_viewport = false);

  /* An item in the list was double clicked */
  void itemDoubleClicked(QListWidgetItem*);

  /* Update npc instances in view */
  void npcInstanceUpdate(QString name_list);

  /* Refreshes the editor npc list in the view */
  void updateList();

  /* Updates the npc sidebar */
  void updateNPCs();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Deletes the selected npc */
  bool deleteNPC();

  /* Duplicates the selected npc */
  bool duplicateNPC();

  /* Get current npc */
  EditorMapNPC* getSelected();

  /* Gets the editor map */
  EditorMap* getEditorMap();

  /* Imports a npc from another file */
  void importNPC();

  /* Creates a new blank npc */
  void newNPC();

  /* Sets the editor map, which contains the data needed */
  void setEditorMap(EditorMap* map);

  /* Updates list in person dialog, needed for event control */
  void updateListItems(QVector<QString> list);
  void updateListMaps(QVector<QString> list);
  void updateListSubmaps(QVector<QString> list);
  void updateListThings(QVector<QString> list);

  /* Update path finished */
  void updatePathFinished();

  /* Update the selected tile for the npc */
  void updateSelectedTile(int id, int x, int y);
};
#endif // MAPNPCVIEW_H
