#include "guisidelines.h"
#include <QApplication>

GuiSidelines::GuiSidelines()
{
}

QRectF GuiSidelines::boundingRect() const
{
    return QRectF(0,0,8200,6200); // 3585 ~ 3600, 2585 ~ 2600
}

void GuiSidelines::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rec = boundingRect();
    QBrush plainBrush(QColor::fromRgb(0,100,0,255), Qt::SolidPattern);
    QBrush crossBrush(QColor::fromRgb(32,178,170,255), Qt::SolidPattern);
    QBrush brush(QColor::fromRgb(0,100,0,255), Qt::SolidPattern);

        painter->setPen(QPen(Qt::darkBlue, 60, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
//        if (plain) {
//            painter->fillRect(rec, plainBrush);
//        }
//        else {
//            painter->fillRect(rec, crossBrush);
//        }
        if (colorScheme == "Default") {
//            brush = (QColor::fromRgb(0,100,0,255), Qt::SolidPattern);
            painter->setPen(QPen(Qt::white, 60, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
            painter->setBrush(QColor::fromRgb(0,100,0,255));
        } else if (colorScheme == "Ice Rink") {
//            brush = (QColor::fromRgb(32,178,170,255), Qt::SolidPattern);
            painter->setBrush(QColor::fromRgb(32,178,170,255));
            painter->setPen(QPen(Qt::darkRed, 60, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
        } else if (colorScheme == "Air Hockey") {
//            brush = (QColor::fromRgb(32,178,170,255), Qt::SolidPattern);
            painter->setBrush(QColor::fromRgb(100,149,237,255));
            painter->setPen(QPen(Qt::white, 60, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
        } else if (colorScheme == "Basketball") {
//            brush = (QColor::fromRgb(32,178,170,255), Qt::SolidPattern);
            painter->setBrush(QColor::fromRgb(160,82,45,255));
            painter->setPen(QPen(Qt::white, 60, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
        } else if (colorScheme == "Arctic") {
            painter->setBrush(QColor::fromRgb(200,200,200,255));
            painter->setPen(QPen(Qt::darkGray, 60, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
        } else if (colorScheme == "Game Boy") {
            painter->setBrush(QColor::fromRgb(143,188,143,255));
            painter->setPen(QPen(QColor::fromRgb(85,107,47,255), 60, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
        }

//        painter->fillRect(rec, brush);
        painter->drawRect(rec);
}

void GuiSidelines::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier) == false) {
        Pressed = true;
    }
    update();
    QGraphicsItem::mousePressEvent(event);
}

void GuiSidelines::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier) == false) {
        Pressed = false;
    }
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

