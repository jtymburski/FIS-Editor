/*******************************************************************************
 * Class Name: EditorEventSet
 * Date Created: November 15, 2015
 * Inheritance: none
 * Description: The class is for managing the interconnection to the game with
 *              EventSet in Game and the viewing/edit widget used in
 *              locations where it's used.
 ******************************************************************************/
#include "Database/EditorEventSet.h"
//#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Blank constructor for editor event set.
 *
 * Inputs: none
 */
EditorEventSet::EditorEventSet()
{
  EventSet set;
  unlocked_state = set.getUnlockedState();
}

/* Constructor function - with input event */
/*
 * Description: Constructor with event set struct input, as defined by EventSet.
 *
 * Inputs: Locked lock - an init lock struct
 */
EditorEventSet::EditorEventSet(EventSet& set) : EditorEventSet()
{
  setEventSet(set);
}

/*
 * Description: The copy constructor, with another editor event set as the
 *              source
 *
 * Inputs: const EditorEventSet &source - the source data
 */
EditorEventSet::EditorEventSet(const EditorEventSet &source) : EditorEventSet()
{
  copySelf(source);
}

/*
 * Description: Destructor function
 */
EditorEventSet::~EditorEventSet()
{
  clear();
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: The copy function that is called by any copying methods in the
 *              class. Utilized by the copy constructor and the copy operator.
 *
 * Inputs: const EditorEventSet &source - the reference editor event set class
 * Output: none
 */
void EditorEventSet::copySelf(const EditorEventSet &source)
{
  /* Clear old data */
  clear();

  /* Locked event */
  event_locked = source.event_locked;

  /* Unlocked event(s) */
  for(int i = 0; i < source.events_unlocked.size(); i++)
    addEventUnlocked(EditorEvent(*source.events_unlocked[i]));

  /* Lock data */
  lock_data = source.lock_data;

  /* Unlocked parsing state */
  unlocked_state = source.unlocked_state;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Adds an event to the unlocked stack of events that can be used.
 *
 * Inputs: Event new_event - the new event to add to the unlocked stack
 * Output: bool - true if event was added
 */
bool EditorEventSet::addEventUnlocked(Event new_event)
{
  EditorEvent event(new_event);
  bool success = addEventUnlocked(event);
  return success;
}

/*
 * Description: Adds an event to the unlocked stack of events that can be used.
 *              This version has already been converted to the Editor format.
 *
 * Inputs: EditorEvent new_event - the new editor event to add to the unlocked
 *                                 stack
 * Output: bool - true if event was added
 */
bool EditorEventSet::addEventUnlocked(EditorEvent new_event)
{
  EditorEvent* event_ptr = new EditorEvent(new_event);
  events_unlocked.push_back(event_ptr);
  return true;
}

/*
 * Description: Clears all the event set data from the class and unlocks and
 *              removes all lock information.
 *
 * Inputs: bool delete_data - true to delete data prior to clear. Otherwise,
 *                            just releases it. Default true.
 * Output: none
 */
void EditorEventSet::clear(bool delete_data)
{
  unsetEventLocked(delete_data);
  unsetEventUnlocked(delete_data);
  unsetLocked();
  //unlocked_state = UnlockedState::ORDERED;
}

/*
 * Description: Returns the defined event if the set is locked.
 *
 * Inputs: none
 * Output: EditorEvent* - the event associated with the locked status
 */
EditorEvent* EditorEventSet::getEventLocked()
{
  return &event_locked;
}

/*
 * Description: Converts the EditorEventSet into the EventSet game class and
 *              returns it. Used for storage within the various map objects
 *
 * Inputs: none
 * Output: EventSet - the game based EventSet class
 */
EventSet EditorEventSet::getEventSet()
{
  EventSet new_set;

  /* Locked event */
  new_set.setEventLocked(EventSet::copyEvent(*event_locked.getEvent()));

  /* Unlocked events */
  for(int i = 0; i < events_unlocked.size(); i++)
    new_set.addEventUnlocked(EventSet::copyEvent(
                                             *events_unlocked[i]->getEvent()));

  /* Locked data */
  new_set.setLocked(*lock_data.getLock());

  /* Unlocked events - how parsed */
  new_set.setUnlockedState(unlocked_state);

  return new_set;
}

/*
 * Description: Returns the stack of events if the set is unlocked.
 *
 * Inputs: none
 * Output: QVector<EditorEvent*> - the editor event(s) associated with the
 *                                 unlocked status
 */
QVector<EditorEvent*> EditorEventSet::getEventUnlocked()
{
  return events_unlocked;
}

/*
 * Description: Returns the editor event at the given index in the unlocked
 *              stack. It will be null if index is out of range.
 *
 * Inputs: int index - the index in the unlocked stack
 * Output: EditorEvent* - the editor event associated with the index in the
 *                        unlocked stack.
 */
EditorEvent* EditorEventSet::getEventUnlocked(int index)
{
  if(index >= 0 && index < getEventUnlockedTotal())
    return events_unlocked[index];
  return nullptr;
}

/*
 * Description: Returns the number of unlocked events in the stack (valid or
 *              not valid)
 *
 * Inputs: none
 * Output: int - stack length total
 */
int EditorEventSet::getEventUnlockedTotal()
{
  return events_unlocked.size();
}

/*
 * Desciption: Returns the locked state class. This defines the locked
 *             properties of the event set.
 *
 * Inputs: none
 * Output: EditorLock* - the editor locked state class
 */
EditorLock* EditorEventSet::getLockedState()
{
  return &lock_data;
}

/* Returns a text list summary of the event set */
// TODO: Comment
QVector<QString> EditorEventSet::getTextSummary()
{
  QVector<QString> stack;

  /* Lock struct */
  stack.push_back(lock_data.getTextSummary());

  /* Lock event */
  stack.push_back(event_locked.getTextSummary("Lock Event: "));

  /* Unlock event(s) */
  QString unlock = "Unlock Events (" +
                   QString::number(events_unlocked.size()) + ")";
  if(events_unlocked.size() > 0)
  {
    unlock += ": " + events_unlocked.front()->getTextSummary("");
    if(events_unlocked.size() > 1)
    {
      unlock += " and ";
      if(events_unlocked.size() > 2)
        unlock += QString::number(events_unlocked.size() - 1) + " others ";
      else
        unlock += events_unlocked[1]->getTextSummary("");
    }
  }
  stack.push_back(unlock);

  return stack;
}

/*
 * Description: Returns the unlocked state of the set. This defines how the
 *              unlocked stack of events is accessed and handled during a get
 *              call.
 *
 * Inputs: none
 * Output: UnlockedState - the unlocked state enum of the set.
 */
UnlockedState EditorEventSet::getUnlockedState()
{
  return unlocked_state;
}

/*
 * Description: Returns if the event set has any events or data set. If true,
 *              there are no events. It will always return true after a clear()
 *              call.
 *
 * Inputs: none
 * Output: bool - true if the event configuration is empty
 */
bool EditorEventSet::isEmpty() const
{
  /* Check unlocked */
  bool unlocked_valid = false;
  for(int i = 0; i < events_unlocked.size(); i++)
    unlocked_valid |= (events_unlocked[i]->getEventType() !=
                       EventClassifier::NOEVENT);

  return (event_locked.getEventType() == EventClassifier::NOEVENT &&
          !unlocked_valid && lock_data.getLockType() == LockedState::NONE);
}

/*
 * Description: Loads the event set data from the XML struct and offset index.
 *              Uses existing functions in game EventSet class.
 *
 * Inputs: XmlData data - the XML data tree struct
 *         int index - the offset index into the struct
 * Output: none
 */
void EditorEventSet::load(XmlData data, int index)
{
  EventSet set = getEventSet();
  set.loadData(data, index, 0);
  setEventSet(set);
}

/*
 * Description: Saves the event set data to the file handling pointer.
 *
 * Inputs: FileHandler* fh - the file handling pointer
 *         bool game_only - true if the data should include game only relevant
 *         QString preface - the wrapper text element. default to "eventset"
 *         bool no_preface - no XML wrapper included if true. Default false
 *         bool
 * Output: none
 */
void EditorEventSet::save(FileHandler* fh, bool game_only,
                          QString preface, bool no_preface, bool skip_empty)
{
  if(fh != nullptr)
  {
    /* If the event set has valid data */
    if(!isEmpty())
    {
      /* Wrapper */
      if(!no_preface)
        fh->writeXmlElement(preface.toStdString());

      /* Locked status */
      lock_data.save(fh, game_only);

      /* Locked event */
      event_locked.save(fh, game_only, "lockevent");

      /* Unlocked event(s) */
      for(int i = 0; i < events_unlocked.size(); i++)
      {
        if(events_unlocked[i]->getEventType() != EventClassifier::NOEVENT)
        {
          fh->writeXmlElement("unlockevent", "id", i);
          events_unlocked[i]->save(fh, game_only, "", true);
          fh->writeXmlElementEnd();
        }
      }

      /* Unlocked parse state */
      EventSet default_set;
      if(default_set.getUnlockedState() != unlocked_state)
      {
        std::string state = "";

        if(unlocked_state == UnlockedState::NONE)
          state = "none";
        else if(unlocked_state == UnlockedState::ORDERED)
          state = "ordered";
        else if(unlocked_state == UnlockedState::RANDOM)
          state = "random";

        if(!state.empty())
          fh->writeXmlData("unlockparse", state);
      }

      /* End Wrapper */
      if(!no_preface)
        fh->writeXmlElementEnd();
    }
    /* If instructed to not skip empty, print none option - used for */
    else if(!skip_empty)
    {
      int zero = 0;

      if(!no_preface)
        fh->writeXmlElement(preface.toStdString());
      fh->writeXmlData("none", zero);
      if(!no_preface)
        fh->writeXmlElementEnd();
    }
  }
}

/*
 * Description: Sets the event to be used while the set is locked. The passed in
 *              event needs to be deleted. Calls setEventLocked(EditorEvent)
 *              after converting the event.
 *
 * Inputs: Event new_event - the new event to use in a locked condition
 *         bool delete_event - true if the existing should be deleted
 * Output: bool - true if the locked event was set
 */
bool EditorEventSet::setEventLocked(Event new_event, bool delete_event)
{
  EditorEvent event(new_event);
  bool success = setEventLocked(event, delete_event);
  return success;
}

/*
 * Description: Sets the editor event to be used while the set is locked.
 *
 * Inputs: EditorEvent new_event - the new editor event to use for locked
 *         bool delete_event - true if the existing should be deleted
 * Output: bool - true if the locked editor event was set
 */
bool EditorEventSet::setEventLocked(EditorEvent new_event, bool delete_event)
{
  event_locked.setEventBlank(delete_event);
  event_locked = new_event;
  return true;
}

/*
 * Description: Takes the game based EventSet class and converts it to the
 *              EditorEventSet class.
 *
 * Inputs: EventSet& set - the game based class to convert
 *         bool delete_prev - delete the existing data. default true. If false,
 *                            just clears it without taking charge of memory
 * Output: bool - true if the event set was successful
 */
bool EditorEventSet::setEventSet(EventSet& set, bool delete_prev)
{
  /* First clear old data */
  clear(delete_prev);

  /* Locked event */
  Event* locked_event = set.getEventLockedRef(true);
  event_locked.setEvent(*locked_event);

  /* Unlocked events */
  std::vector<Event*> unlocked_events = set.getEventUnlockedRef(true);
  for(uint32_t i = 0; i < unlocked_events.size(); i++)
    addEventUnlocked(*unlocked_events[i]);

  /* Locked struct */
  Locked lock_struct = set.getLockedState();
  lock_data.setLock(lock_struct);

  /* Unlocked state */
  UnlockedState state = set.getUnlockedState();
  unlocked_state = state;

  return true;
}

/*
 * Description: Sets the event to be used within the unlocked stack set at the
 *              given index. If replace is true, will delete the event, at the
 *              index if it exists. Otherwise, it will push index+ back one and
 *              insert the new event. If the index is greater than the size of
 *              the array, it will push the event to the back of the stack. This
 *              class copies the data; caller must delete Event after call.
 *
 * Inputs: int index - the index in the unlocked stack to set the event
 *         Event new_event - the new event to insert or set
 *         bool replace - true to replace at index. false to insert. Default
 *                        false
 *         bool delete_event - true if the existing should be deleted
 * Output: bool - true if the insert or set was successful
 */
bool EditorEventSet::setEventUnlocked(int index, Event new_event, bool replace,
                                      bool delete_event)
{
  EditorEvent event(new_event);
  bool success = setEventUnlocked(index, event, replace, delete_event);
  return success;
}

/*
 * Description: Sets the editor event directly to be used within the unlocked
 *              stack set at the given index. If replace is true, will delete
 *              the event, at the index if it exists. Otherwise, it will push
 *              index+ back one and insert the new event. If the index is
 *              greater than the size of the array, it will push the editor
 *              event to the back of the stack.
 *
 *
 * Inputs: int index - the index in the unlocked stack to set the event
 *         EditorEvent new_event - the new editor event to insert or set
 *         bool replace - true to replace at index. false to insert. Default
 *                        false
 *         bool delete_event - true if the existing should be deleted
 * Output: bool - true if the insert or set was successful
 */
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
        events_unlocked[index]->setEventBlank(delete_event);
        delete events_unlocked[index];
        events_unlocked[index] = new EditorEvent(new_event);
      }
      /* Otherwise, append */
      else
      {
        events_unlocked.push_back(new EditorEvent(new_event));
      }
    }
    /* -- Insert only -- */
    else
    {

      /* If in range, insert. Otherwise, append */
      if(index < events_unlocked.size())
        events_unlocked.insert(index, new EditorEvent(new_event));
      else
        events_unlocked.push_back(new EditorEvent(new_event));
    }

    success = true;
  }

  return success;
}

