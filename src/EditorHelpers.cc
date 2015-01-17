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

/* Returns the corresponding layer string */
QString EditorHelpers::getLayerString(EditorEnumDb::Layer layer)
{
  if(layer == EditorEnumDb::BASE)
    return "Base";
  else if(layer == EditorEnumDb::ENHANCER)
    return "Enhancer";
  else if(layer == EditorEnumDb::LOWER1)
    return "Lower 1";
  else if(layer == EditorEnumDb::LOWER2)
    return "Lower 2";
  else if(layer == EditorEnumDb::LOWER3)
    return "Lower 3";
  else if(layer == EditorEnumDb::LOWER4)
    return "Lower 4";
  else if(layer == EditorEnumDb::LOWER5)
    return "Lower 5";
  else if(layer == EditorEnumDb::ITEM)
    return "Item";
  else if(layer == EditorEnumDb::THING)
    return "Thing";
  else if(layer == EditorEnumDb::PERSON)
    return "Person";
  else if(layer == EditorEnumDb::UPPER1)
    return "Upper 1";
  else if(layer == EditorEnumDb::UPPER2)
    return "Upper 2";
  else if(layer == EditorEnumDb::UPPER3)
    return "Upper 3";
  else if(layer == EditorEnumDb::UPPER4)
    return "Upper 4";
  else if(layer == EditorEnumDb::UPPER5)
    return "Upper 5";
  else
    return "Out of Range";
}

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

/*
 * Description: Normalize two points to create an entirely positive rect that
 *              corresponds to the two points using QRectF.
 *
 * Inputs: QPointF point1 - first point
 *         QPointF point2 - second point
 * Output: QRectF - the bounding rectangle
 */
QRectF EditorHelpers::normalizePoints(QPointF point1, QPointF point2)
{
  QRectF rect;

  if(point2.x() >= point1.x() && point2.y() >= point1.y())
  {
    rect.setTopLeft(point1);
    rect.setBottomRight(point2);
  }
  else if(point1.x() >= point2.x() && point1.y() >= point2.y())
  {
    rect.setTopLeft(point2);
    rect.setBottomRight(point1);
  }
  else if(point1.x() > point2.x() && point2.y() > point1.y())
  {
    rect.setTopRight(point1);
    rect.setBottomLeft(point2);
  }
  else if(point2.x() > point1.x() && point1.y() > point2.y())
  {
    rect.setTopRight(point2);
    rect.setBottomLeft(point1);
  }

  return rect;
}
