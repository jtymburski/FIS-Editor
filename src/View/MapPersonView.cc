/*******************************************************************************
 * Class Name: MapPersonView
 * Date Created: March 6, 2015
 * Inheritance: QWidget
 * Description: The side toolbar in the map database that gives access to the
 *              map persons.
 ******************************************************************************/
#include "View/MapPersonView.h"
#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function. Creates the person view, which is the
 *              side bar within the left map database. Controls persons (bases
 *              and instances).
 *
 * Inputs: QWidget* parent - the parent widget
 */
MapPersonView::MapPersonView(QWidget* parent) : QWidget(parent)
{
  /* Initialize variables */
  editor_map = NULL;
  instance_dialog = NULL;
  person_dialog = NULL;

  /* Create the layout */
  createLayout();
}

/*
 * Description: Destructor function
 */
MapPersonView::~MapPersonView()
{

}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: Adds person to the base list. Triggered by the "new person"
 *              press. The ID is the next available.
 *
 * Inputs: EditorMapPerson* person - the new person to add (ID not set yet)
 * Output: none
 */
void MapPersonView::addPerson(EditorMapPerson* person)
{
  if(editor_map != NULL)
  {
    /* Sets the id and puts the person in the library */
    person->setID(editor_map->getNextPersonID());
    person->setTileIcons(editor_map->getTileIcons());
    int index = editor_map->setPerson(person);

    /* Increments the id tracker and updates view */
    updateList();
    person_list->setCurrentRow(index);
  }
}

/*
 * Description: Creates the dialog layout with QT functional widgets.
 *
 * Inputs: none
 * Output: none
 */
void MapPersonView::createLayout()
{
  /* Layout */
  QVBoxLayout* layout = new QVBoxLayout(this);

  /* The list widget */
  person_list = new QListWidget(this);
  connect(person_list, SIGNAL(currentRowChanged(int)),
          this, SLOT(currentRowChanged(int)));
  connect(person_list, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
          this, SLOT(itemDoubleClicked(QListWidgetItem*)));
  layout->addWidget(person_list, 1);

  /* The instances widget */
  QLabel* lbl_instances = new QLabel("Instances:", this);
  layout->addWidget(lbl_instances, 0);
  person_instances = new QListWidget(this);
  person_instances->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(person_instances, SIGNAL(customContextMenuRequested(QPoint)),
          this, SLOT(instanceMenu(QPoint)));
  connect(person_instances, SIGNAL(currentRowChanged(int)),
          this, SLOT(instanceRowChanged(int)));
  connect(person_instances, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
          this, SLOT(instanceDoubleClicked(QListWidgetItem*)));
  layout->addWidget(person_instances, 1);

  /* Right click menu control */
  rightclick_menu = new QMenu("Person Edit", this);
  QAction* edit_thing = new QAction("Edit", rightclick_menu);
  connect(edit_thing, SIGNAL(triggered()), this, SLOT(editInstance()));
  QAction* delete_thing = new QAction("Delete", rightclick_menu);
  connect(delete_thing, SIGNAL(triggered()), this, SLOT(deleteInstance()));
  rightclick_menu->addAction(edit_thing);
  rightclick_menu->addAction(delete_thing);
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

  /* Size label */
  lbl_size = new QLabel("Size:", this);
  layout->addWidget(lbl_size, 0, Qt::AlignHCenter);
}

/*
 * Description: Opens the edit person dialog, on an existing person in the list
 *              (either base or instance).
 *
 * Inputs: EditorMapPerson* sub_person - the person to edit
 * Output: none
 */
