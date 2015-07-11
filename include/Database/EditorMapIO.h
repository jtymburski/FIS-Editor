/*******************************************************************************
 * Class Name: EditorMapIO
 * Date Created: July 11, 2015
 * Inheritance: EditorMapThing
 * Description: The class for managing the interfacing with MapInteractiveObject
 *              and filling it with data. The management pop-up is IODialog.
 ******************************************************************************/
#ifndef EDITORMAPIO_H
#define EDITORMAPIO_H

#include "Database/EditorMapThing.h"
#include "Game/Map/MapInteractiveObject.h"

/*
 * Structure which handles the state processing in the list of the interactive
 * object.
 */
struct EditorState
{
  EditorMatrix* matrix;
  EditorEnumDb::MapIOType type;
  MapState::InteractionState state;
  
  Event event_enter;
  Event event_exit;
  Event event_use;
  Event event_walkover;
};

/*
 * Class definition of EditorMapIO
 */
class EditorMapIO : public EditorMapThing
{
public:
  /* Constructor function */
  EditorMapIO(int id = -1, QString name = "Default Name",
              QString description = "");

  /* Copy constructor function */
  EditorMapIO(const EditorMapIO &source);

  /* Destructor function */
  virtual ~EditorMapIO();

private:
  /* Reference interactive object */
  MapInteractiveObject io;

  /* Set of states */
  QVector<EditorState*> states;

  /* -------------------------- Constants ------------------------- */
  //const static uint8_t kTOTAL_SURFACES; /* The max # of surfaces to walk on */

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Delete defines matrixes stored in class - called once at destruction */
  //void deleteMatrixes();

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorMapIO &source);

  /* Saves the io data - virtualized */
  virtual void saveData(FileHandler* fh, bool game_only = false,
                        bool inc_matrix = true);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Append a state to the tail of the list */
  void appendState(EditorEnumDb::MapIOType type);
  void appendState(EditorState* state);
  
  /* Gets the base ref of the io */
  EditorMapIO* getBaseIO() const;

  /* Returns the inactive time before returning down the state path (ms) */
  int getInactiveTime();

  /* Returns the state or states stored within the class */
  EditorState* getState(int index);
  QVector<EditorState*> getStates();

  /* Insert state at index */
  void insertState(int index, EditorEnumDb::MapIOType type);
  void insertState(int index, EditorState* state);

  /* Loads the io data */
  virtual void load(XmlData data, int index);

  /* Saves the io data */
  virtual void save(FileHandler* fh, bool game_only = false);

  /* Sets the base reference io */
  void setBase(EditorMapIO* base_io);

  /* Sets the inactive time before the state returns down the state path (ms) */
  void setInactiveTime(int time);
  
  /* Sets the state at the index (will replace existing) */
  void setState(int index, EditorEnumDb::MapIOType type);
  void setState(int index, EditorState* state);
  
  /* Unset a state or unset all states */
  bool unsetState(int index);
  bool unsetStates();

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  /* The copy operator */
  EditorMapIO& operator= (const EditorMapIO &source);
};

#endif // EDITORMAPIO_H
