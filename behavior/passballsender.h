#ifndef PASSBALLSENDER_H
#define PASSBALLSENDER_H

#include <vector>
#include "behavior/behavior.h"
#include "model/robot.h"
#include "model/gamemodel.h"
#include "utilities/point.h"
#include "skill/skill.h"
#include "behavior/genericmovementbehavior.h"

/*! @brief Behavior to pass the ball to a robot with PassBallReceiver
 * @author Narges Ghaedi
 * @details This class finds a good receiver robot and
 * sends the ball in front of it.
 * The receiver robot has the following charactristics:
 *      1. It has less number of opp. players compared to others
 *      2. It is closer to sender compared to others
 *      (other charactristics can be added)
 * However, if the receiver is in our field or
 * it is standing near the borders, the sender
 * will pass the ball to the center of opponent's
 * field for the receiver.
 *
 * What we have now is the simplest form of passing the ball.
 * Many things have been implemented here that have not been
 * used but can be used to improve this behavior
 *
 * Squggestions for improvements:
 *      ** send the ball to the robot that is closer
 *      to the opp. goal, closer to the robot, and has least
 *      amount of opponents players in its region*/

class PassBallSender : public GenericMovementBehavior
{
public:
    PassBallSender(Robot* waiter);
    void perform(Robot*);
    bool isFinished() override;

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

    /*! @brief Hold information about players on the firld
     * @details This struct is used to gather all the
     * charactristics of all the robots on the
     * field.
     * we compare these charactristics to decide which
     * robot the sender should pass to
     * */
    struct playersCharactristics{
        Point position;         //!<Position of the player
        int ID;                 //!<ID of the player
        double distanceToGoal;  //!<how far away is the reciver player from the goal
        int surroundingAppNum;  //!<number of opponent players surrounding our team players
        double distanceToRobot; //!<how far away is the recieve player from sender player
        bool inBadArea;         //!<Is robot in an area that is dangerous to make a pass?
    };

    vector <playersCharactristics> myTeamInfo; //stores all the robots charactristics

private:
    Robot* waitingRobot;    //A robot that is waiting at the line, used to start
    Point passingPoint;
    int receiverID;
    Point target;
    enum states {movingBehind, approaching, kicking, idling} state;
};

#endif // PASSBALLSENDER_H
