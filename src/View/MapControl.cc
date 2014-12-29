/*******************************************************************************
 * Class Name: MapControl
 * Date Created: December 23, 2014
 * Inheritance: QWidget
 * Description: Database on the right hand side that contains all of the map
 * layers and controls for those layers.
 ******************************************************************************/
#include "View/MapControl.h"
MapControl::MapControl(QWidget *parent): QWidget(parent)
{
  QVBoxLayout* main_layout = new QVBoxLayout(this);
  QHBoxLayout* button_layout = new QHBoxLayout();
  /* Sets up top toggle buttons */
  grid_toggle = new QPushButton("Grid",this);
  passibility_toggle = new QPushButton("Passibility",this);
  grid_toggle->setCheckable(true);
  grid_toggle->setChecked(true);
  passibility_toggle->setCheckable(true);
  button_layout->addWidget(grid_toggle);
  button_layout->addWidget(passibility_toggle);
  main_layout->addLayout(button_layout);

  /* Sets up the active layer actions, makes them checkable and adds them to
     an action group which allows only one to be active at a time */ 
  top_list = new QListWidget(this);
  active_base_layer = new QListWidgetItem("Base");
  active_enhancer_layer = new QListWidgetItem("Enhancer");
  active_lower_layer_01 = new QListWidgetItem("Lower 1");
  active_lower_layer_02 = new QListWidgetItem("Lower 2");
  active_lower_layer_03 = new QListWidgetItem("Lower 3");
  active_lower_layer_04 = new QListWidgetItem("Lower 4");
  active_lower_layer_05 = new QListWidgetItem("Lower 5");
  active_item_layer = new QListWidgetItem("Item");
  active_thing_layer = new QListWidgetItem("Thing");
  active_person_layer = new QListWidgetItem("Person");
  active_upper_layer_01 = new QListWidgetItem("Upper 1");
  active_upper_layer_02 = new QListWidgetItem("Upper 2");
  active_upper_layer_03 = new QListWidgetItem("Upper 3");
  active_upper_layer_04 = new QListWidgetItem("Upper 4");
  active_upper_layer_05 = new QListWidgetItem("Upper 5");

  /* Sets up the side toolbar which shows the current active layer */
  top_list->addItem(active_base_layer);
  top_list->setCurrentRow(0);
  top_list->addItem(active_enhancer_layer);
  top_list->addItem(active_lower_layer_01);
  top_list->addItem(active_lower_layer_02);
  top_list->addItem(active_lower_layer_03);
  top_list->addItem(active_lower_layer_04);
  top_list->addItem(active_lower_layer_05);
  //sidetoolbar->addItem(active_item_layer);
  //sidetoolbar->addItem(active_thing_layer);
  //sidetoolbar->addItem(active_person_layer);
  top_list->addItem(active_upper_layer_01);
  top_list->addItem(active_upper_layer_02);
  top_list->addItem(active_upper_layer_03);
  top_list->addItem(active_upper_layer_04);
  top_list->addItem(active_upper_layer_05);
  main_layout->addWidget(top_list);

  /* Sets up the second list widget with the leveles of visibility */
  middle_list = new QListWidget(this);
  main_layout->addWidget(middle_list);

  /* Sets up the bottom list of sub maps */
  bottom_list = new QListWidget(this);
  main_layout->addWidget(bottom_list);

  /* Sets up the bottom buttons that interact with the sub-map list */
  new_submap = new QPushButton("New",this);
  remove_submap = new QPushButton("Delete",this);
  duplicate_submap = new QPushButton("Duplicate",this);
  import_submap = new QPushButton("Import",this);
  QHBoxLayout* button_layout2 = new QHBoxLayout();
  button_layout2->addWidget(new_submap);
  button_layout2->addWidget(remove_submap);
  button_layout2->addWidget(duplicate_submap);
  button_layout2->addWidget(import_submap);
  main_layout->addLayout(button_layout2);

}

MapControl::~MapControl()
{

}

QPushButton* MapControl::getGridToggle()
{
  return grid_toggle;
}

QPushButton* MapControl::getPassibilityToggle()
{
  return passibility_toggle;
}

QListWidget* MapControl::getTopList()
{
  return top_list;
}
