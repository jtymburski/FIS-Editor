/*******************************************************************************
 * Class Name: Application
 * Date Created: November 11, 2013
 * Inheritance: QMainWindow
 * Description: The Main Application that handles all the displays for the
 *              entire editor. This includes the map, all applicable toolbars.
 ******************************************************************************/
#include "Application.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/*
 * Description: Constructor function
 *
 * Input: Parent Widget
 */
Application::Application(QWidget* parent)
           : QMainWindow(parent)
{
  /* Gets the users name in windows only */
  file_name = "";
  username = getenv("USERNAME");

  game_db_dock = new QDockWidget(this);
  game_db_dock->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  game_database = new GameDatabase(this);
  game_db_dock->setWindowIcon(QIcon(":/images/fbs_icon.ico"));
  game_db_dock->setWidget(new QWidget(this));
  game_db_dock->setWidget(game_database);
  addDockWidget(Qt::LeftDockWidgetArea, game_db_dock);
  game_db_dock->setFeatures(QDockWidget::NoDockWidgetFeatures);
  game_db_dock->setAllowedAreas(Qt::LeftDockWidgetArea);
  game_db_dock->setTitleBarWidget(new QWidget(this));

  connect(game_database, SIGNAL(changeMode(EditorEnumDb::ViewMode)),
          this, SLOT(setView(EditorEnumDb::ViewMode)));
  connect(game_database, SIGNAL(changeMap(EditorMap*)),
          this, SLOT(setMap(EditorMap*)));
  connect(game_database, SIGNAL(changePerson(EditorPerson*)),
          this, SLOT(setPerson(EditorPerson*)));
  connect(game_database, SIGNAL(changeParty(EditorParty*)),
          this, SLOT(setParty(EditorParty*)));
  connect(game_database, SIGNAL(changeItem(EditorItem*)),
          this, SLOT(setItem(EditorItem*)));
  connect(game_database, SIGNAL(changeAction(EditorAction*)),
          this, SLOT(setAction(EditorAction*)));
  connect(game_database, SIGNAL(changeRace(EditorCategory*)),
          this, SLOT(setRace(EditorCategory*)));
  connect(game_database, SIGNAL(changeBattleclass(EditorCategory*)),
          this, SLOT(setBattleClass(EditorCategory*)));
  connect(game_database, SIGNAL(changeBattleScene(EditorBattleScene*,bool)),
          this, SLOT(setBattleScene(EditorBattleScene*,bool)));
  connect(game_database, SIGNAL(changeSkillset(EditorSkillset*))
          ,this, SLOT(setSkillset(EditorSkillset*)));
  connect(game_database, SIGNAL(changeSkill(EditorSkill*)),
          this, SLOT(setSkill(EditorSkill*)));
//  connect(game_database, SIGNAL(changeEquipment
//                                (EditorEquipment*)),
//          this, SLOT(setEquipment(EditorEquipment*)));
//  connect(game_database, SIGNAL(changeBubby(EditorBubby*)),
//          this, SLOT(setBubby(EditorBubby*)));

  game_view = new GameView(this);
  setCentralWidget(game_view);
  game_view->setViewSounds(game_database->getSoundDatabase());
  //game_view->setGeometry(QApplication::desktop()->availableGeometry());

  /* Connections between game view and game database */
  connect(game_view,SIGNAL(nameChange(QString)),game_database,
          SLOT(updateBottomListName(QString)));

  connect(game_view, SIGNAL(updateBattleSceneObjects()),
          game_database, SLOT(updateBattleSceneObjects()));
  connect(game_view, SIGNAL(updateEventObjects()),
          game_database, SLOT(updateEventObjects()));
  connect(game_view, SIGNAL(updateMusicObjects()),
          game_database, SLOT(updateMusicObjects()));
  connect(game_view, SIGNAL(updateSoundObjects()),
          game_database, SLOT(updateSoundObjects()));

  connect(game_database,
          SIGNAL(updatedBattleScenes(QList<QPair<int,QString>>)),
          game_view, SIGNAL(updatedBattleScenes(QList<QPair<int,QString> >)));
  connect(game_database, SIGNAL(updatedItems(QList<QString>)),
          game_view, SIGNAL(updatedItems(QList<QString>)));
  connect(game_database, SIGNAL(updatedMaps(QList<QString>)),
          game_view, SIGNAL(updatedMaps(QList<QString>)));
  connect(game_database, SIGNAL(updatedMusic(QList<QString>)),
          game_view, SLOT(updatedMusic(QList<QString>)));
  connect(game_database, SIGNAL(updatedParties(QList<QString>)),
          game_view, SIGNAL(updatedParties(QList<QString>)));
  connect(game_database, SIGNAL(updatedSounds(QList<QString>)),
          game_view, SIGNAL(updatedSounds(QList<QString>)));
  game_database->updateMusicObjects();

  connect(game_view->getMapView(), SIGNAL(disableControl(bool)),
          this, SLOT(disableControl(bool)));
  connect(game_view->getMapView(), SIGNAL(layerChanged(EditorEnumDb::Layer)),
          this, SLOT(layerChanged(EditorEnumDb::Layer)));

  /* Calls all setup functions */
  setWindowTitle("Univursa Designer");
  setWindowIcon(QIcon(":/images/fbs_icon.ico"));
  setupTopMenu();
  //setGeometry(16,32,QApplication::desktop()->availableGeometry().width()-32,
   //           QApplication::desktop()->availableGeometry().height()-64);
  setMinimumSize(1280,720);
  showMaximized();
  setCursorBasic(true);

  /* Configure the run process */
  run_process.setWorkingDirectory(EditorHelpers::getProjectDir());
  connect(&run_process, SIGNAL(finished(int)), this, SLOT(playFinished(int)));

  setStyleSheet("QMainWindow::separator { background: rgb(153, 153, 153); \
                                          width: 1px; height: 1px; }");
}

