/*******************************************************************************
 * Class Name: EditorMatrix
 * Date Created: February 1, 2015
 * Inheritance: QGraphicsScene
 * Description: This class is for the editing sprite matrix, which contains a 
 *              scene of EditorTileSprites that are used for thing creation.
 ******************************************************************************/
#include "Database/EditorMatrix.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function for creation. Can be passed a width and
 *              height for the initial size of the scene on creation.
 *
 * Inputs: int width - the width of the scene, in tiles
 *         int height - the height of the scene, in tiles
 */
EditorMatrix::EditorMatrix(int width, int height) : QGraphicsScene()
{
  if(width > 0 && height > 0)
  {
    increaseWidth(width);
    increaseHeight(height - 1);
  }
}

/*
 * Description: Constructor function for copying an existing editor matrix.
 *
 * Inputs: const EditorMatrix &source - the source matrix to copy
 */
EditorMatrix::EditorMatrix(const EditorMatrix &source) : EditorMatrix()
{
  copySelf(source);
}

/*
 * Description: Destructor function
 */
EditorMatrix::~EditorMatrix()
{
  /* Clear the scene */
  QList<QGraphicsItem*> item_list = items();
  for(int i = 0; i < item_list.size(); i++)
    removeItem(item_list[i]);

  /* Delete all the stored editor tile sprites */
  for(int i = 0; i < matrix.size(); i++)
    for(int j = 0; j < matrix[i].size(); j++)
      delete matrix[i][j];
  matrix.clear();
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: Copies all data from source editor matrix to this editor
 *              matrix.
 *
 * Inputs: EditorMatrix &source - the source to copy from
 * Output: none
 */
// TODO
void EditorMatrix::copySelf(const EditorMatrix &source)
{
  /* Copy data for this class */
  //tile_sprite = source.tile_sprite;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Decreases the height of the matrix in the scene, by a count
 *              factor. If count is greater than the size of the matrix, it
 *              will clear the matrix.
 *
 * Inputs: int count - the number of vertical tiles to remove
 * Output: none
 */
void EditorMatrix::decreaseHeight(int count)
{
  /* Deletion limit control */
  bool clear_array = false;
  int max = count;
  if(matrix.size() > 0 && max > matrix.front().size())
  {
    max = matrix.front().size();
    clear_array = true;
  }

  /* Loop through and trim all columns */
  for(int i = 0; i < matrix.size(); i++)
  {
    for(int j = 0; j < max; j++)
    {
      removeItem(matrix[i].last());
      delete matrix[i].last();
      matrix[i].removeLast();
    }
  }

  /* If wiping out all columns, clear matrix */
  if(clear_array)
    matrix.clear();
}

/*
 * Description: Decreases the width of the matrix in the scene, by a count
 *              factor. If count is greater than the size of the matrix, it
 *              will clear the matrix.
 *
 * Inputs: int count - the number of horizontal tiles to remove
 * Output: none
 */
void EditorMatrix::decreaseWidth(int count)
{
  /* Deletion limit control */
  bool clear_array = false;
  int max = count;
  if(max > matrix.size())
  {
    max = matrix.size();
    clear_array = true;
  }

  /* Loop through all rows or up to count */
  for(int i = 0; i < max; i++)
  {
    for(int j = 0; j < matrix.last().size(); j++)
    {
      removeItem(matrix.last()[j]);
      delete matrix.last()[j];
      matrix.last()[j] = NULL;
    }
    matrix.removeLast();
  }

  /* If wiping out all rows, clear matrix */
  if(clear_array)
    matrix.clear();
}

/*
 * Description: Returns the height of the matrix, in tiles.
 *
 * Inputs: none
 * Output: int - height, in tile count
 */
int EditorMatrix::getHeight()
{
  if(getWidth() > 0)
    return matrix.last().size();
  return 0;
}

/*
 * Description: Returns the width of the matrix, in tiles.
 *
 * Inputs: none
 * Output: int - width, in tile count
 */
int EditorMatrix::getWidth()
{
  return matrix.size();
}

/*
 * Description: Increases the height of the matrix in the scene, by a count
 *              factor. If the width is currently 0, it will become one after
 *              this call.
 *
 * Inputs: int count - the number of vertical tiles to add
 * Output: none
 */
void EditorMatrix::increaseHeight(int count)
{
  /* If width is 0, add one column so the height can be increased */
  if(getWidth() == 0)
  {
    QVector<EditorTileSprite*> col;
    for(int i = 0; i < count; i++)
    {
      col.push_back(new EditorTileSprite(0, i));
      addItem(col.last());
    }
    matrix.push_back(col);
  }
  /* Otherwise, just add to each column */
  else
  {
    for(int i = 0; i < matrix.size(); i++)
    {
      int height = matrix[i].size();

      for(int j = 0; j < count; j++)
      {
        matrix[i].push_back(new EditorTileSprite(i, height + j));
        addItem(matrix[i].last());
      }
    }
  }
}

/*
 * Description: Increases the width of the matrix in the scene, by a count
 *              factor. If the height is currently 0, it will become one after
 *              this call.
 *
 * Inputs: int count - the number of horizontal tiles to remove
 * Output: none
 */
void EditorMatrix::increaseWidth(int count)
{
  /* Get the height. If not greater than 0, make 1 */
  int height = getHeight();
  if(height <= 0)
    height = 1;

  /* Go through and add to the width */
  for(int i = 0; i < count; i++)
  {
    /* Create the column of correct height */
    QVector<EditorTileSprite*> col;
    int width = getWidth();
    for(int j = 0; j < height; j++)
    {
      col.push_back(new EditorTileSprite(width, j));
      addItem(col.last());
    }

    /* Put the column in the matrix */
    matrix.push_back(col);
  }
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/*
 * Description: Copy operator construction. This is called when the variable
 *              already exists and equal operator used with another object.
 *
 * Inputs: const EditorMatrix &source - the source class constructor
 * Output: EditorTile& - reference to the copied class
 */
EditorMatrix& EditorMatrix::operator= (const EditorMatrix &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
