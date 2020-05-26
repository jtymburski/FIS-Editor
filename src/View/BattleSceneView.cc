/*******************************************************************************
 * Class Name: BattleSceneView
 * Date Created: January 23, 2016
 * Inheritance: QWidget
 * Description: The battle scene view and editor which handles the properly
 *              preview and edit capabilities to the scene.
 ******************************************************************************/
#include "View/BattleSceneView.h"
//#include <QDebug>

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function. Creates the battle scene view, which
 *              is the widget in the core set of game to create and edit a
 *              scene.
 *
 * Inputs: QWidget* parent - the parent widget
 */
BattleSceneView::BattleSceneView(QWidget* parent) : QWidget(parent)
{
  lay_edit_sprite = nullptr;
  lay_edit_struct = nullptr;
  scene_base = nullptr;

  createLayout();
  loadWorkingInfo();
}

/*
 * Description: Constructor function. Creates the battle scene view and sets
 *              the scene to be edited.
 *
 * Inputs: EditorBattleScene* scene - the scene to edit. If null, not able to be
 *         edited
 *         QWidget* parent - the parent widget
 */
BattleSceneView::BattleSceneView(EditorBattleScene* scene, QWidget* parent)
               : BattleSceneView(parent)
{
  setScene(scene);
}

/*
 * Description: Copy constructor. Calls the blank constructor and then copies
 *              the data from the source.
 *
 * Inputs: const BattleSceneView &source - the source object to copy
 */
BattleSceneView::BattleSceneView(const BattleSceneView &source)
               : BattleSceneView()
{
  copySelf(source);
}

/*
 * Description: Destructor function
 */
