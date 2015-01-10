/*******************************************************************************
 * Class Name: TileRender
 * Date Created: December 26, 2013
 * Inheritance: QWidget
 * Description: A tile representation on the mapeditor
 ******************************************************************************/
#include "View/TileRender.h"
#include <QDebug>
/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function
 *
 * Input: x/y coordinates on the map grid and a z depth
 */
TileRender::TileRender(int x, int y, int z)
{
  grid_color = false;
  xpos = x;
  ypos = y;
  zpos = z;
  active_layer = EditorEnumDb::BASE;
  setAcceptHoverEvents(true);
  base = true;
  enhancer = true;
  lower1 = true;
  lower2 = true;
  lower3 = true;
  lower4 = true;
  lower5 = true;
  upper1 = true;
  upper2 = true;
  upper3 = true;
  upper4 = true;
  upper5 = true;
  grid = true;
  pass = false;
  tile = new Tile();
  tile->setStatus(Tile::ACTIVE);
  toolbox = NULL;
  base_layer = NULL;
  enhancer_layer = NULL;
  lower_layers.push_back(NULL);
  lower_layers.push_back(NULL);
  lower_layers.push_back(NULL);
  lower_layers.push_back(NULL);
  lower_layers.push_back(NULL);
  upper_layers.push_back(NULL);
  upper_layers.push_back(NULL);
  upper_layers.push_back(NULL);
  upper_layers.push_back(NULL);
  upper_layers.push_back(NULL);
}

/*
 * Description: Destructor function
 */
TileRender::~TileRender()
{
  for(int i=0; i<5; i++)
  {
    lower_layers[i]=NULL;
    upper_layers[i]=NULL;
  }
  base_layer = NULL;
  enhancer_layer = NULL;
  tile = NULL;
  toolbox = NULL;
}

/*
 * Description: Sets the layer visibility
 *
 * Input: Visibility toggle
 */
void TileRender::setBase(bool toggle)
{
  base = toggle;
}

/*
 * Description: Sets the layer visibility
 *
 * Input: Visibility toggle
 */
void TileRender::setEnhancer(bool toggle)
{
  enhancer = toggle;
}

/*
 * Description: Sets the layer visibility
 *
 * Input: Visibility toggle
 */
void TileRender::setLower1(bool toggle)
{
  lower1 = toggle;
}

/*
 * Description: Sets the layer visibility
 *
 * Input: Visibility toggle
 */
void TileRender::setLower2(bool toggle)
{
  lower2 = toggle;
}

/*
 * Description: Sets the layer visibility
 *
 * Input: Visibility toggle
 */
void TileRender::setLower3(bool toggle)
{
  lower3 = toggle;
}

/*
 * Description: Sets the layer visibility
 *
 * Input: Visibility toggle
 */
void TileRender::setLower4(bool toggle)
{
  lower4 = toggle;
}

/*
 * Description: Sets the layer visibility
 *
 * Input: Visibility toggle
 */
void TileRender::setLower5(bool toggle)
{
  lower5 = toggle;
}

/*
 * Description: Sets the layer visibility
 *
 * Input: Visibility toggle
 */
void TileRender::setUpper1(bool toggle)
{
  upper1 = toggle;
}

/*
 * Description: Sets the layer visibility
 *
 * Input: Visibility toggle
 */
void TileRender::setUpper2(bool toggle)
{
  upper2 = toggle;
}

/*
 * Description: Sets the layer visibility
 *
 * Input: Visibility toggle
 */
void TileRender::setUpper3(bool toggle)
{
  upper3 = toggle;
}

/*
 * Description: Sets the layer visibility
 *
 * Input: Visibility toggle
 */
void TileRender::setUpper4(bool toggle)
{
  upper4 = toggle;
}

/*
 * Description: Sets the layer visibility
 *
 * Input: Visibility toggle
 */
