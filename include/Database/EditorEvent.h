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

#include <QPair>
#include <QStringList>
#include <QVector>

#include "Event/ConversationEntry.h"
#include "Event/ConversationEntryIndex.h"
#include "Event/Event.h"
#include "Event/EventBattleStart.h"
#include "Event/EventConversation.h"
#include "Event/EventItemGive.h"
#include "Event/EventItemTake.h"
#include "Event/EventMapSwitch.h"
#include "Event/EventMultiple.h"
#include "Event/EventNotification.h"
#include "Event/EventProperty.h"
#include "Event/EventSound.h"
#include "Event/EventTeleport.h"
#include "Event/EventTriggerIO.h"
#include "Event/EventType.h"
#include "Event/EventUnlockIO.h"
#include "Event/EventUnlockThing.h"
#include "Event/EventUnlockTile.h"
#include "Map/Tracking.h"

class EditorEvent
{
public:
  /* Constructor function */
  EditorEvent();

  /* Constructor function - with input event */
  EditorEvent(core::Event* event);

  /* Copy constructor */
  EditorEvent(const EditorEvent &source);

  /* Destructor function */
  ~EditorEvent();

private:
  /* The event to be edited by this class */
  core::Event* event;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Converts the conversation index to usable form */
  core::ConversationEntryIndex convertConversationIndex(QString index);

  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorEvent &source);

  /* Returns if the configured event is an unlock base class */
  bool isUnlockEvent() const;

  /* Saves the conversation - recursive call */
  void saveConversation(FileHandler* fh, Conversation* convo = NULL,
                        QString index = "1");

  /* Updates the provided event with the shared, top level properties */
  void updateEvent(core::Event* event, bool one_shot, int sound_id);

  /* Updates the provided unlock event with the shared properties */
  void updateUnlockEvent(core::EventUnlock* event, bool view, bool view_scroll, int view_time);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Deletes conversation element */
  void deleteConversation(QString index);

  /* Returns an individual conversation, based on the index */
  core::ConversationEntry* getConversation(QString index);
  core::ConversationEntry* getConversationStart();

  /* Returns the event */
  core::Event* getEvent();

  /* Returns the event type */
  core::EventType getEventType() const;

  /* Returns data relating to the give item event. If none, data is invalid */
  int getGiveItemChance();
  int getGiveItemCount();
  bool getGiveItemDropIfNoRoom();
  int getGiveItemID();

  /* Returns the multiple event information. If none, data is invalid */
  core::Event* getMultipleEvent(int index);
  int getMultipleEventCount();

  /* Returns the string notification. If not that event, data is blank */
  QString getNotification();

  /* Returns data relating to the property mod event */
  bool getPropertyActive();
  bool getPropertyActiveSet();
  bool getPropertyDisableMove();
  bool getPropertyDisableMoveSet();
  bool getPropertyForceInteract();
  bool getPropertyForceInteractSet();
  int getPropertyID();
  int getPropertyInactive();
  bool getPropertyInactiveSet();
  bool getPropertyPersonReset();
  int getPropertyRespawn();
  bool getPropertyRespawnSet();
  int getPropertySpeed();
  bool getPropertySpeedSet();
  core::Tracking getPropertyTrack();
  bool getPropertyTrackSet();
  bool getPropertyVisible();
  bool getPropertyVisibleSet();

  /* Returns the sound reference ID, for the related event */
  int getSoundID();

  /* Returns data relating to the start battle event */
  core::Event* getStartBattleEventLose();
  core::Event* getStartBattleEventWin();
  bool getStartBattleGameOverOnLoss();
  bool getStartBattleHideTargetOnWin();
  bool getStartBattleRestoreHealth();
  bool getStartBattleRestoreQd();

  /* Returns if on lose, the game should be over */
  bool isGameOverOnLoss() const;

  /* Returns if on battle end, the player health should be restored */
  bool isHealthRestored() const;

  /* Returns if on battle end, the player qd should be restored */
  bool isQdRestored() const;

  /* Returns if on win, the initiating target should be removed from the map */
  bool isTargetHiddenOnWin() const;

  /* Returns the start map ID, for the related event */
  int getStartMapID();

  /* Returns data relating to the take item event. If none, data is invalid */
  int getTakeItemCount();
  int getTakeItemID();

  /* Returns data relating to the teleport event. If none, data is invalid */
  int getTeleportSection();
  int getTeleportThingID();
  int getTeleportX();
  int getTeleportY();

  /* Returns a text list summary of the event */
  QString getTextSummary(QString prefix = "Event: ");

  /* Trigger IO data */
  int getTriggerIOID();

  /* Unlock IO data */
  bool getUnlockIOEventEnter();
  bool getUnlockIOEventExit();
  bool getUnlockIOEventUse();
  bool getUnlockIOEventWalkover();
  int getUnlockIOID();
  bool getUnlockIOInteraction();
  int getUnlockIOState();

  /* Unlock Thing data */
  int getUnlockThingID();

  /* Unlock Tile data */
  bool getUnlockTileEventEnter();
  bool getUnlockTileEventExit();
  int getUnlockTileSection();
  int getUnlockTileX();
  int getUnlockTileY();

  /* General unlock data - shared */
  bool getUnlockView();
  bool getUnlockViewScroll();
  int getUnlockViewTime();

  /* Insert the conversation entry at the index address. Fails if invalid point */
  bool insertConversation(QString index, core::ConversationEntry& entry);

  /* Returns if the event is a one shot trigger */
  bool isOneShot();

  /* Loads the event data */
  void load(XmlData data, int index);

  /* Saves the event data */
  void save(FileHandler* fh, bool game_only = false, QString preface = "event",
            bool no_preface = false);

  /* Sets the conversation entry at the index */
  bool setConversation(QString index, core::ConversationEntry& entry);

  /* Sets the event */
  void setEvent(core::Event* event);

  /* Sets the event to default / blank */
  void setEventBlank(bool delete_event = true);

  /* Sets the conversation event */
  bool setEventConversation(int sound_id = core::Event::kUNSET_SOUND_ID);

  /* Sets the event to give item */
  bool setEventGiveItem(int id = 0, int count = 1,
                        bool drop_if_no_room = false,
                        int chance = core::EventItemGive::kMAX_CHANCE,
                        int sound_id = core::Event::kUNSET_SOUND_ID);

  /* Sets a single new event in the multiple event */
  bool setEventInMultiple(int index, core::Event& new_event);

  /* Sets the event to multiple set */
  bool setEventMultiple(int sound_id = core::Event::kUNSET_SOUND_ID);

  /* Sets the event to notification text */
  bool setEventNotification(QString notification = "Blank",
                            int sound_id = core::Event::kUNSET_SOUND_ID);

  /* Sets the event to a property modifier event */
  bool setEventProp(int thing_id, int sound_id = core::Event::kUNSET_SOUND_ID);
  bool setEventPropActive(bool modified, bool active = false);
  bool setEventPropDisableMove(bool modified, bool disable_move = false);
  bool setEventPropForceInteract(bool modified, bool force_interact = false);
  bool setEventPropInactive(bool modified, int inactive_time = -1);
  bool setEventPropPersonReset(bool reset);
  bool setEventPropRespawn(bool modified, int respawn_time = -1);
  bool setEventPropSpeed(bool modified, int speed = 0);
  bool setEventPropTrack(bool modified, core::Tracking track_state = core::Tracking::NONE);
  bool setEventPropVisible(bool modified, bool visible = false);

  /* Sets the event to strictly a sound event */
  bool setEventSound(int sound_id = core::Event::kUNSET_SOUND_ID);

  /* Sets the event to start battle */
  bool setEventStartBattle(int sound_id = core::Event::kUNSET_SOUND_ID);
  bool setEventStartBattle(bool game_over_on_loss, bool hide_target_on_win, bool restore_health,
                           bool restore_qd, core::Event& event_win, core::Event& event_lose,
                           int sound_id = core::Event::kUNSET_SOUND_ID);

  /* Sets the event to start map */
  bool setEventStartMap(int id = 0, int sound_id = core::Event::kUNSET_SOUND_ID);

  /* Sets the event to take item */
  bool setEventTakeItem(int id = 0, int count = 1,
                        int sound_id = core::Event::kUNSET_SOUND_ID);

  /* Sets the event to teleport a thing */
  bool setEventTeleport(int thing_id = 0, int section_id = 0, int x = 0,
                        int y = 0, int sound_id = core::Event::kUNSET_SOUND_ID);

  /* Sets the event to the trigger IO */
  bool setEventTriggerIO(int io_id = core::EventTriggerIO::kINITIATING_IO_ID,
                         int sound_id = core::Event::kUNSET_SOUND_ID);

  /* Sets the event to the unlock IO */
  bool setEventUnlockIO(int io_id = 0, bool unlock_enter = false, bool unlock_exit = false,
                        bool unlock_use = false, bool unlock_walkover = false,
                        int state_num = core::EventUnlockIO::kALL_STATES_ID,
                        bool unlock_interaction = false,
                        bool view = false, bool view_scroll = false,
                        int view_time = core::EventUnlock::kDEFAULT_VIEW_TIME_MS,
                        int sound_id = core::Event::kUNSET_SOUND_ID);

  /* Sets the event to the unlock thing */
  bool setEventUnlockThing(int thing_id = 0, bool view = false, bool view_scroll = false,
                           int view_time = core::EventUnlock::kDEFAULT_VIEW_TIME_MS,
                           int sound_id = core::Event::kUNSET_SOUND_ID);

  /* Sets the event to the unlock tile */
  bool setEventUnlockTile(int section_id = core::EventUnlockTile::kACTIVE_SECTION_ID,
                          uint16_t tile_x = 0, uint16_t tile_y = 0,
                          bool unlock_enter = false, bool unlock_exit = false,
                          bool view = false, bool view_scroll = false,
                          int view_time = core::EventUnlock::kDEFAULT_VIEW_TIME_MS,
                          int sound_id = core::Event::kUNSET_SOUND_ID);

  /* Sets if the event is a one shot */
  void setOneShot(bool one_shot);

  /* Sets the sound ID, for the event */
  void setSoundID(int id);

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
  /* Returns the text summary for the given classification */
  static QString classToText(EventClassifier classification,
                             QString prefix = "Event: ", bool one_shot = false);

  /* Using the base index, the conversation could be an option (convert true) */
  static bool couldBeOption(QString base_index, int child_count);
};

#endif // EDITOREVENT_H
