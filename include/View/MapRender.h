   /*******************************************************************************
 * Class Name: MapRender
 * Date Created: November 11, 2013
 * Inheritance: QGLWidget
 * Description: The map editor widget that gives a snapshot view of the window
 *              to make changes to the map from.
 ******************************************************************************/
#ifndef MAPRENDER_H
#define MAPRENDER_H

#include <QDebug>
#include <QGraphicsView>
#include <QTabWidget>
#include <QGraphicsScene>
#include <QList>
#include <QMenu>
#include <QRect>
#include <QScrollBar>
#include <QWidget>

#include "Database/EditorMap.h"
#include "EnumDb.h"

class MapRender : public QGraphicsScene
{
  Q_OBJECT

public:
  /* Constructor function */
  MapRender(QWidget* parent = NULL);

  /* Destructor function */
  ~MapRender();

private:
  /* Active Hover tile */
  //EditorTile* active_tile;

  /* Block data */
  QPointF block_origin;
  bool block_erase;

  /* Cursor type */
  //EditorEnumDb::CursorMode cursor_mode;

  /* The editing map */
  EditorMap* editing_map;

  /* The middle click menu */
  QMenu* middleclick_menu;

  /* The editing path */
  EditorNPCPath* path_edit;

  /* In tile select mode */
  bool tile_select;

  /*------------------- Constants -----------------------*/
  //const static int kELEMENT_DATA;     /* Element data type for sprite */

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Menu adding for tile click */
  bool menuIOs(EditorTile* t, QMenu* menu);
  bool menuItems(EditorTile* t, QMenu* menu);
  bool menuNPCs(EditorTile* t, QMenu* menu);
  bool menuPersons(EditorTile* t, QMenu* menu);
  bool menuSprites(EditorTile* t, QMenu* menu);
  bool menuThings(EditorTile* t, QMenu* menu);

  /* Mouse event */
  bool mouseEvent(QGraphicsSceneMouseEvent* event);

  /* Path click manipulation - for editing paths */
  void pathClickLeft(int x, int y);
  void pathClickRight(int x, int y);

  /* Right click menu initialize */
  void tileClickInit();

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* General event processing */
  bool event(QEvent *event);

  /* Mouse move events */
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Sends the tile position to the map status bar */
  void sendCurrentPosition(int,int);

  /* Sends the selected tile to the thing pop-up */
  void sendSelectedTile(int id, int x, int y);

  /* Tile signals */
  void tileIO(int depth);
  void tileItem(int index);
  void tileNPC(int depth);
  void tilePerson(int depth);
  void tileSprite(EditorEnumDb::Layer layer);
  void tileThing(int depth);

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* NPC Path Add/Remove control */
  void npcPathAdd(EditorNPCPath* path);
  void npcPathRemove(EditorNPCPath* path);

  /* Path edit trigger */
  void pathEditTrigger(EditorNPCPath* path);

  /* Select a tile trigger */
  void selectTile();

  /* Tile slots */
  void tileIO0();
  void tileIO1();
  void tileIO2();
  void tileIO3();
  void tileIO4();
  void tileIO5();
  void tileIO6();
  void tileIO7();
  void tileIO8();
  void tileIO9();
  void tileItem0();
  void tileItem1();
  void tileItem2();
  void tileItem3();
  void tileItem4();
  void tileItem5();
  void tileItem6();
  void tileItem7();
  void tileItem8();
  void tileItem9();
  void tileNPC0();
  void tileNPC1();
  void tileNPC2();
  void tileNPC3();
  void tileNPC4();
  void tileNPC5();
  void tileNPC6();
  void tileNPC7();
  void tileNPC8();
  void tileNPC9();
  void tilePerson0();
  void tilePerson1();
  void tilePerson2();
  void tilePerson3();
  void tilePerson4();
  void tilePerson5();
  void tilePerson6();
  void tilePerson7();
  void tilePerson8();
  void tilePerson9();
  void tileSpriteB();
  void tileSpriteE();
  void tileSpriteL1();
  void tileSpriteL2();
  void tileSpriteL3();
  void tileSpriteL4();
  void tileSpriteL5();
  void tileSpriteU1();
  void tileSpriteU2();
  void tileSpriteU3();
  void tileSpriteU4();
  void tileSpriteU5();
  void tileThing0();
  void tileThing1();
  void tileThing2();
  void tileThing3();
  void tileThing4();
  void tileThing5();
  void tileThing6();
  void tileThing7();
  void tileThing8();
  void tileThing9();

  /* Update the rendering sub-map */
  void updateRenderingMap();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the map being edited */
  EditorMap* getMapEditor();

  /* Returns the width and height */
  int getMapHeight();
  int getMapWidth();

  /* Sets the map being edited */
  void setMapEditor(EditorMap* editor);

  /* Update the entire scene */
  void updateAll();
};

#endif // MAPRENDER_H
