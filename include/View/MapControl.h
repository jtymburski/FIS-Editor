/*******************************************************************************
 * Class Name: MapControl
 * Date Created: December 23, 2014
 * Inheritance: QWidget
 * Description: Database on the right hand side that contains all of the map
 * layers and controls for those layers.
 ******************************************************************************/
#ifndef MAPCONTROL_H
#define MAPCONTROL_H

#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>

class MapControl : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  MapControl(QWidget* parent = NULL);

  /* Destructor function */
  ~MapControl();

  QListWidget* getTopList();

private:
  /* Lists */
  QListWidget* top_list;
  QListWidget* middle_list;
  QListWidget* bottom_list;

  /* Top Buttons */
  QPushButton* grid_toggle;
  QPushButton* passibility_toggle;

  /* Bottom Buttons */
  QPushButton* new_submap;
  QPushButton* remove_submap;
  QPushButton* duplicate_submap;
  QPushButton* import_submap;

  /* Map Mode Shift Actions (Active Layers) */
  QListWidgetItem *active_lower_layer_01;
  QListWidgetItem *active_lower_layer_02;
  QListWidgetItem *active_lower_layer_03;
  QListWidgetItem *active_lower_layer_04;
  QListWidgetItem *active_lower_layer_05;
  QListWidgetItem *active_enhancer_layer;
  QListWidgetItem *active_item_layer;
  QListWidgetItem *active_base_layer;
  QListWidgetItem *active_person_layer;
  QListWidgetItem *active_thing_layer;
  QListWidgetItem *active_upper_layer_01;
  QListWidgetItem *active_upper_layer_02;
  QListWidgetItem *active_upper_layer_03;
  QListWidgetItem *active_upper_layer_04;
  QListWidgetItem *active_upper_layer_05;


protected:

public slots:


signals:

};

#endif // MAPCONTROL_H
