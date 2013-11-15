/*******************************************************************************
 * Class Name: SpriteToolbox
 * Date Created: November 14, 2013
 * Inheritance: QWidget
 * Description: The side toolbar that gives access to sprites.
 ******************************************************************************/
#ifndef SPRITETOOLBOX_H
#define SPRITETOOLBOX_H

#include <QWidget>
#include <QPushButton>

class SpriteToolbox : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  SpriteToolbox(QWidget* parent = 0);

  /* Destructor function */
  ~SpriteToolbox();
protected:
  void paintEvent(QPaintEvent *);
private:
  QPushButton* directory;
};

#endif // SPRITETOOLBOX_H
