#ifndef ATTACK_MAIN_H
#define ATTACK_MAIN_H
#include "behavior.h"
#include "model/gamemodel.h"

/*! @brief AttackMain is the main driver behavior for the NormalGameStrategy.
 * @author Muhaimen Shamsi, JamesW
 * @details A robot assigned this behavior assumes there is another robot
 * with AttackSupport assigned. The behavior evaluates distance
 * to the enemy goal--if too far, the robot will drive a distance
 * then pass to the Attacksupport robot. But if close enough, the
 * robot will shoot to the goal. In addition, if the robot drives
 * too far from the start point, it will make a pass.
 * @see AttackSupport */

class AttackMain:public Behavior
{
public:
    AttackMain(Robot* attacker = nullptr);
   ~AttackMain();
    void perform(Robot *);
    bool isFinished() override;

private:
    GameModel * gm;
    Robot* support_attacker;
    Point drive_start_point, rp, sp, gp, bp;

    double goal_direction;
    const double shot_distance = 1800;
    const double drive_distance = 500;

    bool touched_ball = false, done = false;
    Skill::Skill* drive_skill;
    Skill::Skill* pass_skill;
    Skill::Skill* score_skill;

    enum states { drive, pass, score, initial } state;

};

#endif // ATTACK_MAIN_H
