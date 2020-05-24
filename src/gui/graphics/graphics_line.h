#ifndef _GRAPHICS_LINE_
#define _GRAPHICS_LINE_

#include <QGraphicsItem>
#include "utilities/point.h"


// Graphics element to draw lines defined using the gui interface

class GraphicsLine : public QGraphicsObject
{
    Q_OBJECT

public:
    GraphicsLine();

    // required QGraphicsItem functions
    QRectF boundingRect() const override;    // outermost edges of the object
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;


    // TODO: comment what the function does
    void ageLine();



//    bool newLine = false;
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    int age = 5;
    double lifeSpan = 1;
    bool draw = false;

public slots:
    void decay();

};

#endif // _GRAPHICS_LINE_
