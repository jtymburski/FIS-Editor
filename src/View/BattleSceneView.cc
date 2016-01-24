/*******************************************************************************
 * Class Name: BattleSceneView
 * Date Created: January 23, 2016
 * Inheritance: QWidget
 * Description: The battle scene view and editor which handles the properly
 *              preview and edit capabilities to the scene.
 ******************************************************************************/
#include "View/BattleSceneView.h"
#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor Function */
BattleSceneView::BattleSceneView(QWidget* parent) : QWidget(parent)
{
  scene_base = nullptr;

  createLayout();
  loadWorkingInfo();
}

/* Constructor function with Editor Battle Scene reference */
BattleSceneView::BattleSceneView(EditorBattleScene* scene, QWidget* parent)
               : BattleSceneView(parent)
{
  setScene(scene);
}

/* Copy constructor */
BattleSceneView::BattleSceneView(const BattleSceneView &source)
               : BattleSceneView()
{
  copySelf(source);
}

/* Destructor function */
BattleSceneView::~BattleSceneView()
{
  setScene(nullptr);
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/* Copy function, to be called by a copy or equal operator constructor */
void BattleSceneView::copySelf(const BattleSceneView &source)
{

}

/* Creates interface layout */
void BattleSceneView::createLayout()
{
  /* Layout */
  QGridLayout* layout = new QGridLayout(this);
  layout->setColumnStretch(5, 1);
  layout->setRowMinimumHeight(10, 15);
  layout->setRowStretch(5, 1);

  /* ID */
  QLabel* lbl_id = new QLabel("ID", this);
  lbl_id_view = new QLabel("XXX", this);
  lbl_id_view->setStyleSheet("border: 1px solid #afafaf");
  layout->addWidget(lbl_id, 0, 0);
  layout->addWidget(lbl_id_view, 0, 1);

  /* Name */
  QLabel* lbl_name = new QLabel("Name", this);
  edit_name = new QLineEdit(this);
  connect(edit_name, SIGNAL(textEdited(QString)),
          this, SLOT(changedName(QString)));
  layout->addWidget(lbl_name, 1, 0);
  layout->addWidget(edit_name, 1, 1, 1, 3);

  /* Music */
  QLabel* lbl_music = new QLabel("Music", this);
  combo_music = new QComboBox(this);
  connect(combo_music, SIGNAL(currentIndexChanged(QString)),
          this, SLOT(changedMusic(QString)));
  layout->addWidget(lbl_music, 2, 0);
  layout->addWidget(combo_music, 2, 1, 1, 3);

  /* Background */
  QLabel* lbl_background = new QLabel("Background", this);
  lbl_background_view = new QLabel("sprites/test/doop.png", this);
  lbl_background_view->setStyleSheet("border: 1px solid #afafaf");
  QPushButton* btn_background = new QPushButton("Select", this);
  connect(btn_background, SIGNAL(clicked()), this, SLOT(btnBackground()));
  layout->addWidget(lbl_background, 3, 0);
  layout->addWidget(lbl_background_view, 3, 1, 1, 3);
  layout->addWidget(btn_background, 3, 4);

  /* Preview */
  QLabel* lbl_preview = new QLabel("Preview", this);
  lbl_preview_img = new QLabel(this);
  lbl_preview_img->setStyleSheet("border: 1px solid #afafaf");
  layout->addWidget(lbl_preview, 4, 0);
  layout->addWidget(lbl_preview_img, 5, 0, 5, 6);

  /* Lay Over - Main */
  QLabel* lbl_layover_title = new QLabel("Lay Overs", this);
  combo_layover = new QComboBox(this);
  QStringList items;
  items << "Underlays" << "Midlays" << "Overlays";
  combo_layover->addItems(items);
  connect(combo_layover, SIGNAL(currentIndexChanged(QString)),
          this, SLOT(changedLayClass(QString)));
  QPushButton* btn_lay_add = new QPushButton("Add", this);
  connect(btn_lay_add, SIGNAL(clicked()), this, SLOT(btnLayAdd()));
  btn_lay_del = new QPushButton("Delete", this);
  btn_lay_del->setDisabled(true);
  connect(btn_lay_del, SIGNAL(clicked()), this, SLOT(btnLayDelete()));
  btn_lay_down = new QPushButton("Down", this);
  btn_lay_down->setDisabled(true);
  connect(btn_lay_down, SIGNAL(clicked()), this, SLOT(btnLayDown()));
  btn_lay_edit = new QPushButton("Edit", this);
  btn_lay_edit->setDisabled(true);
  connect(btn_lay_edit, SIGNAL(clicked()), this, SLOT(btnLayEdit()));
  btn_lay_up = new QPushButton("Up", this);
  btn_lay_up->setDisabled(true);
  connect(btn_lay_up, SIGNAL(clicked()), this, SLOT(btnLayUp()));
  list_layover = new QListWidget(this);
  connect(list_layover, SIGNAL(currentRowChanged(int)),
          this, SLOT(listLayOverChanged(int)));
  connect(list_layover, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
          this, SLOT(listLayOverDClicked(QListWidgetItem*)));
  layout->addWidget(lbl_layover_title, 0, 6, 1, 5, Qt::AlignHCenter);
  layout->addWidget(combo_layover, 1, 6, 1, 5);
  layout->addWidget(btn_lay_add, 2, 6);
  layout->addWidget(btn_lay_edit, 2, 7);
  layout->addWidget(btn_lay_del, 2, 8);
  layout->addWidget(btn_lay_up, 2, 9);
  layout->addWidget(btn_lay_down, 2, 10);
  layout->addWidget(list_layover, 3, 6, 3, 5);

  /* Lay Over - Edit Path */
  QLabel* lbl_lay_path = new QLabel("Path", this);
  lbl_lay_path_list = new QLabel("sprites/test/doop.png", this);
  lbl_lay_path_list->setStyleSheet("border: 1px solid #afafaf");
  btn_lay_path = new QPushButton("Select", this);
  btn_lay_path->setDisabled(true);
  connect(btn_lay_path, SIGNAL(clicked()), this, SLOT(btnLayPath()));
  layout->addWidget(lbl_lay_path, 6, 6);
  layout->addWidget(lbl_lay_path_list, 6, 7, 1, 3);
  layout->addWidget(btn_lay_path, 6, 10);

  /* Lay Over - Edit Animate Time */
  QLabel* lbl_lay_anim = new QLabel("Animate Time", this);
  spin_lay_anim = new QSpinBox(this);
  spin_lay_anim->setSuffix(" ms");
  spin_lay_anim->setDisabled(true);
  connect(spin_lay_anim, SIGNAL(valueChanged(int)),
          this, SLOT(spinLayAnimChanged(int)));
  layout->addWidget(lbl_lay_anim, 7, 6);
  layout->addWidget(spin_lay_anim, 7, 7, 1, 2);

  /* Lay Over - Edit Velocity X */
  QLabel* lbl_lay_velx = new QLabel("Velocity X", this);
  spin_lay_velx = new QSpinBox(this);
  spin_lay_velx->setSuffix(" pixels/s");
  spin_lay_velx->setDisabled(true);
  connect(spin_lay_velx, SIGNAL(valueChanged(int)),
          this, SLOT(spinLayVelXChanged(int)));
  layout->addWidget(lbl_lay_velx, 8, 6);
  layout->addWidget(spin_lay_velx, 8, 7, 1, 2);

  /* Lay Over - Edit Velocity Y */
  QLabel* lbl_lay_vely = new QLabel("Velocity Y", this);
  spin_lay_vely = new QSpinBox(this);
  spin_lay_vely->setSuffix(" pixels/s");
  spin_lay_vely->setDisabled(true);
  connect(spin_lay_vely, SIGNAL(valueChanged(int)),
          this, SLOT(spinLayVelYChanged(int)));
  layout->addWidget(lbl_lay_vely, 9, 6);
  layout->addWidget(spin_lay_vely, 9, 7, 1, 2);

  /* Reset / Save finish */
  QPushButton* btn_reset = new QPushButton("Reset", this);
  connect(btn_reset, SIGNAL(clicked()), this, SLOT(btnReset()));
  QPushButton* btn_save = new QPushButton("Save", this);
  connect(btn_save, SIGNAL(clicked()), this, SLOT(btnSave()));
  layout->addWidget(btn_reset, 11, 9);
  layout->addWidget(btn_save, 11, 10);

  /* Disable widget on initial creation */
  setDisabled(true);
}

/* Loads working info into UI objects */
void BattleSceneView::loadWorkingInfo()
{
  /* ID */
  lbl_id_view->setText(QString::number(scene_curr.getID()));

  /* Name */
  edit_name->setText(scene_curr.getName());

  /* Music - find index */
  int music_index = -1;
  if(music_list.size() > 0 && scene_curr.getMusicID() >= 0)
  {
    /* Try and find index */
    for(int i = 0; i < music_list.size(); i++)
    {
      QStringList str_split = music_list[i].split(':');
      if(str_split.size() >= 2)
        if(str_split.front().toInt() == scene_curr.getMusicID())
          music_index = i;
    }

    /* If not found, set music ID to unset */
    if(music_index < 0)
      scene_curr.setMusicID(-1);
  }

  /* Music combo box */
  combo_music->blockSignals(true);
  combo_music->clear();
  combo_music->addItems(QStringList(music_list));
  if(music_index >= 0)
    combo_music->setCurrentIndex(music_index);
  combo_music->blockSignals(false);

  /* Background */
  QString background = scene_curr.getBackground();
  if(background.isEmpty())
    lbl_background_view->setText("Not Set");
  else
    lbl_background_view->setText(scene_curr.getBackground());

  // TODO: Layovers

  /* Final enabled/disabled status */
  setEnabled(scene_base != nullptr);
}

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/

/* Button Triggers */
void BattleSceneView::btnBackground()
{

}

/* Button Triggers */
void BattleSceneView::btnLayAdd()
{

}

/* Button Triggers */
void BattleSceneView::btnLayDelete()
{

}

/* Button Triggers */
void BattleSceneView::btnLayDown()
{

}

/* Button Triggers */
void BattleSceneView::btnLayEdit()
{

}

/* Button Triggers */
void BattleSceneView::btnLayPath()
{

}

/* Button Triggers */
void BattleSceneView::btnLayUp()
{

}

/* Button Triggers */
void BattleSceneView::btnReset()
{
  if(scene_base->isChanged())
  {
    // TODO
  }
}

/* Button Triggers */
void BattleSceneView::btnSave()
{
  if(scene_base->isChanged())
  {
    // TODO
  }
}

/* Widget Change Triggers */
void BattleSceneView::changedLayClass(const QString& category)
{

}

/* Widget Change Triggers */
void BattleSceneView::changedMusic(const QString& text)
{

}

/* Widget Change Triggers */
void BattleSceneView::changedName(const QString& name)
{

}

/* List Index Widget Changes */
void BattleSceneView::listLayOverChanged(int index)
{

}

/* List Index Widget Changes */
void BattleSceneView::listLayOverDClicked(QListWidgetItem*)
{

}

/* Spin widget value changes */
void BattleSceneView::spinLayAnimChanged(int value)
{

}

/* Spin widget value changes */
void BattleSceneView::spinLayVelXChanged(int value)
{

}

/* Spin widget value changes */
void BattleSceneView::spinLayVelYChanged(int value)
{

}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Returns data, as required */
QList<QString> BattleSceneView::getListMusic()
{
  return music_list;
}

/* Returns the base scene that is being edited */
EditorBattleScene* BattleSceneView::getScene()
{
  return scene_base;
}

/* Returns if the battle scene information has been edited but not saved */
bool BattleSceneView::isChanged()
{
  if(scene_base != nullptr)
    return scene_base->isChanged();
  return false;
}

/* Resets the working set trigger */
void BattleSceneView::resetWorking()
{
  if(scene_base != nullptr)
    btnReset();
}

/* Saves the working set trigger */
void BattleSceneView::saveWorking()
{
  if(scene_base != nullptr)
    btnSave();
}

/* Sets the battle scene information */
void BattleSceneView::setScene(EditorBattleScene* scene)
{
  scene_base = scene;
  if(scene_base != nullptr)
  {
    scene_base->setChanged(false);
    scene_curr = *scene_base;
  }
  loadWorkingInfo();
}

/* Updates list in thing dialog, needed for event control */
void BattleSceneView::updateListMusic(QList<QString> list)
{
  music_list = list;
  loadWorkingInfo();
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/* The copy operator */
BattleSceneView& BattleSceneView::operator= (const BattleSceneView &source)
{
  /* Check for self assignment */
  if(this == &source)
    return *this;

  /* Do the copy */
  copySelf(source);

  /* Return the copied object */
  return *this;
}
