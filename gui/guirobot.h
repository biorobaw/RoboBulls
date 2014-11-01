#ifndef GUIROBOT_H
#define GUIROBOT_H
#include <QPainter>
#include <QGraphicsItem>
#include <QDebug>
#include <QPixmap>
#include <QLabel>
#include <QPaintEvent>

class GuiRobot : public QGraphicsItem   // inheriting QGraphicsItem
{
public:
    GuiRobot();

    QRectF boundingRect() const;    // outermost edges of the object (must be implemented with QGraphicsItem)
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    int id;
    bool mainTeam = true;
    bool Pressed = false;
    bool highlighted = false;
    bool kicking = false;
    bool dribling = false;
    bool icon = false;
    bool doubleClicked = false;
    bool enabled = true;
    bool overridden = false;
    std::string myTeam = "Blue";

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);          // we want to know when the mouse is pressed...
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);       // ..and released
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

};

#endif // GUIROBOT_H
