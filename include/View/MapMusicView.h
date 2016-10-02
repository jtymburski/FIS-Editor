/*******************************************************************************
 * Class Name: MapMusicView
 * Date Created: October 15, 2015
 * Inheritance: QWidget
 * Description: The side toolbar in the map database that gives access to the
 *              map music for each sub map. This also includes provisions for
 *              weather manipulation.
 ******************************************************************************/
#ifndef MAPMUSICVIEW_H
#define MAPMUSICVIEW_H

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "Database/EditorMap.h"

class MapMusicView : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  MapMusicView(QWidget* parent = nullptr);

  /* Destructor function */
  ~MapMusicView();

private:
  /* Button widgets */
  QPushButton* btn_add;
  QPushButton* btn_rem;

  /* Combo widgets */
  QComboBox* combo_weather;

  /* The current editing map */
  EditorMap* editor_map;

  /* List widgets */
  QListWidget* list_avail;
  QListWidget* list_used;

  /* Music information, for dropdown and selection list */
  QList<QString> music_list;

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
  void buttonAdd();
  void buttonRemove();

  /* Changed triggers in widgets */
  void changedListLower(int row);
  void changedListUpper(int row);
  void changedWeather(const QString & text);

  /* Refreshes the entire data set within the widget */
  void updateData();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns data, as required */
  QList<QString> getDataMusic();

  /* Gets the editor map */
  EditorMap* getEditorMap();

  /* Sets the editor map, which contains the data needed */
  void setEditorMap(EditorMap* map);

  /* Updates list in thing dialog, needed for event control */
  void updateListMusic(QList<QString> list);
};
#endif // MAPMUSICVIEW_H
