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
void TileWrapper::paint(QPainter *painter,
                        const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  QRect bound(xpos*64,ypos*64,64,64);
  if(base && base_layer != NULL)
  {
    QPixmap base_image(base_layer->getPath(0));
    QTransform temp1;
    QTransform transformation1 = temp1.rotate(base_layer->getQuickRotation());
    painter->drawPixmap(bound,base_image.transformed(transformation1));
  }
  if(enhancer && enhancer_layer !=NULL)
  {
    QPixmap enhancer_image(enhancer_layer->getPath(0));
    QTransform temp2;
    QTransform transformation2 =
        temp2.rotate(enhancer_layer->getQuickRotation());
    painter->drawPixmap(bound,enhancer_image.transformed(transformation2));
  }
  if(lower1 && lower_layers[0] !=NULL)
  {
    QPixmap lower_image(lower_layers[0]->getPath(0));
    QTransform temp3;
    QTransform transformation3 =
        temp3.rotate(lower_layers[0]->getQuickRotation());
    painter->drawPixmap(bound,lower_image.transformed(transformation3));
  }
  if(lower2 && lower_layers[1] !=NULL)
  {
    QPixmap lower_image(lower_layers[1]->getPath(0));
    QTransform temp4;
    QTransform transformation4 =
        temp4.rotate(lower_layers[1]->getQuickRotation());
    painter->drawPixmap(bound,lower_image.transformed(transformation4));
  }
  if(lower3 && lower_layers[2] !=NULL)
  {
    QPixmap lower_image(lower_layers[2]->getPath(0));
    QTransform temp5;
    QTransform transformation5 =
        temp5.rotate(lower_layers[2]->getQuickRotation());
    painter->drawPixmap(bound,lower_image.transformed(transformation5));
  }
  if(lower4 && lower_layers[3] !=NULL)
  {
    QPixmap lower_image(lower_layers[3]->getPath(0));
    QTransform temp6;
    QTransform transformation6 =
        temp6.rotate(lower_layers[3]->getQuickRotation());
    painter->drawPixmap(bound,lower_image.transformed(transformation6));
  }
  if(lower5 && lower_layers[4] !=NULL)
  {
    QPixmap lower_image(lower_layers[4]->getPath(0));
    QTransform temp7;
    QTransform transformation7 =
        temp7.rotate(lower_layers[4]->getQuickRotation());
    painter->drawPixmap(bound,lower_image.transformed(transformation7));
  }
  if(upper1 && upper_layers[0] !=NULL)
  {
    QPixmap upper_image(upper_layers[0]->getPath(0));
    QTransform temp8;
    QTransform transformation8 =
        temp8.rotate(upper_layers[0]->getQuickRotation());
    painter->drawPixmap(bound,upper_image.transformed(transformation8));
  }
  if(upper2 && upper_layers[1] !=NULL)
  {
    QPixmap upper_image(upper_layers[1]->getPath(0));
    QTransform temp9;
    QTransform transformation9 =
        temp9.rotate(upper_layers[1]->getQuickRotation());
    painter->drawPixmap(bound,upper_image.transformed(transformation9));
  }
  if(upper3 && upper_layers[2] !=NULL)
  {
    QPixmap upper_image(upper_layers[2]->getPath(0));
    QTransform temp10;
    QTransform transformation10 =
        temp10.rotate(upper_layers[2]->getQuickRotation());
    painter->drawPixmap(bound,upper_image.transformed(transformation10));
  }
  if(upper4 && upper_layers[3] !=NULL)
  {
    QPixmap upper_image(upper_layers[3]->getPath(0));
    QTransform temp11;
    QTransform transformation11 =
        temp11.rotate(upper_layers[3]->getQuickRotation());
    painter->drawPixmap(bound,upper_image.transformed(transformation11));
  }
  if(upper5 && upper_layers[4] !=NULL)
  {
    QPixmap upper_image(upper_layers[4]->getPath(0));
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
}

/*
 * Description: Returns the bounding rectangle (Needed by API)
 */
QRectF TileWrapper::boundingRect() const
{
  return QRectF(xpos*64,ypos*64,64,64);
}

/*
 * Description: Places the currently selected sprite onto the active map layer
 */
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
 * Description: Removes the currently selected sprite onto the active map layer
 */
void TileWrapper::unplace()
{
  switch(active_layer)
  {
    case EditorEnumDb::BASE:
      base_layer = NULL;
      break;
    case EditorEnumDb::ENHANCER:
      enhancer_layer = NULL;
      break;
    case EditorEnumDb::LOWER1:
      lower_layers[0] = NULL;
      break;
    case EditorEnumDb::LOWER2:
      lower_layers[1] = NULL;
      break;
    case EditorEnumDb::LOWER3:
      lower_layers[2] = NULL;
      break;
    case EditorEnumDb::LOWER4:
      lower_layers[3] = NULL;
      break;
    case EditorEnumDb::LOWER5:
      lower_layers[4] = NULL;
      break;
    case EditorEnumDb::UPPER1:
      upper_layers[0] = NULL;
      break;
    case EditorEnumDb::UPPER2:
      upper_layers[1] = NULL;
      break;
    case EditorEnumDb::UPPER3:
      upper_layers[2] = NULL;
      break;
    case EditorEnumDb::UPPER4:
      upper_layers[3] = NULL;
      break;
    case EditorEnumDb::UPPER5:
      upper_layers[4] = NULL;
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

/*
 * Description: Sets the grid color
 *
 * Input: Color input (true: Selected, false: Unselected)
 */
void TileWrapper::setGridColor(bool color)
{
  grid_color = color;
  update();
}

void TileWrapper::editBase(EditorSprite *selection)
{}
void TileWrapper::editEnhancer(EditorSprite *selection)
{}

void TileWrapper::editLower(EditorSprite *selection, int position)
{}

void TileWrapper::editUpper(EditorSprite *selection, int position)
{}

