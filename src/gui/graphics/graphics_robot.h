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

class GraphicsRobot : public QGraphicsItem   // inheriting QGraphicsItem
{
//    Q_OBJECT
public:
    GraphicsRobot(int team, int id, bool is_icon);
    GuiRobot* robot;

    QRectF boundingRect() const;    // outermost edges of the object (must be implemented with QGraphicsItem)
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);


    void setRobot(GuiRobot* robot);



protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);          // we want to know when the mouse is pressed...
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);       // ..and released
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);


private:
    bool is_icon = false; // if icon, robot is drawn on axis center
    void update_coordinates(); // if not an icon robot updates its coordinates

};

#endif // GUIROBOT_H
