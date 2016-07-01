#ifndef ATTACK_MAIN_H
#define ATTACK_MAIN_H
#include "behavior.h"
#include "include/config/simulated.h"
#include "skill/kicktopointomni.h"
#include "skill/dribbletopoint.h"
#include "model/gamemodel.h"
#include "gui/guiinterface.h"
#include "utilities/region/sector.h"
#include "genericmovementbehavior.h"
#include "algorithm"
#include "utilities/region/defencearea.h"
#include "utilities/comparisons.h"

#include <vector>

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

#define PND_MAIN 30  //Distance between nodes in the probability field
// Probability Field Variables
#define PF_LENGTH_MAIN (FIELD_LENGTH+1)/PND_MAIN
#define PF_WIDTH_MAIN  (FIELD_WIDTH +1)/PND_MAIN
#define PF_SIZE_MAIN  PF_LENGTH_MAIN * PF_WIDTH_MAIN

class AttackMain:public GenericMovementBehavior
{
public:
    // Fills in prob_field with scoring probabilities based on static factors
    AttackMain();
   ~AttackMain();
    void perform(Robot *);
    bool isFinished() override;
    bool hasKickedToGoal();
    bool hasPassed();

private:
    struct ProbNode
    {
        Point point;
        float static_val;
        float dynamic_val;
    };

    ProbNode prob_field[(FIELD_LENGTH+1)/PND_MAIN][(FIELD_WIDTH+1)/PND_MAIN];

    Skill::KickToPointOmni* score_skill;
    Skill::KickToPointOmni* pass_skill;
    Skill::DribbleToPoint* dribble_skill;
    Point kick_point;
    bool has_kicked_to_goal = false;
    bool has_passed = false;

    // Fills in prob_field with scoring probabilities
    void calcStaticProb();
    void calcDynamicProb();

    // Populates clusters with groups of opponents close together
    std::vector<std::vector<Point>> genClusters();

    // Returns true if there is a clear shot into the goal from the robot's position
    // If returning true, also returns the point along the goal post at which to aim
    std::pair<bool, Point> calcBestGoalPoint(Robot*);

    // Returns true if there is a robot that we can pass to
    // If returning true, also returns the point at which to aim
    std::pair<bool, Point> calcBestPassPoint(Robot*);

    // Returns the probability of scoring given a Point
    float getScoreProb(const Point&);

    enum {scoring, passing, dribbling} state;

    // Counters to reduce the impact of noise
    // on the state machine
    int clear_shot_count = 0;
    int clear_pass_count = 0;

};

#endif // ATTACK_MAIN_H
