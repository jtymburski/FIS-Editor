/*******************************************************************************
 * Class Name: EditorParty
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Party
 ******************************************************************************/
#ifndef EDITORPARTY_H
#define EDITORPARTY_H

#include <QInputDialog>
#include <QWidget>

#include "Database/EditorPerson.h"
#include "Database/EditorTemplate.h"
#include "EditorHelpers.h"
#include "Game/Player/Party.h"

class EditorParty : public QWidget, public EditorTemplate
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorParty(QWidget* parent = NULL);

  /* Constructor function with id and name */
  EditorParty(int id, QString name, QWidget* parent = NULL);

  /* Copy constructor */
  EditorParty(const EditorParty &source);

  /* Destructor function */
  virtual ~EditorParty();

private:
  /* Button Widgets */
  QPushButton* btn_item_add;
  QPushButton* btn_item_rem;
  QPushButton* btn_person_add;
  QPushButton* btn_person_down;
  QPushButton* btn_person_rem;
  QPushButton* btn_person_up;

  /* Combo Box Widgets */
  QComboBox* combo_classify;

  /* Line Edit Widgets */
  QLineEdit* edit_id;
  QLineEdit* edit_name;

  /* Editor ID */
  int id;

  /* Item Information */
  QMap<int,int> item_set;
  QMap<int,int> item_set_base;
  QVector<EditorItem*> items_all;

  /* Label Widgets - just for displaying text */
  QLabel* lbl_inv_details;
  QLabel* lbl_person_details;

  /* List Widgets */
  QListWidget* list_items_all;
  QListWidget* list_items_used;
  QListWidget* list_persons_all;
  QListWidget* list_persons_used;

  /* Editor name */
  QString name_base;
  QString name_curr;

  /* The reference party for data */
  Party party_base;
  Party party_curr;

  /* Person Information */
  QVector<QPair<int,int>> person_set;
  QVector<QPair<int,int>> person_set_base;
  QVector<EditorPerson*> persons_all;

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorParty &source);

  /* Creates interface layout */
  void createLayout();

  /* Returns calculated mass of inventory and items by ID */
  int getInvMass();
  int getInvMassRemain();
  EditorItem* getItem(int id);

  /* Loads working info into UI objects */
  void loadWorkingInfo();

  /* Sort Person Set */
  void sortPersons();

  /* Update object lists */
  void updateItemList();
  void updatePersonList();

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Name changed within player widget signal */
  void nameChange(QString);

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/
public slots:
  /* Button Triggers */
  void btnItemAdd();
  void btnItemRemove();
  void btnPersonAdd();
  void btnPersonDown();
  void btnPersonRemove();
  void btnPersonUp();
  void btnReset();
  void btnSave();

  /* Widget Change Triggers */
  void changedClassify(QString index);
  void changedName(QString name);

  /* List Index Widget Changes */
  void listItemAllChanged(int index);
  void listItemAllDClicked(QListWidgetItem*);
  void listItemEdited(QListWidgetItem*);
  void listItemUsedChanged(int index);
  void listPersonAllChanged(int index);
  void listPersonAllDClicked(QListWidgetItem*);
  void listPersonEdited(QListWidgetItem*);
  void listPersonUsedChanged(int index);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the ID of the party */
  virtual int getID() const;

  /* Returns the name of the party */
  virtual QString getName() const;

  /* Returns the name of the party for listing */
  virtual QString getNameList();

  /* Loads the object data */
  void load(XmlData data, int index);

  /* Resets the working set trigger */
  void resetWorking();

  /* Saves the object data */
  void save(FileHandler* fh, bool game_only = false);

  /* Saves the working set trigger */
  void saveWorking();

  /* Sets the ID of the party */
  virtual void setID(int id);

  /* Sets the name of the party */
  virtual void setName(QString name, bool update = true);

  /* Update Calls for data */
  void updateItems(QVector<EditorItem*> items,
                   bool update_working = true);
  void updatePersons(QVector<EditorPerson*> persons,
                     bool update_working = true);

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  /* The copy operator */
  EditorParty& operator= (const EditorParty &source);
};

#endif // EDITORPARTY_H
