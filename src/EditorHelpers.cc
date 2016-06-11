/*******************************************************************************
 * Class Name: EditorHelpers
 * Date Created: January 4, 2015
 * Inheritance: none
 * Description: Contains helper functions for controlling the application. 
 *              Essentially, all public static functions for outside access.
 ******************************************************************************/
#include "EditorHelpers.h"
#include <QDebug>

/*============================================================================
 * PUBLIC VARIABLES
 *===========================================================================*/

/* Previous path used */
QString EditorHelpers::previous_path = "";

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Optimizes the default qt combo box to handle high amounts of
 *              string data.
 *
 * Inputs: QComboBox* combo - the combo box to optimize
 * Output: none
 */
void EditorHelpers::comboBoxOptimize(QComboBox *combo)
{
  /* Size adjust control to minimum */
  combo->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);

  QListView *view = static_cast<QListView*>(combo->view());

  /* Declares that all items are uniform item sizes (strings) */
  view->setUniformItemSizes(true);

  /* Alerts the view that it is able to display and render on the fly */
  view->setLayoutMode(QListView::Batched);
}

/*
 * Description: Returns the converted xml from pseudo game style '[' and ID
 *              references ({ID}) to name string to the version to render with
 *              CSS.
 *
 * Inputs: QString orig_text - the original text to render
 *         QList<QString> list - the list of thing and ID association
 *                               00000: NAME
 * Output: QString - the resulting text after the conversion
 */
QString EditorHelpers::convertXml(QString orig_text, QList<QString> thing_list,
                                  QList<QString> item_list)
{
  /* Handle color replacements */
  QString clr_text = "";
  QStringList split1 = orig_text.split('[');
  for(int i = 0; i < split1.size(); i++)
  {
    QString entry = split1[i];

    /* Root case */
    if(i == 0 && orig_text.at(0) != '[')
    {
      clr_text += entry;
    }
    else
    {
      /* Check if the second delimiter exists */
      if(entry.contains(']'))
      {
        QStringList split2 = entry.split(']');
        for(int j = 0; j < split2.size(); j++)
        {
          QString entry2 = split2[j];

          /* Walk through the inside states */
          if(j == 0)
          {
            if(entry2.size() <= 2)
              clr_text += "[" + entry2 + "]";
            else if(entry2.at(0) == '/')
              clr_text+= "[/font]";
            else
              clr_text += "[font color=\"#" + entry2 + "\"]";
          }
          else
          {
            clr_text += entry2;
          }
        }
      }
      else
      {
        clr_text += entry;
      }
    }
  }

  /* Handle thing replacements */
  QString render_text = "";
  if(clr_text.contains('{') && clr_text.contains('}'))
  {
    /* Search through splits */
    QStringList split1 = clr_text.split('{');
    for(int i = 0; i < split1.size(); i++)
    {
      QString entry = split1[i];

      /* Root case */
      if(i == 0 && clr_text.at(0) != '{')
      {
        render_text += entry;
      }
      else
      {
        /* Check if the second delimiter exists */
        if(entry.contains('}'))
        {
          QStringList split2 = entry.split('}');
          for(int j = 0; j < split2.size(); j++)
          {
            QString entry2 = split2[j];

            /* Walk through the inside states */
            if(j == 0 && entry2.size() > 0)
            {
              QString name = "";

              /* If item category */
              if(entry2.at(0) == 'I')
              {
                entry2.remove(0, 1);
                int id_ref = entry2.toInt();

                /* Try and find the ID */
                for(int k = 0; name.isEmpty() && k < item_list.size(); k++)
                {
                  QStringList name_list = item_list[k].split(':');
                  int name_id = name_list.first().toInt();
                  if(name_id == id_ref)
                    if(name_list.size() > 0)
                      name = name_list.back();
                }

                /* Generate the text */
                render_text += "[font color=\"#ffc400\"]" + name + "[/font]";
              }
              /* Otherwise, thing category */
              else
              {
                int id_ref = entry2.toInt();

                /* Try and find the ID */
                for(int k = 0; name.isEmpty() && k < thing_list.size(); k++)
                {
                  QStringList name_list = thing_list[k].split(':');
                  int name_id = name_list.first().toInt();
                  if(name_id == id_ref)
                    if(name_list.size() > 0)
                      name = name_list.back();
                }

                /* Generate the text */
                render_text += "[font color=\"#00eaff\"]" + name + "[/font]";
              }
            }
            else
            {
              render_text += entry2;
            }
          }
        }
        else
        {
          render_text += entry;
        }
      }
    }
  }
  else
  {
    render_text = clr_text;
  }

  /* Do final replacement to proper css */
  render_text.replace('[', '<');
  render_text.replace(']', '>');

  return render_text;
}

