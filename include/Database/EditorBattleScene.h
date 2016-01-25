/*******************************************************************************
 * Class Name: EditorBattleScene
 * Date Created: January 22, 2016
 * Inheritance: EditorTemplate (interface class)
 * Description: Information to access the battle scene, and edit if applicable
 ******************************************************************************/
#ifndef EDITORBATTLESCENE_H
#define EDITORBATTLESCENE_H

#include <QString>

#include "Database/EditorTemplate.h"
#include "EditorHelpers.h"
#include "FileHandler.h"
#include "Helpers.h"
#include "Sound.h"

class EditorBattleScene : public EditorTemplate
{
public:
  /* Constructor Function */
  EditorBattleScene();

  /* Constructor function with id and name */
  EditorBattleScene(int id, QString name = "New Scene");

  /* Copy constructor */
  EditorBattleScene(const EditorBattleScene &source);

  /* Destructor function */
  virtual ~EditorBattleScene();

private:
  /* Is changed */
  bool changed;

  /* Editor name */
  QString name;

  /* The reference scene for data */
  BattleScene scene;

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorBattleScene &source);

  /* Save the lay over */
  void saveLayOver(LayOver lay_over, std::string element, int index,
                   FileHandler* fh, bool game_only);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Clears class data back to default */
  void clear(bool save_id = false);

  /* Returns the background path of the scene */
  QString getBackground() const;

  /* Returns the ID of the scene */
  virtual int getID() const;
  
  /* Returns the midlay(s) LayOver struct */
  LayOver getMidlay(int index) const;
  LayOver* getMidlayRef(int index);
  int getMidlaySize() const;
  QVector<LayOver> getMidlays() const;
  
  /* Returns the music reference ID of the scene */
  int getMusicID() const;

  /* Returns the name of the scene */
  virtual QString getName() const;

  /* Returns the name of the scene for listing */
  virtual QString getNameList();

  /* Returns the overlay(s) LayOver struct */
  LayOver getOverlay(int index) const;
  LayOver* getOverlayRef(int index);
  int getOverlaySize() const;
  QVector<LayOver> getOverlays() const;

  /* Returns the underlay(s) LayOver struct */
  LayOver getUnderlay(int index) const;
  LayOver* getUnderlayRef(int index);
  int getUnderlaySize() const;
  QVector<LayOver> getUnderlays() const;

  /* Returns if the scene has active edits in view */
  bool isChanged();

  /* Loads the object data */
  bool load(XmlData data, int index);

  /* Saves the object data */
  void save(FileHandler* fh, bool game_only = false);

  /* Sets the background of the scene */
  void setBackground(QString background);

  /* Sets if changes have been made (in the object within BattleSceneView) */
  void setChanged(bool changed);

  /* Sets the ID of the scene */
  virtual void setID(int id);

  /* Sets the midlay(s) for the scene */
  bool setMidlay(int index, LayOver lay_over);
  bool setMidlay(int index, QString path, int anim_time, 
                 float velocity_x, float velocity_y);

  /* Sets the music reference ID of the scene */
  void setMusicID(int music_id);

  /* Sets the name of the scene */
  virtual void setName(QString name, bool update = true);

  /* Sets the scene with the attached structure */
  void setScene(BattleScene new_scene);
  
  /* Sets the overlay(s) for the scene */
  bool setOverlay(int index, LayOver lay_over);
  bool setOverlay(int index, QString path, int anim_time, 
                  float velocity_x, float velocity_y);

  /* Sets the underlay(s) for the scene */
  bool setUnderlay(int index, LayOver lay_over);
  bool setUnderlay(int index, QString path, int anim_time, 
                   float velocity_x, float velocity_y);

  /* Unsets the midlay(s) */
  bool unsetMidlay(int index);
  void unsetMidlays();

  /* Unsets the overlay(s) */
  bool unsetOverlay(int index);
  void unsetOverlays();

  /* Unset the underlay(s) */
  bool unsetUnderlay(int index);
  void unsetUnderlays();

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  /* The copy operator */
  EditorBattleScene& operator= (const EditorBattleScene &source);

/*============================================================================
 * PUBLIC STATIC FUNCTIONS
 *===========================================================================*/
public:
  //static bool greaterThan(EditorSound* s1, EditorSound* s2);
};

#endif // EDITORBATTLESCENE_H
