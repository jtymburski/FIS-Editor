/*******************************************************************************
 * Class Name: EditorMatrix
 * Date Created: February 1, 2015
 * Inheritance: QGraphicsScene
 * Description: This class is for the editing sprite matrix, which contains a 
 *              scene of EditorTileSprites that are used for thing creation.
 ******************************************************************************/
#include "Database/EditorMatrix.h"
#include <QDebug>

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
  place_x = 0;
  place_y = 0;

  /* Increase height and width, if both are greater than 0 */
  if(width > 0 && height > 0)
  {
    increaseWidth(width);
    increaseHeight(height - 1);
  }

  /* Set up defaults for painting visibility control */
  setActiveFrame(0, true);
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
  /* Get the place X and Y, from the active sprite */
  place_x = active_sprite->getX();
  place_y = active_sprite->getY();
 
  /* Trigger the start matrix place in parent class */
  emit initMatrixPlace();
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
  active_sprite->deleteAllFrames();
}
  
/* Size manipulation on matrix */
// TODO: Comment
bool EditorMatrix::setNewOrigin(int x, int y)
{
  if(x >= 0 && y >= 0)
  {
    /* Trim the width first */
    for(int i = 0; i < x; i++)
    {
      for(int j = 0; j < matrix.first().size(); j++)
      {
        removeItem(matrix.first()[j]);
        delete matrix.first()[j];
      }
      matrix.removeFirst();
    }
    
    /* Trim the height next */
    for(int i = 0; i < matrix.size(); i++)
    {
      for(int j = 0; j < y; j++)
      {
        removeItem(matrix[i].first());
        delete matrix[i].first();
        matrix[i].removeFirst();
      }
    }

    /* Re-base matrix */
    for(int i = 0; i < matrix.size(); i++)
    {
      for(int j = 0; j < matrix[i].size(); j++)
      {
        matrix[i][j]->setX(i);
        matrix[i][j]->setY(j);
      }
    }

    return true;
  }
  return false;
}