void MapPersonView::editPerson(EditorMapPerson* sub_person)
{
  EditorMapPerson* current = getSelected();
  if(sub_person != NULL)
    current = sub_person;

  /* -- Is an instance -- */
  if(current->getBasePerson() != NULL)
  {
    if(instance_dialog != NULL)
    {
      disconnect(instance_dialog, SIGNAL(ok(QString)),
                 this, SLOT(personInstanceUpdate(QString)));
      disconnect(
            instance_dialog, SIGNAL(selectTile(EditorEnumDb::MapObjectMode)),
            this, SIGNAL(selectTile(EditorEnumDb::MapObjectMode)));
      disconnect(instance_dialog, SIGNAL(editBase(EditorMapThing*)),
                 this, SLOT(editBasePerson(EditorMapThing*)));
      delete instance_dialog;
    }
    instance_dialog = new InstanceDialog(current, this);
    connect(instance_dialog, SIGNAL(ok(QString)),
            this, SLOT(personInstanceUpdate(QString)));
    connect(instance_dialog, SIGNAL(selectTile(EditorEnumDb::MapObjectMode)),
            this, SIGNAL(selectTile(EditorEnumDb::MapObjectMode)));
    connect(instance_dialog, SIGNAL(editBase(EditorMapThing*)),
            this, SLOT(editBasePerson(EditorMapThing*)));
    instance_dialog->show();
  }
  /* -- Is a base -- */
  else
  {
    /* Delete the old and create the new dialog */
    if(person_dialog != NULL)
    {
      disconnect(person_dialog, SIGNAL(ok()), this, SLOT(updatePersons()));
      disconnect(person_dialog, SIGNAL(selectTile(EditorEnumDb::MapObjectMode)),
                 this, SIGNAL(selectTile(EditorEnumDb::MapObjectMode)));
      delete person_dialog;
    }
    person_dialog = new PersonDialog(current, this);
    connect(person_dialog, SIGNAL(ok()), this, SLOT(updatePersons()));
    connect(person_dialog, SIGNAL(selectTile(EditorEnumDb::MapObjectMode)),
            this, SIGNAL(selectTile(EditorEnumDb::MapObjectMode)));
    person_dialog->show();
  }

  /* Fills the dialogs with data */
  emit fillWithData(EditorEnumDb::PERSON_VIEW);
}

/*
 * Description: Updates the info in the QT widgets within the view.
 *
 * Inputs: none
 * Output: none
 */
