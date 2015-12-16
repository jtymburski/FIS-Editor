/*******************************************************************************
 * Class Name: EditorMapIO
 * Date Created: July 11, 2015
 * Inheritance: EditorMapThing
 * Description: The class for managing the interfacing with MapInteractiveObject
 *              and filling it with data. The management pop-up is IODialog.
 ******************************************************************************/
#include "Database/EditorMapIO.h"
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

  /* Set the lock */
  lock.setLockBlank();
  lock_base = true;
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

  /* Copy the lock */
  setLock(source.lock);
  lock_base = source.lock_base;
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

  /* Next IO data: Is base - write core data */
  if(getBaseIO() == NULL)
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
        QString action = "";
        if(states[i]->interact == MapState::USE)
          action = "use";
        else if(states[i]->interact == MapState::WALKOFF)
          action = "walkoff";
        else if(states[i]->interact == MapState::WALKON)
          action = "walkon";
        fh->writeXmlData("interaction", action.toStdString());

        /* Set saving */
        states[i]->set_enter.save(fh, game_only, "enterset");
        states[i]->set_exit.save(fh, game_only, "exitset");
        states[i]->set_use.save(fh, game_only, "useset");
        states[i]->set_walkover.save(fh, game_only, "walkoverset");
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

    /* Lock */
    lock.save(fh, game_only);
  }
  /* Next item data: Is not base (an instance) */
  else
  {
    /* Write the states */
    bool generated = false;
    for(int i = 0; i < states.size(); i++)
    {
      EditorState* state_base = getState(i);
      EditorState* state_inst = getState(i, true);
      if(state_base != nullptr && state_inst != nullptr)
      {
        /* Event and interaction only relevant for state type */
        if(state_base->type == EditorEnumDb::IO_STATE &&
           (!state_inst->base_enter || !state_inst->base_exit ||
            !state_inst->base_use || !state_inst->base_walkover))
        {
          /* Wrapping header, if not generated already */
          if(!generated)
          {
            fh->writeXmlElement("states");
            generated = true;
          }

          /* Start Header */
          fh->writeXmlElement("state", "id", i);

          /* Set saving */
          if(!state_inst->base_enter)
            state_inst->set_enter.save(fh, game_only, "enterset", false, false);
          if(!state_inst->base_exit)
            state_inst->set_exit.save(fh, game_only, "exitset", false, false);
          if(!state_inst->base_use)
            state_inst->set_use.save(fh, game_only, "useset", false, false);
          if(!state_inst->base_walkover)
            state_inst->set_walkover.save(fh, game_only, "walkoverset",
                                          false, false);

          /* End Header */
          fh->writeXmlElementEnd(); /* </state> */
        }
      }
    }
    if(generated)
      fh->writeXmlElementEnd();

    /* Lock instance */
    if(!lock_base)
      lock.save(fh, game_only, "lock", false, false);
  }
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
 * Description: Consolidates matrix data on all nodes based on the reference
 *              index matrix.
 *
 * Inputs: int ref_index - the index in the node stack of the reference matrix
 * Output: none
 */
