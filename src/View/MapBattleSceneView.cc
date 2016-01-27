/*******************************************************************************
 * Class Name: MapBattleSceneView
 * Date Created: January 25, 2016
 * Inheritance: QWidget
 * Description: The side toolbar in the map database that gives access to the
 *              battle scenes for each sub map. This also includes provisions
 *              for the core based battle scenes.
 ******************************************************************************/
#include "include/View/MapBattleSceneView.h"
//#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor Function */
MapBattleSceneView::MapBattleSceneView(QWidget* parent) : QWidget(parent)
{
  /* Initialize variables */
  editor_map = nullptr;

  /* Create the layout */
  createLayout();
}

/* Destructor function */
MapBattleSceneView::~MapBattleSceneView()
{
  setEditorMap(nullptr);
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* Creates the layout - only called on initial construction */
void MapBattleSceneView::createLayout()
{
  /* Layout */
  QVBoxLayout* layout = new QVBoxLayout(this);

  /* Core data - available */
  QLabel* lbl_core = new QLabel("Core Map Scenes", this);
  layout->addWidget(lbl_core, 0, Qt::AlignHCenter);
  list_core_avail = new QListWidget(this);
  connect(list_core_avail, SIGNAL(currentRowChanged(int)),
          this, SLOT(changedListCoreAvail(int)));
  layout->addWidget(list_core_avail);

  /* Core data - buttons */
  QHBoxLayout* layout_core_btns = new QHBoxLayout();
  btn_core_add = new QPushButton("Add", this);
  btn_core_add->setDisabled(true);
  btn_core_add->setDefault(true);
  connect(btn_core_add, SIGNAL(clicked()), this, SLOT(buttonCoreAdd()));
  layout_core_btns->addWidget(btn_core_add);
  btn_core_rem = new QPushButton("Remove", this);
  btn_core_rem->setDisabled(true);
  connect(btn_core_rem, SIGNAL(clicked()), this, SLOT(buttonCoreRemove()));
  layout_core_btns->addWidget(btn_core_rem);
  layout->addLayout(layout_core_btns);

  /* Core data - used */
  list_core_used = new QListWidget(this);
  connect(list_core_used, SIGNAL(currentRowChanged(int)),
          this, SLOT(changedListCoreUsed(int)));
  layout->addWidget(list_core_used);

  /* Sub map data - available */
  QLabel* lbl_sub = new QLabel("Sub Map Scenes", this);
  layout->addWidget(lbl_sub, 0, Qt::AlignHCenter);
  list_sub_avail = new QListWidget(this);
  connect(list_sub_avail, SIGNAL(currentRowChanged(int)),
          this, SLOT(changedListSubAvail(int)));
  layout->addWidget(list_sub_avail);

  /* Sub map data - buttons */
  QHBoxLayout* layout_sub_btns = new QHBoxLayout();
  btn_sub_add = new QPushButton("Add", this);
  btn_sub_add->setDisabled(true);
  btn_sub_add->setDefault(true);
  connect(btn_sub_add, SIGNAL(clicked()), this, SLOT(buttonSubAdd()));
  layout_sub_btns->addWidget(btn_sub_add);
  btn_sub_rem = new QPushButton("Remove", this);
  btn_sub_rem->setDisabled(true);
  connect(btn_sub_rem, SIGNAL(clicked()), this, SLOT(buttonSubRemove()));
  layout_sub_btns->addWidget(btn_sub_rem);
  layout->addLayout(layout_sub_btns);

  /* Sub map data - used */
  list_sub_used = new QListWidget(this);
  connect(list_sub_used, SIGNAL(currentRowChanged(int)),
          this, SLOT(changedListSubUsed(int)));
  layout->addWidget(list_sub_used);
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

/* Button control triggers */
void MapBattleSceneView::buttonCoreAdd()
{
  if(editor_map != nullptr && list_core_avail->currentRow() >= 0)
  {
    QString curr_item = list_core_avail->currentItem()->text();
    QStringList str_split = curr_item.split(':');
    if(str_split.size() >= 2)
    {
      editor_map->setBattleScene(editor_map->getBattleSceneCount(),
                                 str_split.front().toInt());
      updateData();
    }
  }
}

/* Button control triggers */
void MapBattleSceneView::buttonCoreRemove()
{
  if(editor_map != nullptr && list_core_used->currentRow() >= 0)
  {
    QString curr_item = list_core_used->currentItem()->text();
    QStringList str_split = curr_item.split(':');
    if(str_split.size() >= 2)
    {
      int id = str_split.front().toInt();
      int index = editor_map->getBattleScenes().indexOf(id);
      if(index >= 0)
      {
        editor_map->unsetBattleScene(index);
        updateData();
      }
    }
  }
}

/* Button control triggers */
void MapBattleSceneView::buttonSubAdd()
{
  if(editor_map != nullptr && editor_map->getCurrentMap() != nullptr &&
     list_sub_avail->currentRow() >= 0)
  {
    QString curr_item = list_sub_avail->currentItem()->text();
    QStringList str_split = curr_item.split(':');
    if(str_split.size() >= 2)
    {
      editor_map->getCurrentMap()->battle_scenes.push_back(
                                                   str_split.front().toInt());
      updateData();
    }
  }
}

/* Button control triggers */
void MapBattleSceneView::buttonSubRemove()
{
  if(editor_map != nullptr && editor_map->getCurrentMap() != nullptr &&
     list_sub_used->currentRow() >= 0)
  {
    QString curr_item = list_sub_used->currentItem()->text();
    QStringList str_split = curr_item.split(':');
    if(str_split.size() >= 2)
    {
      int id = str_split.front().toInt();
      int index = editor_map->getCurrentMap()->battle_scenes.indexOf(id);
      if(index >= 0)
      {
        editor_map->getCurrentMap()->battle_scenes.remove(index);
        updateData();
      }
    }
  }
}

/* Changed triggers in widgets */
void MapBattleSceneView::changedListCoreAvail(int row)
{
  if(row >= 0 && list_core_avail->count() > 0)
    btn_core_add->setEnabled(true);
  else
    btn_core_add->setDisabled(true);
}

/* Changed triggers in widgets */
void MapBattleSceneView::changedListCoreUsed(int row)
{
  if(row >= 0 && list_core_used->count() > 0)
    btn_core_rem->setEnabled(true);
  else
    btn_core_rem->setDisabled(true);
}

/* Changed triggers in widgets */
void MapBattleSceneView::changedListSubAvail(int row)
{
  if(row >= 0)
    btn_sub_add->setEnabled(true);
  else
    btn_sub_add->setDisabled(true);
}

/* Changed triggers in widgets */
void MapBattleSceneView::changedListSubUsed(int row)
{
  if(row >= 0)
    btn_sub_rem->setEnabled(true);
  else
    btn_sub_rem->setDisabled(true);
}

/* Refreshes the entire data set within the widget */
void MapBattleSceneView::updateData()
{
  bool valid_map = (editor_map != nullptr);
  bool valid_submap = (valid_map && editor_map->getCurrentMap() != nullptr);

  /* Fills the scenes with data */
  //emit fillWithData(EditorEnumDb::MAPBATTLESCENE_VIEW);

  /* Core data for scenes */
  int index_core_avail = list_core_avail->currentRow();
  list_core_avail->clear();
  int index_core_used = list_core_used->currentRow();
  list_core_used->clear();
  btn_core_add->setEnabled(false);
  btn_core_rem->setEnabled(false);
  if(valid_map && scene_list.size() > 0)
  {
    QVector<bool> valid_list;

    /* Find valid IDs */
    for(int i = 0; i < editor_map->getBattleSceneCount(); i++)
    {
      bool found = false;
      int id = editor_map->getBattleScene(i);

      /* Try and find */
      if(id >= 0)
      {
        for(int j = 0; j < scene_list.size(); j++)
          if(id == scene_list[j].first)
            found = true;
      }

      valid_list.push_back(found);
    }

    /* Clean out invalid */
    for(int i = valid_list.size() - 1; i >= 0; i--)
    {
      if(!valid_list[i])
        editor_map->unsetBattleScene(i);
    }

    /* Proceed to update the lists */
    for(int i = 0; i < scene_list.size(); i++)
    {
      QString entry = QString::number(scene_list[i].first) + ": " +
                      scene_list[i].second;
      if(editor_map->getBattleScenes().contains(scene_list[i].first))
        list_core_used->addItem(entry);
      else
        list_core_avail->addItem(entry);
    }

    /* Restore indexes */
    if(list_core_avail->count() > 0 && index_core_avail >= 0)
    {
      if(index_core_avail < list_core_avail->count())
        list_core_avail->setCurrentRow(index_core_avail);
      else
        list_core_avail->setCurrentRow(list_core_avail->count() - 1);
    }
    if(list_core_used->count() > 0 && index_core_used >= 0)
    {
      if(index_core_used < list_core_used->count())
        list_core_used->setCurrentRow(index_core_used);
      else
        list_core_used->setCurrentRow(list_core_used->count() - 1);
    }
  }

  /* Sub data for scenes */
  int index_sub_avail = list_sub_avail->currentRow();
  list_sub_avail->clear();
  int index_sub_used = list_sub_used->currentRow();
  list_sub_used->clear();
  btn_sub_add->setEnabled(false);
  btn_sub_rem->setEnabled(false);
  if(valid_submap && scene_list.size() > 0)
  {
    QVector<bool> valid_list;
    SubMapInfo* map = editor_map->getCurrentMap();

    /* Find valid IDs */
    for(int i = 0; i < map->battle_scenes.size(); i++)
    {
      bool found = false;
      int id = map->battle_scenes[i];

      /* Try and find */
      if(id >= 0)
      {
        for(int j = 0; j < scene_list.size(); j++)
          if(id == scene_list[j].first)
            found = true;
      }

      valid_list.push_back(found);
    }

    /* Clean out invalid */
    for(int i = valid_list.size() - 1; i >= 0; i--)
    {
      if(!valid_list[i])
        map->battle_scenes.remove(i);
    }

    /* Proceed to update the lists */
    for(int i = 0; i < scene_list.size(); i++)
    {
      QString entry = QString::number(scene_list[i].first) + ": " +
                      scene_list[i].second;
      if(map->battle_scenes.contains(scene_list[i].first))
        list_sub_used->addItem(entry);
      else
        list_sub_avail->addItem(entry);
    }

    /* Restore indexes */
    if(list_sub_avail->count() > 0 && index_sub_avail >= 0)
    {
      if(index_sub_avail < list_sub_avail->count())
        list_sub_avail->setCurrentRow(index_sub_avail);
      else
        list_sub_avail->setCurrentRow(list_sub_avail->count() - 1);
    }
    if(list_sub_used->count() > 0 && index_sub_used >= 0)
    {
      if(index_sub_used < list_sub_used->count())
        list_sub_used->setCurrentRow(index_sub_used);
      else
        list_sub_used->setCurrentRow(list_sub_used->count() - 1);
    }
  }

  /* Widget Enables */
  list_core_avail->setEnabled(valid_map);
  list_core_used->setEnabled(valid_map);
  list_sub_avail->setEnabled(valid_submap);
  list_sub_used->setEnabled(valid_submap);
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Returns data, as required */
QList<QPair<int,QString>> MapBattleSceneView::getDataScenes()
{
  return scene_list;
}

/* Gets the editor map */
EditorMap* MapBattleSceneView::getEditorMap()
{
  return editor_map;
}

/* Sets the editor map, which contains the data needed */
void MapBattleSceneView::setEditorMap(EditorMap* map)
{
  if(editor_map != map)
  {
    /* If existing editor map is not NULL, undo */
    if(editor_map != nullptr)
    {
      disconnect(editor_map, SIGNAL(activeSubChanged()),
                 this, SLOT(updateData()));
    }

    /* Set map */
    editor_map = map;

    /* If new map is not NULL, reconnect */
    if(editor_map != nullptr)
    {
      connect(editor_map, SIGNAL(activeSubChanged()),
              this, SLOT(updateData()));
    }

    /* Update data */
    updateData();
  }
}

/* Updates list used within the view for battle scene information */
void MapBattleSceneView::updateListScenes(QList<QPair<int,QString>> list)
{
  scene_list = list;
  updateData();
}
