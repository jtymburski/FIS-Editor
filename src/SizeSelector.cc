/*******************************************************************************
 * Class Name: SizeSelector
 * Date Created: January 18, 2014
 * Inheritance: QDialog
 * Description: Picks the size for the initial map
 ******************************************************************************/

#include "SizeSelector.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function
 *
 */
SizeSelector::SizeSelector(QWidget *parent) : QDialog(parent)
{
  QGridLayout* layout = new QGridLayout(this);
  layout->addWidget(new QLabel("Choose Initial Map Size"),0,0);
  layout->addWidget(new QLabel("X:"),1,0);
  sizexedit = new QLineEdit("100",this);
  sizex = 100;
  //sizexedit->setInputMask("000");
  layout->addWidget(sizexedit,1,1);
  layout->addWidget(new QLabel("Y:"),2,0);
  sizeyedit = new QLineEdit("100",this);
  sizey = 100;
  //sizeyedit->setInputMask("000");
  layout->addWidget(sizeyedit,2,1);
  QPushButton* ok = new QPushButton("&Create Map",this);
  layout->addWidget(ok,3,0);
  connect(sizexedit,SIGNAL(textChanged(QString)),this,SLOT(setXSize(QString)));
  connect(sizeyedit,SIGNAL(textChanged(QString)),this,SLOT(setYSize(QString)));
  connect(ok,SIGNAL(clicked()),this,SLOT(createMap()));


}

/*
 * Description: Destructor Function
 *
 */

SizeSelector::~SizeSelector()
{

}

void SizeSelector::createMap()
{
  emit createNewMap(sizex,sizey);
  hide();
}

void SizeSelector::setXSize(QString size)
{
  if(size.toInt() > 200)
    size = "200";
  sizex = size.toInt();
}

void SizeSelector::setYSize(QString size)
{
  if(size.toInt() > 200)
    size = "200";
  sizey = size.toInt();
}
