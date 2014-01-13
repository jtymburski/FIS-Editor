/*******************************************************************************
 * Class Name: SpriteToolbox
 * Date Created: November 14, 2013
 * Inheritance: QWidget
 * Description: This class handles the toolbox on the side of the application
 ******************************************************************************/
#include "SpriteToolbox.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function - Set up toolbox
 *
 * Input: parent widget
 */
SpriteToolbox::SpriteToolbox(QWidget *parent) : QWidget(parent)
{
  /* Setup the selection buttons */
  directory = new QPushButton("Select Directory",this);
  directory->show();

  /* Sets up the string path to the sprites folder */
  QString sprites_dir;
  sprites_dir.append(QDir::current().absolutePath());
  sprites_dir.chop(7);
  sprites_dir.append("/Project/sprites");

  /* Setup the dialog that appears when a button is selected */
  select_files = new QFileDialog(this,tr("Select A Directory To View"),
                                 sprites_dir);
  select_files->setFileMode(QFileDialog::DirectoryOnly);
  select_files->setOption(QFileDialog::ShowDirsOnly,true);
  //select_files->setViewMode(QFileDialog::Detail);
  //select_files->setFileMode(QFileDialog::ExistingFiles);

  /* Connect the button press to the dialog opening slot */
  connect(directory,SIGNAL(clicked()),this,SLOT(openDialog()));
}

/*
 * Description: Destructor function
 *
 * Input: none
 */
SpriteToolbox::~SpriteToolbox()
{
  //qDebug()<<"Removing Sprite Toolbox";
  for(int i=0; i<sprites.size(); i++)
  {
    delete sprites[i];
    sprites[i] = NULL;
  }
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: The paint event for the toolbox, this mainly sets up the
 *              positions for all of the sprites that have been selected and
 *              added to the toolbar
 *
 * Inputs: Unused
 */
void SpriteToolbox::paintEvent(QPaintEvent *)
{
  /* Sets up the spacing of all the sprites that will appear in the box */
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
/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/

/*
 * Description: Opens the dialog box, adds all of the selected sprites to the
 *              toolbox.
 */
void SpriteToolbox::openDialog()
{
  /* Creates a temporary list of file paths to all of the selected images */
  QStringList filenames;

  /* Temporary list of detailed file info (For total path) */
  QFileInfoList fileinfolist;

  /* String list for file type filtering */
  QStringList filters;
  filters << "*.png";

  /* Opens the dialog, and stores info for all png's in the chosen directory */
  if(select_files->exec())
  {
    QString totalpath;
    totalpath.append(select_files->selectedFiles().at(0));
    fileinfolist = QDir(totalpath).entryInfoList(filters);
    filenames.clear();
    for(int i=0; i<sprites.size();i++)
      delete sprites[i];
    sprites.resize(0);
  }

  /* Stores the file paths */
  for(int i=0; i<fileinfolist.size(); i++)
    filenames.push_back(fileinfolist[i].absoluteFilePath());

  for(int i=0; i<filenames.size(); i++)
  {
    qDebug()<<filenames[i]<<endl;
  }
  fileinfolist.clear();


  /* Adds each sprite to the overall vector of sprites */

  if(filenames.size() != 0)
  {
    for(int i=0; i<filenames.size(); i++)
      sprites.push_back(new SpriteChoice(this,filenames.at(i),sprites.size()));

    /* Resizes the widget to accomodate each new row of sprites */
    resize(width(),68+(qCeil(sprites.size()/4.0)*68));
    /* Calls update to setup the view */
    update();
  }
}



/*
 * Description: Deselects all sprite choices except the calling sprite choice
 *
 * Input: ID number of Sprite Choice
 */
void SpriteToolbox::deselectOthers(int id)
{
  for(int i=0; i<sprites.size(); i++)
  {
    if(i != id)
      sprites.at(i)->deselect();
  }
}

/*
 * Description: Sets the currently selected sprite path to the chosen path
 *
 * Input: The path to the picked sprite
 */
void SpriteToolbox::setSprite(QString pick)
{
  path = pick;
  emit pathOfImage(path);
}
