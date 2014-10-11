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
    void subCircle(QPainter *painter, QBrush topLft, QBrush topRt, QBrush lowLft, QBrush lowRt);
    void paintEvent(QPaintEvent *);
    int id;
    bool myTeam = true;
    bool Pressed = false;
    bool highlighted = false;
    bool icon = false;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);          // we want to know when the mouse is pressed...
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);       // ..and released

};

#endif // GUIROBOT_H
