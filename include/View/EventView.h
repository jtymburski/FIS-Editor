/*******************************************************************************
 * Class Name: EventView
 * Date Created: February 10, 2015
 * Inheritance: QFrame
 * Description: The class is for managing the display and edit of the
 *              EditorEvent class.
 ******************************************************************************/
#ifndef EVENTVIEW_H
#define EVENTVIEW_H

#include <QComboBox>
#include <QFrame>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QStackedWidget>
#include <QTextEdit>
#include <QTreeWidget>
#include <QVBoxLayout>

#include "Database/EditorEvent.h"

class EventView : public QFrame
{
  Q_OBJECT
public:
  /* Constructor function */
  EventView(EditorEvent* event, QWidget* parent = NULL);

  /* Destructor function */
  ~EventView();

private:
  /* The category of the event view */
  QComboBox* combo_category;

  /* The conversation event - convo tree */
  QTreeWidget* convo_tree;

  /* The connected event - original and working version */
  EditorEvent* event;

  /* Give item event control widgets */
  QSpinBox* item_count;
  QLineEdit* item_name;

  /* Switch maps event - map name view box */
  QLineEdit* map_name;

  /* Notification event text edit box */
  QTextEdit* notification_edit;

  /* Teleport event - view text boxes for map location and thing */
  QLineEdit* tele_map;
  QLineEdit* tele_thing;

  /* View stack for sub-widgets */
  QStackedWidget* view_stack;

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

  /* The change map button press */
  void changeMapPressed();

  /* The give item event slot changes */
  void giveCountChanged(int index);
  void giveItemPressed();

  /* The notification event text changes */
  void notificationTextChanged();

  /* The teleport event button presses */
  void teleportMapPressed();
  void teleportThingPressed();

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
