/*******************************************************************************
 * Class Name: EditorMapPerson
 * Date Created: March 3, 2015
 * Inheritance: EditorMapThing
 * Description: The class for managing the interfacing with MapPerson and
 *              filling it with data. The management pop-up is PersonDialog.
 ******************************************************************************/
#ifndef EDITORMAPPERSON_H
#define EDITORMAPPERSON_H

#include <QHash>

#include "Database/EditorMapThing.h"
#include "Map/MapPerson.h"

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
  QHash<core::MapPersonRenderKey, EditorMatrix*> matrix_set;

  /* Reference person */
  core::MapPerson person;

  /* Map Person to use base speed */
  bool speed_base;

  /* -------------------------- Constants ------------------------- */

  /* Direction enumerator from string key map */
  const static QHash<QString, core::Direction> kDIRECTION_FROM_STRING;

  /* Direction enumerator to string key map */
  const static QHash<core::Direction, QString> kDIRECTION_TO_STRING;

  /* Data storage key names */
  const static QString kKEY_DIRECTION_EAST;
  const static QString kKEY_DIRECTION_NORTH;
  const static QString kKEY_DIRECTION_SOUTH;
  const static QString kKEY_DIRECTION_WEST;
  const static QString kKEY_SURFACE_GROUND;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Delete defines matrixes stored in class - called once at destruction */
  void deleteMatrixes();

  /* Initialize a single matrix tied to the surface and direction dimension */
  void initializeMatrix(core::Surface surface, core::Direction direction);

  /* Initialize matrixes stored in class - called once at start */
  void initializeMatrixes();

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorMapPerson &source);

  /* Saves the person data - virtualized */
  virtual void saveData(core::XmlWriter* writer, bool game_only = false,
                        bool inc_matrix = true);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Gets the base person of the person */
  EditorMapPerson* getBasePerson() const;

  /* Gets the person classification */
  virtual core::MapObjectType getClass() const;

  /* Returns the matrix associated to the current direction */
  EditorMatrix* getMatrix() const;

  /* Returns the speed of the person */
  uint16_t getSpeed() const;

  /* Returns the starting facing direction of the person */
  core::Direction getStartingDirection() const;

  /* Returns the state at the defined surface and direction */
  EditorMatrix* getState(core::Surface surface, core::Direction direction) const;
  QList<EditorMatrix*> getStates() const;

  /* Returns if the tile sprites in all matrixes at that x, y are null */
  virtual bool isAllNull(int x, int y) const;

  /* Returns if the person is using the base speed */
  bool isBaseSpeed() const;

  /* Loads the person data */
  virtual void load(core::XmlData data, int index);

  /* Saves the person data */
  virtual void save(core::XmlWriter* writer, bool game_only = false);

  /* Sets the base reference person */
  void setBase(EditorMapPerson* person, bool synchronize = true);

  /* Sets the speed of the person */
  void setSpeed(uint16_t speed);

  /* Sets the starting direction of the person */
  bool setStartingDirection(core::Direction direction);

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

/*
 * qHash() inline definitions required by QHash<?> in order to hash non-primitive types.
 * It simply attempts to return a unique hash value for unique interpretations.
 */
namespace core
{
  inline uint qHash(const Direction &direction)
  {
    return ::qHash(static_cast<uint>(direction));
  }

  inline uint qHash(const MapPersonRenderKey &key)
  {
    uint h1 = qHash(key.getDirection());
    uint h2 = ::qHash(static_cast<uint>(key.getSurface()));
    return h1 ^ (h2 << 4);
  }
}

#endif // EDITORMAPPERSON_H
