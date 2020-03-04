#ifndef VIDEO_STRATEGIES_H
#define VIDEO_STRATEGIES_H
#include "../strategy.h"
#include "../behavior.h"
#include "../behaviors/genericmovementbehavior.h"
#include "../skills/kicktopointomni.h"
class Robot;

//Constains strategies used in the RoboCup 2015 qualification Video
//Not documented in Doxygen because they are deprecated

//! @cond

namespace Video
{

/* OMNI-ROBOT TEST GIT BRANCH --- TEMPORARY FILE
 * This is a temporary file meant to provide functionality for the Robocup
 * Qulification 2015 video. */

/************************************************************************/
/* BEHAVIORS */

/* OmniRandomKicker
 * A behavior that used OmniRandomKicker to kick the ball to a random point
 * around the `whoIsRecever` bot passed to it
 */

class OmniRandomKicker : public Behavior
{
public:
     OmniRandomKicker(Robot* whoIsRecever);
    ~OmniRandomKicker();
    void perform(Robot* robot);
    bool isFinished();
private:
    Robot* receiver;
    bool   bhasKicked;
    Skill::KickToPointOmni* ktp = nullptr;
};

/* BallReceiver
 * A behavior that waits around at `waitPoint` until the gm's prediction of the ball
 * is near the `waitPoint`, then moves to that point
 */
class BallReceiver : public GenericMovementBehavior
{
public:
    BallReceiver(Point pointToWaitAt);
    void perform(Robot* robot);
private:
    Point waitPoint;
    Point target;
};

/************************************************************************/
/* STRATEGIES */

/* VideoStrategy1
 * A strategy that coordinates two robots to continually pass and recieve 
 * the ball with each other
 */
class VideoStrategy1 : public Strategy
{
public:
     VideoStrategy1(Team* _team,int r1, int r2);
    ~VideoStrategy1();
    void assignBeh();
    bool update();
private:
    Point getSideFor(int);
    int r1ID, r2ID;
    Robot* currentPasser, *currentRecver;
};


/* VideoStrategy2
 * A single robot stays at a "waiting point" and, if the gameModel predicts the ball
 * point will be on the same side, moves to kick it back.
 */
class VideoStrategy2 : public Strategy
{
public:
    VideoStrategy2(Team* _team, int r0, Point pointToWaitAt);
    void assignBeh();
    bool update();
private:
    Robot* robot;
    int   waitTimer;
    Point waitPoint;
    enum { NONE, WAITING, KICKING} state;
};


/* VideoStrategy3
 * A simplified version of VideoStrategy2 in which a robot sits at the penalty point
 * and kicks the ball back if it comes on the side of it. Works with blue team.
 */
class VideoStrategy3 : public Strategy
{
public:
    VideoStrategy3(Team* _team, int who);
    void assignBeh();
    bool update();
private:
    Robot* guy;
    enum {NONE, KICKING} state = NONE;
};

/* VideoStrategy4
 * Robots on each side of the field kick the ball to the other side if
 * the ball is on their side.
 * Hard-coded for RoboCup Qualification video
 */
class VideoStrategy4 : public Strategy
{
public:
    VideoStrategy4(Team* _team,int r1, int r2);
    void assignBeh();
    bool update();
private:
    int r1ID, r2ID;
    Robot* r1;
    Robot* r2;
    Point bp, bpPredict = Point(0,0);
};

/* VideoStrategy5
 * Goal Keeper vs Attacker Scenario
 */
class VideoStrategy5 : public Strategy
{
public:
    VideoStrategy5(Team* _team, int keeper, int kicker);
    void assignBeh();
    bool update();
private:
    int keeperID, kickerID;
    int kick_timer = 0;
    Robot* keeper;
    Robot* kicker;
    Point bp, target;
};

}

//! @endcond

#endif
