#include "include/config/team.h"
#include "model/gamemodel.h"
#include "guibotlabel.h"
#include "movement/move_collisions.h"

GuiBotLabel::GuiBotLabel()
{
    int radius = boundingRect().width() / 2;
    setTransformOriginPoint(radius,radius);   // sets center point, around which it rotates
}

QRectF GuiBotLabel::boundingRect() const
{
    int diameter = 200;
    return QRectF(0,0,diameter,diameter);
}

void GuiBotLabel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    (void)option;
    (void)widget;
    QRectF rec = boundingRect();
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(QPen(Qt::black, 12, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));


    if (hidden == false){
        //Setting color of drawn ID
        if (mainTeam) {
            if(myTeam == "Blue") {
                painter->setPen(QPen(QColor::fromRgb(0,255,255,255), 12, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
            } else if(myTeam == "Yellow") {
                painter->setPen(QPen(QColor::fromRgb(255,215,0,255), 12, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
            }
        } else {
            if(myTeam == "Blue") {
                painter->setPen(QPen(QColor::fromRgb(255,215,0,255), 12, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
            } else if(myTeam == "Yellow") {
                painter->setPen(QPen(QColor::fromRgb(0,255,255,255), 12, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
            }
        }

        //Drawing numerical ID and Displays the current collision status
        //(0) = Move_OK, (1) = MOVE_YIELDING, (2) = MOVE_COLLIDED
        Robot* stateRob = gameModel->findMyTeam(id);
        QString label("" + QString::number(id) + "(" + QString::number(Movement::Collisions::getMoveStatus(stateRob)) + ")");
        QFont sansFont("Courier", rec.width()/4, QFont::Bold);
        painter->setFont(sansFont);
        painter->drawText(rec,label);

        //Drawing ball indicator
        Robot* r = gameModel->getHasBall();
        if(r != NULL && r->getID() == this->id)
        {
            painter->setBrush(QBrush(QColor::fromRgb(255,69,0,255), Qt::SolidPattern));
            if(r->isOnMyTeam() == (TEAM == TEAM_BLUE ? mainTeam : !mainTeam)) {
                QRectF b(100, 50, -50, -50);
                painter->drawEllipse(b);
            }
        }

    }
}
