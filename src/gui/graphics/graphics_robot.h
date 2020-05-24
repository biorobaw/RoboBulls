#ifndef _GRAPHICS_ROBOT_
#define _GRAPHICS_ROBOT_
#include <QGraphicsItem>
class GuiRobot;


// Graphics element to draw the robot

class GraphicsRobot : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    GraphicsRobot(QObject* parent, int team, int id, bool draw_at_origin);

    // required QGraphicsItem functions
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    // used to change the robot drawn by the graphics object
    void setRobot(GuiRobot* robot);
    GuiRobot* getRobot();



protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;  // process mouse press events
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override; // process double click events


private:
    GuiRobot* robot; // pointer to the robot drawn by the graphics item
    bool draw_at_origin = false; // indicates whether to draw the robot at the origin or at its coordinates
    void update_coordinates();   // used to update coordinates at which the graphics item is drawn

};

#endif // _GRAPHICS_ROBOT_
