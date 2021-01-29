/*******************************************************************************
 * Class Name: EditorMapPerson
 * Date Created: March 3, 2015
 * Inheritance: EditorMapThing
 * Description: The class for managing the interfacing with MapPerson and
 *              filling it with data. The management pop-up is PersonDialog.
 ******************************************************************************/
#include "Database/EditorMapPerson.h"
//#include <QDebug>

/* Constant Implementation - see header file for descriptions */
const QString EditorMapPerson::kKEY_DIRECTION_EAST = "east";
const QString EditorMapPerson::kKEY_DIRECTION_NORTH = "north";
const QString EditorMapPerson::kKEY_DIRECTION_SOUTH = "south";
const QString EditorMapPerson::kKEY_DIRECTION_WEST = "west";
const QString EditorMapPerson::kKEY_SURFACE_GROUND = "ground";

/*
 * Direction enumerator to and from string key static map.
 */
const QHash<QString, core::Direction> EditorMapPerson::kDIRECTION_FROM_STRING = {
  { kKEY_DIRECTION_EAST,  core::Direction::EAST  },
  { kKEY_DIRECTION_NORTH, core::Direction::NORTH },
  { kKEY_DIRECTION_SOUTH, core::Direction::SOUTH },
  { kKEY_DIRECTION_WEST,  core::Direction::WEST  }
};
const QHash<core::Direction, QString> EditorMapPerson::kDIRECTION_TO_STRING = {
  { core::Direction::EAST,  kKEY_DIRECTION_EAST  },
  { core::Direction::NORTH, kKEY_DIRECTION_NORTH },
  { core::Direction::SOUTH, kKEY_DIRECTION_SOUTH },
  { core::Direction::WEST,  kKEY_DIRECTION_WEST  }
};

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Main constructor function. All parameters have blank defaults.
 *
 * Inputs: int id - the id of the person
 *         QString name - the name of the person
 *         QString description - the description of the person
 */
EditorMapPerson::EditorMapPerson(int id, QString name, QString description)
               : EditorMapThing(id, name, description),
                 person{core::MapPerson(id)}
{
  initializeMatrixes();

  speed_base = true;
}

/*
 * Description: Copy constructor. Calls the blank constructor and then copies
 *              the data from the source.
 *
 * Inputs: const EditorMapPerson &source - the source person to copy
 */
EditorMapPerson::EditorMapPerson(const EditorMapPerson &source)
               : EditorMapPerson()
{
  copySelf(source);
}

/*
 * Description: Destructor function
 */
