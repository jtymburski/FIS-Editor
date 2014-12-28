#ifndef EDITORITEM_H
#define EDITORITEM_H
/*******************************************************************************
 * Class Name: EditorItem
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Item
 ******************************************************************************/
#include <QWidget>
class EditorItem : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorItem(QWidget* parent = NULL);

  /* Destructor function */
  ~EditorItem();

  /* Clone */
  EditorItem* clone();

private:
protected:
public slots:
signals:

};
#endif // EDITORITEM_H
