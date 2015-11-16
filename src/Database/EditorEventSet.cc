/*******************************************************************************
 * Class Name: EditorEventSet
 * Date Created: November 15, 2015
 * Inheritance: none
 * Description: The class is for managing the interconnection to the game with
 *              EventSet in Game and the viewing/edit widget used in
 *              locations where it's used.
 ******************************************************************************/
#include "Database/EditorEventSet.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
EditorEventSet::EditorEventSet()
{
  unlocked_state = UnlockedState::ORDERED;
}

/* Constructor function - with input event */
EditorEventSet::EditorEventSet(const EventSet& set) : EditorEventSet()
{
  setEventSet(set);
}

/* Copy constructor */
EditorEventSet::EditorEventSet(const EditorEventSet &source) : EditorEventSet()
{
  copySelf(source);
}

/* Destructor function */
EditorEventSet::~EditorEventSet()
{
  clear();
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* Copy function, to be called by a copy or equal operator constructor */
void EditorEventSet::copySelf(const EditorEventSet &source)
{

}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Add calls, for lists */
bool EditorEventSet::addEventUnlocked(Event new_event)
{
  EditorEvent event(new_event);
  bool success = addEventUnlocked(event);
  return success;
}

/* Add calls, for lists */
bool EditorEventSet::addEventUnlocked(EditorEvent new_event)
{
  events_unlocked.push_back(new_event);
  return true;
}

/* Clears all event data within the class */
void EditorEventSet::clear(bool delete_data)
{
  unsetEventLocked(delete_data);
  unsetEventUnlocked(delete_data);
  unsetLocked();
  unlocked_state = UnlockedState::NONE;
}

/* Access getters for locked event */
EditorEvent* EditorEventSet::getEventLocked()
{
  return &event_locked;
}

/* Returns the event set conversion of the Editor Event Set */
EventSet* EditorEventSet::getEventSet()
{

}

/* Access getters for unlocked event(s) */
QVector<EditorEvent> EditorEventSet::getEventUnlocked()
{
  return events_unlocked;
}

/* Access getters for unlocked event(s) */
EditorEvent* EditorEventSet::getEventUnlocked(int index)
{
  if(index >= 0 && index < getEventUnlockedTotal())
    return &events_unlocked[index];
  return nullptr;
}

/* Access getters for unlocked event(s) */
int EditorEventSet::getEventUnlockedTotal()
{
  return events_unlocked.size();
}

/* Returns the locked state struct */
EditorLock* EditorEventSet::getLockedState()
{
  return &lock_data;
}

/* Returns the unlocked state enum */
UnlockedState EditorEventSet::getUnlockedState()
{
  return unlocked_state;
}

/* Returns if the class is empty (default state after a clear() call) */
bool EditorEventSet::isEmpty()
{
  return (event_locked.getEventType() == EventClassifier::NOEVENT &&
          events_unlocked.size() == 0 &&
          lock_data.getLockType() == LockedState::NONE);
}

/* Loads the event data */
void EditorEventSet::load(XmlData data, int index)
{

}

/* Saves the event data */
void EditorEventSet::save(FileHandler* fh, bool game_only,
                          QString preface, bool no_preface)
{

}

/* Individual setters for events */
bool EditorEventSet::setEventLocked(Event new_event, bool delete_event)
{
  EditorEvent event(new_event);
  bool success = setEventLocked(event, delete_event);
  return success;
}

/* Individual setters for events */
bool EditorEventSet::setEventLocked(EditorEvent new_event, bool delete_event)
{
  event_locked.setEventBlank(delete_event);
  event_locked = new_event;
  return true;
}

/* Set for the event set */
bool EditorEventSet::setEventSet(const EventSet& set, bool delete_prev)
{

}

/* Individual setters for events */
bool EditorEventSet::setEventUnlocked(int index, Event new_event, bool replace,
                                      bool delete_event)
{
  EditorEvent event(new_event);
  bool success = setEventUnlocked(index, event, replace, delete_event);
  return success;
}

/* Individual setters for events */
bool EditorEventSet::setEventUnlocked(int index, EditorEvent new_event,
                                      bool replace, bool delete_event)
{
  bool success = false;

  if(index >= 0)
  {
    /* -- Replace existing -- */
    if(replace)
    {
      /* If in range, find and replace */
      if(index < events_unlocked.size())
      {
        events_unlocked[index].setEventBlank(delete_event);
        events_unlocked[index] = new_event;
      }
      /* Otherwise, append */
      else
      {
        events_unlocked.push_back(new_event);
      }
    }
    /* -- Insert only -- */
    else
    {

      /* If in range, insert. Otherwise, append */
      if(index < events_unlocked.size())
        events_unlocked.insert(index, new_event);
      else
        events_unlocked.push_back(new_event);
    }

    success = true;
  }

  return success;
}

/* Sets the locked status */
bool EditorEventSet::setLocked(Locked new_locked)
{
  EditorLock lock(new_locked);
  return setLocked(lock);
}

/* Sets the locked status */
bool EditorEventSet::setLocked(EditorLock new_locked)
{
  lock_data = new_locked;
  return true;
}

/* Sets the unlocked state */
bool EditorEventSet::setUnlockedState(UnlockedState state)
{
  unlocked_state = state;
  return true;
}

/* Unset calls */
bool EditorEventSet::unsetEventLocked(bool delete_event)
{
  event_locked.setEventBlank(delete_event);
  return true;
}

/* Unset calls */
bool EditorEventSet::unsetEventUnlocked(int index, bool delete_event)
{
  if(index >= 0 && index < events_unlocked.size())
  {
    events_unlocked[index].setEventBlank(delete_event);
    events_unlocked.remove(index);
    return true;
  }
  return false;
}

/* Unset calls */
bool EditorEventSet::unsetEventUnlocked(bool delete_events)
{
  for(int i = 0; i < events_unlocked.size(); i++)
    events_unlocked[i].setEventBlank(delete_events);
  events_unlocked.clear();
  return true;
}

/* Unset calls */
bool EditorEventSet::unsetLocked()
{
  lock_data = EventSet::createBlankLocked();
  return true;
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/* The copy operator */
EditorEventSet& EditorEventSet::operator= (const EditorEventSet &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
