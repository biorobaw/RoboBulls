#include "guidrawline.h"
#include <QTimer>
#include <QPainter>
#include <iostream>

GuiDrawLine::GuiDrawLine() {

}

QRectF GuiDrawLine::boundingRect() const
{
    return QRectF(0,0,0,0);
}

void GuiDrawLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    (void)option;
    (void)widget;

    QBrush brush(Qt::white, Qt::SolidPattern);

    QPointF A(x1, y1);
    QPointF B(x2, y2);

    if (age == 5) {
        painter->setPen(QPen(QColor::fromRgb(0,255,0,255), 20, Qt::DotLine, Qt::RoundCap, Qt::MiterJoin));
        brush = (QBrush(QColor::fromRgb(0,255,0,255), Qt::SolidPattern));
    }
    if (age == 4) {
        painter->setPen(QPen(QColor::fromRgb(173,255,47,200), 20, Qt::DotLine, Qt::RoundCap, Qt::MiterJoin));
        brush = (QBrush(QColor::fromRgb(173,255,47,200), Qt::SolidPattern));
    }
    if (age == 3) {
        painter->setPen(QPen(QColor::fromRgb(255,255,0,155), 20, Qt::DotLine, Qt::RoundCap, Qt::MiterJoin));
        brush = (QBrush(QColor::fromRgb(255,255,0,155), Qt::SolidPattern));
    }
    if (age == 2) {
        painter->setPen(QPen(QColor::fromRgb(255,165,0,100), 20, Qt::DotLine, Qt::RoundCap, Qt::MiterJoin));
        brush = (QBrush(QColor::fromRgb(255,165,0,100), Qt::SolidPattern));
    }
    if (age == 1) {
        painter->setPen(QPen(QColor::fromRgb(255,69,0,55), 20, Qt::DotLine, Qt::RoundCap, Qt::MiterJoin));
        brush = (QBrush(QColor::fromRgb(255,69,0,55), Qt::SolidPattern));
    }
    if (age == 0) {
        painter->setPen(QPen(QColor::fromRgb(255,0,0,5), 20, Qt::DotLine, Qt::RoundCap, Qt::MiterJoin));
        brush = (QBrush(QColor::fromRgb(255,0,0,5), Qt::SolidPattern));
    }

//    if (draw) {
    painter->drawLine(A, B);
    painter->setBrush(brush);
    painter->setPen(QPen(QColor::fromRgb(255,0,0,0), 0, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
    painter->drawEllipse(B,30,30);
//    }
}

void GuiDrawLine::ageLine() {
//    cout << "ageLine() \n";
    int milliseconds = lifeSpan*1000;
    int stages = 5;

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(decay()));
    if (age == 5) {
        timer->start(milliseconds/stages);
    } else {
//        timer->stop();
    }
}

void GuiDrawLine::decay() {
    if (age > 0) {
        age--;
//        cout << "...age: " << age << "\n";
    } else {

    }

}

