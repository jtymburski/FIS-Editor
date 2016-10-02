/*******************************************************************************
 * Class Name: EventSetView
 * Date Created: November 22, 2015
 * Inheritance: QFrame
 * Description: The class is for managing the display of the EditorEventSet
 *              class. It includes a trigger to edit it from outside callers.
 ******************************************************************************/
#ifndef EVENTSETVIEW_H
#define EVENTSETVIEW_H

#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include "Database/EditorEventSet.h"
#include "EditorEnumDb.h"

class EventSetView : public QFrame
{
  Q_OBJECT
public:
  /* Constructor function */
  EventSetView(EditorEventSet* event_set = nullptr, QWidget* parent = nullptr);

  /* Destructor function */
  ~EventSetView();

private:
  /* Button for editing the set */
  QPushButton* btn_edit;

  /* The connected event set */
  EditorEventSet* event_set;

  /* Labels for data */
  QLabel* lbl_lock;
  QLabel* lbl_lock_event;
  QLabel* lbl_unlock_events;

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Creates the layout and widgets for this controller */
  void createLayout();

  /* Set layout data */
  void setLayoutData();

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Edit set trigger */
  void editSet(EditorEventSet* set);

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* Button control triggers */
  void buttonEdit();

  /* Event updated trigger */
  void eventUpdated();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the event set */
  EditorEventSet* getEventSet();

  /* Sets the event set */
  void setEventSet(EditorEventSet* set);
};

#endif // EVENTSETVIEW_H
