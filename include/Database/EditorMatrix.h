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
#include <QVector>

#include "Database/EditorTileSprite.h"

class EditorMatrix : public QGraphicsScene
{
  /* Constructor function */
  EditorMatrix(int width = 0, int height = 0);

  /* Copy constructor */
  EditorMatrix(const EditorMatrix &source);

  /* Destructor function */
  ~EditorMatrix();

private:
  /* The structure of tiles in the matrix */
  QVector<QVector<EditorTileSprite*>> matrix;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorMatrix &source);

protected:
public:
  /* Decrease the width and height, by the count factor */
  void decreaseHeight(int count = 1);
  void decreaseWidth(int count = 1);

  /* Returns the height of the matrix */
  int getHeight();

  /* Returns the width of the matrix */
  int getWidth();

  /* Increase the width and height, by the count factor */
  void increaseHeight(int count = 1);
  void increaseWidth(int count = 1);

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  /* The copy operator */
  EditorMatrix& operator= (const EditorMatrix &source);
};

#endif // EDITORMATRIX_H
