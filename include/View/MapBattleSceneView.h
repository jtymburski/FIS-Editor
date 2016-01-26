/*******************************************************************************
 * Class Name: MapBattleSceneView
 * Date Created: January 25, 2016
 * Inheritance: QWidget
 * Description: The side toolbar in the map database that gives access to the
 *              battle scenes for each sub map. This also includes provisions
 *              for the core based battle scenes.
 ******************************************************************************/
#ifndef MAPBATTLESCENEVIEW_H
#define MAPBATTLESCENEVIEW_H

//#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "Database/EditorMap.h"

class MapBattleSceneView : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  MapBattleSceneView(QWidget* parent = nullptr);

  /* Destructor function */
  ~MapBattleSceneView();

private:
  /* Button widgets */
  QPushButton* btn_core_add;
  QPushButton* btn_core_rem;
  QPushButton* btn_sub_add;
  QPushButton* btn_sub_rem;

  /* The current editing map */
  EditorMap* editor_map;

  /* List widgets */
  QListWidget* list_core_avail;
  QListWidget* list_core_used;
  QListWidget* list_sub_avail;
  QListWidget* list_sub_used;

  /* Battle scene information, for dropdown and selection list */
  QList<QString> scene_list;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Creates the layout - only called on initial construction */
  void createLayout();

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Triggers the update the data of objects for use in events */
  //void fillWithData(EditorEnumDb::MapObjectMode view);

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* Button control triggers */
  //void buttonAdd();
  //void buttonRemove();

  /* Changed triggers in widgets */
  //void changedListLower(int row);
  //void changedListUpper(int row);
  //void changedWeather(const QString & text);

  /* Refreshes the entire data set within the widget */
  void updateData();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns data, as required */
  QList<QString> getDataScenes();

  /* Gets the editor map */
  EditorMap* getEditorMap();

  /* Sets the editor map, which contains the data needed */
  void setEditorMap(EditorMap* map);

  /* Updates list used within the view for battle scene information */
  void updateListScenes(QList<QString> list);
};
#endif // MAPMUSICVIEW_H
