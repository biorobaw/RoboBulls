#include "guiball.h"
#include <include/config/simulated.h>

GuiBall::GuiBall()
{
    Pressed = false;
    if (SIMULATED) {
        setFlag(ItemIsMovable); // makes it movable
    }
    int radius = boundingRect().width() / 2;
    setTransformOriginPoint(radius,radius);   // sets center point, around which it rotates


}

QRectF GuiBall::boundingRect() const
{
    int diameter = 200;
    int radius = diameter / 2;
//    return QRectF(radius,radius,diameter,diameter);
    return QRectF(0,0,diameter,diameter);
}

void GuiBall::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rec = boundingRect();
    painter->setBrush(QBrush(QColor::fromRgb(255,20,147,255), Qt::SolidPattern));
    painter->setPen(QPen(Qt::white, 10, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(QPen(Qt::black, 10, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
    if (color == "Red-Orange") {
        painter->setBrush(QBrush(QColor::fromRgb(255,69,0,255), Qt::SolidPattern));
    } else if (color == "Red") {
        painter->setBrush(QBrush(Qt::red, Qt::SolidPattern));
    } else if (color == "Green") {
        painter->setBrush(QBrush(Qt::green, Qt::SolidPattern));
    } else if (color == "White") {
        painter->setBrush(QBrush(Qt::white, Qt::Dense1Pattern));
    } else if (color == "Orange") {
        painter->setBrush(QBrush(QColor::fromRgb(255,165,0,255), Qt::SolidPattern));
    } else if (color == "Blue") {
        painter->setBrush(QBrush(Qt::blue, Qt::SolidPattern));
        painter->setPen(QPen(Qt::white, 10, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
    } else if (color == "Black") {
        painter->setBrush(QBrush(Qt::black, Qt::SolidPattern));
        painter->setPen(QPen(Qt::white, 10, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
    } else if (color == "Yellow") {
        painter->setBrush(QBrush(Qt::yellow, Qt::SolidPattern));
    } else if (color == "Purple") {
        painter->setBrush(QBrush(QColor::fromRgb(160,32,240,255), Qt::SolidPattern));
        painter->setPen(QPen(Qt::white, 10, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
    } else if (color == "Hello Kitty!") {
        painter->setBrush(QBrush(QColor::fromRgb(255,20,147,255), Qt::SolidPattern));
        painter->setPen(QPen(Qt::white, 10, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
    }


    painter->drawEllipse(rec);
}

void GuiBall::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

}

void GuiBall::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

}
