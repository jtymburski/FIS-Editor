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
const uint8_t EditorMapPerson::kTOTAL_DIRECTIONS = 4;
const uint8_t EditorMapPerson::kTOTAL_SURFACES   = 1;

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
               : EditorMapThing(id, name, description)
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
  for(uint16_t i = 0; i < matrix_set.size(); i++)
  {
    for(uint16_t j = 0; j < matrix_set[i].size(); j++)
    {
      delete matrix_set[i][j];
      matrix_set[i][j] = NULL;
    }
  }

  matrix_set.clear();
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

  for(uint8_t i = 0; i < kTOTAL_SURFACES; i++)
  {
    QList<EditorMatrix*> row;

    for(uint8_t j = 0; j < kTOTAL_DIRECTIONS; j++)
      row.push_back(new EditorMatrix(1, 1));
    matrix_set.push_back(row);
  }

  /* Set the initial frames in the thing */
  setMatrix(getState(MapPerson::SurfaceClassifier::GROUND,
                     getStartingDirection()), true);
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
  for(int i = 0; i < matrix_set.size(); i++)
    for(int j = 0; j < matrix_set[i].size(); j++)
      *matrix_set[i][j] = *source.matrix_set[i][j];

  /* Person data */
  setSpeed(source.getSpeed());
  setStartingDirection(source.getStartingDirection());
  setUseBaseSpeed(source.isBaseSpeed());
}

/*
 * Description: Saves the data for the person. This does not include the person
 *              wrapper. Virtualized for other classes as well.
 *
 * Inputs: FileHandler* fh - the file handling data pointer
 *         bool game_only - true if only include game only data
 *         bool inc_matrix - true if include the base thing matrix
 * Output: none
 */
