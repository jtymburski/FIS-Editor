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

/*
 * Description: Blank constructor for editor event.
 *
 * Inputs: none
 */
EditorEvent::EditorEvent()
{
  this->event = nullptr;
}

/*
 * Description: Constructor with single event, as created by EventHandler. Assigns
 *              reference clean up of the allocated memory for the event.
 *
 * Inputs: Event* event - an init event
 */
EditorEvent::EditorEvent(core::Event* event) : EditorEvent()
{
  setEvent(event);
}

/*
 * Description: The copy constructor, with another editor event as the source
 *
 * Inputs: const EditorEvent &source - the source data
 */
EditorEvent::EditorEvent(const EditorEvent &source) : EditorEvent()
{
  copySelf(source);
}

/*
 * Description: Destructor function
 */
EditorEvent::~EditorEvent()
{
  setEventBlank();
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: Converts the compressed conversation index ("4.5.4")
 *              to the standard, expanded conversation index, as required by the
 *              {@link Conversation} class ("1.1.1.1.5.1.1.1.1")
 *
 * Inputs: QString index - the base simplified index
 * Output: ConversationEntryIndex - the entry index, used to access within the conversation tree
 */
core::ConversationEntryIndex EditorEvent::convertConversationIndex(QString index)
{
  QStringList index_split = index.split('.');
  QStringList expanded_index;

  for(int i = 0; i < index_split.size(); i++)
  {
    // Every odd index should be expanded into '1's.
    if(i % 2 == 0)
    {
      int one_count = index_split.at(i).toInt();
      for(int one_index = 0; one_index < one_count; one_index++)
        expanded_index.append("1");
    }
    // Every even index is an option and should be kept unchanged
    else
    {
      expanded_index.append(index_split.at(i));
    }
  }

  return core::ConversationEntryIndex(expanded_index.join(".").toStdString());
}

/*
 * Description: The copy function that is called by any copying methods in the
 *              class. Utilized by the copy constructor and the copy operator.
 *
 * Inputs: const EditorEvent &source - the reference editor event class
 * Output: none
 */
void EditorEvent::copySelf(const EditorEvent &source)
{
  event = EventSet::deleteEvent(event);
  event = EventSet::copyEvent(source.event);
  conversation = event.convo;
}

/*
 * Description: Returns if the configured event is an unlock base class.
 *
 * Inputs: none
 * Output: bool - TRUE if its unlock type event
 */
bool EditorEvent::isUnlockEvent() const
{
  core::EventType type = getEventType();
  return type == core::EventType::UNLOCKIO
     || type == core::EventType::UNLOCKTILE
     || type == core::EventType::UNLOCKTHING;
}

/*
 * Description: Updates the provided event (must be valid) with shared, top level properties.
 *
 * Inputs: core::Event* event - the valid event to update
 *         bool one_shot - true if the event will only trigger once
 *         bool sound_id - the connected sound ID. Default unset ref
 * Output: none
 */
void EditorEvent::updateEvent(core::Event* event, bool one_shot, int sound_id)
{
  if(getEventType() != core::EventType::NONE)
  {
    core::ExecutableEvent* executable_event = static_cast<core::ExecutableEvent*>(event);
    executable_event->setOneShot(one_shot);
    executable_event->setSoundId(sound_id);
  }
}

/*
 * Description: Updates the provided unlock event with the shared properties.
 *
 * Inputs: core::EventUnlock* event - ?
 *         bool view - should the unlock target be viewed by the player camera
 *         bool view_scroll - should the camera scroll to the target instead of fading
 *         int view_time - how long to view the point
 * Output: none
 */
void EditorEvent::updateUnlockEvent(core::EventUnlock* event, bool view, bool view_scroll,
                                    int view_time)
{
  event->setViewTarget(view);
  event->setViewScroll(view_scroll);
  event->setViewTimeMilliseconds(view_time);
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Deletes the conversation at the given reference index. All sub
 *              nodes under the parent node index are also deleted. Note this
 *              call will only execute if the current event is CONVO.
 *
 * Inputs: QString index - the index to the conversation node
 * Output: none
 */
void EditorEvent::deleteConversation(QString index)
{
  if(getEventType() == core::EventType::CONVERSATION)
  {
    core::EventConversation* conversation_event = static_cast<core::EventConversation*>(event);

    core::ConversationEntryIndex entry_index = convertConversationIndex(index);
    conversation_event->getConversation().deleteEntry(entry_index);
  }
}

/*
 * Description: Returns the conversation entry reference to the node of the index.
 *
 * Inputs: QString index - the node reference index
 * Output: ConversationEntry* - the returned conversation entry. NULL if not found or
 *                              not in conversation
 */
core::ConversationEntry* EditorEvent::getConversation(QString index)
{
  if(getEventType() == core::EventType::CONVERSATION)
  {
    core::EventConversation* conversation_event = static_cast<core::EventConversation*>(event);

    core::ConversationEntryIndex entry_index = convertConversationIndex(index);
    if(conversation_event->getConversation().hasEntry(entry_index))
      return &conversation_event->getConversation().getEntry(entry_index);
  }
  return nullptr;
}

/*
 * Description: Returns the base conversation pointer in the tree (head entry
 *              node). Note if the event is not a convo, NULL is returned.
 *
 * Inputs: none
 * Output: ConversationEntry* - the top conversation entry reference
 */
core::ConversationEntry* EditorEvent::getConversationStart()
{
  if(getEventType() == core::EventType::CONVERSATION)
    return &static_cast<core::EventConversation*>(event)->getConversation().getFirstEntry();
  return nullptr;
}

/*
 * Description: Returns the current event, set-up in this class.
 *
 * Inputs: none
 * Output: Event* - pointer to the event class
 */
core::Event* EditorEvent::getEvent()
{
  return event;
}

/*
 * Description: Returns the type of event handled by this class.
 *
 * Inputs: none
 * Output: EventClassification - event classification enum class
 */
core::EventType EditorEvent::getEventType() const
{
  return event != nullptr ? event->getType() : core::EventType::NONE;
}

/*
 * Description: Returns the give item event item chance. If it's not the
 *              give item event, -1 is returned.
 *
 * Inputs: none
 * Output: int - percent chance the give item will occur
 */
int EditorEvent::getGiveItemChance()
{
  if(getEventType() == core::EventType::ITEMGIVE)
    return static_cast<core::EventItemGive*>(event)->getChance();
  return -1;
}

/*
 * Description: Returns the give item event item count. If it's not the
 *              give item event, -1 is returned.
 *
 * Inputs: none
 * Output: int - number of items to be given in the event
 */
int EditorEvent::getGiveItemCount()
{
  if(getEventType() == core::EventType::ITEMGIVE)
    return static_cast<core::EventItemGive*>(event)->getItemCount();
  return -1;
}

/*
 * Description: Returns the give item event drop setting if there is no
 *              room. If it's not the give item event, false is returned.
 *
 * Inputs: none
 * Output: bool - should the item be dropped if there is no inventory room
 */
bool EditorEvent::getGiveItemDropIfNoRoom()
{
  if(getEventType() == core::EventType::ITEMGIVE)
    return static_cast<core::EventItemGive*>(event)->isDropIfNoRoom();
  return false;
}

/*
 * Description: Returns the give item event item ID. If it's not the give item
 *              event, -1 is returned.
 *
 * Inputs: none
 * Output: int - the ID of the item, to be given
 */
int EditorEvent::getGiveItemID()
{
  if(getEventType() == core::EventType::ITEMGIVE)
    return static_cast<core::EventItemGive*>(event)->getItemId();
  return -1;
}

/*
 * Description: Returns the event reference at the given index. If the event is
 *              not a multiple event or the index is out of range, returns null.
 *
 * Inputs: int index - the index in the event stack within the multiple set
 * Output: Event* - the event reference at the given index
 */
core::Event* EditorEvent::getMultipleEvent(int index)
{
  if(getEventType() == core::EventType::MULTIPLE)
  {
    core::EventMultiple* multiple_event = static_cast<core::EventMultiple*>(event);
    if(index >= 0 && index < getMultipleEventCount())
      return &multiple_event->getEvent(index);
  }
  return nullptr;
}

/*
 * Description: Returns the total number of events contained within the multiple event.
 *
 * Inputs: none
 * Output: int - event stack size
 */
int EditorEvent::getMultipleEventCount()
{
  if(getEventType() == core::EventType::MULTIPLE)
  {
    return static_cast<core::EventMultiple*>(event)->getEventCount();
  }
}

/*
 * Description: Returns the string notification, from the notification event.
 *              If the event is not the notification event, an empty string is
 *              returned.
 *
 * Inputs: none
 * Output: QString - the string notification.
 */
QString EditorEvent::getNotification()
{
  if(getEventType() == core::EventType::NOTIFICATION)
    return QString::fromStdString(static_cast<core::EventNotification*>(event)->getNotification());
  return "";
}

/*
 * Description: Returns if the thing's active state should be enabled or disabled.
 *
 * Inputs: none
 * Output: bool - true to make the thing active, false to make it inactive
 */
bool EditorEvent::getPropertyActive()
{
  if(getEventType() == core::EventType::PROPERTY)
    return static_cast<core::EventProperty*>(event)->isThingActive();
  return false;
}

/*
 * Description: Returns if the thing active state is modified in this event.
 *
 * Inputs: none
 * Output: bool - true if it was modified, false if unmodified
 */
bool EditorEvent::getPropertyActiveSet()
{
  if(getEventType() == core::EventType::PROPERTY)
    return static_cast<core::EventProperty*>(event)->isThingActiveSet();
  return false;
}

/*
 * Description: Returns if the person's map movement to enabled or disabled (stuck in place).
 *
 * Inputs: none
 * Output: bool - true to make the disable movement, false to allow normal movement
 */
bool EditorEvent::getPropertyDisableMove()
{
  if(getEventType() == core::EventType::PROPERTY)
    return static_cast<core::EventProperty*>(event)->isPersonMovementDisabled();
  return false;
}

/*
 * Description: Returns if person disabled movement is modified in this event.
 *
 * Inputs: none
 * Output: bool - true if it was modified, false if unmodified
 */
bool EditorEvent::getPropertyDisableMoveSet()
{
  if(getEventType() == core::EventType::PROPERTY)
    return static_cast<core::EventProperty*>(event)->isPersonMovementDisabledSet();
  return false;
}

/*
 * Description: Returns if a player is in range, the NPC will force an interaction.
 *
 * Inputs: none
 * Output: bool - true to make the NPC force interaction, false to require player interaction
 */
bool EditorEvent::getPropertyForceInteract()
{
  if(getEventType() == core::EventType::PROPERTY)
    return static_cast<core::EventProperty*>(event)->isNPCInteractionForced();
  return false;
}

/*
 * Description: Returns if NPC forced interaction is modified in this event.
 *
 * Inputs: none
 * Output: bool - true if it was modified, false if unmodified
 */
bool EditorEvent::getPropertyForceInteractSet()
{
  if(getEventType() == core::EventType::PROPERTY)
    return static_cast<core::EventProperty*>(event)->isNPCInteractionForcedSet();
  return false;
}

/*
 * Description: Returns the property modifier integer ID of the object to edit.
 *              If the event is not a property event, -1 is returned
 *
 * Inputs: none
 * Output: int - the returned property thing ID
 */
int EditorEvent::getPropertyID()
{
  if(getEventType() == core::EventType::PROPERTY)
    return static_cast<core::EventProperty*>(event)->getThingId();
  return core::EventProperty::kUNSET_THING_ID;
}

/*
 * Description: Returns the property modifier integer inactive time of the IO
 *              to edit. If the event is not a property event, -1 is returned
 *
 * Inputs: none
 * Output: int - the returned property IO inactive time in ms
 */
int EditorEvent::getPropertyInactive()
{
  if(getEventType() == core::EventType::PROPERTY)
  {
    core::EventProperty* event_property = static_cast<core::EventProperty*>(event);
    if(!event_property->isIOStateInactiveDisabled())
      return event_property->getIOStateInactiveMillis();
  }
  return -1;
}

/*
 * Description: Returns if interactive object inactive timeout property is modified in this event.
 *
 * Inputs: none
 * Output: bool - true if it was modified, false if unmodified
 */
bool EditorEvent::getPropertyInactiveSet()
{
  if(getEventType() == core::EventType::PROPERTY)
    return static_cast<core::EventProperty*>(event)->isIOStateInactiveSet();
  return false;
}

/*
 * Description: Returns if the person location should be reset by this event.
 *
 * Inputs: none
 * Output: bool - true to reset the location, false to do nothing
 */
bool EditorEvent::getPropertyPersonReset()
{
  if(getEventType() == core::EventType::PROPERTY)
    return static_cast<core::EventProperty*>(event)->isPersonLocationReset();
  return false;
}

/*
 * Description: Returns the property modifier integer active respawn time of
 *              the thing to edit. If the event is not a property event, -1 is
 *              returned
 *
 * Inputs: none
 * Output: int - the returned property thing active respawn time in ms
 */
int EditorEvent::getPropertyRespawn()
{
  if(getEventType() == core::EventType::PROPERTY)
  {
    core::EventProperty* event_property = static_cast<core::EventProperty*>(event);
    if(!event_property->isThingRespawnDisabled())
      return event_property->getThingRespawnMillis();
  }
  return -1;
}

/*
 * Description: Returns if the respawn time property is modified in this event.
 *
 * Inputs: none
 * Output: bool - true if it was modified, false if unmodified
 */
bool EditorEvent::getPropertyRespawnSet()
{
  if(getEventType() == core::EventType::PROPERTY)
    return static_cast<core::EventProperty*>(event)->isThingRespawnSet();
  return false;
}

/*
 * Description: Returns the property modifier integer speed of the person to
 *              edit. If the event is not a property event, -1 is returned
 *
 * Inputs: none
 * Output: int - the returned property person speed in (4096 / value) ms/tile
 */
int EditorEvent::getPropertySpeed()
{
  if(getEventType() == core::EventType::PROPERTY)
    return static_cast<core::EventProperty*>(event)->getPersonSpeed();
  return -1;
}

/*
 * Description: Returns if person map movement speed is modified in this event.
 *
 * Inputs: none
 * Output: bool - true if it was modified, false if unmodified
 */
bool EditorEvent::getPropertySpeedSet()
{
  if(getEventType() == core::EventType::PROPERTY)
    return static_cast<core::EventProperty*>(event)->isPersonSpeedSet();
  return false;
}

/*
 * Description: Returns the property modifier tracking state definition of the
 *              NPC to edit. If the event is not a property event, NONE is
 *              returned
 *
 * Inputs: none
 * Output: Tracking - the new tracking state property for the npc
 */
core::Tracking EditorEvent::getPropertyTrack()
{
  if(getEventType() == core::EventType::PROPERTY)
    return static_cast<core::EventProperty*>(event)->getNPCTracking();
  return core::Tracking::NONE;
}

/*
 * Description: Returns if NPC movement tracking is modified in this event.
 *
 * Inputs: none
 * Output: bool - true if it was modified, false if unmodified
 */
bool EditorEvent::getPropertyTrackSet()
{
  if(getEventType() == core::EventType::PROPERTY)
    return static_cast<core::EventProperty*>(event)->isNPCTrackingSet();
  return false;
}

/*
 * Description: Returns if the thing's rendering visibility should be enabled or disabled.
 *
 * Inputs: none
 * Output: bool - true if the thing should be visible and rendered, false to hide it
 */
bool EditorEvent::getPropertyVisible()
{
  if(getEventType() == core::EventType::PROPERTY)
    return static_cast<core::EventProperty*>(event)->isThingVisible();
  return false;
}

/*
 * Description: Returns if the thing visibile state is modified in this event.
 *
 * Inputs: none
 * Output: bool - true if it was modified, false if unmodified
 */
bool EditorEvent::getPropertyVisibleSet()
{
  if(getEventType() == core::EventType::PROPERTY)
    return static_cast<core::EventProperty*>(event)->isThingVisibleSet();
  return false;
}

/*
 * Description: Returns the connected sound ID from the event.
 *
 * Inputs: none
 * Output: int - the connected sound ID
 */
int EditorEvent::getSoundID()
{
  if(getEventType() != core::EventType::NONE)
    return static_cast<core::ExecutableEvent*>(event)->getSoundId();
  return core::ExecutableEvent::kUNSET_SOUND_ID;
}

/*
 * Description: Returns the start battle related lose event
 *
 * Inputs: none
 * Output: Event* - the returned event reference
 */
core::Event* EditorEvent::getStartBattleEventLose()
{
  if(getEventType() == core::EventType::BATTLESTART)
    return &static_cast<core::EventBattleStart*>(event)->getLoseEvent();
  return nullptr;
}

/*
 * Description: Returns the start battle related win event
 *
 * Inputs: none
 * Output; Event* - the returned event reference
 */
core::Event* EditorEvent::getStartBattleEventWin()
{
  if(getEventType() == core::EventType::BATTLESTART)
    return &static_cast<core::EventBattleStart*>(event)->getWinEvent();
  return nullptr;
}

/*
 * Description: Returns if on lose, the game should be over
 *
 * Inputs: none
 * Output; bool - true to end game on loss, false to go back to the map
 */
bool EditorEvent::getStartBattleGameOverOnLoss()
{
  if(getEventType() == core::EventType::BATTLESTART)
    return static_cast<core::EventBattleStart*>(event)->isGameOverOnLoss();
  return false;
}

/*
 * Description: Returns if on win, the initiating target should be removed from the map
 *
 * Inputs: none
 * Output; bool - true to hide initiating thing on win, false to leave it on the map
 */
bool EditorEvent::getStartBattleHideTargetOnWin()
{
  if(getEventType() == core::EventType::BATTLESTART)
    return static_cast<core::EventBattleStart*>(event)->isTargetHiddenOnWin();
  return false;
}

/*
 * Description: Returns if on battle end, the player health should be restored
 *
 * Inputs: none
 * Output; bool - true to restore health to full when returning from battle. false to leave as is
 */
bool EditorEvent::getStartBattleRestoreHealth()
{
  if(getEventType() == core::EventType::BATTLESTART)
    return static_cast<core::EventBattleStart*>(event)->isHealthRestored();
  return false;
}

/*
 * Description: Returns if on battle end, the player qd should be restored
 *
 * Inputs: none
 * Output; bool - true to restore QD to full when returning from battle. false to leave as is
 */
bool EditorEvent::getStartBattleRestoreQd()
{
  if(getEventType() == core::EventType::BATTLESTART)
    return static_cast<core::EventBattleStart*>(event)->isQdRestored();
  return false;
}

/*
 * Description: Returns the trigger map ID for the switching map event. If
 *              it's not the switch map event, -1 is returned.
 *
 * Inputs: none
 * Output: int - the ID of the map to switch to
 */
int EditorEvent::getStartMapID()
{
  if(getEventType() == core::EventType::MAPSWITCH)
    return static_cast<core::EventMapSwitch*>(event)->getMapId();
  return -1;
}

/*
 * Description: Returns the take item event item count. If it's not the
 *              take item event, -1 is returned.
 *
 * Inputs: none
 * Output: int - number of items to be taken in the event
 */
int EditorEvent::getTakeItemCount()
{
  if(getEventType() == core::EventType::ITEMTAKE)
    return static_cast<core::EventItemTake*>(event)->getItemCount();
  return -1;
}

/*
 * Description: Returns the take item event item ID. If it's not the take item
 *              event, -1 is returned.
 *
 * Inputs: none
 * Output: int - the ID of the item, to be taken
 */
int EditorEvent::getTakeItemID()
{
  if(getEventType() == core::EventType::ITEMTAKE)
    return static_cast<core::EventItemTake*>(event)->getItemId();
  return -1;
}

/*
 * Description: Returns the sub-map section that the teleport event will
 *              be sent to. If it's not the teleport thing event, -1 is
 *              returned.
 *
 * Inputs: none
 * Output: int - the sub-map section ID.
 */
int EditorEvent::getTeleportSection()
{
  if(getEventType() == core::EventType::TELEPORT)
    return static_cast<core::EventTeleport*>(event)->getSectionId();
  return -1;
}

/*
 * Description: Returns the thing ID that will be teleported in the event. If
 *              it's not the teleport event, -1 is returned.
 *
 * Inputs: none
 * Output: int - the thing ID to be teleported
 */
int EditorEvent::getTeleportThingID()
{
  if(getEventType() == core::EventType::TELEPORT)
    return static_cast<core::EventTeleport*>(event)->getThingId();
  return -1;
}

/*
 * Description: Returns the X location in the sub-map for the teleport event.
 *              If it's not the teleport event, -1 is returned.
 *
 * Inputs: none
 * Output: int - the X coordinate in the sub-map
 */
int EditorEvent::getTeleportX()
{
  if(getEventType() == core::EventType::TELEPORT)
    return static_cast<core::EventTeleport*>(event)->getTileHorizontal();
  return -1;
}

/*
 * Description: Returns the Y location in the sub-map for the teleport event.
 *              If it's not the teleport event, -1 is returned.
 *
 * Inputs: none
 * Output: int - the Y coordinate in the sub-map
 */
int EditorEvent::getTeleportY()
{
  if(getEventType() == core::EventType::TELEPORT)
    return static_cast<core::EventTeleport*>(event)->getTileVertical();
  return -1;
}

/*
 * Description: Returns the text based summary of what defines the Editor Event
 *
 * Inputs: QString prefix - the text to prepend. Default "Event: "
 * Output: QString - the returned string description
 */
QString EditorEvent::getTextSummary(QString prefix)
{
  return classToText(getEventType(), prefix, isOneShot());
}

/*
 * Description: Returns the target ID for the trigger IO event. Returns -2 if
 *              the event is not the unlock IO event
 *
 * Inputs: none
 * Output: int - the reference ID. -1 if self reference. -2 if invalid
 */
int EditorEvent::getTriggerIOID()
{
  if(getEventType() == core::EventType::TRIGGERIO)
    return static_cast<core::EventTriggerIO*>(event)->getInteractiveObjectId();
  return -2;
}

/*
 * Description: Returns if the IO unlock should target the enter event.
 *
 * Inputs: none
 * Output: bool - if true, it will try and unlock the enter event
 */
bool EditorEvent::getUnlockIOEventEnter()
{
  if(getEventType() == core::EventType::UNLOCKIO)
    return static_cast<core::EventUnlockIO*>(event)->isUnlockEventEnter();
  return false;
}

/*
 * Description: Returns if the IO unlock should target the exit event.
 *
 * Inputs: none
 * Output: bool - if true, it will try and unlock the exit event
 */
bool EditorEvent::getUnlockIOEventExit()
{
  if(getEventType() == core::EventType::UNLOCKIO)
    return static_cast<core::EventUnlockIO*>(event)->isUnlockEventExit();
  return false;
}

/*
 * Description: Returns if the IO unlock should target the use event.
 *
 * Inputs: none
 * Output: bool - if true, it will try and unlock the use event
 */
bool EditorEvent::getUnlockIOEventUse()
{
  if(getEventType() == core::EventType::UNLOCKIO)
    return static_cast<core::EventUnlockIO*>(event)->isUnlockEventUse();
  return false;
}

/*
 * Description: Returns if the IO unlock should target the walkover event.
 *
 * Inputs: none
 * Output: bool - if true, it will try and unlock the walkover event
 */
bool EditorEvent::getUnlockIOEventWalkover()
{
  if(getEventType() == core::EventType::UNLOCKIO)
    return static_cast<core::EventUnlockIO*>(event)->isUnlockEventWalkover();
  return false;
}

/*
 * Description: Returns the target ID for the unlock IO event. Returns -1 if
 *              the event is not the unlock IO event
 *
 * Inputs: none
 * Output: int - the reference ID
 */
int EditorEvent::getUnlockIOID()
{
  if(getEventType() == core::EventType::UNLOCKIO)
    return static_cast<core::EventUnlockIO*>(event)->getInteractiveObjectId();
  return -1;
}

/*
 * Description: Returns if the IO unlock should target the player interaction available.
 *
 * Inputs: none
 * Output: bool - if true, it will try and unlock the player interaction
 */
bool EditorEvent::getUnlockIOInteraction()
{
  if(getEventType() == core::EventType::UNLOCKIO)
    return static_cast<core::EventUnlockIO*>(event)->isUnlockInteraction();
  return false;
}

/*
 * Description: Returns the target state that are targetted within the IO if
 *              the event view mode is valid. Less than 0 targets all states
 *              in the IO.
 *
 * Inputs: none
 * Output: int - the state reference number
 */
int EditorEvent::getUnlockIOState()
{
  if(getEventType() == core::EventType::UNLOCKIO)
    return static_cast<core::EventUnlockIO*>(event)->getStateId();
  return -1;
}

/*
 * Description: Returns the reference thing ID that is targeted when the unlock
 *              event is triggered. If invalid, returns -1.
 *
 * Inputs: none
 * Output: int - the thing ID reference
 */
int EditorEvent::getUnlockThingID()
{
  if(getEventType() == core::EventType::UNLOCKTHING)
    return static_cast<core::EventUnlockThing*>(event)->getThingId();
  return -1;
}

/*
 * Description: Returns if the tile unlock should target the enter event.
 *
 * Inputs: none
 * Output: bool - if true, it will try and unlock the enter event
 */
bool EditorEvent::getUnlockTileEventEnter()
{
  if(getEventType() == core::EventType::UNLOCKTILE)
    return static_cast<core::EventUnlockTile*>(event)->isUnlockEventEnter();
  return false;
}

/*
 * Description: Returns if the tile unlock should target the exit event.
 *
 * Inputs: none
 * Output: bool - if true, it will try and unlock the exit event
 */
bool EditorEvent::getUnlockTileEventExit()
{
  if(getEventType() == core::EventType::UNLOCKTILE)
    return static_cast<core::EventUnlockTile*>(event)->isUnlockEventExit();
  return false;
}

/*
 * Description: Returns the tile section ID of where to locate the tile to
 *              unlock. If less than 0, it's the current map index where
 *              triggered.
 *
 * Inputs: none
 * Output: int - the section ID of the tile
 */
int EditorEvent::getUnlockTileSection()
{
  if(getEventType() == core::EventType::UNLOCKTILE)
    return static_cast<core::EventUnlockTile*>(event)->getSectionId();
  return -1;
}

/*
 * Description: Returns the tile X index of where to locate the tile to unlock.
 *              If less than 0, the event is not an unlock tile
 *
 * Inputs: none
 * Output: int - the X reference of the tile
 */
int EditorEvent::getUnlockTileX()
{
  if(getEventType() == core::EventType::UNLOCKTILE)
    return static_cast<core::EventUnlockTile*>(event)->getTileHorizontal();
  return -1;
}

/*
 * Description: Returns the tile Y index of where to locate the tile to unlock.
 *              If less than 0, the event is not an unlock tile
 *
 * Inputs: none
 * Output: int - the Y reference of the tile
 */
int EditorEvent::getUnlockTileY()
{
  if(getEventType() == core::EventType::UNLOCKTILE)
    return static_cast<core::EventUnlockTile*>(event)->getTileVertical();
  return -1;
}

/*
 * Description: Returns if the target should be viewed when it gets unlocked.
 *
 * Inputs: none
 * Output: bool - TRUE to view the unlock target. FALSE to unlock without showing the player
 */
bool EditorEvent::getUnlockView()
{
  if(isUnlockEvent())
    return static_cast<core::EventUnlock*>(event)->isViewTarget();
  return false;
}

/*
 * Description: Returns if the view should scroll when the unlock occurs. This depends on
 *              if the target will be viewed at all.
 *
 * Inputs: none
 * Output: bool - TRUE to scroll the viewport to the target location. FALSE to fade in / fade out
 */
bool EditorEvent::getUnlockViewScroll()
{
  if(isUnlockEvent())
    return static_cast<core::EventUnlock*>(event)->isViewScroll();
  return false;
}

/*
 * Description: Returns the view time and how long it views the point where the
 *              unlock occurs. Returns 0 if the event is not an unlock event
 *
 * Inputs: none
 * Output: int - the unlock view time for how the event is handled
 */
int EditorEvent::getUnlockViewTime()
{
  if(isUnlockEvent())
    return static_cast<core::EventUnlock*>(event)->getViewTimeMilliseconds();
  return 0;
}

/*
 * Description: Inserts a passed in conversation node at the index node address that is
 *              passed into the method. If there are tree gaps along the way, it creates
 *              blanks along the way.
 *
 * Inputs: QString index - the index reference node for where to insert
 *         ConversationEntry entry - the inserting conversation tree node
 * Output: bool - true if the conversation tree was modified and the entry was inserted
 */
bool EditorEvent::insertConversation(QString index, core::ConversationEntry& entry)
{
  if(getEventType() == core::EventType::CONVERSATION)
  {
    core::EventConversation* conversation_event = static_cast<core::EventConversation*>(event);

    core::ConversationEntryIndex entry_index = convertConversationIndex(index);
    conversation_event->getConversation().insertEntry(entry_index, entry);
    return true;
  }
  return false;
}

/*
 * Description: Returns if the event will only trigger once when initiated.
 *
 * Inputs: none
 * Output: bool - true if the event is one shot triggered
 */
bool EditorEvent::isOneShot()
{
  if(getEventType() != core::EventType::NONE)
    return static_cast<core::ExecutableEvent*>(event)->isOneShot();
  return false;
}

/*
 * Description: Loads the event data from the XML struct and offset index. Uses
 *              existing functions in game EventHandler class.
 *
 * Inputs: XmlData data - the XML data tree struct
 *         int index - the offset index into the struct
 * Output: none
 */
void EditorEvent::load(core::XmlData data, int index)
{
  event = PersistEvent::load(event, data, index);
}

/*
 * Description: Saves the event data to the file handling pointer.
 *
 * Inputs: FileHandler* fh - the file handling pointer
 *         QString wrapper - the wrapper text element. default to "event"
 *         bool write_wrapper - should the save wrap the event in a parent element? Default true
 * Output: none
 */
void EditorEvent::save(core::XmlWriter* writer, QString wrapper, bool write_wrapper)
{
  if(writer != nullptr && event != nullptr && event->isSaveable())
  {
    if(write_wrapper)
      writer->writeElement(wrapper.toStdString());

    PersistEvent::save(event, writer);

    if(write_wrapper)
      writer->jumpToParent();
  }
}

/*
 * Description: Sets the conversation for the passed in index, which replaces
 *              all the data in that node only. If the index doesn't exist,
 *              it creates it plus all the nodes on the way there.
 *
 * Inputs: QString index - the reference conversation node to change info
 *         Conversation convo - new node information to place in index
 * Output: bool - true if the conversation tree was modified and the entry was set
 */
bool EditorEvent::setConversation(QString index, core::ConversationEntry& entry)
{
  if(getEventType() == core::EventType::CONVERSATION)
  {
    core::EventConversation* conversation_event = static_cast<core::EventConversation*>(event);

    core::ConversationEntryIndex entry_index = convertConversationIndex(index);
    conversation_event->getConversation().setEntry(entry_index, entry);
    return true;
  }
  return false;
}

/*
 * Description: Sets the event to the passed in event struct. Assigns reference clean up of
 *              the allocated memory for the event.
 *
 * Inputs: Event* event - the event to load in
 * Output: none
 */
void EditorEvent::setEvent(core::Event* event)
{
  /* Remove old event */
  setEventBlank();

  /* Store new event */
  this->event = event;
}

/*
 * Description: Sets the event in the class to the blank unused event.
 *
 * Inputs: bool delete_event - delete existing event. default true
 * Output: none
 */
void EditorEvent::setEventBlank(bool delete_event)
{
  if(delete_event)
    delete event;
  event = nullptr;
}

/*
 * Description: Sets the event in the class to the conversation.
 *
 * Inputs: int sound_id - the connected sound ID. Default unset ref
 * Output: bool - true if event changed to conversation
 */
bool EditorEvent::setEventConversation(int sound_id)
{
  core::EventConversation* conversation_event = new core::EventConversation();
  updateEvent(conversation_event, isOneShot(), sound_id);

  setEvent(conversation_event);
  return true;
}

/*
 * Description: Sets the event in the class to the give item event, with a
 *              passed in ID of the item and count (ID needs to be greater
 *              than or equal to 0 and count greater than 0).
 *
 * Inputs: int id - the id of the item to give (game side)
 *         int count - the number of items to give on trigger
 *         bool drop_if_no_room - true to drop items if there is no inventory room
 *         int sound_id - the connected sound ID. Default unset ref
 * Output: bool - true if the event was created
 */
bool EditorEvent::setEventGiveItem(int id, int count, bool drop_if_no_room,
                                   int chance, int sound_id)
{
  if(id >= 0 && count > 0 && chance >= 0)
  {
    core::EventItemGive* give_item_event = new core::EventItemGive();
    give_item_event->setItemId(id);
    give_item_event->setItemCount(count);
    give_item_event->setDropIfNoRoom(drop_if_no_room);
    updateEvent(give_item_event, isOneShot(), sound_id);

    setEvent(give_item_event);
    return true;
  }
  return false;
}

/*
 * Description: Sets the event in the class to the multiple trigger event and
 *              modifies the selected index event to the new event. If the index
 *              does not exist, create blanks up to size and set the new index
 *
 * Inputs: int index - the index of the new event to modify in the stack
 *         Event new_event - the new event to load at said index
 * Output: bool - true if the multiple event was modified
 */
bool EditorEvent::setEventInMultiple(int index, core::Event& new_event)
{
  if(getEventType() == core::EventType::MULTIPLE && index >= 0)
  {
    static_cast<core::EventMultiple*>(event)->setEvent(index, new_event);
    return true;
  }
  return false;
}

/*
 * Description: Sets the event in the class to the multiple trigger event,
 *              with the passed in set of events.
 *
 * Inputs: int sound_id - the connected sound ID. Default unset ref
 * Output: bool - true if the multiple event was created
 */
bool EditorEvent::setEventMultiple(int sound_id)
{
  core::EventMultiple* multiple_event = new core::EventMultiple();
  updateEvent(multiple_event, isOneShot(), sound_id);

  setEvent(multiple_event);
  return true;
}

/*
 * Description: Sets the event in the class to the notification string event,
 *              with a passed in notification string (not empty).
 *
 * Inputs: QString notification - the notification string on trigger
 *         int sound_id - the connected sound ID. Default unset ref
 * Output: bool - true if the notification event was created
 */
bool EditorEvent::setEventNotification(QString notification, int sound_id)
{
  if(!notification.isEmpty())
  {
    core::EventNotification* notification_event = new core::EventNotification();
    notification_event->setNotification(notification.toStdString());
    updateEvent(notification_event, isOneShot(), sound_id);

    setEvent(notification_event);
    return true;
  }
  return false;
}

/*
 * Description: Sets the event in the class to the property modifier event,
 *              with a passed in reference thing ID.
 *
 * Inputs: int thing_id - the thing ID to be modified
 *         int sound_id - the connected sound ID. Default unset ref
 * Output: bool - true if the property event was created
 */
bool EditorEvent::setEventProp(int thing_id, int sound_id)
{
  if(thing_id >= 0)
  {
    core::EventProperty* property_event;
    if(getEventType() != core::EventType::PROPERTY)
    {
      property_event = new core::EventProperty();
      updateEvent(property_event, isOneShot(), sound_id);
      setEvent(property_event);
    }
    else
    {
      property_event = static_cast<core::EventProperty*>(event);
      property_event->setSoundId(sound_id);
    }

    property_event->setThingId(thing_id);
    return true;
  }
  return false;
}

/*
 * Description: Sets the property modifier event thing's active state to either
 *              enabled or disabled.
 *
 * Inputs: bool modified - true if the event should modify this property. false to ignore it
 *         bool active - true if the thing should be active, false to make it inactive
 * Output: bool - true if the property event was modified
 */
bool EditorEvent::setEventPropActive(bool modified, bool active)
{
  if(getEventType() == core::EventType::PROPERTY)
  {
    core::EventProperty* property_event = static_cast<core::EventProperty*>(event);
    if(modified)
      property_event->setThingActive(active);
    else
      property_event->resetThingActive();
    return true;
  }
  return false;
}

/*
 * Description: Sets the property modifier event person's map movement to
 *              enabled or disabled (stuck in place).
 *
 * Inputs: bool modified - true if the event should modify this property. false to ignore it
 *         bool disable_move - true to lock the person in place, false to keep it free
 * Output: bool - true if the property event was modified
 */
bool EditorEvent::setEventPropDisableMove(bool modified, bool disable_move)
{
  if(getEventType() == core::EventType::PROPERTY)
  {
    core::EventProperty* property_event = static_cast<core::EventProperty*>(event);
    if(modified)
      property_event->setPersonMovementDisabled(disable_move);
    else
      property_event->resetPersonMovementDisabled();
    return true;
  }
  return false;
}

/*
 * Description: Sets the property modifier event to modify if the player is in range,
 *              the NPC will force an interaction.
 *
 * Inputs: bool modified - true if the event should modify this property. false to ignore it
 *         bool force_interact - true to make the NPC force interaction, false to require the
 *                               player to interact
 * Output: bool - true if the property event was modified
 */
bool EditorEvent::setEventPropForceInteract(bool modified, bool force_interact)
{
  if(getEventType() == core::EventType::PROPERTY)
  {
    core::EventProperty* property_event = static_cast<core::EventProperty*>(event);
    if(modified)
      property_event->setNPCInteractionForced(force_interact);
    else
      property_event->resetNPCInteractionForced();
    return true;
  }
  return false;
}

/*
 * Description: Sets the property modifier event to modify the time to return to the
 *              previous state in the interactive object if inactive.
 *
 * Inputs: bool modified - true if the event should modify this property. false to ignore it
 *         int inactive_time - inactive timeout in milliseconds
 * Output: bool - true if the property event was modified
 */
bool EditorEvent::setEventPropInactive(bool modified, int inactive_time)
{
  if(getEventType() == core::EventType::PROPERTY)
  {
    core::EventProperty* property_event = static_cast<core::EventProperty*>(event);
    if(modified)
    {
      if(inactive_time < 0)
        property_event->setIOStateInactiveDisabled();
      else
        property_event->setIOStateInactiveMillis(inactive_time);
    }
    else
    {
      property_event->resetIOStateInactive();
    }
    return true;
  }
  return false;
}

/*
 * Description: Sets the property modifier event to modify if the person's location should
 *              be set to their starting map location.
 *
 * Inputs: bool reset - true if the location should be reset. false to make no change
 * Output: bool - true if the property event was modified
 */
bool EditorEvent::setEventPropPersonReset(bool reset)
{
  if(getEventType() == core::EventType::PROPERTY)
  {
    core::EventProperty* property_event = static_cast<core::EventProperty*>(event);
    if(reset)
      property_event->setPersonLocationReset();
    else
      property_event->resetPersonLocationReset();
    return true;
  }
  return false;
}

/*
 * Description: Sets the property modifier event to modify the time for the thing to
 *              respawn after visibility is disabled.
 *
 * Inputs: bool modified - true if the event should modify this property. false to ignore it
 *         int respawn_time - respawn time delay in milliseconds. <0 if disabled
 * Output: bool - true if the property event was modified
 */
bool EditorEvent::setEventPropRespawn(bool modified, int respawn_time)
{
  if(getEventType() == core::EventType::PROPERTY)
  {
    core::EventProperty* property_event = static_cast<core::EventProperty*>(event);
    if(modified)
    {
      if(respawn_time < 0)
        property_event->setThingRespawnDisabled();
      else
        property_event->setThingRespawnMillis(respawn_time);
    }
    else
    {
      property_event->resetThingRespawn();
    }
    return true;
  }
  return false;
}

/*
 * Description: Sets the property modifier event to modify the person map movement speed.
 *
 * Inputs: bool modified - true if the event should modify this property. false to ignore it
 *         int speed - configured person speed, in units (tiles per second) = (speed / 4)
 * Output: bool - true if the property event was modified
 */
bool EditorEvent::setEventPropSpeed(bool modified, int speed)
{
  if(getEventType() == core::EventType::PROPERTY && speed >= 0)
  {
    core::EventProperty* property_event = static_cast<core::EventProperty*>(event);
    if(modified)
      property_event->setPersonSpeed(speed);
    else
      property_event->resetPersonSpeed();
    return true;
  }
  return false;
}

/*
 * Description: Sets the property modifier event to modify the NPC movement tracking for
 *              when the player is in range.
 *
 * Inputs: bool modified - true if the event should modify this property. false to ignore it
 *         Tracking track_state - NPC tracking mode
 * Output: bool - true if the property event was modified
 */
bool EditorEvent::setEventPropTrack(bool modified, core::Tracking track_state)
{
  if(getEventType() == core::EventType::PROPERTY)
  {
    core::EventProperty* property_event = static_cast<core::EventProperty*>(event);
    if(modified)
      property_event->setNPCTracking(track_state);
    else
      property_event->resetNPCTracking();
    return true;
  }
  return false;
}

/*
 * Description: Sets the property modifier event thing's visible state to either
 *              visible (rendered) or hidden.
 *
 * Inputs: bool modified - true if the event should modify this property. false to ignore it
 *         bool active - true if the thing should be visible, false to make it invisible
 * Output: bool - true if the property event was modified
 */
bool EditorEvent::setEventPropVisible(bool modified, bool visible)
{
  if(getEventType() == core::EventType::PROPERTY)
  {
    core::EventProperty* property_event = static_cast<core::EventProperty*>(event);
    if(modified)
      property_event->setThingVisible(visible);
    else
      property_event->resetThingVisible();
    return true;
  }
  return false;
}

/*
 * Description: Sets the event in the class to the just sound trigger event,
 *              with a passed in connected sound ID.
 *
 * Inputs: int sound_id - the connected sound ID
 * Output: bool - true if the event was created
 */
bool EditorEvent::setEventSound(int sound_id)
{
  core::EventSound* sound_event = new core::EventSound();
  updateEvent(sound_event, isOneShot(), sound_id);

  setEvent(sound_event);
  return true;
}

/*
 * Description: Sets the event in the class to the start battle event. No
 *              input necessary since it's triggered from the source and trigger
 *              things.
 *
 * Inputs: int sound_id - the connected sound ID. Default unset ref
 * Output: bool - true if the battle event was created
 */
bool EditorEvent::setEventStartBattle(int sound_id)
{
  core::EventBattleStart* battle_start_event = new core::EventBattleStart();
  updateEvent(battle_start_event, isOneShot(), sound_id);

  setEvent(battle_start_event);
  return true;
}

/*
 * Description: Sets the event in the class to the start battle event. This is
 *              the additional properties call with events and flags.
 *
 * Inputs: bool game_over_on_loss - true to end the game if the player loses the battle
 *         bool hide_target_on_win - true to hide initiating thing on battle win
 *         bool restore_health - true to restore player health on battle end
 *         bool restore_qd - true to restore player QD on battle end
 *         Event* event_win - the event triggered upon battle win
 *         Event* event_lose - the event triggered upon battle lose
 *         int sound_id - the connected sound ID. Default unset ref
 * Output: bool - true if the battle event was created
 */
bool EditorEvent::setEventStartBattle(bool game_over_on_loss, bool hide_target_on_win,
                                      bool restore_health, bool restore_qd,
                                      core::Event& event_win, core::Event& event_lose,
                                      int sound_id)
{
  core::EventBattleStart* battle_start_event = new core::EventBattleStart();
  battle_start_event->setGameOverOnLoss(game_over_on_loss);
  battle_start_event->setHealthRestored(restore_health);
  battle_start_event->setQdRestored(restore_qd);
  battle_start_event->setTargetHiddenOnWin(hide_target_on_win);
  battle_start_event->setWinEvent(event_win);
  battle_start_event->setLoseEvent(event_lose);
  battle_start_event->setOneShot(isOneShot());
  battle_start_event->setSoundId(sound_id);
  updateEvent(battle_start_event, isOneShot(), sound_id);

  setEvent(battle_start_event);
  return true;
}

/*
 * Description: Sets the event in the class to the start map event, using the
 *              map ID. This starts the new map with the player in the new
 *              location.
 *
 * Inputs: int id - the id of the map to trigger start
 *         int sound_id - the connected sound ID. Default unset ref
 * Output: bool - true if the event was created
 */
bool EditorEvent::setEventStartMap(int id, int sound_id)
{
  if(id >= 0)
  {
    core::EventMapSwitch* map_switch_event = new core::EventMapSwitch();
    map_switch_event->setMapId(id);
    updateEvent(map_switch_event, isOneShot(), sound_id);

    setEvent(map_switch_event);
    return true;
  }
  return false;
}

/*
 * Description: Sets the event in the class to the take item event, with a
 *              passed in ID of the item and count (ID needs to be greater
 *              than or equal to 0 and count greater than 0).
 *
 * Inputs: int id - the id of the item to take (game side)
 *         int count - the number of items to take on trigger
 *         int sound_id - the connected sound ID. Default unset ref
 * Output: bool - true if the event was created
 */
bool EditorEvent::setEventTakeItem(int id, int count, int sound_id)
{
  if(id >= 0 && count > 0)
  {
    core::EventItemTake* take_item_event = new core::EventItemTake();
    take_item_event->setItemId(id);
    take_item_event->setItemCount(count);
    updateEvent(take_item_event, isOneShot(), sound_id);

    setEvent(take_item_event);
    return true;
  }
  return false;
}

/*
 * Description: Sets the event in the class to the teleport thing event, using
 *              a map location (id, x, y) and a thing id.
 *
 * Inputs: int thing_id - the thing id to be teleported (0 is player)
 *         int section_id - the sub-map ID to teleport to
 *         int x - the x tile location in the sub-map
 *         int y - the y tile location in the sub-map
 *         int sound_id - the connected sound ID. Default unset ref
 * Output: bool - true if the event was created
 */
bool EditorEvent::setEventTeleport(int thing_id, int section_id, int x, int y,
                                   int sound_id)
{
  if(thing_id >= 0 && x >= 0 && y >= 0)
  {
    core::EventTeleport* teleport_event = new core::EventTeleport();
    teleport_event->setThingId(thing_id);
    teleport_event->setSectionId(section_id);
    teleport_event->setTileHorizontal(x);
    teleport_event->setTileVertical(y);
    updateEvent(teleport_event, isOneShot(), sound_id);

    setEvent(teleport_event);
    return true;
  }
  return false;
}

/*
 * Description: Sets the event in the class to the trigger IO event: using the
 *              IO ID and optional sound ID.
 *
 * Inputs: int io_id - the IO reference ID
 *         int sound_id - the connected sound ID. Default unset ref
 * Output: bool - true if the event was created
 */
bool EditorEvent::setEventTriggerIO(int io_id, int sound_id)
{
  core::EventTriggerIO* trigger_io_event = new core::EventTriggerIO();
  trigger_io_event->setInteractiveObjectId(io_id);
  updateEvent(trigger_io_event, isOneShot(), sound_id);

  setEvent(trigger_io_event);
  return true;
}

/*
 * Description: Sets the event in the class to the unlock IO event, using the
 *              IO ID, mode, state reference, event mode, view mode, and view
 *              time.
 *
 * Inputs: int io_id - the IO reference ID
 *         bool unlock_enter - if true, it will try and unlock the enter event
 *         bool unlock_exit - if true, it will try and unlock the exit event
 *         bool unlock_use - if true, it will try and unlock the use event
 *         bool unlock_walkover - if true, it will try and unlock the walkover event
 *         int state_num - if state mode, the state connected num. -1 if all
 *         bool unlock_interaction - if true, it will try and unlock the player interaction
 *         bool view - should the unlock target be viewed by the player camera
 *         bool view_scroll - should the camera scroll to the target instead of fading
 *         int view_time - how long to view the point
 *         int sound_id - the connected sound ID. Default unset ref
 * Output: bool - true if the event was created
 */
bool EditorEvent::setEventUnlockIO(int io_id, bool unlock_enter, bool unlock_exit,
                                   bool unlock_use, bool unlock_walkover, int state_num,
                                   bool unlock_interaction, bool view, bool view_scroll,
                                   int view_time, int sound_id)
{
  if(view_time >= 0)
  {
    core::EventUnlockIO* unlock_io_event = new core::EventUnlockIO();
    unlock_io_event->setInteractiveObjectId(io_id);
    unlock_io_event->setUnlockEventEnter(unlock_enter);
    unlock_io_event->setUnlockEventExit(unlock_exit);
    unlock_io_event->setUnlockEventUse(unlock_use);
    unlock_io_event->setUnlockEventWalkover(unlock_walkover);
    unlock_io_event->setUnlockInteraction(unlock_interaction);
    unlock_io_event->setStateId(state_num);
    updateUnlockEvent(unlock_io_event, view, view_scroll, view_time);
    updateEvent(unlock_io_event, isOneShot(), sound_id);

    setEvent(unlock_io_event);
    return true;
  }
  return false;
}

/*
 * Description: Sets the event in the class to the unlock thing event, using the
 *              thing ID, view mode, and view time.
 *
 * Inputs: int thing_id - the thing reference ID
 *         bool view - should the unlock target be viewed by the player camera
 *         bool view_scroll - should the camera scroll to the target instead of fading
 *         int view_time - how long to view the point
 *         int sound_id - the connected sound ID. Default unset ref
 * Output: bool - true if the event was created
 */
bool EditorEvent::setEventUnlockThing(int thing_id, bool view, bool view_scroll, int view_time,
                                      int sound_id)
{
  if(view_time >= 0)
  {
    core::EventUnlockThing* unlock_thing_event = new core::EventUnlockThing();
    unlock_thing_event->setThingId(thing_id);
    updateUnlockEvent(unlock_thing_event, view, view_scroll, view_time);
    updateEvent(unlock_thing_event, isOneShot(), sound_id);

    setEvent(unlock_thing_event);
    return true;
  }
  return false;
}

/*
 * Description: Sets the event in the class to the unlock tile event, using the
 *              tile section ID, tile X, tile Y, tile mode, view mode, and view
 *              time.
 *
 * Inputs: int section_id - the section tile reference
 *         uint16_t tile_x - the tile x reference
 *         uint16_t tile_y - the tile y reference
 *         bool unlock_enter - if true, it will try and unlock the enter event
 *         bool unlock_exit - if true, it will try and unlock the exit event
 *         bool view - should the unlock target be viewed by the player camera
 *         bool view_scroll - should the camera scroll to the target instead of fading
 *         int view_time - how long to view the point
 *         int sound_id - the connected sound ID. Default unset ref
 * Output: bool - true if the event was created
 */
bool EditorEvent::setEventUnlockTile(int section_id, uint16_t tile_x, uint16_t tile_y,
                                     bool unlock_enter, bool unlock_exit, bool view,
                                     bool view_scroll, int view_time, int sound_id)
{
  if(view_time >= 0)
  {
    core::EventUnlockTile* unlock_tile_event = new core::EventUnlockTile();
    unlock_tile_event->setSectionId(section_id);
    unlock_tile_event->setTileHorizontal(tile_x);
    unlock_tile_event->setTileVertical(tile_y);
    unlock_tile_event->setUnlockEventEnter(unlock_enter);
    unlock_tile_event->setUnlockEventExit(unlock_exit);
    updateUnlockEvent(unlock_tile_event, view, view_scroll, view_time);
    updateEvent(unlock_tile_event, isOneShot(), sound_id);

    setEvent(unlock_tile_event);
    return true;
  }
  return false;
}

/*
 * Description: Sets if the event will only trigger once.
 *
 * Inputs: bool one_shot - true if the event will only trigger once
 * Output: none
 */
void EditorEvent::setOneShot(bool one_shot)
{
  if(getEventType() != core::EventType::NONE)
    static_cast<core::ExecutableEvent*>(event)->setOneShot(one_shot);
}

/*
 * Description: Sets the sound ID for the event; only if the event has already
 *              been set-up as a valid event (not NOEVENT)
 *
 * Inputs: int id - the new event ID
 * Output: none
 */
void EditorEvent::setSoundID(int id)
{
  if(getEventType() != core::EventType::NONE)
  {
    core::ExecutableEvent* executable_event = static_cast<core::ExecutableEvent*>(event);
    if(id >= 0)
      executable_event->setSoundId(id);
    else
      executable_event->setSoundId(core::ExecutableEvent::kUNSET_SOUND_ID);
  }
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/*
 * Description: Copy operator construction. This is called when the variable
 *              already exists and equal operator used with another editor
 *              event.
 *
 * Inputs: const EditorEvent &source - the source class constructor
 * Output: EditorEvent& - pointer to the copied class
 */
EditorEvent& EditorEvent::operator= (const EditorEvent &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Returns the text summary for the given classification and
 *              additional information as indicated by inputs.
 *
 * Inputs: EventClassifier classification - the classification of the event
 *         QString prefix - prefix text. Default "Event: "
 *         bool one_shot - true if the event is a one shot
 * Output: QString - the summary text result
 */
QString EditorEvent::classToText(EventClassifier classification, QString prefix,
                                 bool one_shot)
{
  QString content = QString::fromStdString(
                                 EventSet::classifierToStr(classification));
  QString suffix = "";
  if(one_shot)
    suffix += " (once)";
  return (prefix + content + suffix);
}

/*
 * Description: Checks if the passed in base index (4.5.4 format) and how many
 *              children the item has and returns if it could become an option.
 *
 * Inputs: QString base_index - the base index (non-simplified form)
 *         int child_count - the number of children the index has
 * Output: bool - true if it could be an option
 */
bool EditorEvent::couldBeOption(QString base_index, int child_count)
{
  if(!base_index.isEmpty())
  {
    /* Only valid if not an option */
    QStringList list = base_index.split('.');
    if(list.size() % 2 == 1)
    {
      /* Only valid if not the first in the stack */
      if(list.last() != "1" && child_count <= 1)
        return true;
    }
  }

  return false;
}