/*
 * Description: Destructor function
 */
Application::~Application()
{
  /* Clean up views before mass deletion */
  game_view->getMapView()->getMapEditorView()->setMapEditor(NULL);
  game_database->deleteAll();
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* Create progress dialog */
QProgressDialog* Application::createProgressDialog(int total_count,
                                                   QString title_text,
                                                   QString label_text)
{
  /* Create dialog */
  QProgressDialog* progress_dialog = new QProgressDialog("", "Cancel", 0,
                                                         total_count, this);
  progress_dialog->setWindowTitle(title_text);
  progress_dialog->setLabelText(label_text);
  //progress_dialog->setCancelButton(nullptr);
  progress_dialog->setWindowModality(Qt::WindowModal);
  //progress_dialog->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint |
  //                             Qt::WindowTitleHint| Qt::WindowSystemMenuHint);

  /* Disable cancel button */
  QList<QPushButton*> list = progress_dialog->findChildren<QPushButton*>();
  if(list.size() > 0)
    list.at(0)->setDisabled(true);

  /* Set settings and finally value */
  progress_dialog->setMinimumDuration(0);
  progress_dialog->setValue(0);
  //progress_dialog->show();

  return progress_dialog;
}

/* Export application to run in Univursa */
void Application::exportGame(QString filename)
{
  /* Start file write */
  FileHandler fh(filename.toStdString(), true, true);
  fh.start();

  /* Get the count */
  int count = game_database->getSaveCount();

  /* Create the progress dialog */
  QProgressDialog* progress_dialog = createProgressDialog(count, "Exporting",
                                                          "Exporting Game...");

  /* Write the data */
  game_database->save(&fh, progress_dialog, true);
  progress_dialog->setValue(count);

  /* Finish the file write */
  fh.stop();
  delete progress_dialog;
}

/* Load application */
void Application::loadApp(QString filename)
{
  if(newGame())
  {
    file_name = filename;
    FileHandler fh(file_name.toStdString(), false, true);
    bool success = fh.start();

    /* Get count */
    int count = fh.getCount();

    /* Create progress dialog */
    QProgressDialog* progress_dialog = createProgressDialog(count, "Loading",
                                                            "Loading Game...");

    /* Load the info into the game database */
    if(success)
      game_database->load(&fh, progress_dialog);
    progress_dialog->setValue(count);

    /* Finish the file read */
    fh.stop();
    delete progress_dialog;
  }
}

/* Save application */
void Application::saveApp()
{
  /* Start file write */
  FileHandler fh(file_name.toStdString(), true, true);
  fh.start();

  /* Get the count */
  int count = game_database->getSaveCount();

  /* Create the progress dialog */
  QProgressDialog* progress_dialog = createProgressDialog(count, "Saving",
                                                          "Saving Game...");

  /* Write the data */
  game_database->save(&fh, progress_dialog);
  progress_dialog->setValue(count);

  /* Finish the file write */
  fh.stop();
  delete progress_dialog;
}

/*
 * Description: Sets up the Top Menu
 *
 * Inputs: none
 */
void Application::setupTopMenu()
{
  /* Sets up the File menu actions */
  QAction* action_show_menu = new QAction("&Menu", this);
  action_show_menu->setIcon(QIcon(":/images/icons/32_database.png"));
  action_show_menu->setCheckable(true);
  action_show_menu->setChecked(true);
  action_new = new QAction("&New",this);
  action_new->setIcon(QIcon(":/images/icons/32_new.png"));
  action_new->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
  action_load = new QAction("&Load",this);
  action_load->setIcon(QIcon(":/images/icons/32_load.png"));
  action_recent = new QAction("&Recent Files",this);
  action_save = new QAction("&Save",this);
  action_save->setIcon(QIcon(":/images/icons/32_save.png"));
  action_save->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
  action_saveas = new QAction("&Save As",this);
  action_saveas->setIcon(QIcon(":/images/icons/32_save_as.png"));
  action_export = new QAction("&Export to Game", this);
  action_export->setIcon(QIcon(":/images/icons/32_export.png"));
  action_export->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));
  QAction* quit_action = new QAction("&Quit",this);

  /* Sets up file menu itself */
  QMenu* file_menu = menuBar()->addMenu("&File");
  file_menu->addAction(action_new);
  file_menu->addAction(action_load);
  file_menu->addAction(action_recent);
  file_menu->addSeparator();
  file_menu->addAction(action_save);
  file_menu->addAction(action_saveas);
  file_menu->addAction(action_export);
  file_menu->addSeparator();
  file_menu->addAction(quit_action);

  /* Connects File menu actions to slots */
  connect(action_show_menu, SIGNAL(triggered()), this, SLOT(showDatabase()));
  connect(quit_action,SIGNAL(triggered()), this, SLOT(close()));

  /* Sets up Edit menu actions*/
  QAction* undo_action = new QAction("&Undo",this);
  undo_action->setIcon(QIcon(":/images/icons/32_undo.png"));
  undo_action->setDisabled(true);
  QAction* redo_action = new QAction("&Redo",this);
  redo_action->setIcon(QIcon(":/images/icons/32_redo.png"));
  redo_action->setDisabled(true);
  QAction* cut_action = new QAction("&Cut",this);
  cut_action->setIcon(QIcon(":/images/icons/32_cut.png"));
  cut_action->setDisabled(true);
  QAction* copy_action = new QAction("&Copy",this);
  copy_action->setIcon(QIcon(":/images/icons/32_copy.png"));
  copy_action->setDisabled(true);
  QAction* paste_action = new QAction("&Paste",this);
  paste_action->setIcon(QIcon(":/images/icons/32_paste.png"));
  paste_action->setDisabled(true);
  QAction* findreplace_action = new QAction("&Find/Replace",this);
  findreplace_action->setDisabled(true);

  /* Sets up Edit menu itself */
  menu_edit = menuBar()->addMenu("&Edit");
  menu_edit->addAction(undo_action);
  menu_edit->addAction(redo_action);
  menu_edit->addSeparator();
  menu_edit->addAction(cut_action);
  menu_edit->addAction(copy_action);
  menu_edit->addAction(paste_action);
  menu_edit->addSeparator();
  menu_edit->addAction(findreplace_action);

  QActionGroup* cursor_group = new QActionGroup(this);
  cursor_group->setExclusive(true);

  action_basic = new QAction("&Basic",cursor_group);
  action_basic->setCheckable(true);
  action_basic->setChecked(true);
  action_basic->setIcon(QIcon(":/images/icons/32_pencil.png"));
  QAction* action_eraser = new QAction("&Eraser",cursor_group);
  action_eraser->setCheckable(true);
  action_eraser->setIcon(QIcon(":/images/icons/32_eraser.png"));
  action_move = new QAction("&Move", cursor_group);
  action_move->setDisabled(true);
  action_move->setCheckable(true);
  action_move->setIcon(QIcon(":/images/icons/32_move.png"));
  action_blockplace = new QAction("&Block Place",cursor_group);
  action_blockplace->setCheckable(true);
  action_blockplace->setIcon(QIcon(":/images/icons/32_rect_fill.png"));
  action_fill = new QAction("&Fill",cursor_group);
  action_fill->setCheckable(true);
  action_fill->setIcon(QIcon(":/images/icons/32_fill.png"));
  action_passall = new QAction("&Passability All", cursor_group);
  action_passall->setCheckable(true);
  action_passall->setIcon(QIcon(":/images/icons/32_passA.png"));
  action_passN = new QAction("&Passability North", cursor_group);
  action_passN->setCheckable(true);
  action_passN->setIcon(QIcon(":/images/icons/32_passN.png"));
  action_passE = new QAction("&Passability East", cursor_group);
  action_passE->setCheckable(true);
  action_passE->setIcon(QIcon(":/images/icons/32_passE.png"));
  action_passS = new QAction("&Passability South", cursor_group);
  action_passS->setCheckable(true);
  action_passS->setIcon(QIcon(":/images/icons/32_passS.png"));
  action_passW = new QAction("&Passability West", cursor_group);
  action_passW->setCheckable(true);
  action_passW->setIcon(QIcon(":/images/icons/32_passW.png"));
  menu_cursor = menuBar()->addMenu("&Cursor Modes");

  /* Sets up the menu toolbars */
  QToolBar* db_menu = new QToolBar("Database View", this);
  db_menu->addAction(action_show_menu);
  addToolBar(Qt::TopToolBarArea, db_menu);
  db_menu->setFloatable(false);
  db_menu->setMovable(false);
  bar_menu = new QToolBar("Menus",this);
  bar_menu->addAction(action_new);
  bar_menu->addAction(action_load);
  bar_menu->addAction(action_save);
  bar_menu->addAction(action_saveas);
  bar_menu->addAction(action_export);
  bar_menu->addSeparator();
  bar_menu->addAction(undo_action);
  bar_menu->addAction(redo_action);
  bar_menu->addAction(cut_action);
  bar_menu->addAction(copy_action);
  bar_menu->addAction(paste_action);
  addToolBar(Qt::TopToolBarArea,bar_menu);
  bar_menu->setFloatable(false);
  bar_menu->setMovable(false);
  connect(action_new, SIGNAL(triggered()), this, SLOT(newGame()));
  connect(action_load, SIGNAL(triggered()), this, SLOT(load()));
  connect(action_save, SIGNAL(triggered()), this, SLOT(save()));
  connect(action_saveas, SIGNAL(triggered()), this, SLOT(saveAs()));
  connect(action_export, SIGNAL(triggered()), this, SLOT(exportTo()));

  /* Sets up the brushes toolbar */
  bar_brush = new QToolBar("Brushes", this);
  bar_brush->addAction(action_basic);
  bar_brush->addAction(action_eraser);
  bar_brush->addAction(action_move);
  bar_brush->addAction(action_blockplace);
  bar_brush->addAction(action_fill);
  bar_brush->addAction(action_passall);
  bar_brush->addAction(action_passN);
  bar_brush->addAction(action_passE);
  bar_brush->addAction(action_passS);
  bar_brush->addAction(action_passW);
  addToolBar(Qt::TopToolBarArea,bar_brush);
  menu_cursor->addAction(action_basic);
  menu_cursor->addAction(action_eraser);
  menu_cursor->addAction(action_move);
  menu_cursor->addAction(action_blockplace);
  menu_cursor->addAction(action_fill);
  menu_cursor->addAction(action_passall);
  menu_cursor->addAction(action_passN);
  menu_cursor->addAction(action_passE);
  menu_cursor->addAction(action_passS);
  menu_cursor->addAction(action_passW);
  bar_brush->setFloatable(false);
  bar_brush->setMovable(false);
  connect(action_basic,SIGNAL(toggled(bool)),this,SLOT(setCursorBasic(bool)));
  connect(action_eraser,SIGNAL(toggled(bool)),this,SLOT(setCursorEraser(bool)));
  connect(action_move, SIGNAL(toggled(bool)), this, SLOT(setCursorMove(bool)));
  connect(action_blockplace,SIGNAL(toggled(bool)),
          this,SLOT(setCursorBlock(bool)));
  connect(action_fill,SIGNAL(toggled(bool)),this,SLOT(setCursorFill(bool)));
  connect(action_passall, SIGNAL(toggled(bool)),
          this, SLOT(setPassAllCursor(bool)));
  connect(action_passN, SIGNAL(toggled(bool)),
          this, SLOT(setPassNorthCursor(bool)));
  connect(action_passE, SIGNAL(toggled(bool)),
          this, SLOT(setPassEastCursor(bool)));
  connect(action_passS, SIGNAL(toggled(bool)),
          this, SLOT(setPassSouthCursor(bool)));
  connect(action_passW, SIGNAL(toggled(bool)),
          this, SLOT(setPassWestCursor(bool)));

  /* Set up the map control bar */
  action_zoom_in = new QAction("&Zoom In", this);
  action_zoom_in->setIcon(QIcon(":/images/icons/32_zoom_in.png"));
  action_zoom_out = new QAction("&Zoom Out", this);
  action_zoom_out->setIcon(QIcon(":/images/icons/32_zoom_out.png"));
  action_play = new QAction("&Test Map", this);
  action_play->setIcon(QIcon(":/images/icons/32_play.png"));
  action_play->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));
  bar_map_ctrl = new QToolBar("Map Control", this);
  bar_map_ctrl->addAction(action_zoom_in);
  bar_map_ctrl->addAction(action_zoom_out);
  bar_map_ctrl->addAction(action_play);
  bar_map_ctrl->setFloatable(false);
  bar_map_ctrl->setMovable(false);
  addToolBar(Qt::TopToolBarArea, bar_map_ctrl);
  connect(action_zoom_in, SIGNAL(triggered()), this, SLOT(zoomInMap()));
  connect(action_zoom_out, SIGNAL(triggered()), this, SLOT(zoomOutMap()));
  connect(action_play, SIGNAL(triggered()), this, SLOT(play()));
}

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/

