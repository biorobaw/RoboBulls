#ifndef ATTACK_MAIN_H
#define ATTACK_MAIN_H

#include "behavior.h"
#include "skill/driveball.h"
#include "utilities/measurments.h"
#include "model/gamemodel.h"
#include "model/robot.h"
#include "utilities/paramlist.h"

//Used in the TwoVOne strategy to control the robot that travels with the ball
//TwoVOne assigns this behavior to the robot closer to the ball
//The complementary behavior is AttackSupport

class AttackMain:public Behavior
{
public:
    AttackMain(const ParameterList & list);
    void perform(Robot *);
private:
    GameModel * gm;
    Point drive_start_point, rp, sp, gp, bp;

    double goal_direction;
    const double shot_distance = 1000;
    const double drive_distance = 500;

    bool touched_ball = false;
    Skill::Skill *drive_skill;

    enum states { drive, pass, score, initial } state;

};

#endif // ATTACK_MAIN_H
