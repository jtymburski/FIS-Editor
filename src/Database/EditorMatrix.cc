/*******************************************************************************
 * Class Name: EditorMatrix
 * Date Created: February 1, 2015
 * Inheritance: QGraphicsScene
 * Description: This class is for the editing sprite matrix, which contains a
 *              scene of EditorTileSprites that are used for thing creation.
 ******************************************************************************/
#include "Database/EditorMatrix.h"
//#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function for creation. Can be passed a width and
 *              height for the initial size of the scene on creation.
 *
 * Inputs: int width - the width of the scene, in tiles
 *         int height - the height of the scene, in tiles
 *         bool black_back - if true, set the background to black
 */
EditorMatrix::EditorMatrix(int width, int height, bool black_back)
            : QGraphicsScene()
{
  active_sprite = NULL;
  edit_all = NULL;
  place_x = -1;
  place_y = -1;
  rightclick_sprite = NULL;
  tile_icons = NULL;

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

  /* Sets the background to be black */
  if(black_back)
    setBackgroundBrush(QBrush(Qt::black));
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
  /* Delete the edit all sprite, if it exists */
  if(edit_all != NULL)
    delete edit_all;
  edit_all = NULL;

  /* Remove all tile sprites */
  removeAll();
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: Triggers the add frames to the active sprite. This emits
 *              initMatrixPlace() to be connected by parent.
 *
 * Inputs: none
 * Output: none
 */
void EditorMatrix::addFramesOnActive()
{
  /* Get the place X and Y, from the active sprite */
  place_x = active_sprite->getX();
  place_y = active_sprite->getY();

  /* Trigger the start matrix place in parent class */
  emit initMatrixPlace();
}

/*
 * Description: Changes the passability in all directions on the active sprite
 *              tile to the passed in value
 *
 * Inputs: bool passable - should the tile be passable?
 * Output: none
 */
void EditorMatrix::changePassOnActive(bool passable)
{
  active_sprite->setPassability(passable);
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
  /* Clean the scene first */
  removeAll();

  if(source.getWidth() > 0 && source.getHeight() > 0)
  {
    /* Make the matrix to size */
    increaseWidth(source.getWidth());
    increaseHeight(source.getHeight() - getHeight());

    /* Make the matrixes equivalent */
    for(int i = 0; i < matrix.size(); i++)
      for(int j = 0; j < matrix[i].size(); j++)
        *matrix[i][j] = *source.matrix[i][j];

    emit matrixChange();
  }
}

/*
 * Description: Decreases the render depth on the active tile by 1.
 *
 * Inputs: bool min - push it to the min as opposed to decrement by 1
 * Output: bool - true if the depth decreased by 1. false if as low as possible
 */
bool EditorMatrix::decrementDepthOnActive(bool min)
{
  if(min)
  {
    if(!active_sprite->decrementRenderDepth())
    {
      incrementDepthOnActive(true);
      return true;
    }
    else
    {
      while(active_sprite->decrementRenderDepth());
      return false;
    }
  }

  return active_sprite->decrementRenderDepth();
}

/*
 * Description: Returns the first valid sprite. It attempts to find one with
 *              frames first. If there are none, returns first non-NULL sprite.
 *              Otherwise, NULL.
 *
 * Inputs: none
 * Output: EditorTileSprite* - the first valid sprite. NULL if not found
 */
EditorTileSprite* EditorMatrix::getValidSprite()
{
  EditorTileSprite* valid_sprite = NULL;

  /* Loop through all first and try to find one with frames */
  for(int i = 0; (valid_sprite == NULL) && (i < matrix.size()); i++)
    for(int j = 0; (valid_sprite == NULL) && (j < matrix[i].size()); j++)
      if(!matrix[i][j]->isAllNull())
        valid_sprite = matrix[i][j];

  /* If not, get the first valid sprite (non-NULL) */
  if(valid_sprite == NULL)
    if(matrix.size() > 0 && matrix.front().size() > 0)
      valid_sprite = matrix.front().front();

  return valid_sprite;
}

/*
 * Description: Increases the render depth on the active tile by 1.
 *
 * Inputs: bool max - push it to the max as opposed to increment by 1
 * Output: bool - true if the depth increased by 1. false if as high as possible
 */
bool EditorMatrix::incrementDepthOnActive(bool max)
{
  if(max)
  {
    if(!active_sprite->incrementRenderDepth())
    {
      decrementDepthOnActive(true);
      return true;
    }
    else
    {
      while(active_sprite->incrementRenderDepth());
      return false;
    }
  }

  return active_sprite->incrementRenderDepth();
}

/*
 * Description: Updates the mouse location on the scene for hover and click
 *              events using a point, the cursor point.
 *
 * Inputs: QPointF point - the mouse location
 * Output: bool - true if the active sprite has changed
 */
bool EditorMatrix::mouseUpdate(QPointF point)
{
  bool new_hover = false;

  /* Check if left the current sprite */
  if(active_sprite != NULL)
  {
    QRectF bound = active_sprite->boundingRect();
    if(!bound.contains(point))
    {
      active_sprite->setHover(false);
      active_sprite = NULL;
    }
  }

  /* Check which sprite it's hovering on now */
  if(active_sprite == NULL)
  {
    QGraphicsItem* hover_item = itemAt(point, QTransform());
    if(hover_item != NULL)
    {
      active_sprite = (EditorTileSprite*)hover_item;
      active_sprite->setHover(true);
      new_hover = true;
    }
  }

  return new_hover;
}

/*
 * Description: Removes all the frames in the sprite on the active hovered one.
 *
 * Inputs: none
 * Output: none
 */
void EditorMatrix::removeFramesOnActive()
{
  active_sprite->deleteAllFrames();
  active_sprite->update();
  emit matrixChange();
}

/*
 * Description: Sets a new origin. This involves removes rows on the top and
 *              columns on the left to rebase the matrix.
 *
 * Inputs: int x - the new x of tile for origin
 *         int y - the new y of tile for origin
 * Output: bool - true if changed
 */
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

/*
 * Description: Sets a new width and height for the matrix. This adds/removes
 *              rows to the bottom and columns to the right.
 *
 * Inputs: int width - the new width of the matrix, in tiles
 *         int height - the new height of the matrix, in tiles
 * Output: bool - true if size adjusted
 */
bool EditorMatrix::setNewSize(int width, int height)
{
  if(width >= 0 && height >= 0)
  {
    /* Fix the width */
    if(width < getWidth())
      decreaseWidth(getWidth() - width);
    else if(width > getWidth())
      increaseWidth(width - getWidth());

    /* Fix the height */
    if(height < getHeight())
      decreaseHeight(getHeight() - height);
    else if(height > getHeight())
      increaseHeight(height - getHeight());

    return true;
  }
  return false;
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: The mouse move event. This re-implemented off the base
 *              QGraphicsScene and handles all hover events and pen control.
 *
 * Inputs: QGraphicsSceneMouseEvent* event - the mouse move event
 * Output: none
 */
void EditorMatrix::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  /* Update mouse location */
  bool new_hover = mouseUpdate(event->scenePos());

  /* If a new hover tile, check if a button is pressed and trigger click */
  if(new_hover)
  {
    if(event->buttons() & Qt::LeftButton)
    {
      if(cursor_mode == EditorEnumDb::THING_REMOVE)
        removeFramesOnActive();
      else if(cursor_mode == EditorEnumDb::THING_RENDER_PLUS)
        incrementDepthOnActive();
      else if(cursor_mode == EditorEnumDb::THING_RENDER_MINUS)
        decrementDepthOnActive();
      else if(cursor_mode == EditorEnumDb::THING_PASS_ALL)
        changePassOnActive(true);
    }
    else if(event->buttons() & Qt::RightButton)
    {
      if(cursor_mode == EditorEnumDb::THING_PASS_ALL)
        changePassOnActive(false);
      else if(cursor_mode == EditorEnumDb::THING_RENDER_PLUS)
        incrementDepthOnActive(true);
      else if(cursor_mode == EditorEnumDb::THING_RENDER_MINUS)
        decrementDepthOnActive(true);
    }
  }
}

/*
 * Description: The mouse press event. This re-implemented off the base
 *              QGraphicsScene and handles all pen and right click control.
 *
 * Inputs: QGraphicsSceneMouseEvent* event - the mouse press event
 * Output: none
 */
void EditorMatrix::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  /* Update mouse location */
  mouseUpdate(event->scenePos());
  emit click();

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
      else if(cursor_mode == EditorEnumDb::THING_PASS_ALL)
        changePassOnActive(true);
    }
    /* Otherwise, show right click menu */
    else if(event->button() == Qt::RightButton)
    {
      if(cursor_mode == EditorEnumDb::THING_PASS_ALL)
      {
        changePassOnActive(false);
      }
      else if(cursor_mode == EditorEnumDb::THING_RENDER_PLUS)
      {
        incrementDepthOnActive(true);
      }
      else if(cursor_mode == EditorEnumDb::THING_RENDER_MINUS)
      {
        decrementDepthOnActive(true);
      }
      else
      {
        rightclick_sprite = active_sprite;
        emit rightClick(rightclick_sprite);
      }
    }
  }
}

