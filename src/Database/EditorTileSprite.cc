/*******************************************************************************
 * Class Name: EditorTileSprite
 * Date Created: January 31, 2015
 * Inheritance: QGraphicsItem, EditorSprite
 * Description: This class is for editing sprites that exist within the thing
 *              structure matrix. It's inherited off of EditorSprite and 
 *              QGraphicsItem (since it's used inside a matrix).
 ******************************************************************************/
#include "Database/EditorTileSprite.h"
#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function for the editor sprite. Since it's a
 *              QGraphicsItem and gets used inside a matrix, which is a 
 *              QGraphicsScene, it has additonal functionality, such as x and
 *              y.
 *
 * Inputs: int x - the x tile location, in the matrix (not pixel x)
 *         int y - the y tile location, in the matrix (not pixel y)
 *         QString img_path - the path to the image
 */
EditorTileSprite::EditorTileSprite(int x, int y, QString img_path)
                : QGraphicsItem(), EditorSprite(img_path)
{
  /* Class control */
  hovered = false;
  tile_icons = NULL;
  visible_grid = true;
  visible_passability = false;
  visible_render = false;
  x_pos = x;
  y_pos = y;
}

/*
 * Description: Copy constructor for EditorTileSprite data.
 *
 * Inputs: const EditorTileSprite &source - the data being copied in
 */
EditorTileSprite::EditorTileSprite(const EditorTileSprite &source)
                : EditorTileSprite()
{
  copySelf(source);
}

/*
 * Description: Destructor function
 */
EditorTileSprite::~EditorTileSprite()
{

}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: Copies all data from source editor tile sprite to this editor
 *              sprite.
 *
 * Inputs: EditorTileSprite &source - the source to copy from
 * Output: none
 */