void EditorMapIO::consolidate(int ref_index)
{
  EditorState* ref_state = getState(ref_index);
  if(ref_state != NULL)
  {
    /* Loop through all states */
    for(int i = 0; i < states.size(); i++)
      if(states[i] != ref_state)
        states[i]->matrix->rebase(ref_state->matrix, false);
  }
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
 * Description: Returns a reference on the event enter set of the state index
 *              If the index is out of range or the index is not a STATE,
 *              returns nullptr.
 *
 * Inputs: int index - the index to access within the state set
 * Output: EditorEventSet* - the enter event set reference
 */
EditorEventSet* EditorMapIO::getEventEnter(int index)
{
  EditorEventSet* found = nullptr;

  /* Get states */
  EditorState* state_base = getState(index);
  EditorState* state_inst = getState(index, true);
  if(state_base != nullptr && state_inst != nullptr)
  {
    /* Check current class first (instance) */
    if(state_base != state_inst && !state_inst->base_enter)
      found = &state_inst->set_enter;

    /* If event is not found, proceed to base */
    if(found == nullptr)
      found = &state_base->set_enter;
  }

  return found;
}

/*
 * Description: Returns a reference on the event exit set of the state index
 *              If the index is out of range or the index is not a STATE,
 *              returns nullptr.
 *
 * Inputs: int index - the index to access within the state set
 * Output: EditorEventSet* - the exit event set reference
 */
EditorEventSet* EditorMapIO::getEventExit(int index)
{
  EditorEventSet* found = nullptr;

  /* Get states */
  EditorState* state_base = getState(index);
  EditorState* state_inst = getState(index, true);
  if(state_base != nullptr && state_inst != nullptr)
  {
    /* Check current class first (instance) */
    if(state_base != state_inst && !state_inst->base_exit)
      found = &state_inst->set_exit;

    /* If event is not found, proceed to base */
    if(found == nullptr)
      found = &state_base->set_exit;
  }

  return found;
}

/*
 * Description: Returns a reference on the event use set of the state index
 *              If the index is out of range or the index is not a STATE,
 *              returns nullptr.
 *
 * Inputs: int index - the index to access within the state set
 * Output: EditorEventSet* - the use event set reference
 */
EditorEventSet* EditorMapIO::getEventUse(int index)
{
  EditorEventSet* found = nullptr;

  /* Get states */
  EditorState* state_base = getState(index);
  EditorState* state_inst = getState(index, true);
  if(state_base != nullptr && state_inst != nullptr)
  {
    /* Check current class first (instance) */
    if(state_base != state_inst && !state_inst->base_use)
      found = &state_inst->set_use;

    /* If event is not found, proceed to base */
    if(found == nullptr)
      found = &state_base->set_use;
  }

  return found;
}

/*
 * Description: Returns a reference on the event walkover set of the state index
 *              If the index is out of range or the index is not a STATE,
 *              returns nullptr.
 *
 * Inputs: int index - the index to access within the state set
 * Output: EditorEventSet* - the walkover event set reference
 */
EditorEventSet* EditorMapIO::getEventWalkover(int index)
{
  EditorEventSet* found = nullptr;

  /* Get states */
  EditorState* state_base = getState(index);
  EditorState* state_inst = getState(index, true);
  if(state_base != nullptr && state_inst != nullptr)
  {
    /* Check current class first (instance) */
    if(state_base != state_inst && !state_inst->base_walkover)
      found = &state_inst->set_walkover;

    /* If event is not found, proceed to base */
    if(found == nullptr)
      found = &state_base->set_walkover;
  }

  return found;
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
 * Description: Returns the lock data reference of the IO
 *
 * Inputs: none
 * Output: EditorLock* - the lock reference. Never null
 */
EditorLock* EditorMapIO::getLock()
{
  EditorMapIO* base_io = getBaseIO();
  if(base_io != nullptr && lock_base)
    return base_io->getLock();
  return &lock;
}

/*
 * Description: Returns the IO state at the given index in the stack. Returns
 *              NULL if the index is out of range.
 *
 * Inputs: int index - the index of the state in the stack
 *         bool force_instance - true to always access instance state. default
 *                               as false
 * Output: EditorState* - the state structure pointer
 */
EditorState* EditorMapIO::getState(int index, bool force_instance)
{
  EditorMapIO* base = getBaseIO();

  /* Always ensure there is sufficient states in instance if base is set */
  if(base != nullptr)
  {
    while(states.size() < base->states.size())
      appendState(EditorEnumDb::IO_STATE);
    while(states.size() > base->states.size())
      unsetState(states.size() - 1);
  }

  /* Check if it's a base and the frames from it should be used instead */
  if(base != nullptr && !force_instance)
  {
    if(index >= 0 && index < base->states.size())
      return base->states[index];
  }
  else
  {
    if(index >= 0 && index < states.size())
      return states[index];
  }

  return nullptr;
}

/*
 * Description: Returns the state(s) and/or instance(s) comma delimited list
 *              for use. If inc_transition is true, transition numbers include
 *              'i' at end. States include nothing except the number.
 *              E.g. 0,1i,2,3i,4
 *
 * Inputs: bool inc_states - include states in list. default true
 *         bool inc_transitino - include transition in list. default false
 * Output: QString - comma delimited result. See description above
 */
QString EditorMapIO::getStateList(bool inc_states, bool inc_transition)
{
  QString list = "";

  /* Parse all states */
  for(int i = 0; i < states.size(); i++)
  {
    if(states[i]->type == EditorEnumDb::IO_STATE && inc_states)
    {
      list += QString::number(i) + ",";
    }
    else if(states[i]->type == EditorEnumDb::IO_TRANSITION && inc_transition)
    {
      list += QString::number(i) + "i,";
    }
  }

  /* Clean-up */
  if(list.length() > 0)
    list.chop(1);

  return list;
}

/*
 * Description: Returns the IO state name for storing in the list at the given
 *              index in the stack. Returns a blank string if the index is out
 *              of range.
 *
 * Inputs: int index - the index of the state in the stack
 * Output: QString - the state summary name
 */
QString EditorMapIO::getStateName(int index)
{
  EditorState* state = getState(index);
  QString name = "";

  if(state != NULL)
  {
    name = QString::number(index);
    if(name.size() < 2)
      name = "0" + name;
    if(name.size() < 3)
      name = "0" + name;
    name += ": ";
    if(state->type == EditorEnumDb::IO_STATE)
      name += "STATE";
    else
      name += "TRANSITION";
  }

  return name;
}

/*
 * Description: Returns a vector of all states in the IO stack.
 *
 * Inputs: none
 * Output: QVector<EditorState*> - the vector of all states
 */
QVector<EditorState*> EditorMapIO::getStates() const
{
  EditorMapIO* base = getBaseIO();

  /* Check if it's a base and the frames from it should be used instead */
  if(base != nullptr)
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
 * Description: Checks all matrixes at the given x and y location, if the
 *              sprites within are null. Virtualized.
 *
 * Inputs: int x - the x offset from top left in matrixes
 *         int y - the y offset from top left in matrixes
 * Output: bool - true if sprites contains only null paths.
 */
bool EditorMapIO::isAllNull(int x, int y) const
{
  bool is_null = true;
  QVector<EditorState*> states = getStates();

  /* Loop through all matrixes */
  for(int i = 0; i < states.size(); i++)
  {
    /* If matrix is valid */
    if(states[i]->matrix != NULL)
    {
      /* Get sprite, if valid */
      EditorTileSprite* sprite = states[i]->matrix->getSprite(x, y);
      if(sprite != NULL)
        is_null &= sprite->isAllNull();
    }
  }

  return is_null;
}

/*
 * Description: Returns if the lock returned by getLock() is the event from
 *              the base class. Only valid if getBaseThing() is not null.
 *
 * Inputs: none
 * Output: bool - true if the lock is the base lock
 */
bool EditorMapIO::isBaseLock() const
{
  return (getBaseIO() != nullptr) && (lock_base);
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
  else if(element == "lock")
  {
    lock.load(data, index + 1);
    lock_base = false;
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
      state = getState(ref, true);

      /* Proceed if not null */
      if(state != nullptr)
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
          state->set_enter.load(data, index + 2);
          state->base_enter = false;
        }
        /* ---- ENTER SET ---- */
        else if(element3 == "enterset")
        {
          state->set_enter.load(data, index + 3);
          state->base_enter = false;
        }
        /* ---- EXIT EVENT ---- */
        else if(element3 == "exitevent")
        {
          state->set_exit.load(data, index + 2);
          state->base_exit = false;
        }
        /* ---- EXIT SET ---- */
        else if(element3 == "exitset")
        {
          state->set_exit.load(data, index + 3);
          state->base_exit = false;
        }
        /* ---- USE EVENT ---- */
        else if(element3 == "useevent")
        {
          state->set_use.load(data, index + 2);
          state->base_use = false;
        }
        /* ---- USE SET ---- */
        else if(element3 == "useset")
        {
          state->set_use.load(data, index + 3);
          state->base_use = false;
        }
        /* ---- WALKOVER EVENT ---- */
        else if(element3 == "walkoverevent")
        {
          state->set_walkover.load(data, index + 2);
          state->base_walkover = false;
        }
        /* ---- WALKOVER SET ---- */
        else if(element3 == "walkoverset")
        {
          state->set_walkover.load(data, index + 3);
          state->base_walkover = false;
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
    saveData(fh, game_only, false);
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

  /* Set the base */
  if(base_io != NULL)
  {
    thing = base_io;
    lock_base = true;

    /* Update the states to be of similar length to base */
    unsetStates();
    while(states.size() < base_io->states.size())
      appendState(EditorEnumDb::IO_STATE);
  }

  EditorMapThing::setBase(thing);
}

/*
 * Description: Sets the enter event set for the given node index.
 *
 * Inputs: int index - the node index in the stack
 *         EditorEventSet set - the enter event set to set for the node
 * Output: bool - true if the event set was set
 */
bool EditorMapIO::setEventEnter(int index, EditorEventSet set)
{
  EditorState* state_inst = getState(index, true);
  if(state_inst != nullptr && state_inst->type == EditorEnumDb::IO_STATE)
  {
    state_inst->set_enter = set;
    state_inst->base_enter = false;
    return true;
  }
  return false;
}

/*
 * Description: Sets the exit event set for the given node index.
 *
 * Inputs: int index - the node index in the stack
 *         EditorEventSet set - the exit event set to set for the node
 * Output: bool - true if the event set was set
 */
bool EditorMapIO::setEventExit(int index, EditorEventSet set)
{
  EditorState* state_inst = getState(index, true);
  if(state_inst != nullptr && state_inst->type == EditorEnumDb::IO_STATE)
  {
    state_inst->set_exit = set;
    state_inst->base_exit = false;
    return true;
  }
  return false;
}

/*
 * Description: Sets the use event set for the given node index.
 *
 * Inputs: int index - the node index in the stack
 *         EditorEventSet set - the use event set to set for the node
 * Output: bool - true if the event set was set
 */
bool EditorMapIO::setEventUse(int index, EditorEventSet set)
{
  EditorState* state_inst = getState(index, true);
  if(state_inst != nullptr && state_inst->type == EditorEnumDb::IO_STATE)
  {
    state_inst->set_use = set;
    state_inst->base_use = false;
    return true;
  }
  return false;
}

/*
 * Description: Sets the walkover event set for the given node index.
 *
 * Inputs: int index - the node index in the stack
 *         EditorEventSet set - the walkover event set to set for the node
 * Output: bool - true if the event set was set
 */
bool EditorMapIO::setEventWalkover(int index, EditorEventSet set)
{
  EditorState* state_inst = getState(index, true);
  if(state_inst != nullptr && state_inst->type == EditorEnumDb::IO_STATE)
  {
    state_inst->set_walkover = set;
    state_inst->base_walkover = false;
    return true;
  }
  return false;
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
 * Description: Sets the restriction lock for the IO which restricts interaction
 *
 * Inputs: EditorLock lock - the new lock to use within the IO
 * Output: none
 */
void EditorMapIO::setLock(EditorLock lock)
{
  this->lock = lock;
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

      /* Base event reference */
      ref->base_enter = state->base_enter;
      ref->base_exit = state->base_exit;
      ref->base_use = state->base_use;
      ref->base_walkover = state->base_walkover;

      /* Event set data */
      ref->set_enter = state->set_enter;
      ref->set_exit = state->set_exit;
      ref->set_use = state->set_use;
      ref->set_walkover = state->set_walkover;
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
 * Description: Instructs the class to use the base enter event set, if there
 *              is a base reference class being used. If set true, the event is
 *              returned from the base class. Otherwise, this class holds the
 *              relevant event.
 *
 * Inputs: int index - index where to edit the use base setting
 *         bool use_base - true to use the event from the base class
 * Output: bool - true if the use base enter event was set
 */
bool EditorMapIO::setUseBaseEnter(int index, bool use_base)
{
  EditorState* state_inst = getState(index, true);
  if(state_inst != nullptr && state_inst->type == EditorEnumDb::IO_STATE)
  {
    state_inst->base_enter = use_base;
    return true;
  }
  return false;
}

/*
 * Description: Instructs the class to use the base exit event set, if there
 *              is a base reference class being used. If set true, the event is
 *              returned from the base class. Otherwise, this class holds the
 *              relevant event.
 *
 * Inputs: int index - index where to edit the use base setting
 *         bool use_base - true to use the event from the base class
 * Output: bool - true if the use base exit event was set
 */
bool EditorMapIO::setUseBaseExit(int index, bool use_base)
{
  EditorState* state_inst = getState(index, true);
  if(state_inst != nullptr && state_inst->type == EditorEnumDb::IO_STATE)
  {
    state_inst->base_exit = use_base;
    return true;
  }
  return false;
}

/*
 * Description: Instructs the class to use the base lock, if there is a base
 *              reference class being used. If set true, the lock is returned
 *              from the base class. Otherwise, this class holds the relevant
 *              lock.
 *
 * Inputs: bool use_base - true to use the lock from the base class
 * Output: none
 */
void EditorMapIO::setUseBaseLock(bool use_base)
{
  lock_base = use_base;
}

/*
 * Description: Instructs the class to use the base use event set, if there
 *              is a base reference class being used. If set true, the event is
 *              returned from the base class. Otherwise, this class holds the
 *              relevant event.
 *
 * Inputs: int index - index where to edit the use base setting
 *         bool use_base - true to use the event from the base class
 * Output: bool - true if the use base use event was set
 */
bool EditorMapIO::setUseBaseUse(int index, bool use_base)
{
  EditorState* state_inst = getState(index, true);
  if(state_inst != nullptr && state_inst->type == EditorEnumDb::IO_STATE)
  {
    state_inst->base_use = use_base;
    return true;
  }
  return false;
}

/*
 * Description: Instructs the class to use the base walkover event set, if there
 *              is a base reference class being used. If set true, the event is
 *              returned from the base class. Otherwise, this class holds the
 *              relevant event.
 *
 * Inputs: int index - index where to edit the use base setting
 *         bool use_base - true to use the event from the base class
 * Output: bool - true if the use base walkover event was set
 */
bool EditorMapIO::setUseBaseWalkover(int index, bool use_base)
{
  EditorState* state_inst = getState(index, true);
  if(state_inst != nullptr && state_inst->type == EditorEnumDb::IO_STATE)
  {
    state_inst->base_walkover = use_base;
    return true;
  }
  return false;
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
  state->base_enter = true;
  state->base_exit = true;
  state->base_use = true;
  state->base_walkover = true;
  state->set_enter.clear();
  state->set_exit.clear();
  state->set_use.clear();
  state->set_walkover.clear();

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

    /* Delete state structure */
    delete state;
  }
}
