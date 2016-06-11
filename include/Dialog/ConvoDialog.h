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
#include <QTextBlock>
#include <QTextEdit>

#include "Game/EventSet.h"
#include "View/EventView.h"

class ConvoDialog : public QDialog
{
  Q_OBJECT
public:
  /* Constructor Function */
  ConvoDialog(Conversation* edit_convo = NULL, bool is_option = false, 
              EventClassifier limiter = EventClassifier::NOEVENT,
              QWidget* parent = NULL);

  /* Destructor Function */
  ~ConvoDialog();

private:
  /* Buttons for text manipulation */
  QPushButton* btn_brush;
  QPushButton* btn_bold;
  QPushButton* btn_italic;
  //QPushButton* btn_thing;
  QPushButton* btn_underline;

  /* Conversation reference */
  Conversation convo_working;
  Conversation* convo_original;

  /* Event view and control */
  EditorEvent* event_ctrl;
  EventView* event_view;
  
  /* The list of objects used in possible events */
  QList<QString> list_things;
  
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
  void createDialog(bool is_option = false,
                    EventClassifier limiter = EventClassifier::NOEVENT);

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

  /* Text manipulator triggers */
  void textBtnBrush();
  void textBtnBold();
  void textBtnItalic();
  void textBtnItem();
  void textBtnPreview();
  void textBtnThing();
  void textBtnUnderline();
  void textSelected(bool yes);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/ 
public:
  /* Returns the event view in the pop-up */
  EventView* getEventView();
  
  /* Returns the list of objects, used for event creation */
  QList<QString> getListMapThings();

  /* Sets the list of objects, used for event creation */
  void setListMapThings(QList<QString> things,
                        QList<QPair<QString,QString>> ios,
                        QList<QString> items, QList<QString> persons,
                        QList<QString> npcs, bool is_thing = true);

};

#endif // CONVODIALOG_H
