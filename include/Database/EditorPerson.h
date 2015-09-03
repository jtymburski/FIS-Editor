/*******************************************************************************
 * Class Name: EditorPerson
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Person
 ******************************************************************************/
#ifndef EDITORPERSON_H
#define EDITORPERSON_H

#include <QWidget>

#include "Database/EditorCategory.h"
#include "Database/EditorItem.h"
#include "Database/EditorTemplate.h"
#include "EditorHelpers.h"
#include "FileHandler.h"
#include "Game/Player/Person.h"

class EditorPerson : public QWidget, public EditorTemplate
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorPerson(QWidget* parent = NULL);

  /* Constructor function with id and name */
  EditorPerson(int id, QString name, QWidget* parent = NULL);

  /* Copy constructor */
  EditorPerson(const EditorPerson &source);

  /* Destructor function */
  virtual ~EditorPerson();

private:
  /* Button Widgets */
  QPushButton* btn_item_add;
  QPushButton* btn_item_rem;

  /* Check Box Widgets */
  QCheckBox* chk_change_equip;
  QCheckBox* chk_gain_xp;
  QCheckBox* chk_learn_skills;
  QCheckBox* chk_lvl_up;

  /* Class information */
  int class_id;
  int class_id_base;
  QVector<EditorCategory*> class_total;

  /* Combo Box Widgets */
  QComboBox* combo_class;
  QComboBox* combo_pri;
  QComboBox* combo_race;
  QComboBox* combo_sec;

  /* The sprite edit dialog */
  SpriteDialog* dialog_sprite;

  /* Line Edit Widgets */
  QLineEdit* edit_id;
  QLineEdit* edit_name;

  /* Editor ID */
  int id;

  /* Item information */
  QVector<int> item_ids;
  QVector<int> item_ids_base;
  QVector<EditorItem*> item_total;

  /* Label Widgets */
  QLabel* lbl_as_img;
  QLabel* lbl_ds_img;
  QLabel* lbl_fp_img;
  QLabel* lbl_tp_img;

  /* List Widgets */
  QListWidget* list_items_all;
  QListWidget* list_items_used;

  /* The reference person for data */
  Person person_base;
  Person person_curr;

  /* Race information */
  int race_id;
  int race_id_base;
  QVector<EditorCategory*> race_total;

  /* Spin Box Widgets */
  QSpinBox* spin_credits;
  QSpinBox* spin_xp;

  /* Sprite data */
  EditorSprite sprite_as;
  EditorSprite sprite_as_base;
  EditorSprite sprite_ds;
  EditorSprite sprite_ds_base;
  EditorSprite sprite_fp;
  EditorSprite sprite_fp_base;
  EditorSprite sprite_tp;
  EditorSprite sprite_tp_base;

  /*------------------- Constants -----------------------*/
  const static int kFRAME_SIZE; /* Size of frame containing sprites */

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorPerson &source);

  /* Creates interface layout */
  void createLayout();

  /* Loads working info into UI objects */
  void loadWorkingInfo();

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Name changed within person widget signal */
  void nameChange(QString);

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/
public slots:

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the ID of the person */
  virtual int getID() const;

  /* Returns the name of the person */
  virtual QString getName() const;

  /* Returns the name of the person for listing */
  virtual QString getNameList();

  /* Loads the object data */
  void load(XmlData data, int index);

  /* Resets the working set trigger */
  void resetWorking();

  /* Saves the object data */
  void save(FileHandler* fh, bool game_only = false);

  /* Saves the working set trigger */
  void saveWorking();

  /* Sets the ID of the person */
  virtual void setID(int id);

  /* Sets the name of the person */
  virtual void setName(QString name);

  /* Update Calls for data */
  void updateClasses(QVector<EditorCategory*> classes); // TODO
  void updateItems(QVector<EditorItem*> items); // TODO
  void updateRaces(QVector<EditorCategory*> races); // TODO

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  /* The copy operator */
  EditorPerson& operator= (const EditorPerson &source);
};

#endif // EDITORPERSON_H
