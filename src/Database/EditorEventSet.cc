/*******************************************************************************
 * Class Name: EditorEventSet
 * Date Created: November 15, 2015
 * Inheritance: none
 * Description: The class is for managing the interconnection to the game with
 *              EventSet in Game and the viewing/edit widget used in
 *              locations where it's used.
 ******************************************************************************/
#include "Database/EditorEventSet.h"

/* Constant Implementation - see header file for descriptions */
const QString EditorEventSet::kKEY_ACCESS_NONE = "none";
const QString EditorEventSet::kKEY_ACCESS_RANDOM = "random";
const QString EditorEventSet::kKEY_ACCESS_SEQUENTIAL = "ordered";
const QString EditorEventSet::kKEY_LOCK = "lock";
const QString EditorEventSet::kKEY_EVENT_LOCK = "lockevent";
const QString EditorEventSet::kKEY_EVENT_UNLOCK = "unlockevent";
const QString EditorEventSet::kKEY_EVENT_UNLOCK_ACCESS = "unlockparse";
const QString EditorEventSet::kKEY_EVENT_UNLOCK_ID = "id";

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
  unlocked_access = source.unlocked_access;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

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

/*
 * Description: Returns a text list summary of the event set for view.
 *
 * Inputs: none
 * Output: QVector<QString> - the set of strings describing the event set
 */
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
 * Description: Returns the unlocked access state of the set. This defines how the
 *              unlocked stack of events is accessed and handled during a get
 *              call.
 *
 * Inputs: none
 * Output: UnlockedState - the unlocked state enum of the set.
 */
core::AccessOperation EditorEventSet::getUnlockedAccess()
{
  return unlocked_access;
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
                       core::EventType::NONE);

  return (event_locked.getEventType() == core::EventType::NONE &&
          !unlocked_valid && lock_data.getLockType() == core::LockType::NONE);
}

/*
 * Description: Loads the event set data from the XML struct and offset index.
 *              Uses existing functions in game EventSet class.
 *
 * Inputs: core::XmlData data - the XML data tree struct
 *         int index - the offset index into the struct
 * Output: none
 */
void EditorEventSet::load(core::XmlData data, int index)
{
  QString element = QString::fromStdString(data.getElement(index));

  /* Parse elements */
  if(element == kKEY_EVENT_LOCK)
  {
    event_locked.load(data, index + 1);
  }
  else if(element == kKEY_EVENT_UNLOCK)
  {
    QString key = QString::fromStdString(data.getKey(index));
    int unlock_index = std::stoi(data.getKeyValue(index));
    if(key == kKEY_EVENT_UNLOCK_ID && unlock_index >= 0)
    {
      while(unlock_index >= events_unlocked.size())
        events_unlocked.append(new EditorEvent());
      getEventUnlocked(index)->load(data, index + 1);
    }
  }
  else if(element == kKEY_EVENT_UNLOCK_ACCESS)
  {
    QString access_str = QString::fromStdString(data.getDataStringOrThrow());

    if(access_str == kKEY_ACCESS_NONE)
      unlocked_access = core::AccessOperation::NONE;
    else if(access_str == kKEY_ACCESS_SEQUENTIAL)
      unlocked_access = core::AccessOperation::SEQUENTIAL;
    else if(access_str == kKEY_ACCESS_RANDOM)
      unlocked_access = core::AccessOperation::RANDOM;
    else
      throw std::domain_error("Access operation mapping for load event set is not defined");
  }
  else if(element == kKEY_LOCK)
  {
    lock_data.load(data, index + 1);
  }
}

/*
 * Description: Saves the event set data to the file handling pointer.
 *
 * Inputs: core::XmlWriter* writer - the save persistence interface
 *         QString wrapper - the wrapper text element. default to "eventset"
 *         bool write_wrapper - should the save wrap the event in a parent element? Default true
 *         bool skip_empty - false to save even if empty (default true)
 * Output: none
 */
void EditorEventSet::save(core::XmlWriter* writer, QString wrapper, bool write_wrapper,
                          bool skip_empty)
{
  if(writer != nullptr && (!isEmpty() || !skip_empty))
  {
    /* Wrapper */
    if(write_wrapper)
      writer->writeElement(wrapper.toStdString());

    /* Locked status */
    lock_data.save(writer, kKEY_LOCK);

    /* Locked event */
    event_locked.save(writer, kKEY_EVENT_LOCK);

    /* Unlocked event(s) */
    for(int i = 0; i < events_unlocked.size(); i++)
    {
      if(events_unlocked[i]->getEventType() != core::EventType::NONE)
      {
        writer->writeElement(kKEY_EVENT_UNLOCK.toStdString(), kKEY_EVENT_UNLOCK_ID.toStdString(),
                             std::to_string(i));
        events_unlocked[i]->save(writer, "", false);
        writer->jumpToParent();
      }
    }

    /* Unlocked parse state */
    if(unlocked_access != core::AccessOperation::SEQUENTIAL)
    {
      QString access_str;

      if(unlocked_access == core::AccessOperation::NONE)
        access_str = kKEY_ACCESS_NONE;
      else if(unlocked_access == core::AccessOperation::SEQUENTIAL)
        access_str = kKEY_ACCESS_SEQUENTIAL;
      else if(unlocked_access == core::AccessOperation::RANDOM)
        access_str = kKEY_ACCESS_RANDOM;
      else
        throw std::domain_error("Access operation mapping for save event set is not defined");

      if(!access_str.isEmpty())
        writer->writeData(kKEY_EVENT_UNLOCK_ACCESS.toStdString(), access_str.toStdString());
    }

    /* End Wrapper */
    if(write_wrapper)
      writer->jumpToParent();
  }
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
bool EditorEventSet::setUnlockedAccess(core::AccessOperation access)
{
  unlocked_access = access;
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
  lock_data = EditorLock();
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
