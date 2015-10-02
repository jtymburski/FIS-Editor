/*******************************************************************************
 * Class Name: EditorSound
 * Date Created: September 29, 2015
 * Inheritance: QWidget
 * Description: Editor Sound storage interface
 ******************************************************************************/
#ifndef EDITORSOUND_H
#define EDITORSOUND_H

#include <QString>

#include "Database/EditorTemplate.h"
#include "EditorHelpers.h"
#include "FileHandler.h"
#include "Sound.h"

class EditorSound : public EditorTemplate
{
public:
  /* Constructor Function */
  EditorSound();

  /* Constructor function with id and name */
  EditorSound(int id, QString name = "New Sound");

  /* Copy constructor */
  EditorSound(const EditorSound &source);

  /* Destructor function */
  virtual ~EditorSound();

private:
  /* File reference */
  QString file_name;

  /* Editor ID */
  int id;

  /* Editor name */
  QString name;

  /* The reference sound for data */
  Sound sound_ref;

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorSound &source);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the sound fade time */
  uint32_t getFadeTime() const;

  /* Returns the sound reference file name: trimmed */
  QString getFileName() const;

  /* Returns the ID of the sound */
  virtual int getID() const;

  /* Returns the name of the sound */
  virtual QString getName() const;

  /* Returns the name of the sound for listing */
  virtual QString getNameList();

  /* Returns the sound volume */
  uint8_t getVolume() const;

  /* Loads the object data */
  void load(XmlData data, int index);

  /* Saves the object data */
  void save(FileHandler* fh, bool game_only = false, QString type = "sound");

  /* Sets the sound fade time */
  void setFadeTime(uint32_t time);

  /* Sets the sound reference file name */
  bool setFileName(QString name);
  bool setFileNameFull(QString name);

  /* Sets the ID of the sound */
  virtual void setID(int id);

  /* Sets the name of the sound */
  virtual void setName(QString name);

  /* Sets the volume of the sound */
  void setVolume(uint8_t volume);

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  /* The copy operator */
  EditorSound& operator= (const EditorSound &source);
};

#endif // EDITORSOUND_H
