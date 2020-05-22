#ifndef _GRAPHICS_POLYGON_
#define _GRAPHICS_POLYGON_

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

#endif // _GRAPHICS_POLYGON_