/*
 * Description: Defines the locked information to use within the set.
 *
 * Inputs: Locked new_locked - the new locked struct with lock information
 * Output: bool - true if the locked struct was set
 */
bool EditorEventSet::setLocked(Locked new_locked)
{
  EditorLock lock(new_locked);
  return setLocked(lock);
}

/*
 * Description: Defined the editor locked class information to use within the
 *              set.
 *
 * Inputs: EditorLock new_locked - the new locked class with lock information
 * Output: bool - true if the locked data was set
 */
bool EditorEventSet::setLocked(EditorLock new_locked)
{
  lock_data = new_locked;
  return true;
}

/*
 * Description: Defines the unlocked enum which defines how the class accesses
 *              and utilizes the unlocked stack of events.
 *
 * Inputs: UnlockedState state - the state enumerator to use
 * Output: bool - true if the unlocked state was set
 */
bool EditorEventSet::setUnlockedState(UnlockedState state)
{
  unlocked_state = state;
  return true;
}

/*
 * Description: Shift the selected unlock index down in the list.
 *
 * Inputs: int index - the current index in the unlocked list
 * Output: int - the new index. -1 if not changed
 */
int EditorEventSet::shiftUnlockedDown(int index)
{
  /* Ensure index is in range */
  if(index >= 0 && index < (events_unlocked.size() - 1))
  {
    /* Remove the old index */
    EditorEvent* event_shift = events_unlocked[index];
    events_unlocked.remove(index);

    /* Find the new index and insert */
    int new_index = index + 1;
    events_unlocked.insert(new_index, event_shift);
    return new_index;
  }
  return -1;
}

