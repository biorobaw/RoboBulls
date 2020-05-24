#ifndef _GRAPHICS_ROBOT_
#define _GRAPHICS_ROBOT_
#include <QGraphicsItem>


class MainWindow;
class GuiRobot;
//A GuiRobot is a robot on the field (FieldPanel)

class GraphicsRobot : public QObject, public QGraphicsItem    // inheriting QGraphicsItem
{
    Q_OBJECT
public:
    GraphicsRobot(QObject* parent, int team, int id, bool is_icon);
    GuiRobot* robot;

    QRectF boundingRect() const;    // outermost edges of the object (must be implemented with QGraphicsItem)
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);


    void setRobot(GuiRobot* robot);



protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);          // we want to know when the mouse is pressed...
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);


private:
    bool is_icon = false; // if icon, robot is drawn on axis center
    void update_coordinates(); // if not an icon robot updates its coordinates

};

#endif // _GRAPHICS_ROBOT_
