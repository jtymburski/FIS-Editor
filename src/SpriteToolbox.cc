/*******************************************************************************
 * Class Name: SpriteToolbox
 * Date Created: November 14, 2013
 * Inheritance: QWidget
 * Description: This class handles the toolbox on the side of the application
 ******************************************************************************/
#include "SpriteToolbox.h"

SpriteToolbox::SpriteToolbox(QWidget *parent) : QWidget(parent)
{
  //layout = new QHBoxLayout(this);
  directory = new QPushButton("Select Directory",this);
  directory->show();
  connect(directory,SIGNAL(clicked()),this,SLOT(openDialog()));
}

SpriteToolbox::~SpriteToolbox()
{
}

void SpriteToolbox::paintEvent(QPaintEvent *)
{
  QPainter painter(this);

  int spacing = 68;
  for(int i=0, j=0, k=0; i<sprites.size(); i++, k++)
  {
    sprites.at(i)->show();
    if(i%4 == 0)
    {
      j+=spacing;
      k=0;
    }
    sprites.at(i)->move(spacing*k,j);
  }
}

void SpriteToolbox::openDialog()
{
  QStringList filenames = QFileDialog::getOpenFileNames(this, tr("Open File"),
                                               "/home",
                                               tr("Images (*.png *.jpg)"));
  if(filenames.size() != 0)
  {
    for(int i=0; i<filenames.size(); i++)
    {
      sprites.push_back(new SpriteChoice(this,filenames.at(i)));
      if(i%4 == 0)
      {
        resize(width(),height()+68);
      }
      //layout->addWidget(sprites[i]);
    }
    update();
  }
}
