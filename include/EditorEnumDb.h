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
  enum Layer {BASE,ENHANCER,LOWER1,LOWER2,LOWER3,LOWER4,
                    LOWER5,UPPER1,UPPER2,UPPER3,UPPER4,UPPER5};
};
#endif // EDITORENUMDB_H
