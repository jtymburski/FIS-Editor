#include "SpriteChoice.h"

SpriteChoice::SpriteChoice(QWidget *parent, QString p) : QWidget(parent)
{
  setGeometry(0,0,66,66);
  path = p;
  pic = new QImage(path);
  pic->load(path);
}

SpriteChoice::~SpriteChoice()
{
}
void SpriteChoice::loadSprite(QString path)
{
  pic->load(path);
  update();
}

void SpriteChoice::paintEvent(QPaintEvent *)
{
  QPainter painter(this);
  if(path != NULL)
  {
    painter.drawRect(0,0,65,65);
    QRect* bound = new QRect(1,1,64,64);
    painter.drawImage(*bound,*pic);
  }
}
