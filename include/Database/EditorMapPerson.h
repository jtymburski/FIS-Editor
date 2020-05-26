/*******************************************************************************
 * Class Name: EditorMapPerson
 * Date Created: March 3, 2015
 * Inheritance: EditorMapThing
 * Description: The class for managing the interfacing with MapPerson and
 *              filling it with data. The management pop-up is PersonDialog.
 ******************************************************************************/
#ifndef EDITORMAPPERSON_H
#define EDITORMAPPERSON_H

#include "Database/EditorMapThing.h"
#include "Game/Map/MapPerson.h"

class EditorMapPerson : public EditorMapThing
{
public:
  /* Constructor function */
  EditorMapPerson(int id = -1, QString name = "Default Name",
                  QString description = "");

  /* Copy constructor function */
  EditorMapPerson(const EditorMapPerson &source);

  /* Destructor function */
  virtual ~EditorMapPerson();

private:
  /* The set of thing matrixes for different grounds and directions */
  QList<QList<EditorMatrix*>> matrix_set;

  /* Reference person */
  MapPerson person;

  /* Map Person to use base speed */
  bool speed_base;

  /* -------------------------- Constants ------------------------- */
  const static uint8_t kTOTAL_DIRECTIONS; /* The max # of directions to move */
  const static uint8_t kTOTAL_SURFACES; /* The max # of surfaces to walk on */

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Delete defines matrixes stored in class - called once at destruction */
  void deleteMatrixes();

  /* Initialize matrixes stored in class - called once at start */
  void initializeMatrixes();

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorMapPerson &source);

  /* Saves the person data - virtualized */
  virtual void saveData(FileHandler* fh, bool game_only = false,
                        bool inc_matrix = true);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Gets the base person of the person */
  EditorMapPerson* getBasePerson() const;

  /* Gets the person classification */
  virtual ThingBase getClass() const;

  /* Returns the matrix associated to the current direction */
  EditorMatrix* getMatrix() const;

  /* Returns the speed of the person */
  uint16_t getSpeed() const;

  /* Returns the starting facing direction of the person */
  Direction getStartingDirection() const;

  /* Returns the state at the defined surface and direction */
  EditorMatrix* getState(MapPerson::SurfaceClassifier surface,
                         Direction direction) const;
  QList<QList<EditorMatrix*>> getStates() const;

  /* Returns if the tile sprites in all matrixes at that x, y are null */
  virtual bool isAllNull(int x, int y) const;

  /* Returns if the person is using the base speed */
  bool isBaseSpeed() const;

  /* Loads the person data */
  virtual void load(XmlData data, int index);

  /* Saves the person data */
  virtual void save(FileHandler* fh, bool game_only = false);

  /* Sets the base reference person */
  void setBase(EditorMapPerson* person, bool synchronize = true);

  /* Sets the speed of the person */
  void setSpeed(uint16_t speed);

  /* Sets the starting direction of the person */
  bool setStartingDirection(Direction starting);

  /* Sets the rendering tile icons */
  void setTileIcons(TileIcons* icons);

  /* Sets the class to use the base speed */
  void setUseBaseSpeed(bool use_base);

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  /* The copy operator */
  EditorMapPerson& operator= (const EditorMapPerson &source);
};

#endif // EDITORMAPPERSON_H
