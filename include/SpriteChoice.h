/*******************************************************************************
 * Class Name: SpriteChoice
 * Date Created: November 11, 2013
 * Inheritance: QWidget
 * Description: A sprite representation that appears in the toolbox
 ******************************************************************************/
#ifndef SPRITECHOICE_H
#define SPRITECHOICE_H

#include <QImage>
#include <QWidget>
#include <QPainter>
#include <QDebug>

class SpriteChoice : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  SpriteChoice(QWidget* parent = 0, QString path = 0);

  /* Destructor function */
  ~SpriteChoice();

public slots:
  /* Loads the image with a valid given path */
  void loadSprite(QString path);

protected:
  /* Paints the sprite in a bounding box */
  void paintEvent(QPaintEvent *);

private:
  /* A Pointer to the image stored */
  QImage* pic;

  /* The path of the image stored */
  QString path;
};
#endif // SPRITECHOICE_H
