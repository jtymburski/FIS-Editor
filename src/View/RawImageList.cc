/*******************************************************************************
 * Class Name: RawImageList
 * Date Created: November 14, 2013
 * Inheritance: QWidget
 * Description: This class handles the toolbox on the side of the application
 ******************************************************************************/
#include "View/RawImageList.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function - Set up toolbox
 *
 * Input: parent widget
 */
RawImageList::RawImageList(QWidget *parent, QFileSystemModel* module)
  : QWidget(parent)
{
  /* Sets up the directory module */
  directory_module = module;

  /* Setup the selection buttons */
  directory = new QPushButton("Select Directory",this);
  directory->hide();

  /* Sets up the string path to the sprites folder */
  QString sprites_dir;
  sprites_dir.append(QDir::current().absolutePath());
  sprites_dir.chop(7);
  sprites_dir.append("/Project/sprites/Map");

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
RawImageList::~RawImageList()
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
void RawImageList::paintEvent(QPaintEvent *)
{
  /* Sets up the spacing of all the sprites that will appear in the box */
  int spacing = 68;
  for(int i=0, j=0, k=0; i<sprites.size(); i++, k++)
  {
    sprites.at(i)->show();
    if(i%4 == 0 && i!=0)
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
void RawImageList::openDialog()
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
      sprites.push_back(new RawImage(this,filenames.at(i),sprites.size()));

    /* Resizes the widget to accomodate each new row of sprites */
    resize(width(),68+(qCeil(sprites.size()/4.0)*68));
    /* Calls update to setup the view */
    update();
  }
}

/*
 * Description: Opens the dialog box, adds all of the selected sprites to the
 *              toolbox.
 */
void RawImageList::switchDirectory(QModelIndex index)
{
  QString path = directory_module->filePath(index);

  /* Creates a temporary list of file paths to all of the selected images */
  QStringList filenames;

  /* Temporary list of detailed file info (For total path) */
  QFileInfoList fileinfolist;

  /* String list for file type filtering */
  QStringList filters;
  filters << "*.png";

  /* Opens the dialog, and stores info for all png's in the chosen directory */
  fileinfolist = QDir(path).entryInfoList(filters);
  filenames.clear();
  for(int i=0; i<sprites.size();i++)
    delete sprites[i];
  sprites.resize(0);

  /* Stores the file paths */
  for(int i=0; i<fileinfolist.size(); i++)
    filenames.push_back(fileinfolist[i].absoluteFilePath());

  fileinfolist.clear();

  /* Adds each sprite to the overall vector of sprites */
  if(filenames.size() != 0)
  {
    QStringList chopped_names;
    QStringList filtered_names;

    for(int i=0; i < filenames.size(); i++)
    {
      int path_to = filenames[i].lastIndexOf('.');
      QString temp = filenames[i];
      temp.chop(temp.size() - path_to);
      QString temp2 = temp;
      temp2.chop(2);

      /* Only add filtered names if they match integers at end */
      if(temp.at(temp.size() - 1).isDigit() &&
         temp.at(temp.size() - 2).isDigit())
      {
        filtered_names.push_back(temp2);
      }
      chopped_names.push_back(temp2);
    }
    for(int i = 0; i < filenames.size(); i++)
    {
      int samecount = 0;
      for(int j = 0; j<filtered_names.size(); j++)
        if(chopped_names[i] == filtered_names[j])
          samecount++;

      sprites.push_back(new RawImage(this,filenames.at(i),
                                     sprites.size(),samecount));
    }

    /* Calls update to setup the view */
    update();
  }

  /* Resizes the widget to accomodate each new row of sprites */
  resize(width(),68+(qCeil(sprites.size()/4.0)*68));
}

/*
 * Description: Deselects all sprite choices except the calling sprite choice
 *
 * Input: ID number of Sprite Choice
 */
void RawImageList::deselectOthers(int id)
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
void RawImageList::setSprite(QString pick)
{
  path = pick;
  emit pathOfImage(path);
}
