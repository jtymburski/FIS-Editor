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

/*
 * Description: Returns the list string for storing for various map objects.
 *
 * Inputs: int id - the id of the object
 *         QString name - the name of the object
 * Output: QString - the combined string in the format "000: sssssssss"
 */
QString EditorHelpers::getListString(int id, QString name)
{
  QString title = "";

  if(id >= 0)
  {
    /* Append modifiers */
    if(id < 100)
      title += "0";
    if(id < 10)
      title += "0";
    title += QString::number(id);

    /* Add the name */
    title += ": " + name;
  }

  return title;
}

/*
 * Description: Returns the sprite directory, based on the location in the
 *              Univursa project folder.
 *
 * Inputs: none
 * Output: QString - string for a full path to the directory
 */
QString EditorHelpers::getSpriteDir()
{
  QString ref_name = "Univursa";
  QString sprites_dir = QDir::current().absolutePath();
  sprites_dir = sprites_dir.left(sprites_dir.indexOf(ref_name) +
                                 ref_name.length()) + "/Project/sprites";
  return sprites_dir;
}

/*
 * Description: Returns the tile width/height for rendering on the editor.
 *
 * Inputs: none
 * Output: int - tile size
 */
int EditorHelpers::getTileSize()
{
  return 64;
}