/*
 * Description: The mouse release event. This is currently not used.
 *
 * Inputs: QGraphicsSceneMouseEvent* event - the mouse release event
 * Output: none
 */
void EditorMatrix::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  (void)event;
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

/*
 * Description: Called when the edit sprites dialog 'ok' button is pressed,
 *              which will copy the information to all sprites. This is called
 *              after initiated by editAllSprites().
 *
 * Inputs: none
 * Output: none
 */
void EditorMatrix::editSpritesOk()
{
  if(edit_all != NULL)
  {
    /* Copy the base sprite for all sprites */
    for(int i = 0; i < matrix.size(); i++)
    {
      for(int j = 0; j < matrix[i].size(); j++)
      {
        matrix[i][j]->copyBaseSprite(*((EditorSprite*)edit_all));
        matrix[i][j]->update();
      }
    }

    /* Delete the edit sprite */
    delete edit_all;
    edit_all = NULL;
  }
}

/*
 * Description: Places a matrix at the place x and y location in class, with a
 *              resulting path, horizontal flip, and vertical flip status.
 *
 * Inputs: QString result_path - the path to the set of sprites, as a matrix
 *         bool hflip - are they all horizontally flipped?
 *         bool vflip - are they all vertically flipped?
 * Output: none
 */
void EditorMatrix::matrixPlace(QString result_path, bool hflip, bool vflip)
{
  if(!result_path.isEmpty() && place_x >= 0 && place_y >= 0)
  {
    addPath(result_path, place_x, place_y, hflip, vflip, false, true);
    emit matrixChange();
  }
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Adds path at x and y location. This separates the path into
 *              root path and file name and passes the function on. See other
 *              addPath() call for more info.
 *
 * Inputs: QString path - the path and filename of matrix sprite(s)
 *         int x - the top left x location of the matrix of sprites
 *         int y - the top left y location of the matrix of sprites
 *         bool hflip - true if all the sprites should be horizontally flipped
 *         bool vflip - true if all the sprites should be vertically flipped
 *         bool reset - true if the entire matrix should be cleared first
 *         bool clear_modified - should the sprites being modified by the matrix
 *                               be cleared prior to the modification
 * Output: bool - true if it was added
 */
bool EditorMatrix::addPath(QString path, int x, int y, bool hflip,
                           bool vflip, bool reset, bool clear_modified)
{
  if(!path.isEmpty())
  {
    QStringList path_set = path.split(QDir::separator());
    return addPath(path.remove(path_set.last()), path_set.last(), x, y,
                   hflip, vflip, reset, clear_modified);
  }

  return false;
}

/*
 * Description: Adds path at x and y location. This takes a root path, filename,
 *              and horizontal flip, vertical flip. It puts the matrix of
 *              sprites starting ata the x and y location in the matrix and will
 *              reset it first if reset is set (empty the matrix).
 *
 * Inputs: QString root_path - the path to where the file is (needs to end in /)
 *         QString file_name - the name of the sprite file
 *         int x - the top left x location of the matrix of sprites
 *         int y - the top left y location of the matrix of sprites
 *         bool hflip - true if all the sprites should be horizontally flipped
 *         bool vflip - true if all the sprites should be vertically flipped
 *         bool reset - true if the entire matrix should be cleared first
 *         bool clear_modified - should the sprites being modified by the matrix
 *                               be cleared prior to the modification
 * Output: bool - true if it was added
 */
bool EditorMatrix::addPath(QString root_path, QString file_name,
                           int x, int y, bool hflip, bool vflip, bool reset,
                           bool clear_modified)
{
  if(!root_path.isEmpty() && !file_name.isEmpty() && x >= 0 && y >= 0)
  {
    /* If reset, clear the scene */
    if(reset)
      decreaseWidth(getWidth());

    /* Split up the set of frames for the matrix */
    std::vector<std::vector<std::string>> name_set =
                               core::FilePath::separateOnRanges(file_name.toStdString());

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
        if(clear_modified)
          matrix[x + i][y + j]->deleteAllFrames();
        matrix[x + i][y + j]->addPath(root_path +
                                      QString::fromStdString(name_set[i][j]),
                                      hflip, vflip);
      }
    }

    emit matrixChange();
    return true;
  }

  return false;
}

