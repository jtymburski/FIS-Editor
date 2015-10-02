/*******************************************************************************
 * Class Name: EditorSound
 * Date Created: September 29, 2015
 * Inheritance: QWidget
 * Description: Editor Sound storage interface
 ******************************************************************************/
#include "Database/EditorSound.h"
//#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Main constructor function. Sets all parameters to blank values.
 *
 * Inputs: none
 */
EditorSound::EditorSound()
{
  file_name = "";
  id = 0;
  name = "New Sound";
}

/*
 * Description: Second constructor function, with ID and Name parameters.
 *
 * Inputs: none
 */
EditorSound::EditorSound(int id, QString name) : EditorSound()
{
  setID(id);
  setName(name);
}

/*
 * Description: Copy constructor. Calls the blank constructor and then copies
 *              the data from the source.
 *
 * Inputs: const EditorSound &source - the source object to copy
 */
EditorSound::EditorSound(const EditorSound &source)
{
  copySelf(source);
}

/*
 * Description: Destructor function
 */
EditorSound::~EditorSound()
{
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
  
/*
 * Description: Copies all data from source editor object to this editor
 *              object.
 *
 * Inputs: EditorSound &source - the source to copy from
 * Output: none
 */
void EditorSound::copySelf(const EditorSound &source)
{
  file_name = source.file_name;
  id = source.id;
  name = source.name;
  sound_ref = source.sound_ref;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Returns the fade time of the sound file, in milliseconds.
 *
 * Inputs: none
 * Output: uint32_t - the fade time
 */
uint32_t EditorSound::getFadeTime() const
{
  return sound_ref.getFadeTime();
}

/*
 * Description: Returns the string trimmed file name of the sound file. The
 *              trimmed version excludes the portion of
 *              EditorHelpers::getProjectDir(). Only approves *.wav or *.ogg.
 *
 * Inputs: none
 * Output: QString - the trimmed file name of the sound file. See above.
 */
QString EditorSound::getFileName() const
{
  return file_name;
}

/*
 * Description: Returns the ID of the sound
 *
 * Inputs: none
 * Output: int - the ID
 */
int EditorSound::getID() const
{
  return id;
}

/*
 * Description: Returns the name of the sound
 *
 * Inputs: none
 * Output: QString - the name
 */
QString EditorSound::getName() const
{
  return name;
}

/*
 * Description: Get the listing name string which includes the ID and name
 *
 * Inputs: none
 * Output: QString - the name list
 */
QString EditorSound::getNameList()
{
  return EditorHelpers::getListString(getID(), getName());
}

/*
 * Description: Returns the volume of the sound file, in the range 0-128.
 *
 * Inputs: none
 * Output: uint32_t - the volume
 */
uint8_t EditorSound::getVolume() const
{
  return sound_ref.getVolume();
}

/*
 * Description: Loads the object data from the XML struct and offset index.
 *
 * Inputs: XmlData data - the XML data tree struct
 *         int index - the offset index into the struct
 * Output: none
 */
void EditorSound::load(XmlData data, int index)
{
  /* Parse elements */
  if(data.getElement(index) == "fade")
  {
    int fade_time = data.getDataInteger();
    if(fade_time >= 0)
      setFadeTime(static_cast<uint32_t>(fade_time));
  }
  else if(data.getElement(index) == "name")
  {
    setName(QString::fromStdString(data.getDataString()));
  }
  else if(data.getElement(index) == "path")
  {
    setFileName(QString::fromStdString(data.getDataString()));
  }
  else if(data.getElement(index)  == "vol")
  {
    int volume = data.getDataInteger();
    if(volume >= 0 && volume <= 255)
      setVolume(static_cast<uint8_t>(volume));
  }
}

/*
 * Description: Saves the object data to the file handling pointer.
 *
 * Inputs: FileHandler* fh - the file handling pointer
 *         bool game_only - true if the data should include game only relevant
 *         QString type - the type to xml wrap the sound. Default "sound"
 * Output: none
 */
void EditorSound::save(FileHandler* fh, bool game_only, QString type)
{
  if(fh != nullptr)
  {
    /* Processing */
    EditorSound blank;

    /* Wrapper */
    fh->writeXmlElement(type.toStdString(), "id", getID());

    /* Name */
    if(!game_only)
      fh->writeXmlData("name", getName().toStdString());

    /* Path */
    if(!file_name.isEmpty())
      fh->writeXmlData("path", getFileName().toStdString());

    /* Fade Time */
    if(blank.getFadeTime() != getFadeTime())
      fh->writeXmlData("fade", (int)getFadeTime());

    /* Volume */
    if(blank.getVolume() != getVolume())
      fh->writeXmlData("vol", (int)getVolume());

    /* End Wrapper */
    fh->writeXmlElementEnd();
  }
}

/*
 * Description: Sets the fade time of the sound file. Essentially limitless.
 *
 * Inputs: uint32_t time - the new fade time, in milliseconds
 * Output: none
 */
void EditorSound::setFadeTime(uint32_t time)
{
  sound_ref.setFadeTime(time);
}

/*
 * Description: Sets the sound reference file. This should be pre-trimmed and
 *              the project directory removed prior. Additionally, it only
 *              accepts *.wav or *.ogg files.
 *
 * Inputs: QString name - the pre-trimmed path of the sound.
 * Output: bool - true if the file name was set and changed
 */
bool EditorSound::setFileName(QString name)
{
  if(name.endsWith(".wav") || name.endsWith(".ogg"))
  {
    file_name = name;
    return true;
  }
  return false;
}

/*
 * Description: Sets the sound reference file. This should be not be trimmed as
 *              the project directory will be removed during the call.
 *              Additionally, it only accepts *.wav or *.ogg files.
 *
 * Inputs: QString name - the raw path of the sound.
 * Output: bool - true if the file name was set and changed
 */
bool EditorSound::setFileNameFull(QString name)
{
  if(name.startsWith(EditorHelpers::getProjectDir()))
    return setFileName(EditorHelpers::trimPath(name));
  return false;
}

/*
 * Description: Sets the ID of the sound.
 *
 * Inputs: int id - the new id
 * Output: none
 */
void EditorSound::setID(int id)
{
  this->id = id;
}

/*
 * Description: Sets the name of the sound.
 *
 * Inputs: QString name - the new name text
 * Output: none
 */
void EditorSound::setName(QString name)
{
  this->name = name;
}

/*
 * Description: Sets the volume of the sound file, in the range 0-128.
 *
 * Inputs: uint8_t volume - the newly set volume
 * Output: none
 */
void EditorSound::setVolume(uint8_t volume)
{
  sound_ref.setVolume(volume);
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
  
/*
 * Description: Copy operator construction. This is called when the variable
 *              already exists and equal operator used with another
 *              EditorSound.
 *
 * Inputs: const EditorSound &source - the source class constructor
 * Output: EditorSound& - pointer to the copied class
 */
EditorSound& EditorSound::operator= (const EditorSound &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
