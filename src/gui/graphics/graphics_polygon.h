#ifndef GUIDRAWREGION_H
#define GUIDRAWREGION_H

#include <QGraphicsItem>
#include <QPainter>
#include <QPolygon>
#include "utilities/point.h"

class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;

class GraphicsPolygon : public QGraphicsObject
{
    Q_OBJECT
public:
    GraphicsPolygon();
    QRectF boundingRect() const;    // outermost edges of the object
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPolygon Q_polygon;
};

#endif // GUIDRAWREGION_H
