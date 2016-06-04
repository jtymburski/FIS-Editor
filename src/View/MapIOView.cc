/*******************************************************************************
 * Class Name: MapIOView
 * Date Created: July 16, 2015
 * Inheritance: QWidget
 * Description: The side toolbar in the map database that gives access to the
 *              map interactive objects.
 ******************************************************************************/
#include "View/MapIOView.h"
//#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function. Creates the IO view, which is the
 *              side bar within the left map database. Controls IOs (bases
 *              and instances).
 *
 * Inputs: QWidget* parent - the parent widget
 */
MapIOView::MapIOView(QWidget* parent) : QWidget(parent)
{
  /* Initialize variables */
  editor_map = NULL;
  instance_dialog = NULL;
  io_dialog = NULL;

  /* Create the layout */
  createLayout();
}

/*
 * Description: Destructor function
 */
MapIOView::~MapIOView()
{

}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: Adds IO to the base list. Triggered by the "new IO"
 *              press. The ID is the next available.
 *
 * Inputs: EditorMapIO* io - the new IO to add (ID not set yet)
 * Output: bool - true if added successfully
 */
bool MapIOView::addIO(EditorMapIO* io)
{
  if(editor_map != nullptr)
  {
    /* Try and get the ID */
    int id = editor_map->getNextIOID();
    if(id >= 0)
    {
      /* Sets the id and puts the IO in the library */
      io->setID(id);
      io->setTileIcons(editor_map->getTileIcons());
      int index = editor_map->setIO(io);

      /* Increments the id tracker and updates view */
      updateList();
      io_list->setCurrentRow(index);

      return true;
    }
    else
    {
      delete io;
      QMessageBox::information(this, "No Space Remaining",
                      "The max limit of IO bases has been reached");
    }
  }
  return false;
}

/*
 * Description: Creates the dialog layout with QT functional widgets.
 *
 * Inputs: none
 * Output: none
 */
void MapIOView::createLayout()
{
  /* Layout */
  QVBoxLayout* layout = new QVBoxLayout(this);

  /* The list widget */
  io_list = new QListWidget(this);
  connect(io_list, SIGNAL(currentRowChanged(int)),
          this, SLOT(currentRowChanged(int)));
  connect(io_list, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
          this, SLOT(itemDoubleClicked(QListWidgetItem*)));
  layout->addWidget(io_list, 1);

  /* The instances widget */
  QLabel* lbl_instances = new QLabel("Instances:", this);
  layout->addWidget(lbl_instances, 0);
  io_instances = new QListWidget(this);
  io_instances->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(io_instances, SIGNAL(customContextMenuRequested(QPoint)),
          this, SLOT(instanceMenu(QPoint)));
  connect(io_instances, SIGNAL(currentRowChanged(int)),
          this, SLOT(instanceRowChanged(int)));
  connect(io_instances, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
          this, SLOT(instanceDoubleClicked(QListWidgetItem*)));
  layout->addWidget(io_instances, 1);

  /* Right click menu control */
  rightclick_menu = new QMenu("IO Edit", this);
  QAction* edit_io = new QAction("Edit", rightclick_menu);
  connect(edit_io, SIGNAL(triggered()), this, SLOT(editInstance()));
  QAction* delete_io = new QAction("Delete", rightclick_menu);
  connect(delete_io, SIGNAL(triggered()), this, SLOT(deleteInstance()));
  rightclick_menu->addAction(edit_io);
  rightclick_menu->addAction(delete_io);
  rightclick_menu->hide();

  /* Image label */
  lbl_image = new QLabel(this);
  lbl_image->setMinimumSize(200, 200);
  lbl_image->setAlignment(Qt::AlignCenter);
  lbl_image->setStyleSheet("border: 1px solid #a8a8a8");
  layout->addWidget(lbl_image, 0, Qt::AlignHCenter);

  /* Name label */
  lbl_name = new QLabel("", this);
  layout->addWidget(lbl_name, 0, Qt::AlignHCenter);

  /* ID label */
  lbl_id = new QLabel("", this);
  layout->addWidget(lbl_id, 0, Qt::AlignHCenter);

  /* Size label */
  lbl_size = new QLabel("", this);
  layout->addWidget(lbl_size, 0, Qt::AlignHCenter);
}

/*
 * Description: Deletes the base dialog, if it exists.
 *
 * Inputs: none
 * Output: none
 */
