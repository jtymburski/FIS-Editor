/*******************************************************************************
 * Class Name: EditorMapNPC
 * Date Created: March 30, 2015
 * Inheritance: EditorMapPerson
 * Description: The class for managing the interfacing with MapNPC and
 *              filling it with data. The base management pop-up is
 *              PersonDialog and the instance management pop-up is ?
 ******************************************************************************/
#ifndef EDITORMAPNPC_H
#define EDITORMAPNPC_H

#include "Database/EditorMapPerson.h"
#include "Database/EditorNPCPath.h"
#include "Game/Map/MapNPC.h"

class EditorMapNPC : public EditorMapPerson
{
public:
  /* Constructor function */
  EditorMapNPC(int id = -1, QString name = "Default Name",
               QString description = "");

  /* Copy constructor function */
  EditorMapNPC(const EditorMapNPC &source);

  /* Destructor function */
  virtual ~EditorMapNPC();

private:
  /* The path of the npc */
  EditorNPCPath path;

  /* -------------------------- Constants ------------------------- */
  //const static uint8_t kTOTAL_DIRECTIONS; /* The max # of directions to... */

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorMapNPC &source);

  /* Saves the npc data - virtualized */
  virtual void saveData(FileHandler* fh, bool game_only = false,
                        bool inc_matrix = true);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Gets the base npc of the person */
  EditorMapNPC* getBaseNPC() const;

  /* Gets the npc classification */
  virtual ThingBase getClass() const;

  /* Gets the npc path container */
  EditorNPCPath* getPath();

  /* Loads the npc data */
  virtual void load(XmlData data, int index);

  /* Saves the npc data */
  virtual void save(FileHandler* fh, bool game_only = false);

  /* Sets the base reference npc */
  void setBase(EditorMapNPC* npc, bool synchronize = true);

  /* Sets the x and y coordinate of the npc - primarily handled by thing */
  bool setX(int x);
  bool setY(int y);

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  /* The copy operator */
  EditorMapNPC& operator= (const EditorMapNPC &source);
};

#endif // EDITORMAPNPC_H