BattleSceneView::~BattleSceneView()
{
  setScene(nullptr);
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: Copies all data from source editor object to this editor
 *              object.
 *
 * Inputs: BattleSceneView &source - the source to copy from
 * Output: none
 */
void BattleSceneView::copySelf(const BattleSceneView &source)
{
  setScene(source.scene_base);
}

/*
 * Description: Creates the battle scene view layout with QT functional widgets.
 *
 * Inputs: none
 * Output: none
 */
void BattleSceneView::createLayout()
{
  /* Layout */
  QGridLayout* layout = new QGridLayout(this);
  //layout->setColumnStretch(5, 1);
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
  lbl_preview_img->setScaledContents(true);
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
  spin_lay_anim->setMaximum(99999);
  spin_lay_anim->setDisabled(true);
  connect(spin_lay_anim, SIGNAL(valueChanged(int)),
          this, SLOT(spinLayAnimChanged(int)));
  layout->addWidget(lbl_lay_anim, 7, 6);
  layout->addWidget(spin_lay_anim, 7, 7, 1, 2);

  /* Lay Over - Edit Velocity X */
  QLabel* lbl_lay_velx = new QLabel("Velocity X", this);
  spin_lay_velx = new QSpinBox(this);
  spin_lay_velx->setSuffix(" pixels/s");
  spin_lay_velx->setMinimum(-68000);
  spin_lay_velx->setMaximum(68000);
  spin_lay_velx->setDisabled(true);
  connect(spin_lay_velx, SIGNAL(valueChanged(int)),
          this, SLOT(spinLayVelXChanged(int)));
  layout->addWidget(lbl_lay_velx, 8, 6);
  layout->addWidget(spin_lay_velx, 8, 7, 1, 2);

  /* Lay Over - Edit Velocity Y */
  QLabel* lbl_lay_vely = new QLabel("Velocity Y", this);
  spin_lay_vely = new QSpinBox(this);
  spin_lay_vely->setSuffix(" pixels/s");
  spin_lay_vely->setMinimum(-68000);
  spin_lay_vely->setMaximum(68000);
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

/*
 * Description: Edits the layover selected index based on the selected index. If
 *              the index is out of range, sets the edit widgets to disabled.
 *
 * Inputs: QString category - the category of the index: underlay, etc
 *         int index - the index within the lay stack
 * Output: none
 */
void BattleSceneView::editLayOver(QString category, int index)
{
  bool valid = false;
  lay_edit_sprite = nullptr;
  lay_edit_struct = nullptr;

  /* Determine if valid and update the information */
  if(index >= 0)
  {
    /* Make modifications */
    if(category == "Underlays")
    {
      if(index < sprite_underlays.size())
        lay_edit_sprite = &sprite_underlays[index];
      lay_edit_struct = scene_curr.getUnderlayRef(index);
    }
    else if(category == "Midlays")
    {
      if(index < sprite_midlays.size())
        lay_edit_sprite = &sprite_midlays[index];
      lay_edit_struct = scene_curr.getMidlayRef(index);
    }
    else if(category == "Overlays")
    {
      if(index < sprite_overlays.size())
        lay_edit_sprite = &sprite_overlays[index];
      lay_edit_struct = scene_curr.getOverlayRef(index);
    }

    /* Update layover information */
    valid = (lay_edit_sprite != nullptr && lay_edit_struct != nullptr);
    if(valid)
    {
      if(lay_edit_struct->path.empty())
        lbl_lay_path_list->setText("Not Set");
      else
        lbl_lay_path_list->setText(QString::fromStdString(
                                                   lay_edit_struct->path));

      spin_lay_anim->setValue(lay_edit_struct->anim_time);

      float velx = (lay_edit_struct->velocity_x * 1000);
      if(velx > 0)
        velx += 0.1;
      else if(velx < 0)
        velx -= 0.1;
      spin_lay_velx->setValue(static_cast<int>(velx));

      float vely = (lay_edit_struct->velocity_y * 1000);
      if(vely > 0)
        vely += 0.1;
      else if(vely < 0)
        vely -= 0.1;
      spin_lay_vely->setValue(static_cast<int>(vely));
    }
  }

  /* Update enabled status of edit widgets */
  btn_lay_path->setEnabled(valid);
  spin_lay_anim->setEnabled(valid);
  spin_lay_velx->setEnabled(valid);
  spin_lay_vely->setEnabled(valid);
}

/*
 * Description: Loads all the UI elements with the contents from the working
 *              Battle Scene information.
 *
 * Inputs: none
 * Output: none
 */
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

  /* Lay Overs */
  changedLayClass(combo_layover->currentText());

  /* Final enabled/disabled status */
  setEnabled(scene_base != nullptr);
}

/*
 * Description: Function called everytime an edit change is made to the current
 *              set. This is what controls if a save or reset needs to occur
 *
 * Inputs: none
 * Output: none
 */
void BattleSceneView::setChanged()
{
  if(scene_base != nullptr)
    scene_base->setChanged(true);
}

/*
 * Description: Updates the battle scene preview. This is called if any path
 *              within the lays or the background is modified. Used to preview
 *              what the battle would look like
 *
 * Inputs: none
 * Output: none
 */
void BattleSceneView::updatePreview()
{
  int width = 608;//304;//1216;
  int height = 352;//176;//704;

  /* Clear pixmap and open painter */
  scene_preview = QPixmap(width, height);
  scene_preview.fill(Qt::black);
  QPainter painter(&scene_preview);

  /* Paint background */
  sprite_background.paint(&painter, 0, 0, width, height);

  /* Paint underlays */
  for(int i = 0; i < sprite_underlays.size(); i++)
    if(!sprite_underlays[i].isAllNull())
      sprite_underlays[i].paint(&painter, 0, 0, width, height);

  /* Paint midlays */
  for(int i = 0; i < sprite_midlays.size(); i++)
    if(!sprite_midlays[i].isAllNull())
      sprite_midlays[i].paint(&painter, 0, 0, width, height);

  /* Paint overlays */
  for(int i = 0; i < sprite_overlays.size(); i++)
    if(!sprite_overlays[i].isAllNull())
      sprite_overlays[i].paint(&painter, 0, 0, width, height);

  /* Update preview label */
  lbl_preview_img->setPixmap(scene_preview);
}

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/

/*
 * Description: Button trigger on background path select press. This will open
 *              the file dialog to select a new background frame
 *
 * Inputs: none
 * Output: none
 */
void BattleSceneView::btnBackground()
{
  /* Choose the background */
  QString last_path = EditorHelpers::getPreviousPath();
  QString path = QFileDialog::getOpenFileName(this,
                                              tr("Select a background frame"),
                                              last_path,
                                              tr("Image Files (*.png)"));
  if(path != "")
  {
    EditorHelpers::setPreviousPath(path);

    /* Parse the path and save */
    QString trim_path = EditorHelpers::trimPath(path);
    scene_curr.setBackground(trim_path);
    lbl_background_view->setText(trim_path);
    sprite_background.deleteAllFrames();
    sprite_background.addPath(path);

    /* Update the display */
    updatePreview();
    setChanged();
  }
}

/*
 * Description: Button trigger on layover add press. This will access the
 *              correct category (underlay, overlay, midlay) and adds it to the
 *              appropriate set.
 *
 * Inputs: none
 * Output: none
 */
void BattleSceneView::btnLayAdd()
{
  editLayOver("", -1);
  QString category_txt = combo_layover->currentText();

  /* Make modifications */
  if(category_txt == "Underlays")
  {
    scene_curr.setUnderlay(scene_curr.getUnderlaySize(),
                           Helpers::createBlankLayOver());
    sprite_underlays.push_back(EditorSprite());
  }
  else if(category_txt == "Midlays")
  {
    scene_curr.setMidlay(scene_curr.getMidlaySize(),
                         Helpers::createBlankLayOver());
    sprite_midlays.push_back(EditorSprite());
  }
  else if(category_txt == "Overlays")
  {
    scene_curr.setOverlay(scene_curr.getOverlaySize(),
                          Helpers::createBlankLayOver());
    sprite_overlays.push_back(EditorSprite());
  }

  /* Update list */
  changedLayClass(category_txt);
  list_layover->setCurrentRow(list_layover->count() - 1);
  updatePreview();
  setChanged();
}

/*
 * Description: Button trigger on layover delete press. This will access the
 *              correct category (underlay, overlay, midlay) and delete the
 *              selected index in the appropriate set.
 *
 * Inputs: none
 * Output: none
 */
void BattleSceneView::btnLayDelete()
{
  editLayOver("", -1);
  int curr_index = list_layover->currentRow();
  if(curr_index >= 0)
  {
    QString category_txt = combo_layover->currentText();

    /* Make modifications */
    if(category_txt == "Underlays")
    {
      scene_curr.unsetUnderlay(curr_index);
      sprite_underlays.removeAt(curr_index);
    }
    else if(category_txt == "Midlays")
    {
      scene_curr.unsetMidlay(curr_index);
      sprite_midlays.removeAt(curr_index);
    }
    else if(category_txt == "Overlays")
    {
      scene_curr.unsetOverlay(curr_index);
      sprite_overlays.removeAt(curr_index);
    }

    /* Update list */
    changedLayClass(category_txt);
    updatePreview();
    setChanged();
  }
}

/*
 * Description: Button trigger on layover down press. This will access the
 *              correct category (underlay, overlay, midlay) and move the
 *              selected index down by one spot in the appropriate set.
 *
 * Inputs: none
 * Output: none
 */
void BattleSceneView::btnLayDown()
{
  editLayOver("", -1);
  int curr_index = list_layover->currentRow();
  if(curr_index >= 0)
  {
    QString category_txt = combo_layover->currentText();

    /* Make modifications */
    if(category_txt == "Underlays")
    {
      LayOver old_lay = scene_curr.getUnderlay(curr_index);
      scene_curr.setUnderlay(curr_index, scene_curr.getUnderlay(curr_index+1));
      scene_curr.setUnderlay(curr_index+1, old_lay);

      EditorSprite old_sprite = sprite_underlays[curr_index];
      sprite_underlays[curr_index] = sprite_underlays[curr_index + 1];
      sprite_underlays[curr_index + 1] = old_sprite;
    }
    else if(category_txt == "Midlays")
    {
      LayOver old_lay = scene_curr.getMidlay(curr_index);
      scene_curr.setMidlay(curr_index, scene_curr.getMidlay(curr_index+1));
      scene_curr.setMidlay(curr_index+1, old_lay);

      EditorSprite old_sprite = sprite_midlays[curr_index];
      sprite_midlays[curr_index] = sprite_midlays[curr_index + 1];
      sprite_midlays[curr_index + 1] = old_sprite;
    }
    else if(category_txt == "Overlays")
    {
      LayOver old_lay = scene_curr.getOverlay(curr_index);
      scene_curr.setOverlay(curr_index, scene_curr.getOverlay(curr_index+1));
      scene_curr.setOverlay(curr_index+1, old_lay);

      EditorSprite old_sprite = sprite_overlays[curr_index];
      sprite_overlays[curr_index] = sprite_overlays[curr_index + 1];
      sprite_overlays[curr_index + 1] = old_sprite;
    }

    /* Update list */
    changedLayClass(category_txt);
    list_layover->setCurrentRow(curr_index + 1);
    updatePreview();
    setChanged();
  }
}

/*
 * Description: Button trigger on layover edit press. This will access the
 *              correct category (underlay, overlay, midlay) and edit the
 *              selected index in the appropriate set on the lower half.
 *
 * Inputs: none
 * Output: none
 */
void BattleSceneView::btnLayEdit()
{
  editLayOver(combo_layover->currentText(), list_layover->currentRow());
}

/*
 * Description: Button trigger on layover edit path select button. This will
 *              select a new path for the selected edit layover if one exists.
 *              This call can handle sprite selection (instead of just frame)
 *
 * Inputs: none
 * Output: none
 */
void BattleSceneView::btnLayPath()
{
  if(lay_edit_struct != nullptr && lay_edit_sprite != nullptr)
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
      lay_edit_struct->path = EditorHelpers::trimPath(path).toStdString();
      lay_edit_sprite->deleteAllFrames();
      lay_edit_sprite->addPath(path);

      /* Update the display */
      lbl_lay_path_list->setText(QString::fromStdString(lay_edit_struct->path));
      changedLayClass(combo_layover->currentText());
      updatePreview();
      setChanged();
    }
  }
}