/* Disables control for when in editing view with map */
void Application::disableControl(bool disabled)
{
  game_db_dock->setDisabled(disabled);

  bar_brush->setDisabled(disabled);
  action_export->setDisabled(disabled);
  action_load->setDisabled(disabled);
  action_new->setDisabled(disabled);
  action_play->setDisabled(disabled);
  action_recent->setDisabled(disabled);
  action_save->setDisabled(disabled);
  action_saveas->setDisabled(disabled);
  bar_menu->setDisabled(disabled);

  menu_cursor->setDisabled(disabled);
  menu_edit->setDisabled(disabled);
}

/* Export action */
void Application::exportTo()
{
  QString file = QFileDialog::getSaveFileName(this, "Export to Game File",
                        EditorHelpers::getSpriteDir() + "/../../Editor/exports",
                        tr("Univursa Game Saves (*.ugv)"));
  if(file != "")
  {
    if(!file.endsWith(".ugv"))
      file += ".ugv";
    exportGame(file);
  }
}

/* Slot for layer changing */
void Application::layerChanged(EditorEnumDb::Layer layer)
{
  /* Enabling / Disabling paint and fill pen */
  if(layer == EditorEnumDb::BASE || layer == EditorEnumDb::ENHANCER ||
     layer == EditorEnumDb::LOWER1 || layer == EditorEnumDb::LOWER2 ||
     layer == EditorEnumDb::LOWER3 || layer == EditorEnumDb::LOWER4 ||
     layer == EditorEnumDb::LOWER5 || layer == EditorEnumDb::UPPER1 ||
     layer == EditorEnumDb::UPPER2 || layer == EditorEnumDb::UPPER3 ||
     layer == EditorEnumDb::UPPER4 || layer == EditorEnumDb::UPPER5)
  {
    action_blockplace->setEnabled(true);
    action_fill->setEnabled(true);

    action_move->setDisabled(true);
    if(action_move->isChecked())
      action_basic->setChecked(true);
  }
  else
  {
    action_blockplace->setDisabled(true);
    action_fill->setDisabled(true);
    action_move->setEnabled(true);

    /* If either selected, change to place pen */
    if(action_blockplace->isChecked() || action_fill->isChecked())
    {
      action_basic->setChecked(true);
    }
  }

  /* Enabling / Disabling passability pens */
  if(layer == EditorEnumDb::BASE || layer == EditorEnumDb::LOWER1 ||
     layer == EditorEnumDb::LOWER2 || layer == EditorEnumDb::LOWER3 ||
     layer == EditorEnumDb::LOWER4 || layer == EditorEnumDb::LOWER5)
  {
    action_passall->setEnabled(true);
    action_passE->setEnabled(true);
    action_passN->setEnabled(true);
    action_passS->setEnabled(true);
    action_passW->setEnabled(true);
  }
  else
  {
    action_passall->setDisabled(true);
    action_passE->setDisabled(true);
    action_passN->setDisabled(true);
    action_passS->setDisabled(true);
    action_passW->setDisabled(true);

    /* If either selected, change to place pen */
    if(action_passall->isChecked() || action_passE->isChecked() ||
       action_passN->isChecked() || action_passS->isChecked() ||
       action_passW->isChecked())
    {
      action_basic->setChecked(true);
    }
  }

  /* Select place pen by default - removed at Jeff's request 2017-02-25 */
  //if(!action_basic->isChecked())
  //  action_basic->setChecked(true);
}