/*
 * Description: Cleans the scene. Used for when moving it to a new view.
 *
 * Inputs: bool just_hover - true if just the hover events are cleaned
 * Output: none
 */
void EditorMatrix::cleanScene(bool just_hover)
{
  /* Clean-up the hover sprite, set by previous views */
  active_sprite = NULL;
  for(int i = 0; i < matrix.size(); i++)
    for(int j = 0; j < matrix[i].size(); j++)
      matrix[i][j]->setHover(false);

  if(!just_hover)
  {
    /* Unset reference sprites */
    rightclick_sprite = NULL;
  }

  /* Set the size of the scene */
  if(matrix.size() > 0)
    setSceneRect(0, 0, matrix.size() * EditorHelpers::getTileSize(),
                 matrix.front().size() * EditorHelpers::getTileSize());
  else
    setSceneRect(0, 0, 0, 0);
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
  if(matrix.size() > 0 && max >= matrix.front().size())
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
  cleanScene(false);
  emit matrixChange();
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
  if(max >= matrix.size())
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
  cleanScene(false);
  emit matrixChange();
}

/*
 * Description: Called to get the EditorTileSprite that is used to edit all the
 *              general settings of the sprite. Edit Pop-up should be connected
 *              with editSpritesOk() to complete the copy.
 *
 * Inputs: none
 * Output: EditorTileSprite* - the editing sprite to load in the pop-up
 */
EditorTileSprite* EditorMatrix::editAllSprites()
{
  /* Make sure the existing edit all sprite isn't null */
  if(edit_all != NULL)
    delete edit_all;
  edit_all = NULL;

  /* Get the first valid sprite and create the edit all sprite */
  EditorTileSprite* valid_sprite = getValidSprite();
  if(valid_sprite != NULL)
    edit_all = new EditorTileSprite(*valid_sprite);

  return edit_all;
}

/*
 * Description: Returns the active frame index, which is being rendered on the
 *              viewport. Default to 0 (the first frame).
 *
 * Inputs: none
 * Output: int - frame index (0+)
 */
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
int EditorMatrix::getHeight() const
{
  if(getWidth() > 0)
    return matrix.last().size();
  return 0;
}

/*
 * Description: Returns the render depth of the sprite at the x and y location
 *              in the matrix.
 *
 * Inputs: int x - the x tile location in matrix (0 is left col)
 *         int y - the y tile location in matrix (0 is top row)
 * Output: int - render depth of the sprite (-1 is invalid)
 */
int EditorMatrix::getRenderDepth(int x, int y)
{
  EditorTileSprite* sprite = getSprite(x, y);
  if(sprite != NULL)
    return sprite->getRenderDepth();
  return -1;
}

/*
 * Description: Returns the right clicked sprite. Stored the last time the
 *              active sprite in the matrix was right clicked on.
 *
 * Inputs: none
 * Output: EditorTileSprite* - the right clicked sprite
 */
EditorTileSprite* EditorMatrix::getRightClicked()
{
  return rightclick_sprite;
}

/*
 * Description: Returns an image snapshot of the scene without any grid,
 *              passability, or render depths. Can pass in a width and height
 *              to scale the pixmap down.
 *
 * Inputs: int width - the max width of the pixmap
 *         int height - the max height of the pixmap
 * Output: QPixmap - the resulting pixmap
 */
