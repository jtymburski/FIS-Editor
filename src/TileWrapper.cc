/*******************************************************************************
 * Class Name: TileWrapper
 * Date Created: December 26, 2013
 * Inheritance: QWidget
 * Description: A tile representation on the mapeditor
 ******************************************************************************/
#include "TileWrapper.h"
#include <QDebug>
/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function
 *
 * Input: x/y coordinates on the map grid and a z depth
 */
TileWrapper::TileWrapper(int x, int y, int z)
{
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
  tile = new Tile();
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
TileWrapper::~TileWrapper()
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
void TileWrapper::setBase(bool toggle)
{
  base = toggle;
}

/*
 * Description: Sets the layer visibility
 *
 * Input: Visibility toggle
 */
void TileWrapper::setEnhancer(bool toggle)
{
  enhancer = toggle;
}

/*
 * Description: Sets the layer visibility
 *
 * Input: Visibility toggle
 */
void TileWrapper::setLower1(bool toggle)
{
  lower1 = toggle;
}

/*
 * Description: Sets the layer visibility
 *
 * Input: Visibility toggle
 */
void TileWrapper::setLower2(bool toggle)
{
  lower2 = toggle;
}

/*
 * Description: Sets the layer visibility
 *
 * Input: Visibility toggle
 */
void TileWrapper::setLower3(bool toggle)
{
  lower3 = toggle;
}

/*
 * Description: Sets the layer visibility
 *
 * Input: Visibility toggle
 */
void TileWrapper::setLower4(bool toggle)
{
  lower4 = toggle;
}

/*
 * Description: Sets the layer visibility
 *
 * Input: Visibility toggle
 */
void TileWrapper::setLower5(bool toggle)
{
  lower5 = toggle;
}

/*
 * Description: Sets the layer visibility
 *
 * Input: Visibility toggle
 */
void TileWrapper::setUpper1(bool toggle)
{
  upper1 = toggle;
}

/*
 * Description: Sets the layer visibility
 *
 * Input: Visibility toggle
 */
void TileWrapper::setUpper2(bool toggle)
{
  upper2 = toggle;
}

/*
 * Description: Sets the layer visibility
 *
 * Input: Visibility toggle
 */
void TileWrapper::setUpper3(bool toggle)
{
  upper3 = toggle;
}

/*
 * Description: Sets the layer visibility
 *
 * Input: Visibility toggle
 */
void TileWrapper::setUpper4(bool toggle)
{
  upper4 = toggle;
}

/*
 * Description: Sets the layer visibility
 *
 * Input: Visibility toggle
 */
void TileWrapper::setUpper5(bool toggle)
{
  upper5 = toggle;
}

/*
 * Description: Sets the grid visibility
 *
 * Input: Visibility toggle
 */
void TileWrapper::setGrid(bool toggle)
{
  grid = toggle;
}


/*
 * Description: Paints the tile
 *
 * Input: Required fields, mostly unused
 */
void TileWrapper::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
           QWidget *widget)
{
  QRect bound(xpos*64,ypos*64,64,64);
  if(base && base_layer != NULL)
    painter->drawImage(bound,QImage(base_layer->getPath(0)));
  if(enhancer && enhancer_layer !=NULL)
    painter->drawImage(bound,QImage(enhancer_layer->getPath(0)));
  if(lower1 && lower_layers[0] !=NULL)
    painter->drawImage(bound,QImage(lower_layers[0]->getPath(0)));
  if(lower2 && lower_layers[1] !=NULL)
    painter->drawImage(bound,QImage(lower_layers[1]->getPath(0)));
  if(lower3 && lower_layers[2] !=NULL)
    painter->drawImage(bound,QImage(lower_layers[2]->getPath(0)));
  if(lower4 && lower_layers[3] !=NULL)
    painter->drawImage(bound,QImage(lower_layers[3]->getPath(0)));
  if(lower5 && lower_layers[4] !=NULL)
    painter->drawImage(bound,QImage(lower_layers[4]->getPath(0)));
  if(upper1 && upper_layers[0] !=NULL)
    painter->drawImage(bound,QImage(upper_layers[0]->getPath(0)));
  if(upper2 && upper_layers[1] !=NULL)
    painter->drawImage(bound,QImage(upper_layers[1]->getPath(0)));
  if(upper3 && upper_layers[2] !=NULL)
    painter->drawImage(bound,QImage(upper_layers[2]->getPath(0)));
  if(upper4 && upper_layers[3] !=NULL)
    painter->drawImage(bound,QImage(upper_layers[3]->getPath(0)));
  if(upper5 && upper_layers[4] !=NULL)
    painter->drawImage(bound,QImage(upper_layers[4]->getPath(0)));
  if(grid)
  {
    painter->setPen(QColor(255,255,255,128));
    painter->drawRect(1+(xpos*64),1+(ypos*64),62,62);
  }
}

/*
 * Description: Returns the bounding rectangle (Needed by API)
 */
QRectF TileWrapper::boundingRect() const
{
  return QRectF(xpos*64,ypos*64,64,64);
}

void TileWrapper::place()
{
  switch(active_layer)
  {
    case EditorEnumDb::BASE:
      base_layer = toolbox->getCurrent();
      break;
    case EditorEnumDb::ENHANCER:
      enhancer_layer = toolbox->getCurrent();
      break;
    case EditorEnumDb::LOWER1:
      lower_layers[0] = toolbox->getCurrent();
      break;
    case EditorEnumDb::LOWER2:
      lower_layers[1] = toolbox->getCurrent();
      break;
    case EditorEnumDb::LOWER3:
      lower_layers[2] = toolbox->getCurrent();
      break;
    case EditorEnumDb::LOWER4:
      lower_layers[3] = toolbox->getCurrent();
      break;
    case EditorEnumDb::LOWER5:
      lower_layers[4] = toolbox->getCurrent();
      break;
    case EditorEnumDb::UPPER1:
      upper_layers[0] = toolbox->getCurrent();
      break;
    case EditorEnumDb::UPPER2:
      upper_layers[1] = toolbox->getCurrent();
      break;
    case EditorEnumDb::UPPER3:
      upper_layers[2] = toolbox->getCurrent();
      break;
    case EditorEnumDb::UPPER4:
      upper_layers[3] = toolbox->getCurrent();
      break;
    case EditorEnumDb::UPPER5:
      upper_layers[4] = toolbox->getCurrent();
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
void TileWrapper::setToolbox(EditorSpriteToolbox *tool)
{
  toolbox = tool;
}

void TileWrapper::editBase(EditorSprite *selection)
{}
void TileWrapper::editEnhancer(EditorSprite *selection)
{}

void TileWrapper::editLower(EditorSprite *selection, int position)
{}

void TileWrapper::editUpper(EditorSprite *selection, int position)
{}



