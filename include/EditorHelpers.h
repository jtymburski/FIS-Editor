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
#include <QPixmap>
#include <QPointF>
#include <QRectF>

#include "EditorEnumDb.h"

/* Struct for icons for tile rendering */
struct TileIcons
{
  QPixmap* passN;
  QPixmap* passE;
  QPixmap* passS;
  QPixmap* passW;

  QPixmap* nopassN;
  QPixmap* nopassE;
  QPixmap* nopassS;
  QPixmap* nopassW;
};

class EditorHelpers
{
/*============================================================================
 * PUBLIC VARIABLES
 *===========================================================================*/

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the corresponding layer string */
  static QString getLayerString(EditorEnumDb::Layer layer);

  /* Returns the list string, taking an id and name */
  static QString getListString(int id, QString name);

  /* Returns the project directory, based on the location of the editor */
  static QString getProjectDir();

  /* Returns the sprite directory, based on the location of the editor */
  static QString getSpriteDir();

  /* Returns the tile rendering width and height */
  static int getTileSize();

  /* Normalize two points to top left and bottom right and return rect */
  static QRectF normalizePoints(QPointF point1, QPointF point2);

  /* Trim path */
  static QString trimPath(QString path);
};

#endif // EDITORHELPERS_H