/* Load action */
void Application::load()
{
  QString file = QFileDialog::getOpenFileName(this, "Load Game Editor File",
                          EditorHelpers::getSpriteDir() + "/../../Editor/saves",
                          tr("Univursa Saves (*.usv)"));
  if(file != "")
    loadApp(file);
}

/* New game */
bool Application::newGame()
{
  QMessageBox::StandardButton reply = QMessageBox::question(this, "New?",
                 "Are you sure you want to lose all changes for a new slate?",
                 QMessageBox::Yes | QMessageBox::No);
  if(reply == QMessageBox::Yes)
  {
    game_database->deleteAll();
    game_database->createStartObjects();
    file_name = "";
    return true;
  }
  return false;
}

/* Plays the current sub-map in the game */
void Application::play()
{
  /* If no process is running, start it */
  if(run_process.state() != QProcess::Running &&
     game_view->getMapView()->getCurrentSubMap() >= 0)
  {
    /* Choose the file name and start */
    QString save_file = EditorHelpers::getProjectDir() +
                        "/../Editor/exports/xXx_TMP_xXx.utv";
    QString play_file = "../Editor/exports/xXx_TMP_xXx.utv";
    FileHandler fh(save_file.toStdString(), true, true);
    fh.start();

    /* Get the count */
    int count = game_database->getSaveCount();

    /* Create the progress dialog */
    QProgressDialog* progress_dialog = createProgressDialog(count, "Exporting",
                                                           "Exporting Game...");
    /* Save the current sub-map */
    game_database->save(&fh, progress_dialog, true);
        // Note: commented out to export all subs - change by re-appending
        //, true, game_view->getMapView()->getCurrentSubMap());
    progress_dialog->setValue(count);

    /* Finish the write */
    fh.stop();
    delete progress_dialog;

    /* Determine which execute program to use */
    QString exec_program = "./Univursa";
#ifdef WIN32
    exec_program = "\"" + EditorHelpers::getProjectDir() + "/Univursa.exe\"";
#endif

    /* Execute the program */
    QStringList arg_list;
    arg_list.push_back(play_file);
    arg_list.push_back(
                 QString::number(game_database->getCurrentMap()->getID()));
    run_process.start(exec_program, arg_list);

    /* If successfully run, disable app. Otherwise, failed to run */
    if(run_process.waitForStarted(500))
    {
      setDisabled(true);
    }
    else
    {
      QMessageBox::information(this, "Failed to Start",
                               "Executable failed to fork");
    }
  }
  /* Otherwise, pop-up warning */
  else if(run_process.state() == QProcess::Running)
  {
    QMessageBox::information(this, "Can't Start!",
                         "Univursa app must be closed before testing another.");
  }
  else
  {
    QMessageBox::information(this, "Can't Start!",
                             "No map selected to test.");
  }
}