EditorMapPerson::~EditorMapPerson()
{
  unsetMatrix();
  deleteMatrixes();
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: Removes the base matrixes in the person. Only called on
 *              destruction.
 *
 * Inputs: none
 * Output: none
 */
void EditorMapPerson::deleteMatrixes()
{
  for(auto it = matrix_set.begin(); it != matrix_set.end(); it++)
    delete it.value();
  matrix_set.clear();
}

/* Initialize a single matrix tied to the surface and direction dimension */
void EditorMapPerson::initializeMatrix(core::Surface surface, core::Direction direction)
{
  matrix_set.insert(core::MapPersonRenderKey(surface, direction), new EditorMatrix(1, 1));
}

/*
 * Description: Initializes the person with necessary direction and surface
 *              matrixes. Only called on creation.
 *
 * Inputs: none
 * Output: none
 */
void EditorMapPerson::initializeMatrixes()
{
  deleteMatrixes();

  initializeMatrix(core::Surface::GROUND, core::Direction::EAST);
  initializeMatrix(core::Surface::GROUND, core::Direction::NORTH);
  initializeMatrix(core::Surface::GROUND, core::Direction::SOUTH);
  initializeMatrix(core::Surface::GROUND, core::Direction::WEST);

  /* Set the initial frames in the thing */
  setMatrix(getState(core::Surface::GROUND, getStartingDirection()), true);
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: Copies all data from source editor person to this editor
 *              person.
 *
 * Inputs: EditorMapPerson &source - the source to copy from
 * Output: none
 */
void EditorMapPerson::copySelf(const EditorMapPerson &source)
{
  EditorMapThing::copySelf(source, false);

  /* Copy matrix data */
  for(auto it = matrix_set.begin(); it != matrix_set.end(); it++)
    *matrix_set[it.key()] = *source.matrix_set[it.key()];

  /* Person data */
  setSpeed(source.getSpeed());
  setStartingDirection(source.getStartingDirection());
  setUseBaseSpeed(source.isBaseSpeed());
}

/*
 * Description: Saves the data for the person. This does not include the person
 *              wrapper. Virtualized for other classes as well.
 *
 * Inputs: XmlWriter* writer - the save persistence interface
 *         bool game_only - true if only include game only data
 *         bool inc_matrix - true if include the base thing matrix
 * Output: none
 */
void EditorMapPerson::saveData(core::XmlWriter* writer, bool game_only, bool inc_matrix)
{
  EditorMapPerson default_person;

  /* First write thing data */
  EditorMapThing::saveData(writer, game_only, inc_matrix);

  /* Next person data: Is base - write core data */
  if(getBasePerson() == NULL)
  {
    /* Write the core thing data */
    if(default_person.getSpeed() != getSpeed())
      writer->writeData("speed", getSpeed());

    /* Matrix saves */
    core::Surface surface = core::Surface::GROUND;
    writer->writeElement(kKEY_SURFACE_GROUND.toStdString());

    writer->writeElement(kKEY_DIRECTION_NORTH.toStdString());
    getState(surface, core::Direction::NORTH)->save(writer, game_only, true);
    writer->jumpToParent();
    writer->writeElement(kKEY_DIRECTION_EAST.toStdString());
    getState(surface, core::Direction::EAST)->save(writer, game_only, true);
    writer->jumpToParent();
    writer->writeElement(kKEY_DIRECTION_SOUTH.toStdString());
    getState(surface, core::Direction::SOUTH)->save(writer, game_only, true);
    writer->jumpToParent();
    writer->writeElement(kKEY_DIRECTION_WEST.toStdString());
    getState(surface, core::Direction::WEST)->save(writer, game_only, true);
    writer->jumpToParent();

    writer->jumpToParent();

    /* Save the render matrix */
    getState(surface, core::Direction::NORTH)->saveRender(writer);
  }
  else
  {
    /* Speed */
    if(!isBaseSpeed())
      writer->writeData("speed", getSpeed());

    /* Starting direction */
    if(default_person.getStartingDirection() != getStartingDirection())
    {
      QString dir_str = kDIRECTION_TO_STRING.value(getStartingDirection(), "");
      writer->writeData("startingdir", dir_str.toStdString());
    }
  }
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Returns the base person. Default to NULL.
 *
 * Inputs: none
 * Output: EditorMapPerson* - the base person reference pointer
 */
EditorMapPerson* EditorMapPerson::getBasePerson() const
{
  if(getBaseThing() != NULL)
    return (EditorMapPerson*)getBaseThing();
  return NULL;
}

/*
 * Description: Returns the person classification - when dealing with casting.
 *
 * Inputs: none
 * Output: MapObjectType - the person object classification
 */
core::MapObjectType EditorMapPerson::getClass() const
{
  return core::MapObjectType::PERSON;
}

/*
 * Description: Returns the matrix from the base associated to the current
 *              direction.
 *
 * Inputs: none
 * Output: EditorMatrix* - the matrix with the given starting direction
 */
EditorMatrix* EditorMapPerson::getMatrix() const
{
  return getState(core::Surface::GROUND, getStartingDirection());
}

/*
 * Description: Returns the animation speed reference point for the movement
 *              of the person.
 *
 * Inputs: none
 * Output: uint16_t - speed ratio
 */
uint16_t EditorMapPerson::getSpeed() const
{
  uint16_t speed = 0;

  /* If instance */
  if(getBasePerson() != NULL)
  {
    if(speed_base)
      speed = getBasePerson()->getSpeed();
    else
      speed = person.getSpeed();
  }
  /* Otherwise, a base */
  else
  {
    speed = person.getSpeed();
  }

  return speed;
}

/*
 * Description: Returns the starting facing direction of the person
 *
 * Inputs: none
 * Output: Direction - the facing direction enum
 */
core::Direction EditorMapPerson::getStartingDirection() const
{
  return person.getStartingDirection();
}

/*
 * Description: Returns the matrix that is connected with the surface
 *              definition and the direction. Returns NULL if unset or
 *              invalid.
 *
 * Inputs: Surface surface - the surface that the person is on
 *         Direction direction - the direction moving in
 * Output: EditorMatrix* - matrix reference, that defines the sprite data
 */
EditorMatrix* EditorMapPerson::getState(core::Surface surface, core::Direction direction) const
{
  // Base is prioritized to fetch the matrix definitions
  EditorMapPerson* base = getBasePerson();
  if(base != nullptr)
    return base->getState(surface, direction);

  // Otherwise, check local definition
  return matrix_set.value(core::MapPersonRenderKey(surface, direction), nullptr);
}

/*
 * Description: Returns the complete 2D list of all editor matrixes, which
 *              contain the editor tile sprites.
 *
 * Inputs: none
 * Output: QList<EditorMatrix*> - stack of matrixes
 */
QList<EditorMatrix*> EditorMapPerson::getStates() const
{
  EditorMapPerson* base = getBasePerson();

  /* Check if it's a base and the frames from it should be used instead */
  if(base != NULL)
    return base->getStates();
  return matrix_set.values();
}

/*
 * Description: Checks all matrixes at the given x and y location, if the
 *              sprites within are null. Virtualized.
 *
 * Inputs: int x - the x offset from top left in matrixes
 *         int y - the y offset from top left in matrixes
 * Output: bool - true if sprites contains only null paths.
 */
bool EditorMapPerson::isAllNull(int x, int y) const
{
  bool is_null = true;

  /* Loop through all matrixes */
  for(auto it = matrix_set.begin(); it != matrix_set.end(); it++)
  {
    EditorTileSprite* sprite = it.value()->getSprite(x, y);
    if(sprite != nullptr)
      is_null &= sprite->isAllNull();
  }

  return is_null;
}

/*
 * Description: Returns if the person is using the speed from the base person
 *              class or if the speed being used is only relative to the
 *              instance.
 *
 * Inputs: none
 * Output: bool - true if the class is using the base speed
 */
bool EditorMapPerson::isBaseSpeed() const
{
  if(getBasePerson() == NULL || speed_base)
    return true;
  return false;
}

/*
 * Description: Loads the person data from the XML struct and offset index.
 *
 * Inputs: XmlData data - the XML data tree entry
 *         int index - the offset index into the struct
 * Output: none
 */
void EditorMapPerson::load(core::XmlData data, int index)
{
  QString element = QString::fromStdString(data.getElement(index));

  /* Parse elements */
  if(element == kKEY_SURFACE_GROUND)
  {
    /* Create the surface identifier */
    core::Surface surface = core::Surface::GROUND;

    /* Determine direction */
    core::Direction dir = kDIRECTION_FROM_STRING.value(
          QString::fromStdString(data.getElement(index + 1)), core::Direction::NONE);

    /* Only proceed if the direction was a valid direction */
    EditorMatrix* matrix = getState(surface, dir);
    if(matrix != nullptr && data.getElement(index + 2) == "sprites")
      matrix->load(data, index + 3);
  }
  else if(element == "rendermatrix")
  {
    for(auto it = matrix_set.begin(); it != matrix_set.end(); it++)
      it.value()->load(data, index);
  }
  else if(element == "speed")
  {
    setSpeed(data.getDataInteger());
    if(getBasePerson() != NULL)
      setUseBaseSpeed(false);
  }
  else if(element == "startingdir")
  {
    core::Direction dir = kDIRECTION_FROM_STRING.value(
          QString::fromStdString(data.getDataStringOrThrow()), core::Direction::NONE);
    if(dir != core::Direction::NONE)
      setStartingDirection(dir);
  }
  else
  {
    EditorMapThing::load(data, index);
  }
}

/*
 * Description: Saves the person data to the file handling pointer.
 *
 * Inputs: XmlWriter* writer - the save persistence interface
 *         bool game_only - true if the data should include game only relevant
 * Output: none
 */
void EditorMapPerson::save(core::XmlWriter* writer, bool game_only)
{
  if(writer != NULL)
  {
    writer->writeElement("mapperson", "id", std::to_string(getID()));
    saveData(writer, game_only, false);
    writer->jumpToParent();
  }
}

/*
 * Description: Sets the base person object for the person. It will be used for
 *              visual representation and when set, also sets the name and
 *              description (which can be changed later).
 *
 * Inputs: EditorMapPerson* person - the base person object
 *         bool synchronize - true to set name, description, and base event
 * Output: none
 */
void EditorMapPerson::setBase(EditorMapPerson* person, bool synchronize)
{
  EditorMapThing* thing = nullptr;

  if(person != nullptr)
  {
    thing = static_cast<EditorMapThing*>(person);
    if(synchronize)
    {
      setSpeed(person->getSpeed());
      setUseBaseSpeed(true);
    }
  }

  EditorMapThing::setBase(thing, synchronize);
}

/*
 * Description: Sets the speed ratio of the person. Indicates how fast between
 *              tiles to move.
 *
 * Inputs: uint16_t speed - the speed ratio
 * Output: none
 */
void EditorMapPerson::setSpeed(uint16_t speed)
{
  person.setSpeed(speed);
}

/*
 * Description: Sets the person starting facing direction. This is used on
 *              initial creation and certain inactive cases.
 *
 * Inputs: Direction direction - the starting direction facing of the person
 * Output: bool - returns if the direction was set
 */
bool EditorMapPerson::setStartingDirection(core::Direction direction)
{
  person.setStartingDirection(direction);
  return true;
}

/*
 * Description: Sets the tile icons, for rendering purposes.
 *
 * Inputs: TileIcons* icons - the rendering icon pointer. Managed by gamedb
 * Output: none
 */
void EditorMapPerson::setTileIcons(TileIcons* icons)
{
  for(auto it = matrix_set.begin(); it != matrix_set.end(); it++)
    it.value()->setTileIcons(icons);

  EditorMapThing::setTileIcons(icons);
}

/*
 * Description: Triggers if the class should use the base speed as opposed to
 *              the instance speed. Only relevant if there is a base set.
 *
 * Inputs: bool use_base - true if the base speed should be used
 * Output: none
 */
void EditorMapPerson::setUseBaseSpeed(bool use_base)
{
  speed_base = use_base;
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/*
 * Description: Copy operator construction. This is called when the variable
 *              already exists and equal operator used with another
 *              EditorMapPerson.
 *
 * Inputs: const EditorMapPerson &source - the source class constructor
 * Output: EditorMapPerson& - pointer to the copied class
 */
EditorMapPerson& EditorMapPerson::operator= (const EditorMapPerson &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
