/*******************************************************************************
 * Class Name: EditorEvent
 * Date Created: February 10, 2015
 * Inheritance: none
 * Description: The class is for managing the interconnection to the game with
 *              Event in EventHandler and the viewing/edit widget used in 
 *              locations where it's used.
 ******************************************************************************/
#include "Database/EditorEvent.h"
#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
EditorEvent::EditorEvent()
{
  conversation = NULL;
  event = handler.createBlankEvent();
  //createLayout();
}

/* Constructor function - with input event */
EditorEvent::EditorEvent(Event event) : EditorEvent()
{
  this->event = event;
}

/* Destructor function */
EditorEvent::~EditorEvent()
{

}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* Creates the layout and widgets for this controller */
//void EditorEvent::createLayout()
//{
//
//}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Returns an individual conversation, based on the index */
Conversation* EditorEvent::getConversation()
{
  if(event.classification == EventClassifier::STARTCONVO)
    return event.convo;
  return NULL;
}

/* Returns an individual conversation, based on the index */
Conversation* getConversation(QString index)
{
  // TODO
}

/* Returns the event */
Event* EditorEvent::getEvent()
{
  return &event;
}

/* Returns the event type */
EventClassifier EditorEvent::getEventType()
{
  return event.classification;
}

/* Returns the event handler */
EventHandler* EditorEvent::getEventHandler()
{
  return &handler;
}

/* Returns data relating to the give item event. If none, data is invalid */
int EditorEvent::getGiveItemCount()
{
  if(event.classification == EventClassifier::GIVEITEM)
    return event.ints[EventHandler::kGIVE_ITEM_COUNT];
  return -1;
}

/* Returns data relating to the give item event. If none, data is invalid */
int EditorEvent::getGiveItemID()
{
  if(event.classification == EventClassifier::GIVEITEM)
    return event.ints[EventHandler::kGIVE_ITEM_ID];
  return -1;
}

/* Returns the string notification. If not that event, data is blank */
QString EditorEvent::getNotification()
{
  if(event.classification == EventClassifier::NOTIFICATION)
    return QString::fromStdString(event.strings.front());
  return "";
}

/* Returns the start map ID, for the related event */
int EditorEvent::getStartMapID()
{
  if(event.classification == EventClassifier::RUNMAP)
    return event.ints.front();
  return -1;
}

/* Returns data relating to the teleport event. If none, data is invalid */
int EditorEvent::getTeleportSection()
{
  if(event.classification == EventClassifier::TELEPORTTHING)
    return event.ints[EventHandler::kTELEPORT_SECTION];
  return -1;
}

/* Returns data relating to the teleport event. If none, data is invalid */
int EditorEvent::getTeleportThingID()
{
  if(event.classification == EventClassifier::TELEPORTTHING)
    return event.ints[EventHandler::kTELEPORT_ID];
  return -1;
}

/* Returns data relating to the teleport event. If none, data is invalid */
int EditorEvent::getTeleportX()
{
  if(event.classification == EventClassifier::TELEPORTTHING)
    return event.ints[EventHandler::kTELEPORT_X];
  return -1;
}

/* Returns data relating to the teleport event. If none, data is invalid */
int EditorEvent::getTeleportY()
{
  if(event.classification == EventClassifier::TELEPORTTHING)
    return event.ints[EventHandler::kTELEPORT_Y];
  return -1;
}

/* Insert conversations at index control points. Fails if invalid point */
bool EditorEvent::insertConversationAfter(QString index, Conversation convo)
{
  // TODO
}

/* Insert conversations at index control points. Fails if invalid point */
bool EditorEvent::insertConversationBefore(QString index, Conversation convo)
{
  // TODO
}

/* Sets the conversation at the index */
bool EditorEvent::setConversation(QString index, Conversation convo)
{

}

/* Sets the event to default / blank */
void EditorEvent::setEventBlank()
{
  event = handler.createBlankEvent();
}

/* Sets the conversation event */
bool EditorEvent::setEventConversation(Conversation* convo)
{
  /* Check the existing conversation */
  if(conversation != NULL)
    delete conversation;

  event = handler.createConversationEvent(convo);
  conversation = event.convo;
  return true;
}

/* Sets the event to give item */
bool EditorEvent::setEventGiveItem(int id, int count)
{
  if(id >= 0 && count > 0)
  {
    event = handler.createGiveItemEvent(id, count);
    return true;
  }
  return false;
}

/* Sets the event to notification text */
bool EditorEvent::setEventNotification(QString notification)
{
  if(!notification.isEmpty())
  {
    event = handler.createNotificationEvent(notification.toStdString());
    return true;
  }
  return false;
}

/* Sets the event to start battle */
bool EditorEvent::setEventStartBattle()
{
  event = handler.createStartBattleEvent();
  return true;
}

/* Sets the event to start map */
bool EditorEvent::setEventStartMap(int id)
{
  if(id >= 0)
  {
    event = handler.createStartMapEvent(id);
    return true;
  }
  return false;
}

/* Sets the event to teleport a thing */
bool EditorEvent::setEventTeleport(int thing_id, int section_id, int x, int y)
{
  if(thing_id >= 0 && section_id >= 0 && x >= 0 && y >= 0)
  {
    event = handler.createTeleportEvent(thing_id, x, y, section_id);
    return true;
  }
  return false;
}