QPixmap EditorMatrix::getSnapshot(int width, int height)
{
  /* Disable all visibility */
  bool visible_grid = this->visible_grid;
  bool visible_passability = this->visible_passability;
  bool visible_render = this->visible_render;
  setVisibilityGrid(false, true);
  setVisibilityPass(false, true);
  setVisibilityRender(false, true);

  /* Set-up the scene */
  QPixmap final_pixmap;
  cleanScene();
  clearSelection();
  setSceneRect(itemsBoundingRect());
  QImage image(sceneRect().size().toSize(), QImage::Format_ARGB32);
  image.fill(Qt::transparent);

  /* If the image is NULL, attempt to scale */
  if(!image.isNull())
  {
    QPainter painter(&image);
    render(&painter);
    if((width > 0 && image.width() > width) ||
       (height > 0 && image.height() > height))
    {
      QImage scaled_image = image.scaled(width, height,
                                         Qt::KeepAspectRatio);
      final_pixmap = QPixmap::fromImage(scaled_image);
    }
    else
    {
      final_pixmap = QPixmap::fromImage(image);
    }
  }

  /* Re-enable visibility */
  setVisibilityGrid(visible_grid, true);
  setVisibilityPass(visible_passability, true);
  setVisibilityRender(visible_render, true);

  return final_pixmap;
}

/*
 * Description: Returns the sprite at the x and y location in the matrix.
 *
 * Inputs: int x - the x tile location in matrix (0 is left col)
 *         int y - the y tile location in matrix (0 is top row)
 * Output: EditorTileSprite* - the sprite reference (NULL is invalid)
 */
EditorTileSprite* EditorMatrix::getSprite(int x, int y)
{
  if(x >= 0 && x < getWidth() && y >= 0 && y < getHeight())
    return matrix[x][y];
  return NULL;
}

/*
 * Description: Returns the highest number of frames that are valid and set in
 *              a sprite in the entire matrix. Used to trim off excess frames.
 *
 * Inputs: bool largest - true to roof at largest. false to roof at smallest but
 *                        valid
 * Output: int - number of frames
 */
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

/*
 * Description: Returns a rect that doesn't include empty rows or columns on all
 *              sides. Used to trim the edges of sprites with no frames set.
 *
 * Inputs: none
 * Output: QRect - the rect of valid sprites in the matrix
 */
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
  for(int i = x2 - 1; valid && (i >= x1); i--)
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
  for(int i = y2 - 1; valid && (i >= y1); i--)
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

/*
 * Description: Returns if the grid is visible in the matrix.
 *
 * Inputs: none
 * Output: bool - true if grid is visible
 */
bool EditorMatrix::getVisibilityGrid()
{
  return visible_grid;
}

/*
 * Description: Returns if the passability is visible.
 *
 * Inputs: none
 * Output: bool - true if passability is visible
 */
bool EditorMatrix::getVisibilityPass()
{
  return visible_passability;
}

/*
 * Description: Returns if the render depth number is visible.
 *
 * Inputs: none
 * Output: bool - true if the render depth is visible
 */
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
int EditorMatrix::getWidth() const
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
    /* Get a valid sprite */
    EditorTileSprite* valid_sprite = getValidSprite();

    for(int i = 0; i < matrix.size(); i++)
    {
      int height = matrix[i].size();

      for(int j = 0; j < count; j++)
      {
        if(valid_sprite != NULL)
          matrix[i].push_back(new EditorTileSprite(*valid_sprite));
        else
          matrix[i].push_back(new EditorTileSprite());
        matrix[i].last()->deleteAllFrames();
        matrix[i].last()->setPassability(false);
        matrix[i].last()->setRenderDepth(0);
        matrix[i].last()->setX(i);
        matrix[i].last()->setY(height + j);
        addItem(matrix[i].last());
      }
    }
  }

  /* Update painting visibility */
  setActiveFrame(active_frame, true);
  setTileIcons(tile_icons);
  setVisibilityGrid(visible_grid, true);
  setVisibilityPass(visible_passability, true);
  setVisibilityRender(visible_render, true);

  /* Clean up the scene */
  cleanScene(true);
  emit matrixChange();
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

  /* Get a valid sprite */
  EditorTileSprite* valid_sprite = getValidSprite();

  /* Go through and add to the width */
  for(int i = 0; i < count; i++)
  {
    /* Create the column of correct height */
    QVector<EditorTileSprite*> col;
    int width = getWidth();
    for(int j = 0; j < height; j++)
    {
      if(valid_sprite != NULL)
        col.push_back(new EditorTileSprite(*valid_sprite));
      else
        col.push_back(new EditorTileSprite());
      col.last()->deleteAllFrames();
      col.last()->setPassability(false);
      col.last()->setRenderDepth(0);
      col.last()->setX(width);
      col.last()->setY(j);
      addItem(col.last());
    }

    /* Put the column in the matrix */
    matrix.push_back(col);
  }

  /* Update painting visibility */
  setActiveFrame(active_frame, true);
  setTileIcons(tile_icons);
  setVisibilityGrid(visible_grid, true);
  setVisibilityPass(visible_passability, true);
  setVisibilityRender(visible_render, true);

  /* Clean up the scene */
  cleanScene(true);
  emit matrixChange();
}

/*
 * Description: Loads the matrix data from the XML struct and offset index. Uses
 *              existing functions in game EventHandler class.
 *
 * Inputs: XmlData data - the XML data tree struct
 *         int index - the offset index into the struct
 * Output: none
 */
