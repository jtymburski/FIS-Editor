/*******************************************************************************
 * Class Name: EditorMapNPC
 * Date Created: March 30, 2015
 * Inheritance: EditorMapPerson
 * Description: The class for managing the interfacing with MapNPC and 
 *              filling it with data. The base management pop-up is 
 *              PersonDialog and the instance management pop-up is ?
 ******************************************************************************/
#include "Database/EditorMapNPC.h"
//#include <QDebug>

/* Constant Implementation - see header file for descriptions */
//const uint8_t EditorMapPerson::kTOTAL_DIRECTIONS = 4;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Main constructor function. All parameters have blank defaults.
 *
 * Inputs: int id - the id of the npc
 *         QString name - the name of the npc
 *         QString description - the description of the npc
 */
EditorMapNPC::EditorMapNPC(int id, QString name, QString description)
            : EditorMapPerson(id, name, description)
{

}

/*
 * Description: Copy constructor. Calls the blank constructor and then copies
 *              the data from the source.
 *
 * Inputs: const EditorMapNPC &source - the source npc to copy
 */
EditorMapNPC::EditorMapNPC(const EditorMapNPC &source) : EditorMapNPC()
{
  copySelf(source);
}
  
/*
 * Description: Destructor function
 */
EditorMapNPC::~EditorMapNPC()
{

}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: Copies all data from source editor npc to this editor
 *              npc.
 *
 * Inputs: EditorMapNPC &source - the source to copy from
 * Output: none
 */
void EditorMapNPC::copySelf(const EditorMapNPC &source)
{
  /* Base data */
  EditorMapPerson::copySelf(source);

  /* NPC data */
  path = source.path;
}

/*
 * Description: Saves the data for the npc. This does not include the npc
 *              wrapper. Virtualized for other classes as well.
 *
 * Inputs: FileHandler* fh - the file handling data pointer
 *         bool game_only - true if only include game only data
 *         bool inc_matrix - true if include the base thing matrix
 * Output: none
 */
void EditorMapNPC::saveData(FileHandler* fh, bool game_only, bool inc_matrix)
{
  /* First write person data */
  EditorMapPerson::saveData(fh, game_only, inc_matrix);

  /* Next npc data: If not base, write path */
  if(getBaseNPC() != NULL)
  {
    /* Save the path */
    path.save(fh, game_only);
  }
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Returns the base npc. Default to NULL.
 *
 * Inputs: none
 * Output: EditorMapNPC* - the base npc reference pointer
 */
EditorMapNPC* EditorMapNPC::getBaseNPC() const
{
  if(getBaseThing() != NULL)
    return (EditorMapNPC*)getBaseThing();
  return NULL;
}

/*
 * Description: Returns the npc classification - when dealing with casting.
 *
 * Inputs: none
 * Output: ThingBase - the npc classification
 */
ThingBase EditorMapNPC::getClass() const
{
  return ThingBase::NPC;
}

/*
 * Description: Returns a pointer to the stored the path of the npc (used for
 *              movement).
 *
 * Inputs: none
 * Output: EditorNPCPath* - the reference to the internal item for path info
 */
EditorNPCPath* EditorMapNPC::getPath()
{
  return &path;
}

/*
 * Description: Loads the npc data from the XML struct and offset index.
 *
 * Inputs: XmlData data - the XML data tree struct
 *         int index - the offset index into the struct
 * Output: none
 */
void EditorMapNPC::load(XmlData data, int index)
{
  /* Check for path items - if not, send to person */
  if(!path.load(data, index))
  {
    EditorMapPerson::load(data, index);
  }
}

/*
 * Description: Saves the npc data to the file handling pointer.
 *
 * Inputs: FileHandler* fh - the file handling pointer
 *         bool game_only - true if the data should include game only relevant
 * Output: none
 */
void EditorMapNPC::save(FileHandler* fh, bool game_only)
{
  if(fh != NULL)
  {
    fh->writeXmlElement("mapnpc", "id", getID());
    saveData(fh, game_only, false);
    fh->writeXmlElementEnd();
  }
}

/*
 * Description: Sets the base npc object for the person. It will be used for
 *              visual representation and when set, also sets the name and
 *              description (which can be changed later).
 *
 * Inputs: EditorMapNPC* person - the base npc object
 *         bool synchronize - true to set name, description, and base event
 * Output: none
 */
void EditorMapNPC::setBase(EditorMapNPC* npc, bool synchronize)
{
  EditorMapPerson::setBase(static_cast<EditorMapPerson*>(npc), synchronize);
}

/*
 * Description: Sets the X coordinate of the top left of the npc (in tile
 *              units). Updates first path node as well to new location.
 *
 * Inputs: int x - the x coordinate of the top left
 * Output: bool - true if the x was set
 */
bool EditorMapNPC::setX(int x)
{
  if(EditorMapThing::setX(x))
  {
    path.setStartNode(x, path.getStartNode().y);
    return true;
  }
  return false;
}

/*
 * Description: Sets the Y coordinate of the top left of the npc (in tile
 *              units).Updates first path node as well to new location.
 *
 * Inputs: int y - the Y coordinate of the top left
 * Output: bool - true if the y was set
 */
bool EditorMapNPC::setY(int y)
{
  if(EditorMapThing::setY(y))
  {
    path.setStartNode(path.getStartNode().x, y);
    return true;
  }
  return false;
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/*
 * Description: Copy operator construction. This is called when the variable
 *              already exists and equal operator used with another
 *              EditorMapNPC.
 *
 * Inputs: const EditorMapNPC &source - the source class constructor
 * Output: EditorMapNPC& - pointer to the copied class
 */
EditorMapNPC& EditorMapNPC::operator= (const EditorMapNPC &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
