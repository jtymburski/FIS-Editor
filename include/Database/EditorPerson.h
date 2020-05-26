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
  /* Group Box Widgets */
  QGroupBox* box_flags;

  /* Check Box Widgets */
  QCheckBox* chk_change_equip;
  QCheckBox* chk_gain_xp;
  QCheckBox* chk_learn_skills;
  QCheckBox* chk_lvl_up;
  bool chk_no_signals;

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
  QLabel* lbl_ally_img;
  QLabel* lbl_ally_def_img;
  QLabel* lbl_ally_off_img;
  QLabel* lbl_dialog_img;
  QLabel* lbl_face_img;
  QLabel* lbl_foe_img;
  QLabel* lbl_foe_off_img;
  QLabel* lbl_foe_def_img;

  /* The reference person for data */
  Person person_base;
  Person person_curr;

  /* Race information */
  int race_id;
  int race_id_base;
  QVector<EditorCategory*> race_total;

  /* Spin Box Widgets */
  QSpinBox* spin_actionx;
  QSpinBox* spin_actiony;
  //QSpinBox* spin_credits;
  QSpinBox* spin_xp;

  /* Sprite data */
  EditorSprite sprite_ally;
  EditorSprite sprite_ally_base;
  EditorSprite sprite_ally_off;
  EditorSprite sprite_ally_off_base;
  EditorSprite sprite_ally_def;
  EditorSprite sprite_ally_def_base;
  EditorSprite sprite_dialog;
  EditorSprite sprite_dialog_base;
  EditorSprite sprite_face;
  EditorSprite sprite_face_base;
  EditorSprite sprite_foe;
  EditorSprite sprite_foe_base;
  EditorSprite sprite_foe_off;
  EditorSprite sprite_foe_off_base;
  EditorSprite sprite_foe_def;
  EditorSprite sprite_foe_def_base;

public:
  /*------------------- Constants -----------------------*/
  const static int kELEMENT_COUNT; /* Number of elements */
  const static int kFRAME_SIZE; /* Size of frame containing sprites */

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorPerson &source);

  /* Creates interface layout */
  void createLayout();

  /* Gets element index for combo box */
  int getElementIndex(Element ele, ElementCurve curve);

  /* Loads working info into UI objects */
  void loadWorkingInfo();

  /* Update used working item list */
  void updateUsedItems();

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
  /* Button Triggers */
  void btnAlly(bool clean_only = false);
  void btnAllyDef(bool clean_only = false);
  void btnAllyOff(bool clean_only = false);
  void btnDialog(bool clean_only = false);
  void btnFace(bool clean_only = false);
  void btnFoe(bool clean_only = false);
  void btnFoeDef(bool clean_only = false);
  void btnFoeOff(bool clean_only = false);

  void btnItemAdd();
  void btnItemRemove();
  void btnReset();
  void btnSave();

  /* Widget Change Triggers */
  void changedActionX(int value);
  void changedActionY(int value);
  void changedClass(int index);
  void changedCredits(int value);
  void changedExperience(int value);
  void changedFlags(int);
  void changedName(QString name);
  void changedPrimary(QString text);
  void changedRace(int index);
  void changedSecondary(QString text);

  /* List Index Widget Changes */
  void listAllIndexChanged(int index);
  void listUsedIndexChanged(int index);

  /* Update Sprites */
  void updateAllySprite();
  void updateAllyDefSprite();
  void updateAllyOffSprite();
  void updateDialogSprite();
  void updateFaceSprite();
  void updateFoeSprite();
  void updateFoeDefSprite();
  void updateFoeOffSprite();

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
  virtual void setName(QString name, bool update = true);

  /* Update Calls for data */
  void updateClasses(QVector<EditorCategory*> classes,
                     bool update_working = true);
  void updateItems(QVector<EditorItem*> items,
                   bool update_working = true);
  void updateRaces(QVector<EditorCategory*> races,
                   bool update_working = true);

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  /* The copy operator */
  EditorPerson& operator= (const EditorPerson &source);
};

#endif // EDITORPERSON_H
