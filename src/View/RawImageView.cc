/*******************************************************************************
 * Class Name: RawImageView
 * Date Created: January 12, 2014
 * Inheritance: QWidget
 * Description: A widget that contains both the RawImageList and a QTreeView
 ******************************************************************************/
#include "View/RawImageView.h"
#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function - Set up toolbox
 *
 * Input: parent widget
 */
RawImageView::RawImageView(QWidget *parent) : QWidget(parent)
{
  QVBoxLayout* layout = new QVBoxLayout(this);

  /* Sets up the file directory tree */
  QFileSystemModel *model = new QFileSystemModel(this);
  QString sprite_dir = EditorHelpers::getSpriteDir() + "/Map";
  model->setRootPath(sprite_dir);
  model->setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
  directory_tree = new QTreeView(this);
  directory_tree->setModel(model);
  directory_tree->setColumnHidden(1,true);
  directory_tree->setColumnHidden(2,true);
  directory_tree->setColumnHidden(3,true);
  directory_tree->setRootIndex(model->index(sprite_dir));

  /* Sets up a scroll area with the toolbox module */
  image_viewer = new RawImageList(this,model);
  image_viewer->setMinimumWidth(286);
  images_tab_scrollwrapper = new QScrollArea(this);
  images_tab_scrollwrapper->setBackgroundRole(QPalette::Dark);
  images_tab_scrollwrapper->setWidget(image_viewer);
  images_tab_scrollwrapper->setMinimumWidth(288);

  /* Connects clicking of directories to a change in the image viewer */
  connect(directory_tree->selectionModel(),
          SIGNAL(currentChanged(QModelIndex,QModelIndex)),
          image_viewer,SLOT(switchDirectory(QModelIndex)));

  layout->addWidget(directory_tree);//, 0, Qt::AlignHCenter);
  layout->addWidget(images_tab_scrollwrapper, 0, Qt::AlignHCenter);
  setLayout(layout);
}

/*
 * Description: Destructor function
 *
 * Input: none
 */
RawImageView::~RawImageView()
{
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
void RawImageView::paintEvent(QPaintEvent *)
{
}

/* Description : Returns the Sprite Toolbox
 *
 * Output: RawImageList
 */

RawImageList* RawImageView::getToolbox()
{
  return image_viewer;
}
