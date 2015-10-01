/*******************************************************************************
 * Class Name: EditorSoundDb
 * Date Created: October 1, 2015
 * Inheritance: QWidget
 * Description: Sound and music handler within the Editor. Controls the data
 *              and the overall view
 ******************************************************************************/
#include "Database/EditorSoundDb.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor Function */
EditorSoundDb::EditorSoundDb(QWidget* parent) : QWidget(parent)
{
  createLayout();
  loadWorkingInfo();
}

/* Copy constructor */
EditorSoundDb::EditorSoundDb(const EditorSoundDb &source) : EditorSoundDb()
{
  copySelf(source);
}

/* Destructor function */
EditorSoundDb::~EditorSoundDb()
{
  // TODO
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
  
/* Copy function, to be called by a copy or equal operator constructor */
void EditorSoundDb::copySelf(const EditorSoundDb &source)
{
  // TODO
}

/* Creates interface layout */
void EditorSoundDb::createLayout()
{
  /* Layout */
  QGridLayout* layout = new QGridLayout(this);
  //layout->setColumnStretch(9, 1);
  //layout->setRowMinimumHeight(7, 15);
  layout->setRowStretch(3, 1);

  /* Fonts */
  QFont bold_font;
  bold_font.setBold(true);

  /* Sound Labels */
  QLabel* lbl_s_title = new QLabel("Sounds", this);
  lbl_s_title->setFont(bold_font);
  layout->addWidget(lbl_s_title, 0, 0, 1, 4, Qt::AlignHCenter);
  QLabel* lbl_s_reserve = new QLabel("Reserved", this);
  layout->addWidget(lbl_s_reserve, 1, 0, 1, 2, Qt::AlignHCenter);
  QLabel* lbl_s_custom = new QLabel("Custom", this);
  layout->addWidget(lbl_s_custom, 1, 2, 1, 2, Qt::AlignHCenter);

  /* Sound Lists */
  QListWidget* list_s_reserve = new QListWidget(this);
  layout->addWidget(list_s_reserve, 2, 0, 1, 2);
  QListWidget* list_s_custom = new QListWidget(this);
  layout->addWidget(list_s_custom, 2, 2, 1, 2);

  /* Sound Buttons */
  QPushButton* btn_s_custom_add = new QPushButton("Add", this);
  layout->addWidget(btn_s_custom_add, 3, 2);
  QPushButton* btn_s_custom_rem = new QPushButton("Remove", this);
  layout->addWidget(btn_s_custom_rem, 3, 3);

  /* Separating Vertical Frame */
  QFrame* frm_vertical = new QFrame(this);
  frm_vertical->setFrameShape(QFrame::VLine);
  frm_vertical->setLineWidth(1);
  layout->addWidget(frm_vertical, 0, 4, 4, 1);

  /* Music Labels */
  QLabel* lbl_m_title = new QLabel("Music", this);
  lbl_m_title->setFont(bold_font);
  layout->addWidget(lbl_m_title, 0, 5, 1, 4, Qt::AlignHCenter);
  QLabel* lbl_m_reserve = new QLabel("Reserved", this);
  layout->addWidget(lbl_m_reserve, 1, 5, 1, 2, Qt::AlignHCenter);
  QLabel* lbl_m_custom = new QLabel("Custom", this);
  layout->addWidget(lbl_m_custom, 1, 7, 1, 2, Qt::AlignHCenter);

  /* Music Lists */
  QListWidget* list_m_reserve = new QListWidget(this);
  layout->addWidget(list_m_reserve, 2, 5, 1, 2);
  QListWidget* list_m_custom = new QListWidget(this);
  layout->addWidget(list_m_custom, 2, 7, 1, 2);

  /* Music Buttons */
  QPushButton* btn_m_custom_add = new QPushButton("Add", this);
  layout->addWidget(btn_m_custom_add, 3, 7);
  QPushButton* btn_m_custom_rem = new QPushButton("Remove", this);
  layout->addWidget(btn_m_custom_rem, 3, 8);

  /* Separating Horizontal Frame */
  QFrame* frm_horizontal = new QFrame(this);
  frm_horizontal->setFrameShape(QFrame::HLine);
  frm_horizontal->setLineWidth(1);
  layout->addWidget(frm_horizontal, 4, 0, 1, 9);

  /* Player - TODO execute properly */
  SoundView* view_sound = new SoundView(this);
  layout->addWidget(view_sound, 5, 0, 1, 9, Qt::AlignHCenter);
}

/* Loads working info into UI objects */
void EditorSoundDb::loadWorkingInfo()
{

}

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/
  
/* Button Triggers */
//void btnItemAdd();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Loads the object data */
void EditorSoundDb::load(XmlData data, int index)
{

}

/* Resets the working set trigger */
void EditorSoundDb::resetWorking()
{

}

/* Saves the object data */
void EditorSoundDb::save(FileHandler* fh, bool game_only)
{

}

/* Saves the working set trigger */
void EditorSoundDb::saveWorking()
{

}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
  
/* The copy operator */
EditorSoundDb& EditorSoundDb::operator= (const EditorSoundDb &source)
{

}
