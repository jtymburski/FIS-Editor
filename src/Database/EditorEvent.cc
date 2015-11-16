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
  conversation = NULL;
  event = EventSet::createBlankEvent();
}

/*
 * Description: Constructor with single event, as created by EventHandler.
 *
 * Inputs: Event event - an init event
 */
EditorEvent::EditorEvent(Event event) : EditorEvent()
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
  //setEventBlank(false);
  conversation = NULL;
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: This is the conversation stack manipulator used to modify
 *              or view the conversation stack. See inputs descriptions for
 *              functionality.
 *
 * Inputs: QString index - this is the working index, in the format "1.1.3"
 *         bool generate - if the path doesn't exist in the tree, generate it
 *         bool before - return the conversation struct one node before index
 *         bool break_tree - if generate is enabled and the new path that
 *                           doesn't follow the tree breaks a node sequence,
 *                           do it anyway.
 * Output: Conversation* - pointer to the conversation struct in the stack
 */
Conversation* EditorEvent::convoManipulator(QString index, bool generate,
                                            bool before, bool break_tree)
{
  Conversation* found = NULL;
  QStringList index_set = index.split('.');
  QVector<uint32_t> int_set;

  /* Check to make sure they are all numbers */
  bool ok;
  int valid = true;
  for(int i = 0; valid && (i < index_set.size()); i++)
  {
    /* Check if its valid */
    for(int j = 0; j < index_set[i].size(); j++)
      if(!index_set[i][j].isDigit())
        valid = false;

    /* If valid, append it to int set */
    if(valid)
    {
      /* Pre-calculation */
      uint32_t second_last = 0;
      if(int_set.size() > 0)
        second_last = int_set.last();

      /* Push back the converted int */
      int_set.push_back(index_set[i].toUInt(&ok));

      /* Check it */
      if(!ok || int_set.last() == 0 || int_set.front() > 1)
        valid = false;
      if(int_set.last() > 1 && second_last != 1)
        valid = false;
    }
  }

  /* If valid (all are digits), proceed */
  if(valid)
  {
    Conversation* current = conversation;
    Conversation* previous = NULL;

    /* Go through the conversation and attempt to find the index */
    for(int i = 1; valid && (i < int_set.size()); i++)
    {
      /* Check if the current is has the frames needed */
      if(current != NULL && (current->next.size() >= int_set[i] || generate))
      {
        /* Check if the existing tree should be broken by a change */
        if(current->next.size() < int_set[i] && current->next.size() == 1)
        {
          /* Check if the next convo is an option set */
          Conversation* next_convo = &current->next.front();
          if(next_convo->next.size() > 1)
          {
            /* If it is, and we're allowed to break the tree, wreck it */
            if(break_tree)
              next_convo->next.resize(1);
            else
              valid = false;
          }
        }

        /* If tree check doesn't allow it, it can't proceed */
        if(valid)
        {
          /* Generate, if one doesn't exist */
          if(generate)
            while(current->next.size() < int_set[i])
              current->next.push_back(EventSet::createBlankConversation());

          previous = current;
          current = &previous->next[int_set[i]-1];
        }
      }
      /* If not, it's not valid */
      else
      {
        valid = false;
      }
    }

    /* If successful, get the conversation */
    if(valid)
    {
      if(before)
        found = previous;
      else
        found = current;
    }
  }

  return found;
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
 * Description: This is the recursive function call for finding the string
 *              index of the reference conversation node.
 *
 * Inputs: Conversation* ref - the reference conversation pointer to the node
 *         Conversation* convo - the current point in the conversation stack
 *         QString index - the index associated to the current convo (above)
 * Output: QString - index of the found reference. Blank if not found
 */
QString EditorEvent::recursiveConversationFind(Conversation* ref,
                                               Conversation* convo,
                                               QString index)
{
  /* Check to see if equivalent */
  if(convo == ref)
    return index;

  /* Call all the children */
  for(uint32_t i = 0; i < convo->next.size(); i++)
  {
    QString result = recursiveConversationFind(ref, &convo->next[i],
                                            index + "." + QString::number(i+1));
    if(result != "")
      return result;
  }

  return "";
}

/*
 * Description: Recursive save of the conversation. Takes a conversation ref
 *              and starting index with file handling pointer and parses
 *              all embedded conversations from there. Default call with just
 *              FH pointer will use the base conversation in class.
 *
 * Inputs: FileHandler* fh - the file handling pointer
 *         Conversation* convo - the starting convo, default to NULL
 *         QString index - the starting index of the starting convo, default "1"
 * Output: none
 */
void EditorEvent::saveConversation(FileHandler* fh, Conversation* convo,
                                   QString index)
{
  bool first_call = false;

  /* If conversation is NULL, load in base conversation */
  if(convo == NULL)
  {
    convo = event.convo;
    first_call = true;
  }

  /* Proceed if not NULL */
  if(convo != NULL)
  {
    fh->writeXmlElement("conversation", "id", index.toStdString());

    /* Conversation Data */
    fh->writeXmlData("text", convo->text);
    fh->writeXmlData("id", convo->thing_id);
    if(first_call && getSoundID() >= 0)
      fh->writeXmlData("sound_id", getSoundID());
    if(convo->action_event.classification != EventClassifier::NOEVENT)
    {
      EditorEvent convo_event(convo->action_event);
      convo_event.save(fh);
    }

    fh->writeXmlElementEnd();

    /* Go to the next conversations */
    for(uint16_t i = 0; i < convo->next.size(); i++)
      saveConversation(fh, &convo->next[i], index + "." + QString::number(i+1));
  }
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
 * Output: QString - the resulting index after. Blank if failed
 */
QString EditorEvent::deleteConversation(QString index)
{
  if(event.classification == EventClassifier::STARTCONVO && !index.isEmpty())
  {
    Conversation* previous = convoManipulator(index, false, true);
    Conversation* current = convoManipulator(index);

    /* Only relevant if the node is real */
    if(current != NULL)
    {
      /* If previous is NULL, it's head. Just delete all data */
      if(previous == NULL)
      {
        current->action_event = EventSet::createBlankEvent();
        current->category = DialogCategory::TEXT;
        current->next.clear();
        current->text = "";
        current->thing_id = -2;
        return index;
      }
      else
      {
        QStringList split = index.split(".");

        /* Find new index */
        QString result_index = "";
        if(previous->next.size() > 1)
        {
          if(split.last() == "1")
            result_index = index;
          else
          {
            for(int i = 0; i < split.size() - 1; i++)
              result_index += split[i] + ".";
            result_index += QString::number(split.last().toInt() - 1);
          }
        }
        else
        {
          for(int i = 0; i < split.size() - 1; i++)
            result_index += split[i] + ".";
          result_index.chop(1);
        }

        /* Erase element */
        previous->next.erase(previous->next.begin() + split.last().toInt() - 1);
        return result_index;
      }
    }
  }
  return "";
}

/*
 * Description: Returns the base conveersation pointer to the stack (head
 *              node). Note if the event is not a convo, NULL is returned.
 *
 * Inputs: none
 * Output: Conversation* - the current conversation pointer
 */
Conversation* EditorEvent::getConversation()
{
  if(event.classification == EventClassifier::STARTCONVO)
    return event.convo;
  return NULL;
}

/*
 * Description: Returns the conversation pointer to the node of the index.
 *              If before is true, it gets one node before the index.
 *
 * Inputs: QString index - the node reference index
 *         bool before - true if the node returned is the node before index
 * Output: Conversation* - the returned conversation. NULL if out of range,
 *                         not found, or not in conversation
 */
Conversation* EditorEvent::getConversation(QString index, bool before)
{
  if(event.classification == EventClassifier::STARTCONVO && !index.isEmpty())
    return convoManipulator(index, false, before);
  return NULL;
}

/*
 * Description: Returns the string index of the conversation pointer. If not
 *              found, the string returned is empty.
 *
 * Inputs: Conversation* convo - the conversation to find the index for
 * Output: QString - the returned index for the passed in conversation
 */
QString EditorEvent::getConversationIndex(Conversation* convo)
{
  if(event.classification == EventClassifier::STARTCONVO)
    return recursiveConversationFind(convo, conversation);
  return "";
}

/*
 * Description: Returns the current event, set-up in this class.
 *
 * Inputs: none
 * Output: Event* - pointer to the event struct
 */
Event* EditorEvent::getEvent()
{
  return &event;
}

/*
 * Description: Returns the type of event handled by this class.
 *
 * Inputs: none
 * Output: EventClassifier - event classification enum class
 */
EventClassifier EditorEvent::getEventType()
{
  return event.classification;
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
  int id,count;
  if(EventSet::dataEventGiveItem(event, id, count))
    return count;
  return -1;
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
  int id,count;
  if(EventSet::dataEventGiveItem(event, id, count))
    return id;
  return -1;
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
  std::string notification;
  if(EventSet::dataEventNotification(event, notification))
    return QString::fromStdString(notification);
  return "";
}

/*
 * Description: Returns the connected sound ID from the event.
 *
 * Inputs: none
 * Output: int - the connected sound ID
 */
int EditorEvent::getSoundID()
{
  if(event.classification != EventClassifier::NOEVENT)
    return event.sound_id;
  return -1;
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
  int id;
  if(EventSet::dataEventStartMap(event, id))
    return id;
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
  int id,count;
  if(EventSet::dataEventTakeItem(event, id, count))
    return count;
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
  int id,count;
  if(EventSet::dataEventTakeItem(event, id, count))
    return id;
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
  int thing_id,x,y,section_id;
  if(EventSet::dataEventTeleport(event, thing_id, x, y, section_id))
    return section_id;
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
  int thing_id,x,y,section_id;
  if(EventSet::dataEventTeleport(event, thing_id, x, y, section_id))
    return thing_id;
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
  int thing_id,x,y,section_id;
  if(EventSet::dataEventTeleport(event, thing_id, x, y, section_id))
    return x;
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
  int thing_id,x,y,section_id;
  if(EventSet::dataEventTeleport(event, thing_id, x, y, section_id))
    return y;
  return -1;
}

/*
 * Description: Inserts a passed in conversation node after the index node
 *              passed in. If the index is on a bad path on the tree, it fails.
 *              If option_node is set, a node with children can be passed in
 *              and set (only one extra layer deep). If the node doesn't exist,
 *              it creates it and all the ones on the way there.
 *
 * Inputs: QString index - the index reference node for where to insert after
 *         Conversation convo - the inserting conversation struct
 *         bool option_node - true if the convo node has children
 * Output: QString - the resulting index. Blank if failed
 */
QString EditorEvent::insertConversationAfter(QString index, Conversation convo,
                                             bool option_node)
{
  if(event.classification == EventClassifier::STARTCONVO && !index.isEmpty())
  {
    Conversation* ref = convoManipulator(index, true);
    if(ref != NULL)
    {
      /* If an option node conversation, insert accordingly */
      if(option_node)
      {
        if(ref->next.size() <= 1 && convo.next.size() > 1)
        {
          /* Clear out the next nodes in the next nodes for the add */
          for(uint16_t i = 0; i < convo.next.size(); i++)
            convo.next[i].next.clear();

          /* Append the existing to the new convo */
          if(ref->next.size() == 1)
            convo.next.front().next.push_back(ref->next.front());
          ref->next.clear();

          /* Append the new to the existing */
          ref->next.push_back(convo);
          return (index + ".1");
        }
      }
      /* Not an option node */
      else
      {
        /* Clear the passed in convo */
        convo.next.clear();

        /* Determine which version the insertion is - parent node to options */
        if(ref->next.size() > 1)
        {
          ref->next.push_back(convo);
          return (index + "." + QString::number(ref->next.size()));
        }
        /* Normal node */
        else
        {
          if(ref->next.size() == 1)
            convo.next.push_back(ref->next.front());
          ref->next.clear();
          ref->next.push_back(convo);
          return (index + ".1");
        }
      }
    }
  }
  return "";
}

/*
 * Description: Inserts a passed in conversation node before the index node
 *              passed in. If the index is on a bad path on the tree, it fails.
 *              If option_node is set, a node with children can be passed in
 *              and set (only one extra layer deep). If the node doesn't exist,
 *              it creates it and all the ones on the way there.
 *
 * Inputs: QString index - the index reference node for where to insert before
 *         Conversation convo - the inserting conversation struct
 *         bool option_node - true if the convo node has children
 * Output: QString - the resulting index. Blank if failed
 */
QString EditorEvent::insertConversationBefore(QString index, Conversation convo,
                                              bool option_node)
{
  if(event.classification == EventClassifier::STARTCONVO && !index.isEmpty())
  {
    Conversation* ref = convoManipulator(index, true, true);
    if(ref != NULL || (index == "1" && conversation != NULL))
    {
      /* If an option node conversation, insert accordingly */
      if(option_node)
      {
        if((index == "1" || ref->next.size() <= 1) && convo.next.size() > 1)
        {
          /* Clear out the next nodes in the next nodes for the add */
          for(uint16_t i = 0; i < convo.next.size(); i++)
            convo.next[i].next.clear();

          /* Separation if index is the top node */
          if(index == "1")
          {
            /* Create the new base conversation */
            Conversation base = EventSet::createBlankConversation();
            base.action_event = conversation->action_event;
            base.category = conversation->category;
            base.next = conversation->next;
            base.text = conversation->text;
            base.thing_id = conversation->thing_id;

            /* Copy the new conversation into the base */
            conversation->action_event = convo.action_event;
            conversation->category = convo.category;
            conversation->next = convo.next;
            conversation->text = convo.text;
            conversation->thing_id = convo.thing_id;
            conversation->next.front().next.push_back(base);
          }
          else
          {
            /* Append the existing to the new convo */
            if(ref->next.size() == 1)
              convo.next.front().next.push_back(ref->next.front());
            ref->next.clear();

            /* Append the new to the existing */
            ref->next.push_back(convo);
          }
          return index;
        }
      }
      /* Not an option node */
      else
      {
        /* Clear the passed in convo */
        convo.next.clear();

        /* Determine which version the insertion is - parent node to options */
        if(index == "1")
        {
          /* Create the new base conversation */
          Conversation base = EventSet::createBlankConversation();
          base.action_event = conversation->action_event;
          base.category = conversation->category;
          base.next = conversation->next;
          base.text = conversation->text;
          base.thing_id = conversation->thing_id;

          /* Copy the new conversation into the base */
          conversation->action_event = convo.action_event;
          conversation->category = convo.category;
          conversation->next = convo.next;
          conversation->text = convo.text;
          conversation->thing_id = convo.thing_id;
          conversation->next.push_back(base);
        }
        else if(ref->next.size() > 1)
        {
          QStringList split_set = index.split(".");
          ref->next.insert(ref->next.begin() + split_set.last().toInt() - 1,
                           convo);
        }
        /* Normal node */
        else
        {
          if(ref->next.size() == 1)
            convo.next.push_back(ref->next.front());
          ref->next.clear();
          ref->next.push_back(convo);
        }

        return index;
      }
    }
  }
  return "";
}

/*
 * Description: Loads the event data from the XML struct and offset index. Uses
 *              existing functions in game EventHandler class.
 *
 * Inputs: XmlData data - the XML data tree struct
 *         int index - the offset index into the struct
 * Output: none
 */
void EditorEvent::load(XmlData data, int index)
{
  event = EventSet::updateEvent(event, data, index, 0);
}

/*
 * Description: Saves the event data to the file handling pointer.
 *
 * Inputs: FileHandler* fh - the file handling pointer
 *         bool game_only - true if the data should include game only relevant
 *         QString preface - the wrapper text element. default to "event"
 *         bool no_preface - no XML wrapper included if true. Default false
 * Output: none
 */
void EditorEvent::save(FileHandler* fh, bool game_only, QString preface,
                       bool no_preface)
{
  (void)game_only;

  if(fh != NULL && event.classification != EventClassifier::NOEVENT)
  {
    if(!no_preface)
      fh->writeXmlElement(preface.toStdString());

    /* -- GIVE ITEM EVENT -- */
    if(event.classification == EventClassifier::GIVEITEM)
    {
      fh->writeXmlElement("giveitem");

      /* Data */
      fh->writeXmlData("id", getGiveItemID());
      fh->writeXmlData("count", getGiveItemCount());
      if(getSoundID() >= 0)
        fh->writeXmlData("sound_id", getSoundID());

      fh->writeXmlElementEnd();
    }
    /* -- JUST SOUND EVENT -- */
    else if(event.classification == EventClassifier::JUSTSOUND)
    {
      if(getSoundID() >= 0)
        fh->writeXmlData("justsound", getSoundID());
    }
    /* -- NOTIFICATION EVENT -- */
    else if(event.classification == EventClassifier::NOTIFICATION)
    {
      if(getSoundID() >= 0)
      {
        fh->writeXmlElement("notification");

        fh->writeXmlData("text", getNotification().toStdString());
        fh->writeXmlData("sound_id", getSoundID());

        fh->writeXmlElementEnd();
      }
      else
      {
        fh->writeXmlData("notification", getNotification().toStdString());
      }
    }
    /* -- EXECUTE BATTLE EVENT -- */
    else if(event.classification == EventClassifier::RUNBATTLE)
    {
      if(getSoundID() >= 0)
      {
        fh->writeXmlElement("startbattle");
        fh->writeXmlData("sound_id", getSoundID());
        fh->writeXmlElementEnd();
      }
      else
      {
        fh->writeXmlData("startbattle", "blank");
      }
    }
    /* -- EXECUTE MAP EVENT -- */
    else if(event.classification == EventClassifier::RUNMAP)
    {
      fh->writeXmlElement("startmap");

      /* Data */
      fh->writeXmlData("id", getStartMapID());
      if(getSoundID() >= 0)
        fh->writeXmlData("sound_id", getSoundID());

      fh->writeXmlElementEnd();
    }
    /* -- START CONVERSATION EVENT -- */
    else if(event.classification == EventClassifier::STARTCONVO)
    {
      saveConversation(fh);
    }
    /* -- TAKE ITEM EVENT -- */
    if(event.classification == EventClassifier::TAKEITEM)
    {
      fh->writeXmlElement("takeitem");

      /* Data */
      fh->writeXmlData("id", getTakeItemID());
      fh->writeXmlData("count", getTakeItemCount());
      if(getSoundID() >= 0)
        fh->writeXmlData("sound_id", getSoundID());

      fh->writeXmlElementEnd();
    }
    /* -- TELEPORT THING EVENT -- */
    else if(event.classification == EventClassifier::TELEPORTTHING)
    {
      fh->writeXmlElement("teleportthing");

      /* Data */
      fh->writeXmlData("x", getTeleportX());
      fh->writeXmlData("y", getTeleportY());
      if(getTeleportSection() >= 0)
        fh->writeXmlData("section", getTeleportSection());
      if(getTeleportThingID() != 0)
        fh->writeXmlData("id", getTeleportThingID());
      if(getSoundID() >= 0)
        fh->writeXmlData("sound_id", getSoundID());

      fh->writeXmlElementEnd();
    }

    if(!no_preface)
      fh->writeXmlElementEnd();
  }
}

/*
 * Description: Sets the conversation for the passed in index, which replaces
 *              all the data in that node only. If the index doesn't exist,
 *              it creates it plus all the nodes on the way there.
 *
 * Inputs: QString index - the reference conversation node to change info
 *         Conversation convo - new node information to place in index
 * Output: bool - true if the data set was successful
 */
bool EditorEvent::setConversation(QString index, Conversation convo)
{
  if(event.classification == EventClassifier::STARTCONVO && !index.isEmpty())
  {
    /* Attempt to get the convo. Generates any on the way that don't exist */
    Conversation* existing = convoManipulator(index, true);
    if(existing != NULL)
    {
      existing->action_event = convo.action_event;
      existing->category = convo.category;
      existing->text = convo.text;
      existing->thing_id = convo.thing_id;
      return true;
    }
  }
  return false;
}

/*
 * Description: Sets the event to the passed in event struct.
 *
 * Inputs: Event event - the event to load in
 * Output: none
 */
void EditorEvent::setEvent(Event event)
{
  /* Remove old event */
  setEventBlank();

  /* Create new event */
  this->event = EventSet::copyEvent(event);
  if(event.classification == EventClassifier::STARTCONVO)
    conversation = this->event.convo;
  //this->event = event;
  //if(event.classification == EventClassifier::STARTCONVO)
  //  conversation = event.convo;
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
    event = EventSet::deleteEvent(event);
  event = EventSet::createBlankEvent();
  conversation = NULL;
}

/*
 * Description: Sets the event in the class to the conversation. If base
 *              conversation is passed in, it is used (and takes control of
 *              deletion). Otherwise, one is generated.
 *
 * Inputs: Conversation* convo - starting conversation tree
 *         int sound_id - the connected sound ID. Default unset ref
 * Output: bool - true if event changed to conversation
 */
bool EditorEvent::setEventConversation(Conversation* convo, int sound_id)
{
  /* Create the new conversation */
  setEventBlank();
  event = EventSet::createEventConversation(convo, sound_id);
  event.convo->text = "First Entry.";
  conversation = event.convo;
  return true;
}

/*
 * Description: Sets the event in the class to the give item event, with a
 *              passed in ID of the item and count (ID needs to be greater
 *              than or equal to 0 and count greater than 0).
 *
 * Inputs: int id - the id of the item to give (game side)
 *         int count - the number of items to give on trigger
 *         int sound_id - the connected sound ID. Default unset ref
 * Output: bool - true if the event was created
 */
bool EditorEvent::setEventGiveItem(int id, int count, int sound_id)
{
  if(id >= 0 && count > 0)
  {
    setEventBlank();
    event = EventSet::createEventGiveItem(id, count, sound_id);
    return true;
  }
  return false;
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
    setEventBlank();
    event = EventSet::createEventNotification(notification.toStdString(),
                                              sound_id);
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
  setEventBlank();
  event = EventSet::createEventSound(sound_id);
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
  setEventBlank();
  event = EventSet::createEventStartBattle(sound_id);
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
    setEventBlank();
    event = EventSet::createEventStartMap(id, sound_id);
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
    setEventBlank();
    event = EventSet::createEventTakeItem(id, count, sound_id);
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
  if(thing_id >= 0 && section_id >= 0 && x >= 0 && y >= 0)
  {
    setEventBlank();
    event = EventSet::createEventTeleport(thing_id, x, y, section_id, sound_id);
    return true;
  }
  return false;
}

/*
 * Description: Sets the sound ID for the event; only if the event has already
 *              been set-up as a valid event (not NOEVENT)
 *
 * Inputs: int id - the new event ID
 * Output: none
 */
bool EditorEvent::setSoundID(int id)
{
  if(event.classification != EventClassifier::NOEVENT)
  {
    if(id >= 0)
      event.sound_id = id;
    else
      event.sound_id = EventSet::kUNSET_ID;
  }
  return false;
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
 * Description: Converts the conversation index generated in EventView ("4.5.4")
 *              to the standardized conversation index, as required by this
 *              class ("1.1.1.1.5.1.1.1.1")
 *
 * Inputs: QString index - the base simplified index
 * Output: QString - the expanded index, for use in this class for reference
 */
QString EditorEvent::convertConversationIndex(QString index)
{
  QStringList list = index.split('.');
  QString result = "";

  /* Loop through the split list */
  for(int i = 0; i < list.size(); i++)
  {
    /* If not head, add separator */
    if(i != 0)
      result += ".";

    if(i % 2 == 0)
    {
      int num = list[i].toInt();
      for(int j = 0; j < num; j++)
      {
        if(j != 0)
          result += ".1";
        else
          result += "1";
      }
    }
    else
    {
      result += list[i];
    }
  }

  return result;
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

/*
 * Description: Public static function. Creates a conversatioin with a given
 *              text, ID, and event.
 *
 * Inputs: QString text - the text displayed in the text box
 *         int id - the thing talking/interacting
 *         Event event - the event triggered on start of conversation node
 * Output: Conversation - created conversation node struct with above data
 */
Conversation EditorEvent::createConversation(QString text, int id, Event event)
{
  Conversation convo = EventSet::createBlankConversation();
  convo.text = text.toStdString();
  convo.thing_id = id;
  if(event.classification != EventClassifier::STARTCONVO)
    convo.action_event = event;

  return convo;
}

/*
 * Description: Public static function. Creates a conversatioin with a given
 *              text, ID, event, and vector of string event pairs for option
 *              children.
 *
 * Inputs: QString text - the text displayed in the text box
 *         int id - the thing talking/interacting
 *         Event event - the event triggered on start of conversation node
 *         QVec<QPair<QString,Event>> - the option string event pairs to add
 * Output: Conversation - created conversation node struct with above data
 */
Conversation EditorEvent::createConversation(QString text, int id, Event event,
                                         QVector<QPair<QString, Event>> options)
{
  Conversation convo = createConversation(text, id, event);

  /* Loop through options */
  for(int i = 0; i < options.size(); i++)
  {
    convo.next.push_back(EventSet::createBlankConversation());
    convo.next.back().text = options[i].first.toStdString();
    if(options[i].second.classification != EventClassifier::STARTCONVO)
      convo.next.back().action_event = options[i].second;
  }

  return convo;
}

/*
 * Description: Prints the conversation to the qDebug terminal. Primarily used
 *              for testing and debugging. Note, this function is recursive
 *              from the initially passed in conversation node.
 *
 * Inputs: Conversation* convo - the convo to print data for and go to children
 *         QString index - the index of the current conversation node
 * Output: none
 */
void EditorEvent::printConversation(Conversation* convo, QString index)
{
  if(convo != NULL)
  {
    /* Print the current */
    qDebug() << index << ": " << QString::fromStdString(convo->text) << " , "
             << convo->thing_id << " , "
             << (int)convo->action_event.classification;

    /* First call the function with all children */
    for(uint32_t i = 0; i < convo->next.size(); i++)
      EditorEvent::printConversation(&convo->next[i],
                                     index + "." + QString::number(i+1));
  }
}
