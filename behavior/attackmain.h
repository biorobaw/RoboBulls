#ifndef ATTACK_MAIN_H
#define ATTACK_MAIN_H

#include "behavior.h"
#include "skill/driveball.h"
#include "utilities/measurments.h"
#include "model/gamemodel.h"
#include "model/robot.h"

/*** Recycled behavior.
 * Old description:
 * Used in the TwoVOne strategy to control the robot that travels with the ball
 * TwoVOne assigns this behavior to the robot closer to the ball
 * The complementary behavior is AttackSupport
 *
 * New description:
 * Used with NormalGameStrategy. Modernized to work better with arbitrary-ID
 * robots, and is able to return more information about itself (hasKicked)
 */

class AttackMain:public Behavior
{
public:
    AttackMain(const ParameterList & list);
   ~AttackMain();
    void perform(Robot *);
    bool hasKicked();
private:
    GameModel * gm;
    Robot* support_attacker;
    Point drive_start_point, rp, sp, gp, bp, stp;

    double goal_direction;
    const double shot_distance = 1500;
    const double drive_distance = 1000;

    bool touched_ball = false, done = false;
    Skill::Skill* drive_skill;
	Skill::Skill* pass_skill;
	Skill::Skill* score_skill;

    enum states { drive, pass, score, initial } state;

};

#endif // ATTACK_MAIN_H
