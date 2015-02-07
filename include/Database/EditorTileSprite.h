/*******************************************************************************
 * Class Name: EditorTileSprite
 * Date Created: January 31, 2015
 * Inheritance: QGraphicsItem, EditorSprite
 * Description: This class is for editing sprites that exist within the thing
 *              structure matrix. It's inherited off of EditorSprite and 
 *              QGraphicsItem (since it's used inside a matrix).
 ******************************************************************************/
#ifndef EDITORTILESPRITE_H
#define EDITORTILESPRITE_H

#include <QGraphicsItem>

#include "Database/EditorSprite.h"
#include "Game/Map/TileSprite.h"

class EditorTileSprite : public QGraphicsItem, public EditorSprite
{
public:
  /* Constructor Function */
  EditorTileSprite(int x = 0, int y = 0, QString img_path = "");

  /* Copy constructor */
  EditorTileSprite(const EditorTileSprite &source);

  /* Destructor function */
  virtual ~EditorTileSprite();

private:
  /* Is the sprite hovered */
  bool hovered;

  /* The rendering tile icons */
  TileIcons* tile_icons;

  /* Data for tile sprite */
  TileSprite tile_sprite;

  /* Visibility painting control */
  bool visible_grid;
  bool visible_passability;
  bool visible_render;

  /* Positions for the tiles on the grid */
  int x_pos, y_pos;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorTileSprite &source);

protected:

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Necessary function for returning the bounding rectangle */
  QRectF boundingRect() const;

  /* Decrements the render depth by 1 */
  bool decrementRenderDepth();

  /* Returns the passability */
  bool getPassability(Direction dir);

  /* Returns the render depth */
  uint8_t getRenderDepth();

  /* Returns the X and Y, for the tile sprite */
  int getX();
  int getY();

  /* Increments the render depth by 1 */
  bool incrementRenderDepth();

    /* Painting function for Tile Sprite wrapper */
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
             QWidget* widget = NULL);
  
  /* Sets the hover state */
  void setHover(bool);

  /* Sets the passability */
  void setPassability(bool set_value);
  void setPassability(Direction dir, bool set_value);

  /* Sets the rendering tile icons */
  void setTileIcons(TileIcons* icons);

  /* Sets the render depth. 0 is the base up to max */
  bool setRenderDepth(uint8_t depth);
  
  /* Sets the visibility for control objects */
  void setVisibilityGrid(bool);
  void setVisibilityPass(bool);
  void setVisibilityRender(bool);

  /* Set location of tile in arrangement */
  void setX(int x);
  void setY(int y);

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  /* The copy operator */
  EditorTileSprite& operator= (const EditorTileSprite &source);
};

#endif // EDITORTILESPRITE_H
