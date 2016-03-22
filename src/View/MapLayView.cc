/*******************************************************************************
 * Class Name: MapLayView
 * Date Created: March 20, 2016
 * Inheritance: QWidget
 * Description: The side toolbar in the map database that gives access to the
 *              lay overs for each sub map.
 ******************************************************************************/
#include "View/MapLayView.h"
#include <QDebug>

/*============================================================================
 * CONSTRUCTOR / DESTRUCTOR FUNCTIONS
 *===========================================================================*/

/* Constructor Function */
MapLayView::MapLayView(QWidget* parent) : QWidget(parent)
{
  /* Initialize variables */
  editor_map = nullptr;

  /* Create the layout */
  createLayout();
}

/* Destructor function */
MapLayView::~MapLayView()
{
  setEditorMap(nullptr);
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* Creates the layout - only called on initial construction */
void MapLayView::createLayout()
{
  /* Layout */
  QGridLayout* layout = new QGridLayout(this);

  /* Layover selection */
  combo_laytype = new QComboBox(this);
  QStringList items;
  items << "Underlays" << "Overlays"; /* If changed, update LayType enum */
  combo_laytype->addItems(items);
  connect(combo_laytype, SIGNAL(currentTextChanged(QString)),
          this, SLOT(changedLayType(QString)));
  layout->addWidget(combo_laytype, 0, 0, 1, 5);
  QToolButton* btn_add = new QToolButton(this);
  btn_add->setIcon(QIcon(":/images/icons/32_plus.png"));
  connect(btn_add, SIGNAL(clicked()), this, SLOT(buttonAdd()));
  layout->addWidget(btn_add, 0, 5);

  /* List of lays (depending on selection combo box) */
  list_lays = new QListWidget(this);
  connect(list_lays, SIGNAL(currentRowChanged(int)),
          this, SLOT(changedListLay(int)));
  layout->addWidget(list_lays, 1, 0, 1, 6);

  /* Horizontal set of buttons */
  btn_del = new QPushButton("Delete", this);
  btn_del->setDisabled(true);
  connect(btn_del, SIGNAL(clicked()), this, SLOT(buttonDelete()));
  layout->addWidget(btn_del, 2, 0, 1, 2);
  btn_up = new QPushButton("Up", this);
  btn_up->setDisabled(true);
  connect(btn_up, SIGNAL(clicked()), this, SLOT(buttonUp()));
  layout->addWidget(btn_up, 2, 2, 1, 2);
  btn_down = new QPushButton("Down", this);
  btn_down->setDisabled(true);
  connect(btn_down, SIGNAL(clicked()), this, SLOT(buttonDown()));
  layout->addWidget(btn_down, 2, 4, 1, 2);

  /* Path */
  QLabel* lbl_path = new QLabel("Path", this);
  lbl_path_list = new QLabel("path/sample/here.png", this);
  lbl_path_list->setStyleSheet("border: 1px solid #afafaf");
  lbl_path_list->setDisabled(true);
  btn_path = new QToolButton(this);
  btn_path->setIcon(QIcon(":/images/icons/32_settings.png"));
  btn_path->setDisabled(true);
  connect(btn_path, SIGNAL(clicked()), this, SLOT(buttonPath()));
  layout->addWidget(lbl_path, 3, 0);
  layout->addWidget(lbl_path_list, 3, 1, 1, 4);
  layout->addWidget(btn_path, 3, 5);

  /* Animation Time */
  QLabel* lbl_anim = new QLabel("Animate", this);
  spin_anim = new QSpinBox(this);
  spin_anim->setSuffix(" ms");
  spin_anim->setMaximum(99999);
  spin_anim->setDisabled(true);
  connect(spin_anim, SIGNAL(valueChanged(int)),
          this, SLOT(spinAnimChanged(int)));
  layout->addWidget(lbl_anim, 4, 0);
  layout->addWidget(spin_anim, 4, 1, 1, 3);

  /* Velocity X */
  QLabel* lbl_velx = new QLabel("Speed X", this);
  spin_velx = new QSpinBox(this);
  spin_velx->setSuffix(" pixels/s");
  spin_velx->setMinimum(-68000);
  spin_velx->setMaximum(68000);
  spin_velx->setDisabled(true);
  connect(spin_velx, SIGNAL(valueChanged(int)),
          this, SLOT(spinVelXChanged(int)));
  layout->addWidget(lbl_velx, 5, 0);
  layout->addWidget(spin_velx, 5, 1, 1, 3);

  /* Velocity Y */
  QLabel* lbl_vely = new QLabel("Speed Y", this);
  spin_vely = new QSpinBox(this);
  spin_vely->setSuffix(" pixels/s");
  spin_vely->setMinimum(-68000);
  spin_vely->setMaximum(68000);
  spin_vely->setDisabled(true);
  connect(spin_vely, SIGNAL(valueChanged(int)),
          this, SLOT(spinVelYChanged(int)));
  layout->addWidget(lbl_vely, 6, 0);
  layout->addWidget(spin_vely, 6, 1, 1, 3);

  /* Player Relative */
  chk_player = new QCheckBox("Speed Relative to Player", this);
  chk_player->setDisabled(true);
  connect(chk_player, SIGNAL(stateChanged(int)),
          this, SLOT(checkPlayerChanged(int)));
  layout->addWidget(chk_player, 7, 1, 1, 4);
}

/* Returns the selected lay over in the list */
LayOver* MapLayView::getSelectedLay()
{
  LayOver* lay_ref = nullptr;

  /* Now determine if row is valid and get information */
  if(editor_map != nullptr && editor_map->getCurrentMap() != nullptr)
  {
    SubMapInfo* map = editor_map->getCurrentMap();

    int index = list_lays->currentRow();
    if(index >= 0)
    {
      if(combo_laytype->currentIndex() == LayType::UNDERLAYS)
      {
        if(index < map->lays_under.size())
          lay_ref = &map->lays_under[index];
      }
      else /* OVERLAYS */
      {
        if(index < map->lays_over.size())
          lay_ref = &map->lays_over[index];
      }
    }
  }

  return lay_ref;
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

/* Button control triggers */
void MapLayView::buttonAdd()
{
  if(editor_map != nullptr && editor_map->getCurrentMap() != nullptr)
  {
    SubMapInfo* map = editor_map->getCurrentMap();

    /* Determine which type to add and add it */
    if(combo_laytype->currentIndex() == LayType::UNDERLAYS)
    {
      map->lays_under.push_back(Helpers::createBlankLayOver());
    }
    else /* OVERLAYS */
    {
      map->lays_over.push_back(Helpers::createBlankLayOver());
    }

    /* Update view and select new row */
    updateData();
    list_lays->setCurrentRow(list_lays->count() - 1);
  }
}

/* Button control triggers */
void MapLayView::buttonDelete()
{
  // TODO
}

/* Button control triggers */
void MapLayView::buttonDown()
{
  // TODO
}

/* Button control triggers */
void MapLayView::buttonPath()
{
  // TODO
}

/* Button control triggers */
void MapLayView::buttonUp()
{
  // TODO
}

/* Changed triggers in widgets */
void MapLayView::changedLayType(QString)
{
  updateData();
}

/* Changed triggers in widgets */
void MapLayView::changedListLay(int row)
{
  /* Set widgets as initially disabled */
  btn_del->setDisabled(true);
  btn_down->setDisabled(true);
  btn_path->setDisabled(true);
  btn_up->setDisabled(true);
  chk_player->setDisabled(true);
  lbl_path_list->setDisabled(true);
  spin_anim->setDisabled(true);
  spin_velx->setDisabled(true);
  spin_vely->setDisabled(true);

  /* Now determine if row is valid and get information */
  if(row >= 0)
  {
    /* Button enables */
    btn_del->setEnabled(true);
    btn_down->setEnabled(row < (list_lays->count() - 1));
    btn_up->setEnabled(row > 0);

    /* Get the reference lay over */
    LayOver* lay_ref = getSelectedLay();
    if(lay_ref != nullptr)
    {
      /* Set widgets as enabled */
      lbl_path_list->setEnabled(true);
      btn_path->setEnabled(true);
      spin_anim->setEnabled(true);
      spin_velx->setEnabled(true);
      spin_vely->setEnabled(true);
      chk_player->setEnabled(true);

      /* Load in new information */
      lbl_path_list->setText(QString::fromStdString(lay_ref->path));
      spin_anim->setValue(lay_ref->anim_time);
      float velx = (lay_ref->velocity_x * 1000);
      if(velx > 0)
        velx += 0.1;
      else if(velx < 0)
        velx -= 0.1;
      spin_velx->setValue(static_cast<int>(velx));
      float vely = (lay_ref->velocity_y * 1000);
      if(vely > 0)
        vely += 0.1;
      else if(vely < 0)
        vely -= 0.1;
      spin_vely->setValue(static_cast<int>(vely));
      chk_player->setChecked(lay_ref->player_relative);
    }
  }
}

/* Check box change triggers */
void MapLayView::checkPlayerChanged(int state)
{
  LayOver* lay_ref = getSelectedLay();
  if(lay_ref != nullptr)
  {
    lay_ref->player_relative = (state == Qt::Checked);
  }
}

/* Spin box change triggers */
void MapLayView::spinAnimChanged(int value)
{
  LayOver* lay_ref = getSelectedLay();
  if(lay_ref != nullptr)
  {
    lay_ref->anim_time = value;
  }
}

/* Spin box change triggers */
void MapLayView::spinVelXChanged(int value)
{
  LayOver* lay_ref = getSelectedLay();
  if(lay_ref != nullptr)
  {
    lay_ref->velocity_x = (value / 1000.0);
  }
}

/* Spin box change triggers */
void MapLayView::spinVelYChanged(int value)
{
  LayOver* lay_ref = getSelectedLay();
  if(lay_ref != nullptr)
  {
    lay_ref->velocity_y = (value / 1000.0);
  }
}

/* Refreshes the entire data set within the widget */
void MapLayView::updateData()
{
  if(editor_map != nullptr && editor_map->getCurrentMap() != nullptr)
  {
    SubMapInfo* map = editor_map->getCurrentMap();

    /* Clear list */
    list_lays->clear();
    changedListLay(-1);

    /* Load in new information */
    if(combo_laytype->currentIndex() == LayType::UNDERLAYS)
    {
      for(int i = 0; i < map->lays_under.size(); i++)
      {
        if(map->lays_under[i].path.empty())
          list_lays->addItem(QString::number(i) + ": <no path set>");
        else
          list_lays->addItem(QString::number(i) + ": " +
                             QString::fromStdString(map->lays_under[i].path));
      }
    }
    else /* OVERLAYS */
    {
      for(int i = 0; i < map->lays_over.size(); i++)
      {
        if(map->lays_over[i].path.empty())
          list_lays->addItem(QString::number(i) + ": <no path set>");
        else
          list_lays->addItem(QString::number(i) + ": " +
                             QString::fromStdString(map->lays_over[i].path));
      }
    }
  }
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Gets the editor map */
EditorMap* MapLayView::getEditorMap()
{
  return editor_map;
}

/* Sets the editor map, which contains the data needed */
void MapLayView::setEditorMap(EditorMap* map)
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
