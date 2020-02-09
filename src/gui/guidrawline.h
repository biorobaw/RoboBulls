#ifndef GUIDRAWLINE_H
#define GUIDRAWLINE_H

#include <QGraphicsItem>
#include <QTimer>
#include <QPainter>
#include <iostream>
#include "src/utilities/point.h"

class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;

class GuiDrawLine : public QGraphicsObject
{
    Q_OBJECT

public:
    GuiDrawLine();
    QRectF boundingRect() const;    // outermost edges of the object
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
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

#endif // GUIDRAWLINE_H
