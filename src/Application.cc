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

  game_db_dock = new QDockWidget();
  game_database = new GameDatabase();
  game_db_dock->setWindowIcon(QIcon(":/Icons/Resources/fbs_icon.ico"));
  game_db_dock->setWidget(new QWidget());
  game_db_dock->setWidget(game_database);
  addDockWidget(Qt::LeftDockWidgetArea,game_db_dock);
  game_db_dock->setFeatures(QDockWidget::NoDockWidgetFeatures);
  game_db_dock->setAllowedAreas(Qt::LeftDockWidgetArea);
  game_db_dock->setTitleBarWidget(new QWidget());

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
  connect(game_database, SIGNAL(changeBattleclass
                                (EditorCategory*)),
          this, SLOT(setBattleClass(EditorCategory*)));
  connect(game_database, SIGNAL(changeSkillset(EditorSkillset*))
          ,this, SLOT(setSkillset(EditorSkillset*)));
  connect(game_database, SIGNAL(changeSkill(EditorSkill*)),
          this, SLOT(setSkill(EditorSkill*)));
  connect(game_database, SIGNAL(changeEquipment
                                (EditorEquipment*)),
          this, SLOT(setEquipment(EditorEquipment*)));
  connect(game_database, SIGNAL(changeBubby(EditorBubby*)),
          this, SLOT(setBubby(EditorBubby*)));

  game_view = new GameView();

  setCentralWidget(game_view);
  game_view->setGeometry(QApplication::desktop()->availableGeometry());

  connect(game_view,SIGNAL(nameChange(QString)),game_database,
          SLOT(updateBottomListName(QString)));

  /* Calls all setup functions */
  setWindowTitle("Univursa Designer");
  setWindowIcon(QIcon(":/Icons/Resources/fbs_icon.ico"));
  setupTopMenu();
  //setGeometry(16,32,QApplication::desktop()->availableGeometry().width()-32,
   //           QApplication::desktop()->availableGeometry().height()-64);
  setMinimumSize(1280,720);
  showMaximized();
  setCursorBasic();

  setStyleSheet("QMainWindow::separator { background: rgb(153, 153, 153); \
                                          width: 1px; height: 1px; }");

  /* TEMPORARY - TODO: REMOVE */
  game_database->tempMake();
}

/*
 * Description: Destructor function
 */
