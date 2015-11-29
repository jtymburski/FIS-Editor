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

#include "FileHandler.h"
#include "Game/EventSet.h"

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

  /* Saves the conversation - recursive call */
  void saveConversation(FileHandler* fh, Conversation* convo = NULL,
                        QString index = "1");

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Deletes conversation element */
  QString deleteConversation(QString index);

  /* Returns an individual conversation, based on the index */
  Conversation* getConversation();
  Conversation* getConversation(QString index, bool before = false);

  /* Returns the conversation index */
  QString getConversationIndex(Conversation* convo);

  /* Returns the event */
  Event* getEvent();

  /* Returns the event type */
  EventClassifier getEventType() const;

  /* Returns data relating to the give item event. If none, data is invalid */
  int getGiveItemCount();
  int getGiveItemID();

  /* Returns the string notification. If not that event, data is blank */
  QString getNotification();

  /* Returns the soun reference ID, for the related event */
  int getSoundID();

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

  /* Insert conversations at index control points. Fails if invalid point */
  QString insertConversationAfter(QString index, Conversation convo,
                                  bool option_node = false);
  QString insertConversationBefore(QString index, Conversation convo,
                                   bool option_node = false);

  /* Loads the event data */
  void load(XmlData data, int index);

  /* Saves the event data */
  void save(FileHandler* fh, bool game_only = false, QString preface = "event",
            bool no_preface = false);

  /* Sets the conversation at the index */
  bool setConversation(QString index, Conversation convo);

  /* Sets the event */
  void setEvent(Event event);

  /* Sets the event to default / blank */
  void setEventBlank(bool delete_event = true);

  /* Sets the conversation event */
  bool setEventConversation(Conversation* convo = NULL,
                            int sound_id = EventSet::kUNSET_ID);

  /* Sets the event to give item */
  bool setEventGiveItem(int id = 0, int count = 1,
                        int sound_id = EventSet::kUNSET_ID);

  /* Sets the event to notification text */
  bool setEventNotification(QString notification = "Blank",
                            int sound_id = EventSet::kUNSET_ID);

  /* Sets the event to strictly a sound event */
  bool setEventSound(int sound_id = EventSet::kUNSET_ID);

  /* Sets the event to start battle */
  bool setEventStartBattle(int sound_id = EventSet::kUNSET_ID);

  /* Sets the event to start map */
  bool setEventStartMap(int id = 0, int sound_id = EventSet::kUNSET_ID);

  /* Sets the event to take item */
  bool setEventTakeItem(int id = 0, int count = 1,
                        int sound_id = EventSet::kUNSET_ID);

  /* Sets the event to teleport a thing */
  bool setEventTeleport(int thing_id = 0, int section_id = 0, int x = 0,
                        int y = 0, int sound_id = EventSet::kUNSET_ID);

  /* Sets the sound ID, for the event */
  bool setSoundID(int id);

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
  /* Converts the conversation index to usable form */
  static QString convertConversationIndex(QString index);

  /* Using the base index, the conversation could be an option (convert true) */
  static bool couldBeOption(QString base_index, int child_count);

  /* Creates conversations used for insertion */
  static Conversation createConversation(QString text, int id, Event event);
  static Conversation createConversation(QString text, int id, Event event,
                                        QVector<QPair<QString, Event>> options);

  /* Prints the conversation */
  static void printConversation(Conversation* convo, QString index = "1");
};

#endif // EDITOREVENT_H
