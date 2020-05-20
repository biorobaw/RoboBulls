#ifndef GUISIDELINES_H
#define GUISIDELINES_H
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

#endif // GUISIDELINES_H
