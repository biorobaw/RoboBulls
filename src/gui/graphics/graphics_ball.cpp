#include "graphics_ball.h"
#include "gui/data/gui_ball.h"
#include "gui/style_sheets/color_palettes.h"
#include <QPainter>

QBrush GraphicsBall::brush = ball_pallete_maps["Red-Orange"].brush;
QPen   GraphicsBall::pen   = ball_pallete_maps["Red-Orange"].pen;



GraphicsBall::GraphicsBall(bool is_icon) : draw_at_origin(is_icon)
{
    int radius = boundingRect().width() / 2;
    setTransformOriginPoint(radius,radius);   // sets center point, around which it rotates
    setToolTip("Ball");
    setZValue(2);
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

    if(!draw_at_origin){
        auto pos = GuiBall::getPosition();
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
