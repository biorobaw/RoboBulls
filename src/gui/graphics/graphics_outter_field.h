#ifndef _GRAPHICS_OUTTER_FIELD_
#define _GRAPHICS_OUTTER_FIELD_
#include <QGraphicsItem>


// Graphics element to draw the outter field

class GraphicsOutterField : public QGraphicsItem
{
public:

    GraphicsOutterField();

    // required QGraphicsItem functions
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;


    QString colorScheme = "Default"; // defines the color palette to be used

};

#endif // _GRAPHICS_OUTTER_FIELD_
