/*******************************************************************************
 * Class Name: EditorBattleScene
 * Date Created: January 23, 2016
 * Inheritance: EditorTemplate (interface class)
 * Description: Information to access the battle scene, and edit if applicable
 ******************************************************************************/
#include "Database/EditorBattleScene.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Main constructor function. Sets all parameters to blank values.
 *
 * Inputs: none
 */
EditorBattleScene::EditorBattleScene()
{
  changed = false;
  clear();
}

/*
 * Description: Second constructor function, with ID and Name parameters.
 *
 * Inputs: none
 */
EditorBattleScene::EditorBattleScene(int id, QString name)
                 : EditorBattleScene()
{
  setID(id);
  setName(name);
}

/*
 * Description: Copy constructor. Calls the blank constructor and then copies
 *              the data from the source.
 *
 * Inputs: const EditorBattleScene &source - the source object to copy
 */
EditorBattleScene::EditorBattleScene(const EditorBattleScene &source)
                 : EditorBattleScene()
{
  copySelf(source);
}

/*
 * Description: Destructor function
 */
EditorBattleScene::~EditorBattleScene()
{
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: Copies all data from source editor object to this editor
 *              object.
 *
 * Inputs: EditorBattleScene &source - the source to copy from
 * Output: none
 */
void EditorBattleScene::copySelf(const EditorBattleScene &source)
{
  name = source.name;
  scene = source.scene;
}

/*
 * Description: Saves the lay over portion of the scene with the passed in
 *              specifier information. Can be used for any lay over: under, mid,
 *              over, etc.
 *
 * Inputs: LayOver lay_over - the lay over information to save
 *         std::string element - the element type: "underlay", "midlay", etc
 *         int index - the index within the lay over stack
 *         FileHandler* fh - the file handling reference pointer
 *         bool game_only - true to only save game related info
 * Output: none
 */
void EditorBattleScene::saveLayOver(LayOver lay_over, std::string element,
                                    int index, FileHandler* fh, bool game_only)
{
  (void)game_only;
  LayOver default_lay = Helpers::createBlankLayOver();

  /* Wrapper */
  fh->writeXmlElement(element, "index", index);

  /* Data */
  fh->writeXmlData("path", lay_over.path);
  if(lay_over.anim_time != default_lay.anim_time)
    fh->writeXmlData("animation", lay_over.anim_time);
  if(lay_over.velocity_x != default_lay.velocity_x)
    fh->writeXmlData("velx", lay_over.velocity_x);
  if(lay_over.velocity_y != default_lay.velocity_y)
    fh->writeXmlData("vely", lay_over.velocity_y);

  /* End Wrapper */
  fh->writeXmlElementEnd();
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Clears class data back to default.
 *
 * Inputs: bool save_id - true to save the old id in the clear. Default false
 * Output: none
 */
void EditorBattleScene::clear(bool save_id)
{
  /* Old ID */
  int old_id = scene.id;

  /* Clear data */
  name = "New Scene";
  scene = Helpers::createBlankScene();
  if(save_id)
    scene.id = old_id;
}

/*
 * Description: Returns the background path of the frame. If blank, it is not
 *              set.
 *
 * Inputs: none
 * Output: QString - the reference background path
 */
QString EditorBattleScene::getBackground() const
{
  return QString::fromStdString(scene.background);
}

/*
 * Description: Returns the ID of the scene.
 *
 * Inputs: none
 * Output: int - the coordination ID
 */
int EditorBattleScene::getID() const
{
  return scene.id;
}

/*
 * Description: Returns the mid lay over at the index within the stack. If the
 *              index is out of range, it returns a blank struct.
 *
 * Inputs: int index - the index of the midlay within the stack of midlays
 * Output: LayOver - the returned structure at given index
 */
LayOver EditorBattleScene::getMidlay(int index) const
{
  if(index >= 0 && index < static_cast<int>(scene.midlays.size()))
    return scene.midlays[index];
  return Helpers::createBlankLayOver();
}

/*
 * Description: Returns the stack of lay overs for the midlay of the scene.
 *
 * Inputs: none
 * Output: QVector<LayOver> - the stack of midlays in a vector
 */
QVector<LayOver> EditorBattleScene::getMidlays() const
{
  return QVector<LayOver>::fromStdVector(scene.midlays);
}

/*
 * Description: Returns the ID of the trigger music for the scene
 *
 * Inputs: none
 * Output: int - the music trigger ID
 */
int EditorBattleScene::getMusicID() const
{
  return scene.music_id;
}

/*
 * Description: Returns the name of the battle scene
 *
 * Inputs: none
 * Output: QString - the name
 */
QString EditorBattleScene::getName() const
{
  return name;
}

/*
 * Description: Get the listing name string which includes the ID and name
 *
 * Inputs: none
 * Output: QString - the name list
 */
QString EditorBattleScene::getNameList()
{
  return EditorHelpers::getListString(getID(), getName());
}

/*
 * Description: Returns the over lay over at the index within the stack. If the
 *              index is out of range, it returns a blank struct.
 *
 * Inputs: int index - the index of the overlay within the stack of overlays
 * Output: LayOver - the returned structure at given index
 */
LayOver EditorBattleScene::getOverlay(int index) const
{
  if(index >= 0 && index < static_cast<int>(scene.overlays.size()))
    return scene.overlays[index];
  return Helpers::createBlankLayOver();
}

/*
 * Description: Returns the stack of lay overs for the overlay of the scene.
 *
 * Inputs: none
 * Output: QVector<LayOver> - the stack of overlays in a vector
 */
QVector<LayOver> EditorBattleScene::getOverlays() const
{
  return QVector<LayOver>::fromStdVector(scene.overlays);
}

/*
 * Description: Returns the under lay over at the index within the stack. If the
 *              index is out of range, it returns a blank struct.
 *
 * Inputs: int index - the index of the underlay within the stack of underlays
 * Output: LayOver - the returned structure at given index
 */
LayOver EditorBattleScene::getUnderlay(int index) const
{
  if(index >= 0 && index < static_cast<int>(scene.underlays.size()))
    return scene.underlays[index];
  return Helpers::createBlankLayOver();
}

/*
 * Description: Returns the stack of lay overs for the underlay of the scene.
 *
 * Inputs: none
 * Output: QVector<LayOver> - the stack of underlays in a vector
 */
QVector<LayOver> EditorBattleScene::getUnderlays() const
{
  return QVector<LayOver>::fromStdVector(scene.underlays);
}

/*
 * Description: Returns if the scene has active edits in the BattleSceneView.
 *              This value means nothing if this class is not being edited.
 *
 * Inputs: none
 * Output: bool - true if has changed. false otherwise
 */
bool EditorBattleScene::isChanged()
{
  return changed;
}

/*
 * Description: Loads the object data from the XML struct and offset index.
 *
 * Inputs: XmlData data - the XML data tree struct
 *         int index - the offset index into the struct
 * Output: bool - true if name was updated
 */
bool EditorBattleScene::load(XmlData data, int index)
{
  bool name_updated = false;

  /* Parse elements */
  if(data.getElement(index) == "name")
  {
    setName(QString::fromStdString(data.getDataString()));
    name_updated = true;
  }
  else
  {
    scene = Helpers::updateScene(scene, data, index);
  }

  return name_updated;
}

/*
 * Description: Saves the object data to the file handling pointer.
 *
 * Inputs: FileHandler* fh - the file handling pointer
 *         bool game_only - true if the data should include game only relevant
 * Output: none
 */
void EditorBattleScene::save(FileHandler* fh, bool game_only)
{
  if(fh != nullptr)
  {
    /* Processing */
    EditorBattleScene default_scene;

    /* Wrapper */
    fh->writeXmlElement("battlescene", "id", getID());

    /* Name */
    if(!game_only)
      fh->writeXmlData("name", getName().toStdString());

    /* Background */
    if(!scene.background.empty())
      fh->writeXmlData("background", scene.background);

    /* Music ID */
    if(getMusicID() != default_scene.getMusicID())
      fh->writeXmlData("music", getMusicID());

    /* Underlays */
    for(uint32_t i = 0; i < scene.underlays.size(); i++)
      if(!scene.underlays[i].path.empty())
        saveLayOver(scene.underlays[i], "underlay", i, fh, game_only);

    /* Midlays */
    for(uint32_t i = 0; i < scene.midlays.size(); i++)
      if(!scene.midlays[i].path.empty())
        saveLayOver(scene.midlays[i], "midlay", i, fh, game_only);

    /* Overlays */
    for(uint32_t i = 0; i < scene.overlays.size(); i++)
      if(!scene.overlays[i].path.empty())
        saveLayOver(scene.overlays[i], "overlay", i, fh, game_only);

    /* End Wrapper */
    fh->writeXmlElementEnd();
  }
}

/*
 * Description: Sets the background of the scene
 *
 * Inputs: QString background - the new background path for the frame
 * Output: none
 */
void EditorBattleScene::setBackground(QString background)
{
  scene.background = background.toStdString();
}

/*
 * Description: Sets if changes have been made to the object copy in
 *              BattleSceneView of this object. This value is irrelevant if
 *              this object isn't being edited.
 *
 * Inputs: bool changed - true if changed. false otherwise
 * Output: none
 */
void EditorBattleScene::setChanged(bool changed)
{
  this->changed = changed;
}

/*
 * Description: Sets the ID of the battle scene.
 *
 * Inputs: int id - the new id
 * Output: none
 */
void EditorBattleScene::setID(int id)
{
  scene.id = id;
}

/*
 * Description: Sets the midlay at the given index with the new lay over
 *              structure. If the index is higher than the size, it fills it
 *              until the index is valid.
 *
 * Inputs: int index - the index within the midlay stack
 *         LayOver lay_over - the new lay structure of data to insert
 * Output: bool - true if the lay was set
 */
bool EditorBattleScene::setMidlay(int index, LayOver lay_over)
{
  if(index >= 0)
  {
    /* Ensure of size */
    while(index >= static_cast<int>(scene.midlays.size()))
      scene.midlays.push_back(Helpers::createBlankLayOver());

    /* Add element and return */
    scene.midlays[index] = lay_over;
    return true;
  }
  return false;
}

/*
 * Description: Sets the midlay at the given index with the new lay over
 *              data. If the index is higher than the size, it fills it
 *              until the index is valid.
 *
 * Inputs: int index - the index within the midlay stack
 *         LayOver lay_over - the new lay structure of data to insert
 *         QString path - the path for the sprite data of the lay over
 *         int anim_time - the animation time of the sprite
 *         float velocity_x - the velocity in x direction in pixels/ms
 *         float velocity_y - the velocity in y direction in pixels/ms
 * Output: bool - true if the lay was set
 */
bool EditorBattleScene::setMidlay(int index, QString path, int anim_time,
                                  float velocity_x, float velocity_y)
{
  LayOver lay_over = Helpers::createBlankLayOver();
  lay_over.anim_time = anim_time;
  lay_over.path = path.toStdString();
  lay_over.velocity_x = velocity_x;
  lay_over.velocity_y = velocity_y;
  return setMidlay(index, lay_over);
}

/*
 * Description: Sets the music reference ID triggered when a battle scene
 *              starts. If less than 0, this will mean no music will trigger.
 *
 * Inputs: int music_id - the reference music id to trigger
 * Output: none
 */
void EditorBattleScene::setMusicID(int music_id)
{
  scene.music_id = music_id;
}

/*
 * Description: Sets the name of the battle scene.
 *
 * Inputs: QString name - the new name text
 *         bool update - should the widget be updated? default true
 * Output: none
 */
void EditorBattleScene::setName(QString name, bool update)
{
  (void)update;

  this->name = name;
}

/*
 * Description: Sets the battle scene with the new struct of information. This
 *              includes all data excluding the name.
 *
 * Inputs: BattleScene new_scene - the structure of scene data
 * Output: none
 */
void EditorBattleScene::setScene(BattleScene new_scene)
{
  scene = new_scene;
}

/*
 * Description: Sets the overlay at the given index with the new lay over
 *              structure. If the index is higher than the size, it fills it
 *              until the index is valid.
 *
 * Inputs: int index - the index within the overlay stack
 *         LayOver lay_over - the new lay structure of data to insert
 * Output: bool - true if the lay was set
 */
bool EditorBattleScene::setOverlay(int index, LayOver lay_over)
{
  if(index >= 0)
  {
    /* Ensure of size */
    while(index >= static_cast<int>(scene.overlays.size()))
      scene.overlays.push_back(Helpers::createBlankLayOver());

    /* Add element and return */
    scene.overlays[index] = lay_over;
    return true;
  }
  return false;
}

/*
 * Description: Sets the overlay at the given index with the new lay over
 *              data. If the index is higher than the size, it fills it
 *              until the index is valid.
 *
 * Inputs: int index - the index within the overlay stack
 *         LayOver lay_over - the new lay structure of data to insert
 *         QString path - the path for the sprite data of the lay over
 *         int anim_time - the animation time of the sprite
 *         float velocity_x - the velocity in x direction in pixels/ms
 *         float velocity_y - the velocity in y direction in pixels/ms
 * Output: bool - true if the lay was set
 */
bool EditorBattleScene::setOverlay(int index, QString path, int anim_time,
                                   float velocity_x, float velocity_y)
{
  LayOver lay_over = Helpers::createBlankLayOver();
  lay_over.anim_time = anim_time;
  lay_over.path = path.toStdString();
  lay_over.velocity_x = velocity_x;
  lay_over.velocity_y = velocity_y;
  return setOverlay(index, lay_over);
}

/*
 * Description: Sets the underlay at the given index with the new lay over
 *              structure. If the index is higher than the size, it fills it
 *              until the index is valid.
 *
 * Inputs: int index - the index within the underlay stack
 *         LayOver lay_over - the new lay structure of data to insert
 * Output: bool - true if the lay was set
 */
bool EditorBattleScene::setUnderlay(int index, LayOver lay_over)
{
  if(index >= 0)
  {
    /* Ensure of size */
    while(index >= static_cast<int>(scene.underlays.size()))
      scene.underlays.push_back(Helpers::createBlankLayOver());

    /* Add element and return */
    scene.underlays[index] = lay_over;
    return true;
  }
  return false;
}

/*
 * Description: Sets the underlay at the given index with the new lay over
 *              data. If the index is higher than the size, it fills it
 *              until the index is valid.
 *
 * Inputs: int index - the index within the underlay stack
 *         LayOver lay_over - the new lay structure of data to insert
 *         QString path - the path for the sprite data of the lay over
 *         int anim_time - the animation time of the sprite
 *         float velocity_x - the velocity in x direction in pixels/ms
 *         float velocity_y - the velocity in y direction in pixels/ms
 * Output: bool - true if the lay was set
 */
bool EditorBattleScene::setUnderlay(int index, QString path, int anim_time,
                                    float velocity_x, float velocity_y)
{
  LayOver lay_over = Helpers::createBlankLayOver();
  lay_over.anim_time = anim_time;
  lay_over.path = path.toStdString();
  lay_over.velocity_x = velocity_x;
  lay_over.velocity_y = velocity_y;
  return setUnderlay(index, lay_over);
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/*
 * Description: Copy operator construction. This is called when the variable
 *              already exists and equal operator used with another
 *              EditorBattleScene.
 *
 * Inputs: const EditorBattleScene &source - the source class constructor
 * Output: EditorBattleScene& - pointer to the copied class
 */
EditorBattleScene& EditorBattleScene::operator=(const EditorBattleScene &source)
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

//static bool greaterThan(EditorSound* s1, EditorSound* s2);