/* Size manipulation on matrix */
// TODO: Comment
bool EditorMatrix::setNewSize(int width, int height)
{
  if(width >= 0 && height >= 0)
  {
    /* Fix the width */
    if(width < getWidth())
      decreaseWidth(getWidth() - width);

    /* Fix the height */
    if(height < getHeight())
      decreaseHeight(getHeight() - height);

    return true;
  }
  return false;
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
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

/* Matrix place sprite trigger */
void EditorMatrix::matrixPlace(QString result_path)
{
  if(!result_path.isEmpty())
    addPath(result_path, place_x, place_y);
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
   
/* Adds a path to the Editor Matrix. It can handle matrix format file names */
// TODO: Comment
bool EditorMatrix::addPath(QString path, int x, int y, bool hflip, 
                           bool vflip, bool reset)
{
  if(!path.isEmpty())
  {
    QStringList path_set = path.split(QDir::separator());
    return addPath(path.remove(path_set.last()), path_set.last(), x, y, 
                   hflip, vflip, reset);
  }

  return false;
}

// TODO: Comment
bool EditorMatrix::addPath(QString root_path, QString file_name, 
                           int x, int y, bool hflip, bool vflip, bool reset)
{
  if(!root_path.isEmpty() && !file_name.isEmpty() && x >= 0 && y >= 0)
  {
    /* If reset, clear the scene */
    if(reset)
      decreaseWidth(getWidth());

    /* Split up the set of frames for the matrix */
    std::vector<std::vector<std::string>> name_set = 
                               Helpers::frameSeparator(file_name.toStdString());

    /* Ensure the matrix is large enough */
    if((x + (int)name_set.size()) > getWidth())
      increaseWidth(x + name_set.size() - getWidth());
    if((y + (int)name_set.front().size()) > getHeight())
      increaseHeight(y + name_set.front().size() - getHeight());

    /* Go through paths and add them */
    for(uint16_t i = 0; i < name_set.size(); i++)
    {
      for(uint16_t j = 0; j < name_set[i].size(); j++)
      {
        matrix[x + i][y + j]->addPath(root_path + 
                                      QString::fromStdString(name_set[i][j]));
        matrix[x + i][y + j]->setHorizontalFlips(hflip);
        matrix[x + i][y + j]->setVerticalFlips(vflip);
      }
    }
  }

  return false;
}

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
  
/* Returns the active frame index */
// TODO: Comment
int EditorMatrix::getActiveFrameIndex()
{
  return active_frame;
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
   
/* Gets trim values - rows, columns, and frames - that are unused */
// TODO: Comment
int EditorMatrix::getTrimFrames(bool largest)
{
  int ref_index = -1;

  for(int i = 0; i < matrix.size(); i++)
  {
    for(int j = 0; j < matrix[i].size(); j++)
    {
      int result = matrix[i][j]->getLastValidFrame();
      if(largest)
      {
        if(result > ref_index)
          ref_index = result;
      }
      else
      {
        if(ref_index == -1)
          ref_index = result;
        else if(result >= 0 && result < ref_index)
          ref_index = result;
      }
    }
  }

  return ref_index;
}

// TODO: Comment
QRect EditorMatrix::getTrimRect()
{
  QVector<QVector<int>> matrix_null;

  /* Parse the entire matrix */
  for(int i = 0; i < matrix.size(); i++)
  {
    QVector<int> col;

    for(int j = 0; j < matrix[i].size(); j++)
    {
      if(matrix[i][j]->isAllNull())
        col.push_back(0);
      else
        col.push_back(1);
    }

    matrix_null.push_back(col);
  }

  /* Set up the initial rect to the size of the matrix */
  int x1 = 0;
  int x2 = getWidth();
  int y1 = 0;
  int y2 = getHeight();

  /* Parse the left columns to see if optimizations can be made */
  bool valid = true;
  for(int i = x1; valid && (i < x2); i++)
  {
    bool all_null = true;

    for(int j = y1; j < y2; j++)
      if(matrix_null[i][j] == 1)
        all_null = false;

    if(all_null)
      x1 = i + 1;
    else
      valid = false;
  }

  /* Parse the right columns to see if optimizations can be made */
  valid = true;
  for(int i = x2 - 1; i >= x1; i--)
  {
    bool all_null = true;

    for(int j = y1; j < y2; j++)
      if(matrix_null[i][j] == 1)
        all_null = false;

    if(all_null)
      x2 = i;
    else
      valid = false;
  }

  /* Parse the top rows to see if optimizations can be made */
  valid = true;
  for(int i = y1; valid && (i < y2); i++)
  {
    bool all_null = true;

    for(int j = x1; j < x2; j++)
      if(matrix_null[j][i] == 1)
        all_null = false;

    if(all_null)
      y1 = i + 1;
    else
      valid = false;
  }

  /* Parse the bottom rows to see if optimizations can be made */
  valid = true;
  for(int i = y2 - 1; i >= y1; i--)
  {
    bool all_null = true;

    for(int j = x1; j < x2; j++)
      if(matrix_null[j][i] == 1)
        all_null = false;

    if(all_null)
      y2 = i;
    else
      valid = false;
  }

  /* Construct rect */
  return QRect(x1, y1, x2 - x1, y2 - y1);
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
  setActiveFrame(active_frame, true);
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
  setActiveFrame(active_frame, true);
  setVisibilityGrid(visible_grid, true);
  setVisibilityPass(visible_passability, true);
  setVisibilityRender(visible_render, true);

  /* Clean up the scene */
  cleanScene();
}
   
/* Sets the active frame for all sprites in the matrix */
// TODO: Comment
void EditorMatrix::setActiveFrame(int index, bool force)
{
  if(index >= 0 && (index != active_frame || force))
  {
    active_frame = index;

    /* Loop through all tile sprites and update */
    for(int i = 0; i < matrix.size(); i++)
      for(int j = 0; j < matrix[i].size(); j++)
        matrix[i][j]->setActiveFrame(index);
  }
}

/* Sets the cursor mode */
// TODO: Comment
void EditorMatrix::setCursorMode(EditorEnumDb::ThingCursor mode)
{
  cursor_mode = mode;
}
  
/* Sets if all tiles should be horizontally or vertically flipped */
// TODO: Comment
void EditorMatrix::setFlipHorizontal(bool flip)
{
  for(int i = 0; i < matrix.size(); i++)
    for(int j = 0; j < matrix[i].size(); j++)
      matrix[i][j]->setHorizontalFlips(flip);
}

// TODO: Comment
void EditorMatrix::setFlipVertical(bool flip)
{
  for(int i = 0; i < matrix.size(); i++)
    for(int j = 0; j < matrix[i].size(); j++)
      matrix[i][j]->setVerticalFlips(flip);
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

/* Trims the scene  (removes excess sprites and frames) */
// TODO: Comment
void EditorMatrix::trim(bool only_se)
{
  /* Clean the scene */
  cleanScene();

  /* Get trimmed rect */
  QRect trim_rect = getTrimRect();

  /* Update width / height */
  setNewSize(trim_rect.x() + trim_rect.width(), 
             trim_rect.y() + trim_rect.height());

  /* Update origin points */
  if(!only_se)
    setNewOrigin(trim_rect.x(), trim_rect.y());

  /* Update number of frames */
  int frame_count = getTrimFrames(false) + 1;
  for(int i = 0; i < matrix.size(); i++)
    for(int j = 0; j < matrix[i].size(); j++)
      matrix[i][j]->setMaximumFrames(frame_count);
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