void TileRender::setUpper5(bool toggle)
{
  upper5 = toggle;
}

/*
 * Description: Sets the grid visibility
 *
 * Input: Visibility toggle
 */
void TileRender::setGrid(bool toggle)
{
  grid = toggle;
}

/*
 * Description: Sets the grid visibility
 *
 * Input: Visibility toggle
 */
void TileRender::setPass(bool toggle)
{
  pass = toggle;
}
/*
 * Description : Gets the game tile
 *
 * Output: Tile pointer
 */
Tile* TileRender::gameTile()
{
  return tile;
}

/*
 * Description : Gets the active layer path
 *
 * Output: Path
 */
int TileRender::getActivePath()
{
  switch(active_layer)
  {
    case EditorEnumDb::BASE:
      if(base_layer != NULL)
        return base_layer->getSprite()->getId();
      break;
    case EditorEnumDb::ENHANCER:
      if(enhancer_layer != NULL)
        return enhancer_layer->getSprite()->getId();
      break;
    case EditorEnumDb::LOWER1:
      if(lower_layers[0] != NULL)
        return lower_layers[0]->getSprite()->getId();
      break;
    case EditorEnumDb::LOWER2:
      if(lower_layers[1] != NULL)
        return lower_layers[1]->getSprite()->getId();
      break;
    case EditorEnumDb::LOWER3:
      if(lower_layers[2] != NULL)
        return lower_layers[2]->getSprite()->getId();
      break;
    case EditorEnumDb::LOWER4:
      if(lower_layers[3] != NULL)
        return lower_layers[3]->getSprite()->getId();
      break;
    case EditorEnumDb::LOWER5:
      if(lower_layers[4] != NULL)
        return lower_layers[4]->getSprite()->getId();
      break;
    case EditorEnumDb::UPPER1:
      if(upper_layers[0] != NULL)
        return upper_layers[0]->getSprite()->getId();
      break;
    case EditorEnumDb::UPPER2:
      if(upper_layers[1] != NULL)
        return upper_layers[1]->getSprite()->getId();
      break;
    case EditorEnumDb::UPPER3:
      if(upper_layers[2] != NULL)
        return upper_layers[2]->getSprite()->getId();
      break;
    case EditorEnumDb::UPPER4:
      if(upper_layers[3] != NULL)
        return upper_layers[3]->getSprite()->getId();
      break;
    case EditorEnumDb::UPPER5:
      if(upper_layers[4] != NULL)
        return upper_layers[4]->getSprite()->getId();
      break;
    default:
      break;
  }
  return -1;
}

/*
 * Description : Gets the toolbox current path
 *
 * Output: Path
 */
int TileRender::getToolPath()
{
  return toolbox->getCurrent()->getSprite()->getId();
}

/*
 * Description: Paints the tile
 *
 * Input: Required fields, mostly unused
 */