/*
 * Description: Button trigger on layover up press. This will access the
 *              correct category (underlay, overlay, midlay) and move the
 *              selected index up by one spot in the appropriate set.
 *
 * Inputs: none
 * Output: none
 */
void BattleSceneView::btnLayUp()
{
  editLayOver("", -1);
  int curr_index = list_layover->currentRow();
  if(curr_index >= 1)
  {
    QString category_txt = combo_layover->currentText();

    /* Make modifications */
    if(category_txt == "Underlays")
    {
      LayOver old_lay = scene_curr.getUnderlay(curr_index);
      scene_curr.setUnderlay(curr_index, scene_curr.getUnderlay(curr_index-1));
      scene_curr.setUnderlay(curr_index-1, old_lay);

      EditorSprite old_sprite = sprite_underlays[curr_index];
      sprite_underlays[curr_index] = sprite_underlays[curr_index - 1];
      sprite_underlays[curr_index - 1] = old_sprite;
    }
    else if(category_txt == "Midlays")
    {
      LayOver old_lay = scene_curr.getMidlay(curr_index);
      scene_curr.setMidlay(curr_index, scene_curr.getMidlay(curr_index-1));
      scene_curr.setMidlay(curr_index-1, old_lay);

      EditorSprite old_sprite = sprite_midlays[curr_index];
      sprite_midlays[curr_index] = sprite_midlays[curr_index - 1];
      sprite_midlays[curr_index - 1] = old_sprite;
    }
    else if(category_txt == "Overlays")
    {
      LayOver old_lay = scene_curr.getOverlay(curr_index);
      scene_curr.setOverlay(curr_index, scene_curr.getOverlay(curr_index-1));
      scene_curr.setOverlay(curr_index-1, old_lay);

      EditorSprite old_sprite = sprite_overlays[curr_index];
      sprite_overlays[curr_index] = sprite_overlays[curr_index - 1];
      sprite_overlays[curr_index - 1] = old_sprite;
    }

    /* Update list */
    changedLayClass(category_txt);
    list_layover->setCurrentRow(curr_index - 1);
    updatePreview();
    setChanged();
  }
}