void MapPersonView::updateInfo()
{
  if(editor_map != NULL)
  {
    /* Blank the labels */
    lbl_id->setText("ID:");
    lbl_image->setPixmap(QPixmap());
    lbl_name->setText("Name:");
    lbl_size->setText("Size:  |  Frames:");

    EditorMapPerson* person = getSelected();
    if(person != NULL)
    {
      /* Set the labels */
      lbl_id->setText("ID: " + QString::number(person->getID()));
      lbl_name->setText("Name: " + person->getName());

      /* If matrix is valid, set the remaining info */
      if(person->getMatrix() != NULL)
      {
        lbl_image->setPixmap(person->getMatrix()->getSnapshot(200, 200));
        lbl_size->setText("Size: " +
                    QString::number(person->getMatrix()->getWidth()) +
                    "W x " +
                    QString::number(person->getMatrix()->getHeight()) +
                    "H  |  Frames: " +
                    QString::number(person->getMatrix()->getTrimFrames() + 1));
      }
    }
  }
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

/*
 * Description: Slot which triggers when the top list changes. Updates the
 *              selected person.
 *
 * Inputs: int index - the new index in the list
 * Output: none
 */
void MapPersonView::currentRowChanged(int index)
{
  if(editor_map != NULL)
  {
    editor_map->setCurrentPerson(index);
    editor_map->setHoverPerson(-1);
  }

  updateInfo();
  person_instances->clearSelection();
}

/*
 * Description: Slot which triggers when an instance is deleted, from a right
 *              click select on the instance.
 *
 * Inputs: none
 * Output: none
 */
void MapPersonView::deleteInstance()
{
  if(person_instances->currentItem() != NULL)
  {
    QString text = person_instances->currentItem()->text();

    /* Create warning about deleting */
    QMessageBox msg_box;
    msg_box.setText("Deleting person instance: " + text);
    msg_box.setInformativeText("Are you sure?");
    msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    if(msg_box.exec() == QMessageBox::Yes)
    {
      int index = MapThingView::getInstanceID(text);
      editor_map->unsetPerson(index, true);
    }
  }
}

/*
 * Description: Triggers by instance dialog to edit base of the instant.
 *
 * Inputs: EditorMapThing* base - the base person to edit
 * Output: none
 */
void MapPersonView::editBasePerson(EditorMapThing* base)
{
  if(base != NULL)
    editPerson((EditorMapPerson*)base);
}

/*
 * Description: Slot which triggers when an instance is edited, from a right
 *              click select on the instance. Opens up edit dialog.
 *
 * Inputs: none
 * Output: none
 */
void MapPersonView::editInstance()
{
  if(person_instances->currentItem() != NULL)
  {
    int id = MapThingView::getInstanceID(
                                 person_instances->currentItem()->text());

    EditorMapPerson* person = editor_map->getPerson(
                                           id, editor_map->getCurrentMap()->id);
    if(person != NULL)
      editPerson(person);
  }
}

/*
 * Description: Instance list item double clicked. Just triggers the edit
 *              instance action.
 *
 * Inputs: QListWidgetItem* - not used
 * Output: none
 */
void MapPersonView::instanceDoubleClicked(QListWidgetItem*)
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
void MapPersonView::instanceMenu(const QPoint & pos)
{
  QListWidgetItem* item = person_instances->itemAt(pos);
  if(item != NULL)
    rightclick_menu->exec(QCursor::pos());
}

/*
 * Description: Slot which triggers when a row in the instance list of persons
 *              changes. This changes which person is hovered on the map.
 *
 * Inputs: int index - the new index of the row in the list
 * Output: none
 */
void MapPersonView::instanceRowChanged(int index)
{
  if(index >= 0 && editor_map != NULL)
  {
    /* Set the hover person in the class */
    int person_id = MapThingView::getInstanceID(
                                       person_instances->currentItem()->text());
    if(editor_map->setHoverPerson(person_id))
    {
      EditorMapPerson* person = editor_map->getPerson(person_id,
                                               editor_map->getCurrentMap()->id);
      if(person != NULL)
      {
        emit ensureVisible(editor_map->getCurrentMap()->tiles[person->getX()]
                                                             [person->getY()]);

        /* Select the base in the list */
        if(person->getBasePerson() != NULL)
        {
          int index = editor_map->getPersonIndex(
                                              person->getBasePerson()->getID());
          person_list->blockSignals(true);
          person_list->setCurrentRow(index);
          person_list->blockSignals(false);
          updateInfo();
        }
      }
    }
  }
}

/*
 * Description: Slot which triggers when an item in the base list is double
 *              clicked. Opens a pop-up for the person to be edited.
 *
 * Inputs: QListWidgetItem* - not used
 * Output: none
 */
void MapPersonView::itemDoubleClicked(QListWidgetItem*)
{
  editPerson();
}

/*
 * Description: Slot which triggers to update the instances in the view and
 *              put them in the list. Triggers on change events, such as editing
 *              and deleting instances, and on initial setup
 *
 * Inputs: QString name_list - the list of the added item. Blank if N/A
 * Output: none
 */
void MapPersonView::personInstanceUpdate(QString name_list)
{
  person_instances->blockSignals(true);
  person_instances->clearSelection();
  person_instances->clearFocus();
  person_instances->clear();

  if(editor_map != NULL)
  {
    /* Set up the instances list */
    int sub_index = editor_map->getCurrentMapIndex();
    if(sub_index >= 0)
    {
      for(int i = 0; i < editor_map->getPersonCount(sub_index); i++)
      {
        person_instances->addItem(
                    editor_map->getPersonByIndex(i, sub_index)->getNameList());
      }
      person_instances->sortItems();
    }
  }

  person_instances->clearSelection();
  person_instances->clearFocus();
  person_instances->blockSignals(false);

  /* If name list is not blank, select the name in the list */
  if(!name_list.isEmpty())
  {
    for(int i = 0; i < person_instances->count(); i++)
      if(person_instances->item(i)->text() == name_list)
        person_instances->setCurrentRow(i);
  }
}

/*
 * Description: Refreshes the Editor Person list.
 *
 * Inputs: none
 * Output: none
 */
void MapPersonView::updateList()
{
  int index = person_list->currentRow();

  /* Set up the base list */
  person_list->clear();
  if(editor_map != NULL)
  {
    /* Set up the base list */
    for(int i = 0; i < editor_map->getPersonCount(); i++)
      person_list->addItem(editor_map->getPersonByIndex(i)->getNameList());
    editor_map->updateAll();

    /* Set up the instances list */
    personInstanceUpdate("");
  }

  person_list->setCurrentRow(index);
  updateInfo();
  update();
}

/*
 * Description: Slot which triggers to update a person upon completion by the
 *              person dialog (on ok()). This will also update all instances of
 *              the object, deleting as necessary.
 *
 * Inputs: none
 * Output: none
 */
void MapPersonView::updatePersons()
{
  /* Update the person in the pop-up */
  if(person_dialog != NULL)
  {
    editor_map->tilesPersonRemove();
    person_dialog->updateOriginal();
    editor_map->tilesPersonAdd();
  }

  /* Update the list in the view */
  updateList();
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Deletes the selected person in the base list. Triggered by
 *              parent.
 *
 * Inputs: none
 * Output: bool - returns true if person deleted.
 */
bool MapPersonView::deletePerson()
{
  EditorMapPerson* selected = getSelected();
  bool success = false;

  /* If valid, proceed deletion */
  if(selected != NULL)
  {
    /* Create warning about deleting */
    QMessageBox msg_box;
    msg_box.setText("Deleting base person " + selected->getNameList());
    msg_box.setInformativeText(
                  "This will delete ALL instances of this base. Are you sure?");
    msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    if(msg_box.exec() == QMessageBox::Yes)
    {
      /* Proceed with deletion */
      int index = person_list->currentRow();
      success = editor_map->unsetPersonByIndex(index);

      /* If successfull, reset list */
      if(success)
      {
        updateList();
        if(index >= person_list->count())
          person_list->setCurrentRow(person_list->count() - 1);
      }
    }
  }

  return success;
}

/*
 * Description: Duplicates the selected person in the base list. Triggered by
 *              parent.
 *
 * Inputs: none
 * Output: bool - returns true if person was duplicated.
 */
bool MapPersonView::duplicatePerson()
{
  EditorMapPerson* selected = getSelected();

  /* If valid, proceed duplication */
  if(selected != NULL)
  {
    EditorMapPerson* new_person = new EditorMapPerson(*selected);
    addPerson(new_person);
    return true;
  }

  return false;
}

/*
 * Description: Returns which person is selected in the base list.
 *
 * Inputs: none
 * Output: EditorMapPerson* - selected person. NULL if none selected
 */
EditorMapPerson* MapPersonView::getSelected()
{
  int index = person_list->currentRow();
  EditorMapPerson* person = NULL;

  /* Check the validity */
  if(editor_map != NULL)
    person = editor_map->getPersonByIndex(index);

  return person;
}

/*
 * Description: Returns the editor map, which contains all persons and instances
 *
 * Inputs: none
 * Output: EditorMap* - pointer to the editor map. NULL if none set.
 */
EditorMap* MapPersonView::getEditorMap()
{
  return editor_map;
}

/*
 * Description: Imports a new person into the base list. Triggered by
 *              parent.
 *
 * Inputs: none
 * Output: none
 */
void MapPersonView::importPerson()
{
  // TODO: Future
  QMessageBox::information(this, "Notification",
                           "Coming soon to a production near you!");
}

/*
 * Description: Creates a new person into the base list. Triggered by
 *              parent. Proceeds to call edit on the new person and opens
 *              PersonDialog.
 *
 * Inputs: none
 * Output: none
 */
void MapPersonView::newPerson()
{
  EditorMapPerson* new_person = new EditorMapPerson();
  addPerson(new_person);
  editPerson();
}

/*
 * Description: Sets the control editor map. This contains the persons, sprites
 *              and all data relevant to display.
 *
 * Inputs: EditorMap* map - the map to use for control
 * Output: none
 */
void MapPersonView::setEditorMap(EditorMap* map)
{
  /* If existing editor map is not NULL, undo */
  if(editor_map != NULL)
  {
    disconnect(editor_map, SIGNAL(personInstanceChanged(QString)),
               this, SLOT(personInstanceUpdate(QString)));
  }

  editor_map = map;

  /* If new map is not NULL, reconnect */
  if(editor_map != NULL)
  {
    connect(editor_map, SIGNAL(personInstanceChanged(QString)),
            this, SLOT(personInstanceUpdate(QString)));
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
void MapPersonView::updateListItems(QVector<QString> list)
{
  if(person_dialog != NULL)
    person_dialog->getEventView()->setListItems(list);
  if(instance_dialog != NULL)
    instance_dialog->getEventView()->setListItems(list);
}

/*
 * Description: Sets the list of maps, used for event creation
 *
 * Inputs: QVector<QString> - list of all maps (for change map event)
 * Output: none
 */
void MapPersonView::updateListMaps(QVector<QString> list)
{
  if(person_dialog != NULL)
    person_dialog->getEventView()->setListMaps(list);
  if(instance_dialog != NULL)
    instance_dialog->getEventView()->setListMaps(list);
}

/*
 * Description: Sets the list of sub-maps, used for event creation.
 *
 * Inputs: QVector<QString> - list of all sub-maps (for teleport event)
 * Output: none
 */
void MapPersonView::updateListSubmaps(QVector<QString> list)
{
  if(person_dialog != NULL)
    person_dialog->getEventView()->setListSubmaps(list);
  if(instance_dialog != NULL)
    instance_dialog->getEventView()->setListSubmaps(list);
}

/*
 * Description: Sets the list of things, used for event creation.
 *
 * Inputs: QVector<QString> - list of all things (for teleport event)
 * Output: none
 */
void MapPersonView::updateListThings(QVector<QString> list)
{
  if(person_dialog != NULL)
    person_dialog->getEventView()->setListThings(list);
  if(instance_dialog != NULL)
    instance_dialog->getEventView()->setListThings(list);
}

/*
 * Description: Updates the event embedded in the person dialog with the
 *              selected tile. Used for event creation. Called by parent.
 *
 * Inputs: int id - the id of the sub-map
 *         int x, y - the coordinates in the map
 * Output: none
 */
void MapPersonView::updateSelectedTile(int id, int x, int y)
{
  if(person_dialog != NULL)
    person_dialog->updateSelectedTile(id, x, y);
  if(instance_dialog != NULL)
    instance_dialog->updateSelectedTile(id, x, y);
}
