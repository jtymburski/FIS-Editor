/*******************************************************************************
 * Class Name: EditorMapPerson
 * Date Created: March 3, 2015
 * Inheritance: EditorMapThing
 * Description: The class for managing the interfacing with MapPerson and 
 *              filling it with data. The management pop-up is PersonDialog.
 ******************************************************************************/
#include "Database/EditorEvent.h"
#include "Database/EditorMapPerson.h"
#include <QDebug>

/* Constant Implementation - see header file for descriptions */
const uint8_t EditorMapPerson::kTOTAL_DIRECTIONS = 4;
const uint8_t EditorMapPerson::kTOTAL_SURFACES   = 1;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/
 
/* Constructor function */
EditorMapPerson::EditorMapPerson(int id, QString name, QString description)
               : EditorMapThing(id, name, description)
{
  initializeMatrixes();
}
                
/* Copy constructor function */
EditorMapPerson::EditorMapPerson(const EditorMapPerson &source)
               : EditorMapPerson()
{
  copySelf(source);
}
  
/* Destructor function */
EditorMapPerson::~EditorMapPerson()
{
  unsetMatrix();
  deleteMatrixes();
}
  
/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* Delete defines matrixes stored in class - called once at destruction */
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

/* Initialize matrixes stored in class - called once at start */
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
  setMatrix(getState(MapPerson::SurfaceClassifier::GROUND, Direction::SOUTH));
}
  
/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/* Copy function, to be called by a copy or equal operator constructor */
void EditorMapPerson::copySelf(const EditorMapPerson &source)
{
  EditorMapThing::copySelf(source, false);

  /* Copy matrix data */
  for(int i = 0; i < matrix_set.size(); i++)
    for(int j = 0; j < matrix_set[i].size(); j++)
      *matrix_set[i][j] = *source.matrix_set[i][j];

  /* Person data */
  setSpeed(source.getSpeed());
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Gets the base person of the person */
EditorMapPerson* EditorMapPerson::getBasePerson() const
{
  if(getBaseThing() != NULL)
    return (EditorMapPerson*)getBaseThing();
  return NULL;
}

/* Returns the speed of the person */
uint16_t EditorMapPerson::getSpeed() const
{
  return person.getSpeed();
}

/* Returns the state at the defined surface and direction */
EditorMatrix* EditorMapPerson::getState(MapPerson::SurfaceClassifier surface,
                                        Direction direction)
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

/* Returns all the matrix for all states */
QList<QList<EditorMatrix*>> EditorMapPerson::getStates()
{
  return matrix_set;
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
  EditorMapPerson default_person;

  if(fh != NULL)
  {
    fh->writeXmlElement("mapperson", "id", getID());

    /* Depending on if base or not, write different data */
    EditorMapPerson* base = getBasePerson();
    if(base != NULL)
    {
      /* Write base settings */
      fh->writeXmlData("base", base->getID());
      QString startpoint = QString::number(getX()) + "," +
                           QString::number(getY());
      fh->writeXmlData("startpoint", startpoint.toStdString());

      /* Check the name and description, if it's different from base */
      if(base->getName() != getName())
        fh->writeXmlData("name", getName().toStdString());
      if(base->getDescription() != getDescription())
        fh->writeXmlData("description", getDescription().toStdString());
    }
    else
    {
      /* Write the core thing data */
      fh->writeXmlData("name", getName().toStdString());
      fh->writeXmlData("description", getDescription().toStdString());
      if(default_person.isVisible() != isVisible())
        fh->writeXmlData("visible", isVisible());
      if(default_person.getSpeed() != getSpeed())
        fh->writeXmlData("speed", getSpeed());

      /* Save the dialog image */
      EditorSprite* dialog_image = getDialogImage();
      if(!dialog_image->isAllNull() && dialog_image->frameCount() == 1)
        fh->writeXmlData("image",
               EditorHelpers::trimPath(dialog_image->getPath(0)).toStdString());

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

      /* Event save */
      EditorEvent edit_event(getEvent());
      edit_event.save(fh, game_only);
    }

    fh->writeXmlElementEnd();
  }
}

/* Sets the base reference thing */
void EditorMapPerson::setBase(EditorMapPerson* person)
{
  EditorMapThing* thing = NULL;

  if(person != NULL)
    thing = person;

  EditorMapThing::setBase(thing);
}

/* Sets the speed of the person */
void EditorMapPerson::setSpeed(uint16_t speed)
{
  person.setSpeed(speed);
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

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/* The copy operator */
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
