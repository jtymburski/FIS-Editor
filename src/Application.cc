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
  x_size = 100;
  y_size = 100;

  game_database = new GameDatabase();
  QDockWidget* game_db_dock = new QDockWidget();
  game_db_dock->setWindowIcon(QIcon(":/Icons/Resources/fbs_icon.ico"));
  game_db_dock->setWidget(game_database);
  addDockWidget(Qt::LeftDockWidgetArea,game_db_dock);
  game_db_dock->setFeatures(QDockWidget::NoDockWidgetFeatures);
  game_db_dock->setAllowedAreas(Qt::LeftDockWidgetArea);
  QPushButton* dock_button =
      new QPushButton(QIcon(":/Icons/Resources/fbs_icon.ico"),"");
  connect(dock_button,SIGNAL(pressed()),game_database,SLOT(resize()));
  game_db_dock->setTitleBarWidget(dock_button);
  connect(game_database, SIGNAL(changeMode(EditorEnumDb::ViewMode)),
          this, SLOT(setView(EditorEnumDb::ViewMode)));

  connect(game_database, SIGNAL(changeMap(QPair<QString,EditorMap*>*)),
          this, SLOT(setMap(QPair<QString,EditorMap*>*)));
  connect(game_database, SIGNAL(changePerson(QPair<QString,EditorPerson*>*)),
          this, SLOT(setPerson(QPair<QString,EditorPerson*>*)));
  connect(game_database, SIGNAL(changeParty(QPair<QString,EditorParty*>*)),
          this, SLOT(setParty(QPair<QString,EditorParty*>*)));
  connect(game_database, SIGNAL(changeItem(QPair<QString,EditorItem*>*)),
          this, SLOT(setItem(QPair<QString,EditorItem*>*)));

  game_view = new GameView();
  setCentralWidget(game_view);
  /* Calls all setup functions */
  setWindowTitle("Univursa Designer");
  setWindowIcon(QIcon(":/Icons/Resources/fbs_icon.ico"));
  setupTopMenu();
  setGeometry(QApplication::desktop()->availableGeometry());
  showMaximized();
  setBasicCursor();
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
  /* Sets up map size dialog */
  mapsize_dialog = new QDialog(this);
  QGridLayout* layout = new QGridLayout(mapsize_dialog);
  layout->addWidget(new QLabel("Choose Initial Map Size"),0,0);
  layout->addWidget(new QLabel("X:"),1,0);
  QLineEdit* sizexedit = new QLineEdit("100",this);
  layout->addWidget(sizexedit,1,1);
  layout->addWidget(new QLabel("Y:"),2,0);
  QLineEdit* sizeyedit = new QLineEdit("100",this);
  layout->addWidget(sizeyedit,2,1);
  QPushButton* ok = new QPushButton("&Create Map",this);
  layout->addWidget(ok,3,0);
  connect(ok,SIGNAL(clicked()),this,SLOT(setupMap()));

  /* Sets up the File menu actions */
  new_action = new QAction("&New",this);
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
  connect(quit_action,SIGNAL(triggered()), this, SLOT(close()));
  connect(new_action,SIGNAL(triggered()),mapsize_dialog,SLOT(show()));

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

  QMenu* cursor_menu = menuBar()->addMenu("&Cursor Modes");


  /* Sets up the menu toolbars */
  menubar = new QToolBar("Menus",this);
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

  gameview_group = new QActionGroup(this);
  gameview_group->setExclusive(true);

  mapview_action = new QAction("&Map",gameview_group);
  mapview_action->setCheckable(true);
  mapview_action->setChecked(true);
  //mapview_action->setIcon(QIcon(":/Icons/Resources/pencil-icon.png"));
  person_action = new QAction("&Person",gameview_group);
  person_action->setCheckable(true);
  //erasermode_action->setIcon(QIcon(":/Icons/Resources/eraser-icon.png"));
  party_action = new QAction("&Party",gameview_group);
  party_action->setCheckable(true);
  //blockplacemode_action->setIcon(QIcon(":/Icons/Resources/rect-icon.png"));
  item_action = new QAction("&Item",gameview_group);
  item_action->setCheckable(true);
  //fill_action->setIcon(QIcon(":/Icons/Resources/flood-icon.png"));

  connect(mapview_action,SIGNAL(triggered()),this,SLOT(setMapView()));
  connect(person_action,SIGNAL(triggered()),this,SLOT(setPersonView()));
  connect(party_action,SIGNAL(triggered()),this,SLOT(setPartyView()));
  connect(item_action,SIGNAL(triggered()),this,SLOT(setItemView()));

  /* Sets up the view modes toolbar */
  viewbar = new QToolBar("View Modes",this);
  viewbar->addAction(mapview_action);
  viewbar->addAction(person_action);
  viewbar->addAction(party_action);
  viewbar->addAction(item_action);
  addToolBar(Qt::TopToolBarArea,viewbar);
  viewbar->setFloatable(false);
  viewbar->setMovable(false);

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
void Application::setMap(QPair<QString,EditorMap*>* pair)
{
  qDebug()<<pair->first;
}
/*
 * Description: Sets the person
 */
void Application::setPerson(QPair<QString,EditorPerson*>* pair)
{
  qDebug()<<pair->first;
}
/*
 * Description: Sets the map
 */
void Application::setParty(QPair<QString,EditorParty*>* pair)
{
  qDebug()<<pair->first;
}
/*
 * Description: Sets the map
 */
void Application::setItem(QPair<QString,EditorItem*>* pair)
{
  qDebug()<<pair->first;
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
 * Description: Sets up the map based on the popup with the size
 *
 * Inputs: none
 * Output: none
 */
void Application::setupMap()
{
  if(mapsize_dialog != NULL)
  {
    QLayout* layout = mapsize_dialog->layout();
    QString x_line = ((QLineEdit*)((QGridLayout*)layout)->itemAtPosition(1,1)
                                                        ->widget())->text();
    QString y_line = ((QLineEdit*)((QGridLayout*)layout)->itemAtPosition(2,1)
                                                        ->widget())->text();

    game_view->getMapView()->setupMapView(x_line.toInt(), y_line.toInt());
    mapsize_dialog->close();
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
}