/*
 * Description: Slot triggered by reset button. Resets data in editing battle
 *              scene.
 *
 * Inputs: none
 * Output: none
 */
void BattleSceneView::btnReset()
{
  if(scene_base != nullptr && scene_base->isChanged())
  {
    setScene(scene_base);
  }
}

/*
 * Description: Slot triggered by save button. Saves data in editing battle
 *              scene.
 *
 * Inputs: none
 * Output: none
 */
void BattleSceneView::btnSave()
{
  if(scene_base != nullptr && scene_base->isChanged())
  {
    bool changed_name = (scene_base->getName() != scene_curr.getName());
    *scene_base = scene_curr;
    scene_base->setChanged(false);
    if(changed_name)
      emit nameChange(scene_base->getNameList());
  }
}

/*
 * Description: Slot triggered when the layover combo box drop down selection
 *              is changed. This updates which layovers are being viewed.
 *
 * Inputs: QString category - the new selected category
 * Output: none
 */
void BattleSceneView::changedLayClass(const QString& category)
{
  editLayOver("", -1);
  int curr_index = list_layover->currentRow();
  list_layover->clear();

  /* Update list */
  if(category == "Underlays")
  {
    QVector<LayOver> underlays = scene_curr.getUnderlays();
    for(int i = 0; i < underlays.size(); i++)
    {
      if(underlays[i].path.empty())
        list_layover->addItem(QString::number(i) + ": Not Set");
      else
        list_layover->addItem(QString::number(i) + ": " +
                              QString::fromStdString(underlays[i].path));
    }
  }
  else if(category == "Midlays")
  {
    QVector<LayOver> midlays = scene_curr.getMidlays();
    for(int i = 0; i < midlays.size(); i++)
    {
      if(midlays[i].path.empty())
        list_layover->addItem(QString::number(i) + ": Not Set");
      else
        list_layover->addItem(QString::number(i) + ": " +
                              QString::fromStdString(midlays[i].path));
    }
  }
  else if(category == "Overlays")
  {
    QVector<LayOver> overlays = scene_curr.getOverlays();
    for(int i = 0; i < overlays.size(); i++)
    {
      if(overlays[i].path.empty())
        list_layover->addItem(QString::number(i) + ": Not Set");
      else
        list_layover->addItem(QString::number(i) + ": " +
                              QString::fromStdString(overlays[i].path));
    }
  }

  /* Re-select index, if applicable */
  if(curr_index >= 0 && list_layover->count() > 0)
  {
    if(curr_index < list_layover->count())
      list_layover->setCurrentRow(curr_index);
    else
      list_layover->setCurrentRow(list_layover->count() - 1);
  }
  else
  {
    listLayOverChanged(-1);
  }
}

