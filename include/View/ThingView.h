/*******************************************************************************
 * Class Name: ThingView
 * Date Created: February 13, 2015
 * Inheritance: QWidget
 * Description: The side toolbar in the map database that gives access to the
 *              map things.
 ******************************************************************************/
#ifndef THINGVIEW_H
#define THINGVIEW_H

#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "Database/EditorMap.h"
#include "Dialog/ThingDialog.h"
#include "EditorHelpers.h"
//#include "View/SpriteViewList.h"

class ThingView : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  ThingView(QWidget* parent = NULL);

  /* Destructor function */
  ~ThingView();

private:
  /* The current editing map */
  EditorMap* editor_map;

  /* Label views for selected thing */
  QLabel* lbl_id;
  QLabel* lbl_image;
  QLabel* lbl_name;
  QLabel* lbl_size;

  /* The thing dialog for editing */
  ThingDialog* thing_dialog;

  /* The list of things in the view */
  QListWidget* thing_list;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Adds an editor thing to the editing map */
  void addThing(EditorThing* thing);

  /* Creates the layout - only called on initial construction */
  void createLayout();

  /* Opens the thing editing dialog */
  void editThing();

  /* Refreshes the editor thing list in the view */
  void updateList();

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Sets up the painting of all selected files */
  //void paintEvent(QPaintEvent *);

  /* Views Frames */
  //void mouseDoubleClickEvent(QMouseEvent *e);

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Sends up the Editor Sprite */
  //void sendUpEditorSprite(EditorSprite* sprite);

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* Adds an Editor Sprite to the toolbox */
  //void addEditorSprite(EditorSprite* e);

  /* The current row changes in the list widget */
  void currentRowChanged(int index);

  /* Opens the sprite editing dialog */
  //void editSprite();

  /* An item in the list was double clicked */
  void itemDoubleClicked(QListWidgetItem*);

  /* Updates the sprite list */
  //void updateList();

  /* Views the frame sequence */
  //void viewFrameSequence();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Deletes the selected thing */
  bool deleteThing();

  /* Duplicates the selected thing */
  bool duplicateThing();

  /* Get current thing */
  EditorThing* getSelected();

  /* Gets the editor map */
  EditorMap* getEditorMap();

  /* Imports a thing from another file */
  void importThing();

  /* Creates a new blank thing */
  void newThing();

  /* Sets the editor map, which contains the data needed */
  void setEditorMap(EditorMap* map);
};
#endif // THINGVIEW_H
