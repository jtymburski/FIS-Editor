/*******************************************************************************
 * Class Name: NodeDialog
 * Date Created: April 12, 2015
 * Inheritance: QDialog
 * Description: The NPC node dialog, which handles editing a singular node in
 *              the NPC set. Used by MapDatabase with NPC node view controller.
 ******************************************************************************/
#ifndef NODEDIALOG_H
#define NODEDIALOG_H

#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>

#include "Database/EditorNPCPath.h"
#include "View/EventView.h"

class NodeDialog : public QDialog
{
  Q_OBJECT
public:
  /* Constructor Function */
  NodeDialog(EditorNPCPath* path, int node_num, QWidget* parent = NULL);

  /* Destructor Function */
  ~NodeDialog();

private:
  /* Event view - future */
  EventView* event_view;

  /* Path reference */
  int path_node;
  EditorNPCPath* path_original;
  EditorNPCPath* path_working;

  /* Radio buttons for control of direction of movement */
  QRadioButton* radio_xy;
  QRadioButton* radio_yx;

  /* The delay of the node */
  QSpinBox* spin_delay;

  /*------------------- Constants -----------------------*/
  //const static int kSCENE_SIZE; /* Render size of the scene */

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Creates the dialog */
  void createDialog();

  /* Fill with data */
  void updateData();

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Custom close event */
  void closeEvent(QCloseEvent*);

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Successful finished trigger */
  void success();

/*============================================================================
 * PUBLIC SLOT FUNCTIONS
 *===========================================================================*/
public slots:
  /* Button triggers */
  void buttonCancel();
  void buttonOk();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns the event view in the pop-up */
  //EventView* getEventView();
};

#endif // NODEDIALOG_H