Application::~Application()
{
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/

/* Export application to run in Univursa */
void Application::exportGame()
{

}

/* Load application */
void Application::loadApp()
{
  qDebug() << "Load: " << file_name;
}

/* Save application */
void Application::saveApp()
{
  /* Start file write */
  FileHandler fh(file_name.toStdString(), true, true);
  fh.start();

  /* Write the data */
  game_database->save(&fh);

  /* Finish the file write */
  fh.stop();
}

/*
 * Description: Sets up the Top Menu
 *
 * Inputs: none
 */
void Application::setupTopMenu()
{
  /* Sets up the File menu actions */
  QAction* show_menu_action = new QAction("&Menu", this);
  show_menu_action->setIcon(QIcon(":/Icons/Resources/database.png"));
  QAction* new_action = new QAction("&New",this);
  new_action->setDisabled(true);
  new_action->setIcon(QIcon(":/Icons/Resources/new-icon.png"));
  QAction* load_action = new QAction("&Load",this);
  load_action->setIcon(QIcon(":/Icons/Resources/load-icon.png"));
  QAction* recentfiles_action = new QAction("&Recent Files",this);
  QAction* save_action = new QAction("&Save",this);
  save_action->setIcon(QIcon(":/Icons/Resources/save-icon.png"));
  QAction* saveas_action = new QAction("&Save As",this);
  saveas_action->setIcon(QIcon(":/Icons/Resources/saveas-icon.png"));
  QAction* quit_action = new QAction("&Quit",this);

  /* Sets up file menu itself */
  QMenu* file_menu = menuBar()->addMenu("&File");
  file_menu->addAction(new_action);
  file_menu->addAction(load_action);
  file_menu->addAction(recentfiles_action);
  file_menu->addSeparator();
  file_menu->addAction(save_action);
  file_menu->addAction(saveas_action);
  file_menu->addSeparator();
  file_menu->addAction(quit_action);

  /* Connects File menu actions to slots */
  connect(show_menu_action, SIGNAL(triggered()), this, SLOT(showDatabase()));
  connect(quit_action,SIGNAL(triggered()), this, SLOT(close()));

  /* Sets up Edit menu actions*/
  QAction* undo_action = new QAction("&Undo",this);
  undo_action->setIcon(QIcon(":/Icons/Resources/undo-icon.png"));
  QAction* redo_action = new QAction("&Redo",this);
  redo_action->setIcon(QIcon(":/Icons/Resources/redo-icon.png"));
  QAction* cut_action = new QAction("&Cut",this);
  cut_action->setIcon(QIcon(":/Icons/Resources/cut-icon.png"));
  QAction* copy_action = new QAction("&Copy",this);
  copy_action->setIcon(QIcon(":/Icons/Resources/copy-icon.png"));
  QAction* paste_action = new QAction("&Paste",this);
  paste_action->setIcon(QIcon(":/Icons/Resources/paste-icon.png"));
  QAction* findreplace_action = new QAction("&Find/Replace",this);
  QAction* mapsize_action = new QAction("&Map Size",this);

  /* Sets up Edit menu itself */
  QMenu* edit_menu = menuBar()->addMenu("&Edit");
  edit_menu->addAction(undo_action);
  edit_menu->addAction(redo_action);
  edit_menu->addSeparator();
  edit_menu->addAction(cut_action);
  edit_menu->addAction(copy_action);
  edit_menu->addAction(paste_action);
  edit_menu->addSeparator();
  edit_menu->addAction(findreplace_action);
  edit_menu->addSeparator();
  edit_menu->addAction(mapsize_action);

  QActionGroup* cursor_group = new QActionGroup(this);
  cursor_group->setExclusive(true);

  QAction* basicmode_action = new QAction("&Basic",cursor_group);
  basicmode_action->setCheckable(true);
  basicmode_action->setChecked(true);
  basicmode_action->setIcon(QIcon(":/Icons/Resources/pencil-icon.png"));
  QAction* erasermode_action = new QAction("&Eraser",cursor_group);
  erasermode_action->setCheckable(true);
  erasermode_action->setIcon(QIcon(":/Icons/Resources/eraser-icon.png"));
  QAction* blockplacemode_action = new QAction("&Block Place",cursor_group);
  blockplacemode_action->setCheckable(true);
  blockplacemode_action->setIcon(QIcon(":/Icons/Resources/rect-icon.png"));
  QAction* fill_action = new QAction("&Fill",cursor_group);
  fill_action->setCheckable(true);
  fill_action->setIcon(QIcon(":/Icons/Resources/flood-icon.png"));
  QAction* pass_all_action = new QAction("&Passability All", cursor_group);
  pass_all_action->setCheckable(true);
  pass_all_action->setIcon(QIcon(":/Icons/Resources/changepass_all.png"));
  QAction* pass_north_action = new QAction("&Passability North", cursor_group);
  pass_north_action->setCheckable(true);
  pass_north_action->setIcon(QIcon(":/Icons/Resources/changepass_N.png"));
  QAction* pass_east_action = new QAction("&Passability East", cursor_group);
  pass_east_action->setCheckable(true);
  pass_east_action->setIcon(QIcon(":/Icons/Resources/changepass_E.png"));
  QAction* pass_south_action = new QAction("&Passability South", cursor_group);
  pass_south_action->setCheckable(true);
  pass_south_action->setIcon(QIcon(":/Icons/Resources/changepass_S.png"));
  QAction* pass_west_action = new QAction("&Passability West", cursor_group);
  pass_west_action->setCheckable(true);
  pass_west_action->setIcon(QIcon(":/Icons/Resources/changepass_W.png"));
  cursor_menu = menuBar()->addMenu("&Cursor Modes");

  /* Sets up the menu toolbars */
  bar_menu = new QToolBar("Menus",this);
  bar_menu->addAction(show_menu_action);
  bar_menu->addSeparator();
  bar_menu->addAction(new_action);
  bar_menu->addAction(load_action);
  bar_menu->addAction(save_action);
  bar_menu->addAction(saveas_action);
  bar_menu->addSeparator();
  bar_menu->addAction(undo_action);
  bar_menu->addAction(redo_action);
  bar_menu->addAction(cut_action);
  bar_menu->addAction(copy_action);
  bar_menu->addAction(paste_action);
  addToolBar(Qt::TopToolBarArea,bar_menu);
  bar_menu->setFloatable(false);
  bar_menu->setMovable(false);
  connect(load_action, SIGNAL(triggered()), this, SLOT(load()));
  connect(save_action, SIGNAL(triggered()), this, SLOT(save()));
  connect(saveas_action, SIGNAL(triggered()), this, SLOT(saveAs()));

  /* Sets up the brushes toolbar */
  bar_brush = new QToolBar("Brushes", this);
  bar_brush->addSeparator();
  bar_brush->addAction(basicmode_action);
  bar_brush->addAction(erasermode_action);
  bar_brush->addAction(blockplacemode_action);
  bar_brush->addAction(fill_action);
  bar_brush->addAction(pass_all_action);
  bar_brush->addAction(pass_north_action);
  bar_brush->addAction(pass_east_action);
  bar_brush->addAction(pass_south_action);
  bar_brush->addAction(pass_west_action);
  addToolBar(Qt::TopToolBarArea,bar_brush);
  cursor_menu->addAction(basicmode_action);
  cursor_menu->addAction(erasermode_action);
  cursor_menu->addAction(blockplacemode_action);
  cursor_menu->addAction(fill_action);
  cursor_menu->addAction(pass_all_action);
  cursor_menu->addAction(pass_north_action);
  cursor_menu->addAction(pass_east_action);
  cursor_menu->addAction(pass_south_action);
  cursor_menu->addAction(pass_west_action);
  bar_brush->setFloatable(false);
  bar_brush->setMovable(false);

  connect(basicmode_action,SIGNAL(triggered()),this,SLOT(setCursorBasic()));
  connect(erasermode_action,SIGNAL(triggered()),this,SLOT(setCursorEraser()));
  connect(blockplacemode_action,SIGNAL(triggered()),
          this,SLOT(setCursorBlock()));
  connect(fill_action,SIGNAL(triggered()),this,SLOT(setCursorFill()));
  connect(pass_all_action, SIGNAL(triggered()), this, SLOT(setPassAllCursor()));
  connect(pass_north_action, SIGNAL(triggered()),
          this, SLOT(setPassNorthCursor()));
  connect(pass_east_action, SIGNAL(triggered()),
          this, SLOT(setPassEastCursor()));
  connect(pass_south_action, SIGNAL(triggered()),
          this, SLOT(setPassSouthCursor()));
  connect(pass_west_action, SIGNAL(triggered()),
          this, SLOT(setPassWestCursor()));
}

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/

/* Load action */
void Application::load()
{
  QString file = QFileDialog::getOpenFileName(this, "Load Game Editor File",
                          EditorHelpers::getSpriteDir() + "/../../Editor/saves",
                          tr("Univursa Saves (*.usv)"));
  if(file != "")
  {
    file_name = file;
    loadApp();
  }
}

/* Save and save as action */
void Application::save()
{
  if(file_name == "")
    saveAs();
  else
    saveApp();
}

void Application::saveAs()
{
  QString file = QFileDialog::getSaveFileName(this, "Save Game Editor File",
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
  qDebug() << "Battle Class: " << battle_class;
}

/*
 * Description: Sets the bubby
 */
void Application::setBubby(EditorBubby* bubby)
{
  qDebug() << "Bubby: " << bubby;
}

/*
 * Description: Sets to basic cursor mode
 */
void Application::setCursorBasic()
{
  game_view->getMapView()->setCursorMode(EditorEnumDb::BASIC);
}

/*
 * Description: Sets to block place cursor mode
 */
void Application::setCursorBlock()
{
  game_view->getMapView()->setCursorMode(EditorEnumDb::BLOCKPLACE);
}

/*
 * Description: Sets to eraser cursor mode
 */
void Application::setCursorEraser()
{
  game_view->getMapView()->setCursorMode(EditorEnumDb::ERASER);
}

/*
 * Description: Sets to fill cursor mode
 */
void Application::setCursorFill()
{
  game_view->getMapView()->setCursorMode(EditorEnumDb::FILL);
}

/*
 * Description: Sets the equipment
 */
void Application::setEquipment(EditorEquipment* equipment)
{
  qDebug() << "Equipment: " << equipment;
}

/*
 * Description: Sets the item
 */
void Application::setItem(EditorItem* item)
{
  qDebug() << "Item: " << item;
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
  qDebug() << "Party: " << party;
}

/* Sets to Passability Mode */
void Application::setPassAllCursor()
{
  game_view->getMapView()->setCursorMode(EditorEnumDb::PASS_ALL);
}

/* Sets to Passability Mode */
void Application::setPassEastCursor()
{
  game_view->getMapView()->setCursorMode(EditorEnumDb::PASS_E);
}

/* Sets to Passability Mode */
void Application::setPassNorthCursor()
{
  game_view->getMapView()->setCursorMode(EditorEnumDb::PASS_N);
}

/* Sets to Passability Mode */
void Application::setPassSouthCursor()
{
  game_view->getMapView()->setCursorMode(EditorEnumDb::PASS_S);
}

/* Sets to Passability Mode */
void Application::setPassWestCursor()
{
  game_view->getMapView()->setCursorMode(EditorEnumDb::PASS_W);
}

/*
 * Description: Sets the person
 */
void Application::setPerson(EditorPerson* person)
{
  qDebug() << "Person: " << person;
}

/*
 * Description: Sets the race
 */
void Application::setRace(EditorCategory* race)
{
  qDebug() << "Race: " << race;
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
    cursor_menu->setDisabled(true);
  }
  else
  {
    bar_brush->setEnabled(true);
    cursor_menu->setEnabled(true);
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
