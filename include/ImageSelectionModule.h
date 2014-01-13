/*******************************************************************************
 * Class Name: ImageSelectionModule
 * Date Created: January 12, 2014
 * Inheritance: QWidget
 * Description: A widget that contains both the SpriteToolbox and a QTreeView
 ******************************************************************************/
#ifndef IMAGESELECTIONMODULE_H
#define IMAGESELECTIONMODULE_H

#include <QTreeView>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QFileSystemModel>
#include "SpriteToolbox.h"

class ImageSelectionModule : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  ImageSelectionModule(QWidget* parent = 0);

  /* Destructor function */
  ~ImageSelectionModule();

  /* Gets the Toolbox */
  SpriteToolbox* getToolbox();
public slots:

protected:
  /* Sets up the painting of all selected files */
  void paintEvent(QPaintEvent *);

private:
  /* Toolbox scroll area */
  QScrollArea* images_tab_scrollwrapper;

  /* The Sprite Toolbox */
  SpriteToolbox* image_viewer;

  /* The Directory view */
  QTreeView* directory_tree;

signals:

};

#endif // IMAGESELECTIONMODULE_H
