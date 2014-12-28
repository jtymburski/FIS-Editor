#ifndef EDITORPARTY_H
#define EDITORPARTY_H
/*******************************************************************************
 * Class Name: EditorParty
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Party
 ******************************************************************************/
#include <QWidget>
class EditorParty : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorParty(QWidget* parent = NULL);

  /* Destructor function */
  ~EditorParty();

  /* Clone */
  EditorParty* clone();

private:
protected:
public slots:
signals:

};
#endif // EDITORPARTY_H
