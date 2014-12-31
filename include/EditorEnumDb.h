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

  /* Enum for the tile layers */
  enum Layer {BASE,ENHANCER,LOWER1,LOWER2,LOWER3,LOWER4,
                    LOWER5,UPPER1,UPPER2,UPPER3,UPPER4,UPPER5};
  /* Enum for cursor mode */
  enum CursorMode {BASIC,ERASER,BLOCKPLACE,FILL};

  /* Enum for type of FrameView */
  enum FrameViewType {FRAME,HEAD,TAIL,MIDPOINT,VIEWONLY};

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
