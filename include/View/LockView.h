/*******************************************************************************
 * Class Name: LockView
 * Date Created: November 17, 2015
 * Inheritance: QFrame
 * Description: The class is for managing the display and edit of the
 *              EditorLock class.
 ******************************************************************************/
#ifndef LOCKVIEW_H
#define LOCKVIEW_H

#include <QCheckBox>
#include <QComboBox>
#include <QFrame>
#include <QGridLayout>
#include <QLabel>
#include <QSpinBox>
#include <QStackedWidget>
#include <QStringList>
#include <QVBoxLayout>

#include "Database/EditorLock.h"
#include "EditorEnumDb.h"

class LockView : public QFrame
{
  Q_OBJECT
public:
  /* Constructor function */
  LockView(EditorLock* lock = nullptr, QWidget* parent = nullptr,
           bool view_only = false);

  /* Destructor function */
  ~LockView();

private:
  /* The category of the event view */
  QComboBox* combo_category;

  /* Have item lock control widgets */
  QCheckBox* item_consume;
  QSpinBox* item_count;
  QComboBox* item_name;
  QCheckBox* item_permanent;

  /* The list of objects used in possible events */
  QList<QString> list_items;

  /* The connected lock */
  EditorLock* lock;

  /* Trigger lock control widgets */
  QCheckBox* trigger_permanent;

  /* View only flag */
  bool view_only;

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
 * SIGNALS
 *===========================================================================*/
signals:
  /* Updated trigger */
  void updated();

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* Category changed */
  void categoryChanged(int index);

  /* The have item lock slot changes */
  void haveConsumeChanged(int state);
  void haveCountChanged(int index);
  void haveItemChanged(int index);
  void havePermanentChanged(int state);

  /* The trigger lock slot changes */
  void triggerPermanentChanged(int state);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the list of objects, used for event creation */
  QList<QString> getListItems();

  /* Returns the lock */
  EditorLock* getLock();

  /* Sets the list of objects, used for event creation */
  void setListItems(QList<QString> items);

  /* Sets the editor lock */
  void setLock(EditorLock* lock);

  /* Updates the lock */
  void updateLock();
};

#endif // LOCKVIEW_H
