#ifndef GAMEDATABASE_H
#define GAMEDATABASE_H
/*******************************************************************************
 * Class Name: GameDatabase
 * Date Created: December 23, 2014
 * Inheritance: QWidget
 * Description: Far left view that determines what the game view will become.
 ******************************************************************************/
#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QVector>
#include <QPushButton>
#include <QPair>
#include "EditorEnumDb.h"
#include "Database/EditorMap.h"
#include "Database/EditorPerson.h"
#include "Database/EditorParty.h"
#include "Database/EditorItem.h"

class GameDatabase : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  GameDatabase(QWidget* parent = NULL);

  /* Destructor function */
  ~GameDatabase();

private:
  /* List Widgets */
  QListWidget* top_view;
  QListWidget* bottom_view;

  /* Layout */
  QVBoxLayout* layout;

  /* Vector for actual data in bottom list, populated from game xml data */
  QVector<QPair<QString,EditorMap*>* > map_pair;
  QVector<QPair<QString,EditorPerson*>* > person_pair;
  QVector<QPair<QString,EditorParty*>* > party_pair;
  QVector<QPair<QString,EditorItem*>* > item_pair;

  /* Currently selected object */
  QPair<QString,EditorMap*>* current_map;
  QPair<QString,EditorPerson*>* current_person;
  QPair<QString,EditorParty*>* current_party;
  QPair<QString,EditorItem*>* current_item;

  int current_map_index;
  int current_person_index;
  int current_party_index;
  int current_item_index;

  int current_map_selection;
  int current_person_selection;
  int current_party_selection;
  int current_item_selection;


  QPushButton* new_button;
  QPushButton* del_button;
  QPushButton* import_button;
  QPushButton* duplicate_button;


protected:
public slots:
  void resize();
  void modifyBottomList(int index);
  void modifySelection(QModelIndex index);
  void modifyIndex(int);

  void createNewResource();
  void duplicateResource();
  void deleteResource();

signals:
  void changeMode(EditorEnumDb::ViewMode);
  void changeMap(QPair<QString,EditorMap*>*);
  void changePerson(QPair<QString,EditorPerson*>*);
  void changeParty(QPair<QString,EditorParty*>*);
  void changeItem(QPair<QString,EditorItem*>*);
};
#endif // GAMEDATABASE_H
