/*******************************************************************************
 * Class Name: EditorTile
 * Date Created: January 5, 2015
 * Inheritance: QGraphicsItem
 * Description: A tile representation in a single map
 ******************************************************************************/
#ifndef EDITORTILE_H
#define EDITORTILE_H

#include <QAction>
#include <QGraphicsItem>
#include <QGraphicsSceneHoverEvent>
#include <QList>
#include <QMenu>
#include <QPainter>
#include <QRect>
#include <QWidget>

#include "Database/EditorSprite.h"
#include "Database/EditorMapIO.h"
#include "Database/EditorMapItem.h"
#include "Database/EditorMapNPC.h"
#include "Database/EditorMapPerson.h"
#include "Database/EditorMapThing.h"
#include "EditorEnumDb.h"
#include "EditorHelpers.h"
#include "Game/Map/Tile.h"

class EditorTile;

/* Struct for handling hover information */
struct HoverInfo
{
  EditorEnumDb::Layer active_layer;
  EditorEnumDb::CursorMode active_cursor;
  bool path_edit_mode;

  EditorMapIO* active_io;
  EditorMapItem* active_item;
  EditorMapNPC* active_npc;
  EditorMapPerson* active_person;
  EditorSprite* active_sprite;
  EditorMapThing* active_thing;

  EditorTile* hover_tile;
  QRect selected_thing;
};

/* Struct for frame option storage */
struct TileRenderInfo
{
  EditorSprite* sprite;
  EditorMapThing* thing;
  bool visible;
};

/* Editor Tile Class */
class EditorTile : public QGraphicsItem
{
public:
  /* Constructor Function */
  EditorTile(int x = 0, int y = 0, TileIcons* icons = NULL);

  /* Copy constructor function */
  EditorTile(const EditorTile &source);

  /* Destructor Function */
  ~EditorTile();

  /* Control functions */
  enum ItemType { Type_TileRender = UserType + 1};
  int type() const { return Type_TileRender; }

private:
  /* Events for the tile */
  Event event_enter;
  Event event_exit;

  /* Is the item hovered */
  HoverInfo* hover_info;
  bool hovered;
  bool hovered_invalid;

  /* Editor Sprite layers */
  TileRenderInfo layer_base;
  TileRenderInfo layer_enhancer;
  QList<TileRenderInfo> layers_lower;
  QList<TileRenderInfo> layers_upper;

  /* Things on the tile */
  QList<TileRenderInfo> ios;
  QList<TileRenderInfo> items;
  QList<TileRenderInfo> npcs;
  QList<TileRenderInfo> persons;
  QList<TileRenderInfo> things;

  /* The Tile that will be placed into Univursa.exe */
  Tile tile;

  /* The rendering tile icons */
  TileIcons* tile_icons;

  /* Visibility painting control */
  bool visible_events;
  bool visible_grid;
  bool visible_passability;

