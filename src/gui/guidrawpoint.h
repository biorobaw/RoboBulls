#ifndef GUIDRAWPOINT_H
#define GUIDRAWPOINT_H

#include <QGraphicsItem>
#include <QPainter>
#include "src/utilities/point.h"
#include <vector>

class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;

class GuiDrawPoint : public QGraphicsObject
{
    Q_OBJECT
public:
    GuiDrawPoint();
    QRectF boundingRect() const;    // outermost edges of the object
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    std::vector<Point> points;
};

#endif // GUIDRAWPOINT_H
