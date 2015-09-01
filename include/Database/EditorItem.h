/*******************************************************************************
 * Class Name: EditorItem
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Item
 ******************************************************************************/
#ifndef EDITORITEM_H
#define EDITORITEM_H

#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

#include "Database/EditorTemplate.h"
#include "Dialog/SpriteDialog.h"
#include "EditorHelpers.h"
#include "FileHandler.h"
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
  /* Check Box Widgets */
  QCheckBox* chk_f_bubby;
  QCheckBox* chk_f_consumed;
  QCheckBox* chk_f_defensive;
  QCheckBox* chk_f_equip;
  QCheckBox* chk_f_gene;
  QCheckBox* chk_f_healing;
  QCheckBox* chk_f_key;
  QCheckBox* chk_f_material;
  QCheckBox* chk_f_money;
  QCheckBox* chk_f_nocat;
  QCheckBox* chk_f_offensive;
  QCheckBox* chk_f_relieving;
  QCheckBox* chk_f_skilllearn;
  QCheckBox* chk_f_statalter;
  QCheckBox* chk_m_digital;
  QCheckBox* chk_m_electric;
  QCheckBox* chk_m_fire;
  QCheckBox* chk_m_forest;
  QCheckBox* chk_m_ice;
  QCheckBox* chk_m_nihil;

  /* Combo Box Widgets */
  QComboBox* combo_buff_preset;
  QComboBox* combo_tier;
  QComboBox* combo_use;

  /* The thumbnail and animation frame(s) control dialogs */
  SpriteDialog* dialog_anim;
  FrameDialog* dialog_thumb;

  /* Label Widgets */
  QLabel* lbl_anim_img;
  QLabel* lbl_thumb_img;

  /* Line Edit Widgets */
  QLineEdit* edit_brief_desc;
  QLineEdit* edit_id;
  QLineEdit* edit_msg;
  QLineEdit* edit_name;
  QLineEdit* edit_prefix;

  /* Editor ID */
  int id;

  /* Item data */
  Item item_base;
  Item item_curr;

  /* Radio Button Widgets */
  QRadioButton* radio_brass;
  QRadioButton* radio_graphene;
  QRadioButton* radio_nonphysical;
  QRadioButton* radio_physical;
  QRadioButton* radio_steel;
  QRadioButton* radio_titanium;
  QRadioButton* radio_wood;

  /* Spin Box Widgets */
  QVector<QSpinBox*> spin_buffs;
  QSpinBox* spin_dura;
  QSpinBox* spin_mass;
  QSpinBox* spin_value;

  /* Sprite data */
  EditorSprite sprite_anim;
  EditorSprite sprite_anim_base;
  EditorSprite sprite_thumb;
  EditorSprite sprite_thumb_base;

  /* Text Edit Widgets */
  QTextEdit* text_desc;

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorItem &source);

  /* Creates interface layout */
  void createLayout();

  /* Loads working info into UI objects */
  void loadWorkingInfo();

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Name changed within item widget signal */
  void nameChange(QString);

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/
public slots:

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the ID of the item */
  virtual int getID() const;

  /* Returns the name of the item */
  virtual QString getName() const;

  /* Returns the name of the item for listing */
  virtual QString getNameList();

  /* Loads the object data */
  void load(XmlData data, int index);

  /* Resets the working set trigger */
  void resetWorking();

  /* Saves the object data */
  void save(FileHandler* fh, bool game_only = false);

  /* Saves the working set trigger */
  void saveWorking();

  /* Sets the ID of the item */
  virtual void setID(int id);

  /* Sets the name of the item */
  virtual void setName(QString name);

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  /* The copy operator */
  EditorItem& operator= (const EditorItem &source);
};

#endif // EDITORITEM_H
