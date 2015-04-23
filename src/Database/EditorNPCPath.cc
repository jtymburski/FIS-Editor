/*******************************************************************************
 * Class Name: EditorNPCPath
 * Date Created: March 29, 2015
 * Inheritance: QGraphicsObject
 * Description: A graphics object which represents the path of a map npc.
 ******************************************************************************/
#include "Database/EditorNPCPath.h"
#include <QDebug>

/* Constant Implementation - see header file for descriptions */
const uint8_t EditorNPCPath::kBORDER_W = 2;
const uint8_t EditorNPCPath::kCOLOR_ALPHA = 191;
const float EditorNPCPath::kHOVER_ALPHA = 0.5;
const uint8_t EditorNPCPath::kLINE_W = 3;
const uint8_t EditorNPCPath::kRECT_W = 15;

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructs the NPC path with a starting x and y location (the
 *              first location of the NPC. Also, allows for the delay to be
 *              customized.
 *
 * Inputs: int x - the starting x tile location
 *         int y - the starting y tile location
 *         int delay - the number of ms to pause at the node
 *         bool xy_flip - true next node goes y, then x. Otherwise, x then y
 */
EditorNPCPath::EditorNPCPath(int x, int y, int delay, bool xy_flip)
             : QGraphicsObject()
{
  /* Initial settings */
  setAcceptHoverEvents(true);
  setEnabled(true);
  setZValue(1);

  /* Set up color presets */
  color_presets.push_back(QColor(255, 255, 0, kCOLOR_ALPHA));
  color_presets.push_back(QColor(255, 175, 0, kCOLOR_ALPHA));
  color_presets.push_back(QColor(255, 100, 0, kCOLOR_ALPHA));
  color_presets.push_back(QColor(255, 0, 0, kCOLOR_ALPHA));
  color_presets.push_back(QColor(255, 154, 255, kCOLOR_ALPHA));
  color_presets.push_back(QColor(255, 0, 255, kCOLOR_ALPHA));
  color_presets.push_back(QColor(128, 0, 255, kCOLOR_ALPHA));
  color_presets.push_back(QColor(0, 0, 255, kCOLOR_ALPHA));
  color_presets.push_back(QColor(0, 128, 255, kCOLOR_ALPHA));
  color_presets.push_back(QColor(0, 255, 255, kCOLOR_ALPHA));
  color_presets.push_back(QColor(0, 255, 0, kCOLOR_ALPHA));
  color_presets.push_back(QColor(0, 128, 0, kCOLOR_ALPHA));

  /* Initial values */
  setColorPreset(0);
  hovered = false;
  index_move = -1;
  index_select = -1;
  interact = false;
  state = MapNPC::LOOPED;
  tracking = MapNPC::NOTRACK;
  visible_by_control = true;
  visible_by_edit = true;

  /* Set up base node, based on starting point */
  Path base_node;
  base_node.x = x;
  base_node.y = y;
  base_node.delay = delay;
  base_node.xy_flip = xy_flip;
  nodes.clear();
  nodes.push_back(base_node);

  /* Clear the hover node */
  hover_node.x = 0;
  hover_node.y = 0;
  hover_used = false;

  /* Clear the move node */
  move_node.x = 0;
  move_node.y = 0;
}

/*
 * Description: Copy constructor function
 *
 * Inputs: EditorNPCPath &source - the source to copy from
 */
EditorNPCPath::EditorNPCPath(const EditorNPCPath &source) : EditorNPCPath()
{
  copySelf(source);
}

/*
 * Description: Destructor function
 */
EditorNPCPath::~EditorNPCPath()
{

}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: The copy function that is called by any copying methods in the
 *              class. Utilized by the copy constructor and the copy operator.
 *
 * Inputs: const EditorNPCPath &source - the reference npc path class
 * Output: none
 */
void EditorNPCPath::copySelf(const EditorNPCPath &source)
{
  prepareGeometryChange();

  color_r = source.color_r;
  color_g = source.color_g;
  color_b = source.color_b;
  color_a = source.color_a;

  interact = source.interact;
  nodes = source.nodes;
  state = source.state;
  tracking = source.tracking;

  update();
}

/*
 * Description: Returns the hover color. If hovered is true, border color is
 *              white. Otherwise, border color is black.
 *
 * Inputs: bool hover_node - is the node to get the border for a hover node
 * Output: QColor - the color to be used for borders
 */
QColor EditorNPCPath::getHoverColor(bool hover_node)
{
  if(hover_node)
  {
    return QColor(0, 0, 0, 255 * kHOVER_ALPHA);
  }
  else
  {
    if(hovered)
      return QColor(225, 225, 225);
    return QColor(0, 0, 0);
  }
}

/*
 * Description: Paints the corner of a path between nodes, when the delta
 *              x and y between nodes are both not 0. The corners are dependent
 *              on the entering and exit directions of the feeders.
 *
 * Inputs: QPainter* painter - the painting engine
 *         int x - the x tile location for the corner
 *         int y - the y tile location for the corner
 *         Direction enter - the side of the tile the entering line comes from
 *         Direction exit - the side of the tile the exiting line leaves from
 *         QColor color - the color to paint the colored portion of path
 * Output: none
 */
void EditorNPCPath::paintCorner(QPainter* painter, int x, int y,
                                Direction enter, Direction exit, QColor color)
{
  int size = EditorHelpers::getTileSize();
  int line_w = kLINE_W + kBORDER_W;
  int delta_line = (kRECT_W - line_w) / 2;
  int delta_rect = (size - kRECT_W) / 2;
  QColor hover_color = getHoverColor();
  int start_x = x * size + delta_rect;
  int start_y = y * size + delta_rect;

  /* --- NE corner --- */
  if((enter == Direction::NORTH && exit == Direction::EAST) ||
     (enter == Direction::EAST && exit == Direction::NORTH))
  {
    /* North line */
    painter->fillRect(start_x + delta_line, start_y,
                      kLINE_W, delta_line, color);
    painter->fillRect(start_x + delta_line + kLINE_W, start_y,
                      kBORDER_W, delta_line, hover_color);

    /* East line */
    painter->fillRect(start_x + delta_line, start_y + delta_line,
                      delta_line + line_w, kLINE_W, color);
    painter->fillRect(start_x + delta_line, start_y + delta_line + kLINE_W,
                      delta_line + line_w, kBORDER_W, hover_color);
  }
  /* --- SE corner --- */
  else if((enter == Direction::EAST && exit == Direction::SOUTH) ||
          (enter == Direction::SOUTH && exit == Direction::EAST))
  {
    /* East line */
    painter->fillRect(start_x + delta_line + kLINE_W, start_y + delta_line,
                      delta_line + kBORDER_W, kLINE_W, color);
    painter->fillRect(start_x + delta_line + kLINE_W,
                      start_y + delta_line + kLINE_W,
                      delta_line + kBORDER_W, kBORDER_W, hover_color);

    /* South line */
    painter->fillRect(start_x + delta_line, start_y + delta_line + kLINE_W,
                      kLINE_W, delta_line + kBORDER_W, color);
    painter->fillRect(start_x + delta_line + kLINE_W,
                      start_y + delta_line + kLINE_W, kBORDER_W,
                      delta_line + kBORDER_W, hover_color);

    /* Corner rect */
    painter->fillRect(start_x + delta_line, start_y + delta_line,
                      kLINE_W, kLINE_W, color);
  }
  /* --- SW corner --- */
  else if((enter == Direction::SOUTH && exit == Direction::WEST) ||
          (enter == Direction::WEST && exit == Direction::SOUTH))
  {
    /* West line */
    painter->fillRect(start_x, start_y + delta_line, delta_line,
                      kLINE_W, color);
    painter->fillRect(start_x, start_y + delta_line + kLINE_W, delta_line,
                      kBORDER_W, hover_color);

    /* South line */
    painter->fillRect(start_x + delta_line, start_y + delta_line, kLINE_W,
                      delta_line + line_w, color);
    painter->fillRect(start_x + delta_line + kLINE_W, start_y + delta_line,
                      kBORDER_W, delta_line + line_w, hover_color);
  }
  /* --- NW corner --- */
  else if((enter == Direction::WEST && exit == Direction::NORTH) ||
          (enter == Direction::NORTH && exit == Direction::WEST))
  {
    /* North line */
    painter->fillRect(start_x + delta_line, start_y,
                      kLINE_W, delta_line + line_w, color);
    painter->fillRect(start_x + delta_line + kLINE_W, start_y,
                      kBORDER_W, delta_line + line_w, hover_color);

    /* West line */
    painter->fillRect(start_x, start_y + delta_line, delta_line,
                      kLINE_W, color);
    painter->fillRect(start_x, start_y + delta_line + kLINE_W, delta_line,
                      kBORDER_W, hover_color);

    /* Bottom border */
    painter->fillRect(start_x + delta_line, start_y + delta_line + kLINE_W,
                      line_w, kBORDER_W, hover_color);
  }
}

/*
 * Description: Paints the defined border line for the path between nodes. This
 *              can only handle a straight line on the horizontal or vertical.
 *              Paints the width as per the defined constants.
 *
 * Inputs: QPainter* painter - the painting engine
 *         int x1 - the starting x tile point of the line
 *         int y1 - the starting y tile point of the line
 *         int x2 - the ending x tile point of the line
 *         int y2 - the ending y tile point of the line
 *         QColor color - the color to paint the colored portion of the path
 * Output: none
 */
void EditorNPCPath::paintLine(QPainter* painter, int x1, int y1, int x2, int y2,
                              QColor color)
{
  int line_w = kLINE_W + kBORDER_W;
  int size = EditorHelpers::getTileSize();
  int tile_w = (size - kRECT_W) / 2;
  int tile_w2 = (size - line_w) / 2;

  /* Determine if x or y line */
  if(x1 != x2)
  {
    /* Flip order to make 1 less than 2 */
    if(x1 > x2)
    {
      int temp = x1;
      x1 = x2;
      x2 = temp;
    }

    /* Painting calculation ranges */
    int x_start = x1 * size + (size - tile_w) - 1;
    int x_end = x2 * size + tile_w;
    int y_start = y1 * size + tile_w2;

    /* Paint the rects */
    painter->fillRect(x_start, y_start, x_end - x_start, kLINE_W, color);
    painter->fillRect(x_start, y_start + kLINE_W,
                      x_end - x_start, kBORDER_W, getHoverColor());
  }
  else if(y1 != y2)
  {
    /* Flip order to make 1 less than 2 */
    if(y1 > y2)
    {
      int temp = y1;
      y1 = y2;
      y2 = temp;
    }

    /* Painting calculation ranges */
    int y_start = y1 * size + (size - tile_w) - 1;
    int y_end = y2 * size + tile_w;
    int x_start = x1 * size + tile_w2;

    /* Paint the rects */
    painter->fillRect(x_start, y_start, kLINE_W, y_end - y_start, color);
    painter->fillRect(x_start + kLINE_W, y_start,
                      kBORDER_W, y_end - y_start, getHoverColor());
  }
}

/*
 * Description: Paints all the lines between two nodes. This will handle
 *              painting the horizontal, vertical, and corner portion if
 *              necessary. If the nodes are directly across from each other (
 *              either vertical or horizontal), it just paints the straight
 *              line.
 *
 * Inputs: QPainter* painter - the painting engine
 *         Path* curr - the current path node
 *         Path* next - the next path node
 *         QColor color - the color to paint the colored portion of the path
 * Output: none
 */
void EditorNPCPath::paintLines(QPainter* painter, Path* curr, Path* next,
                               QColor color)
{
  if(next != NULL)
  {
    int delta_x = next->x - curr->x;
    int delta_y = next->y - curr->y;
    bool x_paint = false;
    bool y_paint = false;

    /* Paint the first line */
    if(delta_x != 0 && (!curr->xy_flip || delta_y == 0))
    {
      paintLine(painter, curr->x, curr->y, next->x, curr->y, color);
      x_paint = true;
    }
    else if(delta_y != 0 && (curr->xy_flip || delta_x == 0))
    {
      paintLine(painter, curr->x, curr->y, curr->x, next->y, color);
      y_paint = true;
    }

    /* Paint the second line */
    if(x_paint && delta_y != 0)
    {
      /* Paint line */
      paintLine(painter, next->x, curr->y, next->x, next->y, color);

      /* Direction decision */
      Direction enter = Direction::WEST;
      if(delta_x < 0)
        enter = Direction::EAST;
      Direction exit = Direction::SOUTH;
      if(delta_y < 0)
        exit = Direction::NORTH;

      /* Paint corner */
      paintCorner(painter, next->x, curr->y, enter, exit, color);
    }
    else if(y_paint && delta_x != 0)
    {
      /* Paint line */
      paintLine(painter, curr->x, next->y, next->x, next->y, color);

      /* Direction decision */
      Direction enter = Direction::NORTH;
      if(delta_y < 0)
        enter = Direction::SOUTH;
      Direction exit = Direction::EAST;
      if(delta_x < 0)
        exit = Direction::WEST;

      /* Paint corner */
      paintCorner(painter, curr->x, next->y, enter, exit, color);
    }
  }
}

/*
 * Description: Paints the relevant node point. Pass in the current, next, and
 *              previous nodes to allow the border to be painted with holes to
 *              allow for the line penetrations. If node_num is set, it paints
 *              a node number on top of the node.
 *
 * Inputs: QPainter* painter - the painting engine
 *         Path* prev - the previous path node
 *         Path* curr - the current path node
 *         Path* next - the next path node
 *         QColor color - the color to paint the colored center of the node
 *         int node_num - the number of the node. Defaults to unused and paints
 *                        nothing
 * Output: none
 */
void EditorNPCPath::paintNode(QPainter* painter, Path* prev, Path* curr,
                              Path* next, QColor color, int node_num)
{
  int size = EditorHelpers::getTileSize();
  int tile_x = curr->x * size;
  int tile_y = curr->y * size;

  /* Paint color rect */
  int offset = (size - kRECT_W) / 2;
  int rect_x = tile_x + offset;
  int rect_y = tile_y + offset;
  QRect rect(rect_x, rect_y, kRECT_W, kRECT_W);
  if(node_num == -1)
  {
    color.setRgb(color.red(), color.green(),
                 color.blue(), color.alpha() * kHOVER_ALPHA);
  }
  else
  {
    if(node_num == index_select)
      color.setRgb(255, 255, 255, color.alpha());
    if(node_num == index_move)
      color.setRgb(color.red(), color.green(),
                   color.blue(), color.alpha() * 0.25);
  }
  painter->fillRect(rect, color);

  /* Paint border corners */
  QColor hover_color = getHoverColor(node_num == -1);
  painter->fillRect(rect_x - 1, rect_y - 1, kBORDER_W, kBORDER_W, hover_color);
  painter->fillRect(rect_x + kRECT_W - 1, rect_y - 1,
                    kBORDER_W, kBORDER_W, hover_color);
  painter->fillRect(rect_x - 1, rect_y + kRECT_W - 1,
                    kBORDER_W, kBORDER_W, hover_color);
  painter->fillRect(rect_x + kRECT_W - 1, rect_y + kRECT_W - 1,
                    kBORDER_W, kBORDER_W, hover_color);

  /* Check where the penetrations are */
  bool bottom = false;
  bool left = false;
  bool right = false;
  bool top = false;
  if(prev != NULL)
  {
    int delta_x = curr->x - prev->x;
    int delta_y = curr->y - prev->y;

    /* Check where */
    if(delta_x != 0 && (prev->xy_flip || delta_y == 0))
    {
      if(delta_x > 0)
        left = true;
      else
        right = true;
    }
    else if(delta_y != 0 && (!prev->xy_flip || delta_x == 0))
    {
      if(delta_y > 0)
        top = true;
      else
        bottom = true;
    }
  }
  if(next != NULL)
  {
    int delta_x = next->x - curr->x;
    int delta_y = next->y - curr->y;

    /* Check where */
    if(delta_x != 0 && (!curr->xy_flip || delta_y == 0))
    {
      if(delta_x > 0)
        right = true;
      else
        left = true;
    }
    else if(delta_y != 0 && (curr->xy_flip || delta_x == 0))
    {
      if(delta_y > 0)
        bottom = true;
      else
        top = true;
    }
  }

  /* Range calculations for line penetrations */
  int line_w = kLINE_W + kBORDER_W;
  int chunk_w = (kRECT_W - line_w) / 2;

  /* Paint top border */
  if(top)
  {
    painter->fillRect(rect_x, rect_y - kBORDER_W, chunk_w,
                      kBORDER_W, hover_color);
    painter->fillRect(rect_x + chunk_w + line_w, rect_y - kBORDER_W, chunk_w,
                      kBORDER_W, hover_color);
  }
  else
  {
    painter->fillRect(rect_x, rect_y - kBORDER_W, kRECT_W,
                      kBORDER_W, hover_color);
  }

  /* Paint bottom border */
  if(bottom)
  {
    painter->fillRect(rect_x, rect_y + kRECT_W, chunk_w,
                      kBORDER_W, hover_color);
    painter->fillRect(rect_x + chunk_w + line_w, rect_y + kRECT_W, chunk_w,
                      kBORDER_W, hover_color);
  }
  else
  {
    painter->fillRect(rect_x, rect_y + kRECT_W, kRECT_W,
                      kBORDER_W, hover_color);
  }

  /* Paint left border */
  if(left)
  {
    painter->fillRect(rect_x - kBORDER_W, rect_y,
                      kBORDER_W, chunk_w, hover_color);
    painter->fillRect(rect_x - kBORDER_W, rect_y + chunk_w + line_w, kBORDER_W,
                      chunk_w, hover_color);
  }
  else
  {
    painter->fillRect(rect_x - kBORDER_W, rect_y, kBORDER_W,
                      kRECT_W, hover_color);
  }

  /* Paint right border */
  if(right)
  {
    painter->fillRect(rect_x + kRECT_W, rect_y, kBORDER_W,
                      chunk_w, hover_color);
    painter->fillRect(rect_x + kRECT_W, rect_y + chunk_w + line_w, kBORDER_W,
                      chunk_w, hover_color);
  }
  else
  {
    painter->fillRect(rect_x + kRECT_W, rect_y, kBORDER_W,
                      kRECT_W, hover_color);
  }

  /* Paint the node number */
  painter->setFont(QFont("Helvetica", 10, QFont::Bold));
  painter->setPen(hover_color);
  if(node_num >= 0)
    painter->drawText(rect, Qt::AlignCenter, QString::number(node_num));
  else if(node_num == -1)
    painter->drawText(rect, Qt::AlignCenter, "H");
}

/*
 * Description: Unsets all indexes with moving and selecting. Only occurs on
 *              any modification to the node structure.
 *
 * Inputs: none
 * Output: none
 */
void EditorNPCPath::unsetAllIndexes()
{
  unsetIndexMove(true, false);
  unsetIndexSelect(false);
  update();
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: The hover enter event of the item (when the mouse enters the
 *              defined shape(). This is re-implemented from parent and then
 *              sets the hover status.
 *
 * Inputs: QGraphicsSceneHoverEvent* - not used
 * Output: none
 */
void EditorNPCPath::hoverEnterEvent(QGraphicsSceneHoverEvent*)
{
  setHovered(true);
  emit hoverInit(this);
}

/*
 * Description: The hover exit event of the item (when the mouse exits the
 *              defined shape(). This is re-implemented from parent and then
 *              sets the hover status.
 *
 * Inputs: QGraphicsSceneHoverEvent* - not used
 * Output: none
 */
void EditorNPCPath::hoverLeaveEvent(QGraphicsSceneHoverEvent*)
{
  setHovered(false);
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Appends the node to the end of the node set, with the
 *              appropriate properties.
 *
 * Inputs: int x - the x tile location of the node
 *         int y - the y tile location of the node
 *         int delay - the delay in ms to wait on the node
 *         bool xy_flip - should the parsing go y->x instead of x->y
 * Output: bool - true if data was valid and could be appended
 */
bool EditorNPCPath::appendNode(int x, int y, int delay, bool xy_flip)
{
  if(x >= 0 && y >= 0 && delay >= 0 && (state == MapNPC::LOOPED ||
     state == MapNPC::BACKANDFORTH || state == MapNPC::RANDOMRANGE))
  {
    prepareGeometryChange();

    Path new_node;
    new_node.x = x;
    new_node.y = y;
    new_node.delay = delay;
    if(state == MapNPC::RANDOMRANGE)
      new_node.xy_flip = false;
    else
      new_node.xy_flip = xy_flip;

    /* Remove last node if random range */
    if(state == MapNPC::RANDOMRANGE && nodes.size() >= 2)
      while(nodes.size() >= 2)
        nodes.removeLast();

    /* Push back nodes and update */
    nodes.push_back(new_node);
    unsetAllIndexes();
    emit pathChanged();
    return true;
  }
  return false;
}

/*
 * Description: Returns the bounding rectangle (Needed by API)
 *
 * Inputs: none
 * Output: QRectF - a float rect struct
 */
QRectF EditorNPCPath::boundingRect() const
{
  int min_x = -1;
  int max_x = -1;
  int min_y = -1;
  int max_y = -1;

  /* Find the lowest and highest x nodes */
  for(int i = 0; i < nodes.size(); i++)
  {
    /* Check min */
    if(min_x < 0 || nodes[i].x < min_x)
      min_x = nodes[i].x;

    /* Check max */
    if(max_x < 0 || nodes[i].x > max_x)
      max_x = nodes[i].x;
  }

  /* Find the lowest and highest y nodes */
  for(int i = 0; i < nodes.size(); i++)
  {
    /* Check min */
    if(min_y < 0 || nodes[i].y < min_y)
      min_y = nodes[i].y;

    /* Check max */
    if(max_y < 0 || nodes[i].y > max_y)
      max_y = nodes[i].y;
  }

  /* Modifications for adding hover node, if relevant */
  if(hover_used)
  {
    if(hover_node.x < min_x)
      min_x = hover_node.x;
    if(hover_node.y < min_y)
      min_y = hover_node.y;
    if(hover_node.x > max_x)
      max_x = hover_node.x;
    if(hover_node.y > max_y)
      max_y = hover_node.y;
  }

  /* Calculate the bounding box */
  int size = EditorHelpers::getTileSize();
  int x = min_x * size;
  int y = min_y * size;
  int w = (max_x - min_x) * size;
  if(w == 0)
    w = size;
  int h = (max_y - min_y) * size;
  if(h == 0)
    h = size;
  return QRectF(x, y, w + size, h + size);
}

/*
 * Description: Checks all nodes with the passed in size of the corresponding
 *              map. If any node falls outside this range, the node is removed.
 *              Also, can be used to limit an npc path nodes to a range.
 *
 * Inputs: int x - the x start of map, usually 0
 *         int y - the y start of map, usually 0
 *         int w - the width of map
 *         int h - the height of map
 * Output: none
 */
void EditorNPCPath::checkNodes(int x, int y, int w, int h)
{
  for(int i = 0; i < nodes.size(); i++)
  {
    if(nodes[i].x < x || nodes[i].y < y || nodes[i].x >= w || nodes[i].y >= h)
    {
      deleteNode(i);
      i--;
    }
  }
}

/*
 * Description: Deletes the node at the passed in index. Index is 0 for the
 *              front and then plus from there.
 *
 * Inputs: int index - the index of the node to delete
 * Output: bool - true if a node is deleted
 */
bool EditorNPCPath::deleteNode(int index)
{
  if(index >= 1 && index < nodes.size())
  {
    prepareGeometryChange();

    nodes.removeAt(index);

    unsetAllIndexes();
    emit pathChanged();
    return true;
  }
  return false;
}

/*
 * Description: Deletes all nodes, except the head, in the node paths.
 *
 * Inputs: none
 * Output: none
 */
void EditorNPCPath::deleteNodes()
{
  prepareGeometryChange();

  while(nodes.size() > 1)
    nodes.removeLast();

  unsetAllIndexes();
  emit pathChanged();
}

/*
 * Description: Edits the node at the passed in index with the following
 *              properties.
 *
 * Inputs: int index - the index of the node to edit
 *         int x - the x tile location of the new node
 *         int y - the y tile location of the new node
 *         int delay - the delay for the npc to wait in ms on the node
 *         bool xy_flip - should the parsing go y->x instead of x->y
 * Output: bool - true if the node was edited.
 */
bool EditorNPCPath::editNode(int index, int x, int y, int delay, bool xy_flip)
{
  Path* node = getNode(index);
  if(node != NULL && x >= 0 && y >= 0)
  {
    prepareGeometryChange();

    node->x = x;
    node->y = y;
    if(delay >= 0)
      node->delay = delay;
    if(state == MapNPC::RANDOMRANGE)
      node->xy_flip = false;
    else
      node->xy_flip = xy_flip;

    unsetAllIndexes();
    emit pathChanged();
    return true;
  }
  return false;
}

/*
 * Description: Returns the color of the path node.
 *
 * Inputs: none
 * Output: QColor - the color of the path representation
 */
QColor EditorNPCPath::getColor()
{
  return QColor(color_r, color_g, color_b, color_a);
}

/*
 * Description: Returns the color of the preset at the index
 *
 * Inputs: int index - index of the preset to acquire
 * Output: QColor - the color of the preset at the index. Black if not found
 */
QColor EditorNPCPath::getColorPreset(int index)
{
  if(index >= 0 && index < color_presets.size())
    return color_presets[index];
  return QColor(0, 0, 0, 0);
}

/*
 * Description: Returns the number of color presets in the stack.
 *
 * Inputs: none
 * Output: int - the number of presets
 */
int EditorNPCPath::getColorPresetCount()
{
  return color_presets.size();
}

/*
 * Description: Returns the hover state which indicates where the hovering node
 *              is in relation to the primary path.
 *
 * Inputs: int &index - pass by reference node index
 *         int hx - the hover x. default invalid and uses internal hover x
 *         int hy - the hover y. default invalid and uses internal hover y
 * Output: EditorNPCPath::HoverState - the relevant hover state enum
 */
EditorNPCPath::HoverState EditorNPCPath::getHoverState(int &index, int hx,
                                                       int hy)
{
  HoverState hover_state = NO_HOVER;

  if(hover_used)
  {
    int x = hover_node.x;
    int y = hover_node.y;
    if(hx >= 0 && hy >= 0)
    {
      x = hx;
      y = hy;
    }

    /* Loop through all nodes */
    for(int i = 1; (hover_state == NO_HOVER) && (i < nodes.size()); i++)
    {
      /* First check if it's on the node */
      if(nodes[i].x == x && nodes[i].y == y)
        hover_state = ON_NODE;

      /* Second, check if it's on the path */
      if(hover_state == NO_HOVER)
      {
        int curr_x = nodes[i].x;
        int curr_y = nodes[i].y;
        int prev_x = nodes[i-1].x;
        int prev_y = nodes[i-1].y;
        int delta_x = curr_x - prev_x;
        int delta_y = curr_y - prev_y;

        /* Direction parsing, depending on flip */
        if(nodes[i-1].xy_flip)
        {
          /* Check y first */
          if(delta_y > 0)
          {
            if(prev_x == x && (y - prev_y ) > 0 && (y - prev_y) <= delta_y)
              hover_state = ON_PATH;
          }
          else if(delta_y < 0)
          {
            if(prev_x == x && (y - prev_y) < 0 && (y - prev_y) >= delta_y)
              hover_state = ON_PATH;
          }

          /* Check x next */
          if(delta_x > 0)
          {
            if(y == curr_y && (x - prev_x) > 0 && (x - prev_x) < delta_x)
              hover_state = ON_PATH;
          }
          else if(delta_x < 0)
          {
            if(y == curr_y && (x - prev_x) < 0 && (x - prev_x) > delta_x)
              hover_state = ON_PATH;
          }
        }
        else
        {
          /* Check x first */
          if(delta_x > 0)
          {
            if(prev_y == y && (x - prev_x) > 0 && (x - prev_x) <= delta_x)
              hover_state = ON_PATH;
          }
          else if(delta_x < 0)
          {
            if(prev_y == y && (x - prev_x) < 0 && (x - prev_x) >= delta_x)
              hover_state = ON_PATH;
          }

          /* Check y next */
          if(delta_y > 0)
          {
            if(x == curr_x && (y - prev_y) > 0 && (y - prev_y) < delta_y)
              hover_state = ON_PATH;
          }
          else if(delta_y < 0)
          {
            if(x == curr_x && (y - prev_y) < 0 && (y - prev_y) > delta_y)
              hover_state = ON_PATH;
          }
        }
      }

      /* If the hover state is valid now, set index */
      if(hover_state != NO_HOVER)
        index = i;
    }

    /* If not no hover, set as general hover */
    if(hover_state == NO_HOVER)
      hover_state = GENERAL;
  }

  return hover_state;
}

/*
 * Description: Returns the move node index.
 *
 * Inputs: none
 * Output: int - the move node index
 */
int EditorNPCPath::getIndexMove()
{
  return index_move;
}

/*
 * Description: Returns the select node index.
 *
 * Inputs: none
 * Output: int - the select node index
 */
int EditorNPCPath::getIndexSelect()
{
  return index_select;
}

/*
 * Description: Returns the path node at the given index (reference)
 *
 * Inputs: int index - the index of the node in the set
 * Output: Path* - the path struct defining the node
 */
Path* EditorNPCPath::getNode(int index)
{
  if(index >= 0 && index < nodes.size())
    return &nodes[index];
  return NULL;
}

/*
 * Description: Returns the path node delay at the given index.
 *
 * Inputs: int index - the index of the node in the set
 * Output: int - the delay of said node, in ms
 */
int EditorNPCPath::getNodeDelay(int index)
{
  Path* node = getNode(index);
  if(node != NULL)
    return node->delay;
  return -1;
}

/*
 * Description: Returns a summary of the node at the index as a string. Used
 *              in lists when working with edit control of the node stack.
 *
 * Inputs: int index - the index of the node in the set
 * Output: QString - the string summary of the node
 */
QString EditorNPCPath::getNodeStr(int index)
{
  QString name_str = "";
  Path* node = getNode(index);

  if(node != NULL)
  {
    /* If path based node system */
    if(state == MapNPC::LOOPED || state == MapNPC::BACKANDFORTH)
    {
      /* Index */
      name_str += QString::number(index) + ": ";

      /* X, Y */
      name_str += "At (" + QString::number(node->x) + "," +
                           QString::number(node->y) + ") ";

      /* Delay */
      name_str += "For " + QString::number(node->delay) + " ms ";

      /* XY Flip */
      if(state == MapNPC::BACKANDFORTH && (index+1) == nodes.size())
        name_str += "Then Reverse Direction";
      else if(node->xy_flip)
        name_str += "Then Y-X Move";
      else
        name_str += "Then X-Y Move";
    }
    /* Random rectangle range (only two points) */
    else if(state == MapNPC::RANDOMRANGE)
    {
      if(index == 0)
        name_str += "Starting corner of Rectangle (";
      else
        name_str += "Other corner of Rectangle (";

      /* Finalize display */
      name_str += QString::number(node->x) + "," +
                  QString::number(node->y) + ") ";
    }
    /* Random over entire map */
    else if(state == MapNPC::RANDOM)
    {
      name_str += "Starting At (" + QString::number(node->x) + "," +
                                    QString::number(node->y) + ") ";
    }
    /* Locked */
    else
    {
      name_str += "At (" + QString::number(node->x) + "," +
                           QString::number(node->y) + ") Forever";
    }
  }

  return name_str;
}

/*
 * Description: Returns the path node x tile location at the given index.
 *
 * Inputs: int index - the index of the node in the set
 * Output: int - the x tile location of the node
 */
int EditorNPCPath::getNodeX(int index)
{
  Path* node = getNode(index);
  if(node != NULL)
    return node->x;
  return -1;
}

/*
 * Description: Returns the path node xy flip setting. If true, the walking to
 *              the next node goes y direction then x direction. Otherwise, it
 *              defaults x direction then y direction.
 *
 * Inputs: int index - the index of the node in the set
 * Output: bool - true if xy flip for node parsing
 */
bool EditorNPCPath::getNodeXYFlip(int index)
{
  Path* node = getNode(index);
  if(node != NULL)
    return node->xy_flip;
  return false;
}

/*
 * Description: Returns the path node y tile location at the given index.
 *
 * Inputs: int index - the index of the node in the set
 * Output: int - the y tile location of the node
 */
int EditorNPCPath::getNodeY(int index)
{
  Path* node = getNode(index);
  if(node != NULL)
    return node->y;
  return -1;
}

/*
 * Description: Returns the set of all nodes in the npc path
 *
 * Inputs: none
 * Output: QList<Path> - the set of all path nodes
 */
QList<Path> EditorNPCPath::getNodes()
{
  return nodes;
}

/*
 * Description: Gets the state of the node path set.
 *
 * Inputs: none
 * Output: MapNPC::NodeState - the state of the nodes in the stack
 */
MapNPC::NodeState EditorNPCPath::getState()
{
  return state;
}

/*
 * Description: Sets the tracking of the NPC node on the path, as per the enum
 *              defined in the MapNPC::TrackingState.
 *
 * Inputs: none
 * Output: MapNPC::TrackingState - the tracking state currently being used
 */
MapNPC::TrackingState EditorNPCPath::getTracking()
{
  return tracking;
}

/*
 * Description: Inserts a node with the passed in information after the passed
 *              in index.
 *
 * Inputs: int index - the index to insert a path node after
 *         int x - the x tile location of the node
 *         int y - the y tile location of the node
 *         int delay - the delay of the path node, in ms
 *         bool xy_flip - should the parsing go y->x instead of x->y
 * Output: bool - true if the node was inserted. Otherwise bad data or index
 */
bool EditorNPCPath::insertNodeAfter(int index, int x, int y,
                                    int delay, bool xy_flip)
{
  return insertNodeBefore(index+1, x, y, delay, xy_flip);
}

/*
 * Description: Inserts a node with the passed in information before the passed
 *              in index.
 *
 * Inputs: int index - the index to insert a path node after
 *         int x - the x tile location of the node
 *         int y - the y tile location of the node
 *         int delay - the delay of the path node, in ms
 *         bool xy_flip - should the parsing go y->x instead of x->y
 * Output: bool - true if the node was inserted. Otherwise bad data or index
 */
bool EditorNPCPath::insertNodeBefore(int index, int x, int y,
                                     int delay, bool xy_flip)
{
  if(index >= 1 && (state == MapNPC::LOOPED || state == MapNPC::BACKANDFORTH))
  {
    if(index < nodes.size())
    {
      if(x >= 0 && y >= 0 && delay >= 0)
      {
        prepareGeometryChange();

        Path new_node;
        new_node.x = x;
        new_node.y = y;
        new_node.delay = delay;
        new_node.xy_flip = xy_flip;

        /* Insert the node */
        nodes.insert(index, new_node);
        unsetAllIndexes();

        emit pathChanged();
        return true;
      }

    }
    else
    {
      return appendNode(x, y, delay);
    }
  }
  return false;
}

/*
 * Description: Returns if the npc on the path will force interaction, if
 *              possible.
 *
 * Inputs: none
 * Output: bool - true if the npc will force interaction
 */
bool EditorNPCPath::isForcedInteraction()
{
  return interact;
}

/*
 * Description: Returns if the path node set is currently being hovered on
 *
 * Inputs: none
 * Output: bool - true if being hovered on
 */
bool EditorNPCPath::isHovered()
{
  return hovered;
}

/*
 * Description: Returns if the visibility setting from map control is true.
 *
 * Inputs: none
 * Output: bool - true if visible based on control setting
 */
bool EditorNPCPath::isVisibleControl()
{
  return visible_by_control;
}

/*
 * Description: Returns if the visibility setting from map render edit is true.
 *
 * Inputs: none
 * Output: bool - true if visible based on render edit setting
 */
bool EditorNPCPath::isVisibleEdit()
{
  return visible_by_edit;
}

/* Loads the path data */
// TODO: Comment
void EditorNPCPath::load(XmlData data, int index)
{
  // TODO: Implementation
}

/*
 * Description: Paints the path node set
 *
 * Input: QPainter* painter - the painting engine
 *        const QStyleOptionGraphcisItem* - not used
 *        QWidget* - not used
 * Output: none
 */
void EditorNPCPath::paint(QPainter* painter, const QStyleOptionGraphicsItem*, 
                          QWidget*)
{
  if(isVisible())
  {
    /* Determine the color */
    QColor color(color_r, color_g, color_b, color_a);

    /* Paint the hover node */
    if(hover_used && index_move < 0)
      paintNode(painter, NULL, &hover_node, NULL, color);

    /* Parse all paths */
    for(int i = 0; i < nodes.size(); i++)
    {
      Path* curr = &nodes[i];
      Path* next = NULL;
      if(i < (nodes.size() - 1))
        next = &nodes[i+1];
      else if(i == (nodes.size() - 1) && (state == MapNPC::LOOPED ||
              state == MapNPC::RANDOMRANGE) && nodes.size() > 1)
        next = &nodes.front();

      /* Paint the path between nodes */
      paintLines(painter, curr, next, color);
    }

    /* Parse all nodes */
    for(int i = 0; i < nodes.size(); i++)
    {
      Path* curr = &nodes[i];
      Path* prev = NULL;
      if(i > 0)
        prev = &nodes[i-1];
      else if(i == 0 && (state == MapNPC::LOOPED ||
              state == MapNPC::RANDOMRANGE) && nodes.size() > 1)
        prev = &nodes.last();
      Path* next = NULL;
      if(i < (nodes.size() - 1))
        next = &nodes[i+1];
      else if(i == (nodes.size() - 1) && (state == MapNPC::LOOPED ||
              state == MapNPC::RANDOMRANGE) && nodes.size() > 1)
        next = &nodes.front();

      /* Paint node rect */
      paintNode(painter, prev, curr, next, color, i);
    }
  }
}

/*
 * Description: Saves the path data to the file handling pointer.
 *
 * Inputs: FileHandler* fh - the file handling pointer
 *         bool game_only - true if the data should include game only relevant
 * Output: none
 */
void EditorNPCPath::save(FileHandler* fh, bool game_only)
{
  EditorNPCPath default_path;
  (void)game_only;

  if(fh != NULL)
  {
    /* Write the path state */
    if(default_path.getState() != getState())
    {
      std::string element = "nodestate";
      fh->writeXmlData(element, MapNPC::getNodeString(getState()));
    }

    /* Write the tracking state */
    if(default_path.getTracking() != getTracking())
    {
      std::string element = "tracking";
      fh->writeXmlData(element, MapNPC::getTrackingString(getTracking()));
    }

    /* Write the forced interaction state */
    if(default_path.isForcedInteraction() != isForcedInteraction())
      fh->writeXmlData("forcedinteraction", isForcedInteraction());

    /* Write the nodes */
    for(int i = 0; i < nodes.size(); i++)
    {
      fh->writeXmlElement("node", "id", QString::number(i).toStdString());

      /* X and y location */
      fh->writeXmlData("x", nodes[i].x);
      fh->writeXmlData("y", nodes[i].y);

      /* Delay */
      if(nodes[i].delay > 0)
        fh->writeXmlData("delay", nodes[i].delay);

      /* XY Flip */
      if(nodes[i].xy_flip)
        fh->writeXmlData("xyflip", nodes[i].xy_flip);

      fh->writeXmlElementEnd();
    }
  }
}

/*
 * Description: Sets the color of the path rendered on the scene
 *
 * Inputs: int r - the red color value (0-255)
 *         int g - the green color value (0-255)
 *         int b - the blue color value (0-255)
 *         int a - the alpha value (0-255, default 75%)
 * Output: bool - true if color set (data was good)
 */
bool EditorNPCPath::setColor(int r, int g, int b, int a)
{
  if(r >= 0 && r <= 255 && g >= 0 && g <= 255 &&
     b >= 0 && b <= 255 && a >= 0 && a <= 255)
  {
    color_r = r;
    color_g = g;
    color_b = b;
    color_a = a;

    update();
    return true;
  }
  return false;
}

/*
 * Description: Sets the color preset as the current color for the path being
 *              rendered. To check what each color is, use getColorPreset()
 *
 * Inputs: int index - the index of the color preset in the stack
 * Output: bool - true if the index is within range
 */
bool EditorNPCPath::setColorPreset(int index)
{
  if(index >= 0 && index < color_presets.size())
  {
    color_r = color_presets[index].red();
    color_g = color_presets[index].green();
    color_b = color_presets[index].blue();
    color_a = color_presets[index].alpha();

    update();
    return true;
  }
  return false;
}

/*
 * Description: Sets if the npc on path will force interaction.
 *
 * Inputs: bool forced - true if the npc should force interaction
 * Output: none
 */
void EditorNPCPath::setForcedInteraction(bool forced)
{
  if(this->interact != forced)
  {
    this->interact = forced;
    update();
  }
}

/*
 * Description: Sets if the npc path is hovered (changes the border color)
 *
 * Inputs: bool hovered - is the path nodes hovered on?
 * Output: none
 */
void EditorNPCPath::setHovered(bool hovered)
{
  if(this->hovered != hovered)
  {
    this->hovered = hovered;
    update();
  }
}

/* Sets the hover node x, y. Default unsets */
void EditorNPCPath::setHoverNode(int x, int y)
{
  prepareGeometryChange();

  if(x >= 0 && y >= 0)
  {
    hover_node.x = x;
    hover_node.y = y;
    hover_used = true;

    /* If move cycle, move that node to the new location */
    if(index_move >= 0)
    {
      nodes[index_move].x = x;
      nodes[index_move].y = y;
    }
  }
  else
  {
    /* First set it in new bounding box */
    hover_node.x = 0;
    hover_node.y = 0;
    hover_used = false;
  }

  update();
}

/*
 * Description: Sets the index of the node that is in the process of being
 *              moved. Renders it more transparent during the move cycle.
 *
 * Inputs: int index - the node index being moved
 * Output: bool - true if the index was in range and was changed
 */
bool EditorNPCPath::setIndexMove(int index)
{
  if(index_move < 0 && index > 0 && index < nodes.size())
  {
    index_move = index;

    /* Set move node old location */
    move_node.x = nodes[index].x;
    move_node.y = nodes[index].y;

    update();
    return true;
  }
  return false;
}

/*
 * Description: Sets the index of the node that is selected (by the menu
 *              selection for viewing). Renders it brighter while it is
 *              selected.
 *
 * Inputs: int index - the node index being selected
 * Output: bool - true if the index was in range and was changed
 */
bool EditorNPCPath::setIndexSelect(int index)
{
  if(index >= 0 && index < nodes.size())
  {
    index_select = index;
    update();
    return true;
  }
  return false;
}

/*
 * Description: Sets the state of the node set (how the npc parses the nodes)
 *
 * Inputs: MapNPC::NodeState state - the new node state
 * Output: none
 */
void EditorNPCPath::setState(MapNPC::NodeState state)
{
  if(this->state != state)
  {
    this->state = state;
    prepareGeometryChange();

    /* Update the node count depending on the state change */
    if(state == MapNPC::RANDOMRANGE)
    {
      /* Clean up nodes - max is 2 */
      while(nodes.size() > 2)
        nodes.removeLast();

      /* Loop through all and correct parsing direction */
      for(int i = 0; i < nodes.size(); i++)
        nodes[i].xy_flip = false;
    }
    else if(state == MapNPC::RANDOM || state == MapNPC::LOCKED)
    {
      while(nodes.size() > 1)
        nodes.removeLast();
    }

    unsetAllIndexes();
  }
}

/*
 * Description: Sets the tracking state of the node set. This defines how the
 *              npc will behave when a player is in the vicinity.
 *
 * Inputs: MapNPC::TrackingState tracking - the new tracking definition for NPC
 * Output: none
 */
void EditorNPCPath::setTracking(MapNPC::TrackingState tracking)
{
  if(this->tracking != tracking)
  {
    this->tracking = tracking;
    update();
  }
}

/*
 * Description: Sets the visibility of the path. Controlled by the MapControl
 *              view and affects visibility.
 *
 * Inputs: bool visible - true if path should be visible
 * Output: none
 */
void EditorNPCPath::setVisibleControl(bool visible)
{
  if(visible != visible_by_control)
  {
    visible_by_control = visible;
    setEnabled(visible_by_control && visible_by_edit);
    setVisible(visible_by_control && visible_by_edit);
    update();
  }
}

/*
 * Description: Sets the visibility of the path. Controlled by the MapRender
 *              view and affects visibility.
 *
 * Inputs: bool visible - true if path should be visible
 * Output: none
 */
void EditorNPCPath::setVisibleEdit(bool visible)
{
  if(visible != visible_by_edit)
  {
    visible_by_edit = visible;
    setEnabled(visible_by_control && visible_by_edit);
    setVisible(visible_by_control && visible_by_edit);
    update();
  }
}

/*
 * Description: Sets the hover custom shape of the npc path set (which is
 *              different to the more rudimentary boundingBox). Allows for more
 *              fine tooth control of the hover state. Re-implemented virtual
 *              function.
 *
 * Inputs: none
 * Output: QPainterPath - the painter path of the shape outline
 */
QPainterPath EditorNPCPath::shape() const
{
  QPainterPath path;
  int size = EditorHelpers::getTileSize();
  int rect = kRECT_W + kLINE_W * 2;
  int offset = (size - rect) / 2;

  for(int i = 0; i < nodes.size(); i++)
  {
    int x = nodes[i].x * size + offset;
    int y = nodes[i].y * size + offset;
    path.addRect(x, y, rect, rect);
  }

  // TODO: FUTURE - FOR LINES??
  return path;
}

/*
 * Description: Unsets the move index (either on completion of move or cancel).
 *
 * Inputs: bool cancel - true to cancel the move. false to use hover location
 *         bool allow_update - update the QGraphicsItem. Default to true
 * Output: none
 */
void EditorNPCPath::unsetIndexMove(bool cancel, bool allow_update)
{
  if(index_move >= 0)
  {
    /* If cancel, reset the location */
    if(cancel)
    {
      prepareGeometryChange();
      nodes[index_move].x = move_node.x;
      nodes[index_move].y = move_node.y;
    }

    /* Kick out the old move index */
    index_move = -1;
    if(allow_update)
      update();
  }
}

/*
 * Description: Unsets the select index, once the list is refreshed.
 *
 * Inputs: bool allow_update - update the QGraphicsItem. Default to true
 * Output: none
 */
void EditorNPCPath::unsetIndexSelect(bool allow_update)
{
  if(index_select >= 0)
  {
    index_select = -1;
    if(allow_update)
      update();
  }
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
  
/*
 * Description: Copy operator construction. This is called when the variable
 *              already exists and equal operator used with another object.
 *
 * Inputs: const EditorNPCPath &source - the source class constructor
 * Output: EditorNPCPath& - reference to the copied class
 */
EditorNPCPath& EditorNPCPath::operator= (const EditorNPCPath &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
