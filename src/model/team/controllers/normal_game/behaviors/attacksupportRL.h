#ifndef ATTACK_SUPPORTRL_H
#define ATTACK_SUPPORTRL_H
#include "model/robot/behaviors/genericmovementbehavior.h"
#include "utilities/region/defencearea.h"
#include <algorithm>
#include "utilities/comparisons.h"

/*! @file
 * AttackSupport tries to help score a goal by positioning a robot in an opportune position.
 *
 * A robot assigned this behavior will try to move to a position that satisfies the following criteria:
 * - There are no obstacles between the position and the goal.
 * - There are no obstacles between the position and the ball.
 * - The position is far enough from a ball possessor to make the pass viable.
 * - The position is not between the ball and the goal, to avoid blocking shots on goal.
 *
 * A position satisfying all four criteria is found by casting "shadows" on a probability field,
 * as described (in more detail) in AttackMain. @see AttackMain()
 *
 * The points in the probability field (discrete represenation of actual field)
 * are assigned values as follows:
 * - Decreasing probabilities as the points get farther from the goal post.
 * - Decreasing probabilities as the points get farther from the centerline stretching from one goal post to the other.
 * - Zero probability in the triangle between the ball point and the goal post (plus some tolerance).
 * - Zero probability in a circle around the ball, to avoid passes that are too short.
 */

// Distance between nodes in the probability field
// (Probability Node Distance)
#define PND_SUPP 30

// Probability Field Variables
#define PF_LENGTH_SUPP (Field::FIELD_LENGTH+1)/PND_SUPP
#define PF_WIDTH_SUPP  (Field::FIELD_WIDTH +1)/PND_SUPP
#define PF_SIZE_SUPP  PF_LENGTH_SUPP * PF_WIDTH_SUPP

class AttackSupportRL : public Behavior //public GenericMovementBehavior
{
public:
    // Fills in prob_field with scoring probabilities based on static factors
    AttackSupportRL(Robot*);
   ~AttackSupportRL();
    bool perform() override;
    bool isFinished() override;
    string getName() override;

private:
    bool finished;
    enum {intercept, position} state;

    //Store initial point of passing robot
    bool recievingPass = false;
    Robot *recieve_from;


    // Counters to reduce the impact of noise on the state machine
    int switch2intercept_count = 0;
    int switch2position_count = 0;

    struct ProbNode
    {
        Point point;
        float static_val;   // Values that do not change during the game
        float dynamic_val;  // Values that change during the game
    };

    int prob_field_rows;
    int prob_field_cols;
//    ProbNode prob_field[(Field::FIELD_LENGTH+1)/PND_SUPP][(Field::FIELD_WIDTH+1)/PND_SUPP];
    ProbNode** prob_field;

    // Fills in prob_field with scoring probabilities
    void calcStaticProb();
    void calcDynamicProb();

    /*! Iterates through the probability nodes to find the one with the
     * maximum value (static + dynamic). Only contains points on the opponent side.
     * @return ProbNode The node with the max probability.
     */
    ProbNode findMaxNode();

    /*! Cast shadows using the entire goal post as a light source and opp
     * robots as opaque objects to rule out impossible scoring positions */
    void genGoalShadows();

    /*! Sets a dead-zone near teammates so that the support attacker
     * maintains a good distance from them to make passes meaningful
     * @param Robot* Needed for excluding the support attacker itself from distance calculations.
     */

    void genDistanceFromTeammates();

    /*! Cast shadows with ball as light source and opponents as opaque objects
     * Rules out impossible receiving positions
     * @todo Integrate clustering to this */
    void genBallShadows();

    /*! Set the probability in the triangle between the ball and the goal-post to 0
     * so that the support attacker doesn't position in the way of a shot on goal */
    void genGoalShotAvoidance();

    /*! Returns information about clusters of robots.
     * @return A 2D vector is returned with each row representing a cluster.
     * Each column in the returned 2D vector points to a robot in the cluster.
     * A cluster is defined as a group of robots that are within a certain distance of each other.
     * The clustering distance is selected so as to cluster robots that are close such that
     * a ball could not pass between them.
     */
    std::vector<std::vector<Point>> genClusters( );

    /*! Given a point, returns the probability of scoring a goal from there.
     * This function assumes that probability is already calculated and stored in
     * the probability array by calcDynamicProb() and calcStaticProb().
     * @see calcDynamicProb() @see calcStaticProb().
     * @param Point The point at which the probability is queried.
     * @return Probability of scoring.
     */
    float getScoreProb(const Point&);
};

#endif // ATTACK_SUPPORTRL_H
