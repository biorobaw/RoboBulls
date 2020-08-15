#ifndef _GRAPHICS_ROBOT_LABEL_
#define _GRAPHICS_ROBOT_LABEL_
#include <QGraphicsItem>
class GuiRobot;

// Graphics element that draws robot labels (id of robot + has ball indicator)

class GraphicsRobotLabel : public QGraphicsItem
{
public:
    GraphicsRobotLabel(int team, int id);

    // required QGraphicsItem functions
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    bool hidden = true; // defines whether to draw or not the label
private:
    GuiRobot* robot = nullptr; // pointer to the robot represented by this graphics item
};

#endif // _GRAPHICS_ROBOT_LABEL_

