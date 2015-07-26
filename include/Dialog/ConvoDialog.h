/*******************************************************************************
 * Class Name: ConvoDialog
 * Date Created: February 15, 2015
 * Inheritance: QDialog
 * Description: The conversation dialog, which handles editing a singular 
 *              conversation node. Used primarily by ThingDialog and all
 *              children.
 ******************************************************************************/
#ifndef CONVODIALOG_H
#define CONVODIALOG_H

#include <QComboBox>
#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>

#include "Game/EventHandler.h"
#include "View/EventView.h"

class ConvoDialog : public QDialog
{
  Q_OBJECT
public:
  /* Constructor Function */
  ConvoDialog(Conversation* edit_convo = NULL, bool is_option = false, 
              QWidget* parent = NULL);

  /* Destructor Function */
  ~ConvoDialog();

private:
  /* Conversation reference */
  Conversation convo_working;
  Conversation* convo_original;

  /* Event view and control */
  EditorEvent* event_ctrl;
  EventView* event_view;
  
  /* The list of objects used in possible events */
  QVector<QString> list_things;
  
  /* Conversation text edit box */
  QTextEdit* text_box;

  /* Thing selection combo box */
  QComboBox* thing_combo;

  /*------------------- Constants -----------------------*/
  //const static int kSCENE_SIZE; /* Render size of the scene */

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Creates the dialog */
  void createDialog(bool is_option = false);

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
  EventView* getEventView();
  
  /* Returns the list of objects, used for event creation */
  QVector<QString> getListThings();

  /* Sets the list of objects, used for event creation */
  void setListThings(QVector<QString> things, bool is_thing = true);
};

#endif // CONVODIALOG_H
