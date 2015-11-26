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
  MapState::InteractionState interact;
  
  // TODO: REMOVE
  //Event event_enter;
  //Event event_exit;
  //Event event_use;
  //Event event_walkover;

  /* Base utilize */
  bool base_enter;
  bool base_exit;
  bool base_use;
  bool base_walkover;

  /* Event Sets */
  EditorEventSet set_enter;
  EditorEventSet set_exit;
  EditorEventSet set_use;
  EditorEventSet set_walkover;
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
  /* Creates a fresh new blank state */
  //EditorState* createBlankState();

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
  bool appendState(EditorState* state);
  
  /* Consolidates matrix data on all states */
  void consolidate(int ref_index);

  /* Gets the base ref of the io */
  EditorMapIO* getBaseIO() const;

  /* Returns event sets for the different state indexes */
  EditorEventSet* getEventEnter(int index);
  EditorEventSet* getEventExit(int index);
  EditorEventSet* getEventUse(int index);
  EditorEventSet* getEventWalkover(int index);

  /* Returns the inactive time before returning down the state path (ms) */
  int getInactiveTime() const;

  /* Returns the state or states stored within the class */
  EditorState* getState(int index, bool force_instance = false);
  QString getStateName(int index);
  QVector<EditorState*> getStates() const;

  /* Insert state at index */
  bool insertState(int index, EditorEnumDb::MapIOType type);
  bool insertState(int index, EditorState* state);

  /* Returns if the tile sprites in all matrixes at that x, y are null */
  virtual bool isAllNull(int x, int y) const;

  /* Loads the io data */
  virtual void load(XmlData data, int index);

  /* Saves the io data */
  virtual void save(FileHandler* fh, bool game_only = false);

  /* Sets the base reference io */
  void setBase(EditorMapIO* base_io);

  /* Sets the events */
  bool setEventEnter(int index, EditorEventSet set);
  bool setEventExit(int index, EditorEventSet set);
  bool setEventUse(int index, EditorEventSet set);
  bool setEventWalkover(int index, EditorEventSet set);
  //bool setEventEnter(int index, Event event); // TODO: Remove
  //bool setEventExit(int index, Event event); // TODO: Remove
  //bool setEventUse(int index, Event event); // TODO: Remove
  //bool setEventWalkover(int index, Event event); // TODO: Remove

  /* Sets the inactive time before the state returns down the state path (ms) */
  void setInactiveTime(int time);
  
  /* Sets the state at the index (will replace existing) */
  bool setState(int index, EditorState* state, bool data_only = false);

  /* Sets the rendering tile icons */
  void setTileIcons(TileIcons* icons);

  /* Sets the class to use the base event, if relevant */
  bool setUseBaseEnter(int index, bool use_base);
  bool setUseBaseExit(int index, bool use_base);
  bool setUseBaseUse(int index, bool use_base);
  bool setUseBaseWalkover(int index, bool use_base);

  /* Unset a state or unset all states */
  bool unsetState(int index);
  void unsetStates();

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  /* The copy operator */
  EditorMapIO& operator= (const EditorMapIO &source);

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/
public:
  /* Creates a fresh new blank state */
  static EditorState* createBlankState();

  /* Deletes a slate at the given pointer */
  static void deleteState(EditorState* state);
};

#endif // EDITORMAPIO_H
