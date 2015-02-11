/*******************************************************************************
 * Class Name: EventView
 * Date Created: February 10, 2015
 * Inheritance: QWidget
 * Description: The class is for managing the display and edit of the
 *              EditorEvent class.
 ******************************************************************************/
#ifndef EVENTVIEW_H
#define EVENTVIEW_H

#include <QComboBox>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>

#include "Database/EditorEvent.h"

class EventView : public QWidget
{
  Q_OBJECT
public:
  /* Constructor function */
  EventView(EditorEvent* event, QWidget* parent = NULL);

  /* Destructor function */
  ~EventView();

private:
  /* The connected event - original and working version */
  EditorEvent* event;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Creates the layout and widgets for this controller */
  void createLayout();

  /* Set layout data */
  void setLayoutData();

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* Category changed */
  void categoryChanged(int index);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the event */
  EditorEvent* getEvent();

  /* Sets the editor event */
  void setEvent(EditorEvent* event);
};

#endif // EVENTVIEW_H
