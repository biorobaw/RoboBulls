#ifndef PASSBALLSENDER_H
#define PASSBALLSENDER_H

#include <vector>
#include "behavior/behavior.h"
#include "model/robot.h"
#include "model/gamemodel.h"
#include "utilities/point.h"
#include "skill/skill.h"
#include "skill/kicktopoint.h"
#include "behavior/genericmovementbehavior.h"

/*
 * PassBallSender class
 * This class finds a point in front of one of
 * the team's players robots that is free and closer to
 * the sender compared to other players.
 * Then, it passes the ball to that robot.
 *
 * What we have now is the simplest form of passing the ball.
 * Many things have been implemented here that have not been
 * used but can be used to improve this behavior
 *
 * Squggestions for improvements:
 *      ** send the ball to the robot that is closer
 *      to the goal, closer to the robot, and has least
 *      amount of opponents players in its region
 * */

class PassBallSender : public GenericMovementBehavior /*public Behavior*/
{
public:
    PassBallSender();
    void perform(Robot*);

    /*
     * finds the passing point based on the
     * charactrastic we discussed on the
     * description of the class
     * */
    Point findPassPoint(Robot*);

    /*
     * simple set and get function
     * */
    void setPassingPoint(Point);
    Point getPassingPoint();


    bool kicked;

    /*
     * this struct is used to gather all the
     * charactristics of all the robots on the
     * field.
     * we compare these charactristics to decide which
     * robot the sender should pass to
     * */
    struct playersCharactristics{
        Point position;
        int ID;
        double distanceToGoal;  // how far away is the reciver player from the goal
        int surroundingAppNum;  //number of opponent players surrounding our team players
        double distanceToRobot; // how far away is the recieve player from sender player
    };

    vector <playersCharactristics> myTeamInfo; //stores all the robots charactristics

private:
    Point passingPoint;
    int receiverID;
    Point target;
    enum states {initial, movingBehind, approaching, kicking, idling} state;

};

#endif // PASSBALLSENDER_H
