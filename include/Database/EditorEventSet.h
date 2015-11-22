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
#include "FileHandler.h"

class EditorEventSet
{
public:
  /* Constructor function */
  EditorEventSet();

  /* Constructor function - with input event */
  EditorEventSet(EventSet& set);

  /* Copy constructor */
  EditorEventSet(const EditorEventSet &source);

  /* Destructor function */
  ~EditorEventSet();

private:
  /* Event sets */
  EditorEvent event_locked;
  QVector<EditorEvent*> events_unlocked;
  //QVector<EditorEvent> events_unlocked;

  /* Locked status struct */
  EditorLock lock_data;

  /* State of unlocked events */
  UnlockedState unlocked_state;

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
  bool addEventUnlocked(Event new_event);
  bool addEventUnlocked(EditorEvent new_event);

  /* Clears all event data within the class */
  void clear(bool delete_data = true);

  /* Access getters for locked event */
  EditorEvent* getEventLocked();

  /* Returns the event set conversion of the Editor Event Set. This creates
   * copy of data for storage elsewhere (to be managed by caller) */
  EventSet getEventSet();

  /* Access getters for unlocked event(s) */
  QVector<EditorEvent*> getEventUnlocked();
  EditorEvent* getEventUnlocked(int index);
  int getEventUnlockedTotal();

  /* Returns the locked state struct */
  EditorLock* getLockedState();

  /* Returns the unlocked state enum */
  UnlockedState getUnlockedState();

  /* Returns if the class is empty (default state after a clear() call) */
  bool isEmpty();

  /* Loads the event data */
  void load(XmlData data, int index);

  /* Saves the event data */
  void save(FileHandler* fh, bool game_only = false,
            QString preface = "eventset", bool no_preface = false);

  /* Setters for locked event */
  bool setEventLocked(Event new_event, bool delete_event = true);
  bool setEventLocked(EditorEvent new_event, bool delete_event = true);

  /* Set for the event set */
  bool setEventSet(EventSet& set, bool delete_prev = true);

  /* Setters for unlocked event(s) */
  bool setEventUnlocked(int index, Event new_event, bool replace = false,
                        bool delete_event = true);
  bool setEventUnlocked(int index, EditorEvent new_event, bool replace = false,
                        bool delete_event = true);

  /* Sets the locked status */
  bool setLocked(Locked new_locked);
  bool setLocked(EditorLock new_locked);

  /* Sets the unlocked state */
  bool setUnlockedState(UnlockedState state);

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

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/
public:
  /* Converts the conversation index to usable form */
  //static QString convertConversationIndex(QString index);
};

#endif // EDITOREVENTSET_H
