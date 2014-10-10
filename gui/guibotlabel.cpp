#include "guibotlabel.h"

GuiBotLabel::GuiBotLabel()
{
//    setFlag(ItemIsMovable); // makes it movable
    int radius = boundingRect().width() / 2;
    setTransformOriginPoint(radius,radius);   // sets center point, around which it rotates
}

QRectF GuiBotLabel::boundingRect() const
{
//    int diameter = 100;
//    int radius = diameter / -2;
////    return QRectF(5,-120,diameter,diameter);   // wonky, unpatterned values due to matrix transformations and font size
//    return QRectF(40,-200,diameter,diameter);
    int diameter = 200;
    int radius = diameter/2;
    return QRectF(0,0,diameter,diameter);
}

void GuiBotLabel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rec = boundingRect();
    QGraphicsTextItem name;
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(QPen(Qt::black, 12, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
//    painter->setBrush(QBrush(Qt::black, Qt::SolidPattern));

    QString label("" + QString::number(id));
    if (hidden == false){
//        painter->drawEllipse(rec);
        if (myTeam) {
            painter->setPen(QPen(Qt::cyan, 12, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
        } else {
            painter->setPen(QPen(Qt::yellow, 12, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
        }
        QFont sansFont("Courier", rec.width()/4, QFont::Bold);
         painter->setFont(sansFont);
//         painter->drawRect(rec);
        painter->drawText(rec,label);
    }
}
