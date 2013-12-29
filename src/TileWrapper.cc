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
 * Description: Constructor function - Requires a path
 *
 * Input: File path
 */
TileWrapper::TileWrapper(QWidget* parent) : QWidget(parent)
{
  setFixedSize(64,64);
  QPalette pal(palette());
  pal.setColor(backgroundRole(),Qt::black);
  setAutoFillBackground(true);
  setPalette(pal);
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

TileWrapper::~TileWrapper()
{
  /*
  //qDebug()<<"Removing Tile Wrapper";
  for(int i=0; i<5; i++)
  {
    delete lower_layers[i];
    //qDebug()<<"    Removing Lower "<<i;
    delete upper_layers[i];
    //qDebug()<<"    Removing Upper "<<i;
  }
  delete base_layer;
  //qDebug()<<"    Removing Base";
  delete enhancer_layer;
  //qDebug()<<"    Removing Enhancer";
  delete tile;
  //qDebug()<<"    Removing Tile";
  */
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
void TileWrapper::setBase(bool toggle)
{
  base = toggle;
}

void TileWrapper::paintEvent(QPaintEvent *)
{
  QPainter painter(this);
  /* Draw Base */
  QRect bound(0,0,64,64);
  if(base && base_layer != NULL)
    painter.drawImage(bound,QImage(base_layer->getPath(0)));

  painter.setPen(QColor(255,255,255,128));
  painter.drawRect(1,1,62,62);

}

void TileWrapper::mousePressEvent(QMouseEvent *)
{
  base_layer = toolbox->getCurrent();
  update();
}

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