void EditorMapPerson::saveData(FileHandler* fh, bool game_only, bool inc_matrix)
{
  EditorMapPerson default_person;

  /* First write thing data */
  EditorMapThing::saveData(fh, game_only, inc_matrix);

  /* Next person data: Is base - write core data */
  if(getBasePerson() == NULL)
  {
    /* Write the core thing data */
    if(default_person.getSpeed() != getSpeed())
      fh->writeXmlData("speed", getSpeed());

    /* Matrix saves */
    MapPerson::SurfaceClassifier surface = MapPerson::GROUND;
    fh->writeXmlElement("ground");

    fh->writeXmlElement("north");
    getState(surface, Direction::NORTH)->save(fh, game_only, true);
    fh->writeXmlElementEnd();
    fh->writeXmlElement("east");
    getState(surface, Direction::EAST)->save(fh, game_only, true);
    fh->writeXmlElementEnd();
    fh->writeXmlElement("south");
    getState(surface, Direction::SOUTH)->save(fh, game_only, true);
    fh->writeXmlElementEnd();
    fh->writeXmlElement("west");
    getState(surface, Direction::WEST)->save(fh, game_only, true);
    fh->writeXmlElementEnd();

    fh->writeXmlElementEnd();

    /* Save the render matrix */
    getState(surface, Direction::NORTH)->saveRender(fh);
  }
  else
  {
    /* Speed */
    if(!isBaseSpeed())
      fh->writeXmlData("speed", getSpeed());

    /* Starting direction */
    if(default_person.getStartingDirection() != getStartingDirection())
    {
      std::string dir_str = Helpers::directionToString(getStartingDirection());
      std::transform(dir_str.begin(),dir_str.end(),dir_str.begin(),::tolower);
      fh->writeXmlData("startingdir", dir_str);
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
 * Description: Returns the matrix from the base associated to the current
 *              direction.
 *
 * Inputs: none
 * Output: EditorMatrix* - the matrix with the given starting direction
 */
EditorMatrix* EditorMapPerson::getMatrix() const
{
  return getState(MapPerson::GROUND, getStartingDirection());
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
Direction EditorMapPerson::getStartingDirection() const
{
  return person.getStartingDirection();
}

/*
 * Description: Returns the matrix that is connected with the surface
 *              definition and the direction. Returns NULL if unset or
 *              invalid.
 *
 * Inputs: SurfaceClassifier surface - the surface that the person is on
 *         Direction direction - the direction moving in
 * Output: EditorMatrix* - matrix reference, that defines the sprite data
 */
EditorMatrix* EditorMapPerson::getState(MapPerson::SurfaceClassifier surface,
                                        Direction direction) const
{
  int surface_index = static_cast<int>(surface);
  int dir_index = MapPerson::dirToInt(direction);
  EditorMapPerson* base = getBasePerson();

  /* Check if it's a base and the frames from it should be used instead */
  if(base != NULL)
  {
    if(surface_index >= 0 && dir_index >= 0 &&
       surface_index < base->matrix_set.size() &&
       dir_index < base->matrix_set[surface_index].size())
    {
      return base->matrix_set[surface_index][dir_index];
    }
  }
  else
  {
    if(surface_index >= 0 && dir_index >= 0 &&
       surface_index < matrix_set.size() &&
       dir_index < matrix_set[surface_index].size())
    {
      return matrix_set[surface_index][dir_index];
    }
  }

  return NULL;
}

/*
 * Description: Returns the complete 2D list of all editor matrixes, which
 *              contain the editor tile sprites.
 *
 * Inputs: none
 * Output: QList<QList<EditorMatrix*>> - stack of matrixes
 */
QList<QList<EditorMatrix*>> EditorMapPerson::getStates() const
{
  EditorMapPerson* base = getBasePerson();

  /* Check if it's a base and the frames from it should be used instead */
  if(base != NULL)
    return base->matrix_set;
  return matrix_set;
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
  QList<QList<EditorMatrix*>> set = getStates();

  /* Loop through all matrixes */
  for(int i = 0; i < set.size(); i++)
  {
    for(int j = 0; j < set[i].size(); j++)
    {
      /* If matrix is valid */
      if(set[i][j] != NULL)
      {
        /* Get sprite, if valid */
        EditorTileSprite* sprite = set[i][j]->getSprite(x, y);
        if(sprite != NULL)
          is_null &= sprite->isAllNull();
      }
    }
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
 * Inputs: XmlData data - the XML data tree struct
 *         int index - the offset index into the struct
 * Output: none
 */
void EditorMapPerson::load(XmlData data, int index)
{
  QString element = QString::fromStdString(data.getElement(index));
  std::vector<std::string> elements = data.getTailElements(index);

  /* Parse elements */
  if(elements.size() >= 4 && elements[2] == "sprites")
  {
    /* Create the surface identifier */
    MapPerson::SurfaceClassifier surface = MapPerson::GROUND;
    if(elements[0] == "ground")
      surface = MapPerson::GROUND;

    /* Determine direction */
    Direction dir = Direction::DIRECTIONLESS;
    QString dir_element = QString::fromStdString(data.getElement(index+1));
    if(dir_element == "north")
      dir = Direction::NORTH;
    else if(dir_element == "east")
      dir = Direction::EAST;
    else if(dir_element == "south")
      dir = Direction::SOUTH;
    else if(dir_element == "west")
      dir = Direction::WEST;

    /* Only proceed if the direction was a valid direction */
    EditorMatrix* matrix = getState(surface, dir);
    if(matrix != NULL)
      matrix->load(data, index + 3);
  }
  else if(element == "rendermatrix")
  {
    for(int i = 0; i < matrix_set.size(); i++)
      for(int j = 0; j < matrix_set[i].size(); j++)
        matrix_set[i][j]->load(data, index);
  }
  else if(element == "speed")
  {
    setSpeed(data.getDataInteger());
    if(getBasePerson() != NULL)
      setUseBaseSpeed(false);
  }
  else if(element == "startingdir")
  {
    person.addThingInformation(data, index, -1, nullptr, "");
  }
  else
  {
    EditorMapThing::load(data, index);
  }
}

/*
 * Description: Saves the person data to the file handling pointer.
 *
 * Inputs: FileHandler* fh - the file handling pointer
 *         bool game_only - true if the data should include game only relevant
 * Output: none
 */
void EditorMapPerson::save(FileHandler* fh, bool game_only)
{
  if(fh != NULL)
  {
    fh->writeXmlElement("mapperson", "id", getID());
    saveData(fh, game_only, false);
    fh->writeXmlElementEnd();
  }
}

/*
 * Description: Sets the base person object for the person. It will be used for
 *              visual representation and when set, also sets the name and
 *              description (which can be changed later).
 *
 * Inputs: EditorMapPerson* person - the base person object
 * Output: none
 */
void EditorMapPerson::setBase(EditorMapPerson* person)
{
  EditorMapThing* thing = NULL;

  if(person != NULL)
  {
    thing = person;
    setSpeed(person->getSpeed());
    setUseBaseSpeed(true);
  }

  EditorMapThing::setBase(thing);
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
 * Inputs: Direction starting - the starting direction facing of the person
 * Output: bool - returns if the direction was set
 */
bool EditorMapPerson::setStartingDirection(Direction starting)
{
  if(person.setStartingDirection(starting))
  {
    /* Set the initial frames in the thing */
    //setMatrix(getState(MapPerson::SurfaceClassifier::GROUND,
    //                   getStartingDirection()));
    return true;
  }
  return false;
}

/*
 * Description: Sets the tile icons, for rendering purposes.
 *
 * Inputs: TileIcons* icons - the rendering icon pointer. Managed by gamedb
 * Output: none
 */
void EditorMapPerson::setTileIcons(TileIcons* icons)
{
  for(int i = 0; i < matrix_set.size(); i++)
    for(int j = 0; j < matrix_set[i].size(); j++)
      matrix_set[i][j]->setTileIcons(icons);

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
