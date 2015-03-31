/*******************************************************************************
 * Class Name: EditorMapNPC
 * Date Created: March 30, 2015
 * Inheritance: EditorMapPerson
 * Description: The class for managing the interfacing with MapNPC and 
 *              filling it with data. The base management pop-up is 
 *              PersonDialog and the instance management pop-up is ?
 ******************************************************************************/
#include "Database/EditorMapNPC.h"
#include <QDebug>

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
  // TODO
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
  // TODO
}

/*
 * Description: Sets the base npc object for the person. It will be used for
 *              visual representation and when set, also sets the name and
 *              description (which can be changed later).
 *
 * Inputs: EditorMapNPC* person - the base npc object
 * Output: none
 */
void EditorMapNPC::setBase(EditorMapNPC* npc)
{
  EditorMapThing* thing = NULL;

  if(npc != NULL)
    thing = npc;

  EditorMapThing::setBase(thing);
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
    path.editNode(0, x, path.getNodeY(0),
                  path.getNodeDelay(0), path.getNodeXYFlip(0));
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
    path.editNode(0, path.getNodeX(0), y,
                  path.getNodeDelay(0), path.getNodeXYFlip(0));
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
