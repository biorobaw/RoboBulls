#ifndef VIDEO_STRATEGIES_H
#define VIDEO_STRATEGIES_H

#include "strategy/strategy.h"
#include "behavior/behavior.h"
#include "behavior/genericmovementbehavior.h"
#include "skill/kicktopointomni.h"

/*
 * OMNI-ROBOT TEST GIT BRANCH --- TEMPORARY FILE 
 * This is a temporary file meant to provide functionality for the Robocup
 * Qulification 2015 video.
 */

class Robot;

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
     VideoStrategy1(int r1, int r2);
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
    VideoStrategy2(int r0, Point pointToWaitAt);
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
    VideoStrategy3(int who);
    void assignBeh();
    bool update();
private:
    Robot* guy;
    enum {NONE, KICKING} state = NONE;
};

/* VideoStrategy4
 * The robot kicks the ball into the goal if it near the opponent goal post.
 * Hard-coded for RoboCup Qualification video
 */
class VideoStrategy4 : public Strategy
{
public:
    VideoStrategy4(int who);
    void assignBeh();
    bool update();
private:
    Robot* guy;
    Point bp;
    bool done_kicking = 0;
    bool ball_near_goal = 0;
};

#endif
