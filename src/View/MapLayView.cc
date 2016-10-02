/*******************************************************************************
 * Class Name: MapLayView
 * Date Created: March 20, 2016
 * Inheritance: QWidget
 * Description: The side toolbar in the map database that gives access to the
 *              lay overs for each sub map.
 ******************************************************************************/
#include "View/MapLayView.h"
//#include <QDebug>

/*============================================================================
 * CONSTRUCTOR / DESTRUCTOR FUNCTIONS
 *===========================================================================*/

/*
 * Description: Base constructor function. Just defines parent and creates
 *              layout.
 *
 * Inputs: QWidget* parent - parent widget
 */
MapLayView::MapLayView(QWidget* parent) : QWidget(parent)
{
  /* Initialize variables */
  editor_map = nullptr;

  /* Create the layout */
  createLayout();
}

/*
 * Description: Destructor function
 */
MapLayView::~MapLayView()
{
  setEditorMap(nullptr);
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/*
 * Description: Creates the map lay view layout with QT functional widgets.
 *
 * Inputs: none
 * Output: none
 */
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

/*
 * Description: Processes the current settings of the widgets to return the
 *              lay over reference struct selected. If nothing found or any
 *              error, a null pointer is returned.
 *
 * Inputs: none
 * Output: LayOver* - the lay over reference struct connected to the selection
 */
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

/*
 * Description: Button trigger on layover add press. This will access the
 *              correct category (underlay, overlay) and adds it to the
 *              appropriate set.
 *
 * Inputs: none
 * Output: none
 */
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

/*
 * Description: Button trigger on layover delete press. This will access the
 *              correct category (underlay, overlay) and delete the
 *              selected index in the appropriate set.
 *
 * Inputs: none
 * Output: none
 */
void MapLayView::buttonDelete()
{
  if(editor_map != nullptr && editor_map->getCurrentMap() != nullptr &&
     list_lays->currentRow() >= 0)
  {
    bool deleted = false;
    int index = list_lays->currentRow();
    SubMapInfo* map = editor_map->getCurrentMap();

    /* Create the message box */
    QMessageBox msg_box;
    msg_box.setText("Deleting lay at index " + QString::number(index));
    msg_box.setInformativeText("Are you sure?");
    msg_box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

    /* Determine which type and attempt to remove it */
    if(combo_laytype->currentIndex() == LayType::UNDERLAYS)
    {
      if(index < map->lays_under.size())
      {
        if(msg_box.exec() == QMessageBox::Yes)
        {
          map->lays_under.remove(index);
          deleted = true;
        }
      }
    }
    else /* OVERLAYS */
    {
      if(index < map->lays_over.size())
      {
        if(msg_box.exec() == QMessageBox::Yes)
        {
          map->lays_over.remove(index);
          deleted = true;
        }
      }
    }

    /* Update view and select row at same location */
    if(deleted)
    {
      editor_map->updateLays();
      updateData();
      if(list_lays->count() > 0)
      {
        if(index < list_lays->count())
          list_lays->setCurrentRow(index);
        else
          list_lays->setCurrentRow(list_lays->count() - 1);
      }
    }
  }
}

/*
 * Description: Button trigger on layover down press. This will access the
 *              correct category (underlay, overlay) and move the
 *              selected index down by one spot in the appropriate set.
 *
 * Inputs: none
 * Output: none
 */
void MapLayView::buttonDown()
{
  if(editor_map != nullptr && editor_map->getCurrentMap() != nullptr &&
     list_lays->currentRow() >= 0)
  {
    int index = list_lays->currentRow();
    SubMapInfo* map = editor_map->getCurrentMap();

    /* Determine which type and attempt to shift it down */
    if(combo_laytype->currentIndex() == LayType::UNDERLAYS)
    {
      if((index + 1) < map->lays_under.size())
      {
        LayOver swap = map->lays_under[index];
        map->lays_under[index] = map->lays_under[index + 1];
        map->lays_under[index + 1] = swap;
        index++;
      }
    }
    else /* OVERLAYS */
    {
      if((index + 1) < map->lays_over.size())
      {
        LayOver swap = map->lays_over[index];
        map->lays_over[index] = map->lays_over[index + 1];
        map->lays_over[index + 1] = swap;
        index++;
      }
    }

    /* Update view and select new row */
    editor_map->updateLays();
    updateData();
    if(list_lays->count() > 0)
    {
      if(index < list_lays->count())
        list_lays->setCurrentRow(index);
      else
        list_lays->setCurrentRow(list_lays->count() - 1);
    }
  }
}

/*
 * Description: Button trigger on layover edit path select button. This will
 *              select a new path for the selected edit layover if one exists.
 *              This call can handle sprite selection (instead of just frame)
 *
 * Inputs: none
 * Output: none
 */
void MapLayView::buttonPath()
{
  LayOver* lay_ref = getSelectedLay();
  if(lay_ref != nullptr)
  {
    /* Choose the layover path */
    QString last_path = EditorHelpers::getPreviousPath();
    QString path = QFileDialog::getOpenFileName(this,
                                                tr("Select a layover sprite"),
                                                last_path,
                                                tr("Image Files (*.png)"));
    if(path != "")
    {
      /* Path parsing */
      EditorHelpers::setPreviousPath(path);
      path = QDir::toNativeSeparators(path);
      QStringList path_set = path.split(QDir::separator());
      QString initial_file = path_set.last();
      QString initial_path = path;
      initial_path.remove(initial_file);

      /* Split file and determine if last two in file name are digits */
      QString no_tail = initial_file;
      no_tail.chop(QString(".png").size());
      QChar dig1_str = no_tail.at(no_tail.size() - 2);
      QChar dig2_str = no_tail.at(no_tail.size() - 1);
      if(dig1_str.isDigit() && dig2_str.isDigit())
      {
        /* Check if the last two are 0; therefore base sprite */
        int dig1 = dig1_str.digitValue();
        int dig2 = dig2_str.digitValue();
        if(dig1 == 0 && dig2 == 0)
        {
          /* Load into sprite struct and determine valid summarized path */
          QString no_tail_or_num = no_tail;
          no_tail_or_num.chop(2);
          EditorSprite sprite_test;
          sprite_test.addPath(initial_path + no_tail_or_num + "|" +
                              QString::number(20) + "|.png");

          /* Clean out tail frames */
          int max_size = sprite_test.getLastValidFrame();
          while((max_size + 1) < sprite_test.frameCount())
            sprite_test.deleteFrame(sprite_test.frameCount() - 1);
          \
          /* Set the new path */
          QList<QPair<QString,QString>> set = sprite_test.getPathSet();
          if(set.size() == 1)
            path = EditorHelpers::getProjectDir() + QDir::separator() +
                   set.front().second;
        }
      }

      /* Final path output */
      lay_ref->path = EditorHelpers::trimPath(path).toStdString();
      editor_map->updateLays();

      /* Update the display */
      QString display_txt = EditorHelpers::getFilename(
                                        QString::fromStdString(lay_ref->path));
      lbl_path_list->setText(display_txt);
      list_lays->currentItem()->setText(
                QString::number(list_lays->currentRow()) + ": " + display_txt);
    }
  }
}

/*
 * Description: Button trigger on layover up press. This will access the
 *              correct category (underlay, overlay) and move the
 *              selected index up by one spot in the appropriate set.
 *
 * Inputs: none
 * Output: none
 */
void MapLayView::buttonUp()
{
  if(editor_map != nullptr && editor_map->getCurrentMap() != nullptr &&
     list_lays->currentRow() > 0)
  {
    int index = list_lays->currentRow();
    SubMapInfo* map = editor_map->getCurrentMap();

    /* Determine which type and attempt to shift it down */
    if(combo_laytype->currentIndex() == LayType::UNDERLAYS)
    {
      if(index < map->lays_under.size())
      {
        LayOver swap = map->lays_under[index];
        map->lays_under[index] = map->lays_under[index - 1];
        map->lays_under[index - 1] = swap;
        index--;
      }
    }
    else /* OVERLAYS */
    {
      if(index < map->lays_over.size())
      {
        LayOver swap = map->lays_over[index];
        map->lays_over[index] = map->lays_over[index - 1];
        map->lays_over[index - 1] = swap;
        index--;
      }
    }

    /* Update view and select new row */
    editor_map->updateLays();
    updateData();
    if(list_lays->count() > 0)
    {
      if(index < list_lays->count())
        list_lays->setCurrentRow(index);
      else
        list_lays->setCurrentRow(list_lays->count() - 1);
    }
  }
}

/*
 * Description: Slot triggered when the layover combo box drop down selection
 *              is changed. This updates which layover type is being viewed.
 *
 * Inputs: QString - not used
 * Output: none
 */
void MapLayView::changedLayType(QString)
{
  updateData();
}

/*
 * Description: Slot triggered when the selected lay over in the list is
 *              modified. This changes which lay over is being edited
 *              automatically
 *
 * Inputs: int row - the selected row in the list
 * Output: none
 */
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
      lbl_path_list->setText(EditorHelpers::getFilename(
                                       QString::fromStdString(lay_ref->path)));
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
    if(lay_ref->player_relative)
    {
      spin_velx->setSuffix(" px / kpx moved");
      spin_vely->setSuffix(" px / kpx moved");
    }
    else
    {
      spin_velx->setSuffix(" pixels / s");
      spin_vely->setSuffix(" pixels / s");
    }
  }
}