  /* Positions and depths for the tiles on the map grid */
  int x_pos, y_pos;

public:
  /*------------------- Constants -----------------------*/
  const static uint8_t kLOWER_COUNT_MAX; /* The max number of lower layers */
  const static uint8_t kMAX_ITEMS; /* The max number of items stored */
  const static uint8_t kUPPER_COUNT_MAX; /* The max number of upper layers */

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorTile &source);

  /* Determine if hovering sprite or thing in tile */
  bool isHoverIO();
  bool isHoverItem();
  bool isHoverNPC();
  bool isHoverPerson();
  bool isHoverSprite();
  bool isHoverThing();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Adds item to tile stack */
  bool addItem(EditorMapItem* item);

  /* Necessary function for returning the bounding rectangle */
  QRectF boundingRect() const;

  /* Returns the active layers in a string */
  QString getActiveLayers();

  /* Gets the events */
  Event getEventEnter() const;
  Event getEventExit() const;

  /* Gets the tile for editing */
  Tile* getGameTile();

  /* Gets the hover information */
  HoverInfo* getHoverInfo();

  /* Returns the passability based on layer and direction */
  bool getPassability(EditorEnumDb::Layer layer, Direction direction);

  /* Returns the passability of the IO */
  bool getPassabilityIO(Direction direction);

  /* Returns a number between 0 and 15 for what the passability is */
  int getPassabilityNum(EditorEnumDb::Layer layer);

  /* Returns the passability of the npc */
  bool getPassabilityNPC(Direction direction);

  /* Returns the passability of the person */
  bool getPassabilityPerson(Direction direction);

  /* Returns the passability of the thing */
  bool getPassabilityThing(Direction direction);

  /* Returns the passability based on direction and what layers are visible */
  bool getPassabilityVisible(Direction direction);

  /* Returns the map io pointer at the given render depth and all ios */
  EditorMapIO* getIO(int render_level);
  QVector<EditorMapIO*> getIOs();

  /* Returns the map item pointer(s) */
  EditorMapItem* getItemBaseID(int id);
  EditorMapItem* getItemID(int id);
  EditorMapItem* getItem(int index);
  QVector<EditorMapItem*> getItems();

  /* Returns the map npc pointer at the given render depth */
  EditorMapNPC* getNPC(int render_level);
  QVector<EditorMapNPC*> getNPCs();

  /* Returns the map person pointer at the given render depth */
  EditorMapPerson* getPerson(int render_level);
  QVector<EditorMapPerson*> getPersons();

  /* Returns the sprite based on layer and direction */
  EditorSprite* getSprite(EditorEnumDb::Layer layer);

  /* Returns the map thing pointer(s) for the generic thing */
  EditorMapThing* getThing(int render_level);
  QVector<EditorMapThing*> getThings();

  /* Returns layer visibility */
  bool getVisibility(EditorEnumDb::Layer layer);
  bool getVisibilityIO(int render_level);
  bool getVisibilityItems();
  bool getVisibilityNPC(int render_level);
  bool getVisibilityPerson(int render_level);
  bool getVisibilityThing(int render_level);

  /* Returns grid/pass/events visibility */
  bool getVisibilityEvents();
  bool getVisibilityGrid();
  bool getVisibilityPass();

  /* Returns X and Y, in tile set */
  int getX();
  int getY();

  /* Is tile events set */
  bool isEventEnterSet() const;
  bool isEventExitSet() const;

  /* Painting function for Tile Wrapper */
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
             QWidget* widget = NULL);

  /* Function to place a current sprite on the maps active layer */
  bool place();
  bool place(EditorEnumDb::Layer layer, EditorSprite* sprite,
             bool load = false);

  /* Sets the tile events */
  void setEventEnter(Event event, bool just_update = false);
  void setEventExit(Event event, bool just_update = false);

  /* Sets the hover state */
  void setHover(bool hover, bool hover_invalid = false);

  /* Sets the hover information - from editor map */
  void setHoverInfo(HoverInfo* new_info);
  void setHoverInfo(bool hover, bool hover_invalid = false,
                    HoverInfo* new_info = NULL);

  /* Sets the io sprite pointer, stored within the class */
  bool setIO(EditorMapIO* io);

  /* Sets the passability based on layer and direction */
  void setPassability(EditorEnumDb::Layer layer, bool passable);
  void setPassability(EditorEnumDb::Layer layer, Direction direction,
                      bool passable);

  /* Sets the npc sprite pointer, stored within the class */
  bool setNPC(EditorMapNPC* npc);

  /* Sets the person sprite pointer, stored within the class */
  bool setPerson(EditorMapPerson* person);

  /* Sets the thing sprite pointer, stored within the class */
  bool setThing(EditorMapThing* thing);

  /* Sets the rendering tile icons */
  void setTileIcons(TileIcons* icons);

  /* Sets the various layer visibilities */
  void setVisibility(EditorEnumDb::Layer layer, bool visible);
  void setVisibilityBase(bool);
  void setVisibilityEnhancer(bool);
  void setVisibilityIO(bool visible);
  bool setVisibilityIO(int render_level, bool visible);
  void setVisibilityItems(bool visible);
  bool setVisibilityLower(int, bool);
  void setVisibilityNPC(bool visible);
  bool setVisibilityNPC(int render_level, bool visible);
  void setVisibilityPerson(bool visible);
  bool setVisibilityPerson(int render_level, bool visible);
  void setVisibilityThing(bool visible);
  bool setVisibilityThing(int render_level, bool visible);
  bool setVisibilityUpper(int, bool);

  /* Sets the grid/pass/events visibility */
  void setVisibilityEvents(bool);
  void setVisibilityGrid(bool);
  void setVisibilityPass(bool);

  /* Function for removing a sprite from the maps active layer */
  void unplace(EditorEnumDb::Layer layer);
  void unplace(EditorSprite* sprite);

  /* Unsets the tile events */
  void unsetEventEnter();
  void unsetEventExit();

  /* Unsets the stored io pointer(s) */
  bool unsetIO(EditorMapIO* io);
  bool unsetIO(int render_level);
  void unsetIOs();

  /* Unsets the stored item pointer(s) */
  bool unsetItem(EditorMapItem* item);
  bool unsetItem(int index);
  void unsetItems();

  /* Unsets the stored npc pointer(s) */
  bool unsetNPC(EditorMapNPC* npc);
  bool unsetNPC(int render_level);
  void unsetNPCs();

  /* Unsets the stored person pointer(s) */
  bool unsetPerson(EditorMapPerson* person);
  bool unsetPerson(int render_level);
  void unsetPersons();

  /* Unsets the stored thing pointer(s) */
  bool unsetThing(EditorMapThing* thing);
  bool unsetThing(int render_level);
  void unsetThings();

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  /* The copy operator */
  EditorTile& operator= (const EditorTile &source);
};

#endif // EDITORTILE_H