void TileRender::paint(QPainter *painter,
                        const QStyleOptionGraphicsItem*, QWidget*)
{
  QRect bound(xpos*64,ypos*64,64,64);

  if(base && base_layer != NULL)
    base_layer->paint(painter, bound);

  if(enhancer && enhancer_layer !=NULL)
    enhancer_layer->paint(painter, bound);

  if(lower1 && lower_layers[0] !=NULL)
    lower_layers[0]->paint(painter, bound);

  if(lower2 && lower_layers[1] !=NULL)
    lower_layers[1]->paint(painter, bound);

  if(lower3 && lower_layers[2] !=NULL)
    lower_layers[2]->paint(painter, bound);

  if(lower4 && lower_layers[3] !=NULL)
    lower_layers[3]->paint(painter, bound);

  if(lower5 && lower_layers[4] !=NULL)
    lower_layers[4]->paint(painter, bound);

  if(upper1 && upper_layers[0] !=NULL)
    upper_layers[0]->paint(painter, bound);

  if(upper2 && upper_layers[1] !=NULL)
    upper_layers[1]->paint(painter, bound);

  if(upper3 && upper_layers[2] !=NULL)
    upper_layers[2]->paint(painter, bound);

  if(upper4 && upper_layers[3] !=NULL)
    upper_layers[3]->paint(painter, bound);

  if(upper5 && upper_layers[4] !=NULL)
    upper_layers[4]->paint(painter, bound);

  if(grid)
  {
    if(grid_color)
      painter->setPen(QColor(0,255,100,255));
    else
      painter->setPen(QColor(255,255,255,128));

    painter->drawRect(1+(xpos*64),1+(ypos*64),62,62);
  }
  if(pass)
  {
    painter->setPen(QColor(0,0,0,0));
    QPointF Npoints[4] =
    {
      QPointF((xpos*64)+0,(ypos*64)+0),
      QPointF((xpos*64)+4,(ypos*64)+4),
      QPointF((xpos*64)+60,(ypos*64)+4),
      QPointF((xpos*64)+64,(ypos*64)+0)
    };
    QPointF Epoints[4] =
    {
      QPointF((xpos*64)+64,(ypos*64)+0),
      QPointF((xpos*64)+60,(ypos*64)+4),
      QPointF((xpos*64)+60,(ypos*64)+60),
      QPointF((xpos*64)+64,(ypos*64)+64)
    };
    QPointF Spoints[4] =
    {
      QPointF((xpos*64)+0,(ypos*64)+64),
      QPointF((xpos*64)+4,(ypos*64)+60),
      QPointF((xpos*64)+60,(ypos*64)+60),
      QPointF((xpos*64)+64,(ypos*64)+64)
    };
    QPointF Wpoints[4] =
    {
      QPointF((xpos*64)+0,(ypos*64)+0),
      QPointF((xpos*64)+4,(ypos*64)+4),
      QPointF((xpos*64)+4,(ypos*64)+60),
      QPointF((xpos*64)+0,(ypos*64)+64)
    };
    if(gameTile()->getPassabilityExiting(Direction::NORTH))
      painter->setBrush(QColor(0,255,0,128));
    else
      painter->setBrush(QColor(255,0,0,128));
    painter->drawPolygon(Npoints,4);

    if(gameTile()->getPassabilityExiting(Direction::EAST))
      painter->setBrush(QColor(0,255,0,128));
    else
      painter->setBrush(QColor(255,0,0,128));
    painter->drawPolygon(Epoints,4);

    if(gameTile()->getPassabilityExiting(Direction::SOUTH))
      painter->setBrush(QColor(0,255,0,128));
    else
      painter->setBrush(QColor(255,0,0,128));
    painter->drawPolygon(Spoints,4);

    if(gameTile()->getPassabilityExiting(Direction::WEST))
      painter->setBrush(QColor(0,255,0,128));
    else
      painter->setBrush(QColor(255,0,0,128));
    painter->drawPolygon(Wpoints,4);
  }
}

/*
 * Description: Returns the bounding rectangle (Needed by API)
 */
QRectF TileRender::boundingRect() const
{
  return QRectF(xpos*64,ypos*64,64,64);
}

/*
 * Description: Places the currently selected sprite onto the active map layer
 */