/* Play execution finished */
void Application::playFinished(int)
{
  QString play_file = EditorHelpers::getProjectDir() +
                      "/../Editor/exports/xXx_TMP_xXx.utv";

  /* Output the command line info */
  qDebug() << " -- PLAY OUTPUT -- ";
  qDebug() << run_process.readAll();
  qDebug() << " -- END -- ";

  /* Re-enable and remove the file */
  setEnabled(true);
  QFile::remove(play_file);
}

/* Save Action */
void Application::save()
{
  if(file_name == "")
    saveAs();
  else
    saveApp();
}

/* Save As Action */
void Application::saveAs()
{
  QString file = QFileDialog::getSaveFileName(this, "Save Game Editor File As",
                          EditorHelpers::getSpriteDir() + "/../../Editor/saves",
                          tr("Univursa Saves (*.usv)"));
  if(file != "")
  {
    if(!file.endsWith(".usv"))
      file += ".usv";
    file_name = file;
    saveApp();
  }
}

/*
 * Description: Sets the action
 */
void Application::setAction(EditorAction* action)
{
  game_view->setActionView(action);
}

/*
 * Description: Sets the battle class
 */
void Application::setBattleClass(EditorCategory* battle_class)
{
  game_view->setClassView(battle_class);
}

/*
 * Description: Sets to battle scene and updates the view
 *
 * Inputs: EditorBattleScene* scene - the new edit scene
 *         bool save - true to save if existing is not null
 * Output: none
 */
