/******************************************************************************
* Class Name: EnumDb
* Date Created: November 24, 2013
* Inheritance:
* Description:  Enum Database holds non-flag Enums
 *****************************************************************************/
#ifndef EDITORENUMDB_H
#define EDITORENUMDB_H

#include "Sprite.h"
#undef main

class EditorEnumDb
{
public:
  /* ---- Constants ---- */

  const static int kBASE_ID_IOS = 50000; /* The instant base ID for MapIOs */
  const static int kBASE_ID_ITEMS = 60000; /* The instant base ID for items */
  const static int kBASE_ID_PERSON = 0; /* The instant base ID for persons */
  const static int kBASE_ID_NPC = 10000; /* The instant base ID for npcs */
  const static int kBASE_ID_THING = 20000; /* The base thing ID */
  const static int kEVENT_SET_VIEW_H = 186; /* Height of event set view */
  const static int kEVENT_VIEW_H = 360;//236; /* Height of event view widget */
  const static int kEVENT_VIEW_W = 352; /* Width of event view widget */
  const static int kMAX_COUNT_BASES = 10000; /* Max count of base things */
  const static int kMAX_COUNT_IOS = 10000; /* Max count of instance IOs */
  const static int kMAX_COUNT_ITEMS = 5000; /* Max count of instance items */
  const static int kMAX_COUNT_NPCS = 10000; /* Max count of instance npcs */
  const static int kMAX_COUNT_PERSONS = 10000; /* Max count of inst persons */
  const static int kMAX_COUNT_THINGS = 30000; /* Max count of inst things */
  const static int kMAX_PERSON_LVL = 127; /* The max level of a person */

  /* ---- Enumerations ---- */

  /* Enum for sprite/image border modes */
  enum Border {STANDARD, HOVERED, SELECTED};

  /* Enum for cursor mode */
  enum CursorMode {BASIC,ERASER,MOVE,BLOCKPLACE,FILL,PASS_ALL,
                   PASS_N,PASS_E,PASS_S,PASS_W,NO_CURSOR};

  /* Enum for type of FrameView */
  enum FrameViewType {FRAME,HEAD,TAIL,MIDPOINT,VIEWONLY};

  /* Enum for the tile layers */
  enum Layer {BASE = 0,
              ENHANCER = 1,
              LOWER1 = 2,
              LOWER2 = 3,
              LOWER3 = 4,
              LOWER4 = 5,
              LOWER5 = 6,
              ITEM = 7,
              THING = 8,
              IO = 9,
              PERSON = 10,
              NPC = 11,
              UPPER1 = 12,
              UPPER2 = 13,
              UPPER3 = 14,
              UPPER4 = 15,
              UPPER5 = 16,
              NO_LAYER = 17
             };

  /* Map edit mode */
  enum MapEditMode {NORMAL_EDIT = 0,
                    PATH_EDIT = 1,
                    TELEPORT_SELECT = 2,
                   };

  /* Map interactive object type */
  enum MapIOType {IO_STATE, IO_TRANSITION};

  /* Map object mode - map database */
  enum MapObjectMode {RAW_VIEW = 0,
                      SPRITE_VIEW = 1,
                      THING_VIEW = 2,
                      IO_VIEW = 3,
                      ITEM_VIEW = 4,
                      PERSON_VIEW = 5,
                      NPC_VIEW = 6,
                      MUSIC_VIEW = 7,
                      MAPBATTLESCENE_VIEW = 8,
                      MAPLAY_VIEW = 9
                     };

  /* Sprite edit mode */
  enum SpriteEditMode {SPRITE_FRAMES, SPRITE_DATA, SPRITE_ALL};

  /* Thing cursor for editing the scene */
  enum ThingCursor {THING_ADD, THING_REMOVE, 
                    THING_RENDER_PLUS, THING_RENDER_MINUS,
                    THING_PASS_ALL};

  /* Enum For View Mode - game database */
  enum ViewMode {MAPVIEW = 0,
                 BLANKVIEW1 = 1,
                 PARTYVIEW = 2,
                 PERSONVIEW = 3,
                 BUBBYVIEW = 4,
                 EQUIPMENTVIEW = 5,
                 ITEMVIEW = 6,
                 BATTLECLASSVIEW = 7,
                 RACEVIEW = 8,
                 SKILLSETVIEW = 9,
                 SKILLVIEW = 10,
                 ACTIONVIEW = 11,
                 BLANKVIEW2 = 12,
                 AUDIOVIEW = 13,
                 BATTLESCENEVIEW = 14
                };
};
#endif // EDITORENUMDB_H
