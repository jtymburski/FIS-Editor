/*******************************************************************************
 * Class Name: EditorMapItem
 * Date Created: April 25, 2015
 * Inheritance: EditorMapThing
 * Description: The class for managing the interfacing with MapItem and
 *              filling it with data. The management pop-up is ItemDialog.
 ******************************************************************************/
#ifndef EDITORMAPITEM_H
#define EDITORMAPITEM_H

#include "Database/EditorMapThing.h"
#include "Game/Map/MapItem.h"

class EditorMapItem : public EditorMapThing
{
public:
  /* Constructor function */
  EditorMapItem(int id = -1, QString name = "Default Name",
                QString description = "");

  /* Copy constructor function */
  EditorMapItem(const EditorMapItem &source);

  /* Destructor function */
  virtual ~EditorMapItem();

private:
  /* Reference item */
  MapItem item;

  /* -------------------------- Constants ------------------------- */
  //const static uint8_t kTOTAL_SURFACES; /* The max # of surfaces to walk on */

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Delete defines matrixes stored in class - called once at destruction */
  //void deleteMatrixes();

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorMapItem &source);

  /* Saves the person data - virtualized */
  virtual void saveData(FileHandler* fh, bool game_only = false,
                        bool inc_matrix = true);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Gets the base item of the item */
  EditorMapItem* getBaseItem() const;

  /* Gets the item classification */
  virtual ThingBase getClass() const;

  /* Returns the count of how many of these items are available */
  uint32_t getCount();

  /* Returns if the item is picked up by just walking over */
  bool isWalkover();

  /* Loads the item data */
  virtual void load(XmlData data, int index);

  /* Saves the item data */
  virtual void save(FileHandler* fh, bool game_only = false);

  /* Sets the base reference item */
  void setBase(EditorMapItem* item, bool synchronize = true);

  /* Sets the number of this item */
  void setCount(uint32_t count);

  /* Correlates this data to the item struct */
  bool setData(ItemData data);

  /* Sets if the item is picked up by merely walking over it */
  void setWalkover(bool walkover);

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  /* The copy operator */
  EditorMapItem& operator= (const EditorMapItem &source);
};

#endif // EDITORMAPITEM_H
