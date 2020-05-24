#ifndef _GRAPHICS_POINTS_
#define _GRAPHICS_POINTS_

#include <QGraphicsItem>
#include "utilities/point.h"



// Graphics element to draw points defined by gui_interface

class GraphicsPoints : public QGraphicsObject
{
    Q_OBJECT
public:
    GraphicsPoints();

    // required QGraphicsItem functions
    QRectF boundingRect() const override;    // outermost edges of the object
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;



    std::vector<Point> points;
};

#endif // _GRAPHICS_POINTS_
