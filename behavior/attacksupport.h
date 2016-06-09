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

// Distance between nodes in the probability field
// (Probability Node Distance)
#define PND_SUPP 30

// Probability Field Variables
#define PF_LENGTH_SUPP (FIELD_LENGTH+1)/PND_SUPP
#define PF_WIDTH_SUPP  (FIELD_WIDTH +1)/PND_SUPP
#define PF_SIZE_SUPP  PF_LENGTH_SUPP * PF_WIDTH_SUPP

class AttackSupport : public GenericMovementBehavior
{
public:
    // Fills in prob_field with scoring probabilities based on static factors
    AttackSupport();
   ~AttackSupport();
    void perform(Robot *) override;
    bool isFinished() override;

private:
    bool finished;

    struct ProbNode
    {
        Point point;
        float static_val;
        float dynamic_val;
    };

    ProbNode prob_field[(FIELD_LENGTH+1)/PND_SUPP][(FIELD_WIDTH+1)/PND_SUPP];

    // Fills in prob_field with scoring probabilities
    void calcStaticProb();
    void calcDynamicProb(Robot * robot);
    ProbNode findMaxNode();

    void genGoalShadows();
    void genDistanceFromTeammates(Robot* robot);
    void genBallShadows();
    void genGoalShotAvoidance();

    // Populates clusters with groups of opponents close together
    std::vector<std::vector<Point>> genClusters();

    // Returns true if there is a robot that we can pass to
    // If returning true, also returns the point at which to aim
    std::pair<bool, Point> calcBestSupportPoint(Robot*);

    // Returns the probability of scoring given a Point
    float getScoreProb(const Point&);

    enum {intercept, position} state;

    int switch2intercept_count = 0;
    int switch2position_count = 0;
};

#endif // ATTACK_SUPPORT_H
