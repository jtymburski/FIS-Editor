/*******************************************************************************
 * Class Name: SpriteToolbox
 * Date Created: November 14, 2013
 * Inheritance: QWidget
 * Description: This class handles the toolbox on the side of the application
 ******************************************************************************/
#include "SpriteToolbox.h"

SpriteToolbox::SpriteToolbox(QWidget *parent) : QWidget(parent)
{
  directory = new QPushButton("Select Directory",this);
  //directory->show();
  update();
}

SpriteToolbox::~SpriteToolbox()
{
}

void SpriteToolbox::paintEvent(QPaintEvent *)
{

}