void Application::setBattleScene(EditorBattleScene* scene, bool save)
{
  game_view->setBattleSceneView(scene, save);
}

/*
 * Description: Sets the bubby
 */
//void Application::setBubby(EditorBubby* bubby)
//{
//  qDebug() << "Bubby: " << bubby;
//}

/*
 * Description: Sets to basic cursor mode
 *
 * Inputs: bool checked - true if button triggered on
 * Output: none
 */
void Application::setCursorBasic(bool checked)
{
  if(checked)
    game_view->getMapView()->setCursorMode(EditorEnumDb::BASIC);
}

/*
 * Description: Sets to block place cursor mode
 *
 * Inputs: bool checked - true if button triggered on
 * Output: none
 */
void Application::setCursorBlock(bool checked)
{
  if(checked)
    game_view->getMapView()->setCursorMode(EditorEnumDb::BLOCKPLACE);
}

/*
 * Description: Sets to eraser cursor mode
 *
 * Inputs: bool checked - true if button triggered on
 * Output: none
 */
void Application::setCursorEraser(bool checked)
{
  if(checked)
    game_view->getMapView()->setCursorMode(EditorEnumDb::ERASER);
}

/*
 * Description: Sets to fill cursor mode
 *
 * Inputs: bool checked - true if button triggered on
 * Output: none
 */
