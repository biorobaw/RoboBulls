#include "graphics_ball.h"
#include "gui/data/gui_ball.h"
#include "gui/style_sheets/color_palettes.h"

QBrush GraphicsBall::brush = ball_pallete_maps["Red-Orange"].brush;
QPen   GraphicsBall::pen   = ball_pallete_maps["Red-Orange"].pen;



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

    auto& pallete = ball_pallete_maps[color];
    brush = pallete.brush;
    pen = pallete.pen;

}
