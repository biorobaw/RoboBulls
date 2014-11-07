#ifndef GUIDRAWLINE_H
#define GUIDRAWLINE_H
#include <QPainter>
#include <QGraphicsItem>
#include <QDebug>
#include <QPixmap>
#include <QLabel>
#include "utilities/point.h"
#include <deque>
#include <QObject>


class GuiDrawLine : public QGraphicsObject
{
    Q_OBJECT

public:
    GuiDrawLine();
    QRectF boundingRect() const;    // outermost edges of the object
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void ageLine();

    int x1;
    int y1;
    int x2;
    int y2;
    int age = 5;
    int lifeSpan = 6000;

public slots:
    void decay();

};

#endif // GUIDRAWLINE_H