void Application::setCursorFill(bool checked)
{
  if(checked)
    game_view->getMapView()->setCursorMode(EditorEnumDb::FILL);
}

/*
 * Description: Sets to move cursor mode
 *
 * Inputs: bool checked - true if button triggered on
 * Output: none
 */
void Application::setCursorMove(bool checked)
{
  if(checked)
    game_view->getMapView()->setCursorMode(EditorEnumDb::MOVE);
}

/*
 * Description: Sets the equipment
 */
//void Application::setEquipment(EditorEquipment* equipment)
//{
//  qDebug() << "Equipment: " << equipment;
//}

/*
 * Description: Sets the item
 */
void Application::setItem(EditorItem* item)
{
  game_view->setItemView(item);
}

/*
 * Description: Sets the map
 */
void Application::setMap(EditorMap* map)
{
  game_view->getMapView()->setMapEditor(map);
}

/*
 * Description: Sets the party
 */
void Application::setParty(EditorParty* party)
{
  game_view->setPartyView(party);
}

/*
 * Description: Sets to passability all directions mode
 *
 * Inputs: bool checked - true if button triggered on
 * Output: none
 */
void Application::setPassAllCursor(bool checked)
{
  if(checked)
    game_view->getMapView()->setCursorMode(EditorEnumDb::PASS_ALL);
}

/*
 * Description: Sets to passability east direction mode
 *
 * Inputs: bool checked - true if button triggered on
 * Output: none
 */
void Application::setPassEastCursor(bool checked)
{
  if(checked)
    game_view->getMapView()->setCursorMode(EditorEnumDb::PASS_E);
}

