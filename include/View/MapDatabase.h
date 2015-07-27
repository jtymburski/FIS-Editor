/*******************************************************************************
 * Class Name: MapDatabase
 * Date Created: December 23, 2014
 * Inheritance: QWidget
 * Description: Database on the left hand side that contains all of the sprite
 * creation/selection and editing components.
 ******************************************************************************/
#ifndef MAPDATABASE_H
#define MAPDATABASE_H

#include <QComboBox>
#include <QListWidget>
#include <QStackedWidget>

#include "Dialog/NodeDialog.h"
#include "View/MapIOView.h"
#include "View/MapItemView.h"
#include "View/MapNPCView.h"
#include "View/MapPersonView.h"
#include "View/MapThingView.h"
#include "View/RawImageView.h"
#include "View/SpriteView.h"

class MapDatabase : public QStackedWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  MapDatabase(QWidget* parent = NULL);

  /* Destructor function */
  ~MapDatabase();

private:
  /* Buttons for database control */
  QPushButton* button_delete;
  QPushButton* button_duplicate;
  QPushButton* button_import;
  QPushButton* button_new;

  /* The top selection combo box */
  QComboBox* combo_top;

  /* The editing map for the database */
  EditorMap* editing_map;

  /* Last thing mode to call update */
  EditorEnumDb::MapObjectMode mode_for_data;
  EditorEnumDb::MapObjectMode mode_for_tile;

  /* Working path control variables */
  QAction* path_delete_node;
  QAction* path_edit_node;
  QListWidget* path_list;
  QMenu* path_menu;
  EditorNPCPath* path_working;

  /* The Views */
  MapIOView* view_io;
  MapItemView* view_item;
  MapNPCView* view_npc;
  MapPersonView* view_person;
  RawImageView* view_raw;
  SpriteView* view_sprite;
  MapThingView* view_thing;

  /* Widgets in stacked control */
  EditorEnumDb::MapEditMode widget_mode;
  QWidget* widget_main;
  QWidget* widget_path;
  QWidget* widget_teleport;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Set-up main widget */
  void setupMain();

  /* Set-up path editor widget */
  void setupPathEdit();

  /* Set-up teleport selector */
  void setupTeleportSelect();

  /* Sets the widget mode */
  void setWidgetMode(EditorEnumDb::MapEditMode mode);

protected:
  /* Paints the sprite in a bounding box */
//  void paintEvent(QPaintEvent *);

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Change layer and pen trigger */
  void changeLayer(EditorEnumDb::Layer layer);

  /* Signals map rendering view to ensure the following item is visible */
  void ensureVisible(QGraphicsItem* item);

  /* Path edit start/stop signal */
  void pathEditTrigger(EditorNPCPath* path);

  /* Select tile trigger to map render */
  void selectTile();

  /* Updates event objects in this class */
  void updateEventObjects();

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* Button clicks */
  void buttonDelete();
  void buttonDuplicate();
  void buttonImport();
  void buttonNew();

  /* Fills thing with data */
  void fillWithData(EditorEnumDb::MapObjectMode view);

  /* Path widget control */
  void pathChanged(int current_row);
  void pathClickDouble(QListWidgetItem*);
  void pathClickRight(const QPoint & pos);
  void pathEditStart(EditorNPCPath*);
  void pathFinished();
  void pathNodeDelete();
  void pathNodeEdit();

  /* Select a tile trigger - to the map render */
  void selectTile(EditorEnumDb::MapObjectMode view);

  /* Sends the selected tile to the appropriate thing pop-up */
  void sendSelectedTile(int id, int x, int y);

  /* Update all lists */
  void updateAllLists();

  /* Updated data from higher up in the stack */
  void updatedItems(QVector<QString> items);
  void updatedMaps(QVector<QString> maps);

  /* Updates the path nodes in the list widget */
  void updatePathNodes();

  /* Updates based on selected index */
  void updateSelected(int index);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the views */
  MapIOView* getIOView();
  MapItemView* getItemView();
  MapNPCView* getNPCView();
  MapPersonView* getPersonView();
  RawImageView* getRawView();
  SpriteView* getSpriteView();
  MapThingView* getThingView();

  /* Sets the map editor */
  void setMapEditor(EditorMap* editing_map);
};

#endif // MAPDATABASE_H
