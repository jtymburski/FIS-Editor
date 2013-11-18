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
#include <QMouseEvent>
#include <QDebug>

class SpriteChoice : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  SpriteChoice(QWidget* parent = 0, QString path = 0, int id = 0);

  /* Destructor function */
  ~SpriteChoice();

  /* Enum for sprite border modes */
  enum Border {STANDARD, HOVERED, SELECTED };

public slots:
  /* Loads the image with a valid given path */
  void loadSprite(QString path);

  /* Deselects the sprite choice manually */
  void deselect();

protected:
  /* Paints the sprite in a bounding box */
  void paintEvent(QPaintEvent *);

  /* Mouse Enter, Leave and click events */
  void enterEvent(QEvent *);
  void leaveEvent(QEvent *);
  void mousePressEvent(QMouseEvent *);

private:
  /* A Pointer to the image stored */
  QImage* pic;

  /* The view mode for the border */
  Border mode;

  /* The path of the image stored */
  QString path;

  /* Id number */
  int id_number;

signals:
  /* Emits to parent when selected to deselect all others */
  void chosen(int);

  /* Emits to parent to send an image path */
  void pathOfImage(QString);
};
#endif // SPRITECHOICE_H
