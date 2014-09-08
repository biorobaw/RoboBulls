#include "kicktogoal.h"

#include "model/gamemodel.h"
#include "skill/skill.h"
#include "skill/kick.h"

#define TEN_DEGREES   (10 * M_PI/180)
#define CLOSE_TO_BALL 110
#define BEHIND_RADIUS (ROBOT_RADIUS*2.25)

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
        setMovementTargets(behindBall, ballToGoal);
		GenericMovementBehavior::perform(r, Movement::Type::SharpTurns);
        break;
    case approaching:
        setMovementTargets(ball, ballToGoal, false);
		GenericMovementBehavior::perform(r, Movement::Type::SharpTurns);
        break;
    case kicking:
        {
			Skill::Kick k;
            k.perform(r);
		}
        break;
    }

    // Evaluate possible transitions
    switch (state){
    case goingBehind:
        if (Measurments::distance(behindBall, rob) < 100  &&
            Measurments::isClose(robAng, ballToGoal, TEN_DEGREES)) {
            state = approaching;
        }
        break;
    case approaching:
        if (Measurments::distance(ball, rob) < 110 &&
            Measurments::isClose(robAng, ballToGoal, TEN_DEGREES))
            state = kicking;
        else if (Measurments::distance(ball, rob) > 200) {
            state = goingBehind;
        }
        break;
    case kicking:
        state = goingBehind;
        break;
    }
}
