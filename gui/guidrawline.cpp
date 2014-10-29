#include "guidrawline.h"

GuiDrawLine::GuiDrawLine()
{
}

QRectF GuiDrawLine::boundingRect() const
{

}

void GuiDrawLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPointF origin(x1, y1);
    QPointF end(x2, y2);
    painter->setPen(QPen(Qt::red, 15, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
    painter->drawLine(origin, end);
}