/*
 * Description: Returns the converted xml from pseudo game style '[' and ID
 *              references ({ID}) to name string to the version to render with
 *              CSS.
 *
 * Inputs: QString orig_text - the original text to render
 *         QVector<QString> thing_list - the list of thing and ID association
 *                                       00000: NAME
 *         QVector<ItemData> item_list - the list of item and ID name struct
 * Output: QString - the resulting text after the conversion
 */
QString EditorHelpers::convertXml(QString orig_text,
                                  QVector<QString> thing_list,
                                  QVector<QString> item_list)
{
  return convertXml(orig_text, thing_list.toList(), item_list.toList());
}

/*
 * Description: Returns the file name extracted from a path. This assumes
 *              QDir::separator() is used as the delimiter
 *
 * Inputs: QString path - the path to process
 * Output: QString - the return file name. Blank if none found
 */
QString EditorHelpers::getFilename(const QString &path)
{
  QString converted_path = QDir::toNativeSeparators(path);
  QStringList sep_set = converted_path.split(QDir::separator());
  if(sep_set.size() > 0)
    return sep_set.back();
  return "";
}

/*
 * Description: Returns the corresponding layer string from the enum.
 *
 * Inputs: EditorEnumDb::Layer layer - the layer to convert
 * Output: QString - the output string result from layer
 */
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
  else if(layer == EditorEnumDb::IO)
    return "Interactive Object";
  else if(layer == EditorEnumDb::PERSON)
    return "Person";
  else if(layer == EditorEnumDb::NPC)
    return "NPC";
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
 *         int base_id - the base object id, if one exists. default invalid
 *         bool higher_cap - raise the max from 999 to 99999 (things)
 * Output: QString - the combined string in the format "000: sssssssss"
 */
QString EditorHelpers::getListString(int id, QString name,
                                     int base_id, bool higher_cap)
{
  QString title = "";

  if(id >= 0)
  {
    /* Check if the base id is relevant and add it to the front */
    if(base_id >= 0)
    {
      /* Append actual id */
      if(higher_cap)
      {
        if(base_id < 10000)
          title += "0";
        if(base_id < 1000)
          title += "0";
      }
      if(base_id < 100)
        title += "0";
      if(base_id < 10)
        title += "0";
      title += QString::number(base_id) + "(";
    }

    /* Append actual id */
    if(higher_cap)
    {
      if(id < 10000)
        title += "0";
      if(id < 1000)
        title += "0";
    }
    if(id < 100)
      title += "0";
    if(id < 10)
      title += "0";
    title += QString::number(id);

    /* Append closing brace, if base id is relevant */
    if(base_id >= 0)
      title += ")";

    /* Add the name */
    title += ": " + name;
  }

  return title;
}

/*
 * Description: Converts a comma delimited string into passability values.
 *
 * Inputs: QString str - the comma delimited string to parse
 *         bool north - north direction passable
 *         bool east - east direction passable
 *         bool south - south direction passable
 *         bool west - west direction passable
 * Output: none
 */
void EditorHelpers::getPassability(QString str, bool &north, bool &east,
                                   bool &south, bool &west)
{
  north = false;
  east = false;
  south = false;
  west = false;

  QStringList set = str.split(",");
  for(int i = 0; i < set.size(); i++)
  {
    if(set[i] == "N")
      north = true;
    else if(set[i] == "E")
      east = true;
    else if(set[i] == "S")
      south = true;
    else if(set[i] == "W")
      west = true;
  }
}

