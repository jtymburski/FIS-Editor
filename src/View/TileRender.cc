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
 * Description: Transforms the given frame from the given sprite
 *
 * Input: Editor sprite and position (Default is zero)
 *
 * Output: Returns the transformed pixmap
 */
QPixmap TileRender::transformPixmap(EditorSprite* pic, int pos)
{
  QTransform transform;

  if(pic->getHorizontalFlip(pos))
  {
    qreal m11 = transform.m11();    // Horizontal scaling
    qreal m12 = transform.m12();    // Vertical shearing
    qreal m13 = transform.m13();    // Horizontal Projection
    qreal m21 = transform.m21();    // Horizontal shearing
    qreal m22 = transform.m22();    // vertical scaling
    qreal m23 = transform.m23();    // Vertical Projection
    qreal m31 = transform.m31();    // Horizontal Position (DX)
    qreal m32 = transform.m32();    // Vertical Position (DY)
    qreal m33 = transform.m33();    // Addtional Projection Factor

    qreal scale = m11;

    m11 = -m11;

    // Re-position back to origin
    if(m31 > 0)
      m31 = 0;
    else
      m31 = (64 * scale);

    // Write back to the matrix
    transform.setMatrix(m11, m12, m13, m21, m22, m23, m31, m32, m33);
  }
  if(pic->getVerticalFlip(pos))
  {
    qreal m11 = transform.m11();    // Horizontal scaling
    qreal m12 = transform.m12();    // Vertical shearing
    qreal m13 = transform.m13();    // Horizontal Projection
    qreal m21 = transform.m21();    // Horizontal shearing
    qreal m22 = transform.m22();    // vertical scaling
    qreal m23 = transform.m23();    // Vertical Projection
    qreal m31 = transform.m31();    // Horizontal Position (DX)
    qreal m32 = transform.m32();    // Vertical Position (DY)
    qreal m33 = transform.m33();    // Addtional Projection Factor

    qreal scale = m22;
    m22 = -m22;

    // Re-position back to origin
    if(m32 > 0)
      m32 = 0;
    else
      m32 = (64 * scale);

    // Write back to the matrix
    transform.setMatrix(m11, m12, m13, m21, m22, m23, m31, m32, m33);
  }
  QTransform trans = transform;
  transform = trans.rotate(pic->getFrameAngle(pos));
  QPixmap returnimage(pic->getPath(pos));
  return returnimage.transformed(transform);
}

QPixmap TileRender::setBrightness(int delta, QPixmap original)
{
  QImage original_image = original.toImage();
  QImage editing = original.toImage();
  QColor old_color;
  int r,g,b;

  for(int i=0; i<editing.width(); i++)
  {
    for(int j=0; j<editing.height(); j++)
    {
      old_color = QColor(original_image.pixel(i,j));
      r = old_color.red() + delta;
      g = old_color.green() + delta;
      b = old_color.blue() + delta;

      r = qBound(0,r,255);
      g = qBound(0,g,255);
      b = qBound(0,b,255);

      editing.setPixel(i,j,qRgb(r,g,b));
    }
  }
  QPixmap output;
  output = output.fromImage(editing);
  return output;
}

