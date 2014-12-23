/*******************************************************************************
 * Class Name: GameView
 * Date Created: December 22, 2014
 * Inheritance: QWidget
 * Description: View of the game editor component for all game related data:
 *              maps,
 ******************************************************************************/
#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QWidget>

class GameView : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  GameView(QWidget* parent = NULL);

  /* Destructor function */
  ~GameView();

private:
  /* The number of same images that trail this one */
//  int followers;

protected:
  /* Paints the sprite in a bounding box */
//  void paintEvent(QPaintEvent *);

public slots:
  /* Loads the image with a valid given path */
//  void loadSprite(QString path);

signals:
  /* Emits to parent when selected to deselect all others */
//  void chosen(int);
};
#endif // GAMEVIEW_H
