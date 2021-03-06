/*******************************************************************************
 * Class Name: EditorMapThing
 * Date Created: February 7, 2015
 * Inheritance: EditorTemplate
 * Description: The class for managing the interfacing with MapThing and filling
 *              it with data. The management pop-up is ThingDialog.
 ******************************************************************************/
#include "Database/EditorMapThing.h"
//#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Main constructor function. All parameters have blank defaults.
 *
 * Inputs: int id - the id of the thing
 *         QString name - the name of the thing
 *         QString description - the description of the thing
 */
EditorMapThing::EditorMapThing(int id, QString name, QString description)
              : base{nullptr},
                thing{core::MapThing(id)}
{
  matrix = new EditorMatrix(1, 1, false);

  /* Make sure there's one frame in the sprite */
  if(dialog_image.frameCount() == 0)
    dialog_image.setPath(0, "");

  /* Load data */
  EditorMapThing::setName(name);
  setDescription(description);
}

/*
 * Description: Copy constructor. Calls the blank constructor and then copies
 *              the data from the source.
 *
 * Inputs: const EditorMapThing &source - the source thing to copy
 */
EditorMapThing::EditorMapThing(const EditorMapThing &source) : EditorMapThing()
{
  copySelf(source);
}

/*
 * Description: Destructor function
 */