void EditorMatrix::load(core::XmlData data, int index)
{
  QString element = QString::fromStdString(data.getElement(index));

  /* Parse elements */
  if(element == "multiple")
  {
    /* Parse the range from the multiple */
    QString range = QString::fromStdString(data.getKeyValue(index));
    QStringList xy_range = range.split(",");
    if(xy_range.size() == 2)
    {
      QStringList x_range = xy_range.front().split("-");
      QStringList y_range = xy_range.back().split("-");
      if(x_range.size() >= 1 && y_range.size() >= 1)
      {
        int x = x_range.front().toInt();
        int x_end = x_range.back().toInt();
        int y = y_range.front().toInt();
        int y_end = y_range.back().toInt();

        /* Determine type of multiple */
        QString second_elem = QString::fromStdString(data.getElement(index+1));
        QStringList element_list = second_elem.split("_");
        if(element_list.front() == "path")
        {
          /* Determine flips */
          bool hflip = false;
          bool vflip = false;
          if(element_list.contains("hf"))
            hflip = true;
          if(element_list.contains("vf"))
            vflip = true;

          /* Add path */
          addPath(EditorHelpers::getProjectDir() + QDir::separator() +
                  QString::fromStdString(data.getDataString()),
                  x, y, hflip, vflip);
        }
        else if(second_elem == "passability")
        {
          /* Ensure matrix is large enough */
          if(getWidth() <= x_end || getHeight() <= y_end)
          {
            increaseWidth(x_end - getWidth() + 1);
            increaseHeight(y_end - getHeight() + 1);
          }

          /* Loop through and add passabilities */
          for(int i = x; i < x_end; i++)
            for(int j = y; j < y_end; j++)
              matrix[i][j]->setPassability(
                                  QString::fromStdString(data.getDataString()));
        }
      }
    }
  }
  else if(element == "rendermatrix")
  {
    /* Get the matrix */
    QString set = QString::fromStdString(data.getDataString());
    QStringList split = set.split(".");
    if(split.size() > 0)
    {
      QList<QList<int>> render_depths;

      for(int i = 0; i < split.size(); i++)
      {
        QStringList split_2 = split[i].split(",");
        if(split_2.size() > 0)
        {
          /* Make sure new matrix is large enough */
          while(render_depths.size() < split_2.size())
            render_depths.push_back(QList<int>());

          /* Add depths */
          for(int j = 0; j < split_2.size(); j++)
            render_depths[j].push_back(split_2[j].toInt());
        }
      }

      /* Ensure matrix is large enough */
      if(getWidth() < render_depths.size() ||
         getHeight() < render_depths.front().size())
      {
        increaseWidth(render_depths.size() - getWidth());
        increaseHeight(render_depths.front().size() - getHeight());
      }

      /* Add render depths to matrix */
      for(int i = 0; i < render_depths.size(); i++)
        for(int j = 0; j < render_depths[i].size(); j++)
          matrix[i][j]->setRenderDepth(render_depths[i][j]);
    }
  }
  else if(element == "sprite")
  {
    EditorTileSprite* sprite = editAllSprites();
    sprite->load(data, index + 1);
    editSpritesOk();
  }
  else if(element == "x" && data.getElement(index + 1) == "y")
  {
    /* Get contents and ensure it conforms to standard - comma delimited list */
    QString x_set = QString::fromStdString(data.getKeyValue(index));
    QString y_set = QString::fromStdString(data.getKeyValue(index + 1));
    QStringList x_split = x_set.split(",");
    QStringList y_split = y_set.split(",");
    if(x_split.size() == y_split.size())
    {
      /* Determine type of element */
      QString define_elem = QString::fromStdString(data.getElement(index + 2));
      QStringList element_list = define_elem.split("_");
      bool hflip = false;
      bool vflip = false;
      if(element_list.front() == "path")
      {
        /* Determine flips */
        if(element_list.contains("hf"))
          hflip = true;
        if(element_list.contains("vf"))
          vflip = true;
      }

      /* Path determination */
      QString base_str = QString::fromStdString(data.getDataString());
      QString path_combined = EditorHelpers::getProjectDir() +
                              QDir::separator() + base_str;

      /* Loop through pairs */
      for(int i = 0; i < x_split.size(); i++)
      {
        /* Split the pair - dash delimited */
        QStringList x_pair = x_split[i].split("-");
        QStringList y_pair = y_split[i].split("-");

        /* X set */
        int x1 = x_pair.first().toInt();
        int x2 = x_pair.last().toInt();
        if(x1 > x2)
        {
          int temp = x1;
          x1 = x2;
          x2 = temp;
        }

        /* Y set */
        int y1 = y_pair.first().toInt();
        int y2 = y_pair.last().toInt();
        if(y1 > y2)
        {
          int temp = y1;
          y1 = y2;
          y2 = temp;
        }

        /* Ensure matrix is large enough */
        if(getWidth() <= x2 || getHeight() <= y2)
        {
          increaseWidth(x2 - getWidth() + 1);
          increaseHeight(y2 - getHeight() + 1);
        }

        /* Loop through x,y set */
        for(int j = x1; j <= x2; j++)
        {
          for(int k = y1; k <= y2; k++)
          {
            /* Determine type of element */
            if(element_list.front() == "path")
            {
              addPath(path_combined, j, k, hflip, vflip);
            }
            else if(define_elem == "passability")
            {
              matrix[j][k]->setPassability(base_str);
            }
          }
        }
      }
    }
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
 *         bool shadow - true to render as shadow instead. Default false
 *         QColor shadow_color - the color to render the shadow if true
 * Output: bool - true if the sprite was rendered
 */
bool EditorMatrix::paint(QPainter* painter, QRect rect, int offset_x,
                         int offset_y, bool shadow, QColor shadow_color)
{
  return paint(active_frame, painter, rect, offset_x, offset_y, shadow,
               shadow_color);
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
 *         bool shadow - true to render as shadow instead. Default false
 *         QColor shadow_color - the color to render the shadow if true
 * Output: bool - true if the sprite was rendered
 */
bool EditorMatrix::paint(int frame_index, QPainter* painter, QRect rect,
                         int offset_x, int offset_y, bool shadow,
                         QColor shadow_color)
{
  if(offset_x >= 0 && offset_x < getWidth() &&
     offset_y >= 0 && offset_y < getHeight() &&
     matrix[offset_x][offset_y] != nullptr)
  {
    EditorSprite* paint_sprite = matrix[offset_x][offset_y];
    return paint_sprite->paint(frame_index, painter, rect, shadow,
                               shadow_color);
  }
  return false;
}

/*
 * Description: Rebase the matrix to match the passed in matrix. This conforms
 *              the matrix size, the render depths for each tile sprite, and the
 *              passability.
 *
 * Inputs: EditorMatrix* base_matrix - the matrix to match
 *         bool passability - true if passability shoudl be rebased
 * Output: bool - true if the matrix was matched
 */
bool EditorMatrix::rebase(EditorMatrix* base_matrix, bool passability)
{
  if(base_matrix != nullptr)
  {
    /* Set the size */
    setNewSize(base_matrix->getWidth(), base_matrix->getHeight());

    /* Conform the render depth and passability */
    for(int i = 0; i < matrix.size(); i++)
    {
      for(int j = 0; j < matrix[i].size(); j++)
      {
        matrix[i][j]->setRenderDepth(
                        base_matrix->matrix[i][j]->getRenderDepth());
        if(passability)
          matrix[i][j]->setPassability(
                                   base_matrix->matrix[i][j]->getPassability());
      }
    }

    return true;
  }
  return false;
}

/*
 * Description: Removes all frames from the matrix.
 *
 * Inputs: none
 * Output: none
 */
void EditorMatrix::removeAll()
{
  active_sprite = NULL;
  rightclick_sprite = NULL;

  /* Clear the scene */
  QList<QGraphicsItem*> item_list = items();
  for(int i = 0; i < item_list.size(); i++)
    removeItem(item_list[i]);

  /* Delete all the stored editor tile sprites */
  for(int i = 0; i < matrix.size(); i++)
    for(int j = 0; j < matrix[i].size(); j++)
      delete matrix[i][j];
  matrix.clear();

  emit matrixChange();
}

/*
 * Description: Saves the matrix data to the file handling pointer.
 *
 * Inputs: XmlWriter* writer - the save persistence interface
 *         bool game_only - true if the data should include game only relevant
 *         bool no_render - include render data?
 * Output: none
 */
void EditorMatrix::save(core::XmlWriter* writer, bool game_only, bool no_render)
{
  if(writer != NULL)
  {
    writer->writeElement("sprites");

    /* Get parsing information */
    QList<QList<QList<QPair<QString,QString>>>> set;
    QList<QList<bool>> skip_set;
    for(int i = 0; i < matrix.size(); i++)
    {
      QList<QList<QPair<QString,QString>>> row_set;
      QList<bool> row_skip;
      for(int j = 0; j < matrix[i].size(); j++)
      {
        row_set.push_back(matrix[i][j]->getPathSet());
        row_skip.push_back(false);
      }
      set.push_back(row_set);
      skip_set.push_back(row_skip);
    }

    /* Parse the matrix for simplifications */
    for(int i = 0; i < matrix.size(); i++)
    {
      for(int j = 0; j < matrix[i].size(); j++)
      {
        /* Determine if the base frame could be optimized */
        if(set[i][j].size() == 1 && !skip_set[i][j])
        {
          /* Check if the file name is the standard structure */
          QList<QString> root_stack =
                     EditorHelpers::getValidFileSplit(set[i][j].front().second);
          if(root_stack.size() > 0)
          {
            /* Info on root for comparing */
            QString back = "_" + root_stack.at(3) + root_stack.at(4);
            bool dir_x_for = true;
            bool dir_y_for = true;
            int frames = matrix[i][j]->frameCount();
            int height = 1;
            int letter_a = root_stack.at(1).at(0).unicode();
            int letter_b = root_stack.at(2).at(0).unicode();
            QString front = root_stack.at(0) + "_";
            int width = 1;

            /* First try to find the length of the equivalent col */
            bool finished = false;
            for(int k = j + 1; !finished && k < matrix[i].size(); k++)
            {
              /* Determine if it's forward or backwards if first run */
              if(k == (j + 1))
              {
                QString test_path = front + QString(QChar(letter_a)) +
                                    QString(QChar(letter_b - k + j)) + back;
                if(set[i][k].size() == 1 &&
                   set[i][k].front().second == test_path)
                  dir_y_for = false;
              }

              /* Proceed to check path */
              QString path = front;
              if(dir_y_for)
                path += QString(QChar(letter_a)) +
                        QString(QChar(letter_b + k - j)) + back;
              else
                path += QString(QChar(letter_a)) +
                        QString(QChar(letter_b - k + j)) + back;
              if(matrix[i][k]->frameCount() != frames ||
                 set[i][k].size() != 1 ||
                 set[i][k].front().second != path ||
                 set[i][k].front().first != set[i][j].front().first ||
                 skip_set[i][k])
              {
                finished = true;
              }
              else
              {
                height = k - j + 1;
              }
            }

            /* Using the height, determine the number of rows */
            finished = false;
            for(int k = i + 1; !finished && k < matrix.size(); k++)
            {
              for(int l = 0; l < height; l++)
              {
                /* Determine if it's forward or backwards if first run */
                if(k == (i + 1) && l == 0)
                {
                  QString test_path = front + QString(QChar(letter_a - k + i)) +
                                              QString(QChar(letter_b)) + back;
                  if(set[k][j].size() == 1 &&
                     set[k][j].front().second == test_path)
                    dir_x_for = false;
                }

                /* Proceed to check path */
                QString path = front;
                if(dir_x_for)
                  path += QString(QChar(letter_a + k - i));
                else
                  path += QString(QChar(letter_a - k + i));
                if(dir_y_for)
                  path += QString(QChar(letter_b + l)) + back;
                else
                  path += QString(QChar(letter_b - l)) + back;
                if(matrix[k][j+l]->frameCount() != frames ||
                   set[k][j+l].size() != 1 ||
                   set[k][j+l].front().second != path ||
                   set[k][j+l].front().first != set[i][j].front().first ||
                   skip_set[k][j+l])
                {
                  finished = true;
                }
              }
              if(!finished)
                width++;
            }

            /* If the width and height are valid, add the path */
            if(width > 1 || height > 1)
            {
              /* Assemble the range and path */
              QString path = front;
              QString range = "";
              if(width > 1)
              {
                range += QString::number(i) + "-" +
                         QString::number(i + width - 1) + ",";
                if(dir_x_for)
                  path += "[" + QString(QChar(letter_a)) + "-" +
                          QString(QChar(letter_a + width - 1)) + "]";
                else
                  path += "[" + QString(QChar(letter_a)) + "-" +
                          QString(QChar(letter_a - width + 1)) + "]";
              }
              else
              {
                range += QString::number(i) + ",";
                path += QString(QChar(letter_a));
              }
              if(height > 1)
              {
                range += QString::number(j) + "-" +
                         QString::number(j + height - 1);
                if(dir_y_for)
                  path += "[" + QString(QChar(letter_b)) + "-" +
                          QString(QChar(letter_b + height - 1)) + "]";
                else
                  path += "[" + QString(QChar(letter_b)) + "-" +
                          QString(QChar(letter_b - height + 1)) + "]";
              }
              else
              {
                range += QString::number(j);
                path += QString(QChar(letter_b));
              }
              path += back;

              /* Write data */
              writer->writeElement("multiple", "range", range.toStdString());
              writer->writeData(set[i][j].front().first.toStdString(),
                                path.toStdString());
              writer->jumpToParent();

              /* Flag which tiles have been added */
              for(int x = 0; x < width; x++)
                for(int y = 0; y < height; y++)
                  skip_set[i+x][j+y] = true;
            }
          }
        }
      }
    }

    /* Set up passability matrix */
    QList<QList<QPoint>> pass_set;
    int pass_max = EditorHelpers::getPassabilityNum(true, true, true, true);
    QList<QPoint> blank_set;
    QList<QList<QPoint>> blank_dset;
    for(int i = 0; i <= pass_max; i++)
      pass_set.push_back(blank_set);
    QStringList str_keys;
    QStringList str_paths;
    QList<QList<QList<QPoint>>> str_points;

    /* Loop through matrix and add paths and passability remaining */
    for(int i = 0; i < matrix.size(); i++)
    {
      for(int j = 0; j < matrix[i].size(); j++)
      {
        /* Ensure there is at least one valid frame */
        if(matrix[i][j]->frameCount() != 0)
        {
          /* Do check for both to see if elements should be written */
          int pass_num = matrix[i][j]->getPassabilityNum();
          bool passability = matrix[i][j]->getRenderDepth() == 0 &&
                             pass_num > 0;

          /* Add path(s), if relevant */
          if(!skip_set[i][j])
          {
            /* If there is more than one set, the order matters so load them
             * in individually */
            if(set[i][j].size() > 1)
            {
              writer->writeElement("x", "index", std::to_string(i));
              writer->writeElement("y", "index", std::to_string(j));

              /* Path data */
              for(int k = 0; k < set[i][j].size(); k++)
                writer->writeData(set[i][j][k].first.toStdString(),
                                  set[i][j][k].second.toStdString());

              writer->jumpToParent();
              writer->jumpToParent();
            }
            /* Otherwise, add them to the optimize array list */
            else
            {
              /* Find string index */
              int index = str_keys.indexOf(set[i][j].front().second);
              if(index < 0)
              {
                index = str_keys.size();
                str_keys.push_back(set[i][j].front().second);
                str_points.push_back(blank_dset);
              }

              /* Find path index */
              int index2 = str_paths.indexOf(set[i][j].front().first);
              if(index2 < 0)
              {
                index2 = str_paths.size();
                str_paths.push_back(set[i][j].front().first);
              }
              while(str_points[index].size() <= index2)
                str_points[index].push_back(blank_set);

              /* Place the point */
              str_points[index][index2].push_back(QPoint(i, j));
            }
          }

          /* Passability handling */
          if(passability)
            pass_set[pass_num].push_back(QPoint(i, j));
        }
      }
    }

    /* Optimize and push the last sprites data to file */
    for(int i = 0; i < str_points.size(); i++)
    {
      QList<QPair<QString,QString>> path_set =
                                   EditorHelpers::optimizePoints(str_points[i]);
      for(int j = 0; j < path_set.size(); j++)
      {
        if(!path_set[j].first.isEmpty())
        {
          /* Write the data */
          writer->writeElement("x", "index", path_set[j].first.toStdString());
          writer->writeElement("y", "index", path_set[j].second.toStdString());
          writer->writeData(str_paths[j].toStdString(),
                            str_keys[i].toStdString());
          writer->jumpToParent();
          writer->jumpToParent();
        }
      }
    }

    /* Optimize and Push passability data to file */
    QList<QPair<QString,QString>> index_set =
                                        EditorHelpers::optimizePoints(pass_set);
    for(int i = 1; i < index_set.size(); i++)
    {
      /* Don't proceed if pair is empty */
      if(!index_set[i].first.isEmpty())
      {
        writer->writeElement("x", "index", index_set[i].first.toStdString());
        writer->writeElement("y", "index", index_set[i].second.toStdString());
        writer->writeData("passability",
                          EditorHelpers::getPassabilityStr(i).toStdString());
        writer->jumpToParent();
        writer->jumpToParent();
      }
    }

    /* Save the sprite data */
    EditorTileSprite* sprite = getValidSprite();
    if(sprite != NULL)
      sprite->save(writer, game_only, true);

    writer->jumpToParent(); /* </sprites> */

    /* Save the render matrix */
    if(!no_render)
      saveRender(writer);
  }
}

/*
 * Description: Saves the render matrix data to the file handling pointer.
 *
 * Inputs: XmlWriter* writer - the save persistence interface
 * Output: none
 */
void EditorMatrix::saveRender(core::XmlWriter* writer)
{
  QString render_matrix = "";
  bool all_zero = true;
  if(writer != NULL && matrix.size() > 0)
  {
    for(int j = 0; j < matrix.front().size(); j++)
    {
      for(int i = 0; i < matrix.size(); i++)
      {
        if(matrix[i][j]->getRenderDepth() != 0)
          all_zero = false;
        render_matrix.push_back(
                       QString::number(matrix[i][j]->getRenderDepth()) + ",");
      }
      if(render_matrix.endsWith(","))
        render_matrix.chop(1);
      render_matrix += ".";
    }
    if(render_matrix.endsWith("."))
      render_matrix.chop(1);

    /* Only render if not all zero */
    if(!all_zero)
      writer->writeData("rendermatrix", render_matrix.toStdString());
  }
}

/*
 * Description: Sets the active frame index number. If forced, it sets it even
 *              if it equals the one in the class already.
 *
 * Inputs: int index - the frame index
 *         bool force - true if its set even if the class is already set
 * Output: none
 */
void EditorMatrix::setActiveFrame(int index, bool force)
{
  if(index >= 0 && (index != active_frame || force))
  {
    active_frame = index;

    /* Loop through all tile sprites and update */
    for(int i = 0; i < matrix.size(); i++)
    {
      for(int j = 0; j < matrix[i].size(); j++)
      {
        matrix[i][j]->setActiveFrame(index);
        matrix[i][j]->update();
      }
    }
  }
}

/*
 * Description: Sets the cursor mode of the matrix pen.
 *
 * Inputs: EditorEnumDb::ThingCursor mode - the pen mode
 * Output: none
 */
void EditorMatrix::setCursorMode(EditorEnumDb::ThingCursor mode)
{
  cursor_mode = mode;
}

/*
 * Description: Sets all sprites in the matrix to either be horizontally flipped
 *              or not.
 *
 * Inputs: bool flip - true if they should be flipped. false if reset
 * Output: none
 */
void EditorMatrix::setFlipHorizontal(bool flip)
{
  for(int i = 0; i < matrix.size(); i++)
    for(int j = 0; j < matrix[i].size(); j++)
      matrix[i][j]->setHorizontalFlips(flip);
}

/*
 * Description: Sets all sprites in the matrix to either be vertically flipped
 *              or not.
 *
 * Inputs: bool flip - true if they should be flipped. false if reset
 * Output: none
 */
void EditorMatrix::setFlipVertical(bool flip)
{
  for(int i = 0; i < matrix.size(); i++)
    for(int j = 0; j < matrix[i].size(); j++)
      matrix[i][j]->setVerticalFlips(flip);
}

/*
 * Description: Sets the tile icons, for rendering purposes.
 *
 * Inputs: TileIcons* icons - the rendering icon pointer. Managed by gamedb
 * Output: none
 */
void EditorMatrix::setTileIcons(TileIcons* icons)
{
  tile_icons = icons;

  for(int i = 0; i < matrix.size(); i++)
    for(int j = 0; j < matrix[i].size(); j++)
      matrix[i][j]->setTileIcons(icons);
}

/*
 * Description: Sets the visibility of the grid in all tiles. If forced, it sets
 *              it even if the tile has it already set.
 *
 * Inputs: bool visible - true if grid should be visible
 *         bool force - true if the setting should be forced
 * Output: none
 */
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

/*
 * Description: Sets the visibility of the passability in all tiles. If forced,
 *              it sets it even if the tile has it already set.
 *
 * Inputs: bool visible - true if passability should be visible
 *         bool force - true if the setting should be forced
 * Output: none
 */
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

/*
 * Description: Sets the visibility of the render depth in all tiles. If forced,
 *              it sets it even if the tile has it already set.
 *
 * Inputs: bool visible - true if render depth should be visible
 *         bool force - true if the setting should be forced
 * Output: none
 */
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

/*
 * Description: Trims the matrix of sprites with no valid frames in it on the
 *              edges. If only_se is set, it only trims width and height and
 *              doesn't change the origin (left and top).
 *
 * Inputs: bool only_se - true if only the south and east sides are trimmed
 * Output: none
 */
void EditorMatrix::trim(bool only_se)
{
  /* Clean the scene */
  cleanScene(true);

  /* Get trimmed rect */
  QRect trim_rect = getTrimRect();

  /* Update width / height */
  setNewSize(trim_rect.x() + trim_rect.width(),
             trim_rect.y() + trim_rect.height());

  /* Update origin points */
  if(!only_se)
    setNewOrigin(trim_rect.x(), trim_rect.y());

  /* Update number of frames */
  int frame_count = getTrimFrames(true) + 1;
  for(int i = 0; i < matrix.size(); i++)
    for(int j = 0; j < matrix[i].size(); j++)
      matrix[i][j]->setMaximumFrames(frame_count);

  /* Clean matrix and emit change */
  cleanScene();
  emit matrixChange();
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
