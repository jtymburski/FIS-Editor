/*******************************************************************************
 * Class Name: SizeSelector
 * Date Created: January 18, 2014
 * Inheritance: QDialog
 * Description: Picks the size for the initial map
 ******************************************************************************/
#ifndef SIZESELECTOR_H
#define SIZESELECTOR_H
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QDialog>
#include <QLineEdit>

class SizeSelector : public QDialog
{
  Q_OBJECT
public:
  /* Constructor */
  SizeSelector(QWidget* parent = 0);

  /* Destructor */
  ~SizeSelector();
public slots:

  /* Creates a map and sends it up */
  void createMap();

  /* Resize X */
  void setXSize(QString);

  /* resize Y */
  void setYSize(QString);
protected:
private:
  /* X Input Dialog */
  QLineEdit* sizexedit;

  /* Y Input Dialog */
  QLineEdit* sizeyedit;

  /* X and Y sizes */
  int sizex;
  int sizey;

signals:
  void createNewMap(int,int);
};

#endif // SIZESELECTOR_H
