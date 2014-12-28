#ifndef EDITORMAP_H
#define EDITORMAP_H
/*******************************************************************************
 * Class Name: EditorMap
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Map
 ******************************************************************************/
#include <QWidget>
class EditorMap : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorMap(QWidget* parent = NULL);

  /* Destructor function */
  ~EditorMap();

  /* Clone */
  EditorMap* clone();

private:
protected:
public slots:
signals:

};
#endif // EDITORMAP_H
