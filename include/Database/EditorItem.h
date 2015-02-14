/*******************************************************************************
 * Class Name: EditorItem
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Item
 ******************************************************************************/
#ifndef EDITORITEM_H
#define EDITORITEM_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QGroupBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

#include "Database/EditorTemplate.h"
#include "EditorHelpers.h"
#include "Game/Player/Item.h"

class EditorItem : public QWidget, public EditorTemplate
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorItem(QWidget* parent = NULL);

  /* Constructor function with id and name */
  EditorItem(int id, QString name, QWidget* parent = NULL);

  /* Copy constructor */
  EditorItem(const EditorItem &source);

  /* Destructor function */
  virtual ~EditorItem();

private:
  /* Corresponding Item */
  Item item;
  Item base;
  Item working;

  /* Editor ID */
  int id;
  /* Editor Name */
  QString name;
  /* Set of Attributes */
  //AttributeSet* buff_set;

  /* Descriptions */
  QLabel* brief_description_label;
  QTextEdit* brief_description_edit;
  QLabel* description_label;
  QTextEdit* description_edit;

  QGroupBox* composition_box;
  QRadioButton* composition_wood;
  QRadioButton* composition_steel;
  QRadioButton* composition_brass;
  QRadioButton* composition_titanium;
  QRadioButton* composition_graphene;
  QRadioButton* composition_physical;
  QRadioButton* composition_nonphysical;
  QCheckBox* composition_fire;
  QCheckBox* composition_forest;
  QCheckBox* composition_ice;
  QCheckBox* composition_electric;
  QCheckBox* composition_digital;
  QCheckBox* composition_nihil;

  QLabel* max_durability_label;
  QLineEdit* max_durability_edit;
  QLabel* durability_label;
  QLineEdit* durability_edit;

  QGroupBox* itemflags_box;
  QCheckBox* itemflags_consumed;
  QCheckBox* itemflags_offensive;
  QCheckBox* itemflags_defensive;
  QCheckBox* itemflags_equipment;
  QCheckBox* itemflags_bubby;
  QCheckBox* itemflags_keyitem;
  QCheckBox* itemflags_material;
  QCheckBox* itemflags_genecomp;
  QCheckBox* itemflags_nocategory;
  QCheckBox* itemflags_stataltering;
  QCheckBox* itemflags_skilllearning;
  QCheckBox* itemflags_healingitem;
  QCheckBox* itemflags_relievingitem;
  QCheckBox* itemflags_money;

  QGroupBox* itemtier_box;
  QRadioButton* itemtier_trash;
  QRadioButton* itemtier_common;
  QRadioButton* itemtier_uncommon;
  QRadioButton* itemtier_rare;
  QRadioButton* itemtier_veryrare;
  QRadioButton* itemtier_legendary;
  QRadioButton* itemtier_none;

  QLabel* mass_label;
  QLineEdit* mass_edit;

  QLabel* name_label;
  QLineEdit* name_edit;

  QLabel* prefix_label;
  QLineEdit* prefix_edit;

  QGroupBox* occasion_box;
  QRadioButton* occasion_always;
  QRadioButton* occasion_battle;
  QRadioButton* occasion_menu;
  QRadioButton* occasion_none;

  QLabel* usingmessage_label;
  QLineEdit* usingmessage_edit;
  QLabel* value_label;
  QLineEdit* value_edit;
  QLabel* value_modifier;
  QLineEdit* value_modifier_edit;

  //TODO : Stuff to add after combination
  // thumbnail
  // using_skill
  // using_animation
  // using_sound




protected:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorItem &source);

public slots:
signals:

public:
  /* Clone */
  EditorItem* clone();

  /* Returns the ID of the item */
  virtual int getID() const;

  /* Returns the name of the item */
  virtual QString getName() const;

  /* Returns the name of the item for listing */
  virtual QString getNameList();

  /* Sets the ID of the item */
  virtual void setID(int id);

  /* Sets the name of the item */
  virtual void setName(QString name);

/* Operator functions */
public:
  /* The copy operator */
  EditorItem& operator= (const EditorItem &source);
};

#endif // EDITORITEM_H
