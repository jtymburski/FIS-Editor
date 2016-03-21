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
  items << "Underlays" << "Overlays";
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

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

/* Button control triggers */
void MapLayView::buttonAdd()
{
  // TODO
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
void MapLayView::changedLayType(const QString &text)
{
  // TODO
}

/* Changed triggers in widgets */
void MapLayView::changedListLay(int row)
{
  // TODO
}

/* Check box change triggers */
void MapLayView::checkPlayerChanged(int state)
{
  // TODO
}

/* Spin box change triggers */
void MapLayView::spinAnimChanged(int value)
{
  // TODO
}

/* Spin box change triggers */
void MapLayView::spinVelXChanged(int value)
{
  // TODO
}

/* Spin box change triggers */
void MapLayView::spinVelYChanged(int value)
{
  // TODO
}

/* Refreshes the entire data set within the widget */
void MapLayView::updateData()
{
  // TODO
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
