/*******************************************************************************
 * Class Name: GameView
 * Date Created: December 22, 2014
 * Inheritance: QStackedWidget
 * Description: View of the game editor component for all game related data:
 *              maps,
 ******************************************************************************/
#include "View/GameView.h"
#include <QDebug>

/* Constructor function */
GameView::GameView(QWidget* parent) : QStackedWidget(parent)
{
  map_view = new MapView(this);
  addWidget(map_view);

  person_view = new QWidget(this);
  addWidget(person_view);

  party_view = new QWidget(this);
  addWidget(party_view);

  item_view = new QWidget(this);
  addWidget(item_view);

  action_view = new EditorAction(this);
  addWidget(action_view);
  
  race_view = new QWidget(this);
  addWidget(race_view);
  
  battleclass_view = new QWidget(this);
  addWidget(battleclass_view);
  
  skillset_view = new QWidget(this);
  addWidget(skillset_view);
  
  skill_view = new QWidget(this);
  addWidget(skill_view);
  
  equipment_view = new QWidget(this);
  addWidget(equipment_view);
  
  bubby_view = new QWidget(this);
  addWidget(bubby_view);
  
  person_view->setStyleSheet("background-color:red;");
  party_view->setStyleSheet("background-color:blue;");
  item_view->setStyleSheet("background-color:green;");
  //action_view->setStyleSheet("background-color:black;");
  race_view->setStyleSheet("background-color:purple;");
  battleclass_view->setStyleSheet("background-color:pink;");
  skillset_view->setStyleSheet("background-color:orange;");
  skill_view->setStyleSheet("background-color:yellow;");
  equipment_view->setStyleSheet("background-color:violet;");
  bubby_view->setStyleSheet("background-color:black;");

  setViewMode(EditorEnumDb::MAPVIEW);
}

/* Destructor function */
GameView::~GameView()
{
  delete map_view;
}

/* Returns the MapView */
MapView* GameView::getMapView()
{
  return map_view;
}

/* Sets the map view */
void GameView::setMapView(MapView *view)
{

}

/* Returns the Editor Action View */
EditorAction* GameView::getActionView()
{
  return action_view;
}

/* Sets the Editor Action View */
void GameView::setActionView(EditorAction *action)
{
  //delete action_view;
  action_view = action;
  qDebug()<<"Set action view";
}

/* Sets The View Mode */
void GameView::setViewMode(EditorEnumDb::ViewMode v)
{
  mode = v;
  switch(mode)
  {
    case EditorEnumDb::MAPVIEW:
      setCurrentIndex(0);
      break;
    case EditorEnumDb::PERSONVIEW:

      //removeWidget(person_view);
      //person_view = new QWidget(this);
      //person_view->setStyleSheet("background-color:black");
      //insertWidget(1, person_view);

      setCurrentIndex(1);
      break;
    case EditorEnumDb::PARTYVIEW:
      setCurrentIndex(2);
      break;
    case EditorEnumDb::ITEMVIEW:
      setCurrentIndex(3);
      break;
    case EditorEnumDb::ACTIONVIEW:
      setCurrentIndex(4);
      break;
    case EditorEnumDb::RACEVIEW:
      setCurrentIndex(5);
      break;
    case EditorEnumDb::BATTLECLASSVIEW:
      setCurrentIndex(6);
      break;
    case EditorEnumDb::SKILLSETVIEW:
      setCurrentIndex(7);
      break;
    case EditorEnumDb::SKILLVIEW:
      setCurrentIndex(8);
      break;
    case EditorEnumDb::EQUIPMENTVIEW:
      setCurrentIndex(9);
      break;
    case EditorEnumDb::BUBBYVIEW:
      setCurrentIndex(10);
      break;
    default:
      setCurrentIndex(0);
      break;
  }
}
