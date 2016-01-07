/*******************************************************************************
 * Class Name: EditorNPCPath
 * Date Created: March 29, 2015
 * Inheritance: QGraphicsItem
 * Description: A graphics object which represents the path of a map npc.
 ******************************************************************************/
#ifndef EDITORNPCPATH_H
#define EDITORNPCPATH_H

#include <QColor>
#include <QGraphicsObject>
#include <QList>
#include <QPainter>
#include <QRectF>

#include "EditorEnumDb.h"
#include "EditorHelpers.h"
#include "FileHandler.h"
#include "Game/Map/MapNPC.h"

/* Editor NPC Path Class */
class EditorNPCPath : public QGraphicsObject
{
  Q_OBJECT
public:
  /* Constructor Function */
  EditorNPCPath(int x = -1, int y = -1, int delay = 0, bool xy_flip = false);

  /* Copy constructor function */
  EditorNPCPath(const EditorNPCPath &source);

  /* Destructor Function */
  virtual ~EditorNPCPath();

  /* Control functions */
  enum { Type_PathRender = UserType + 2};
  int type() const { return Type_PathRender; }

  /* Hover area enum to indicate where passed location is */
  enum HoverState {NO_HOVER, GENERAL, ON_NODE, ON_PATH};

private:
  /* Color of path */
  int color_a;
  int color_b;
  int color_g;
  QList<QColor> color_presets;
  int color_r;

  /* Is the path hovered? */
  Path hover_node;
  bool hover_used;
  bool hovered;

  /* Indexes for handle control */
  int index_move;
  int index_select;

  /* Force interaction when within range? */
  bool interact;

  /* Old move node, prior to move command */
  Path move_node;

  /* List of path nodes */
  QList<Path> nodes;

  /* Start node */
  Path start_node;

  /* Sets the path state */
  MapNPC::NodeState state;

  /* Tracking setpoints */
  int track_dist_max;
  int track_dist_min;
  int track_dist_run;

  /* Tracking properties */
  TrackingState tracking;

  /* Visibility control */
  bool visible_by_control;
  bool visible_by_edit;

  /*------------------- Constants -----------------------*/
  const static uint8_t kBORDER_W; /* Border width around lines and rects */
  const static uint8_t kCOLOR_ALPHA; /* Default color alpha */
  const static float kHOVER_ALPHA; /* The ratio for hover alpha mod */
  const static uint8_t kLINE_W; /* Line width (without border) */
  const static int kNODE_START; /* Use start node for painting node */
  const static uint8_t kRECT_W; /* Rect width (without border) */

/*============================================================================
 * PRIVATE FUNCTIONS
 *===========================================================================*/
private:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorNPCPath &source);

  /* Get hover color */
  QColor getHoverColor(bool hover_node = false);

  /* Corner paint function */
  void paintCorner(QPainter* painter, int x, int y,
                   Direction enter, Direction exit, QColor color);

  /* Line paint function */
  void paintLine(QPainter* painter, int x1, int y1, int x2, int y2,
                 QColor color);
  void paintLines(QPainter* painter, Path* curr, Path* next, QColor color);

  /* Node paint function */
  void paintNode(QPainter* painter, Path* prev, Path* curr, Path* next,
                 QColor color, int node_num = -2);

  /* Unsets all indexes modifying the path structure */
  void unsetAllIndexes();

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Hover control */
  void hoverEnterEvent(QGraphicsSceneHoverEvent*);
  void hoverLeaveEvent(QGraphicsSceneHoverEvent*);

/*============================================================================
 * SIGNALS
 *===========================================================================*/
signals:
  /* Triggers if the hover event was fired */
  void hoverInit(EditorNPCPath* path);

  /* Triggers if the path was changed */
  void pathChanged();

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Appends node to the end of the path */
  bool appendNode(int x, int y, int delay = 0, bool xy_flip = false);

  /* Necessary function for returning the bounding rectangle - virtual */
  QRectF boundingRect() const;

  /* Checks all nodes to see if they are in map range */
  void checkNodes(int x, int y, int w, int h);

  /* Delete nodes from list */
  bool deleteNode(int index);
  void deleteNodes();

  /* Edits the node at the index */
  bool editNode(int index, int x, int y, int delay = -1, bool xy_flip = false);

  /* Edits the start node */
  void editStartNode(int delay, bool xy_flip);

  /* Returns color of the path */
  QColor getColor();
  QColor getColorPreset(int index);
  int getColorPresetCount();

  /* Returns the hover state */
  HoverState getHoverState(int &index, int hx = -1, int hy = -1);

  /* Gets the index of actions that can be done on the path */
  int getIndexMove();
  int getIndexSelect();

  /* Returns the path node or node information at the selected index */
  Path* getNode(int index);
  int getNodeDelay(int index);
  QString getNodeStr(int index);
  int getNodeX(int index);
  bool getNodeXYFlip(int index);
  int getNodeY(int index);

  /* Returns the set of path nodes */
  QList<Path> getNodes();

  /* Returns the start node */
  Path getStartNode();

  /* Returns the state of the path */
  MapNPC::NodeState getState();

  /* Returns tracking distance setpoints */
  int getTrackDistMax();
  int getTrackDistMin();
  int getTrackDistRun();

  /* Returns the tracking state of the npc on the path */
  TrackingState getTracking();

  /* Insert nodes at location */
  bool insertNodeAfter(int index, int x, int y, int delay = 0,
                       bool xy_flip = false);
  bool insertNodeBefore(int index, int x, int y, int delay = 0,
                        bool xy_flip = false);

  /* Returns if the npc on path will force interaction */
  bool isForcedInteraction();

  /* Returns if the path is hovered */
  bool isHovered();

  /* Returns visibility status */
  bool isVisibleControl();
  bool isVisibleEdit();

  /* Loads the path data */
  void load(XmlData data, int index);

  /* Painting function for Path Wrapper - virtual */
  void paint(QPainter* painter, const QStyleOptionGraphicsItem*,
             QWidget*);

  /* Saves the path data */
  void save(FileHandler* fh, bool game_only = false);

  /* Sets the color of the path */
  bool setColor(int r, int g, int b, int a = kCOLOR_ALPHA);
  bool setColorPreset(int index);

  /* Sets if the npc on path will force interaction */
  void setForcedInteraction(bool forced);

  /* Sets if the path is hovered */
  void setHovered(bool hovered);

  /* Sets the hover node x, y. Default unsets */
  void setHoverNode(int x = -1, int y = -1);

  /* Sets the index of actions that can be done on the path */
  bool setIndexMove(int index);
  bool setIndexSelect(int index);

  /* Sets the start node */
  bool setStartNode(int x, int y);

  /* Sets the path state */
  void setState(MapNPC::NodeState state);

  /* Sets the tracking distance setpoints */
  bool setTrackDistMax(int dist);
  bool setTrackDistMin(int dist);
  bool setTrackDistRun(int dist);

  /* Sets the tracking state of the npc on the path */
  void setTracking(TrackingState tracking);

  /* Set visibility control */
  void setVisibleControl(bool visible);
  void setVisibleEdit(bool visible);

  /* Shape definition function - virtual */
  QPainterPath shape() const;

  /* Unsets index of actions on the path */
  void unsetIndexMove(bool cancel = false, bool allow_update = true);
  void unsetIndexSelect(bool allow_update = true);

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  /* The copy operator */
  EditorNPCPath& operator= (const EditorNPCPath &source);
};

#endif // EDITORNPCPATH_H
