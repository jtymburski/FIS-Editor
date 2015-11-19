/*******************************************************************************
 * Class Name: EventDialog
 * Date Created: November 18, 2015
 * Inheritance: QDialog
 * Description: The class is for managing the display and edit of the
 *              EditorEventSet class - within a QDialog widget.
 ******************************************************************************/
#include "Dialog/EventDialog.h"

/*============================================================================
 * CONSTRUCTORS / DESTRUCTORS
 *===========================================================================*/

/* Constructor function */
EventDialog::EventDialog(EditorEventSet* set, QWidget* parent)
{

}

/* Destructor function */
EventDialog::~EventDialog()
{

}

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
  
/* Creates the layout and widgets for this controller */
void EventDialog::createLayout()
{

}

/* Set layout data */
void EventDialog::setLayoutData()
{

}

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
  
/* Category changed */
//void categoryChanged(int index);

/* Edit conversation trigger */
void EventDialog::editConversation(Conversation* convo, bool is_option)
{

}

/* Select tile trigger */
void EventDialog::selectTileConvo()
{

}

/* Select tile trigger */
void EventDialog::selectTileMain()
{

}

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/

/* Returns the list of objects, used for event creation */
QVector<QString> EventDialog::getListItems()
{

}

/* Returns the list of objects, used for event creation */
QVector<QString> EventDialog::getListMaps()
{

}

/* Returns the list of objects, used for event creation */
QList<QString> EventDialog::getListSounds()
{

}

/* Returns the list of objects, used for event creation */
QVector<QString> EventDialog::getListSubmaps()
{

}

/* Returns the list of objects, used for event creation */
QVector<QString> EventDialog::getListThings()
{

}

/* Returns the event set */
EditorEventSet* EventDialog::getEventSet()
{

}

/* Returns views */
EventView* EventDialog::getViewEvent()
{

}

/* Returns views */
LockView* EventDialog::getViewLock()
{

}

/* Sets the list of objects, used for event creation */
void EventDialog::setListItems(QVector<QString> items)
{

}

/* Sets the list of objects, used for event creation */
void EventDialog::setListMaps(QVector<QString> maps)
{

}

/* Sets the list of objects, used for event creation */
void EventDialog::setListSounds(QList<QString> sounds)
{

}

/* Sets the list of objects, used for event creation */
void EventDialog::setListSubmaps(QVector<QString> sub_maps)
{

}

/* Sets the list of objects, used for event creation */
void EventDialog::setListThings(QVector<QString> things)
{

}

/* Sets the event set */
EditorEventSet* EventDialog::setEventSet(EditorEventSet* set)
{

}

/* Updates the event set */
void EventDialog::updateSet()
{

}

/* Update the selected tile for the thing */
void EventDialog::updateSelectedTile(int id, int x, int y)
{

}
