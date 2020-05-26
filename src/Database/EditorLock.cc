/*******************************************************************************
 * Class Name: EditorLock
 * Date Created: November 15, 2015
 * Inheritance: none
 * Description: The class is for managing the interconnection to the game with
 *              Lock struct in EventSet and the viewing/edit widget used in
 *              locations where it's used.
 ******************************************************************************/
#include "Database/EditorLock.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Blank constructor for editor lock.
 *
 * Inputs: none
 */
EditorLock::EditorLock()
{
  setLockBlank();
}

/*
 * Description: Constructor with single lock struct, as created by EventSet.
 *
 * Inputs: Locked lock - an init lock struct
 */
EditorLock::EditorLock(Locked lock) : EditorLock()
{
  setLock(lock);
}

/*
 * Description: The copy constructor, with another editor lock as the source
 *
 * Inputs: const EditorLock &source - the source data
 */
EditorLock::EditorLock(const EditorLock &source) : EditorLock()
{
  copySelf(source);
}

/*
 * Description: Destructor function
 */
EditorLock::~EditorLock()
{
  setLockBlank();
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: The copy function that is called by any copying methods in the
 *              class. Utilized by the copy constructor and the copy operator.
 *
 * Inputs: const EditorLock &source - the reference editor lock class
 * Output: none
 */
void EditorLock::copySelf(const EditorLock &source)
{
  lock = source.lock;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Returns the have item consume flag. If it's not the have item
 *              lock, false is returned
 *
 * Inputs: none
 * Output: bool - true if the item will be consumed on check
 */
bool EditorLock::getHaveItemConsume()
{
  int id, count;
  bool consume;
  if(EventSet::dataLockedItem(lock, id, count, consume))
    return consume;
  return false;
}

/*
 * Description: Returns the have item lock item count. If it's not the
 *              have item lock, 0 is returned.
 *
 * Inputs: none
 * Output: int - number of items to be checked in the lock struct
 */
int EditorLock::getHaveItemCount()
{
  int id, count;
  bool consume;
  if(EventSet::dataLockedItem(lock, id, count, consume))
    return count;
  return 0;
}

/*
 * Description: Returns the have item lock item ID. If it's not the
 *              have item lock, -1 is returned.
 *
 * Inputs: none
 * Output: int - the ID of the item, to be checked
 */
int EditorLock::getHaveItemID()
{
  int id, count;
  bool consume;
  if(EventSet::dataLockedItem(lock, id, count, consume))
    return id;
  return EventSet::kUNSET_ID;
}

/*
 * Description: Returns the current lock struct, set-up in this class.
 *
 * Inputs: none
 * Output: Locked* - pointer to the lock struct contained
 */
Locked* EditorLock::getLock()
{
  return &lock;
}

/*
 * Description: Returns the type of lock handled by this class.
 *
 * Inputs: none
 * Output: LockedState - lock classification enum
 */
LockedState EditorLock::getLockType() const
{
  return lock.state;
}

/*
 * Description: Returns the text based summary of what defines the Editor Lock
 *
 * Inputs: QString prefix - the text to prepend. Default "Lock: "
 * Output: QString - the returned string description
 */
QString EditorLock::getTextSummary(QString prefix)
{
  QString content = "";
  if(getLockType() == LockedState::ITEM)
    content = "Have Item";// ID " + QString::number(getHaveItemID());
  else if(getLockType() == LockedState::TRIGGER)
    content = "Trigger";
  else
    content = "None";

  return (prefix + content);
}

/*
 * Description: Returns if the lock event is set and if flagged as permanent.
 *              This defines if during the game, the lock is unlocked, it stays
 *              unlocked infinitely.
 *
 * Inputs: none
 * Output: bool - true if the lock is permanent
 */
bool EditorLock::isPermanent()
{
  if(lock.state != LockedState::NONE)
    return lock.permanent;
  return false;
}

/*
 * Description: Loads the lock data from the XML struct and offset index. Uses
 *              existing functions in game EventSet class.
 *
 * Inputs: XmlData data - the XML data tree struct
 *         int index - the offset index into the struct
 * Output: none
 */
void EditorLock::load(XmlData data, int index)
{
  lock = EventSet::updateLocked(lock, data, index);
}

/*
 * Description: Saves the lock data to the file handling pointer.
 *
 * Inputs: FileHandler* fh - the file handling pointer
 *         bool game_only - true if the data should include game only relevant
 *         QString preface - the wrapper text element. default to "lock"
 *         bool no_preface - no XML wrapper included if true. Default false
 *         bool skip_empty - true to skip the empty state and save nothing
 * Output: none
 */
void EditorLock::save(FileHandler* fh, bool game_only, QString preface,
                      bool no_preface, bool skip_empty)
{
  (void)game_only;

  if(fh != nullptr)
  {
    /* If the lock has valid data */
    if(lock.state != LockedState::NONE)
    {
      if(!no_preface)
        fh->writeXmlElement(preface.toStdString());

      /* -- HAVE ITEM LOCK -- */
      if(lock.state == LockedState::ITEM)
      {
        Locked lock_struct = EventSet::createLockHaveItem();
        EditorLock default_lock(lock_struct);

        fh->writeXmlElement("item");

        /* Data */
        fh->writeXmlData("id", getHaveItemID());
        if(getHaveItemCount() != default_lock.getHaveItemCount())
          fh->writeXmlData("count", getHaveItemCount());
        if(getHaveItemConsume() != default_lock.getHaveItemConsume())
          fh->writeXmlData("consume", getHaveItemConsume());
        if(isPermanent() != default_lock.isPermanent())
          fh->writeXmlData("permanent", isPermanent());

        fh->writeXmlElementEnd();
      }
      else if(lock.state == LockedState::TRIGGER)
      {
        Locked lock_struct = EventSet::createLockTriggered();
        EditorLock default_lock(lock_struct);

        /* Parse how the save is handled depending on data */
        if(isPermanent() != default_lock.isPermanent())
        {
          fh->writeXmlElement("trigger");
          fh->writeXmlData("permanent", isPermanent());
          fh->writeXmlElementEnd();
        }
        else
        {
          int none = 0;
          fh->writeXmlData("trigger", none);
        }
      }

      if(!no_preface)
        fh->writeXmlElementEnd();
    }
    /* If instructed to not skip empty, print none option - used for instance */
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
 * Description: Sets the lock to the passed in locked handler (this).
 *
 * Inputs: Locked lock - the lock to load in
 * Output: none
 */
void EditorLock::setLock(Locked lock)
{
  this->lock = lock;
}

/*
 * Description: Sets the lock in the class to the blank unused lock.
 *
 * Inputs: none
 * Output: none
 */
void EditorLock::setLockBlank()
{
  lock = EventSet::createBlankLocked();
}

/*
 * Description: Sets the lock in the class to the have item lock, with a
 *              passed in ID of the item and count (ID needs to be greater
 *              than or equal to 0 and count greater than 0).
 *
 * Inputs: int id - the id of the item to have (game side)
 *         int count - the number of items to check on trigger
 *         bool consume - true if the items are consumed if they exist
 *         bool permanent - true if the lock once unlocked is permanent
 * Output: bool - true if the lock was created
 */
bool EditorLock::setLockHaveItem(int id, int count, bool consume,
                                 bool permanent)
{
  if(id >= 0 && count > 0)
  {
    lock = EventSet::createLockHaveItem(id, count, consume, permanent);
    return true;
  }
  return false;
}

/*
 * Description: Sets the lock in the class to the trigger based unlock.
 *
 * Inputs: bool permanent - true if the lock once unlocked is permanent
 * Output: bool - true if the lock was created
 */
bool EditorLock::setLockTrigger(bool permanent)
{
  lock = EventSet::createLockTriggered(permanent);
  return true;
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/*
 * Description: Copy operator construction. This is called when the variable
 *              already exists and equal operator used with another editor
 *              lock.
 *
 * Inputs: const EditorLock &source - the source class constructor
 * Output: EditorLock& - pointer to the copied class
 */
EditorLock& EditorLock::operator= (const EditorLock &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