/*
 * Description: Sets to passability north direction mode
 *
 * Inputs: bool checked - true if button triggered on
 * Output: none
 */
void Application::setPassNorthCursor(bool checked)
{
  if(checked)
    game_view->getMapView()->setCursorMode(EditorEnumDb::PASS_N);
}

/*
 * Description: Sets to passability south direction mode
 *
 * Inputs: bool checked - true if button triggered on
 * Output: none
 */
void Application::setPassSouthCursor(bool checked)
{
  if(checked)
    game_view->getMapView()->setCursorMode(EditorEnumDb::PASS_S);
}

/*
 * Description: Sets to passability west direction mode
 *
 * Inputs: bool checked - true if button triggered on
 * Output: none
 */
void Application::setPassWestCursor(bool checked)
{
  if(checked)
    game_view->getMapView()->setCursorMode(EditorEnumDb::PASS_W);
}

/*
 * Description: Sets the person
 */
void Application::setPerson(EditorPerson* person)
{
  game_view->setPersonView(person);
}

/*
 * Description: Sets the race
 */
void Application::setRace(EditorCategory* race)
{
  game_view->setRaceView(race);
}

/*
 * Description: Sets the skill
 */
void Application::setSkill(EditorSkill* skill)
{
  game_view->setSkillView(skill);
}

/*
 * Description: Sets the skill set
 */
void Application::setSkillset(EditorSkillset* skill_set)
{
  game_view->setSkillsetView(skill_set);
}

/*
 * Description: Sets to any mde
 */
void Application::setView(EditorEnumDb::ViewMode v)
{
  game_view->setViewMode(v);
  if(v != EditorEnumDb::MAPVIEW)
  {
    bar_brush->setDisabled(true);
    menu_cursor->setDisabled(true);
    bar_map_ctrl->setDisabled(true);
  }
  else
  {
    bar_brush->setEnabled(true);
    menu_cursor->setEnabled(true);
    bar_map_ctrl->setEnabled(true);
  }
}

/*
 * Description: Sets to item view mode
 */
void Application::setViewItem()
{
  game_view->setViewMode(EditorEnumDb::ITEMVIEW);
}

/*
 * Description: Sets to map view mode
 */
void Application::setViewMap()
{
  game_view->setViewMode(EditorEnumDb::MAPVIEW);
}

/*
 * Description: Sets to party view mode
 */
void Application::setViewParty()
{
  game_view->setViewMode(EditorEnumDb::PARTYVIEW);
}

/*
 * Description: Sets to person view mode
 */
void Application::setViewPerson()
{
  game_view->setViewMode(EditorEnumDb::PERSONVIEW);
}

/*
 * Description: Show the database dock on the left side
 *
 * Inputs: none
 * Output: none
 */
void Application::showDatabase()
{
  if(game_db_dock->isVisible())
    game_db_dock->hide();
  else
    game_db_dock->show();
}

/* Zoom in or out in the map */
void Application::zoomInMap()
{
  if(action_zoom_in->isEnabled())
  {
    /* Zoom out should be enabled if zooming in */
    action_zoom_out->setEnabled(true);

    /* If zoom in returns true, it's reached the end */
    if(game_view->getMapView()->zoomIn())
      action_zoom_in->setDisabled(true);
  }
}

/* Zoom in or out in the map */
void Application::zoomOutMap()
{
  if(action_zoom_out->isEnabled())
  {
    /* Zoom in should be enabled if zooming out */
    action_zoom_in->setEnabled(true);

    /* If zoom in returns true, it's reached the end */
    if(game_view->getMapView()->zoomOut())
      action_zoom_out->setDisabled(true);
  }
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/

/*
 * Description: The custom close event for saving changes
 *
 * Inputs: The event passed
 */
void Application::closeEvent(QCloseEvent *event)
{
/*
  QString message = "Are you sure you want to do this ";
  message.append(username);
  message.append("...");

  if(QMessageBox::question(this, "Exit Editor",
                           message,
                           QMessageBox::Ok | QMessageBox::Cancel,
                           QMessageBox::Ok) == QMessageBox::Ok)
    event->accept();
  else
    event->ignore();
*/
  event->accept();
}
