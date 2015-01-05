/*******************************************************************************
 * Class Name: EditorHelpers
 * Date Created: January 4, 2015
 * Inheritance: none
 * Description: Contains helper functions for controlling the application. 
 *              Essentially, all public static functions for outside access.
 ******************************************************************************/
#include "EditorHelpers.h"

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/

QString EditorHelpers::getSpriteDir()
{
  QString ref_name = "Univursa";
  QString sprites_dir = QDir::current().absolutePath();
  sprites_dir = sprites_dir.left(sprites_dir.indexOf(ref_name) +
                                 ref_name.length()) + "/Project/sprites";
  return sprites_dir;
}