/*
 * Description: Slot triggered when the animation delay change is modified.
 *              This function is only valid if an edit is active
 *
 * Inputs: int value - the new animation time value, in ms
 * Output: none
 */
void MapLayView::spinAnimChanged(int value)
{
  LayOver* lay_ref = getSelectedLay();
  if(lay_ref != nullptr)
  {
    lay_ref->anim_time = value;
  }
}

/*
 * Description: Slot triggered when the layover velocity x value is changed.
 *              This function is only valid if an edit is active
 *
 * Inputs: int value - the new velocity x value, in tiles/second
 * Output: none
 */
void MapLayView::spinVelXChanged(int value)
{
  LayOver* lay_ref = getSelectedLay();
  if(lay_ref != nullptr)
  {
    lay_ref->velocity_x = (value / 1000.0);
  }
}

/*
 * Description: Slot triggered when the layover velocity y value is changed.
 *              This function is only valid if an edit is active
 *
 * Inputs: int value - the new velocity y value, in tiles/second
 * Output: none
 */
void MapLayView::spinVelYChanged(int value)
{
  LayOver* lay_ref = getSelectedLay();
  if(lay_ref != nullptr)
  {
    lay_ref->velocity_y = (value / 1000.0);
  }
}

/*
 * Description: Updates the info in the QT widgets within the view.
 *
 * Inputs: none
 * Output: none
 */
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
                          EditorHelpers::getFilename(QString::fromStdString(
                                                    map->lays_under[i].path)));
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
                          EditorHelpers::getFilename(QString::fromStdString(
                                                    map->lays_over[i].path)));
      }
    }
  }
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Returns the editor map, which contains all things and instances.
 *
 * Inputs: none
 * Output: EditorMap* - pointer to the editor map. NULL if none set.
 */
EditorMap* MapLayView::getEditorMap()
{
  return editor_map;
}

/*
 * Description: Sets the control editor map. This contains the things, sprites
 *              and all data relevant to display.
 *
 * Inputs: EditorMap* map - the map to use for control
 * Output: none
 */
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
