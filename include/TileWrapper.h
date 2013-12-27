/*******************************************************************************
 * Class Name: TileWrapper
 * Date Created: December 26, 2013
 * Inheritance: QWidget
 * Description: A tile representation on the mapeditor
 ******************************************************************************/
#ifndef TILEWRAPPER_H
#define TILEWRAPPER_H

#include <QWidget>
#include <QPainter>
#include <QList>
#include "EditorSprite.h"
#include "Game/Map/Tile.h"

class TileWrapper : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  TileWrapper(QWidget* parent = 0);

  /* Destructor Function */
  ~TileWrapper();
public slots:
  void editBase(EditorSprite* selection);
  void editEnhancer(EditorSprite* selection);
  void editLower(EditorSprite* selection, int position = 0);
  void editUpper(EditorSprite* selection, int position = 0);
protected:
  void paintEvent(QPaintEvent *);
private:
  Tile* tile;
  EditorSprite* base_layer;
  EditorSprite* enhancer_layer;
  QList<EditorSprite*> lower_layers;
  QList<EditorSprite*> upper_layers;
signals:
};

#endif // TILEWRAPPER_H
