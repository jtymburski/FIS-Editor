/*******************************************************************************
 * Class Name: MapItemView
 * Date Created: June 28, 2015
 * Inheritance: QWidget
 * Description: The side toolbar in the map database that gives access to the
 *              map items.
 ******************************************************************************/
#include "View/MapItemView.h"
#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function. Creates the item view, which is the
 *              side bar within the left map database. Controls items (bases
 *              and instances).
 *
 * Inputs: QWidget* parent - the parent widget
 */
MapItemView::MapItemView(QWidget* parent) : QWidget(parent)
{
  /* Initialize variables */
  editor_map = NULL;
  item_dialog = NULL;

  /* Create the layout */
  createLayout();
}

/*
 * Description: Destructor function
 */
MapItemView::~MapItemView()
{

}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: Adds item to the base list. Triggered by the "new item"
 *              press. The ID is the next available.
 *
 * Inputs: EditorMapItem* item - the new item to add (ID not set yet)
 * Output: none
 */
void MapItemView::addItem(EditorMapItem* item)
{
  if(editor_map != NULL)
  {
    /* Sets the id and puts the item in the library */
    item->setID(editor_map->getNextItemID());
    item->setTileIcons(editor_map->getTileIcons());
    int index = editor_map->setItem(item);

    /* Increments the id tracker and updates view */
    updateList();
    item_list->setCurrentRow(index);
  }
}

/*
 * Description: Creates the dialog layout with QT functional widgets.
 *
 * Inputs: none
 * Output: none
 */
void MapItemView::createLayout()
{
  /* Layout */
  QVBoxLayout* layout = new QVBoxLayout(this);

  /* The list widget */
  item_list = new QListWidget(this);
  connect(item_list, SIGNAL(currentRowChanged(int)),
          this, SLOT(currentRowChanged(int)));
  connect(item_list, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
          this, SLOT(itemDoubleClicked(QListWidgetItem*)));
  layout->addWidget(item_list, 1);

  /* The instances widget */
  QLabel* lbl_instances = new QLabel("Instances:", this);
  layout->addWidget(lbl_instances, 0);
  item_instances = new QListWidget(this);
  item_instances->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(item_instances, SIGNAL(customContextMenuRequested(QPoint)),
          this, SLOT(instanceMenu(QPoint)));
  connect(item_instances, SIGNAL(currentRowChanged(int)),
          this, SLOT(instanceRowChanged(int)));
  connect(item_instances, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
          this, SLOT(instanceDoubleClicked(QListWidgetItem*)));
  layout->addWidget(item_instances, 1);

  /* Right click menu control */
  rightclick_menu = new QMenu("Item Edit", this);
  QAction* edit_item = new QAction("Edit", rightclick_menu);
  connect(edit_item, SIGNAL(triggered()), this, SLOT(editInstance()));
  QAction* delete_item = new QAction("Delete", rightclick_menu);
  connect(delete_item, SIGNAL(triggered()), this, SLOT(deleteInstance()));
  rightclick_menu->addAction(edit_item);
  rightclick_menu->addAction(delete_item);
  rightclick_menu->hide();

  /* Image label */
  lbl_image = new QLabel(this);
  lbl_image->setMinimumSize(200, 200);
  lbl_image->setAlignment(Qt::AlignCenter);
  lbl_image->setStyleSheet("border: 1px solid black");
  layout->addWidget(lbl_image, 0, Qt::AlignHCenter);

  /* Name label */
  lbl_name = new QLabel("Name:", this);
  layout->addWidget(lbl_name, 0, Qt::AlignHCenter);

  /* ID label */
  lbl_id = new QLabel("ID:", this);
  layout->addWidget(lbl_id, 0, Qt::AlignHCenter);

  /* Walkover label */
  lbl_walkover = new QLabel("Walkover:", this);
  layout->addWidget(lbl_walkover, 0, Qt::AlignHCenter);
}

/*
 * Description: Opens the edit item dialog, on an existing item in the list
 *              (either base or instance).
 *
 * Inputs: EditorMapItem* sub_item - the item to edit
 * Output: none
 */