QPixmap TileRender::setColor(int deltared, int deltablue,
                             int deltagreen, QPixmap original)
{
  QImage original_image = original.toImage();
  QImage editing = original.toImage();
  QColor old_color;
  int r,g,b;

  for(int i=0; i<editing.width(); i++)
  {
    for(int j=0; j<editing.height(); j++)
    {
      old_color = QColor(original_image.pixel(i,j));
      r = old_color.red()+deltared-255;
      g = old_color.green()+deltablue-255;
      b = old_color.blue()+deltagreen-255;

      r = qBound(0,r,255);
      g = qBound(0,g,255);
      b = qBound(0,b,255);

      editing.setPixel(i,j,qRgb(r,g,b));
    }
  }
  QPixmap output;
  output = output.fromImage(editing);
  return output;
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
  {
    QPixmap base_image(base_layer->getPath(0));
    painter->setOpacity(base_layer->getOpacity()/100.0);
    base_image = setBrightness(base_layer->getBrightness()-255,base_image);
    base_image = setColor(base_layer->getColorRed(),base_layer->getColorBlue(),
                          base_layer->getColorGreen(),base_image);
    QTransform temp1;
    QTransform transformation1 =
        temp1.rotate(base_layer->getQuickRotation());
    QImage image = base_image.toImage();
    painter->drawPixmap(bound,base_image.transformed(transformation1));
  }
  if(enhancer && enhancer_layer !=NULL)
  {
    QPixmap enhancer_image(enhancer_layer->getPath(0));
    painter->setOpacity(enhancer_layer->getOpacity()/100.0);
    QTransform temp2;
    QTransform transformation2 =
        temp2.rotate(enhancer_layer->getQuickRotation());
    painter->drawPixmap(bound,enhancer_image.transformed(transformation2));
  }
  if(lower1 && lower_layers[0] !=NULL)
  {
    QPixmap lower_image(lower_layers[0]->getPath(0));
    painter->setOpacity(lower_layers[0]->getOpacity()/100.0);
    QTransform temp3;
    QTransform transformation3 =
        temp3.rotate(lower_layers[0]->getQuickRotation());
    painter->drawPixmap(bound,lower_image.transformed(transformation3));
  }
  if(lower2 && lower_layers[1] !=NULL)
  {
    QPixmap lower_image(lower_layers[1]->getPath(0));
    painter->setOpacity(lower_layers[1]->getOpacity()/100.0);
    QTransform temp4;
    QTransform transformation4 =
        temp4.rotate(lower_layers[1]->getQuickRotation());
    painter->drawPixmap(bound,lower_image.transformed(transformation4));
  }
  if(lower3 && lower_layers[2] !=NULL)
  {
    QPixmap lower_image(lower_layers[2]->getPath(0));
    painter->setOpacity(lower_layers[2]->getOpacity()/100.0);
    QTransform temp5;
    QTransform transformation5 =
        temp5.rotate(lower_layers[2]->getQuickRotation());
    painter->drawPixmap(bound,lower_image.transformed(transformation5));
  }
  if(lower4 && lower_layers[3] !=NULL)
  {
    QPixmap lower_image(lower_layers[3]->getPath(0));
    painter->setOpacity(lower_layers[3]->getOpacity()/100.0);
    QTransform temp6;
    QTransform transformation6 =
        temp6.rotate(lower_layers[3]->getQuickRotation());
    painter->drawPixmap(bound,lower_image.transformed(transformation6));
  }
  if(lower5 && lower_layers[4] !=NULL)
  {
    QPixmap lower_image(lower_layers[4]->getPath(0));
    painter->setOpacity(lower_layers[4]->getOpacity()/100.0);
    QTransform temp7;
    QTransform transformation7 =
        temp7.rotate(lower_layers[4]->getQuickRotation());
    painter->drawPixmap(bound,lower_image.transformed(transformation7));
  }
  if(upper1 && upper_layers[0] !=NULL)
  {
    QPixmap upper_image(upper_layers[0]->getPath(0));
    painter->setOpacity(upper_layers[0]->getOpacity()/100.0);
    QTransform temp8;
    QTransform transformation8 =
        temp8.rotate(upper_layers[0]->getQuickRotation());
    painter->drawPixmap(bound,upper_image.transformed(transformation8));
  }
  if(upper2 && upper_layers[1] !=NULL)
  {
    QPixmap upper_image(upper_layers[1]->getPath(0));
    painter->setOpacity(upper_layers[1]->getOpacity()/100.0);
    QTransform temp9;
    QTransform transformation9 =
        temp9.rotate(upper_layers[1]->getQuickRotation());
    painter->drawPixmap(bound,upper_image.transformed(transformation9));
  }
  if(upper3 && upper_layers[2] !=NULL)
  {
    QPixmap upper_image(upper_layers[2]->getPath(0));
    painter->setOpacity(upper_layers[2]->getOpacity()/100.0);
    QTransform temp10;
    QTransform transformation10 =
        temp10.rotate(upper_layers[2]->getQuickRotation());
    painter->drawPixmap(bound,upper_image.transformed(transformation10));
  }
  if(upper4 && upper_layers[3] !=NULL)
  {
    QPixmap upper_image(upper_layers[3]->getPath(0));
    painter->setOpacity(upper_layers[3]->getOpacity()/100.0);
    QTransform temp11;
    QTransform transformation11 =
        temp11.rotate(upper_layers[3]->getQuickRotation());
    painter->drawPixmap(bound,upper_image.transformed(transformation11));
  }
  if(upper5 && upper_layers[4] !=NULL)
  {
    QPixmap upper_image(upper_layers[4]->getPath(0));
    painter->setOpacity(upper_layers[4]->getOpacity()/100.0);
    QTransform temp12;
    QTransform transformation12 =
        temp12.rotate(upper_layers[4]->getQuickRotation());
    painter->drawPixmap(bound,upper_image.transformed(transformation12));
  }
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
    qDebug() << gameTile()->getBasePassability(Direction::NORTH) << " " << gameTile()->getLowerPassability(Direction::NORTH);
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

