#include "guidrawregion.h"

GuiDrawRegion::GuiDrawRegion()
{
}

QRectF GuiDrawRegion::boundingRect() const
{
    return QRectF(0,0,0,0);
}

void GuiDrawRegion::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    (void)option;
    (void)widget;

    painter->setPen(Qt::red);
    painter->drawPolygon(Q_polygon);
}
