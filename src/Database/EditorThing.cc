/*******************************************************************************
 * Class Name: EditorThing
 * Date Created: February 7, 2015
 * Inheritance: EditorTemplate
 * Description: The class for managing the interfacing with MapThing and filling
 *              it with data. The management pop-up is ThingDialog.
 ******************************************************************************/
#include "Database/EditorThing.h"
#include <QDebug>

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
EditorThing::EditorThing(int id, QString name, QString description)
{
  matrix = new EditorMatrix(1, 1, false);//true);

  /* Load data */
  setID(id);
  setName(name);
  setDescription(description);
}

/*
 * Description: Copy constructor. Calls the blank constructor and then copies
 *              the data from the source.
 *
 * Inputs: const EditorThing &source - the source thing to copy
 */
EditorThing::EditorThing(const EditorThing &source) : EditorThing()
{
  copySelf(source);
}

/*
 * Description: Destructor function
 */
EditorThing::~EditorThing()
{
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
 * Inputs: EditorThing &source - the source to copy from
 * Output: none
 */
void EditorThing::copySelf(const EditorThing &source)
{
  thing = source.thing;
  *matrix = *source.matrix;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
   
/*
 * Description: Returns the editor description string.
 *
 * Inputs: none
 * Output: QString - the thing description
 */
QString EditorThing::getDescription()
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
EditorSprite* EditorThing::getDialogImage()
{
  /* Make sure only one frame is in the dialog image */
  while(dialog_image.frameCount() > 1)
    dialog_image.deleteFrame(dialog_image.frameCount() - 1);

  return &dialog_image;
}

/*
 * Description: Returns the editor thing ID
 *
 * Inputs: none
 * Output: int - the id of the editor thing
 */
int EditorThing::getID()
{
  return thing.getID();
}

/*
 * Description: The matrix in the thing that contains the sprite set.
 *
 * Inputs: none
 * Output: EditorMatrix* - the matrix sprite set
 */
EditorMatrix* EditorThing::getMatrix()
{
  return matrix;
}

/*
 * Description: Returns the editor thing name
 *
 * Inputs: none
 * Output: The name of the editor thing
 */
QString EditorThing::getName()
{
  return QString::fromStdString(thing.getName());
}

/*
 * Description: Returns the formatted name and ID for listing. It's in the
 *              format: "XXX: sssssssssssss"
 *
 * Inputs: none
 * Output: QString - the name for a list
 */
QString EditorThing::getNameList()
{
  return EditorHelpers::getListString(getID(), getName());
}

/*
 * Description: Returns if the thing is visible when displayed in game.
 *
 * Inputs: none
 * Output: bool - true if visible when the game is running
 */
bool EditorThing::isVisible()
{
  return thing.isVisible();
}

/*
 * Description: Sets the description of the thing.
 *
 * Inputs: QString description - the description text
 * Output: none
 */
void EditorThing::setDescription(QString description)
{
  thing.setDescription(description.toStdString());
}

/*
 * Description: Sets the dialog image, based on the path, for the thing.
 *
 * Inputs: QString path - the path for the dialog image
 * Output: none
 */
void EditorThing::setDialogImage(QString path)
{
  /* Make sure the image is empty and ready for the new path */
  while(dialog_image.frameCount() > 0)
    dialog_image.deleteFrame(dialog_image.frameCount() - 1);

  /* Set the new path to the base index */
  dialog_image.setPath(0, path);
}

/*
 * Description: Sets the map thing ID in the base class.
 *
 * Inputs: int id - the thing id
 * Output: none
 */
void EditorThing::setID(int id)
{
  if(id >= 0)
    thing.setID(id);
}

/*
 * Description: Sets the name of the thing.
 *
 * Inputs: QString name - the name text
 * Output: none
 */
void EditorThing::setName(QString name)
{
  thing.setName(name.toStdString());
}

/*
 * Description: Sets the tile icons, for rendering purposes.
 *
 * Inputs: TileIcons* icons - the rendering icon pointer. Managed by gamedb
 * Output: none
 */
void EditorThing::setTileIcons(TileIcons* icons)
{
  tile_icons = icons;
  if(matrix != NULL)
    matrix->setTileIcons(tile_icons);
}

/*
 * Description: Sets the thing visibility when rendering in the game.
 *
 * Inputs: bool visible - true if it should be visible
 * Output: none
 */
void EditorThing::setVisibility(bool visible)
{
  thing.setVisibility(visible);
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
  
/*
 * Description: Copy operator construction. This is called when the variable
 *              already exists and equal operator used with another
 *              EditorThing.
 *
 * Inputs: const EditorThing &source - the source class constructor
 * Output: EditorThing& - pointer to the copied class
 */
EditorThing& EditorThing::operator= (const EditorThing &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
