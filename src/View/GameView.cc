/*******************************************************************************
 * Class Name: GameView
 * Date Created: December 22, 2014
 * Inheritance: QWidget
 * Description: View of the game editor component for all game related data:
 *              maps,
 ******************************************************************************/
#include "View/GameView.h"
#include <QDebug>
/* Constructor function */
GameView::GameView(QWidget* parent) : QWidget(parent)
{
  map_view = new MapView(this);
  map_view->setGeometry(0,0,QApplication::desktop()->
                           availableGeometry().width()-260,
                           QApplication::desktop()->
                           availableGeometry().height()-128);
  person_view = new QWidget(this);
  person_view->setGeometry(0,0,QApplication::desktop()->
                           availableGeometry().width()-256,
                           QApplication::desktop()->
                           availableGeometry().height()-128);

  party_view = new QWidget(this);
  party_view->setGeometry(0,0,QApplication::desktop()->
                              availableGeometry().width()-256,
                              QApplication::desktop()->
                              availableGeometry().height()-128);
  item_view = new QWidget(this);
  item_view->setGeometry(0,0,QApplication::desktop()->
                             availableGeometry().width()-256,
                             QApplication::desktop()->
                             availableGeometry().height()-128);
  action_view = new EditorAction(this);
  action_view->setGeometry(0,0,QApplication::desktop()->
                             availableGeometry().width()-256,
                             QApplication::desktop()->
                             availableGeometry().height()-128);
  race_view = new QWidget(this);
  race_view->setGeometry(0,0,QApplication::desktop()->
                             availableGeometry().width()-256,
                             QApplication::desktop()->
                             availableGeometry().height()-128);
  battleclass_view = new QWidget(this);
  battleclass_view->setGeometry(0,0,QApplication::desktop()->
                             availableGeometry().width()-256,
                             QApplication::desktop()->
                             availableGeometry().height()-128);
  skillset_view = new QWidget(this);
  skillset_view->setGeometry(0,0,QApplication::desktop()->
                             availableGeometry().width()-256,
                             QApplication::desktop()->
                             availableGeometry().height()-128);
  skill_view = new QWidget(this);
  skill_view->setGeometry(0,0,QApplication::desktop()->
                             availableGeometry().width()-256,
                             QApplication::desktop()->
                             availableGeometry().height()-128);
  equipment_view = new QWidget(this);
  equipment_view->setGeometry(0,0,QApplication::desktop()->
                             availableGeometry().width()-256,
                             QApplication::desktop()->
                             availableGeometry().height()-128);
  bubby_view = new QWidget(this);
  bubby_view->setGeometry(0,0,QApplication::desktop()->
                             availableGeometry().width()-256,
                             QApplication::desktop()->
                             availableGeometry().height()-128);
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
      map_view->show();
      person_view->hide();
      party_view->hide();
      item_view->hide();
      action_view->hide();
      race_view->hide();
      battleclass_view->hide();
      skillset_view->hide();
      skill_view->hide();
      equipment_view->hide();
      bubby_view->hide();
      break;
    case EditorEnumDb::PERSONVIEW:
      map_view->hide();
      person_view->show();
      party_view->hide();
      item_view->hide();
      action_view->hide();
      race_view->hide();
      battleclass_view->hide();
      skillset_view->hide();
      skill_view->hide();
      equipment_view->hide();
      bubby_view->hide();
      break;
    case EditorEnumDb::PARTYVIEW:
      map_view->hide();
      person_view->hide();
      party_view->show();
      item_view->hide();
      action_view->hide();
      race_view->hide();
      battleclass_view->hide();
      skillset_view->hide();
      skill_view->hide();
      equipment_view->hide();
      bubby_view->hide();
      break;
    case EditorEnumDb::ITEMVIEW:
      map_view->hide();
      person_view->hide();
      party_view->hide();
      item_view->show();
      action_view->hide();
      race_view->hide();
      battleclass_view->hide();
      skillset_view->hide();
      skill_view->hide();
      equipment_view->hide();
      bubby_view->hide();
      break;
    case EditorEnumDb::ACTIONVIEW:
      map_view->hide();
      person_view->hide();
      party_view->hide();
      item_view->hide();
      action_view->show();
      race_view->hide();
      battleclass_view->hide();
      skillset_view->hide();
      skill_view->hide();
      equipment_view->hide();
      bubby_view->hide();
      break;
    case EditorEnumDb::RACEVIEW:
      map_view->hide();
      person_view->hide();
      party_view->hide();
      item_view->hide();
      action_view->hide();
      race_view->show();
      battleclass_view->hide();
      skillset_view->hide();
      skill_view->hide();
      equipment_view->hide();
      bubby_view->hide();
      break;
    case EditorEnumDb::BATTLECLASSVIEW:
      map_view->hide();
      person_view->hide();
      party_view->hide();
      item_view->hide();
      action_view->hide();
      race_view->hide();
      battleclass_view->show();
      skillset_view->hide();
      skill_view->hide();
      equipment_view->hide();
      bubby_view->hide();
      break;
    case EditorEnumDb::SKILLSETVIEW:
      map_view->hide();
      person_view->hide();
      party_view->hide();
      item_view->hide();
      action_view->hide();
      race_view->hide();
      battleclass_view->hide();
      skillset_view->show();
      skill_view->hide();
      equipment_view->hide();
      bubby_view->hide();
      break;
    case EditorEnumDb::SKILLVIEW:
      map_view->hide();
      person_view->hide();
      party_view->hide();
      item_view->hide();
      action_view->hide();
      race_view->hide();
      battleclass_view->hide();
      skillset_view->hide();
      skill_view->show();
      equipment_view->hide();
      bubby_view->hide();
      break;
    case EditorEnumDb::EQUIPMENTVIEW:
      map_view->hide();
      person_view->hide();
      party_view->hide();
      item_view->hide();
      action_view->hide();
      race_view->hide();
      battleclass_view->hide();
      skillset_view->hide();
      skill_view->hide();
      equipment_view->show();
      bubby_view->hide();
      break;
    case EditorEnumDb::BUBBYVIEW:
      map_view->hide();
      person_view->hide();
      party_view->hide();
      item_view->hide();
      action_view->hide();
      race_view->hide();
      battleclass_view->hide();
      skillset_view->hide();
      skill_view->hide();
      equipment_view->hide();
      bubby_view->show();
      break;
    default:
      map_view->show();
      person_view->hide();
      party_view->hide();
      item_view->hide();
      action_view->hide();
      race_view->hide();
      battleclass_view->hide();
      skillset_view->hide();
      skill_view->hide();
      equipment_view->hide();
      bubby_view->hide();
      break;
    }
}
