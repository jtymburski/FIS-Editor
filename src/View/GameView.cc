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
  person_view->setStyleSheet("background-color:red;");
  party_view->setStyleSheet("background-color:blue;");
  item_view->setStyleSheet("background-color:green;");

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
      break;
    case EditorEnumDb::PERSONVIEW:
      map_view->hide();
      person_view->show();
      party_view->hide();
      item_view->hide();
      break;
    case EditorEnumDb::PARTYVIEW:
      map_view->hide();
      person_view->hide();
      party_view->show();
      item_view->hide();
      break;
    case EditorEnumDb::ITEMVIEW:
      map_view->hide();
      person_view->hide();
      party_view->hide();
      item_view->show();
      break;
    default:
      map_view->show();
      person_view->hide();
      party_view->hide();
      item_view->hide();
      break;
    }
}
