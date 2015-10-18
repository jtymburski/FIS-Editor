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
#include <QString>

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
public:
  /* Previous path used */
  static QString previous_path;

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the corresponding layer string */
  static QString getLayerString(EditorEnumDb::Layer layer);

  /* Returns the list string, taking an id and name */
  static QString getListString(int id, QString name,
                               int base_id = -1, bool higher_cap = false);

  /* Converts the string into passability values */
  static void getPassability(QString str, bool &north, bool &east,
                             bool &south, bool &west);

  /* Returns a base 10 number of the representing passability (as a binary) */
  static int getPassabilityNum(bool north, bool east, bool south, bool west);

  /* Returns the passability string */
  static QString getPassabilityStr(bool north, bool east,
                                   bool south, bool west);
  static QString getPassabilityStr(int passability_num);

  /* Returns the previous path used for file access */
  static QString getPreviousPath();

  /* Returns the project directory, based on the location of the editor */
  static QString getProjectDir();

  /* Returns the sprite directory, based on the location of the editor */
  static QString getSpriteDir();

  /* Returns the tile rendering width and height */
  static int getTileSize();

  /* Returns the valid file split */
  static QList<QString> getValidFileSplit(QString file, bool* base = NULL);

  /* Returns the index of the passed in integer compared to a list of strings
   * with the string namelist format ('000: blah blah') */
  static int indexOf(int id, QList<QString> list);
  static int indexOf(int id, QVector<QString> list);

  /* Normalize two points to top left and bottom right and return rect */
  static QRectF normalizePoints(QPointF point1, QPointF point2);

  /* Optimize points for index saving */
  static QList<QPair<QString,QString>> optimizePoints(
                                             QList<QList<QPoint>> orig_set);

  /* Rectilinear split */
  static QList<QPair<QString, QString>> rectilinearSplit(
                                                       QList<QPoint> point_set);

  /* Sets the previous path used for file access */
  static void setPreviousPath(QString path, bool includes_file = true); // TODO

  /* Trim path */
  static QString trimPath(QString path);
};

#endif // EDITORHELPERS_H
