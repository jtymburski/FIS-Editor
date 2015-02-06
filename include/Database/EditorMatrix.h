/*******************************************************************************
 * Class Name: EditorMatrix
 * Date Created: February 1, 2015
 * Inheritance: QGraphicsScene
 * Description: This class is for the editing sprite matrix, which contains a 
 *              scene of EditorTileSprites that are used for thing creation.
 ******************************************************************************/
#ifndef EDITORMATRIX_H
#define EDITORMATRIX_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QVector>

#include "Database/EditorTileSprite.h"
#include "EditorEnumDb.h"

class EditorMatrix : public QGraphicsScene
{
  /* Constructor function */
  EditorMatrix(int width = 0, int height = 0);

  /* Copy constructor */
  EditorMatrix(const EditorMatrix &source);

  /* Destructor function */
  ~EditorMatrix();

private:
  /* Active hover sprite */
  EditorTileSprite* active_sprite;

  /* Cursor type */
  EditorEnumDb::ThingCursor cursor_mode;

  /* The structure of tiles in the matrix */
  QVector<QVector<EditorTileSprite*>> matrix;

  /* Visibility painting control */
  bool visible_grid;
  bool visible_passability;
  bool visible_render;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Adds the frames to the selected active sprite */
  void addFramesOnActive();

  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorMatrix &source);

  /* Decrements the render depth on the active tile */
  bool decrementDepthOnActive();

  /* Increments the render depth on the active tile */
  bool incrementDepthOnActive();

  /* Removes the frames from the selected active sprite */
  void removeFramesOnActive();

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
    /* Mouse move, press, and release events */
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Cleans the scene. Used for when adding or removing from view */
  void cleanScene();

  /* Decrease the width and height, by the count factor */
  void decreaseHeight(int count = 1);
  void decreaseWidth(int count = 1);

  /* Returns the height of the matrix */
  int getHeight();

  /* Gets the visibility for control objects */
  bool getVisibilityGrid();
  bool getVisibilityPass();
  bool getVisibilityRender();

  /* Returns the width of the matrix */
  int getWidth();

  /* Increase the width and height, by the count factor */
  void increaseHeight(int count = 1);
  void increaseWidth(int count = 1);
  
  /* Sets the cursor mode */
  void setCursorMode(EditorEnumDb::ThingCursor mode);
  
  /* Sets the visibility for control objects */
  void setVisibilityGrid(bool visible, bool force = false);
  void setVisibilityPass(bool visible, bool force = false);
  void setVisibilityRender(bool visible, bool force = false);

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  /* The copy operator */
  EditorMatrix& operator= (const EditorMatrix &source);
};

#endif // EDITORMATRIX_H
