#ifndef GUIBALL_H
#define GUIBALL_H
#include <QPainter>
#include <QGraphicsItem>
#include <QDebug>
#include <QPixmap>
#include <QLabel>
#include "utilities/point.h"

//GuiBall isthe ball on the field (FieldPanel)

class GuiBall
{
public:
    static GuiBall ball;
    static void updateBall();
    static Point getPosition();
    static Point getVelocity();
    static float getSpeed();


protected:

    Point position = Point(0,0);
    Point velocity = Point(0,0);
    float speed = 0;

private:
    GuiBall();

};

#endif // GUIBALL_H
