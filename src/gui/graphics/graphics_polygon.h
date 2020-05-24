#ifndef _GRAPHICS_POLYGON_
#define _GRAPHICS_POLYGON_

#include <QGraphicsItem>


// Graphics element to draw a polygon defined by bui_interface
// TODO: does this class have any point? it's just a wrapper but doesn't do anythin special

class GraphicsPolygon : public QGraphicsObject
{
    Q_OBJECT
public:
    GraphicsPolygon();

    // required QGraphicsItem functions
    QRectF boundingRect() const override;    // outermost edges of the object
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;



    QPolygon Q_polygon;
};

#endif // _GRAPHICS_POLYGON_