/*
 * Description: Shift the selected unlock index up in the list.
 *
 * Inputs: int index - the current index in the unlocked list
 * Output: int - the new index. -1 if not changed
 */
int EditorEventSet::shiftUnlockedUp(int index)
{
  /* Ensure index is in range */
  if(index > 0 && index < events_unlocked.size())
  {
    /* Remove the old index */
    EditorEvent* event_shift = events_unlocked[index];
    events_unlocked.remove(index);

    /* Find the new index and insert */
    int new_index = index - 1;
    events_unlocked.insert(new_index, event_shift);
    return new_index;
  }
  return -1;
}

/*
 * Description: Unsets the locked event and replaces it with a blank one.
 *
 * Inputs: bool delete_event - true if the data should be deleted
 * Output: bool - true if the locked event was unset. Only fails if base set
 */
bool EditorEventSet::unsetEventLocked(bool delete_event)
{
  event_locked.setEventBlank(delete_event);
  return true;
}

/*
 * Description: Unsets the unlocked event at the given index. Once the index is
 *              removed, its position is removed from the stack.
 *
 * Inputs: int index - the index in the unlocked stack to remove. If out of
 *                     range, nothing occurs
 *         bool delete_event - true if the data should be deleted
 * Output: bool - true if the unlocked event was unset. Fails if out of range
 */
