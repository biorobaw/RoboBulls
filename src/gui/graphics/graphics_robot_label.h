#ifndef _GRAPHICS_ROBOT_LABEL_
#define _GRAPHICS_ROBOT_LABEL_
#include <QPainter>
#include <QGraphicsItem>
#include <QDebug>
#include <QPixmap>
#include <QLabel>

//GuiBotLabel is the ID and ball indication label on the robot on the field.
//(GuiRobot on the FieldPanel)

class GuiRobot;

class GraphicsLabel : public QGraphicsItem
{
public:
    GraphicsLabel(int team, int id);

    QRectF boundingRect() const;    // outermost edges of the object (must be implemented with QGraphicsItem)
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    bool hidden = true;
private:
    GuiRobot* robot = nullptr;
};

#endif // _GRAPHICS_ROBOT_LABEL_

