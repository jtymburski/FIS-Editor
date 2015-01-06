/*******************************************************************************
 * Class Name: EditorHelpers
 * Date Created: January 4, 2015
 * Inheritance: none
 * Description: Contains helper functions for controlling the application. 
 *              Essentially, all public static functions for outside access.
 ******************************************************************************/
#ifndef EDITORHELPERS_H
#define EDITORHELPERS_H

#include <QDir>

class EditorHelpers
{
/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the sprite directory, based on the location of the editor */
  static QString getSpriteDir();

  /* Returns the tile rendering width and height */
  static int getTileSize();
};

#endif // EDITORHELPERS_H