void EditorTileSprite::copySelf(const EditorTileSprite &source)
{
  /* Copy data for this class */
  tile_sprite = source.tile_sprite;

  /* Copy data for parent class */
  EditorSprite::copySelf(source);
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Returns the bounding rectangle (Needed by API)
 *
 * Inputs: none
 * Output: QRectF - a float rect struct
 */
QRectF EditorTileSprite::boundingRect() const
{
  int size = EditorHelpers::getTileSize();
  return QRectF(x_pos * size, y_pos * size, size, size);
}

/*
 * Description: Decrements the render depth, if possible, by 1. Lowest point is
 *              0.
 *
 * Inputs: none
 * Output: bool - true if decremented
 */
bool EditorTileSprite::decrementRenderDepth()
{
  uint8_t render_depth = getRenderDepth();
  if(render_depth > 0)
    return setRenderDepth(render_depth - 1);
  return false;

}

/* 
 * Description: Returns the passability of a given direction.
 *
 * Inputs: Direction dir - the direction to return passability for
 * Output: bool - true if passable leaving the tile
 */
bool EditorTileSprite::getPassability(Direction dir)
{
  return tile_sprite.getPassability(dir);
}

/*
 * Description: Returns the render depth of the EditorTile sprite. This is a 
 *              number from 0 to X, of the point it is rendered on the map.
 *
 * Inputs: none
 * Output: uint8_t - the unsigned integer for render depth
 */
uint8_t EditorTileSprite::getRenderDepth()
{
  return tile_sprite.getRenderDepth();
}
  
/*
 * Description: Returns the X, tile location, of the sprite.
 *
 * Inputs: none
 * Output: int - the x tile offset of the tile. 0 is base
 */
int EditorTileSprite::getX()
{
  return x_pos;
}

/*
 * Description: Returns the Y, tile location, of the sprite.
 *
 * Inputs: none
 * Output: int - the y tile offset of the tile. 0 is base
 */
int EditorTileSprite::getY()
{
  return y_pos;
}

/*
 * Description: Increments the render depth, if possible, by 1. Highest point is 
 *              10 (defined by Helpers.cc).
 *
 * Inputs: none
 * Output: bool - true if incremented
 */
bool EditorTileSprite::incrementRenderDepth()
{
  return setRenderDepth(getRenderDepth() + 1);
}

/*
 * Description: Paints the editor tile sprite.
 *
 * Input: QPainter* painter - the painting engine
 *        QStyleOptionGraphicsItem*, QWidget* - unused
 * Output: none
 */
void EditorTileSprite::paint(QPainter* painter, 
                             const QStyleOptionGraphicsItem*, QWidget*)
{
  int size = EditorHelpers::getTileSize();
  QRect bound(x_pos * size, y_pos * size, size, size);

  /* Render the sprite */
  EditorSprite::paint(painter, bound);

  /* Render the grid */
  if(visible_grid)
  {
    painter->setPen(QColor(255, 255, 255, 128));
    painter->drawRect(1 + (x_pos * size), 1 + (y_pos * size),
                      size - 2, size - 2);
  }

  /* Render the hover */
  if(hovered)
  {
    painter->fillRect(x_pos * size + 1, y_pos * size + 1, size - 2, size - 2,
                      QColor(200, 200, 200, 128));
  }

  /* Render the passability */
  if(visible_passability && tile_icons != NULL && getRenderDepth() == 0)
  {
    /* North Passability */
    if(getPassability(Direction::NORTH))
      painter->drawPixmap(x_pos * size, y_pos * size, size, size,
                          *tile_icons->passN);
    else
      painter->drawPixmap(x_pos * size, y_pos * size, size, size,
                          *tile_icons->nopassN);

    /* East Passability */
    if(getPassability(Direction::EAST))
      painter->drawPixmap(x_pos * size, y_pos * size, size, size,
                          *tile_icons->passE);
    else
      painter->drawPixmap(x_pos * size, y_pos * size, size, size,
                          *tile_icons->nopassE);

    /* South Passability */
    if(getPassability(Direction::SOUTH))
      painter->drawPixmap(x_pos * size, y_pos * size, size, size,
                          *tile_icons->passS);
    else
      painter->drawPixmap(x_pos * size, y_pos * size, size, size,
                          *tile_icons->nopassS);

    /* West Passability */
    if(getPassability(Direction::WEST))
      painter->drawPixmap(x_pos * size, y_pos * size, size, size,
                          *tile_icons->passW);
    else
      painter->drawPixmap(x_pos * size, y_pos * size, size, size,
                          *tile_icons->nopassW);
  }

  /* Render the render depth value */
  if(visible_render)
  {
    QFont font;
    font.setBold(true);
    font.setPointSize(14);
    painter->setFont(font);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(Qt::white);
    painter->drawText(bound, Qt::AlignCenter, 
                      QString::number(getRenderDepth()));
  }
}
  
/*
 * Description: Sets the hover state of the QGraphicsItem.
 *
 * Inputs: bool hover - is it being hovered on?
 * Output: none
 */
void EditorTileSprite::setHover(bool hover)
{
  hovered = hover;
  update();
}

/*
 * Description: Sets the passability of the editor tile sprite, of all
 *              directions to the set value.
 *
 * Inputs: bool set_value - the set value of the passability
 * Output: none
 */
void EditorTileSprite::setPassability(bool set_value)
{
  tile_sprite.setPassability(Direction::NORTH, set_value);
  tile_sprite.setPassability(Direction::EAST, set_value);
  tile_sprite.setPassability(Direction::SOUTH, set_value);
  tile_sprite.setPassability(Direction::WEST, set_value);
  update();
}

/*
 * Description: Sets the passability of the editor tile sprite, in the given
 *              direction to the set value.
 *
 * Inputs: Direction dir - the direction to set the passability for
 *         bool set_value - the value to set the passability to
 * Output: none
 */
void EditorTileSprite::setPassability(Direction dir, bool set_value)
{
  tile_sprite.setPassability(dir, set_value);
  update();
}

/*
 * Description: Sets the tile icons, for rendering purposes.
 *
 * Inputs: TileIcons* icons - the rendering icon pointer. Managed by gamedb
 * Output: none
 */
void EditorTileSprite::setTileIcons(TileIcons* icons)
{
  tile_icons = icons;
  update();
}

/*
 * Description: Sets the render depth of the tile sprite. Range is 0 to 10.
 *
 * Inputs: uint8_t depth - the render depth
 * Output: bool - true if set. false if out of range
 */
bool EditorTileSprite::setRenderDepth(uint8_t depth)
{
  return tile_sprite.setRenderDepth(depth);
  update();
}
  
/*
 * Description: Sets the visibility of the grid in the sprite.
 *
 * Inputs: bool toggle - true if the grid should be visible
 * Output: none
 */
void EditorTileSprite::setVisibilityGrid(bool toggle)
{
  visible_grid = toggle;
  update();
}

/*
 * Description: Sets the visiblity of the passability indicators in the sprite.
 *
 * Inputs: bool toggle - true if the passability should be visible
 * Output: none
 */
void EditorTileSprite::setVisibilityPass(bool toggle)
{
  visible_passability = toggle;
  update();
}

/*
 * Description: Sets the visibility of the render value indicator on the sprite.
 *
 * Inputs: bool toggle - true if the render value should be visible
 * Output: none
 */
void EditorTileSprite::setVisibilityRender(bool toggle)
{
  visible_render = toggle;
  update();
}

/*
 * Description: Sets the X tile location in the matrix (not pixel value).
 *
 * Inputs: int x - the x location in the matrix.
 * Output: none
 */
void EditorTileSprite::setX(int x)
{
  x_pos = x;
  update();
}

/*
 * Description: Sets the Y tile location in the matrix (not pixel value).
 *
 * Inputs: int y - the y location in the matrix.
 * Output: none
 */
void EditorTileSprite::setY(int y)
{
  y_pos = y;
  update();
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/*
 * Description: Copy operator construction. This is called when the variable
 *              already exists and equal operator used with another object.
 *
 * Inputs: const EditorTileSprite &source - the source class constructor
 * Output: EditorTile& - reference to the copied class
 */
EditorTileSprite& EditorTileSprite::operator= (const EditorTileSprite &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