bool EditorEventSet::unsetEventUnlocked(int index, bool delete_event)
{
  if(index >= 0 && index < events_unlocked.size())
  {
    events_unlocked[index]->setEventBlank(delete_event);
    delete events_unlocked[index];
    events_unlocked.remove(index);
    return true;
  }
  return false;
}

/*
 * Description: Unsets all unlocked events from the stack and clears out the
 *              stack.
 *
 * Inputs: bool delete_event - true if the data should be deleted
 * Output: bool - true if the unlocked events were unset.
 */
bool EditorEventSet::unsetEventUnlocked(bool delete_events)
{
  for(int i = 0; i < events_unlocked.size(); i++)
  {
    events_unlocked[i]->setEventBlank(delete_events);
    delete events_unlocked[i];
  }
  events_unlocked.clear();
  return true;
}

/*
 * Description: Unsets the locked struct information and replaces it with a
 *              blank permanently unlocked version.
 *
 * Inputs: none
 * Output: bool - true if the locked struct was unset.
 */
bool EditorEventSet::unsetLocked()
{
  lock_data = EventSet::createBlankLocked();
  return true;
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/*
 * Description: Copy operator construction. This is called when the variable
 *              already exists and equal operator used with another
 *              EditorEventSet.
 *
 * Inputs: const EditorEventSet& source - the source class constructor
 * Output: EditorEventSet& - pointer to the copied class
 */
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
