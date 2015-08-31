/*******************************************************************************
 * Class Name: EditorSkillset
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Skillset which contains a set of Editor Skills.
 ******************************************************************************/
#ifndef EDITORSKILLSET_H
#define EDITORSKILLSET_H

#include <QWidget>
#include <QInputDialog>
#include <QListWidget>
#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>

#include "Database/EditorSkill.h"
#include "Database/EditorTemplate.h"
#include "EditorHelpers.h"
#include "Game/Player/SkillSet.h"

class EditorSkillset : public QWidget, public EditorTemplate
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorSkillset(QWidget* parent = NULL);

  /* Constructor function with id and name */
  EditorSkillset(int id, QString name, QWidget* parent = NULL);

  /* Copy constructor */
  EditorSkillset(const EditorSkillset &source);

  /* Destructor function */
  virtual ~EditorSkillset();

private:
  /* Editor ID */
  int id;
  /* Editor Name */
  QString name;

  /* Buttons */
  QPushButton* btn_add;
  QPushButton* btn_remove;

  /* Line Edits */
  QLineEdit* edit_name;

  /* List Widgets */
  QListWidget* list_available;
  QListWidget* list_used;

  /* Data sets */
  QVector<QPair<int, int>> set_base;
  QVector<EditorSkill*> set_total;
  QVector<QPair<int, int>> set_working;

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorSkillset &source);

  /* Get skill pointer, based on ID */
  EditorSkill* getByID(int id);

  /* Loads working info into UI objects */
  void loadWorkingInfo();

  /* Get Skill string */
  QString skillString(EditorSkill* skill, int lvl = -1);

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  void nameChange(QString);

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/
public slots:
  /* Add skill to list trigger */
  void addSkill();

  /* Name changed trigger */
  void nameEdited(QString str);

  /* Remove skill from list trigger */
  void removeSkill();

  /* Resets the working set trigger */
  void resetWorking();

  /* Saves the working set trigger */
  void saveWorking();

  /* Set name and ID based on string */
  void setNameAndID(QString str);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the ID of the skillset */
  virtual int getID() const;

  /* Returns the name of the skillset */
  virtual QString getName() const;

  /* Returns the name of the skillset for listing */
  virtual QString getNameList();

  /* Loads the object data */
  virtual void load(XmlData data, int index);

  /* Saves the object data */
  virtual void save(FileHandler* fh, bool game_only = false);

  /* Sets the ID of the skillset */
  virtual void setID(int id);

  /* Sets the name of the skillset */
  virtual void setName(QString name);

  /* Update skills */
  void updateSkills(QVector<EditorSkill*> skills);

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  /* The copy operator */
  EditorSkillset& operator= (const EditorSkillset &source);
};

#endif // EDITORSKILLSET_H
