#include "guidrawline.h"

GuiDrawLine::GuiDrawLine()
{
}

QRectF GuiDrawLine::boundingRect() const
{
    return QRectF(0,0,0,0);
}

void GuiDrawLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    (void)option;
    (void)widget;
    QPointF origin(x1, y1);
    QPointF end(x2, y2);
    painter->setPen(QPen(Qt::red, 15, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
    painter->drawLine(origin, end);
}
