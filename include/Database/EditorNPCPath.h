/*******************************************************************************
 * Class Name: EditorNPCPath
 * Date Created: March 29, 2015
 * Inheritance: QGraphicsItem
 * Description: A graphics item which represents the path of a map npc.
 ******************************************************************************/
#ifndef EDITORNPCPATH_H
#define EDITORNPCPATH_H

#include <QColor>
#include <QGraphicsItem>
#include <QList>
#include <QPainter>
#include <QRect>

#include "EditorEnumDb.h"
#include "EditorHelpers.h"
#include "Game/Map/MapNPC.h"

/* Editor NPC Path Class */
class EditorNPCPath : public QGraphicsItem
{
public:
  /* Constructor Function */
  EditorNPCPath(int x = 0, int y = 0, int delay = 0, bool xy_flip = false);

  /* Copy constructor function */
  EditorNPCPath(const EditorNPCPath &source);

  /* Destructor Function */
  ~EditorNPCPath();

  /* Control functions */
  enum { Type_PathRender = UserType + 2};
  int type() const { return Type_PathRender; }

private:
  /* Color of path */
  QColor color;
  QList<QColor> color_presets;

  /* Is the path hovered? */
  bool hovered;

  /* Force interaction when within range? */
  bool interact;

  /* List of path nodes */
  QList<Path> nodes;

  /* Sets the path state */
  MapNPC::NodeState state;

  /* Tracking properties */
  MapNPC::TrackingState tracking;

  /*------------------- Constants -----------------------*/
  const static uint8_t kBORDER_W; /* Border width around lines and rects */
  const static uint8_t kCOLOR_ALPHA; /* Default color alpha */
  const static uint8_t kLINE_W; /* Line width (without border) */
  const static uint8_t kRECT_W; /* Rect width (without border) */

/*============================================================================
 * PROTECTED FUNCTIONS
 *===========================================================================*/
protected:
  /* Copy function, to be called by a copy or equal operator constructor */
  void copySelf(const EditorNPCPath &source);

  /* Get hover color */
  QColor getHoverColor();

  /* Hover control */
  void hoverEnterEvent(QGraphicsSceneHoverEvent*);
  void hoverLeaveEvent(QGraphicsSceneHoverEvent*);

  /* Corner paint function */
  void paintCorner(QPainter* painter, int x, int y,
                   Direction enter, Direction exit);

  /* Line paint function */
  void paintLine(QPainter* painter, int x1, int y1, int x2, int y2);
  void paintLines(QPainter* painter, Path* curr, Path* next);

  /* Node paint function */
  void paintNode(QPainter* painter, Path* prev, Path* curr, Path* next,
                 int node_num = -1);

/*============================================================================
 * PUBLIC FUNCTIONS
 *===========================================================================*/
public:
  /* Appends node to the end of the path */
  bool appendNode(int x, int y, int delay = 0, bool xy_flip = false);

  /* Necessary function for returning the bounding rectangle - virtual */
  QRectF boundingRect() const;

  /* Delete nodes from list */
  bool deleteNode(int index);
  void deleteNodes();

  /* Edits the node at the index */
  bool editNode(int index, int x, int y, int delay = -1, bool xy_flip = false);

  /* Returns color of the path */
  QColor getColor();
  QColor getColorPreset(int index);
  int getColorPresetCount();

  /* Returns the path node or node information at the selected index */
  Path* getNode(int index);
  int getNodeDelay(int index);
  QString getNodeStr(int index);
  int getNodeX(int index);
  bool getNodeXYFlip(int index);
  int getNodeY(int index);

  /* Returns the set of path nodes */
  QList<Path> getNodes();

  /* Returns the state of the path */
  MapNPC::NodeState getState();

  /* Returns the tracking state of the npc on the path */
  MapNPC::TrackingState getTracking();

  /* Insert nodes at location */
  bool insertNodeAfter(int index, int x, int y, int delay = 0,
                       bool xy_flip = false);
  bool insertNodeBefore(int index, int x, int y, int delay = 0,
                        bool xy_flip = false);

  /* Returns if the npc on path will force interaction */
  bool isForcedInteraction();

  /* Returns if the path is hovered */
  bool isHovered();

  /* Painting function for Path Wrapper - virtual */
  void paint(QPainter* painter, const QStyleOptionGraphicsItem*,
             QWidget*);

  /* Sets the color of the path */
  bool setColor(int r, int g, int b, int a = kCOLOR_ALPHA);
  bool setColorPreset(int index);

  /* Sets if the npc on path will force interaction */
  void setForcedInteraction(bool forced);

  /* Sets if the path is hovered */
  void setHovered(bool hovered);

  /* Sets the path state */
  void setState(MapNPC::NodeState state);

  /* Sets the tracking state of the npc on the path */
  void setTracking(MapNPC::TrackingState tracking);

  /* Shape definition function - virtual */
  QPainterPath shape() const;

/*============================================================================
 * OPERATOR FUNCTIONS
 *===========================================================================*/
public:
  /* The copy operator */
  EditorNPCPath& operator= (const EditorNPCPath &source);
};

#endif // EDITORNPCPATH_H
