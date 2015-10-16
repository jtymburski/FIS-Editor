/*******************************************************************************
 * Class Name: MapMusicView
 * Date Created: October 15, 2015
 * Inheritance: QWidget
 * Description: The side toolbar in the map database that gives access to the
 *              map music for each sub map. This also includes provisions for
 *              weather manipulation.
 ******************************************************************************/
#include "View/MapMusicView.h"
#include <QDebug>

// TODO: Need to have this update anytime the music list is modified.
// My first idea is each time it shows but there is the case where it will be
// visible and the dev goes to game database and switches to music, adds a
// bunch and then returns to map in gameDB which won't have the updates...
// Review


/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/
  
/* Constructor Function */
MapMusicView::MapMusicView(QWidget* parent) : QWidget(parent)
{
  /* Initialize variables */
  editor_map = nullptr;

  /* Create the layout */
  createLayout();
}

/* Destructor function */
MapMusicView::~MapMusicView()
{
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
  
/* Creates the layout - only called on initial construction */
void MapMusicView::createLayout()
{
  /* Layout */
  QVBoxLayout* layout = new QVBoxLayout(this);

  /* Horizontal separator */
  QFrame* frm_horiz1 = new QFrame(this);
  frm_horiz1->setFrameShape(QFrame::HLine);
  frm_horiz1->setLineWidth(1);
  layout->addWidget(frm_horiz1);

  /* Weather dropdown selection */
  QHBoxLayout* layout_weather = new QHBoxLayout();
  QLabel* lbl_weather = new QLabel("Weather", this);
  layout_weather->addWidget(lbl_weather);
  combo_weather = new QComboBox(this);
  layout_weather->addWidget(combo_weather, 1);
  layout->addLayout(layout_weather);

  /* Horizontal separator */
  QFrame* frm_horiz2 = new QFrame(this);
  frm_horiz2->setFrameShape(QFrame::HLine);
  frm_horiz2->setLineWidth(1);
  layout->addWidget(frm_horiz2);

  /* Available sub-map music list */
  QLabel* lbl_avail = new QLabel("Available Music:", this);
  layout->addWidget(lbl_avail);
  list_avail = new QListWidget(this);
  layout->addWidget(list_avail);

  /* Selection buttons */
  QHBoxLayout* layout_btns = new QHBoxLayout();
  btn_add = new QPushButton("Add", this);
  btn_add->setDisabled(true);
  layout_btns->addWidget(btn_add);
  btn_rem = new QPushButton("Remove", this);
  btn_rem->setDisabled(true);
  layout_btns->addWidget(btn_rem);
  layout->addLayout(layout_btns);

  /* Selected sub-map music list */
  QLabel* lbl_used = new QLabel("Sub-Map Music:", this);
  layout->addWidget(lbl_used);
  list_used = new QListWidget(this);
  layout->addWidget(list_used);
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

/* Refreshes the info in the lower half of the widget */
void MapMusicView::updateData()
{
  qDebug() << "Updating Data";

  /* Weather data - find index */
  int weather_index = -1;
  if(editor_map != nullptr && editor_map->getCurrentMap() != nullptr &&
     music_list.size() > 0 && editor_map->getCurrentMap()->weather >= 0)
  {
    /* Try and find index */
    for(int i = 0; i < music_list.size(); i++)
    {
      QStringList str_split = music_list[i].split(':');
      if(str_split.size() >= 2)
        if(str_split.front().toInt() == editor_map->getCurrentMap()->weather)
          weather_index = i;
    }

    /* If not found, reset weather back to default */
    editor_map->getCurrentMap()->weather = -1;
  }

  /* Weather combo box */
  combo_weather->blockSignals(true);
  combo_weather->clear();
  combo_weather->addItems(QStringList(music_list));
  if(weather_index >= 0)
    combo_weather->setCurrentIndex(weather_index);
  combo_weather->blockSignals(false);

  /* Map Music data - processing and widget management */
  int index_top = list_avail->currentRow();
  list_avail->clear();
  int index_bot = list_used->currentRow();
  list_used->clear();
  if(editor_map != nullptr && editor_map->getCurrentMap() != nullptr &&
     music_list.size() > 0)
  {
    QVector<bool> valid_list;
    SubMapInfo* map = editor_map->getCurrentMap();

    /* Find valid IDs */
    for(int i = 0; i < map->music.size(); i++)
    {
      if(map->music[i] >= 0 &&
         EditorHelpers::indexOf(map->music[i], music_list) >= 0)
      {
        valid_list.push_back(true);
      }
      else
      {
        valid_list.push_back(false);
      }
    }

    /* Clean out invalid IDs */
    for(int i = valid_list.size() - 1; i >= 0; i--)
    {
      if(!valid_list[i])
        map->music.removeAt(i);
    }

    /* Proceed to update views */
    for(int i = 0; i < music_list.size(); i++)
    {
      QStringList str_split = music_list[i].split(':');
      if(str_split.size() >= 2)
      {
        if(map->music.contains(str_split.front().toInt()))
          list_used->addItem(music_list[i]);
        else
          list_avail->addItem(music_list[i]);
      }
    }

    /* Restore indexes */
    if(index_top >= 0)
    {
      if(index_top < list_avail->count())
        list_avail->setCurrentRow(index_top);
      else
        list_avail->setCurrentRow(list_avail->count() - 1);
    }
    if(index_bot >= 0)
    {
      if(index_bot < list_used->count())
        list_used->setCurrentRow(index_top);
      else
        list_used->setCurrentRow(list_used->count() - 1);
    }
  }
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
  
/* Returns data, as required */
QList<QString> MapMusicView::getDataMusic()
{
  return music_list;
}

/* Gets the editor map */
EditorMap* MapMusicView::getEditorMap()
{
  return editor_map;
}

/* Sets the editor map, which contains the data needed */
void MapMusicView::setEditorMap(EditorMap* map)
{
  /* If existing editor map is not NULL, undo */
  if(editor_map != nullptr)
  {
    disconnect(editor_map, SIGNAL(activeSubChanged()),
               this, SLOT(updateData()));
  }

  /* Set map */
  editor_map = map;

  /* If new map is not NULL, reconnect */
  if(editor_map != nullptr)
  {
    connect(editor_map, SIGNAL(activeSubChanged()),
            this, SLOT(updateData()));
  }

  /* Update data */
  updateData();
  qDebug() << "Updating Editor Map";
}

/* Updates list in thing dialog, needed for event control */
void MapMusicView::updateListMusic(QList<QString> list)
{
  music_list = list;
  qDebug() << "Updating Music List";
  updateData();
}
