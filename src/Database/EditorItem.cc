/*******************************************************************************
 * Class Name: EditorItem
 * Date Created: December 27, 2014
 * Inheritance: QWidget
 * Description: Editor Item
 ******************************************************************************/
#include "Database/EditorItem.h"

EditorItem::EditorItem(QWidget *parent) : QWidget(parent)
{
  QHBoxLayout* nameprefix_layout = new QHBoxLayout();
  QVBoxLayout* descriptions_layout = new QVBoxLayout();
  QGridLayout* values_layout = new QGridLayout();
  QVBoxLayout* left_layout = new QVBoxLayout();

  QHBoxLayout* grouptop_layout = new QHBoxLayout();
  QVBoxLayout* smallgroupbox_layout = new QVBoxLayout();
  QHBoxLayout* groupbottom_layout = new QHBoxLayout();
  QVBoxLayout* right_layout = new QVBoxLayout();

  QHBoxLayout* total_layout = new QHBoxLayout(this);

  name_label = new QLabel("Name",this);
  nameprefix_layout->addWidget(name_label);
  name_edit = new QLineEdit(this);
  nameprefix_layout->addWidget(name_edit);

  prefix_label = new QLabel("Prefix",this);
  nameprefix_layout->addWidget(prefix_label);
  prefix_edit = new QLineEdit(this);
  nameprefix_layout->addWidget(prefix_edit);

  brief_description_label = new QLabel("Brief Description",this);
  descriptions_layout->addWidget(brief_description_label);
  brief_description_edit = new QTextEdit(this);
  descriptions_layout->addWidget(brief_description_edit);
  description_label = new QLabel("Full Description",this);
  descriptions_layout->addWidget(description_label);
  description_edit = new QTextEdit(this);
  descriptions_layout->addWidget(description_edit);
  usingmessage_label = new QLabel("Using Message",this);
  descriptions_layout->addWidget(usingmessage_label);
  usingmessage_edit = new QLineEdit(this);
  descriptions_layout->addWidget(usingmessage_edit);

  max_durability_label = new QLabel("Maximum Durability",this);
  values_layout->addWidget(max_durability_label,0,0);
  max_durability_edit = new QLineEdit(this);
  values_layout->addWidget(max_durability_edit,0,1);
  durability_label = new QLabel("Durability",this);
  values_layout->addWidget(durability_label,1,0);
  durability_edit = new QLineEdit(this);
  values_layout->addWidget(durability_edit,1,1);
  mass_label = new QLabel("Mass",this);
  values_layout->addWidget(mass_label,2,0);
  mass_edit = new QLineEdit(this);
  values_layout->addWidget(mass_edit,2,1);
  value_label = new QLabel("Value",this);
  values_layout->addWidget(value_label,3,0);
  value_edit = new QLineEdit(this);
  values_layout->addWidget(value_edit,3,1);
  value_modifier = new QLabel("Value Modifier",this);
  values_layout->addWidget(value_modifier,4,0);
  value_modifier_edit = new QLineEdit(this);
  values_layout->addWidget(value_modifier_edit,4,1);

  left_layout->addLayout(nameprefix_layout);
  left_layout->addLayout(descriptions_layout);
  left_layout->addLayout(values_layout);

  total_layout->addLayout(left_layout);

  QVBoxLayout* composition_layout = new QVBoxLayout();
  composition_box = new QGroupBox("Composition",this);
  composition_wood = new QRadioButton("Wood",this);
  composition_layout->addWidget(composition_wood);
  composition_steel = new QRadioButton("Steel",this);
  composition_layout->addWidget(composition_steel);
  composition_brass = new QRadioButton("Brass",this);
  composition_layout->addWidget(composition_brass);
  composition_titanium = new QRadioButton("Titanium",this);
  composition_layout->addWidget(composition_titanium);
  composition_graphene = new QRadioButton("Graphene",this);
  composition_layout->addWidget(composition_graphene);
  composition_physical = new QRadioButton("Physical",this);
  composition_layout->addWidget(composition_physical);
  composition_nonphysical = new QRadioButton("Non-Physical",this);
  composition_layout->addWidget(composition_nonphysical);
  composition_fire = new QCheckBox("Fire",this);
  composition_layout->addWidget(composition_fire);
  composition_forest = new QCheckBox("Forest",this);
  composition_layout->addWidget(composition_forest);
  composition_ice = new QCheckBox("Ice",this);
  composition_layout->addWidget(composition_ice);
  composition_electric = new QCheckBox("Electric",this);
  composition_layout->addWidget(composition_electric);
  composition_digital = new QCheckBox("Digital",this);
  composition_layout->addWidget(composition_digital);
  composition_nihil = new QCheckBox("Nihil",this);
  composition_layout->addWidget(composition_nihil);
  composition_box->setLayout(composition_layout);


  QVBoxLayout* itemflags_layout = new QVBoxLayout();
  itemflags_box = new QGroupBox("Item Flags",this);
  itemflags_consumed = new QCheckBox("Consumed",this);
  itemflags_layout->addWidget(itemflags_consumed);
  itemflags_offensive = new QCheckBox("Offensive",this);
  itemflags_layout->addWidget(itemflags_offensive);
  itemflags_defensive = new QCheckBox("Defensive",this);
  itemflags_layout->addWidget(itemflags_defensive);
  itemflags_equipment = new QCheckBox("Equipment",this);
  itemflags_layout->addWidget(itemflags_equipment);
  itemflags_bubby = new QCheckBox("B.U.B.B.I.E.",this);
  itemflags_layout->addWidget(itemflags_bubby);
  itemflags_keyitem = new QCheckBox("Key Item",this);
  itemflags_layout->addWidget(itemflags_keyitem);
  itemflags_material = new QCheckBox("Material",this);
  itemflags_layout->addWidget(itemflags_material);
  itemflags_genecomp = new QCheckBox("Gene Composition",this);
  itemflags_layout->addWidget(itemflags_genecomp);
  itemflags_nocategory = new QCheckBox("No Category",this);
  itemflags_layout->addWidget(itemflags_nocategory);
  itemflags_stataltering = new QCheckBox("Stat Altering",this);
  itemflags_layout->addWidget(itemflags_stataltering);
  itemflags_skilllearning = new QCheckBox("Skill Learning",this);
  itemflags_layout->addWidget(itemflags_skilllearning);
  itemflags_healingitem = new QCheckBox("Healing Item",this);
  itemflags_layout->addWidget(itemflags_healingitem);
  itemflags_relievingitem = new QCheckBox("Relieving Item",this);
  itemflags_layout->addWidget(itemflags_relievingitem);
  itemflags_money = new QCheckBox("Money",this);
  itemflags_layout->addWidget(itemflags_money);
  itemflags_box->setLayout(itemflags_layout);

  grouptop_layout->addWidget(composition_box);
  grouptop_layout->addWidget(itemflags_box);

  QVBoxLayout* itemtier_layout = new QVBoxLayout();
  itemtier_box = new QGroupBox("Item Tier",this);
  itemtier_trash = new QRadioButton("Trash",this);
  itemtier_layout->addWidget(itemtier_trash);
  itemtier_common = new QRadioButton("Common",this);
  itemtier_layout->addWidget(itemtier_common);
  itemtier_uncommon = new QRadioButton("Uncommon",this);
  itemtier_layout->addWidget(itemtier_uncommon);
  itemtier_rare = new QRadioButton("Rare",this);
  itemtier_layout->addWidget(itemtier_rare);
  itemtier_veryrare = new QRadioButton("Very Rare",this);
  itemtier_layout->addWidget(itemtier_veryrare);
  itemtier_legendary = new QRadioButton("Legendary",this);
  itemtier_layout->addWidget(itemtier_legendary);
  itemtier_none = new QRadioButton("None",this);
  itemtier_layout->addWidget(itemtier_none);
  itemtier_box->setLayout(itemtier_layout);

  QVBoxLayout* occasion_layout = new QVBoxLayout();
  occasion_box = new QGroupBox("Occasion",this);
  occasion_always = new QRadioButton("Always",this);
  occasion_layout->addWidget(occasion_always);
  occasion_battle = new QRadioButton("Battle",this);
  occasion_layout->addWidget(occasion_battle);
  occasion_menu = new QRadioButton("Menu",this);
  occasion_layout->addWidget(occasion_menu);
  occasion_none = new QRadioButton("None",this);
  occasion_layout->addWidget(occasion_none);
  occasion_box->setLayout(occasion_layout);

  smallgroupbox_layout->addWidget(itemtier_box);
  smallgroupbox_layout->addWidget(occasion_box);
  groupbottom_layout->addLayout(smallgroupbox_layout);
  total_layout->addLayout(grouptop_layout);
  total_layout->addLayout(groupbottom_layout);

}

/* Constructor function with id and name */
EditorItem::EditorItem(int id, QString name, QWidget* parent)
          : EditorItem(parent)
{
  setID(id);
  setName(name);
}

/* Copy constructor */
EditorItem::EditorItem(const EditorItem &source) : EditorItem()
{
  copySelf(source);
}

EditorItem::~EditorItem()
{
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/* Copy function, to be called by a copy or equal operator constructor */
void EditorItem::copySelf(const EditorItem &source)
{
  item = source.item;
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

EditorItem* EditorItem::clone()
{
  return this;
}

/* Returns the ID of the item */
int EditorItem::getID() const
{
  return item.getGameID();
}

/* Returns the name of the item */
QString EditorItem::getName() const
{
  return QString::fromStdString(item.getName());
}

/* Returns the name of the item for listing */
QString EditorItem::getNameList()
{
  return EditorHelpers::getListString(getID(), getName());
}

/* Sets the ID of the item */
void EditorItem::setID(int id)
{
  item.setGameID(id);
}

/* Sets the name of the item */
void EditorItem::setName(QString name)
{
  item.setName(name.toStdString());
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/* The copy operator */
EditorItem& EditorItem::operator= (const EditorItem &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