/*
 * Description: Slot triggered when the music selection combo box is changed.
 *              Sets the new music ID
 *
 * Inputs: QString text - the new selected music index
 * Output: none
 */
void BattleSceneView::changedMusic(const QString& text)
{
  int id = -1;
  QStringList item_split = text.split(':');
  if(item_split.size() >= 2)
    id = item_split.front().toInt();
  scene_curr.setMusicID(id);
  setChanged();
}

/*
 * Description: Slot triggered when the name is changed in the line edit widget.
 *
 * Inputs: QString name - the new changed name
 * Output: none
 */
void BattleSceneView::changedName(const QString& name)
{
  scene_curr.setName(name);
  setChanged();
}

/*
 * Description: Slot triggered when the selected lay over in the list is
 *              modified. This changes which lay over is being edited
 *
 * Inputs: int index - the selected row in the list
 * Output: none
 */
void BattleSceneView::listLayOverChanged(int index)
{
  editLayOver(combo_layover->currentText(), index);

  btn_lay_edit->setEnabled(index >= 0 && list_layover->count() > 0);
  btn_lay_del->setEnabled(index >= 0 && list_layover->count() > 0);
  btn_lay_up->setEnabled(index >= 1);
  btn_lay_down->setEnabled(index >= 0 && index < (list_layover->count() - 1));
}

/*
 * Description: Slot triggered when the selected lay over is double clicked.
 *              This calls edit on the selected index
 *
 * Inputs: QListWidgetItem* - not used
 * Output: none
 */
void BattleSceneView::listLayOverDClicked(QListWidgetItem*)
{
  btnLayEdit();
}

/*
 * Description: Slot triggered when the animation delay change is modified.
 *              This function is only valid if an edit is active
 *
 * Inputs: int value - the new animation time value, in ms
 * Output: none
 */
void BattleSceneView::spinLayAnimChanged(int value)
{
  if(lay_edit_struct != nullptr)
  {
    lay_edit_struct->anim_time = value;
    setChanged();
  }
}

/*
 * Description: Slot triggered when the layover velocity x value is changed.
 *              This function is only valid if an edit is active
 *
 * Inputs: int value - the new velocity x value, in tiles/second
 * Output: none
 */
void BattleSceneView::spinLayVelXChanged(int value)
{
  if(lay_edit_struct != nullptr)
  {
    lay_edit_struct->velocity_x = value / 1000.0;
    setChanged();
  }
}