EditorMapThing::~EditorMapThing()
{
  base = NULL;
  if(matrix != NULL)
    delete matrix;
  matrix = NULL;
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: Copies all data from source editor thing to this editor
 *              thing.
 *
 * Inputs: EditorMapThing &source - the source to copy from
 *         bool inc_matrix - include matrix data in save (false for children)
 * Output: none
 */
void EditorMapThing::copySelf(const EditorMapThing &source, bool inc_matrix)
{
  /* Copy the thing data */
  setActive(source.isActive());
  setActiveRespawn(source.getActiveRespawn());
  setBase(source.getBaseThing());
  setDescription(source.getDescription());
  event_base = source.event_base;
  setEventSet(source.set);
  setGameID(source.getGameID());
  setID(source.getID());
  setName(source.getName());
  setSoundID(source.getSoundID());
  setVisibility(source.isVisible());

  /* Copy the visual data */
  dialog_image = source.dialog_image;
  if(inc_matrix)
    *matrix = *source.matrix;

  /* Copy the control variables */
  setTileIcons(source.tile_icons);
}

/*
 * Description: Saves the data for the thing. This does not include the thing
 *              wrapper. Virtualized for other classes as well.
 *
 * Inputs: XmlWriter* writer - the save persistence interface
 *         bool game_only - true if only include game only data
 *         bool inc_matrix - true if include the base thing matrix
 * Output: none
 */
void EditorMapThing::saveData(core::XmlWriter* writer, bool game_only, bool inc_matrix)
{
  EditorMapThing default_thing;

  /* Depending on if base or not, write different data */
  if(base != NULL)
  {
    /* Write base settings */
    writer->writeData("base", base->getID());
    QString startpoint = QString::number(getX()) + "," +
                         QString::number(getY());
    writer->writeData("startpoint", startpoint.toStdString());

    /* Check the name and description, if it's different from base */
    if(base->getName() != getName())
      writer->writeData("name", getName().toStdString());
    if(base->getDescription() != getDescription())
      writer->writeData("description", getDescription().toStdString());

    /* Check game ID */
    if(getGameID() >= 0)
      writer->writeData("game_id", getGameID());

    /* Activity status' */
    if(default_thing.isActive() != isActive())
      writer->writeData("active", isActive());
    if(default_thing.getActiveRespawn() != getActiveRespawn())
      writer->writeData("activetime", getActiveRespawn());

    /* Event save, if relevant (isBaseEvent() is true) */
    if(!event_base)
      set.save(writer, "eventset", true, false);
  }
  /* Otherwise, is base - write core data */
  else
  {
    /* Write the core thing data */
    writer->writeData("name", getName().toStdString());
    writer->writeData("description", getDescription().toStdString());
    if(default_thing.isVisible() != isVisible())
      writer->writeData("visible", isVisible());

    /* Check game ID */
    if(getGameID() >= 0)
      writer->writeData("game_id", getGameID());

    /* Check sound ID */
    if(getSoundID() >= 0)
      writer->writeData("sound_id", getSoundID());

    /* Save the dialog image */
    if(!dialog_image.isAllNull() && dialog_image.frameCount() == 1)
      writer->writeData("image",
                EditorHelpers::trimPath(dialog_image.getPath(0)).toStdString());

    /* Matrix save */
    if(inc_matrix)
      matrix->save(writer, game_only);

    /* Event save */
    set.save(writer);
  }
}

/*
 * Description: Sets the matrix in the thing class. This is used by children
 *              to utilize the painting and control of the thing.
 *
 * Inputs: EditorMatrix* matrix - the new matrix to use. Must be not NULL
 *         bool first_call - is this the first time called? Delete thing matrix
 * Output: bool - true if the matrix was set
 */
bool EditorMapThing::setMatrix(EditorMatrix* matrix, bool first_call)
{
  if(matrix != NULL)
  {
    /* If this is the first call, delete the thing matrix */
    if(first_call)
      delete this->matrix;

    this->matrix = matrix;
    //update();
    return true;
  }
  return false;
}

/*
 * Description: Unsets the matrix in the class. No deletion occurs.
 *
 * Inputs: none
 * Output: none
 */
void EditorMapThing::unsetMatrix()
{
  matrix = NULL;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Returns the active respawn time of the thing. If less than 0,
 *              the respawn is disabled
 *
 * Inputs: none
 * Output: int - the respawn time in milliseconds
 */
int EditorMapThing::getActiveRespawn() const
{
  return thing.isRespawnTimeSet() ? thing.getRespawnTimeMilliseconds() : -1;
}

/*
 * Description: Returns the base thing. Default to NULL.
 *
 * Inputs: none
 * Output: EditorMapThing* - the base thing reference pointer
 */
EditorMapThing* EditorMapThing::getBaseThing() const
{
  return base;
}

/*
 * Description: Returns the thing classification - when dealing with casting.
 *
 * Inputs: none
 * Output: MapObjectType - the thing object classification
 */
core::MapObjectType EditorMapThing::getClass() const
{
  return core::MapObjectType::THING;
}

/*
 * Description: Returns the editor description string.
 *
 * Inputs: none
 * Output: QString - the thing description
 */
QString EditorMapThing::getDescription() const
{
  return QString::fromStdString(thing.getDescription());
}

/*
 * Description: Returns the dialog image, as a ptr, stored in the thing for
 *              manipulation by an outside source. This should always contain
 *              at most one frame.
 *
 * Inputs: none
 * Output: none
 */
EditorSprite* EditorMapThing::getDialogImage()
{
  if(base != NULL)
  {
    return base->getDialogImage();
  }
  else
  {
    /* Make sure only one frame is in the dialog image */
    while(dialog_image.frameCount() > 1)
      dialog_image.deleteFrame(dialog_image.frameCount() - 1);
    if(dialog_image.frameCount() == 0)
      dialog_image.setPath(0, "");

    return &dialog_image;
  }
}

/*
 * Description: Returns the event set of the thing
 *
 * Inputs: none
 * Output: EditorEventSet* - the stored set
 */
EditorEventSet* EditorMapThing::getEventSet()
{
  if(base != nullptr && event_base)
    return base->getEventSet();
  return &set;
}

/*
 * Description: Returns the core game ID that is connected with the map thing.
 *
 * Inputs: none
 * Output: int - the game ID. -1 if unset
 */
int EditorMapThing::getGameID() const
{
  return thing.isGameIdSet() ? thing.getGameId() : -1;
}

/*
 * Description: Returns the editor thing ID
 *
 * Inputs: none
 * Output: int - the id of the editor thing
 */
int EditorMapThing::getID() const
{
  return thing.getId();
}

/*
 * Description: The matrix in the thing that contains the sprite set.
 *
 * Inputs: none
 * Output: EditorMatrix* - the matrix sprite set
 */
EditorMatrix* EditorMapThing::getMatrix() const
{
  if(base != NULL)
    return base->matrix;
  return matrix;
}

/*
 * Description: Returns the editor thing name
 *
 * Inputs: none
 * Output: The name of the editor thing
 */
QString EditorMapThing::getName() const
{
  return QString::fromStdString(thing.getName());
}

/*
 * Description: Returns the formatted name and ID for listing. It's in the
 *              format: "XXX: sssssssssssss" unless has base it's in
 *              "BASEID(ID): NAME"
 *
 * Inputs: none
 * Output: QString - the name for a list
 */
QString EditorMapThing::getNameList()
{
  int base_id = -1;
  if(base != NULL)
    base_id = base->getID();

  QString name = getName();
  if(getGameID() >= 0)
    name += "*";

  return EditorHelpers::getListString(getID(), name, base_id, true);
}

/*
 * Description: Returns the formatted name and ID for listing. It's in the
 *              format: "XXX: sssssssssssss" unless has base it's in
 *              "BASEID(ID): NAME"
 *
 * Inputs: none
 * Output: QString - the name for a list
 */
QString EditorMapThing::getNameList(bool shortened)
{
  if(shortened)
    return EditorHelpers::getListString(getID(), getName(), -1, true);
  return getNameList();
}

/*
 * Description: Returns the reference sound ID for the thing. If less than 0,
 *              it is unset.
 *
 * Inputs: none
 * Output: int - the connected sound ID
 */
int EditorMapThing::getSoundID() const
{
  if(base != nullptr && getClass() != core::MapObjectType::ITEM)
    return base->getSoundID();
  return thing.getSoundId();
}

/*
 * Description: Returns the X coordinate for the top left of the thing
 *              (in tile units)
 *
 * Inputs: none
 * Output: int - the x location in tile units
 */
int EditorMapThing::getX()
{
  return thing.isTileHorizontalSet() ? thing.getTileHorizontal() : 0;
}

/*
 * Description: Returns the Y coordinate for the top left of the thing
 *              (in tile units)
 *
 * Inputs: none
 * Output: int - the y location in tile units
 */
int EditorMapThing::getY()
{
  return thing.isTileVerticalSet() ? thing.getTileVertical() : 0;
}

/*
 * Description: Checks if the thing is set to start as active on the map space.
 *
 * Inputs: none
 * Output: bool - true if starting active. false otherwise
 */
bool EditorMapThing::isActive() const
{
  return thing.isActive();
}

/*
 * Description: Checks the matrix at the given x and y location, if the sprites
 *              within are null. Virtualized.
 *
 * Inputs: int x - the x offset from top left in matrix
 *         int y - the y offset from top left in matrix
 * Output: bool - true if sprite contains only null paths.
 */
bool EditorMapThing::isAllNull(int x, int y) const
{
  bool is_null = true;

  /* If matrix is valid */
  if(getMatrix() != NULL)
  {
    /* Get sprite, if valid */
    EditorTileSprite* sprite = getMatrix()->getSprite(x, y);
    if(sprite != NULL)
      is_null = sprite->isAllNull();
  }

  return is_null;
}

/*
 * Description: Returns if the event returned by getEvent() is the event from
 *              the base class. Only possible if getBaseThing() is not NULL.
 *
 * Inputs: none
 * Output: bool - true if the event is the base event
 */
bool EditorMapThing::isBaseEvent() const
{
  return (base != NULL) && (event_base);
}

/*
 * Description: Returns if the thing is visible when displayed in game.
 *
 * Inputs: none
 * Output: bool - true if visible when the game is running
 */
bool EditorMapThing::isVisible() const
{
  if(base != nullptr && getClass() != core::MapObjectType::ITEM)
    return base->isVisible();
  return thing.isVisible();
}

/*
 * Description: Loads the thing data from the XML struct and offset index.
 *
 * Inputs: core::XmlData data - the XML data tree entry
 *         int index - the offset index into the struct
 * Output: none
 */
void EditorMapThing::load(core::XmlData data, int index)
{
  QString element = QString::fromStdString(data.getElement(index));

  /* Parse elements */
  if(element == "active")
  {
    setActive(data.getDataBoolean());
  }
  else if(element == "activetime")
  {
    setActiveRespawn(data.getDataInteger());
  }
  else if(element == "description")
  {
    setDescription(QString::fromStdString(data.getDataString()));
  }
  else if(element == "event")
  {
    set.load(data, index);
    event_base = false;
  }
  else if(element == "eventset")
  {
    set.load(data, index + 1);
    event_base = false;
  }
  else if(element == "game_id" || element == "core_id")
  {
    setGameID(data.getDataInteger());
  }
  else if(element == "image")
  {
    dialog_image.deleteAllFrames();
    dialog_image.setPath(0, EditorHelpers::getProjectDir() +
              QDir::separator() + QString::fromStdString(data.getDataString()));
  }
  else if(element == "name")
  {
    setName(QString::fromStdString(data.getDataString()));
  }
  else if(element == "rendermatrix")
  {
    matrix->load(data, index);
  }
  else if(element == "sprites")
  {
    matrix->load(data, index + 1);
  }
  else if(element == "sound_id")
  {
    setSoundID(data.getDataInteger());
  }
  else if(element == "startpoint")
  {
    QString set = QString::fromStdString(data.getDataString());
    QStringList split = set.split(",");
    if(split.size() == 2 && split.front().toInt() >= 0 &&
                            split.back().toInt() >= 0)
    {
      setX(split.front().toInt());
      setY(split.back().toInt());
    }
  }
  else if(element == "visible")
  {
    setVisibility(data.getDataBoolean());
  }
}

/*
 * Description: Paint the active frame in the matrix into the bounding box.
 *              The offset x and y is the sprite in the matrix offset from the
 *              top left.
 *
 * Inputs: QPainter* painter - the reference paint controller
 *         QRect rect - the bounding box of the rendering frame
 *         int offset_x - the offset from left of the sprite in matrix
 *         int offset_y - the offset from top of the sprite in matrix
 * Output: bool - true if the sprite was rendered
 */
bool EditorMapThing::paint(QPainter* painter, QRect rect,
                        int offset_x, int offset_y)
{
  if(getMatrix() != nullptr)
  {
    bool shadow = (!isActive() || !isVisible());
    QColor shadow_color(255, 255, 255, 180);
    if(!isActive())
      shadow_color = QColor(0, 0, 0, 180);
    return getMatrix()->paint(painter, rect, offset_x, offset_y,
                              shadow, shadow_color);
  }
  return false;
}

/*
 * Description: Paint the frame index in the matrix into the bounding box.
 *              The offset x and y is the sprite in the matrix offset from the
 *              top left.
 *
 * Inputs: int frame_index - the index of the frame to render (0 for base)
 *         QPainter* painter - the reference paint controller
 *         QRect rect - the bounding box of the rendering frame
 *         int offset_x - the offset from left of the sprite in matrix
 *         int offset_y - the offset from top of the sprite in matrix
 * Output: bool - true if the sprite was rendered
 */
bool EditorMapThing::paint(int frame_index, QPainter* painter, QRect rect,
                           int offset_x, int offset_y)
{
  if(getMatrix() != nullptr)
  {
    bool shadow = (!isActive() || !isVisible());
    QColor shadow_color(255, 255, 255, 180);
    if(!isActive())
      shadow_color = QColor(0, 0, 0, 180);
    return getMatrix()->paint(frame_index, painter, rect, offset_x, offset_y,
                              shadow, shadow_color);
  }
  return false;
}

/*
 * Description: Saves the thing data to the file handling pointer.
 *
 * Inputs: XmlWriter* writer - the save persistence interface
 *         bool game_only - true if the data should include game only relevant
 * Output: none
 */
void EditorMapThing::save(core::XmlWriter* writer, bool game_only)
{
  if(writer != NULL)
  {
    writer->writeElement("mapthing", "id", std::to_string(getID()));
    saveData(writer, game_only);
    writer->jumpToParent();
  }
}

/*
 * Description: Sets if the thing is active and usable within the space
 *
 * Inputs: bool active - true if the thing should be active. false otherwise
 * Output: bool - returns if the thing is active
 */
bool EditorMapThing::setActive(bool active)
{
  thing.setActive(active);
  return true;
}

/*
 * Description: Sets the time once a thing is inactive before it will respawn
 *              on the map (assuming no thing is placed in its starting tile
 *              location). If set less than 0, it will never respawn (default).
 *
 * Inputs: int time - the time in milliseconds before respawning the thing
 * Output: none
 */
void EditorMapThing::setActiveRespawn(int time)
{
  thing.setRespawnTimeMilliseconds(time);
}

/*
 * Description: Sets the base thing object for the thing. It will be used for
 *              visual representation and when set, also sets the name and
 *              description (which can be changed later).
 *
 * Inputs: EditorMapThing* thing - the base thing object
 *         bool synchronize - true to set name, description, and base event
 * Output: none
 */
void EditorMapThing::setBase(EditorMapThing* thing, bool synchronize)
{
  base = thing;

  /* Sets the name and description initially to what the base is */
  if(base != nullptr && synchronize)
  {
    event_base = true;

    setName(base->getName());
    setDescription(base->getDescription());
  }
}

/*
 * Description: Sets the description of the thing.
 *
 * Inputs: QString description - the description text
 * Output: none
 */
void EditorMapThing::setDescription(QString description)
{
  thing.setDescription(description.toStdString());
}

/*
 * Description: Sets the dialog image, based on the path, for the thing.
 *
 * Inputs: QString path - the path for the dialog image
 * Output: none
 */
void EditorMapThing::setDialogImage(QString path)
{
  /* Make sure the image is empty and ready for the new path */
  while(dialog_image.frameCount() > 0)
    dialog_image.deleteFrame(dialog_image.frameCount() - 1);

  /* Set the new path to the base index */
  dialog_image.setPath(0, path);
}

/*
 * Description: Sets the interaction event set for the thing.
 *
 * Inputs: EditorEventSet set - the event set to utilize
 * Output: none
 */
void EditorMapThing::setEventSet(EditorEventSet set)
{
  this->set = set;
}

/*
 * Description: Sets the game upper level reference ID in the base class.
 *
 * Inputs: int id - the new reference id. Less than 0 is invalid
 * Output: none
 */
void EditorMapThing::setGameID(int id)
{
  if(id >= 0)
    thing.setGameId(id);
  else
    thing.unsetGameId();
}

/*
 * Description: Sets the map thing ID in the base class.
 *
 * Inputs: int id - the thing id
 * Output: none
 */
void EditorMapThing::setID(int id)
{
  if(id >= 0)
    thing.setId(id);
}

/*
 * Description: Sets the name of the thing.
 *
 * Inputs: QString name - the name text
 *         bool update - should the widget be updated? default true
 * Output: none
 */
void EditorMapThing::setName(QString name, bool update)
{
  (void)update;

  thing.setName(name.toStdString());
}

/*
 * Description: Sets the connected sound ID. If the passed in ID is less than 0,
 *              it unsets the sound ID.
 *
 * Inputs: int id - the new connected sound ID
 * Output: none
 */
void EditorMapThing::setSoundID(int id)
{
  if(id >= 0)
    thing.setSoundId(id);
  else
    thing.unsetSoundId();
}

/*
 * Description: Sets the tile icons, for rendering purposes.
 *
 * Inputs: TileIcons* icons - the rendering icon pointer. Managed by gamedb
 * Output: none
 */
void EditorMapThing::setTileIcons(TileIcons* icons)
{
  tile_icons = icons;
  if(matrix != NULL)
    matrix->setTileIcons(tile_icons);
}

/*
 * Description: Instructs the class to use the base event, if there is a base
 *              reference class being used. If set true, the event is returned
 *              from the base class. Otherwise, this class holds the relevant
 *              event.
 *
 * Inputs: bool use_base - true to use the event from the base class
 * Output: none
 */
void EditorMapThing::setUseBaseEvent(bool use_base)
{
  event_base = use_base;
}

/*
 * Description: Sets the thing visibility when rendering in the game.
 *
 * Inputs: bool visible - true if it should be visible
 * Output: none
 */
void EditorMapThing::setVisibility(bool visible)
{
  thing.setVisible(visible);
}

/*
 * Description: Sets the X coordinate of the top left of the thing (in tile
 *              units).
 *
 * Inputs: int x - the x coordinate of the top left
 * Output: bool - true if the x was set
 */
bool EditorMapThing::setX(int x)
{
  if(x >= 0)
  {
    thing.setTileHorizontal(x);
    return true;
  }
  return false;
}

/*
 * Description: Sets the Y coordinate of the top left of the thing (in tile
 *              units).
 *
 * Inputs: int y - the Y coordinate of the top left
 * Output: bool - true if the y was set
 */
bool EditorMapThing::setY(int y)
{
  if(y >= 0)
  {
    thing.setTileVertical(y);
    return true;
  }
  return false;
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/*
 * Description: Copy operator construction. This is called when the variable
 *              already exists and equal operator used with another
 *              EditorMapThing.
 *
 * Inputs: const EditorMapThing &source - the source class constructor
 * Output: EditorMapThing& - pointer to the copied class
 */
EditorMapThing& EditorMapThing::operator= (const EditorMapThing &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
