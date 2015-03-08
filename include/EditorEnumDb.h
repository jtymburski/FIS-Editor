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

  /* Enum for sprite/image border modes */
  enum Border {STANDARD, HOVERED, SELECTED};

  /* Enum for cursor mode */
  enum CursorMode {BASIC,ERASER,BLOCKPLACE,FILL,PASS_ALL,
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

  /* Map view mode */
  enum MapViewMode {RAW_VIEW = 0,
                    SPRITE_VIEW = 1,
                    THING_VIEW = 2
                   };

  /* Sprite edit mode */
  enum SpriteEditMode {SPRITE_FRAMES, SPRITE_DATA, SPRITE_ALL};

  /* Thing cursor for editing the scene */
  enum ThingCursor {THING_ADD, THING_REMOVE, 
                    THING_RENDER_PLUS, THING_RENDER_MINUS,
                    THING_PASS_ALL};

  /* Enum For View Mode */
  enum ViewMode {MAPVIEW = 0,
                 PERSONVIEW = 1,
                 PARTYVIEW = 2,
                 ITEMVIEW = 3,
                 ACTIONVIEW = 4,
                 RACEVIEW = 5,
                 BATTLECLASSVIEW = 6,
                 SKILLSETVIEW = 7,
                 SKILLVIEW = 8,
                 EQUIPMENTVIEW = 9,
                 BUBBYVIEW = 10
                };
};
#endif // EDITORENUMDB_H
