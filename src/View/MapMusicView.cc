/*******************************************************************************
 * Class Name: MapMusicView
 * Date Created: October 15, 2015
 * Inheritance: QWidget
 * Description: The side toolbar in the map database that gives access to the
 *              map music for each sub map. This also includes provisions for
 *              weather manipulation.
 ******************************************************************************/
#include "View/MapMusicView.h"

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
  layout_btns->addWidget(btn_add);
  btn_rem = new QPushButton("Remove", this);
  layout_btns->addWidget(btn_rem);
  layout->addLayout(layout_btns);

  /* Selected sub-map music list */
  QLabel* lbl_used = new QLabel("Sub-Map Music:", this);
  layout->addWidget(lbl_used);
  list_used = new QListWidget(this);
  layout->addWidget(list_used);
}

/* Refreshes the info in the lower half of the widget */
void MapMusicView::updateData()
{

}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
  
/* Gets the editor map */
EditorMap* MapMusicView::getEditorMap()
{
  return editor_map;
}

/* Sets the editor map, which contains the data needed */
void MapMusicView::setEditorMap(EditorMap* map)
{
  editor_map = map;
  updateData();
}

/* Updates list in thing dialog, needed for event control */
void MapMusicView::updateListMusic(QList<QString> list)
{
  music_list = list;
  updateData();
}
