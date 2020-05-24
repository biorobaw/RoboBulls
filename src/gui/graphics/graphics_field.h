#ifndef _GRAPHICS_FIELD_
#define _GRAPHICS_FIELD_

#include <QGraphicsItem>

// Graphics element to draw the field

class GraphicsField : public QGraphicsItem
{
public:

    // graphics item constructor
    GraphicsField();

    // required QGraphicsItem functions
    QRectF boundingRect() const override;    // outermost edges of the object
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;



    QString colorScheme = "Default"; // determines the pallete of colors for the field
    bool grid = false;               // determines whether to draw a grid on top of the field or not
    int gridScale = 250; // sets the scale of the grid percentually where 250% = 0.5m

};

#endif // _GRAPHICS_FIELD_
