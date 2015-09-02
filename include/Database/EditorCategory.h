/*******************************************************************************
 * Class Name: EditorCategory
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: The category that defines persons and what they are capable of.
 ******************************************************************************/
#ifndef EDITORCATEGORY_H
#define EDITORCATEGORY_H

#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

#include "Database/EditorTemplate.h"
#include "EditorHelpers.h"
#include "Game/Player/Category.h"
#include "FileHandler.h"

class EditorCategory : public QWidget, public EditorTemplate
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorCategory(QWidget* parent = NULL);

  /* Constructor function with id and name */
  EditorCategory(int id, QString name, QWidget* parent = NULL);

  /* Copy constructor */
  EditorCategory(const EditorCategory &source);

  /* Destructor function */
  virtual ~EditorCategory();

private:
  /* The reference category for data */
  Category cat_base;
  Category cat_curr;

  /* Check Box Widgets */
  QCheckBox* chk_def;
  QCheckBox* chk_e_big_arms;
  QCheckBox* chk_e_claws;
  QCheckBox* chk_e_heavy_armor;
  QCheckBox* chk_e_light_armor;
  QCheckBox* chk_e_long_arms;
  QCheckBox* chk_e_mid_armor;
  QCheckBox* chk_e_small_arms;
  QCheckBox* chk_e_staff;
  QCheckBox* chk_e_sword;
  QCheckBox* chk_grd;
  QCheckBox* chk_imp;
  QCheckBox* chk_power_def;
  QCheckBox* chk_power_grd;
  bool chk_no_signals;

  /* Check Box Stack Widgets */
  QVector<QCheckBox*> chk_immunities;

  /* Combo Box Widgets */
  QComboBox* combo_qd;
  QComboBox* combo_stats_base;
  QComboBox* combo_stats_max;
  QComboBox* combo_vita;

  /* Line Edit Widgets */
  QLineEdit* edit_denonym;
  QLineEdit* edit_id;
  QLineEdit* edit_name;

  /* Editor ID */
  int id;

  /* Spin Box Stack Widgets */
  QVector<QSpinBox*> spin_atts_base;
  QVector<QSpinBox*> spin_atts_max;

  /* Text Edit Widgets */
  QTextEdit* text_desc;

  /*------------------- Constants -----------------------*/
  const static int kMAX_PRESETS; /* Max number of legitimate presets */

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorCategory &source);

  /* Creates interface layout */
  void createLayout();

  /* Loads working info into UI objects */
  void loadWorkingInfo();

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Name changed within category widget signal */
  void nameChange(QString);

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/
public slots:
  /* Button Triggers */
  void btnReset();
  void btnSave();

  /* Widget Change Triggers */
  void changedDenonym(QString denonym);
  void changedDescription();
  void changedFlags(int);
  void changedImmunities(int);
  void changedName(QString name);
  void changedRegenQD(QString qd);
  void changedRegenVita(QString vita);
  void changedStatsBase(QString);
  void changedStatsMax(QString);

  /* Stat Preset Triggers */
  void statBasePreset(int index);
  void statMaxPreset(int index);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the ID of the category */
  virtual int getID() const;

  /* Returns the name of the category */
  virtual QString getName() const;

  /* Returns the name of the category for listing */
  virtual QString getNameList();

  /* Loads the object data */
  void load(XmlData data, int index);

  /* Resets the working set trigger */
  void resetWorking();

  /* Saves the object data */
  void save(FileHandler* fh, bool game_only = false,
            QString wrapper = "category");

  /* Saves the working set trigger */
  void saveWorking();

  /* Sets the ID of the category */
  virtual void setID(int id);

  /* Sets the name of the category */
  virtual void setName(QString name);

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  /* The copy operator */
  EditorCategory& operator= (const EditorCategory &source);
};

#endif // EDITORCATEGORY_H
