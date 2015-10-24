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

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/
  
/*
 * Description: Constructor function. Creates the music view, which is the
 *              side bar within the left map database. Controls sub-map music
 *              and weather audio selection.
 *
 * Inputs: QWidget* parent - the parent widget
 */
MapMusicView::MapMusicView(QWidget* parent) : QWidget(parent)
{
  /* Initialize variables */
  editor_map = nullptr;

  /* Create the layout */
  createLayout();
}

/*
 * Description: Destructor function
 */
MapMusicView::~MapMusicView()
{
  setEditorMap(nullptr);
}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
  
/*
 * Description: Creates the dialog layout with QT functional widgets.
 *
 * Inputs: none
 * Output: none
 */
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
  connect(combo_weather, SIGNAL(currentIndexChanged(QString)),
          this, SLOT(changedWeather(QString)));
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
  connect(list_avail, SIGNAL(currentRowChanged(int)),
          this, SLOT(changedListUpper(int)));
  layout->addWidget(list_avail);

  /* Selection buttons */
  QHBoxLayout* layout_btns = new QHBoxLayout();
  btn_add = new QPushButton("Add", this);
  btn_add->setDisabled(true);
  btn_add->setDefault(true);
  connect(btn_add, SIGNAL(clicked()),
          this, SLOT(buttonAdd()));
  layout_btns->addWidget(btn_add);
  btn_rem = new QPushButton("Remove", this);
  btn_rem->setDisabled(true);
  connect(btn_rem, SIGNAL(clicked()),
          this, SLOT(buttonRemove()));
  layout_btns->addWidget(btn_rem);
  layout->addLayout(layout_btns);

  /* Selected sub-map music list */
  QLabel* lbl_used = new QLabel("Sub-Map Music:", this);
  layout->addWidget(lbl_used);
  list_used = new QListWidget(this);
  connect(list_used, SIGNAL(currentRowChanged(int)),
          this, SLOT(changedListLower(int)));
  layout->addWidget(list_used);
}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

/*
 * Description: Button slot on the add button. Moves a selected ID from the
 *              available sound list to add to sub-map music stack.
 *
 * Inputs: none
 * Output: none
 */
void MapMusicView::buttonAdd()
{
  if(editor_map != nullptr && editor_map->getCurrentMap() != nullptr &&
     list_avail->currentRow() >= 0)
  {
    QString curr_item = list_avail->currentItem()->text();
    QStringList str_split = curr_item.split(':');
    if(str_split.size() >= 2)
    {
      editor_map->getCurrentMap()->music.push_back(str_split.front().toInt());
      updateData();
    }
  }
}

/*
 * Description: Button slot on the remove button. Removes a selected ID from the
 *              sub-music stack.
 *
 * Inputs: none
 * Output: none
 */
void MapMusicView::buttonRemove()
{
  if(editor_map != nullptr && editor_map->getCurrentMap() != nullptr &&
     list_used->currentRow() >= 0)
  {
    QString curr_item = list_used->currentItem()->text();
    QStringList str_split = curr_item.split(':');
    if(str_split.size() >= 2)
    {
      int id = str_split.front().toInt();
      int index = editor_map->getCurrentMap()->music.indexOf(id);
      if(index >= 0)
      {
        editor_map->getCurrentMap()->music.remove(index);
        updateData();
      }
    }
  }
}

/*
 * Description: Slot triggered on lower list changed selection. Enables or
 *              disables the remove button.
 *
 * Inputs: int row - the new row index in the list
 * Output: none
 */
void MapMusicView::changedListLower(int row)
{
  if(row >= 0)
    btn_rem->setEnabled(true);
  else
    btn_rem->setDisabled(true);
}

/*
 * Description: Slot triggered on upper list changed selection. Enables or
 *              disables the remove button.
 *
 * Inputs: int row - the new row index in the list
 * Output: none
 */
void MapMusicView::changedListUpper(int row)
{
  if(row >= 0)
    btn_add->setEnabled(true);
  else
    btn_add->setDisabled(true);
}

/*
 * Description: Slot triggered on changed selection of the sub-map weather
 *              audio in the drop-down. Updates the current sub-map with the
 *              new weather ID.
 *
 * Inputs: QString text - the new selected entry text
 * Output: none
 */
void MapMusicView::changedWeather(const QString & text)
{
  if(editor_map != nullptr && editor_map->getCurrentMap() != nullptr)
  {
    QStringList str_list = text.split(':');
    if(str_list.size() >= 2)
      editor_map->getCurrentMap()->weather = str_list.front().toInt();
    else
      editor_map->getCurrentMap()->weather = -1;
  }
}

/*
 * Description: Updates the data in the widgets. CreateLayout() must be called
 *              prior.
 *
 * Inputs: none
 * Output: none
 */
void MapMusicView::updateData()
{
  bool valid_map = (editor_map != nullptr &&
                    editor_map->getCurrentMap() != nullptr);

  /* Weather data - find index */
  int weather_index = -1;
  if(valid_map && music_list.size() > 0 &&
     editor_map->getCurrentMap()->weather >= 0)
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
    if(weather_index < 0)
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
  if(valid_map && music_list.size() > 0)
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
        map->music.remove(i);
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
    if(list_avail->count() > 0 && index_top >= 0)
    {
      if(index_top < list_avail->count())
        list_avail->setCurrentRow(index_top);
      else
        list_avail->setCurrentRow(list_avail->count() - 1);
    }
    else
    {
      btn_add->setDisabled(true);
    }
    if(list_used->count() > 0 && index_bot >= 0)
    {
      if(index_bot < list_used->count())
        list_used->setCurrentRow(index_bot);
      else
        list_used->setCurrentRow(list_used->count() - 1);
    }
    else
    {
      btn_rem->setDisabled(true);
    }
  }

  /* Widget Enables */
  if(valid_map)
  {
    combo_weather->setEnabled(true);
    list_avail->setEnabled(true);
    list_used->setEnabled(true);
  }
  else
  {
    combo_weather->setDisabled(true);
    list_avail->setDisabled(true);
    list_used->setDisabled(true);
  }
}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/*
 * Description: Returns the stack of music reference listing stored within the
 *              view.
 *
 * Inputs: none
 * Output: QList<QString> - the music reference list
 */
QList<QString> MapMusicView::getDataMusic()
{
  return music_list;
}

/*
 * Description: Returns the editor map, which contains all the sub-map music
 *              data.
 *
 * Inputs: none
 * Output: EditorMap* - pointer to the editor map. NULL if none set.
 */
EditorMap* MapMusicView::getEditorMap()
{
  return editor_map;
}

/*
 * Description: Sets the control editor map. This contains the things, sprites
 *              and all data relevant to display.
 *
 * Inputs: EditorMap* map - the map to use for control
 * Output: none
 */
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
}

/*
 * Description: Sets the list of music, used throughout the widget
 *
 * Inputs: QList<QString> list - list of all music resources available
 * Output: none
 */
void MapMusicView::updateListMusic(QList<QString> list)
{
  music_list = list;
  updateData();
}