void TileRender::place()
{
  switch(active_layer)
  {
    case EditorEnumDb::BASE:
      base_layer = toolbox->getCurrent();
      tile->setBase(base_layer->getSprite());
      break;
    case EditorEnumDb::ENHANCER:
      enhancer_layer = toolbox->getCurrent();
      tile->setEnhancer(enhancer_layer->getSprite());
      break;
    case EditorEnumDb::LOWER1:
      lower_layers[0] = toolbox->getCurrent();
      tile->insertLower(lower_layers[0]->getSprite(),0);
      break;
    case EditorEnumDb::LOWER2:
      lower_layers[1] = toolbox->getCurrent();
      tile->insertLower(lower_layers[1]->getSprite(),1);
      break;
    case EditorEnumDb::LOWER3:
      lower_layers[2] = toolbox->getCurrent();
      tile->insertLower(lower_layers[2]->getSprite(),2);
      break;
    case EditorEnumDb::LOWER4:
      lower_layers[3] = toolbox->getCurrent();
      tile->insertLower(lower_layers[3]->getSprite(),3);
      break;
    case EditorEnumDb::LOWER5:
      lower_layers[4] = toolbox->getCurrent();
      tile->insertLower(lower_layers[4]->getSprite(),4);
      break;
    case EditorEnumDb::UPPER1:
      upper_layers[0] = toolbox->getCurrent();
      tile->insertUpper(upper_layers[0]->getSprite(),0);
      break;
    case EditorEnumDb::UPPER2:
      upper_layers[1] = toolbox->getCurrent();
      tile->insertUpper(upper_layers[1]->getSprite(),1);
      break;
    case EditorEnumDb::UPPER3:
      upper_layers[2] = toolbox->getCurrent();
      tile->insertUpper(upper_layers[2]->getSprite(),2);
      break;
    case EditorEnumDb::UPPER4:
      upper_layers[3] = toolbox->getCurrent();
      tile->insertUpper(upper_layers[3]->getSprite(),3);
      break;
    case EditorEnumDb::UPPER5:
      upper_layers[4] = toolbox->getCurrent();
      tile->insertUpper(upper_layers[4]->getSprite(),4);
      break;
    default:
      break;
  }
  update();
}

/*
 * Description: Removes the currently selected sprite onto the active map layer
 */
void TileRender::unplace()
{
  switch(active_layer)
  {
    case EditorEnumDb::BASE:
      base_layer = NULL;
      tile->unsetBase();
      break;
    case EditorEnumDb::ENHANCER:
      enhancer_layer = NULL;
      tile->unsetEnhancer();
      break;
    case EditorEnumDb::LOWER1:
      lower_layers[0] = NULL;
      tile->unsetLower(0);
      break;
    case EditorEnumDb::LOWER2:
      lower_layers[1] = NULL;
      tile->unsetLower(1);
      break;
    case EditorEnumDb::LOWER3:
      lower_layers[2] = NULL;
      tile->unsetLower(2);
      break;
    case EditorEnumDb::LOWER4:
      lower_layers[3] = NULL;
      tile->unsetLower(3);
      break;
    case EditorEnumDb::LOWER5:
      lower_layers[4] = NULL;
      tile->unsetLower(4);
      break;
    case EditorEnumDb::UPPER1:
      upper_layers[0] = NULL;
      tile->unsetUpper(0);
      break;
    case EditorEnumDb::UPPER2:
      upper_layers[1] = NULL;
      tile->unsetUpper(1);
      break;
    case EditorEnumDb::UPPER3:
      upper_layers[2] = NULL;
      tile->unsetUpper(2);
      break;
    case EditorEnumDb::UPPER4:
      upper_layers[3] = NULL;
      tile->unsetUpper(3);
      break;
    case EditorEnumDb::UPPER5:
      upper_layers[4] = NULL;
      tile->unsetUpper(4);
      break;
    default:
      break;
  }
  update();
}

/*
 * Description: Sets the toolbox pointer
 *
 * Input: Editor Sprite Toolbox
 */
void TileRender::setToolbox(SpriteView *tool)
{
  toolbox = tool;
}

/*
 * Description: Sets the grid color
 *
 * Input: Color input (true: Selected, false: Unselected)
 */
void TileRender::setGridColor(bool color)
{
  grid_color = color;
  update();
}

void TileRender::editBase(EditorSprite *)
{}
void TileRender::editEnhancer(EditorSprite *)
{}

void TileRender::editLower(EditorSprite *, int)
{}

void TileRender::editUpper(EditorSprite *, int)
{}

