#include "graphics_ball.h"
#include "gui/data/gui_ball.h"

QBrush GraphicsBall::brush= QBrush(QColor::fromRgb(255,69,0,255), Qt::SolidPattern);;
QPen   GraphicsBall::pen= QPen(Qt::black, 10, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);;



GraphicsBall::GraphicsBall(bool is_icon) :
    ball(&GuiBall::ball), is_icon(is_icon)
{
    int radius = boundingRect().width() / 2;
    setTransformOriginPoint(radius,radius);   // sets center point, around which it rotates
    setToolTip("Ball");
    show();
}

QRectF GraphicsBall::boundingRect() const
{
    int diameter = 200;
    return QRectF(0,0,diameter,diameter);
}

void GraphicsBall::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    (void)option;
    (void)widget;
    QRectF rec = boundingRect();

    if(!is_icon){
        auto pos = ball->getPosition();
        setX(pos.x);
        setY(pos.y);
    }

    painter->setBrush(brush);
    painter->setPen(pen);

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->drawEllipse(rec);
}


void GraphicsBall::setColor(QString color){

    if (color == "Red-Orange") {
        brush= QBrush(QColor::fromRgb(255,69,0,255), Qt::SolidPattern);
        pen = QPen(Qt::black, 10, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
    } else if (color == "Red") {
        brush = QBrush(Qt::red, Qt::SolidPattern);
        pen = QPen(Qt::black, 10, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
    } else if (color == "Green") {
        brush = QBrush(Qt::green, Qt::SolidPattern);
        pen = QPen(Qt::black, 10, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
    } else if (color == "White") {
        brush = QBrush(Qt::white, Qt::Dense1Pattern);
        pen = QPen(Qt::black, 10, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
    } else if (color == "Orange") {
        brush = QBrush(QColor::fromRgb(255,165,0,255), Qt::SolidPattern);
        pen = QPen(Qt::black, 10, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
    } else if (color == "Blue") {
        brush = QBrush(Qt::blue, Qt::SolidPattern);
        pen = QPen(Qt::white, 10, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
    } else if (color == "Black") {
        brush = QBrush(Qt::black, Qt::SolidPattern);
        pen = QPen(Qt::white, 10, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
    } else if (color == "Yellow") {
        brush = QBrush(Qt::yellow, Qt::SolidPattern);
    } else if (color == "Purple") {
        brush = QBrush(QColor::fromRgb(160,32,240,255), Qt::SolidPattern);
        pen = QPen(Qt::white, 10, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
    } else if (color == "Hello Kitty!") {
        brush =  QBrush(QColor::fromRgb(255,20,147,255), Qt::SolidPattern);
        pen = QPen(Qt::white, 10, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
    }

}
