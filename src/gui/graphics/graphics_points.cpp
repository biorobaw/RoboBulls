#include "graphics_points.h"


GraphicsPoints::GraphicsPoints()
{
}

QRectF GraphicsPoints::boundingRect() const
{
    return QRectF(0,0,0,0);
}

void GraphicsPoints::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    (void)option;
    (void)widget;

    int num_points = points.size();
    QPointF* Q_points = new QPointF[num_points];

    for(int i = 0; i < num_points; ++i)
    {
        Q_points[i].setX(points.at(i).x);
        Q_points[i].setY(points.at(i).y);
    }

    painter->setPen(Qt::black);
    painter->drawPoints(Q_points, num_points);
    delete[] Q_points;
}
