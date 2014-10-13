#ifndef GUISIDELINES_H
#define GUISIDELINES_H
#include <QPainter>
#include <QGraphicsPixmapItem>
#include <QGraphicsItem>
#include <QDebug>

class GuiSidelines : public QGraphicsItem
{
public:
    GuiSidelines();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    bool plain = false;
    QString colorScheme = "Default";
    bool Pressed = false;
    bool highlighted = false;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);          // we want to know when the mouse is pressed...
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);       // ..and released
};

#endif // GUISIDELINES_H
