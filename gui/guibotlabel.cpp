#include <iostream>
#include "include/config/team.h"
#include "model/gamemodel.h"
#include "guibotlabel.h"
#include"movement/move_collisions.h"

GuiBotLabel::GuiBotLabel(int team)
    : team(team)
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

    if (hidden == false)
    {
        //Setting color of drawn ID
        auto color = QColor::fromRgb(0,255,255);
        if(team == TEAM_YELLOW)
            color = QColor::fromRgb(255,215,0);
        painter->setPen(QPen(color, 12, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));

        /* If we're on our team ,get the move status. Otherwise just
         * get the Id for the above label */
        QString label;
        if(team == TEAM) {
           Robot* robot = gameModel->findMyTeam(id);
           if(robot) {
                label = "" + QString::number(id) + "(" + QString::number(Movement::Collisions::getMoveStatus(robot)) + ")";
           }
        } else {
           label = QString::number(id);
        }
        QFont sansFont("Courier", rec.width()/4, QFont::Bold);
        painter->setFont(sansFont);
        painter->drawText(rec,label);

        //Drawing ball indicator
        Robot* r = gameModel->getHasBall();
        if(r != NULL && r->getID() == id)
        {
            painter->setBrush(QBrush(QColor::fromRgb(255,69,0,255), Qt::SolidPattern));
            if(r->isOnMyTeam() == (TEAM == TEAM_BLUE ? mainTeam : !mainTeam)) {
                QRectF b(100, 0, -50, -50);
                painter->drawEllipse(b);
            }
        }
    }
}

