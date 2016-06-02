#ifndef ATTACK_SUPPORT_H
#define ATTACK_SUPPORT_H
#include "behavior/genericmovementbehavior.h"
#include "utilities/region/defencearea.h"
#include "model/gamemodel.h"
#include "model/robot.h"
#include <algorithm>
#include "utilities/comparisons.h"

/*! @brief AttackSupport is a complement to Attackmain, used in NormalGameStrategy.
 * @author Muhaimen Shamsi, JamesW
 * @details It goes to the most optimal half of the enemy field--the one with least
 * enemies, and the one that the AttackMain robot is not inside of--and waits
 * for a pass from the AttackMain robot. */

#define PND_SUP 30  //Distance between nodes in the probability field
// Probability Field Variables
#define PF_LENGTH_SUP (FIELD_LENGTH+1)/PND_SUP
#define PF_WIDTH_SUP  (FIELD_WIDTH +1)/PND_SUP
#define PF_SIZE_SUP  PF_LENGTH_SUP * PF_WIDTH_SUP

class AttackSupport : public GenericMovementBehavior
{
public:
    // Fills in prob_field with scoring probabilities based on static factors
    AttackSupport();
   ~AttackSupport();
    void perform(Robot *) override;
    bool isFinished() override;

private:
    struct ProbNode
    {
        Point point;
        float static_val;
        float dynamic_val;
    };

    ProbNode prob_field[(FIELD_LENGTH+1)/PND_SUP][(FIELD_WIDTH+1)/PND_SUP];

    // Fills in prob_field with scoring probabilities
    void calcStaticProb();
    void calcDynamicProb(Robot * robot);

    void genGoalShadows();
    void genDistanceFromTeammates(Robot* robot);
    void genBallShadows();

    // Populates clusters with groups of opponents close together
    std::vector<std::vector<Point>> genClusters();

    // Returns true if there is a robot that we can pass to
    // If returning true, also returns the point at which to aim
    std::pair<bool, Point> calcBestSupportPoint(Robot*);

    // Returns the probability of scoring given a Point
    float getScoreProb(const Point&);
};

#endif // ATTACK_SUPPORT_H