void MapIOView::deletePopBase()
{
  if(io_dialog != nullptr)
  {
    disconnect(io_dialog, SIGNAL(ok()), this, SLOT(updateIOs()));
    disconnect(io_dialog, SIGNAL(selectTile(EditorEnumDb::MapObjectMode)),
               this, SIGNAL(selectTile(EditorEnumDb::MapObjectMode)));
    delete io_dialog;
    io_dialog = nullptr;
  }
}

/*
 * Description: Deletes the instance dialog, if it exists.
 *
 * Inputs: none
 * Output: none
 */
void MapIOView::deletePopInstance()
{
  if(instance_dialog != nullptr)
  {
    disconnect(instance_dialog, SIGNAL(ok(QString)),
               this, SLOT(ioInstanceUpdate(QString)));
    disconnect(instance_dialog,
               SIGNAL(selectTile(EditorEnumDb::MapObjectMode)),
               this, SIGNAL(selectTile(EditorEnumDb::MapObjectMode)));
    disconnect(instance_dialog, SIGNAL(editBase(EditorMapThing*)),
               this, SLOT(editBaseIO(EditorMapThing*)));
    delete instance_dialog;
    instance_dialog = nullptr;
  }
}

/*
 * Description: Opens the edit IO dialog, on an existing IO in the list
 *              (either base or instance).
 *
 * Inputs: EditorMapIO* sub_io - the IO to edit
 * Output: none
 */
void MapIOView::editIO(EditorMapIO* sub_io)
{
  EditorMapIO* current = getSelected();
  if(sub_io != NULL)
    current = sub_io;

  /* Delete all existing pop-ups first */
  deletePopBase();
  deletePopInstance();

  /* -- Is an instance -- */
  if(current->getBaseIO() != NULL)
  {
    /* Create the new instance dialog */
    instance_dialog = new InstanceDialog(current, this);
    connect(instance_dialog, SIGNAL(ok(QString)),
            this, SLOT(ioInstanceUpdate(QString)));
    connect(instance_dialog, SIGNAL(selectTile(EditorEnumDb::MapObjectMode)),
            this, SIGNAL(selectTile(EditorEnumDb::MapObjectMode)));
    connect(instance_dialog, SIGNAL(editBase(EditorMapThing*)),
            this, SLOT(editBaseIO(EditorMapThing*)));
    instance_dialog->show();
  }
  /* -- Is a base -- */
  else
  {
    /* Create the new base dialog */
    io_dialog = new IODialog(current, this);
    connect(io_dialog, SIGNAL(ok()), this, SLOT(updateIOs()));
    connect(io_dialog, SIGNAL(selectTile(EditorEnumDb::MapObjectMode)),
            this, SIGNAL(selectTile(EditorEnumDb::MapObjectMode)));
    io_dialog->show();
  }

  /* Fills the dialogs with data */
  emit fillWithData(EditorEnumDb::IO_VIEW);
}

/*
 * Description: Updates the info in the QT widgets within the view.
 *
 * Inputs: none
 * Output: none
 */
