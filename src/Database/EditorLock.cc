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
 * Description: Constructor with single lock object.
 *
 * Inputs: core::Lock* lock - an init lock
 */
EditorLock::EditorLock(core::Lock* lock) : EditorLock()
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
  delete lock;
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
  delete lock;
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
  if(getLockType() == core::LockType::ITEM)
    return static_cast<core::LockItem*>(lock)->isConsumeToUnlock();
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
  if(getLockType() == core::LockType::ITEM)
    return static_cast<core::LockItem*>(lock)->getItemCount();
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
  if(getLockType() == core::LockType::ITEM)
    return static_cast<core::LockItem*>(lock)->getItemId();
  return core::LockItem::kUNSET_ITEM_ID;
}

/*
 * Description: Returns the current lock object, set-up in this class.
 *
 * Inputs: none
 * Output: core::Lock* - pointer to the lock object managed internally
 */
core::Lock* EditorLock::getLock()
{
  return lock;
}

/*
 * Description: Returns the type of lock handled by this class.
 *
 * Inputs: none
 * Output: core::LockType - lock classification enum
 */
core::LockType EditorLock::getLockType() const
{
  return lock != nullptr ? lock->getType() : core::LockType::NONE;
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
  if(getLockType() == core::LockType::ITEM)
    content = "Have Item";// ID " + QString::number(getHaveItemID());
  else if(getLockType() == core::LockType::TRIGGER)
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
  if(getLockType() != core::LockType::NONE)
    return static_cast<core::FunctionalLock*>(lock)->isPermanent();
  return false;
}

/*
 * Description: Loads the lock data from the XML struct and offset index. Uses
 *              existing functions in game EventSet class.
 *
 * Inputs: core::XmlData data - the XML data tree struct
 *         int index - the offset index into the struct
 * Output: none
 */
void EditorLock::load(core::XmlData data, int index)
{
  lock = core::PersistLock::load(lock, data, index);
}

/*
 * Description: Saves the lock data to the file handling pointer.
 *
 * Inputs: core::XmlWriter* writer - the save persistence interface
 *         QString wrapper - the wrapper text element. default to "lock"
 *         bool write_wrapper - TRUE (default) to wrap the lock save. FALSE to directly write
 *         bool skip_empty - TRUE (default) to skip the empty state. FALSE to save even if blank
 * Output: none
 */
void EditorLock::save(core::XmlWriter* writer, QString wrapper, bool write_wrapper, bool skip_empty)
{
  if(writer != nullptr && lock != nullptr && (lock->isSaveable() || !skip_empty))
  {
    if(write_wrapper)
      writer->writeElement(wrapper.toStdString());

    core::PersistLock::save(lock, writer);

    if(write_wrapper)
      writer->jumpToParent();
  }
}

/*
 * Description: Sets the lock to the passed in locked handler (this).
 *
 * Inputs: core::Lock* lock - the lock to load in
 * Output: none
 */
void EditorLock::setLock(core::Lock* lock)
{
  setLockBlank();
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
  delete lock;
  lock = new core::LockNone();
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
    core::LockItem* item_lock = new core::LockItem();
    item_lock->setItemId(id);
    item_lock->setItemCount(count);
    item_lock->setConsumeToUnlock(consume);
    item_lock->setPermanent(permanent);

    setLock(item_lock);
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
  core::LockTrigger* trigger_lock = new core::LockTrigger();
  trigger_lock->setPermanent(permanent);

  setLock(trigger_lock);
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