/*
 * Description: Returns the passability number corresponding to the 4
 *              directions. NESW is the order of the binary number.
 *
 * Inputs: bool north - north direction passable
 *         bool east - east direction passable
 *         bool south - south direction passable
 *         bool west - west direction passable
 * Output: int - integer representation of the passability
 */
int EditorHelpers::getPassabilityNum(bool north, bool east,
                                     bool south, bool west)
{
  int num = 0;

  if(north)
    num += 8;
  if(east)
    num += 4;
  if(south)
    num += 2;
  if(west)
    num += 1;

  return num;
}

/*
 * Description: Returns the passability in string format for storing in file.
 *              This puts the directions comma delimited.
 *
 * Inputs: bool north - north direction passable
 *         bool east - east direction passable
 *         bool south - south direction passable
 *         bool west - west direction passable
 * Output: QString - comma delimited string of directions
 */
QString EditorHelpers::getPassabilityStr(bool north, bool east,
                                         bool south, bool west)
{
  QString passability = "";

  /* Passability binary parsing */
  if(north)
    passability += "N,";
  if(east)
    passability += "E,";
  if(south)
    passability += "S,";
  if(west)
    passability += "W,";

  /* Clean-up */
  if(passability.size() > 0)
    passability.chop(1);
  return passability;
}

/*
 * Description: Returns the passability in string format for storing in file.
 *              This puts the directions comma delimited.
 *
 * Inputs: int passability_num - the integer representation of all 4 directions
 * Output: QString - comma delimited string of directions
 */
QString EditorHelpers::getPassabilityStr(int passability_num)
{
  QString passability = "";

  /* Passability number unroll */
  if(passability_num >= 8)
  {
    passability += "N,";
    passability_num -= 8;
  }
  if(passability_num >= 4)
  {
    passability += "E,";
    passability_num -= 4;
  }
  if(passability_num >= 2)
  {
    passability += "S,";
    passability_num -= 2;
  }
  if(passability_num >= 1)
  {
    passability += "W,";
    passability_num -= 1;
  }

  /* Final checks and clean-up */
  if(passability.size() > 0)
    passability.chop(1);
  return passability;
}

/*
 * Description: Returns the previous sprite path, used for file access.
 *
 * Inputs: none
 * Output: QString - the previous sprite path
 */
QString EditorHelpers::getPreviousPath()
{
  if(previous_path.isEmpty())
    previous_path = getSpriteDir();

  //qDebug() << "GET: " << previous_path;
  return previous_path;
}

/*
 * Description: Returns the project directory, based on the location in the
 *              Univursa project folder.
 *
 * Inputs: none
 * Output: QString - string for a full path to the directory
 */
