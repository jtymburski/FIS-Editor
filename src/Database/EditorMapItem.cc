/*******************************************************************************
 * Class Name: EditorMapItem
 * Date Created: April 25, 2015
 * Inheritance: EditorMapThing
 * Description: The class for managing the interfacing with MapItem and
 *              filling it with data. The management pop-up is PersonDialog.
 ******************************************************************************/
#include "Database/EditorMapItem.h"
//#include <QDebug>

/* Constant Implementation - see header file for descriptions */
//const uint8_t EditorMapItem::kTOTAL_SURFACES   = 1;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Main constructor function. All parameters have blank defaults.
 *
 * Inputs: int id - the id of the item
 *         QString name - the name of the item
 *         QString description - the description of the item
 */
EditorMapItem::EditorMapItem(int id, QString name, QString description)
             : EditorMapThing(id, name, description)
{
  item.setCount(1);
}

/*
 * Description: Copy constructor. Calls the blank constructor and then copies
 *              the data from the source.
 *
 * Inputs: const EditorMapItem &source - the source item to copy
 */
EditorMapItem::EditorMapItem(const EditorMapItem &source)
             : EditorMapItem()
{
  copySelf(source);
}

/*
 * Description: Destructor function
 */
EditorMapItem::~EditorMapItem()
{
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* Delete defines matrixes stored in class - called once at destruction */
//void deleteMatrixes();

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: Copies all data from source editor item to this editor
 *              item.
 *
 * Inputs: EditorMapItem &source - the source to copy from
 * Output: none
 */
void EditorMapItem::copySelf(const EditorMapItem &source)
{
  EditorMapThing::copySelf(source);

  item = source.item;
}

/*
 * Description: Saves the data for the item. This does not include the item
 *              wrapper. Virtualized for other classes as well.
 *
 * Inputs: FileHandler* fh - the file handling data pointer
 *         bool game_only - true if only include game only data
 *         bool inc_matrix - true if include the base thing matrix
 * Output: none
 */
void EditorMapItem::saveData(FileHandler* fh, bool game_only, bool inc_matrix)
{
  EditorMapItem default_item;

  /* First write thing data */
  EditorMapThing::saveData(fh, game_only, inc_matrix);

  /* Next item data: Is base - write core data */
  if(getBaseItem() == nullptr)
  {
    //if(default_item.isWalkover() != isWalkover())
    //  fh->writeXmlData("walkover", isWalkover());
  }
  /* Next item data: Is not base */
  else
  {
    /* Start count */
    fh->writeXmlData("startcount", (int)getCount());

    /* Visible */
    if(default_item.isVisible() != isVisible())
      fh->writeXmlData("visible", isVisible());

    /* Walkover */
    if(default_item.isWalkover() != isWalkover())
      fh->writeXmlData("walkover", isWalkover());

    /* Check sound ID */
    if(getSoundID() >= 0)
      fh->writeXmlData("sound_id", getSoundID());
  }
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Returns the base item. Default to NULL.
 *
 * Inputs: none
 * Output: EditorMapItem* - the base item reference pointer
 */
EditorMapItem* EditorMapItem::getBaseItem() const
{
  if(getBaseThing() != NULL)
    return (EditorMapItem*)getBaseThing();
  return NULL;
}

/*
 * Description: Returns the item classification - when dealing with casting.
 *
 * Inputs: none
 * Output: ThingBase - the item classification
 */
ThingBase EditorMapItem::getClass() const
{
  return ThingBase::ITEM;
}

/*
 * Description: Returns the item count of this item that will be on map.
 *
 * Inputs: none
 * Output: uint32_t - the item count
 */
uint32_t EditorMapItem::getCount()
{
  return item.getStartCount();
}

/*
 * Description: Returns if the item is picked up by walking over the item. If
 *              false, it requires an action key while on the item.
 *
 * Inputs: none
 * Output: bool - true if the item is picked up by walking over
 */
bool EditorMapItem::isWalkover()
{
  return item.isWalkover();
}

/*
 * Description: Loads the item data from the XML struct and offset index.
 *
 * Inputs: XmlData data - the XML data tree struct
 *         int index - the offset index into the struct
 * Output: none
 */
void EditorMapItem::load(XmlData data, int index)
{
  QString element = QString::fromStdString(data.getElement(index));

  /* Parse elements */
  if(element == "startcount")
  {
    setCount(data.getDataInteger());
  }
  else if(element == "walkover")
  {
    setWalkover(data.getDataBool());
  }
  else
  {
    EditorMapThing::load(data, index);
  }
}

/*
 * Description: Saves the item data to the file handling pointer.
 *
 * Inputs: FileHandler* fh - the file handling pointer
 *         bool game_only - true if the data should include game only relevant
 * Output: none
 */
void EditorMapItem::save(FileHandler* fh, bool game_only)
{
  if(fh != NULL)
  {
    fh->writeXmlElement("mapitem", "id", getID());
    saveData(fh, game_only);
    fh->writeXmlElementEnd();
  }
}

/*
 * Description: Sets the base item object for the item. It will be used for
 *              visual representation and when set, also sets the name and
 *              description (which can be changed later).
 *
 * Inputs: EditorMapItem* item - the base item object
 *         bool synchronize - true to set name, description, and base event
 * Output: none
 */
void EditorMapItem::setBase(EditorMapItem* item, bool synchronize)
{
  EditorMapThing::setBase(static_cast<EditorMapThing*>(item), synchronize);
}

/*
 * Description: Sets the number of this particular map item that's on the stack.
 *
 * Inputs: uint32_t count - the new count of the map item stack
 * Output: none
 */
void EditorMapItem::setCount(uint32_t count)
{
  item.setStartCount(count);
}

/*
 * Description: Correlates the core data with this base item. Fails if the ID
 *              does not match.
 *
 * Inputs: ItemData data - the data to sync
 * Output: bool - true if successful
 */
bool EditorMapItem::setData(ItemData data)
{
  if(getBaseItem() == nullptr && data.id == getID())
  {
    getMatrix()->addPath(QString::fromStdString(data.frame_path), 0, 0);
    setDescription(QString::fromStdString(data.description));
    setName(QString::fromStdString(data.name));
    return true;
  }
  return false;
}

/*
 * Description: Sets if the map item is picked up by walking over it. If false
 *              it requires that the action key is pressed prior to picking up.
 *
 * Inputs: bool walkover - true if the item is picked up by walking over
 * Output: none
 */
void EditorMapItem::setWalkover(bool walkover)
{
  item.setWalkover(walkover);
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/*
 * Description: Copy operator construction. This is called when the variable
 *              already exists and equal operator used with another
 *              EditorMapItem.
 *
 * Inputs: const EditorMapItem &source - the source class constructor
 * Output: EditorMapItem& - pointer to the copied class
 */
EditorMapItem& EditorMapItem::operator= (const EditorMapItem &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
