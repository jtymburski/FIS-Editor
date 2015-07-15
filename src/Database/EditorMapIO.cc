/*******************************************************************************
 * Class Name: EditorMapIO
 * Date Created: July 11, 2015
 * Inheritance: EditorMapThing
 * Description: The class for managing the interfacing with MapInteractiveObject
 *              and filling it with data. The management pop-up is IODialog.
 ******************************************************************************/
#include "Database/EditorEvent.h"
#include "Database/EditorMapIO.h"
#include <QDebug>

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
EditorMapIO::EditorMapIO(int id, QString name, QString description)
           : EditorMapThing(id, name, description)
{
  /* Create root state */
  EditorState* state = createBlankState();
  state->matrix->increaseWidth();
  state->type = EditorEnumDb::IO_STATE;
  states.push_back(state);

  /* Set the initial frames in the thing */
  setMatrix(state->matrix);
}

/*
 * Description: Copy constructor. Calls the blank constructor and then copies
 *              the data from the source.
 *
 * Inputs: const EditorMapIO &source - the source interactive object to copy
 */
EditorMapIO::EditorMapIO(const EditorMapIO &source)
           : EditorMapIO()
{
  copySelf(source);
}

/*
 * Description: Destructor function
 */
EditorMapIO::~EditorMapIO()
{
  unsetMatrix();
  unsetStates();

  /* Delete last state */
  deleteState(states.front());
  states.clear();
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* Creates a fresh new blank state */
//EditorState* EditorMapIO::createBlankState();

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: Copies all data from source editor interactive object to this 
 *              editor interactive object.
 *
 * Inputs: EditorMapIO &source - the source to copy from
 * Output: none
 */
void EditorMapIO::copySelf(const EditorMapIO &source)
{
  /* Copy base data */
  EditorMapThing::copySelf(source);

  /* Remove all states, then add blank states up to size */
  unsetStates();
  while(states.size() < source.states.size())
    appendState(EditorEnumDb::IO_STATE);

  /* Set the state information */
  for(int i = 0; i < states.size(); i++)
    setState(i, source.states[i], true);

  /* Copy Inactive time */
  setInactiveTime(source.getInactiveTime());
}

/*
 * Description: Saves the data for the interactive object. This does not include
 *              the IO wrapper. Virtualized for other classes as well.
 *
 * Inputs: FileHandler* fh - the file handling data pointer
 *         bool game_only - true if only include game only data
 *         bool inc_matrix - true if include the base thing matrix
 * Output: none
 */
void EditorMapIO::saveData(FileHandler* fh, bool game_only, bool inc_matrix)
{
  EditorMapIO default_io;

  /* First write core thing data */
  EditorMapThing::saveData(fh, game_only, inc_matrix);

  /* Next IO data: Is not base - write core data */
  if(getBaseIO() != NULL)
  {
    /* Write the states */
    fh->writeXmlElement("states");
    for(int i = 0; i < states.size(); i++)
    {
      /* Header */
      if(states[i]->type == EditorEnumDb::IO_STATE)
        fh->writeXmlElement("state", "id", i);
      else
        fh->writeXmlElement("transition", "id", i);

      /* Write matrix */
      states[i]->matrix->save(fh, game_only, true);

      /* Event and interaction only relevant for state type */
      if(states[i]->type == EditorEnumDb::IO_STATE)
      {
        /* Write interaction trigger */
        if(states[i]->interact == MapState::USE)
          fh->writeXmlData("interaction", "use");
        else if(states[i]->interact == MapState::WALKOFF)
          fh->writeXmlData("interaction", "walkoff");
        else if(states[i]->interact == MapState::WALKON)
          fh->writeXmlData("interaction", "walkon");

        /* Enter event */
        if(states[i]->event_enter.classification != EventClassifier::NOEVENT)
        {
          EditorEvent event(states[i]->event_enter);
          event.save(fh, game_only, "enterevent");
        }

        /* Exit event */
        if(states[i]->event_exit.classification != EventClassifier::NOEVENT)
        {
          EditorEvent event(states[i]->event_exit);
          event.save(fh, game_only, "exitevent");
        }

        /* Use event */
        if(states[i]->event_use.classification != EventClassifier::NOEVENT)
        {
          EditorEvent event(states[i]->event_use);
          event.save(fh, game_only, "useevent");
        }

        /* Walkover event */
        if(states[i]->event_walkover.classification != EventClassifier::NOEVENT)
        {
          EditorEvent event(states[i]->event_walkover);
          event.save(fh, game_only, "walkoverevent");
        }
      }

      /* End Header */
      fh->writeXmlElementEnd(); /* </state> or </transition> */
    }
    fh->writeXmlElementEnd(); /* </states> */

    /* Render matrix */
    if(states.size() > 0)
      states.front()->matrix->saveRender(fh);

    /* Inactivity time */
    if(default_io.getInactiveTime() != getInactiveTime() &&
       getInactiveTime() >= 0)
    {
      fh->writeXmlData("inactive", getInactiveTime());
    }
  }
  /* Next item data: Is base - not used currently */
  //else
  //{
  //
  //}
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Appends a blank state to the back side of the stack with the
 *              passed in type.
 *
 * Inputs: EditorEnumDb::MapIOType type - the type of the state
 * Output: none
 */
void EditorMapIO::appendState(EditorEnumDb::MapIOType type)
{
  EditorState* state = createBlankState();
  state->type = type;
  appendState(state);
}

/*
 * Description: Appends the passed in state pointer to the back side of the
 *              stack. If this function returns true, it will take control of
 *              releasing the pointer memory when it no longer needs it.
 *
 * Inputs: EditorState* state - the new state to append
 * Output: bool - true if it was pushed to the back of the array
 */
bool EditorMapIO::appendState(EditorState* state)
{
  if(state != NULL)
  {
    if(states.size() > 0)
    {
      state->matrix->setTileIcons(tile_icons);
      state->matrix->rebase(states.front()->matrix, false);
    }
    states.push_back(state);
    return true;
  }
  return false;
}

/*
 * Description: Returns the base IO. Default to NULL.
 *
 * Inputs: none
 * Output: EditorMapIO* - the base IO reference pointer
 */
EditorMapIO* EditorMapIO:: getBaseIO() const
{
  if(getBaseThing() != NULL)
    return (EditorMapIO*)getBaseThing();
  return NULL;
}

/*
 * Description: Returns the inactive time before returning down the state path.
 *              This time will step back one state and then start timing again.
 *
 * Inputs: none
 * Output: int - the inactive time, in milliseconds
 */
int EditorMapIO::getInactiveTime() const
{
  EditorMapIO* base = getBaseIO();

  if(base != NULL)
    return base->getInactiveTime();
  return io.getInactiveTime();
}

/*
 * Description: Returns the IO state at the given index in the stack. Returns
 *              NULL if the index is out of range.
 *
 * Inputs: int index - the index of the state in the stack
 * Output: EditorState* - the state structure pointer
 */
EditorState* EditorMapIO::getState(int index)
{
  EditorMapIO* base = getBaseIO();

  /* Check if it's a base and the frames from it should be used instead */
  if(base != NULL)
  {
    if(index >= 0 && index < base->states.size())
      return base->states[index];
  }
  else
  {
    if(index >= 0 && index < states.size())
      return states[index];
  }

  return NULL;
}

/*
 * Description: Returns a vector of all states in the IO stack.
 *
 * Inputs: none
 * Output: QVector<EditorState*> - the vector of all states
 */
QVector<EditorState*> EditorMapIO::getStates()
{
  EditorMapIO* base = getBaseIO();

  /* Check if it's a base and the frames from it should be used instead */
  if(base != NULL)
    return base->states;
  return states;
}

/*
 * Description: Inserts a blank state of the indicated type at the index.
 *              The state at the index and all after are pushed back by one.
 *              Cannot insert in front of array at root.
 *
 * Inputs: int index - the index to insert the new state at
 *         EditorEnumDb::MapIOType type - the type of the state
 * Output: bool - true if the the state was inserted. Fails if the index is out
 *                of range.
 */
bool EditorMapIO::insertState(int index, EditorEnumDb::MapIOType type)
{
  EditorState* state = createBlankState();
  state->type = type;
  if(insertState(index, state))
    return true;

  /* Otherwise, just delete the state */
  delete state;
  return false;
}

/*
 * Description: Inserts the new state pointer at the index. The state at the
 *              index and all after are pushed back by one. If the function
 *              returns true, the class takes control of releasing the memory
 *              of the pointer. Can not insert in front of array at root.
 *
 * Inputs: int index - the index to insert the new state at
 *         EditorState* state - the state to insert at the index
 * Output: bool - true if the state was inserted. Fails if the index is out
 *                of range or the pointer is NULL.
 */
bool EditorMapIO::insertState(int index, EditorState* state)
{
  if(index > 0 && index <= states.size() && state != NULL)
  {
    if(states.size() > 0)
    {
      state->matrix->setTileIcons(tile_icons);
      state->matrix->rebase(states.front()->matrix, false);
    }
    states.insert(index, state);
    return true;
  }
  return false;
}

/*
 * Description: Loads the IO data from the XML struct and offset index.
 *
 * Inputs: XmlData data - the XML data tree struct
 *         int index - the offset index into the struct
 * Output: none
 */
void EditorMapIO::load(XmlData data, int index)
{
  QString element = QString::fromStdString(data.getElement(index));

  /* Parse elements */
  if(element == "inactive")
  {
    setInactiveTime(data.getDataInteger());
  }
  else if(element == "rendermatrix")
  {
    /* Ensure there is at least one */
    if(states.size() == 0)
      appendState(EditorEnumDb::IO_STATE);

    /* Next, load the render matrix in to each state */
    for(int i = 0; i < states.size(); i++)
      states[i]->matrix->load(data, index);
  }
  else if(element == "states")
  {
    /* Check index first */
    EditorState* state = NULL;
    int ref = QString::fromStdString(data.getKeyValue(index + 1)).toInt();
    if(ref >= 0)
    {
      while(states.size() <= ref)
        appendState(EditorEnumDb::IO_STATE);
      state = getState(ref);

      /* Proceed if not null */
      if(state != NULL)
      {
        /* Ensure type is right */
        QString element2 = QString::fromStdString(data.getElement(index + 1));
        if(element2 == "state")
          state->type = EditorEnumDb::IO_STATE;
        else if(element2 == "transition")
          state->type = EditorEnumDb::IO_TRANSITION;

        /* Process the objects inside the state type definition */
        QString element3 = QString::fromStdString(data.getElement(index + 2));
        /* ---- SPRITES (MATRIX) ---- */
        if(element3 == "sprites")
        {
          state->matrix->load(data, index + 3);
        }
        /* ---- TRANSITION INTERACTION ---- */
        else if(element3 == "interaction")
        {
          QString interaction = QString::fromStdString(data.getDataString());
          if(interaction == "use")
            state->interact = MapState::USE;
          else if(interaction == "walkoff")
            state->interact = MapState::WALKOFF;
          else if(interaction == "walkon")
            state->interact = MapState::WALKON;
        }
        /* ---- ENTER EVENT ---- */
        else if(element3 == "enterevent")
        {
          EditorEvent event(state->event_enter);
          event.load(data, index + 3);
          if(event.getEvent() != NULL)
            state->event_enter = *event.getEvent();
        }
        /* ---- EXIT EVENT ---- */
        else if(element3 == "exitevent")
        {
          EditorEvent event(state->event_exit);
          event.load(data, index + 3);
          if(event.getEvent() != NULL)
            state->event_enter = *event.getEvent();
        }
        /* ---- USE EVENT ---- */
        else if(element3 == "useevent")
        {
          EditorEvent event(state->event_use);
          event.load(data, index + 3);
          if(event.getEvent() != NULL)
            state->event_enter = *event.getEvent();
        }
        /* ---- WALKOVER EVENT ---- */
        else if(element3 == "walkoverevent")
        {
          EditorEvent event(state->event_walkover);
          event.load(data, index + 3);
          if(event.getEvent() != NULL)
            state->event_enter = *event.getEvent();
        }
      }
    }
  }
  else
  {
    EditorMapThing::load(data, index);
  }
}

/*
 * Description: Saves the IO data to the file handling pointer.
 *
 * Inputs: FileHandler* fh - the file handling pointer
 *         bool game_only - true if the data should include game only relevant
 * Output: none
 */
void EditorMapIO::save(FileHandler* fh, bool game_only)
{
  if(fh != NULL)
  {
    fh->writeXmlElement("mapio", "id", getID());
    saveData(fh, game_only);
    fh->writeXmlElementEnd();
  }
}

/*
 * Description: Sets the base IO object for this IO. It will be used for
 *              visual representation and when set, also sets the name and
 *              description (which can be changed later).
 *
 * Inputs: EditorMapIO* base_io - the base IO object
 * Output: none
 */
void EditorMapIO::setBase(EditorMapIO* base_io)
{
  EditorMapThing* thing = NULL;

  if(base_io != NULL)
    thing = base_io;

  EditorMapThing::setBase(thing);
}

/*
 * Description: Sets the inactive time for the IO state handling. This time is
 *              in milliseconds and indicates the amount of time the IO will
 *              remain at the current state before returning back towards the
 *              first state. If time is less than 0, it de-activates the time.
 *
 * Inputs: int time - the inactive time, in milliseconds
 * Output: none
 */
void EditorMapIO::setInactiveTime(int time)
{
  io.setInactiveTime(time);
}

/*
 * Description: Sets the state at the given index to the new pointer. If the
 *              class returns true, it takes control of the memory of the new
 *              state pointer. However, if data_only is true, it just copies
 *              the data from the state and does not delete or take control of
 *              the passed in state.
 *
 * Inputs: int index - index where to edit/replace the state
 *         EditorState* state - the state to edit/replace the state at the index
 *         bool data_only - default false. If true, the data from the state is
 *                          copied. If false, the state pointer is used to
 *                          replace the state at the index
 * Output: bool - true if the edit/replace occurred
 */
bool EditorMapIO::setState(int index, EditorState* state, bool data_only)
{
  if(index >= 0 && index < states.size() && state != NULL)
  {
    /* If data only, just make data cross over */
    if(data_only)
    {
      EditorState* ref = states[index];

      /* Base state data */
      *ref->matrix = *state->matrix;
      if(index == 0)
        ref->matrix->setTileIcons(tile_icons);
      ref->type = state->type;
      ref->interact = state->interact;

      /* Event state data */
      ref->event_enter = EventHandler::copyEvent(state->event_enter);
      ref->event_exit = EventHandler::copyEvent(state->event_exit);
      ref->event_use = EventHandler::copyEvent(state->event_use);
      ref->event_walkover = EventHandler::copyEvent(state->event_walkover);
    }
    /* Otherwise, this new pointer replaces the existing and the existing is
     * deleted. */
    else
    {
      if(index == 0)
        unsetMatrix();

      deleteState(states[index]);
      states.replace(index, state);

      if(index == 0)
      {
        state->matrix->setTileIcons(tile_icons);
        setMatrix(state->matrix);
      }
    }

    return true;
  }
  return false;
}

/*
 * Description: Sets the tile icons, for rendering purposes.
 *
 * Inputs: TileIcons* icons - the rendering icon pointer. Managed by gamedb
 * Output: none
 */
void EditorMapIO::setTileIcons(TileIcons* icons)
{
  for(int i = 0; i < states.size(); i++)
    states[i]->matrix->setTileIcons(icons);

  EditorMapThing::setTileIcons(icons);
}

/*
 * Description: Unsets a single state at the given index. The stack of states
 *              is automatically compressed after removing the state. Returns
 *              true if the state was removed. This deletes the memory.
 *
 * Inputs: int index - the index to remove the state from the stack.
 * Output: bool - true if the state at the index was removed.
 */
bool EditorMapIO::unsetState(int index)
{
  if(index > 0 && index < states.size())
  {
    deleteState(states[index]);
    states.remove(index);
    return true;
  }
  return false;
}

/*
 * Description: Unsets all states in the IO stack. This deletes the memory.
 *
 * Inputs: none
 * Output: none
 */
void EditorMapIO::unsetStates()
{
  while(states.size() > 1)
    unsetState(states.size() - 1);
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/*
 * Description: Copy operator construction. This is called when the variable
 *              already exists and equal operator used with another
 *              EditorMapIO.
 *
 * Inputs: const EditorMapIO &source - the source class constructor
 * Output: EditorMapIO& - pointer to the copied class
 */
EditorMapIO& EditorMapIO::operator= (const EditorMapIO &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Creates an entirely blank editor state structure. This is a
 *              public static function and retains no memory management. The
 *              caller assumes all responsibility for deleting the associated
 *              state pointer with a call to EditorMapIO::deleteState().
 *
 * Inputs: none
 * Output: EditorState* - the newly created blank state pointer
 */
EditorState* EditorMapIO::createBlankState()
{
  EditorState* state = new EditorState;

  state->matrix = new EditorMatrix();
  state->type = EditorEnumDb::IO_STATE;
  state->interact = MapState::NOINTERACTION;
  state->event_enter = EventHandler::createEventTemplate();
  state->event_exit = EventHandler::createEventTemplate();
  state->event_use = EventHandler::createEventTemplate();
  state->event_walkover = EventHandler::createEventTemplate();

  return state;
}

/*
 * Description: Deletes the state structure and all internal allocated memory.
 *              This is a public static function and is isolated from the
 *              EditorMapIO class.
 *
 * Inputs: EditorState* state - the state pointer to delete.
 * Output: none
 */
void EditorMapIO::deleteState(EditorState* state)
{
  if(state != NULL)
  {
    /* Delete memory in structure */
    delete state->matrix;
    EventHandler::deleteEvent(state->event_enter);
    EventHandler::deleteEvent(state->event_exit);
    EventHandler::deleteEvent(state->event_use);
    EventHandler::deleteEvent(state->event_walkover);

    /* Delete state structure */
    delete state;
  }
}
