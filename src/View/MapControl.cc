/*******************************************************************************
 * Class Name: MapControl
 * Date Created: December 23, 2014
 * Inheritance: QWidget
 * Description: Database on the right hand side that contains all of the map
 * layers and controls for those layers.
 ******************************************************************************/
#include "View/MapControl.h"
#include <QDebug>

#include "Database/EditorEvent.h"
#include "View/EventView.h"

MapControl::MapControl(QWidget *parent): QWidget(parent)
{
  editing_map = NULL;
  mapsize_dialog = NULL;

  QVBoxLayout* main_layout = new QVBoxLayout(this);
  QHBoxLayout* button_layout = new QHBoxLayout();
  QHBoxLayout* button_layout2 = new QHBoxLayout();

  /* Sets up top toggle buttons */
  grid_toggle = new QPushButton("Grid", this);
  grid_toggle->setCheckable(true);
  grid_toggle->setChecked(true);
  connect(grid_toggle, SIGNAL(toggled(bool)),
          this, SLOT(toggleGrid(bool)));
  passability_toggle = new QPushButton("Passibility", this);
  passability_toggle->setCheckable(true);
  connect(passability_toggle, SIGNAL(toggled(bool)),
          this, SLOT(togglePassability(bool)));
  button_layout->addWidget(grid_toggle);
  button_layout->addWidget(passability_toggle);
  main_layout->addLayout(button_layout);

  /* Sets up second row of toggle buttons */
  events_toggle = new QPushButton("Events", this);
  events_toggle->setCheckable(true);
  connect(events_toggle, SIGNAL(toggled(bool)),
          this, SLOT(toggleEvents(bool)));
  button_layout2->addWidget(events_toggle);
  main_layout->addLayout(button_layout2);

  /* Sets up the active layer actions, makes them checkable and adds them to
     an action group which allows only one to be active at a time */ 
  list_top = new QListWidget(this);
  for(int i = 0; i < (int)EditorEnumDb::NO_LAYER; i++)
  {
    QListWidgetItem* item = new QListWidgetItem(
               EditorHelpers::getLayerString((EditorEnumDb::Layer)i), list_top);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    item->setCheckState(Qt::Checked);
  }
  list_top->setCurrentRow(0);
  main_layout->addWidget(list_top);
  connect(list_top, SIGNAL(itemChanged(QListWidgetItem*)),
          this, SLOT(itemCheckChanged(QListWidgetItem*)));
  connect(list_top, SIGNAL(currentRowChanged(int)),
          this, SLOT(layerChanged(int)));

  /* Sets up the second list widget with the leveles of visibility */
  list_middle = new QListWidget(this);
  main_layout->addWidget(list_middle);

  /* Sets up the bottom list of sub maps */
  list_bottom = new QListWidget(this);
  list_bottom->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(list_bottom, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
          this, SLOT(selectSubMap(QListWidgetItem*)));
  connect(list_bottom, SIGNAL(currentRowChanged(int)),
          this, SLOT(updateBottomRow(int)));
  connect(list_bottom, SIGNAL(customContextMenuRequested(QPoint)),
          this, SLOT(listMenuRequested(QPoint)));
  main_layout->addWidget(list_bottom);

  /* Right click menu control */
  rightclick_menu = new QMenu("Map Edit", this);
  rename_map = new QAction("Rename", rightclick_menu);
  connect(rename_map, SIGNAL(triggered()), this, SLOT(renameSubMap()));
  QAction* resize_map = new QAction("Resize", rightclick_menu);
  connect(resize_map, SIGNAL(triggered()), this, SLOT(resizeTrigger()));
  rightclick_menu->addAction(rename_map);
  rightclick_menu->addAction(resize_map);
  rightclick_menu->hide();
  resize_index = 0;

  /* Sets up the bottom buttons that interact with the sub-map list */
  QHBoxLayout* bottom_button_layout = new QHBoxLayout();
  button_new = new QPushButton("New",this);
  button_delete = new QPushButton("Delete",this);
  bottom_button_layout->addWidget(button_new);
  bottom_button_layout->addWidget(button_delete);
  connect(button_new, SIGNAL(clicked()), this, SLOT(newSubMap()));
  connect(button_delete, SIGNAL(clicked()), this, SLOT(deleteSubMap()));
  main_layout->addLayout(bottom_button_layout);

  /* Sets up the second set of bottom buttons */
  QHBoxLayout* bottom_button_layout2 = new QHBoxLayout();
  button_duplicate = new QPushButton("Duplicate",this);
  button_import = new QPushButton("Import",this);
  bottom_button_layout2->addWidget(button_import);
  bottom_button_layout2->addWidget(button_duplicate);
  connect(button_duplicate, SIGNAL(clicked()), this, SLOT(duplicateSubMap()));
  connect(button_import, SIGNAL(clicked()), this, SLOT(importSubMap()));
  main_layout->addLayout(bottom_button_layout2);

  setMaximumWidth(190);
}

