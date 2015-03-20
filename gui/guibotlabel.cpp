#include "guibotlabel.h"
#include "model/gamemodel.h"
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

    //int radius = diameter/2;
    int diameter = 200;
    return QRectF(0,0,diameter,diameter);
}

void GuiBotLabel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    (void)option;
    (void)widget;
    QRectF rec = boundingRect();
    QGraphicsTextItem name;
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(QPen(Qt::black, 12, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
//    painter->setBrush(QBrush(Qt::black, Qt::SolidPattern));

    QString label("" + QString::number(id));
    if (hidden == false){
//        painter->drawEllipse(rec);
        if (mainTeam) {
            if          (myTeam == "Blue") {
                painter->setPen(QPen(QColor::fromRgb(0,255,255,255), 12, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
//                painter->setPen(QPen(Qt::cyan, 12, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
            } else if   (myTeam == "Yellow") {
                painter->setPen(QPen(QColor::fromRgb(255,215,0,255), 12, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
            }
        } else {
            if          (myTeam == "Blue") {
                painter->setPen(QPen(QColor::fromRgb(255,215,0,255), 12, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
            } else if   (myTeam == "Yellow") {
                painter->setPen(QPen(QColor::fromRgb(0,255,255,255), 12, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
            }
        }

        QFont sansFont("Courier", rec.width()/4, QFont::Bold);
         painter->setFont(sansFont);
//         painter->drawRect(rec);
        painter->drawText(rec,label);
        Robot* r = gameModel->getHasBall();
        if(r != NULL && r->getID() == this->id)
        {
            painter->setBrush(QBrush(QColor::fromRgb(255,69,0,255), Qt::SolidPattern));
            if(r->isOnMyTeam() == mainTeam) {
                QRectF b(100, 50, -50, -50);
                painter->drawEllipse(b);
            }
        }
    }
}
