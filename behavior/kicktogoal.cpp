#include "kicktogoal.h"

#include "model/gamemodel.h"
#include "skill/skill.h"
#include "skill/kick.h"

#if SIMULATED
    #define ANGLE   (10 * M_PI/180)
    #define CLOSE_TO_BALL 110
    #define BEHIND_RADIUS (ROBOT_RADIUS*2.25)
    #define CLOSE_ENOUGH 110
#else
    #define ANGLE   (15 * M_PI/180)
    #define CLOSE_TO_BALL 110
    #define BEHIND_RADIUS (ROBOT_RADIUS*2.25)
    #define CLOSE_ENOUGH 220
#endif

KickToGoal::KickToGoal(const ParameterList& list)
	: GenericMovementBehavior(list)
{
    UNUSED_PARAM(list);
    state = goingBehind;
}

void KickToGoal::perform(Robot * r)
{
    GameModel* gm = GameModel::getModel();
    Point ball = gm->getBallPoint();
    Point goal = gm->getOpponentGoal();
	Point rob  = r->getRobotPosition();
	float robAng = r->getOrientation();
    float goalToBall = Measurments::angleBetween(goal,ball);
    float ballToGoal = Measurments::angleBetween(ball, goal);

    Point behindBall(BEHIND_RADIUS*cos(goalToBall)+ball.x,
                     BEHIND_RADIUS*sin(goalToBall)+ball.y);

    // Create a different skill depending on the state
    switch (state) {
    case goingBehind:
        target = behindBall;
        setMovementTargets(behindBall, ballToGoal, false);
        GenericMovementBehavior::perform(r, Movement::Type::Default);
        break;
    case approaching:
        setMovementTargets(ball, ballToGoal, false);
		GenericMovementBehavior::perform(r, Movement::Type::SharpTurns);
        break;
    case kicking:
        {
            Skill::Kick k(0, 0);
            k.perform(r);
		}
        break;
    }

    // Evaluate possible transitions
    switch (state){
    case goingBehind:
        cout << "going behind" << endl;
//        cout << "1\t" << Measurments::distance(behindBall, rob) << endl;
//        cout << "2\t" << abs(Measurments::angleDiff(robAng, ballToGoal))/M_PI*180 << endl;
        if (Measurments::distance(behindBall, rob) < CLOSE_ENOUGH  &&
            Measurments::isClose(robAng, ballToGoal, ANGLE)) {
            state = approaching;
            target = ball;
        }
        else if (!Measurments::isClose(target, behindBall, CLOSE_ENOUGH))
        {
            state = goingBehind;
        }
        break;
    case approaching:
        cout << "approching" << endl;
//        cout << "1\t" << Measurments::distance(ball, rob) << endl;
//        cout << "2\t" << abs(Measurments::angleDiff(robAng, ballToGoal))/M_PI*180 << endl;
        if (Measurments::distance(ball, rob) < CLOSE_ENOUGH &&
            Measurments::isClose(robAng, ballToGoal, ANGLE))
            state = kicking;
        else if (!Measurments::isClose(target, ball, CLOSE_ENOUGH))
        {
            state = goingBehind;
        }
//        else if (Measurments::distance(ball, rob) > CLOSE_ENOUGH*2) {
//            state = goingBehind;
//        }
        break;
    case kicking:
        cout << "kicking" << endl;
        state = goingBehind;
        break;
    }
}