/*
 * Description: Slot triggered when the layover velocity y value is changed.
 *              This function is only valid if an edit is active
 *
 * Inputs: int value - the new velocity y value, in tiles/second
 * Output: none
 */
void BattleSceneView::spinLayVelYChanged(int value)
{
  if(lay_edit_struct != nullptr)
  {
    lay_edit_struct->velocity_y = value / 1000.0;
    setChanged();
  }
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Returns the list of music in the database
 *
 * Inputs: none
 * Output: QList<QString> - the list of strings in the format ("XXX: Sound")
 */
QList<QString> BattleSceneView::getListMusic()
{
  return music_list;
}

/*
 * Description: Returns the scene being edited by the view
 *
 * Inputs: none
 * Output: EditorBattleScene* - the scene structure
 */
EditorBattleScene* BattleSceneView::getScene()
{
  return scene_base;
}

/*
 * Description: Returns if the battle scene information has been edited but not
 *              saved.
 *
 * Inputs: none
 * Output: bool - true if it has been changed
 */
bool BattleSceneView::isChanged()
{
  if(scene_base != nullptr)
    return scene_base->isChanged();
  return false;
}

/*
 * Description: Resets the working information back to the base (last saved)
 *              data. This will also automatically update all UI widgets with
 *              the new data.
 *
 * Inputs: none
 * Output: none
 */
void BattleSceneView::resetWorking()
{
  btnReset();
}

/*
 * Description: Saves the working information back to the base (last saved)
 *              data.
 *
 * Inputs: none
 * Output: none
 */
void BattleSceneView::saveWorking()
{
  btnSave();
}

/*
 * Description: Sets the battle scene information to be edited. If the edit
 *              widget is null, unsets edit portion.
 *
 * Inputs: EditorBattleScene* scene - the new edit data
 * Output: none
 */
void BattleSceneView::setScene(EditorBattleScene* scene)
{
  /* Bring in the new information and load into the data widgets */
  scene_base = scene;
  if(scene_base != nullptr)
  {
    scene_base->setChanged(false);
    scene_curr = *scene_base;
  }
  loadWorkingInfo();

  /* If the scene base is valid, update the sprites */
  if(scene_base != nullptr)
  {
    QString project_dir = EditorHelpers::getProjectDir();

    /* Background */
    sprite_background.deleteAllFrames();
    if(!scene_curr.getBackground().isEmpty())
      sprite_background.addPath(project_dir + QDir::separator() +
                                scene_curr.getBackground());

    /* Underlays */
    sprite_underlays.clear();
    QVector<LayOver> underlays = scene_curr.getUnderlays();
    for(int i = 0; i < underlays.size(); i++)
    {
      sprite_underlays.push_back(EditorSprite());
      if(!underlays[i].path.empty())
        sprite_underlays.last().addPath(project_dir + QDir::separator() +
                                    QString::fromStdString(underlays[i].path));
    }

    /* Midlays */
    sprite_midlays.clear();
    QVector<LayOver> midlays = scene_curr.getMidlays();
    for(int i = 0; i < midlays.size(); i++)
    {
      sprite_midlays.push_back(EditorSprite());
      if(!midlays[i].path.empty())
        sprite_midlays.last().addPath(project_dir + QDir::separator() +
                                    QString::fromStdString(midlays[i].path));
    }

    /* Overlays */
    sprite_overlays.clear();
    QVector<LayOver> overlays = scene_curr.getOverlays();
    for(int i = 0; i < overlays.size(); i++)
    {
      sprite_overlays.push_back(EditorSprite());
      if(!overlays[i].path.empty())
        sprite_overlays.last().addPath(project_dir + QDir::separator() +
                                    QString::fromStdString(overlays[i].path));
    }

    /* Update image */
    updatePreview();
  }
}

/*
 * Description: Updates list of music available for selection in the view
 *
 * Inputs: QList<QString> list - the list of music items
 * Output: none
 */
void BattleSceneView::updateListMusic(QList<QString> list)
{
  music_list = list;
  loadWorkingInfo();
}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/

/*
 * Description: Copy operator construction. This is called when the variable
 *              already exists and equal operator used with another
 *              BattleSceneView.
 *
 * Inputs: const BattleSceneView &source - the source class constructor
 * Output: BattleSceneView& - pointer to the copied class
 */
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