void MapItemView::editItem(EditorMapItem* sub_item)
{
  EditorMapItem* current = getSelected();
  if(sub_item != NULL)
    current = sub_item;

  /* -- Is an instance -- */
  if(current->getBaseItem() != NULL)
  {
    bool ok;
    int result = QInputDialog::getInt(this, tr("Edit Item Count"),
                                      tr("Item Count:"), current->getCount(),
                                      1, ItemDialog::kMAX_COUNT, 1, &ok);
    if(ok)
      current->setCount(result);
  }
  /* -- Is a base -- */
  else
  {
    /* Delete the old and create the new dialog */
    if(item_dialog != NULL)
    {
      disconnect(item_dialog, SIGNAL(ok()), this, SLOT(updateItems()));
      delete item_dialog;
    }
    item_dialog = new ItemDialog(current, this);
    connect(item_dialog, SIGNAL(ok()), this, SLOT(updateItems()));
    item_dialog->show();

    /* Fills the dialogs with data */
    emit fillWithData(EditorEnumDb::ITEM_VIEW);
  }
}

/*
 * Description: Updates the info in the QT widgets within the view.
 *
 * Inputs: none
 * Output: none
 */
void MapItemView::updateInfo()
{
  if(editor_map != NULL)
  {
    /* Blank the labels */
    lbl_id->setText("ID:");
    lbl_image->setPixmap(QPixmap());
    lbl_name->setText("Name:");
    lbl_walkover->setText("Walkover:");

    EditorMapItem* item = getSelected();
    if(item != NULL)
    {
      /* Set the labels */
      lbl_id->setText("ID: " + QString::number(item->getID()));
      lbl_name->setText("Name: " + item->getName());

      /* If matrix is valid, set the image */
      if(item->getMatrix() != NULL)
      {
        lbl_image->setPixmap(item->getMatrix()->getSnapshot(200, 200));
      }

      /* Walkover info */
      if(item->isWalkover())
        lbl_walkover->setText("Walkover: TRUE");
      else
        lbl_walkover->setText("Walkover: FALSE");
    }
  }
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

/*
 * Description: Slot which triggers when the top list changes. Updates the
 *              selected item.
 *
 * Inputs: int index - the new index in the list
 * Output: none
 */
void MapItemView::currentRowChanged(int index)
{
  if(editor_map != NULL)
  {
    editor_map->setCurrentItem(index);
    editor_map->setHoverItem(-1);
  }

  updateInfo();
  item_instances->clearSelection();
}

/*
 * Description: Slot which triggers when an instance is deleted, from a right
 *              click select on the instance.
 *
 * Inputs: none
 * Output: none
 */
void MapItemView::deleteInstance()
{
  if(item_instances->currentItem() != NULL)
  {
    QString text = item_instances->currentItem()->text();

    /* Create warning about deleting */
    QMessageBox msg_box;
    msg_box.setText("Deleting item instance: " + text);
    msg_box.setInformativeText("Are you sure?");
    msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    if(msg_box.exec() == QMessageBox::Yes)
    {
      int index = MapThingView::getInstanceID(text);
      editor_map->unsetItem(index, true);
    }
  }
}

/*
 * Description: Triggers by instance dialog to edit base of the instant.
 *
 * Inputs: EditorMapItem* base - the base item to edit
 * Output: none
 */
void MapItemView::editBaseItem(EditorMapThing* base)
{
  if(base != NULL)
    editItem((EditorMapItem*)base);
}

/*
 * Description: Triggers to edit the hover tile item instance at the index.
 *
 * Inputs: int index - the depth index on the tile
 * Output: none
 */
void MapItemView::editHoverInstance(int index)
{
  if(editor_map != NULL)
  {
    /* Check if hover tile is valid */
    EditorTile* t = editor_map->getHoverInfo()->hover_tile;
    if(t != NULL)
    {
      /* Check if item at index is valid */
      EditorMapItem* item = t->getItem(index);
      if(item != NULL)
        editItem(item);
    }
  }
}

/*
 * Description: Slot which triggers when an instance is edited, from a right
 *              click select on the instance. Opens up edit dialog.
 *
 * Inputs: none
 * Output: none
 */
void MapItemView::editInstance()
{
  if(item_instances->currentItem() != NULL)
  {
    int id = MapThingView::getInstanceID(item_instances->currentItem()->text());

    EditorMapItem* item = editor_map->getItem(id, 
                                              editor_map->getCurrentMap()->id);
    if(item != NULL)
      editItem(item);
  }
}

/*
 * Description: Slot which triggers a right click menu when an instance is
 *              right clicked within the list. Offers delete and edit actions.
 *
 * Inputs: const QPoint & pos - the position right clicked in widget
 * Output: none
 */
void MapItemView::instanceMenu(const QPoint & pos)
{
  QListWidgetItem* item = item_instances->itemAt(pos);
  if(item != NULL)
    rightclick_menu->exec(QCursor::pos());
}

/*
 * Description: Instance list item double clicked. Just triggers the edit
 *              instance action.
 *
 * Inputs: QListWidgetItem* - not used
 * Output: none
 */
void MapItemView::instanceDoubleClicked(QListWidgetItem*)
{
  editInstance();
}

/*
 * Description: Slot which triggers when a row in the instance list of items
 *              changes. This changes which item is hovered on the map.
 *
 * Inputs: int index - the new index of the row in the list
 * Output: none
 */
void MapItemView::instanceRowChanged(int index)
{
  if(index >= 0 && editor_map != NULL)
  {
    /* Set the hover item in the class */
    int id = MapThingView::getInstanceID(item_instances->currentItem()->text());
    if(editor_map->setHoverItem(id))
    {
      EditorMapItem* item = editor_map->getItem(id, 
                                               editor_map->getCurrentMap()->id);
      if(item != NULL)
      {
        emit ensureVisible(editor_map->getCurrentMap()->tiles[item->getX()]
                                                             [item->getY()]);

        /* Select the base in the list */
        if(item->getBaseItem() != NULL)
        {
          int index = editor_map->getItemIndex(item->getBaseItem()->getID());
          item_list->blockSignals(true);
          item_list->setCurrentRow(index);
          item_list->blockSignals(false);
          updateInfo();
        }
      }
    }
  }
}

/*
 * Description: Slot which triggers when an item in the base list is double
 *              clicked. Opens a pop-up for the item to be edited.
 *
 * Inputs: QListWidgetItem* - not used
 * Output: none
 */
void MapItemView::itemDoubleClicked(QListWidgetItem*)
{
  editItem();
}

/*
 * Description: Slot which triggers to update the instances in the view and
 *              put them in the list. Triggers on change events, such as editing
 *              and deleting instances, and on initial setup
 *
 * Inputs: QString name_list - the list of the added item. Blank if N/A
 * Output: none
 */
void MapItemView::itemInstanceUpdate(QString name_list)
{
  item_instances->blockSignals(true);
  item_instances->clearSelection();
  item_instances->clearFocus();
  item_instances->clear();

  if(editor_map != NULL)
  {
    /* Set up the instances list */
    int sub_index = editor_map->getCurrentMapIndex();
    if(sub_index >= 0)
    {
      for(int i = 0; i < editor_map->getItemCount(sub_index); i++)
      {
        item_instances->addItem(
                       editor_map->getItemByIndex(i, sub_index)->getNameList());
      }
      item_instances->sortItems();
    }
  }

  item_instances->clearSelection();
  item_instances->clearFocus();
  item_instances->blockSignals(false);

  /* If name list is not blank, select the name in the list */
  if(!name_list.isEmpty())
  {
    for(int i = 0; i < item_instances->count(); i++)
      if(item_instances->item(i)->text() == name_list)
        item_instances->setCurrentRow(i);
  }
}

/*
 * Description: Refreshes the Editor Item list.
 *
 * Inputs: none
 * Output: none
 */
void MapItemView::updateList()
{
  int index = item_list->currentRow();

  /* Set up the base list */
  item_list->clear();
  if(editor_map != NULL)
  {
    /* Set up the base list */
    for(int i = 0; i < editor_map->getItemCount(); i++)
      item_list->addItem(editor_map->getItemByIndex(i)->getNameList());
    editor_map->updateAll();

    /* Set up the instances list */
    itemInstanceUpdate("");
  }

  item_list->setCurrentRow(index);
  updateInfo();
  update();
}

/*
 * Description: Slot which triggers to update a item upon completion by the
 *              item dialog (on ok()). This will also update all instances of
 *              the object, deleting as necessary.
 *
 * Inputs: none
 * Output: none
 */
void MapItemView::updateItems()
{
  /* Update the item in the pop-up */
  if(item_dialog != NULL)
  {
    editor_map->tilesItemRemove();
    item_dialog->updateOriginal();
    editor_map->tilesItemAdd();
  }

  /* Update the list in the view */
  updateList();
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Deletes the selected item in the base list. Triggered by
 *              parent.
 *
 * Inputs: none
 * Output: bool - returns true if item deleted.
 */
bool MapItemView::deleteItem()
{
  EditorMapItem* selected = getSelected();
  bool success = false;

  /* If valid, proceed deletion */
  if(selected != NULL)
  {
    /* Create warning about deleting */
    QMessageBox msg_box;
    msg_box.setText("Deleting base item " + selected->getNameList());
    msg_box.setInformativeText(
                  "This will delete ALL instances of this base. Are you sure?");
    msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    if(msg_box.exec() == QMessageBox::Yes)
    {
      /* Proceed with deletion */
      int index = item_list->currentRow();
      success = editor_map->unsetItemByIndex(index);

      /* If successful, reset list */
      if(success)
      {
        updateList();
        if(index >= item_list->count())
          item_list->setCurrentRow(item_list->count() - 1);
      }
    }
  }

  return success;
}

/*
 * Description: Duplicates the selected item in the base list. Triggered by
 *              parent.
 *
 * Inputs: none
 * Output: bool - returns true if item was duplicated.
 */
bool MapItemView::duplicateItem()
{
  EditorMapItem* selected = getSelected();

  /* If valid, proceed duplication */
  if(selected != NULL)
  {
    EditorMapItem* new_item = new EditorMapItem(*selected);
    addItem(new_item);
    return true;
  }

  return false;
}

/*
 * Description: Returns which item is selected in the base list.
 *
 * Inputs: none
 * Output: EditorMapItem* - selected item. NULL if none selected
 */
EditorMapItem* MapItemView::getSelected()
{
  int index = item_list->currentRow();
  EditorMapItem* item = NULL;

  /* Check the validity */
  if(editor_map != NULL)
    item = editor_map->getItemByIndex(index);

  return item;
}

/*
 * Description: Returns the editor map, which contains all items and instances
 *
 * Inputs: none
 * Output: EditorMap* - pointer to the editor map. NULL if none set.
 */
EditorMap* MapItemView::getEditorMap()
{
  return editor_map;
}

/*
 * Description: Imports a new item into the base list. Triggered by parent.
 *
 * Inputs: none
 * Output: none
 */
void MapItemView::importItem()
{
  // TODO: Future
  QMessageBox::information(this, "Notification",
                           "Coming soon to a production near you!");
}

/*
 * Description: Creates a new item into the base list. Triggered by
 *              parent. Proceeds to call edit on the new item and opens
 *              ItemDialog.
 *
 * Inputs: none
 * Output: none
 */
void MapItemView::newItem()
{
  EditorMapItem* new_item = new EditorMapItem();
  addItem(new_item);
  editItem();
}

/*
 * Description: Sets the control editor map. This contains the items, sprites
 *              and all data relevant to display.
 *
 * Inputs: EditorMap* map - the map to use for control
 * Output: none
 */
void MapItemView::setEditorMap(EditorMap* map)
{
  /* If existing editor map is not NULL, undo */
  if(editor_map != NULL)
  {
    disconnect(editor_map, SIGNAL(itemInstanceChanged(QString)),
               this, SLOT(itemInstanceUpdate(QString)));
  }

  editor_map = map;

  /* If new map is not NULL, reconnect */
  if(editor_map != NULL)
  {
    connect(editor_map, SIGNAL(itemInstanceChanged(QString)),
            this, SLOT(itemInstanceUpdate(QString)));
  }

  /* Finally, update list */
  updateList();
}

/*
 * Description: Sets the list of items, used for event creation
 *
 * Inputs: QVector<QString> - list of all items (for give item event)
 * Output: none
 */
void MapItemView::updateListItems(QVector<QString> list)
{
  if(item_dialog != NULL)
    item_dialog->setListItems(list);
}
