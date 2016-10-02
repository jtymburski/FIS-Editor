/*******************************************************************************
 * Class Name: BattleSceneView
 * Date Created: January 22, 2016
 * Inheritance: QWidget
 * Description: The battle scene view and editor which handles the properly
 *              preview and edit capabilities to the scene.
 ******************************************************************************/
#ifndef BATTLESCENEVIEW_H
#define BATTLESCENEVIEW_H

#include <QComboBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QSpinBox>
#include <QWidget>

#include "Database/EditorBattleScene.h"
#include "Database/EditorSprite.h"

class BattleSceneView : public QWidget
{
  Q_OBJECT
public:
  /* Constructor Function */
  BattleSceneView(QWidget* parent = nullptr);

  /* Constructor function with Editor Battle Scene reference */
  BattleSceneView(EditorBattleScene* scene, QWidget* parent = nullptr);

  /* Copy constructor */
  BattleSceneView(const BattleSceneView &source);

  /* Destructor function */
  ~BattleSceneView();

private:
  /* Button Widgets */
  QPushButton* btn_lay_del;
  QPushButton* btn_lay_down;
  QPushButton* btn_lay_edit;
  QPushButton* btn_lay_path;
  QPushButton* btn_lay_up;

  /* Combo Box Widgets */
  QComboBox* combo_layover;
  QComboBox* combo_music;

  /* Line Edit Widgets */
  QLineEdit* edit_name;

  /* Label Widgets - just for displaying text */
  QLabel* lbl_background_view;
  QLabel* lbl_id_view;
  QLabel* lbl_lay_path_list;
  QLabel* lbl_preview_img;

  /* Lay Over Edit Information */
  EditorSprite* lay_edit_sprite;
  LayOver* lay_edit_struct;

  /* List Widgets */
  QListWidget* list_layover;

  /* Music information, for dropdown and selection list */
  QList<QString> music_list;

  /* Scene information */
  EditorBattleScene* scene_base;
  EditorBattleScene scene_curr;

  /* Scene preview */
  QPixmap scene_preview;

  /* Spin Box Widgets */
  QSpinBox* spin_lay_anim ;
  QSpinBox* spin_lay_velx;
  QSpinBox* spin_lay_vely;

  /* Sprite lays */
  EditorSprite sprite_background;
  QList<EditorSprite> sprite_midlays;
  QList<EditorSprite> sprite_overlays;
  QList<EditorSprite> sprite_underlays;

  /*------------------- Constants -----------------------*/
  //const static int kFADE_INTERVAL; /* The fade interval, in milliseconds */

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const BattleSceneView &source);

  /* Creates interface layout */
  void createLayout();

  /* Edits the layover selected index based on the selected index */
  void editLayOver(QString category, int index);

  /* Loads working info into UI objects */
  void loadWorkingInfo();

  /* Flag called everytime a change is made to the curr set */
  void setChanged();

  /* Update preview image */
  void updatePreview();

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Name changed within scene widget signal */
  void nameChange(QString);

/*============================================================================
 * PUBLIC SLOTS
 *===========================================================================*/
public slots:
  /* Button Triggers */
  void btnBackground();
  void btnLayAdd();
  void btnLayDelete();
  void btnLayDown();
  void btnLayEdit();
  void btnLayPath();
  void btnLayUp();
  void btnReset();
  void btnSave();

  /* Widget Change Triggers */
  void changedLayClass(const QString& category);
  void changedMusic(const QString& text);
  void changedName(const QString& name);

  /* List Index Widget Changes */
  void listLayOverChanged(int index);
  void listLayOverDClicked(QListWidgetItem*);

  /* Spin widget value changes */
  void spinLayAnimChanged(int value);
  void spinLayVelXChanged(int value);
  void spinLayVelYChanged(int value);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Returns data, as required */
  QList<QString> getListMusic();

  /* Returns the base scene that is being edited */
  EditorBattleScene* getScene();

  /* Returns if the battle scene information has been edited but not saved */
  bool isChanged();

  /* Resets the working set trigger */
  void resetWorking();

  /* Saves the working set trigger */
  void saveWorking();

  /* Sets the battle scene information */
  void setScene(EditorBattleScene* scene);

  /* Updates list in thing dialog, needed for event control */
  void updateListMusic(QList<QString> list);

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  /* The copy operator */
  BattleSceneView& operator= (const BattleSceneView &source);
};

#endif // BATTLESCENEVIEW_H
