/*******************************************************************************
 * Class Name: MapControl
 * Date Created: December 23, 2014
 * Inheritance: QWidget
 * Description: Database on the right hand side that contains all of the map
 * layers and controls for those layers.
 ******************************************************************************/
#include "View/MapControl.h"
MapControl::MapControl(QWidget *parent): QListWidget(parent)
{
  /* Sets up the active layer actions, makes them checkable and adds them to
     an action group which allows only one to be active at a time */
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
  addItem(active_base_layer);
  setCurrentRow(0);
  addItem(active_enhancer_layer);
  addItem(active_lower_layer_01);
  addItem(active_lower_layer_02);
  addItem(active_lower_layer_03);
  addItem(active_lower_layer_04);
  addItem(active_lower_layer_05);
  //sidetoolbar->addItem(active_item_layer);
  //sidetoolbar->addItem(active_thing_layer);
  //sidetoolbar->addItem(active_person_layer);
  addItem(active_upper_layer_01);
  addItem(active_upper_layer_02);
  addItem(active_upper_layer_03);
  addItem(active_upper_layer_04);
  addItem(active_upper_layer_05);
}

MapControl::~MapControl()
{

}
