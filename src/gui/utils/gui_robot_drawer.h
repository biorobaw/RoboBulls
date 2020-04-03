#ifndef GUIROBOT_H
#define GUIROBOT_H
#include <QPainter>
#include <QGraphicsItem>
#include <QDebug>
#include <QPixmap>
#include <QLabel>
#include <QPaintEvent>
#include <QGraphicsView>
#include "utilities/point.h"
#include "model/constants.h"
#include <string>
#include "gui/data/gui_robot.h"
using std::string;


class MainWindow;
//A GuiRobot is a robot on the field (FieldPanel)

class GuiRobotDrawer : public QGraphicsItem   // inheriting QGraphicsItem
{
//    Q_OBJECT
public:
    static GuiRobotDrawer proxies[2][MAX_ROBOTS_PER_TEAM];

    GuiRobotDrawer(int team, int id);
    GuiRobot* robot;

    QRectF boundingRect() const;    // outermost edges of the object (must be implemented with QGraphicsItem)
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);



    bool icon = true;



protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);          // we want to know when the mouse is pressed...
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);       // ..and released
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);




};

#endif // GUIROBOT_H
