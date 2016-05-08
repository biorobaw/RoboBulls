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
 * too far from the start point, it will make a pass is the supporter is
 * at least some distance away and we haven't told it to always kick to goal (forceGoalKick)
 * @see AttackSupport */

class AttackMain:public Behavior
{
public:
    AttackMain(Robot* supp_attacker = nullptr, bool forceGoalKick = false);
   ~AttackMain();
    void perform(Robot *);
    bool isFinished() override;

private:
    GameModel * gm;
    Robot* support_attacker;
    Point drive_start_point, rp, sp, gp, bp;

    double goal_direction;
    const double shot_distance = 2000;
    const double drive_distance = 200;

    bool touched_ball = false, done = false, forcedGoalKick = false;
    Skill::Skill* skill;

    enum states { initial, driving, end } state;

    // Returns true if there is a clear shot into the goal from the robot's position
    // If returning true, also returns the point along the goal post at which to aim
    std::pair<bool, Point> calcBestGoalPoint(Robot*);
};

#endif // ATTACK_MAIN_H
