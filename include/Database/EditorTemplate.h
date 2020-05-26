/*******************************************************************************
 * Class Name: EditorTemplate
 * Date Created: December 27, 2014
 * Inheritance: none
 * Description: An interface class for Editor Objects for the common
 *              functionality between all.
 ******************************************************************************/
#ifndef EDITORTEMPLATE_H
#define EDITORTEMPLATE_H

class EditorTemplate
{
public:
  /* Virtual destructor */
  virtual ~EditorTemplate() {}

  /* Returns the ID of the editor object */
  virtual int getID() const = 0;

  /* Returns the name of the editor object */
  virtual QString getName() const = 0;

  /* Returns the ID and name of the object for listing */
  virtual QString getNameList() = 0;

  /* Sets the ID of the editor object */
  virtual void setID(int id) = 0;

  /* Sets the name of the editor object */
  virtual void setName(QString name, bool update) = 0;
};

#endif // EDITORTEMPLATE_H
