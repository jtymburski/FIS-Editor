/*******************************************************************************
 * Class Name: EditorSounds
 * Date Created: September 28, 2015
 * Inheritance: QWidget
 * Description: Sound and music handler within the Editor
 ******************************************************************************/
#include "Database/EditorSounds.h"
  
/* Constructor Function */
EditorSounds::EditorSounds(QWidget* parent) : QWidget(parent)
{
  createLayout();
  loadWorkingInfo();
}

/* Copy constructor */
EditorSounds::EditorSounds(const EditorSounds &source) : EditorSounds()
{
  copySelf(source);
}

/* Destructor function */
EditorSounds::~EditorSounds()
{
  // TODO
}

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
  
/* Copy function, to be called by a copy or equal operator constructor */
void EditorSounds::copySelf(const EditorSounds &source)
{
  // TODO
}

/* Creates interface layout */
void EditorSounds::createLayout()
{
  /* Layout */
  QGridLayout* layout = new QGridLayout(this);
  //layout->setColumnStretch(5, 1);
  //layout->setRowMinimumHeight(7, 15);
  //layout->setRowStretch(5, 1);

  /* Testing */
  QLabel* lbl_test = new QLabel("TEST", this);
  layout->addWidget(lbl_test, 0, 0, Qt::AlignCenter);
}

/* Loads working info into UI objects */
void EditorSounds::loadWorkingInfo()
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
void EditorSounds::load(XmlData data, int index)
{

}

/* Resets the working set trigger */
void EditorSounds::resetWorking()
{

}

/* Saves the object data */
void EditorSounds::save(FileHandler* fh, bool game_only)
{

}

/* Saves the working set trigger */
void EditorSounds::saveWorking()
{

}

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
  
/* The copy operator */
EditorSounds& EditorSounds::operator= (const EditorSounds &source)
{

}
