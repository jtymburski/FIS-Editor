/*******************************************************************************
 * Class Name: EditorMatrix
 * Date Created: February 1, 2015
 * Inheritance: QGraphicsScene
 * Description: This class is for the editing sprite matrix, which contains a 
 *              scene of EditorTileSprites that are used for thing creation.
 ******************************************************************************/
// TODO: Integrate in
// Link: http://stackoverflow.com/questions/7451183/
//              how-to-create-image-file-from-qgraphicsscene-qgraphicsview
//QImage image(fn);
//QPainter painter(&image);
//painter.setRenderHint(QPainter::Antialiasing);
//scene.render(&painter);
//image.save("file_name.png")
// --
//scene->clearSelection();
//scene->setSceneRect(scene->itemsBoundingRect());
//QImage image(scene->sceneRect().size().toSize(), QImage::Format_ARGB32);
//image.fill(Qt::transparent);
//QPainter painter(&image);
//scene->render(&painter);
//image.save("file_name.png");

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
  active_sprite = NULL;

  /* Increase height and width, if both are greater than 0 */
  if(width > 0 && height > 0)
  {
    increaseWidth(width);
    increaseHeight(height - 1);
  }

  /* Set up defaults for painting visibility control */
  setCursorMode(EditorEnumDb::THING_ADD);
  setVisibilityGrid(true, true);
  setVisibilityPass(false, true);
  setVisibilityRender(false, true);
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
  
/* Adds the frames to the selected active sprite */
// TODO: Comment
void EditorMatrix::addFramesOnActive()
{
  // TODO: Implementation
}

/*
 * Description: Copies all data from source editor matrix to this editor
 *              matrix.
 *
 * Inputs: EditorMatrix &source - the source to copy from
 * Output: none
 */
void EditorMatrix::copySelf(const EditorMatrix &source)
{
  // TODO: Implementation
  /* Copy data for this class */
  //tile_sprite = source.tile_sprite;
}
  
/* Decrements the render depth on the active tile */
// TODO: Comment
bool EditorMatrix::decrementDepthOnActive()
{
  return active_sprite->decrementRenderDepth();
}

/* Increments the render depth on the active tile */
// TODO: Comment
bool EditorMatrix::incrementDepthOnActive()
{
  return active_sprite->incrementRenderDepth();
}

/* Removes the frames from the selected active sprite */
// TODO: Comment
void EditorMatrix::removeFramesOnActive()
{
  // TODO: Implementation
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/* Mouse move, press, and release events */
// TODO: Comment
void EditorMatrix::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  bool new_hover = false;

  /* Check if left the current sprite */
  if(active_sprite != NULL)
  {
    QRectF bound = active_sprite->boundingRect();
    if(!bound.contains(event->scenePos()))
    {
      active_sprite->setHover(false);
      active_sprite = NULL;
    }
  }

  /* Check which sprite it's hovering on now */
  if(active_sprite == NULL)
  {
    QGraphicsItem* hover_item = itemAt(event->scenePos(), QTransform());
    if(hover_item != NULL)
    {
      active_sprite = (EditorTileSprite*)hover_item;
      active_sprite->setHover(true);
      new_hover = true;
    }
  }

  /* If a new hover tile, check if a button is pressed and trigger click */
  if(new_hover && (event->buttons() & Qt::LeftButton))
  {
    if(cursor_mode == EditorEnumDb::THING_REMOVE)
      removeFramesOnActive();
    else if(cursor_mode == EditorEnumDb::THING_RENDER_PLUS)
      incrementDepthOnActive();
    else if(cursor_mode == EditorEnumDb::THING_RENDER_MINUS)
      decrementDepthOnActive();
  }
}

// TODO: Comment
void EditorMatrix::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  if(active_sprite != NULL)
  {
    /* If left click, utilize the cursors */
    if(event->button() == Qt::LeftButton)
    {
      if(cursor_mode == EditorEnumDb::THING_ADD)
        addFramesOnActive();
      else if(cursor_mode == EditorEnumDb::THING_REMOVE)
        removeFramesOnActive();
      else if(cursor_mode == EditorEnumDb::THING_RENDER_PLUS)
        incrementDepthOnActive();
      else if(cursor_mode == EditorEnumDb::THING_RENDER_MINUS)
        decrementDepthOnActive();
    }
    /* Otherwise, show right click menu */
    else if(event->button() == Qt::RightButton)
    {
      // TODO: Implementation
    }
  }
}

// TODO: Comment
void EditorMatrix::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  (void)event;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
  
/* Cleans the scene. Used for when adding or removing from view */
// TODO: Comment
void EditorMatrix::cleanScene()
{
  /* Clean-up the hover sprite, set by previous views */
  active_sprite = NULL;
  for(int i = 0; i < matrix.size(); i++)
    for(int j = 0; j < matrix[i].size(); j++)
      matrix[i][j]->setHover(false);
}

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

  /* Clean up the scene */
  cleanScene();
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

  /* Clean up the scene */
  cleanScene();
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
  
/* Gets the visibility for control objects */
// TODO: Comment
bool EditorMatrix::getVisibilityGrid()
{
  return visible_grid;
}

// TODO: Comment
bool EditorMatrix::getVisibilityPass()
{
  return visible_passability;
}

// TODO: Comment
bool EditorMatrix::getVisibilityRender()
{
  return visible_render;
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

  /* Update painting visibility */
  setVisibilityGrid(visible_grid, true);
  setVisibilityPass(visible_passability, true);
  setVisibilityRender(visible_render, true);
  
  /* Clean up the scene */
  cleanScene();
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

  /* Update painting visibility */
  setVisibilityGrid(visible_grid, true);
  setVisibilityPass(visible_passability, true);
  setVisibilityRender(visible_render, true);

  /* Clean up the scene */
  cleanScene();
}
  
/* Sets the cursor mode */
// TODO: Comment
void EditorMatrix::setCursorMode(EditorEnumDb::ThingCursor mode)
{
  cursor_mode = mode;
}

/* Sets the visibility for control objects */
// TODO: Comment
void EditorMatrix::setVisibilityGrid(bool visible, bool force)
{
  if(visible != visible_grid || force)
  {
    visible_grid = visible;

    /* Loop through all tile sprites and update */
    for(int i = 0; i < matrix.size(); i++)
      for(int j = 0; j < matrix[i].size(); j++)
        matrix[i][j]->setVisibilityGrid(visible);
  }
}

// TODO: Comment
void EditorMatrix::setVisibilityPass(bool visible, bool force)
{
  if(visible != visible_passability || force)
  {
    visible_passability = visible;

    /* Loop through all tile sprites and update */
    for(int i = 0; i < matrix.size(); i++)
      for(int j = 0; j < matrix[i].size(); j++)
        matrix[i][j]->setVisibilityPass(visible);
  }
}

// TODO: Comment
void EditorMatrix::setVisibilityRender(bool visible, bool force)
{
  if(visible != visible_render || force)
  {
    visible_render = visible;

    /* Loop through all tile sprites and update */
    for(int i = 0; i < matrix.size(); i++)
      for(int j = 0; j < matrix[i].size(); j++)
        matrix[i][j]->setVisibilityRender(visible);
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
