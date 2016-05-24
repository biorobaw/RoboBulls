#include "guidrawpoint.h"


GuiDrawPoint::GuiDrawPoint()
{
}

QRectF GuiDrawPoint::boundingRect() const
{
    return QRectF(0,0,0,0);
}

void GuiDrawPoint::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    (void)option;
    (void)widget;

    int num_points = points.size();
    QPointF Q_points[num_points];

    for(int i = 0; i < num_points; ++i)
    {
        Q_points[i].setX(points.at(i).x);
        Q_points[i].setY(points.at(i).y);
    }

    painter->setPen(Qt::red);
    painter->drawPoints(Q_points, num_points);
}
