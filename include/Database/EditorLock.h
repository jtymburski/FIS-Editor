/*******************************************************************************
 * Class Name: EditorLock
 * Date Created: November 15, 2015
 * Inheritance: none
 * Description: The class is for managing the interconnection to the game with
 *              Lock struct in EventSet and the viewing/edit widget used in
 *              locations where it's used.
 ******************************************************************************/
#ifndef EDITORLOCK_H
#define EDITORLOCK_H

#include <QString>

#include "FileHandler.h"
#include "Game/EventSet.h"

class EditorLock
{
public:
  /* Constructor function */
  EditorLock();

  /* Constructor function - with input lock struct */
  EditorLock(Locked lock);

  /* Copy constructor */
  EditorLock(const EditorLock &source);

  /* Destructor function */
  ~EditorLock();

private:
  /* The locked struct to be edited by this class */
  Locked lock;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorLock &source);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns data relating to the have item locked struct. */
  bool getHaveItemConsume();
  int getHaveItemCount();
  int getHaveItemID();

  /* Returns the locked struct */
  Locked* getLock();

  /* Returns the lock type */
  LockedState getLockType();

  /* Returns a text list summary of the lock */
  QString getTextSummary(QString prefix = "Lock: ");

  /* Returns if the lock is set to be permanent */
  bool isPermanent();

  /* Loads the lock data */
  void load(XmlData data, int index);

  /* Saves the lock data */
  void save(FileHandler* fh, bool game_only = false, QString preface = "lock",
            bool no_preface = false);

  /* Sets the locked struct */
  void setLock(Locked lock);

  /* Set the lock to default / none */
  void setLockBlank();

  /* Sets the lock struct to a have item based lock */
  bool setLockHaveItem(int id = 0, int count = 1,
                       bool consume = true, bool permanent = true);

  /* Sets the lock struct to a trigger based lock */
  bool setLockTrigger(bool permanent = true);


/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  /* The copy operator */
  EditorLock& operator= (const EditorLock &source);

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/
public:
  /* Converts the conversation index to usable form */
  //static QString convertConversationIndex(QString index);
};

#endif // EDITORLOCK_H
