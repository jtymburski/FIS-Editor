/*******************************************************************************
 * Class Name: EditorEvent
 * Date Created: February 10, 2015
 * Inheritance: none
 * Description: The class is for managing the interconnection to the game with
 *              Event in EventHandler and the viewing/edit widget used in 
 *              locations where it's used.
 ******************************************************************************/
#ifndef EDITOREVENT_H
#define EDITOREVENT_H

#include <QStringList>
#include <QVector>

#include "Game/EventHandler.h"

class EditorEvent
{
public:
  /* Constructor function */
  EditorEvent();

  /* Constructor function - with input event */
  EditorEvent(Event event);

  /* Copy constructor */
  EditorEvent(const EditorEvent &source);

  /* Destructor function */
  ~EditorEvent();

private:
  /* The conversation stack */
  Conversation* conversation;

  /* The event to be edited by this class */
  Event event;
  
  /* The event handler and controller */
  EventHandler handler;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Follows the path to the conversation and generates it if necessary.
   * Multi-function to act as get and create */
  Conversation* convoManipulator(QString index, bool generate = false,
                                 bool before = false, bool break_tree = false);

  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorEvent &source);

  /* A recursive parser through conversation set to find equivalent address */
  QString recursiveConversationFind(Conversation* ref, Conversation* convo,
                                    QString index = "1");

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Deletes conversation element */
  bool deleteConversation(QString index);

  /* Returns an individual conversation, based on the index */
  Conversation* getConversation();
  Conversation* getConversation(QString index, bool before = false);

  /* Returns the conversation index */
  QString getConversationIndex(Conversation* convo);

  /* Returns the event */
  Event* getEvent();

  /* Returns the event type */
  EventClassifier getEventType();

  /* Returns the event handler */
  EventHandler* getEventHandler();

  /* Returns data relating to the give item event. If none, data is invalid */
  int getGiveItemCount();
  int getGiveItemID();

  /* Returns the string notification. If not that event, data is blank */
  QString getNotification();

  /* Returns the start map ID, for the related event */
  int getStartMapID();

  /* Returns data relating to the teleport event. If none, data is invalid */
  int getTeleportSection();
  int getTeleportThingID();
  int getTeleportX();
  int getTeleportY();

  /* Insert conversations at index control points. Fails if invalid point */
  bool insertConversationAfter(QString index, Conversation convo,
                               bool option_node = false);
  bool insertConversationBefore(QString index, Conversation convo,
                                bool option_node = false);

  /* Sets the conversation at the index */
  bool setConversation(QString index, Conversation convo);

  /* Sets the event to default / blank */
  void setEventBlank();

  /* Sets the conversation event */
  bool setEventConversation(Conversation* convo = NULL);

  /* Sets the event to give item */
  bool setEventGiveItem(int id, int count);

  /* Sets the event to notification text */
  bool setEventNotification(QString notification);

  /* Sets the event to start battle */
  bool setEventStartBattle();

  /* Sets the event to start map */
  bool setEventStartMap(int id);

  /* Sets the event to teleport a thing */
  bool setEventTeleport(int thing_id, int section_id, int x, int y);

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  /* The copy operator */
  EditorEvent& operator= (const EditorEvent &source);

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/
public:
  /* Copies the past in event */
  static Event copyEvent(Event source);

  /* Creates conversations used for insertion */
  static Conversation createConversation(QString text, int id, Event event);
  static Conversation createConversation(QString text, int id, Event event,
                                        QVector<QPair<QString, Event>> options);

  /* Prints the conversation */
  static void printConversation(Conversation* convo, QString index = "1");
};

#endif // EDITOREVENT_H
