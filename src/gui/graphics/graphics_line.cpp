#include "graphics_line.h"
#include <QPainter>
#include <QTimer>

#include "gui/style_sheets/color_palettes.h"

GraphicsLine::GraphicsLine() {

}

QRectF GraphicsLine::boundingRect() const
{
    return QRectF(0,0,0,0);
}

void GraphicsLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    (void)option;
    (void)widget;

    QBrush brush(Qt::white, Qt::SolidPattern);

    QPointF A(x1, y1);
    QPointF B(x2, y2);

    painter->setPen(line_age_colors[age].pen);
    brush = line_age_colors[age].brush;


//    if (draw) {
    painter->drawLine(A, B);
    painter->setBrush(brush);
    painter->setPen(graphics_line_ellipse_pen);
    painter->drawEllipse(B,10,10);
//    }
}

void GraphicsLine::ageLine() {
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

void GraphicsLine::decay() {
    if (age > 0) {
        age--;
//        cout << "...age: " << age << "\n";
    } else {

    }

}

