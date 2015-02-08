/*******************************************************************************
 * Class Name: EditorMatrix
 * Date Created: February 1, 2015
 * Inheritance: QGraphicsScene
 * Description: This class is for the editing sprite matrix, which contains a 
 *              scene of EditorTileSprites that are used for thing creation.
 ******************************************************************************/
#ifndef EDITORMATRIX_H
#define EDITORMATRIX_H

#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
//#include <QMenu>
#include <QVector>

#include "Database/EditorTileSprite.h"
#include "EditorEnumDb.h"
#include "Helpers.h"

class EditorMatrix : public QGraphicsScene
{
  Q_OBJECT
public:
  /* Constructor function */
  EditorMatrix(int width = 0, int height = 0, bool black_back = false);

  /* Copy constructor */
  EditorMatrix(const EditorMatrix &source);

  /* Destructor function */
  ~EditorMatrix();

private:
  /* Active frame reference */
  int active_frame;

  /* Active hover sprite */
  EditorTileSprite* active_sprite;

  /* Cursor type */
  EditorEnumDb::ThingCursor cursor_mode;

  /* The edit all tile sprite */
  EditorTileSprite* edit_all;

  /* The structure of tiles in the matrix */
  QVector<QVector<EditorTileSprite*>> matrix;

  /* Placement location - the active sprite when clicked */
  int place_x;
  int place_y;

  /* Right click menu on matrix */
  //QMenu* rightclick_menu;

  /* Right clicked sprite */
  EditorTileSprite* rightclick_sprite;
  
  /* The rendering tile icons */
  TileIcons* tile_icons;
  
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

  /* Gets the first valid sprite */
  EditorTileSprite* getValidSprite();

  /* Increments the render depth on the active tile */
  bool incrementDepthOnActive();

  /* Removes the frames from the selected active sprite */
  void removeFramesOnActive();

  /* Size manipulation on matrix */
  bool setNewOrigin(int x, int y);
  bool setNewSize(int width, int height);

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
    /* Mouse move, press, and release events */
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Triggers a parent class to start matrix place process */ 
  void initMatrixPlace();

  /* Triggers the right click on the matrix */
  void rightClick(EditorTileSprite* clicked);

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* Triggered after edit all sprites is ok */
  void editSpritesOk();

  /* Matrix place sprite trigger */
  void matrixPlace(QString result_path, bool hflip, bool vflip);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Adds a path to the Editor Matrix. It can handle matrix format file names */
  bool addPath(QString path, int x = 0, int y = 0, bool hflip = false, 
               bool vflip = false, bool reset = false);
  bool addPath(QString root_path, QString file_name, int x = 0, int y = 0, 
               bool hflip = false, bool vflip = false, bool reset = false);

  /* Cleans the scene. Used for when adding or removing from view */
  void cleanScene(bool just_hover = false);

  /* Decrease the width and height, by the count factor */
  void decreaseHeight(int count = 1);
  void decreaseWidth(int count = 1);

  /* Called by parent to edit all sprites. Returns generic edit sprite */
  EditorTileSprite* editAllSprites();
  
  /* Returns the active frame index */
  int getActiveFrameIndex();

  /* Returns the height of the matrix */
  int getHeight() const;

  /* Returns the right clicked sprite */
  EditorTileSprite* getRightClicked();

  /* Gets trim values - rows, columns, and frames - that are unused */
  int getTrimFrames(bool largest = true);
  QRect getTrimRect();

  /* Gets the visibility for control objects */
  bool getVisibilityGrid();
  bool getVisibilityPass();
  bool getVisibilityRender();

  /* Returns the width of the matrix */
  int getWidth() const;

  /* Increase the width and height, by the count factor */
  void increaseHeight(int count = 1);
  void increaseWidth(int count = 1);

  /* Removes all sprites */
  void removeAll();

  /* Sets the active frame for all sprites in the matrix */
  void setActiveFrame(int index, bool force = false);

  /* Sets the cursor mode */
  void setCursorMode(EditorEnumDb::ThingCursor mode);
 
  /* Sets if all tiles should be horizontally or vertically flipped */
  void setFlipHorizontal(bool flip);
  void setFlipVertical(bool flip);
  
  /* Sets the rendering tile icons */
  void setTileIcons(TileIcons* icons); 
  
  /* Sets the visibility for control objects */
  void setVisibilityGrid(bool visible, bool force = false);
  void setVisibilityPass(bool visible, bool force = false);
  void setVisibilityRender(bool visible, bool force = false);

  /* Trims the scene  (removes excess sprites and frames) */
  void trim(bool only_se = true);

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  /* The copy operator */
  EditorMatrix& operator= (const EditorMatrix &source);
};

#endif // EDITORMATRIX_H
