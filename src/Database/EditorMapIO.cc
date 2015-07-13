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
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* Creates a fresh new blank state */
// TODO: Comment
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
  EditorMapThing::copySelf(source);

  // TODO: Implementation
  //io = source.io;
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

  /* First write thing data */
  EditorMapThing::saveData(fh, game_only, inc_matrix);

  /* Next item data: Is base - write core data */
  // TODO: Implementation
  //if(getBaseItem() == NULL)
  //{
  //  if(getCoreID() >= 0)
  //    fh->writeXmlData("core_id", getCoreID());
  //  if(default_item.isWalkover() != isWalkover())
  //    fh->writeXmlData("walkover", isWalkover());
  //}
  /* Next item data: Is not base */
  //else
  //{
  //  fh->writeXmlData("count", (int)getCount());
  //}
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

// TODO: Comment
void EditorMapIO::appendState(EditorEnumDb::MapIOType type)
{
  EditorState* state = createBlankState();
  state->type = type;
  appendState(state);
}

// TODO: Comment
bool EditorMapIO::appendState(EditorState* state)
{
  if(state != NULL)
  {
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

/* Returns the inactive time before returning down the state path (ms) */
// TODO: Comment
int EditorMapIO::getInactiveTime()
{
  return io.getInactiveTime();
}

/* Returns the state or states stored within the class */
// TODO: Comment
EditorState* EditorMapIO::getState(int index)
{
  if(index >= 0 && index < states.size())
    return states[index];
  return NULL;
}

/* Returns the state or states stored within the class */
// TODO: Comment
QVector<EditorState*> EditorMapIO::getStates()
{
  return states;
}

/* Insert state at index */
// TODO: Comment
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

/* Insert state at index */
// TODO: Comment
bool EditorMapIO::insertState(int index, EditorState* state)
{
  if(index >= 0 && index <= states.size() && state != NULL)
  {
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
        appendState(createBlankState());
      state = getState(ref);

      /* Proceed if not null */
      if(state != NULL)
      {
        /* Ensure type is right */
        QString element2 = QString::fromStdString(data.getElement(index + 1));
        if(element2 == "state")
          state->type = EditorEnumDb::IO_STATE;
        else if(element2 == "transition")
          state->type == EditorEnumDb::IO_TRANSITION;

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

/* Sets the inactive time before the state returns down the state path (ms) */
// TODO: Comment
void EditorMapIO::setInactiveTime(int time)
{
  io.setInactiveTime(time);
}

/* Sets the state at the index (will replace existing) */
// TODO: Comment
bool EditorMapIO::setState(int index, EditorState* state)
{
  if(index >= 0 && index < states.size() && state != NULL)
  {
    deleteState(states[index]);
    states.replace(index, state);
    return true;
  }
  return false;
}

/* Unset a state or unset all states */
// TODO: Comment
bool EditorMapIO::unsetState(int index)
{
  if(index >= 0 && index < states.size())
  {
    deleteState(states[index]);
    states.remove(index);
    return true;
  }
  return false;
}

/* Unset a state or unset all states */
// TODO: Comment
void EditorMapIO::unsetStates()
{
  while(states.size() > 0)
    unsetState(0);
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

/* Creates a fresh new blank state */
// TODO: Comment
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

/* Deletes a slate at the given pointer */
// TODO: Comment
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
