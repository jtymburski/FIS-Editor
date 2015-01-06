/*******************************************************************************
 * Class Name: EditorMap
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Map
 ******************************************************************************/
#ifndef EDITORMAP_H
#define EDITORMAP_H

#include <QWidget>

class EditorMap : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorMap(QWidget* parent = NULL);

  /* Destructor function */
  ~EditorMap();

private:
protected:
public slots:
signals:
public:
  /* Clone */
  EditorMap* clone();
};

#endif // EDITORMAP_H
