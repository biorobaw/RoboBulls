#ifndef GUIBALL_H
#define GUIBALL_H
#include <QPainter>
#include <QGraphicsItem>
#include <QDebug>
#include <QPixmap>
#include <QLabel>
#include "utilities/point.h"

//GuiBall isthe ball on the field (FieldPanel)

class GuiBall : public QGraphicsItem
{
public:
    static GuiBall ball;
    static void updateBall();
    static Point getPosition();
    static Point getVelocity();
    static float getSpeed();



    QRectF boundingRect() const;    // outermost edges of the object (must be implemented with QGraphicsItem)
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    bool Pressed = false;
    QString color;



protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);          // we want to know when the mouse is pressed...
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);       // ..and released

    Point position = Point(0,0);
    Point velocity = Point(0,0);
    float speed = 0;

private:
    GuiBall();

};

#endif // GUIBALL_H
