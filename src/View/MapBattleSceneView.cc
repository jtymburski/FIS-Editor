/*******************************************************************************
 * Class Name: MapBattleSceneView
 * Date Created: January 25, 2016
 * Inheritance: QWidget
 * Description: The side toolbar in the map database that gives access to the
 *              battle scenes for each sub map. This also includes provisions
 *              for the core based battle scenes.
 ******************************************************************************/
#include "include/View/MapBattleSceneView.h"

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
  layout->addWidget(list_core_avail);

  /* Core data - buttons */
  QHBoxLayout* layout_core_btns = new QHBoxLayout();
  btn_core_add = new QPushButton("Add", this);
  btn_core_add->setDisabled(true);
  btn_core_add->setDefault(true);
  layout_core_btns->addWidget(btn_core_add);
  btn_core_rem = new QPushButton("Remove", this);
  btn_core_rem->setDisabled(true);
  layout_core_btns->addWidget(btn_core_rem);
  layout->addLayout(layout_core_btns);

  /* Core data - used */
  list_core_used = new QListWidget(this);
  layout->addWidget(list_core_used);

  /* Sub map data - available */
  QLabel* lbl_sub = new QLabel("Sub Map Scenes", this);
  layout->addWidget(lbl_sub, 0, Qt::AlignHCenter);
  list_sub_avail = new QListWidget(this);
  layout->addWidget(list_sub_avail);

  /* Sub map data - buttons */
  QHBoxLayout* layout_sub_btns = new QHBoxLayout();
  btn_sub_add = new QPushButton("Add", this);
  btn_sub_add->setDisabled(true);
  btn_sub_add->setDefault(true);
  layout_sub_btns->addWidget(btn_sub_add);
  btn_sub_rem = new QPushButton("Remove", this);
  btn_sub_rem->setDisabled(true);
  layout_sub_btns->addWidget(btn_sub_rem);
  layout->addLayout(layout_sub_btns);

  /* Sub map data - used */
  list_sub_used = new QListWidget(this);
  layout->addWidget(list_sub_used);
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

/* Refreshes the entire data set within the widget */
void MapBattleSceneView::updateData()
{

}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Returns data, as required */
QList<QString> MapBattleSceneView::getDataScenes()
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

/* Updates list used within the view for battle scene information */
void MapBattleSceneView::updateListScenes(QList<QString> list)
{
  scene_list = list;
  updateData();
}
