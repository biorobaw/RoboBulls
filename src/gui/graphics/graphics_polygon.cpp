#include "graphics_polygon.h"

#include <QPainter>

GraphicsPolygon::GraphicsPolygon()
{
}

QRectF GraphicsPolygon::boundingRect() const
{
    return QRectF(0,0,0,0);
}

void GraphicsPolygon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    (void)option;
    (void)widget;

    painter->setPen(Qt::red);
    painter->drawPolygon(Q_polygon);
}
