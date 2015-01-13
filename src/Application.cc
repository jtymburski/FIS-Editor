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
Application::Application(QWidget* parent) :
  QMainWindow(parent)
{

  /* Gets the users name in windows only */
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
  setBasicCursor();

  setStyleSheet("QMainWindow::separator { background: rgb(153, 153, 153); \
                                          width: 1px; height: 1px; }");
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
/*
 * Description: Sets up the Top Menu
 *
 * Inputs: none
 */
void Application::setupTopMenu()
{
  /* Sets up the File menu actions */
  show_menu_action = new QAction("&Menu", this);
  show_menu_action->setIcon(QIcon(":/Icons/Resources/database.png"));
  new_action = new QAction("&New",this);
  new_action->setDisabled(true);
  new_action->setIcon(QIcon(":/Icons/Resources/new-icon.png"));
  load_action = new QAction("&Load",this);
  load_action->setIcon(QIcon(":/Icons/Resources/load-icon.png"));
  recentfiles_action = new QAction("&Recent Files",this);
  save_action = new QAction("&Save",this);
  save_action->setIcon(QIcon(":/Icons/Resources/save-icon.png"));
  saveas_action = new QAction("&Save As",this);
  saveas_action->setIcon(QIcon(":/Icons/Resources/saveas-icon.png"));
  quit_action = new QAction("&Quit",this);

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
  // TODO: This should be not map, but new game. New map should be triggered
  // from a different view
  //connect(new_action,SIGNAL(triggered()),mapsize_dialog,SLOT(show()));

  /* Sets up Edit menu actions*/
  undo_action = new QAction("&Undo",this);
  undo_action->setIcon(QIcon(":/Icons/Resources/undo-icon.png"));
  redo_action = new QAction("&Redo",this);
  redo_action->setIcon(QIcon(":/Icons/Resources/redo-icon.png"));
  cut_action = new QAction("&Cut",this);
  cut_action->setIcon(QIcon(":/Icons/Resources/cut-icon.png"));
  copy_action = new QAction("&Copy",this);
  copy_action->setIcon(QIcon(":/Icons/Resources/copy-icon.png"));
  paste_action = new QAction("&Paste",this);
  paste_action->setIcon(QIcon(":/Icons/Resources/paste-icon.png"));
  findreplace_action = new QAction("&Find/Replace",this);
  mapsize_action = new QAction("&Map Size",this);

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

  cursor_group = new QActionGroup(this);
  cursor_group->setExclusive(false);

  basicmode_action = new QAction("&Basic",cursor_group);
  basicmode_action->setCheckable(true);
  basicmode_action->setChecked(true);
  basicmode_action->setIcon(QIcon(":/Icons/Resources/pencil-icon.png"));
  erasermode_action = new QAction("&Eraser",cursor_group);
  erasermode_action->setCheckable(true);
  erasermode_action->setIcon(QIcon(":/Icons/Resources/eraser-icon.png"));
  blockplacemode_action = new QAction("&Block Place",cursor_group);
  blockplacemode_action->setCheckable(true);
  blockplacemode_action->setIcon(QIcon(":/Icons/Resources/rect-icon.png"));
  fill_action = new QAction("&Fill",cursor_group);
  fill_action->setCheckable(true);
  fill_action->setIcon(QIcon(":/Icons/Resources/flood-icon.png"));

  cursor_menu = menuBar()->addMenu("&Cursor Modes");

  /* Sets up the menu toolbars */
  menubar = new QToolBar("Menus",this);
  menubar->addAction(show_menu_action);
  menubar->addSeparator();
  menubar->addAction(new_action);
  menubar->addAction(load_action);
  menubar->addAction(save_action);
  menubar->addAction(saveas_action);
  menubar->addSeparator();
  menubar->addAction(undo_action);
  menubar->addAction(redo_action);
  menubar->addAction(cut_action);
  menubar->addAction(copy_action);
  menubar->addAction(paste_action);
  addToolBar(Qt::TopToolBarArea,menubar);
  menubar->setFloatable(false);
  menubar->setMovable(false);

  /* Sets up the brushes toolbar */
  brushbar = new QToolBar("Brushes",this);
  brushbar->addSeparator();
  brushbar->addAction(basicmode_action);
  brushbar->addAction(erasermode_action);
  brushbar->addAction(blockplacemode_action);
  brushbar->addAction(fill_action);
  addToolBar(Qt::TopToolBarArea,brushbar);
  cursor_menu->addAction(basicmode_action);
  cursor_menu->addAction(erasermode_action);
  cursor_menu->addAction(blockplacemode_action);
  cursor_menu->addAction(fill_action);
  brushbar->setFloatable(false);
  brushbar->setMovable(false);

  connect(basicmode_action,SIGNAL(triggered()),this,SLOT(setBasicCursor()));
  connect(erasermode_action,SIGNAL(triggered()),this,SLOT(setEraserCursor()));
  connect(blockplacemode_action,SIGNAL(triggered()),
          this,SLOT(setBlockCursor()));
  connect(fill_action,SIGNAL(triggered()),this,SLOT(setFillCursor()));
}

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/
/*
 * Description: Sets to any mde
 */
void Application::setView(EditorEnumDb::ViewMode v)
{
  game_view->setViewMode(v);
  if(v != EditorEnumDb::MAPVIEW)
  {
    brushbar->setDisabled(true);
    cursor_menu->setDisabled(true);
  }
  else
  {
    brushbar->setEnabled(true);
    cursor_menu->setEnabled(true);
  }
}

/*
 * Description: Sets to map view mode
 */
void Application::setMapView()
{
  game_view->setViewMode(EditorEnumDb::MAPVIEW);
}

/*
 * Description: Sets to person view mode
 */
void Application::setPersonView()
{
  game_view->setViewMode(EditorEnumDb::PERSONVIEW);
}

/*
 * Description: Sets to party view mode
 */
void Application::setPartyView()
{
  game_view->setViewMode(EditorEnumDb::PARTYVIEW);
}

/*
 * Description: Sets to item view mode
 */
void Application::setItemView()
{
  game_view->setViewMode(EditorEnumDb::ITEMVIEW);
}

/*
 * Description: Sets the map
 */
void Application::setMap(EditorMap* map)
{
  qDebug() << "Map: " << map;
}
/*
 * Description: Sets the person
 */
void Application::setPerson(EditorPerson* person)
{
  qDebug() << "Person: " << person;
}
/*
 * Description: Sets the party
 */
void Application::setParty(EditorParty* party)
{
  qDebug() << "Party: " << party;
}
/*
 * Description: Sets the item
 */
void Application::setItem(EditorItem* item)
{
  qDebug() << "Item: " << item;
}
/*
 * Description: Sets the action
 */
void Application::setAction(EditorAction* action)
{
  game_view->setActionView(action);
}

/*
 * Description: Sets the race
 */
void Application::setRace(EditorCategory* race)
{
  qDebug() << "Race: " << race;
}

/*
 * Description: Sets the battle class
 */
void Application::setBattleClass(EditorCategory* battle_class)
{
  qDebug() << "Battle Class: " << battle_class;
}

/*
 * Description: Sets the skill set
 */
void Application::setSkillset(EditorSkillset* skill_set)
{
  game_view->setSkillsetView(skill_set);
}

/*
 * Description: Sets the skill
 */
void Application::setSkill(EditorSkill* skill)
{
  game_view->setSkillView(skill);
}

/*
 * Description: Sets the equipment
 */
void Application::setEquipment(EditorEquipment* equipment)
{
  qDebug() << "Equipment: " << equipment;
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
void Application::setBasicCursor()
{
  erasermode_action->setChecked(false);
  blockplacemode_action->setChecked(false);
  fill_action->setChecked(false);
  cursor_mode = EditorEnumDb::BASIC;
  game_view->getMapView()->map_editor->setCursorMode(EditorEnumDb::BASIC);
}

/*
 * Description: Sets to eraser cursor mode
 */
void Application::setEraserCursor()
{
  basicmode_action->setChecked(false);
  blockplacemode_action->setChecked(false);
  fill_action->setChecked(false);
  cursor_mode = EditorEnumDb::ERASER;
  game_view->getMapView()->map_editor->setCursorMode(EditorEnumDb::ERASER);
}

/*
 * Description: Sets to block place cursor mode
 */
void Application::setBlockCursor()
{
  basicmode_action->setChecked(false);
  erasermode_action->setChecked(false);
  fill_action->setChecked(false);
  cursor_mode = EditorEnumDb::BLOCKPLACE;
  game_view->getMapView()->map_editor->setCursorMode(EditorEnumDb::BLOCKPLACE);
}


/*
 * Description: Sets to fill cursor mode
 */
void Application::setFillCursor()
{
  basicmode_action->setChecked(false);
  blockplacemode_action->setChecked(false);
  erasermode_action->setChecked(false);
  cursor_mode = EditorEnumDb::FILL;
  game_view->getMapView()->map_editor->setCursorMode(EditorEnumDb::FILL);
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
