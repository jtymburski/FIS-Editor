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
  void loadSprite(QString path);
protected:
  void paintEvent(QPaintEvent *);
  //void keyPressEvent(QKeyEvent *);
private:
  QImage* pic;
  QString path;
};
#endif // SPRITECHOICE_H
