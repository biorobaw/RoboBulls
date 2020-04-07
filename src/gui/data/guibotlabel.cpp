#include <iostream>

#include "model/game_state.h"
#include "guibotlabel.h"
#include"robot/navigation/path_planning/move_collisions.h"

#include "model/ball.h"
#include "model/constants.h"
#include "model/team.h"
#include "robot/robot.h"

GuiBotLabel::GuiBotLabel(int team, int id)
    : id(id), team(team)
{
    int radius = boundingRect().width() / 2;
    setTransformOriginPoint(radius,radius);   // sets center point, around which it rotates
    setScale(2.5);
}

QRectF GuiBotLabel::boundingRect() const
{
    int diameter = 200;
    return QRectF(0,0,diameter,diameter);
}

void GuiBotLabel::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    QRectF rec = boundingRect();
    painter->setRenderHint(QPainter::Antialiasing, true);

    if (!hidden)
    {
        //Setting color of drawn ID
        auto color = team == ROBOT_TEAM_BLUE ? QColor::fromRgb(0,255,255) : QColor::fromRgb(255,215,0);

        painter->setPen(QPen(color, 12, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));

        /* If the robots are being controllerd by our software, get the move status. Otherwise just
         * get the Id for the above label and draw ball if in possession */
        QString label;
        if(RobotTeam::getTeam(team)->isControlled()) {
           Robot* robot = RobotTeam::getTeam(team)->getRobot(id);
           if(robot) {
                label = "" + QString::number(id) + "(" + QString::number(Collisions::getMoveStatus(robot)) + ")";

                //Drawing ball indicator
                Robot* r = Ball::getRobotWithBall();
                if(r != NULL && r->getID() == id) // does this robot have the ball?
                {
                    painter->setBrush(QBrush(QColor::fromRgb(255,69,0,255), Qt::SolidPattern));
                    QRectF b(100, 0, -50, -50);
                    painter->drawEllipse(b);
                }

           }
        } else {
           label = QString::number(id);
        }
        QFont sansFont("Courier", rec.width()/4, QFont::Bold);
        painter->setFont(sansFont);
        painter->drawText(rec,label);


    }
}