QString EditorHelpers::getProjectDir()
{
  QString ref_name = "Univursa";
  QString project_dir = QDir::current().absolutePath();
  project_dir = project_dir.left(project_dir.indexOf(ref_name) +
                                 ref_name.length()) + "/Project";
  return project_dir;
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
  return (EditorHelpers::getProjectDir() + "/sprites");
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
 * Description: Returns a file split on the standard sprite path, if the file
 *              matches the defined design sprite file. Order is as follows:
 *              EG - C/bler/test/bler_AA_A00.png
 *              1 - C/bler/test/bler
 *              2 - A
 *              3 - A
 *              4 - A00
 *              5 - .png
 * Note: If list is empty, the path didn't meet the designed file structure
 *
 * Inputs: QString file - the file or path name
 *         bool* base - is this sprite a base (e.g. A00, not A01)
 * Output: QList<QString> - the stack of the split, as defined above
 */
QList<QString> EditorHelpers::getValidFileSplit(QString file, bool* base)
{
  QList<QString> stack;
  if(base != NULL)
    *base = false;

  /* Ensure that the file ends with png */
  QString tail = ".png";
  if(file.endsWith(tail))
  {
    file.chop(tail.size());
    QStringList file_split = file.split('_');

    /* Ensure there are at least enough elements */
    if(file_split.size() >= 3)
    {
      QString last = file_split.last();
      QString second_last = file_split.at(file_split.size() - 2);

      /* Check that the last and second last sections meet the standard */
      if(((last.size() == 3 && last.at(0).isUpper() &&
           last.at(1).isDigit() && last.at(2).isDigit()) ||
          (last.size() == 4 && last.at(0).isUpper() && last.at(1) == '|' &&
           last.at(2).isDigit() && last.at(3) == '|')) &&
         second_last.size() == 2 &&
         second_last.at(0).isUpper() && second_last.at(1).isUpper())
      {
        /* Load in the string to the stack */
        stack.push_back(file_split.front());
        for(int i = 1; i < file_split.size() - 2; i++)
          stack.last().push_back("_" + file_split[i]);
        stack.push_back(second_last.at(0));
        stack.push_back(second_last.at(1));
        stack.push_back(last);
        stack.push_back(tail);

        /* Check if both digits are 0 (base frame */
        if(last.at(1).digitValue() == 0 && last.at(2).digitValue() == 0 &&
           base != NULL)
        {
          *base = true;
        }
      }
    }
  }

  return stack;
}

/*
 * Description: Takes a reference ID and a list of strings and returns the index
 *              of the found connected string. If none found, -1 is returned.
 *              The approved strings are in the format of '000: blah blah' and
 *              it extracts the ID from the front.
 *
 * Inputs: int id - the reference ID to find
 *         QList<QString> list - the list to search through
 * Output: int - the found index. -1 if not found
 */
int EditorHelpers::indexOf(int id, QList<QString> list)
{
  int found_index = -1;

  /* Go through the list */
  for(int i = 0; found_index < 0 && i < list.size(); i++)
  {
    QStringList str_list = list[i].split(':');
    if(str_list.size() >= 2)
      if(str_list.front().toInt() == id)
        found_index = i;
  }

  return found_index;
}

/*
 * Description: Takes a reference ID and a list of strings and returns the index
 *              of the found connected string. If none found, -1 is returned.
 *              The approved strings are in the format of '000: blah blah' and
 *              it extracts the ID from the front.
 *
 * Inputs: int id - the reference ID to find
 *         QVector<QString> list - the list to search through
 * Output: int - the found index. -1 if not found
 */
int EditorHelpers::indexOf(int id, QVector<QString> list)
{
  return indexOf(id, QList<QString>::fromVector(list));
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

/*
 * Description: Takes a set of stacked integer x and y coordinate points
 *              organized in vector stack and optimizes. Returns a set of same
 *              vector stack organized string pair that's the x and y index
 *              for saving to a file. Order is as follows for input: first vec
 *              is sorted by category of pairs, second vec is all the integer x,
 *              y pairs of that category. Order is as follows for output: vector
 *              is sorted category of a point of x index and y index for file
 *              saving. Category size matches input.
 *
 * Inputs: QList<QList<QPoint>> orig_set - see description
 * Output: QList<QPair<QString,QString>> - see description
 */
QList<QPair<QString,QString>> EditorHelpers::optimizePoints(
                                              QList<QList<QPoint>> orig_set)
{
  QList<QPair<QString,QString>> return_set;

  /* Loop through origin set for optimization */
  for(int i = 0; i < orig_set.size(); i++)
  {
    QString x_set = "";
    QString y_set = "";

    /* Only proceed if there are pairs */
    if(orig_set[i].size() > 0)
    {
      QList<QPair<QString,QString>> rect_set = rectilinearSplit(orig_set[i]);

      /* Go through all pairs and make single string */
      for(int j = 0; j < rect_set.size(); j++)
      {
        x_set += rect_set[j].first + ",";
        y_set += rect_set[j].second + ",";
      }
      x_set.chop(1);
      y_set.chop(1);
    }

    /* Push to pair */
    return_set.push_back(QPair<QString,QString>(x_set, y_set));
  }

  return return_set;
}

/*
 * Description: Rectilinear split of the points to rectangle sets that can
 *              be stored within the file.
 *
 * Inputs: QList<QPoint<int,int>> - the points (x, y) that are in the tile
 * Output: QList<QPoint<QString,QString>> - the combined rectangle sets
 */
QList<QPair<QString,QString>> EditorHelpers::rectilinearSplit(
                                               QList<QPoint> point_set)
{
  int count = 0;
  QList<int> int_set;
  QList<QPair<QString,QString>> set;
  QList<QString> str_set;
  int x_index = -1;
  int y_index = -1;

  /* Go through all the points */
  for(int i = 0; i < point_set.size(); i++)
  {
    /* If in the valid range, check if the next number is valid */
    if(x_index != -1)
    {
      if(x_index != point_set[i].x() || (y_index + count) != point_set[i].y())
      {
        if(count > 1)
        {
          str_set.push_back(QString::number(y_index) + "-" +
                            QString::number(y_index + count - 1));
        }
        else
        {
          str_set.push_back(QString::number(y_index));
        }

        int_set.push_back(x_index);
        x_index = -1;
      }
      else
      {
        count++;
      }
    }

    /* If the x_index is -1, then reset the values */
    if(x_index == -1)
    {
      x_index = point_set[i].x();
      y_index = point_set[i].y();
      count = 1;
    }
  }

  /* Grab the final number */
  if(count > 1)
  {
    str_set.push_back(QString::number(y_index) + "-" +
                      QString::number(y_index + count - 1));
  }
  else
  {
    str_set.push_back(QString::number(y_index));
  }
  int_set.push_back(x_index);

  /* Try to optimize further with side lines of similar size */
  QString size = "";
  QList<int> indexes;
  while(int_set.size() > 0)
  {
    /* Get the first value */
    count = 1;
    bool done = false;
    indexes.push_back(0);
    x_index = int_set.front();
    size = str_set.front();

    /* Loop through and check for optimizations */
    for(int i = 1; !done && (i < int_set.size()); i++)
    {
      if(int_set[i] == (x_index + count) && str_set[i] == size)
      {
        count++;
        indexes.push_back(i);
      }
      else if(int_set[i] > (x_index + count))
      {
        done = true;
      }
    }

    /* Load up set */
    if(count > 1)
      set.push_back(QPair<QString,QString>(QString::number(x_index) + "-" +
                                   QString::number(x_index + count - 1), size));
    else
      set.push_back(QPair<QString,QString>(QString::number(x_index), size));
    for(int i = indexes.size() - 1; i >= 0; i--)
    {
      int_set.removeAt(indexes[i]);
      str_set.removeAt(indexes[i]);
    }
    indexes.clear();
  }

  return set;
}

/*
 * Description: Sets the previous sprite path to the passed path. If the
 *              bool parameter is true, the passed path has a file portion to
 *              be removed prior.
 *
 * Inputs: QString path - the path to set the new path to
 *         bool includes_file - true if the path includes the file portion
 * Output: none
 */
void EditorHelpers::setPreviousPath(QString path, bool includes_file)
{
  /* Trim path, if it includes file */
  if(includes_file)
  {
    std::string std_path = path.toStdString();
    std_path = std_path.substr(0, std_path.find_last_of("\\/"));
    path = QString::fromStdString(std_path);
  }

  /* Set it */
  previous_path = path;
  //qDebug() << "SET: " << previous_path;
}

/*
 * Description: Takes a path from the load procedure of sprite and splits it
 *              to determine if there are multiple frames.
 *
 * Inputs: QString base_path - the base path to split
 * Output: QList<QString> - the set of paths from the split (possibly 1)
 */
QList<QString> EditorHelpers::splitPath(QString base_path)
{
  QList<QString> paths;

  /* Split first */
  QStringList split_set = base_path.split("|");
  if(split_set.size() == 3 && split_set[1].toInt() > 0)
  {
    for(int i = 0; i < split_set[1].toInt(); i++)
    {
      if(i < 10)
        paths.push_back(split_set[0] + "0" + QString::number(i) + split_set[2]);
      else
        paths.push_back(split_set[0] + QString::number(i) + split_set[2]);
    }
  }
  else
  {
    paths.push_back(base_path);
  }

  return paths;
}

/*
 * Description: Trims a path and removes the project directory portion from
 *              the head of it, if it exists.
 *
 * Inputs: QString path - the path to clean-up
 * Output: QString - the resulting cleaned-up path without project directory
 */
QString EditorHelpers::trimPath(QString path)
{
  QString ref_path = EditorHelpers::getProjectDir();
  path = path.replace('\\', '/');
  int index = path.indexOf(ref_path);

  if(index == 0)
    return path.right(path.size() - ref_path.size() - 1);
  return path;
}