MapControl::~MapControl()
{

}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/* Select the sub-map, by index */
void MapControl::selectSubMap(int index)
{
  if(editing_map != NULL)
  {
    SubMapInfo* selected = editing_map->getMapByIndex(index);
    if(selected != NULL)
    {
      editing_map->setCurrentMap(index);
      emit updateMap();
      updateMapList();
    }
  }
}

/* Updates list at the bottom right for maps selected */
void MapControl::updateMapList(int selected)
{
  int bold_index = -1;
  QStringList list_set;
  int last_row = list_bottom->currentRow();

  /* Clear the bottom list */
  list_bottom->clear();

  /* Fill the data */
  if(editing_map != NULL)
  {
    /* Set the bold index */
    bold_index = editing_map->getCurrentMapIndex();

    for(int i = 0; i < editing_map->getMapCount(); i++)
      list_set << editing_map->getMapNameList(i);

    /* Set up the bottom view */
    QFont font;
    list_bottom->addItems(list_set);
    font.setBold(false);
    for(int i = 0; i < list_bottom->count(); i++)
      list_bottom->item(i)->setFont(font);
    if(bold_index != -1)
    {
      font.setBold(true);
      list_bottom->item(bold_index)->setFont(font);
    }

    /* If there is a selected map, re-select it */
    if(selected >= 0 && selected < list_bottom->count())
      list_bottom->setCurrentRow(selected);
    else if(last_row >= 0 && last_row < list_bottom->count())
      list_bottom->setCurrentRow(last_row);
  }
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

/* Change layer slot - from map DB */
// TODO: Comment
void MapControl::changeLayer(EditorEnumDb::Layer layer)
{
  list_top->setCurrentRow((int)layer);
}

/* Creates the new Sub-Map, as instantiated from dialog */
void MapControl::createNewSubMap()
{
  QString name = EditorMap::getDialogName(mapsize_dialog);
  int width = EditorMap::getDialogWidth(mapsize_dialog);
  int height = EditorMap::getDialogHeight(mapsize_dialog);

  /* Add the new map */
  int index = -1;
  if(!name.isEmpty() && width > 0 && height > 0)
    index = editing_map->setMap(editing_map->getNextMapID(), name, width, height);

  /* Finally, close the dialog */
  mapsize_dialog->close();
  delete mapsize_dialog;
  mapsize_dialog = NULL;

  /* Update bottom view */
  if(index >= 0)
  {
    updateMapList();
    list_bottom->setCurrentRow(index);
  }
}

/* Delete Sub-Map */
void MapControl::deleteSubMap()
{
  if(list_bottom->currentRow() >= 0)
  {
    int index = list_bottom->currentRow();

    /* Check if it's the bottom row */
    bool bottom = false;
    if((index + 1) == list_bottom->count())
      bottom = true;

    /* Create warning about deleting */
    QMessageBox msg_box;
    msg_box.setText("Deleting sub map \"" +
                    list_bottom->currentItem()->text() + "\".");
    msg_box.setInformativeText("Are you sure?");
    msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    if(msg_box.exec() == QMessageBox::Yes)
    {
      /* Delete the sub-map */
      if(index == editing_map->getCurrentMapIndex())
      {
        editing_map->setCurrentMap(-1);
        emit updateMap();
      }
      editing_map->unsetMapByIndex(index);

      /* Update list */
      if(bottom)
        updateMapList(editing_map->getMapCount() - 1);
      else
        updateMapList(index);
    }
  }
}

/* Duplicate Sub-Map */
void MapControl::duplicateSubMap()
{
  if(list_bottom->currentRow() >= 0)
  {
    int id = editing_map->getNextMapID();
    int index = list_bottom->currentRow();
    SubMapInfo* copy_map = editing_map->getMapByIndex(index);

    /* Create new map */
    int map_index = editing_map->setMap(id, "TEMP", 1, 1);
    SubMapInfo* new_map = editing_map->getMapByIndex(map_index);
    editing_map->copySubMap(copy_map, new_map);
    if(new_map->name == "MAIN")
      new_map->name += " duped";

    /* Update the list */
    updateMapList(map_index);
  }
}

/* Import Sub-Map */
void MapControl::importSubMap()
{
  // TODO: Future
  QMessageBox::information(this, "Notification",
                           "Coming soon to a production near you!");
}

/* Item check change */
void MapControl::itemCheckChanged(QListWidgetItem* item)
{
  EditorEnumDb::Layer layer = (EditorEnumDb::Layer)list_top->row(item);
  editing_map->setVisibility(layer, item->checkState() == Qt::Checked);
}

/* Selected layer changed */
void MapControl::layerChanged(int current_row)
{
  EditorEnumDb::Layer layer = (EditorEnumDb::Layer)current_row;
  emit layerChanged(layer);

  if(editing_map != NULL)
    editing_map->setHoverLayer(layer);
}

/* Right click list menu on bottom list */
void MapControl::listMenuRequested(const QPoint & pos)
{
  (void)pos;
  /* Only proceed if it's the map set */
  if(editing_map != NULL)
  {
    if(list_bottom->currentRow() == 0)
      rename_map->setDisabled(true);
    else
      rename_map->setEnabled(true);
    rightclick_menu->exec(QCursor::pos());
  }
}

/* New Sub-Map */
void MapControl::newSubMap()
{
  if(mapsize_dialog != NULL)
    delete mapsize_dialog;
  mapsize_dialog = EditorMap::createMapDialog(this);
  connect(EditorMap::getDialogButton(mapsize_dialog), SIGNAL(clicked()),
          this, SLOT(createNewSubMap()));
  mapsize_dialog->show();
}

/* Renames the selected map */
void MapControl::renameSubMap()
{
  rightclick_menu->hide();

  QListWidgetItem* item = list_bottom->currentItem();
  if(item != NULL)
  {
    int row_index = list_bottom->currentRow();
    bool ok;
    QString text = QInputDialog::getText(this, "Rename Map", "Map Name:",
           QLineEdit::Normal, editing_map->getMapByIndex(row_index)->name, &ok);
    if(ok && !text.isEmpty())
    {
      editing_map->getMapByIndex(row_index)->name = text;
      updateMapList();
    }
  }
}

/* Resizes the selected map */
void MapControl::resizeSubMap()
{
  QString name = EditorMap::getDialogName(mapsize_dialog);
  int width = EditorMap::getDialogWidth(mapsize_dialog);
  int height = EditorMap::getDialogHeight(mapsize_dialog);

  /* Modify the sub-map information, if changed */
  editing_map->resizeMap(resize_index, width, height);
  SubMapInfo* info = editing_map->getMapByIndex(resize_index);
  if(info != NULL)
      info->name = name;

  /* Finally, close the dialog */
  mapsize_dialog->close();
  delete mapsize_dialog;
  mapsize_dialog = NULL;

  /* Update list, and view if applicable */
  updateMapList();
  emit updateAllLists();
  if(info == editing_map->getCurrentMap())
    emit updateMap();
}

/* Resize trigger from the menu */
void MapControl::resizeTrigger()
{
  rightclick_menu->hide();

  QListWidgetItem* item = list_bottom->currentItem();
  if(item != NULL)
  {
    resize_index = list_bottom->currentRow();

    /* Open map size dialog */
    if(mapsize_dialog != NULL)
      delete mapsize_dialog;
    SubMapInfo* info = editing_map->getMapByIndex(resize_index);
    mapsize_dialog = EditorMap::createMapDialog(this, "Revise Map",
                                                info->name, info->tiles.size(),
                                                info->tiles.front().size());
    connect(EditorMap::getDialogButton(mapsize_dialog), SIGNAL(clicked()),
            this, SLOT(resizeSubMap()));
    mapsize_dialog->show();
  }
}

/* Select the sub-map, by double click */
void MapControl::selectSubMap(QListWidgetItem* item)
{
  int row = list_bottom->row(item);
  selectSubMap(row);
}

/* Toggles the event notifier visibility */
void MapControl::toggleEvents(bool visible)
{
  if(editing_map != NULL)
    editing_map->setVisibilityEvents(visible);
}

/* Toggles the grid */
void MapControl::toggleGrid(bool visible)
{
  if(editing_map != NULL)
    editing_map->setVisibilityGrid(visible);
}

/* Toggles the passability */
void MapControl::togglePassability(bool visible)
{
  if(editing_map != NULL)
    editing_map->setVisibilityPass(visible);
}

/* Update map row on bottom list */
void MapControl::updateBottomRow(int current_row)
{
  if(current_row == 0)
    button_delete->setDisabled(true);
  else
    button_delete->setEnabled(true);
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Disables control on changing sub for when in editing view with map */
void MapControl::disableControl(bool disabled)
{
  list_bottom->setDisabled(disabled);
  button_delete->setDisabled(disabled);
  button_duplicate->setDisabled(disabled);
  button_import->setDisabled(disabled);
  button_new->setDisabled(disabled);

  /* Fix bottom row buttons */
  if(!disabled)
    updateBottomRow(list_bottom->currentRow());
}

/* Get the current sub-map */
SubMapInfo* MapControl::getCurrentMap()
{
  return editing_map->getCurrentMap();
}

/* Returns the current map index */
int MapControl::getCurrentMapIndex()
{
  return list_bottom->currentRow();
}

/* Returns event notifier toggle status */
bool MapControl::getEventsToggle()
{
  return events_toggle->isChecked();
}

/* Gets the status of the grid toggle */
bool MapControl::getGridToggle()
{
  return grid_toggle->isChecked();
}

/* Returns the map being edited */
EditorMap* MapControl::getMapEditor()
{
  return editing_map;
}

/* Gets the status of the passability toggle */
bool MapControl::getPassabilityToggle()
{
  return passability_toggle->isChecked();
}

/* Gets the top list widget */
QListWidget* MapControl::getTopList()
{
  return list_top;
}

/* Returns the selected layer in the top list */
EditorEnumDb::Layer MapControl::getSelectedLayer()
{
  return (EditorEnumDb::Layer)list_top->currentRow();
}

/* Returns the visibility setting of the checked boxes in the top list */
bool MapControl::getVisibility(EditorEnumDb::Layer layer)
{
  if(layer != EditorEnumDb::NO_LAYER)
    return (list_top->item((int)layer)->checkState() == Qt::Checked);
  return false;
}

/* Sets the map being edited */
void MapControl::setMapEditor(EditorMap* editor)
{
  editing_map = NULL;
  list_bottom->setCurrentRow(-1);

  // TODO: Clean-up processing...if necessary

  editing_map = editor;
  if(editing_map != NULL)
  {
    /* Update visibility of grid and passability */
    editing_map->setVisibilityEvents(getEventsToggle());
    editing_map->setVisibilityGrid(getGridToggle());
    editing_map->setVisibilityPass(getPassabilityToggle());

    /* Update layer */
    editing_map->setHoverLayer(getSelectedLayer());

    /* Update visibility of all layers */
    for(int i = 0; i < (int)EditorEnumDb::NO_LAYER; i++)
      editing_map->setVisibility((EditorEnumDb::Layer)i,
                                 getVisibility((EditorEnumDb::Layer)i));

    /* Finally, select main sub-map - always exists */
    selectSubMap(0);
  }

  /* Update map list */
  updateMapList();
  list_bottom->setCurrentRow(0);
}
