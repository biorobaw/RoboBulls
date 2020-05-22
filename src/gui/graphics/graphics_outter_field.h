#ifndef _GRAPHICS_OUTTER_FIELD_
#define _GRAPHICS_OUTTER_FIELD_
#include <QPainter>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QDebug>

class GraphicsOutterField : public QGraphicsItem
{
public:
    GraphicsOutterField();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QString colorScheme = "Default";
//    bool Pressed = false;
//    bool highlighted = false;

//protected:
//    void mousePressEvent(QGraphicsSceneMouseEvent *event);          // we want to know when the mouse is pressed...
//    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);       // ..and released
};

#endif // _GRAPHICS_OUTTER_FIELD_
