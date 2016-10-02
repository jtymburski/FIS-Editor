/*******************************************************************************
 * Class Name: RawImageView
 * Date Created: January 12, 2014
 * Inheritance: QWidget
 * Description: A widget that contains both the RawImageList and a QTreeView
 ******************************************************************************/
#ifndef RAWIMAGEVIEW_H
#define RAWIMAGEVIEW_H

#include <QTreeView>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QFileSystemModel>

#include "EditorHelpers.h"
#include "View/RawImageList.h"

class RawImageView : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  RawImageView(QWidget* parent = 0);

  /* Destructor function */
  ~RawImageView();

  /* Gets the Toolbox */
  RawImageList* getToolbox();
public slots:

protected:
  /* Sets up the painting of all selected files */
  void paintEvent(QPaintEvent *);

private:
  /* Toolbox scroll area */
  QScrollArea* images_tab_scrollwrapper;

  /* The Sprite Toolbox */
  RawImageList* image_viewer;

  /* The Directory view */
  QTreeView* directory_tree;

signals:

};

#endif // RAWIMAGEVIEW_H
