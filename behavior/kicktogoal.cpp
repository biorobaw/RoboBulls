#include "kicktogoal.h"

#include "model/gamemodel.h"
#include "skill/skill.h"
#include "skill/gotopositionwithorientation.h"
#include "skill/kick.h"

KickToGoal::KickToGoal(const ParameterList& list)
{
    UNUSED_PARAM(list);
    state = goingBehind;
}

void KickToGoal::perform(Robot * r)
{
    GameModel* gm = GameModel::getModel();

    Point ball = gm->getBallPoint();
    Point goal = gm->getOpponentGoal();
    float goalToBall = Measurments::angleBetween(goal,ball);
    float ballToGoal = Measurments::angleBetween(ball, goal);
    Point behindBall(50*cos(goalToBall)+ball.x, 50*sin(goalToBall)+ball.y);

    // Create a different skill depending on the state
    Skill::Skill * skill;
    switch (state) {
    case goingBehind:
        skill = new Skill::GoToPositionWithOrientation(behindBall, ballToGoal);
        break;
    case approaching:
        skill = new Skill::GoToPositionWithOrientation(ball, ballToGoal);
        break;
    case kicking:
        skill = new Skill::Kick();
        break;
    }

    // Perform the skill
    skill->perform(r);

    // Evaluate possible transitions
    switch (state){
    case goingBehind:
        if (Measurments::distance(behindBall, r->getRobotPosition()) < 100 && abs(Measurments::angleDiff(r->getOrientation(), ballToGoal)) < 10 * M_PI/180)
            state = approaching;
        break;
    case approaching:
        if (Measurments::distance(ball, r->getRobotPosition()) < 110 && abs(Measurments::angleDiff(r->getOrientation(), ballToGoal)) < 10 * M_PI/180)
            state = kicking;
        else if (Measurments::distance(ball, r->getRobotPosition()) > 200)
            state = goingBehind;
        break;
    case kicking:
        state = goingBehind;
    }

}
