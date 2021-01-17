/*******************************************************************************
 * Class Name: EditorEventSet
 * Date Created: November 15, 2015
 * Inheritance: none
 * Description: The class is for managing the interconnection to the game with
 *              EventSet in Game and the viewing/edit widget used in
 *              locations where it's used.
 ******************************************************************************/
#ifndef EDITOREVENTSET_H
#define EDITOREVENTSET_H

#include <QString>
#include <QVector>

#include "Database/EditorEvent.h"
#include "Database/EditorLock.h"
#include "Foundation/AccessOperation.h"

class EditorEventSet
{
public:
  /* Constructor function */
  EditorEventSet();

  /* Copy constructor */
  EditorEventSet(const EditorEventSet &source);

  /* Destructor function */
  ~EditorEventSet();

private:
  /* Event sets */
  EditorEvent event_locked;
  QVector<EditorEvent*> events_unlocked;

  /* Locked status struct */
  EditorLock lock_data;

  /* State of access for unlocked events */
  core::AccessOperation unlocked_access = core::AccessOperation::SEQUENTIAL;

  /*------------------- Constants -----------------------*/
private:
  /* Load and save key names */
  const static QString kKEY_ACCESS_NONE;
  const static QString kKEY_ACCESS_RANDOM;
  const static QString kKEY_ACCESS_SEQUENTIAL;
  const static QString kKEY_LOCK;
  const static QString kKEY_EVENT_LOCK;
  const static QString kKEY_EVENT_UNLOCK;
  const static QString kKEY_EVENT_UNLOCK_ACCESS;
  const static QString kKEY_EVENT_UNLOCK_ID;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorEventSet &source);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Add calls, for lists */
  bool addEventUnlocked(EditorEvent new_event);

  /* Clears all event data within the class */
  void clear(bool delete_data = true);

  /* Access getters for locked event */
  EditorEvent* getEventLocked();

  /* Access getters for unlocked event(s) */
  QVector<EditorEvent*> getEventUnlocked();
  EditorEvent* getEventUnlocked(int index);
  int getEventUnlockedTotal();

  /* Returns the locked state struct */
  EditorLock* getLockedState();

  /* Returns a text list summary of the event set */
  QVector<QString> getTextSummary();

  /* Returns the unlocked access enum */
  core::AccessOperation getUnlockedAccess();

  /* Returns if the class is empty (default state after a clear() call) */
  bool isEmpty() const;

  /* Loads the event data */
  void load(core::XmlData data, int index);

  /* Saves the event data */
  void save(core::XmlWriter* writer, QString wrapper = "eventset", bool write_wrapper = true,
            bool skip_empty = true);

  /* Setters for locked event */
  bool setEventLocked(EditorEvent new_event, bool delete_event = true);

  /* Setters for unlocked event(s) */
  bool setEventUnlocked(int index, EditorEvent new_event, bool replace = false,
                        bool delete_event = true);

  /* Sets the locked status */
  bool setLocked(EditorLock new_locked);

  /* Sets the unlocked state */
  bool setUnlockedAccess(core::AccessOperation access);

  /* Shift unlocked events in list */
  int shiftUnlockedDown(int index);
  int shiftUnlockedUp(int index);

  /* Unset calls */
  bool unsetEventLocked(bool delete_event = true);
  bool unsetEventUnlocked(int index, bool delete_event = true);
  bool unsetEventUnlocked(bool delete_events = true);
  bool unsetLocked();

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  /* The copy operator */
  EditorEventSet& operator= (const EditorEventSet &source);
};

#endif // EDITOREVENTSET_H