void MapIOView::updateInfo()
{
  if(editor_map != NULL)
  {
    /* Blank the labels */
    lbl_id->setText("");
    lbl_image->setPixmap(QPixmap());
    lbl_name->setText("");
    lbl_size->setText("");

    EditorMapIO* io = getSelected();
    if(io != NULL)
    {
      /* Set the labels */
      lbl_id->setText("ID: " + QString::number(io->getID()));
      lbl_name->setText("Name: " + io->getName());

      /* If matrix is valid, set the remaining info */
      if(io->getMatrix() != NULL)
      {
        lbl_image->setPixmap(io->getMatrix()->getSnapshot(200, 200));
        lbl_size->setText("Size: " +
                      QString::number(io->getMatrix()->getWidth()) +
                      "W x " +
                      QString::number(io->getMatrix()->getHeight()) +
                      "H  |  Nodes: " +
                      QString::number(io->getStates().size()));
      }
    }
  }
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

/*
 * Description: Slot which triggers when the top list changes. Updates the
 *              selected IO.
 *
 * Inputs: int index - the new index in the list
 * Output: none
 */
void MapIOView::currentRowChanged(int index)
{
  if(editor_map != NULL)
  {
    editor_map->setCurrentIO(index);
    editor_map->setHoverIO(-1);
    if(editor_map->getHoverInfo()->active_layer != EditorEnumDb::IO)
      emit changeLayer(EditorEnumDb::IO);
  }

  updateInfo();
  io_instances->clearSelection();
}

/*
 * Description: Slot which triggers when an instance is deleted, from a right
 *              click select on the instance.
 *
 * Inputs: none
 * Output: none
 */
void MapIOView::deleteInstance()
{
  if(io_instances->currentItem() != NULL)
  {
    QString text = io_instances->currentItem()->text();

    /* Create warning about deleting */
    QMessageBox msg_box;
    msg_box.setText("Deleting IO instance: " + text);
    msg_box.setInformativeText("Are you sure?");
    msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    if(msg_box.exec() == QMessageBox::Yes)
    {
      int index = MapThingView::getInstanceID(text);
      editor_map->unsetIO(index, true);
    }
  }
}

/*
 * Description: Triggers by instance dialog to edit base of the instant.
 *
 * Inputs: EditorMapThing* base - the base IO to edit
 * Output: none
 */
void MapIOView::editBaseIO(EditorMapThing* base)
{
  /* Trigger edit on base */
  if(base != NULL)
    editIO((EditorMapIO*)base);
}

/*
 * Description: Triggers to edit the hover tile IO instance at the index.
 *
 * Inputs: int index - the depth index on the tile
 * Output: none
 */
void MapIOView::editHoverInstance(int index)
{
  if(editor_map != NULL)
  {
    /* Check if hover tile is valid */
    EditorTile* t = editor_map->getHoverInfo()->hover_tile;
    if(t != NULL)
    {
      /* Check if IO at index is valid */
      EditorMapIO* io = t->getIO(index);
      if(io != NULL)
        editIO(io);
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
void MapIOView::editInstance()
{
  if(io_instances->currentItem() != NULL)
  {
    int id = MapThingView::getInstanceID(io_instances->currentItem()->text());

    EditorMapIO* io = editor_map->getIO(id, editor_map->getCurrentMap()->id);
    if(io != NULL)
      editIO(io);
  }
}

/*
 * Description: Instance list item double clicked. Just triggers the edit
 *              instance action.
 *
 * Inputs: QListWidgetItem* - not used
 * Output: none
 */
void MapIOView::instanceDoubleClicked(QListWidgetItem*)
{
  editInstance();
}

/*
 * Description: Slot which triggers a right click menu when an instance is
 *              right clicked within the list. Offers delete and edit actions.
 *
 * Inputs: const QPoint & pos - the position right clicked in widget
 * Output: none
 */
void MapIOView::instanceMenu(const QPoint & pos)
{
  QListWidgetItem* item = io_instances->itemAt(pos);
  if(item != NULL)
    rightclick_menu->exec(QCursor::pos());
}

/*
 * Description: Slot which triggers when a row in the instance list of IOs
 *              changes. This changes which IO is hovered on the map.
 *
 * Inputs: int index - the new index of the row in the list
 *         bool lock_viewport - lock the viewport from jumping to IO.
 *                              Default to false.
 * Output: none
 */
void MapIOView::instanceRowChanged(int index, bool lock_viewport)
{
  if(index >= 0 && editor_map != NULL)
  {
    /* Set the hover IO in the class */
    int io_id = MapThingView::getInstanceID(
                                         io_instances->currentItem()->text());
    if(editor_map->setHoverIO(io_id))
    {
      EditorMapIO* io = editor_map->getIO(io_id, 
                                          editor_map->getCurrentMap()->id);
      if(io != NULL)
      {
        if(!lock_viewport)
          emit ensureVisible(editor_map->getCurrentMap()->tiles[io->getX()]
                                                               [io->getY()]);

        /* Select the base in the list */
        if(io->getBaseIO() != NULL)
        {
          /* Get index and block signals */
          int index = editor_map->getIOIndex(io->getBaseIO()->getID());
          io_list->blockSignals(true);

          /* Processing */
          io_list->setCurrentRow(index);
          editor_map->setCurrentIO(index);
          if(editor_map->getHoverInfo()->active_layer != EditorEnumDb::IO)
            emit changeLayer(EditorEnumDb::IO);

          /* Unblock and update */
          io_list->blockSignals(false);
          updateInfo();
        }
      }
    }
  }
}

/*
 * Description: Slot which triggers when an item in the base list is double
 *              clicked. Opens a pop-up for the IO to be edited.
 *
 * Inputs: QListWidgetItem* - not used
 * Output: none
 */
void MapIOView::itemDoubleClicked(QListWidgetItem*)
{
  editIO();
}

/*
 * Description: Slot which triggers to update the instances in the view and
 *              put them in the list. Triggers on change events, such as editing
 *              and deleting instances, and on initial setup
 *
 * Inputs: QString name_list - the list of the added IO. Blank if N/A
 * Output: none
 */
void MapIOView::ioInstanceUpdate(QString name_list)
{
  io_instances->blockSignals(true);
  io_instances->clearSelection();
  io_instances->clearFocus();
  io_instances->clear();

  if(editor_map != NULL)
  {
    /* Set up the instances list */
    int sub_index = editor_map->getCurrentMapIndex();
    if(sub_index >= 0)
    {
      for(int i = 0; i < editor_map->getIOCount(sub_index); i++)
      {
        io_instances->addItem(editor_map->getIOByIndex(i, sub_index)
                                                               ->getNameList());
      }
      io_instances->sortItems();
    }
  }

  io_instances->clearSelection();
  io_instances->clearFocus();

  /* If name list is not blank, select the name in the list */
  if(!name_list.isEmpty())
  {
    for(int i = 0; i < io_instances->count(); i++)
    {
      if(io_instances->item(i)->text() == name_list)
      {
        io_instances->setCurrentRow(i);
        instanceRowChanged(i, true);
      }
    }
  }

  io_instances->blockSignals(false);
}

/*
 * Description: Refreshes the Editor IO list.
 *
 * Inputs: none
 * Output: none
 */
void MapIOView::updateList()
{
  int index = io_list->currentRow();

  /* Set up the base list */
  io_list->clear();
  if(editor_map != NULL)
  {
    /* Set up the base list */
    for(int i = 0; i < editor_map->getIOCount(); i++)
      io_list->addItem(editor_map->getIOByIndex(i)->getNameList());
    editor_map->updateAll();

    /* Set up the instances list */
    ioInstanceUpdate("");
  }

  io_list->setCurrentRow(index);
  updateInfo();
  update();
}

/*
 * Description: Slot which triggers to update a IO upon completion by the
 *              IO dialog (on ok()). This will also update all instances of
 *              the object, deleting as necessary.
 *
 * Inputs: none
 * Output: none
 */
void MapIOView::updateIOs()
{
  /* Update the IO in the pop-up */
  if(io_dialog != NULL)
  {
    editor_map->tilesIORemove();
    io_dialog->updateOriginal();
    editor_map->tilesIOAdd();
  }

  /* Update the list in the view */
  updateList();
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Deletes the selected IO in the base list. Triggered by
 *              parent.
 *
 * Inputs: none
 * Output: bool - returns true if IO deleted.
 */
bool MapIOView::deleteIO()
{
  EditorMapIO* selected = getSelected();
  bool success = false;

  /* If valid, proceed deletion */
  if(selected != NULL)
  {
    /* Create warning about deleting */
    QMessageBox msg_box;
    msg_box.setText("Deleting base IO " + selected->getNameList());
    msg_box.setInformativeText(
                  "This will delete ALL instances of this base. Are you sure?");
    msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    if(msg_box.exec() == QMessageBox::Yes)
    {
      /* Proceed with deletion */
      int index = io_list->currentRow();
      success = editor_map->unsetIOByIndex(index);

      /* If successfull, reset list */
      if(success)
      {
        updateList();
        if(index >= io_list->count())
          io_list->setCurrentRow(io_list->count() - 1);
      }
    }
  }

  return success;
}

/*
 * Description: Duplicates the selected IO in the base list. Triggered by
 *              parent.
 *
 * Inputs: none
 * Output: bool - returns true if IO was duplicated.
 */
bool MapIOView::duplicateIO()
{
  EditorMapIO* selected = getSelected();

  /* If valid, proceed duplication */
  if(selected != NULL)
  {
    EditorMapIO* new_io = new EditorMapIO(*selected);
    if(addIO(new_io))
      return true;
  }

  return false;
}

/*
 * Description: Returns which IO is selected in the base list.
 *
 * Inputs: none
 * Output: EditorMapIO* - selected IO. NULL if none selected
 */
EditorMapIO* MapIOView::getSelected()
{
  int index = io_list->currentRow();
  EditorMapIO* io = NULL;

  /* Check the validity */
  if(editor_map != NULL)
    io = editor_map->getIOByIndex(index);

  return io;
}

/*
 * Description: Returns the editor map, which contains all IOs and instances.
 *
 * Inputs: none
 * Output: EditorMap* - pointer to the editor map. NULL if none set.
 */
EditorMap* MapIOView::getEditorMap()
{
  return editor_map;
}

/*
 * Description: Imports a new IO into the base list. Triggered by
 *              parent.
 *
 * Inputs: none
 * Output: none
 */
void MapIOView::importIO()
{
  // TODO: Future
  QMessageBox::information(this, "Notification",
                           "Coming soon to a production near you!");
}

/*
 * Description: Creates a new IO into the base list. Triggered by
 *              parent. Proceeds to call edit on the new IO and opens
 *              IODialog.
 *
 * Inputs: none
 * Output: none
 */
void MapIOView::newIO()
{
  EditorMapIO* new_io = new EditorMapIO();
  if(addIO(new_io))
    editIO();
}

/*
 * Description: Sets the control editor map. This contains the IOs, sprites
 *              and all data relevant to display.
 *
 * Inputs: EditorMap* map - the map to use for control
 * Output: none
 */
void MapIOView::setEditorMap(EditorMap* map)
{
  /* If existing editor map is not NULL, undo */
  if(editor_map != NULL)
  {
    disconnect(editor_map, SIGNAL(ioInstanceChanged(QString)),
               this, SLOT(ioInstanceUpdate(QString)));
  }

  editor_map = map;

  /* If new map is not NULL, reconnect */
  if(editor_map != NULL)
  {
    connect(editor_map, SIGNAL(ioInstanceChanged(QString)),
            this, SLOT(ioInstanceUpdate(QString)));
  }

  /* Finally, update list */
  updateList();
}

/*
 * Description: Sets the list of items, used for event creation
 *
 * Inputs: QList<QString> - list of all items (for give item event)
 * Output: none
 */
void MapIOView::updateListItems(QList<QString> list)
{
  if(io_dialog != NULL)
    io_dialog->setListItems(list);
  if(instance_dialog != NULL)
    instance_dialog->setListItems(list);
}

/*
 * Description: Sets the list of maps, used for event creation
 *
 * Inputs: QList<QString> - list of all maps (for change map event)
 * Output: none
 */
void MapIOView::updateListMaps(QList<QString> list)
{
  if(io_dialog != NULL)
    io_dialog->setListMaps(list);
  if(instance_dialog != NULL)
    instance_dialog->setListMaps(list);
}

/*
 * Description: Sets the list of things, ios, items, persons, npcs, used for
 *              event creation.
 *
 * Inputs: QList<QString> things - list of all map things
 *         QList<QPair<QString,QString>> ios - list of all map ios
 *         QList<QString> items - list of all map items
 *         QList<QString> persons - list of all map persons
 *         QList<QString> npcs - list of all map npcs
 * Output: none
 */
void MapIOView::updateListMapThings(QList<QString> things,
                                    QList<QPair<QString,QString>> ios,
                                    QList<QString> items,
                                    QList<QString> persons,
                                    QList<QString> npcs)
{
  if(io_dialog != nullptr)
    io_dialog->setListMapThings(things, ios, items, persons, npcs);
  if(instance_dialog != nullptr)
    instance_dialog->setListMapThings(things, ios, items, persons, npcs);
}

/*
 * Description: Sets the list of maps, used for the party dropdown
 *
 * Inputs: QList<QString> - list of all parties (for selected dropdown)
 * Output: none
 */
void MapIOView::updateListParties(QList<QString> list)
{
  if(instance_dialog != NULL)
    instance_dialog->setListParties(list);
}

/*
 * Description: Sets the list of sounds, used for the sound dropdown
 *
 * Inputs: QList<QString> list - list of all sound resources available
 * Output: none
 */
void MapIOView::updateListSounds(QList<QString> list)
{
  if(io_dialog != nullptr)
    io_dialog->setListSounds(list);
  if(instance_dialog != nullptr)
    instance_dialog->setListSounds(list);
}

/*
 * Description: Sets the list of sub-maps, used for event creation.
 *
 * Inputs: QList<QString> - list of all sub-maps (for teleport event)
 * Output: none
 */
void MapIOView::updateListSubmaps(QList<QString> list)
{
  if(io_dialog != NULL)
    io_dialog->setListSubmaps(list);
  if(instance_dialog != NULL)
    instance_dialog->setListSubmaps(list);
}

/*
 * Description: Updates the event embedded in the IO dialog with the
 *              selected tile. Used for event creation. Called by parent.
 *
 * Inputs: int id - the id of the sub-map
 *         int x, y - the coordinates in the map
 * Output: none
 */
void MapIOView::updateSelectedTile(int id, int x, int y)
{
  if(io_dialog != NULL)
    io_dialog->updateSelectedTile(id, x, y);
  if(instance_dialog != NULL)
    instance_dialog->updateSelectedTile(id, x, y);
}
