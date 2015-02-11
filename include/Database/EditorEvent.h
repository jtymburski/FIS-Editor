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

#include <QString>
#include <QVector>

#include "Game/EventHandler.h"

class EditorEvent
{
public:
  /* Constructor function */
  EditorEvent();

  /* Constructor function - with input event */
  EditorEvent(Event event);

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
  /* Creates the layout and widgets for this controller */
  //void createLayout();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns an individual conversation, based on the index */
  Conversation* getConversation();
  Conversation* getConversation(QString index);

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
  bool insertConversationAfter(QString index, Conversation convo);
  bool insertConversationBefore(QString index, Conversation convo);

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
};

#endif // EDITOREVENT_H
