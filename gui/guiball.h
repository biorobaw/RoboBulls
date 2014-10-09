#ifndef GUIBALL_H
#define GUIBALL_H
#include <QPainter>
#include <QGraphicsItem>
#include <QDebug>
#include <QPixmap>
#include <QLabel>

class GuiBall : public QGraphicsItem
{
public:
    GuiBall();

    QRectF boundingRect() const;    // outermost edges of the object (must be implemented with QGraphicsItem)
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    bool Pressed = false;
    QString color;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);          // we want to know when the mouse is pressed...
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);       // ..and released


};

#endif // GUIBALL_H
