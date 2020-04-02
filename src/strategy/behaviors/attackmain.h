#ifndef ATTACK_MAIN_H
#define ATTACK_MAIN_H

#include "../behavior.h"
#include "../skills/kicktopointomni.h"
#include "../skills/dribbletopoint.h"
#include "model/game_state.h"
#include "gui/guiinterface.h"
#include "utilities/region/sector.h"
#include "genericmovementbehavior.h"
#include "algorithm"
#include "utilities/region/defencearea.h"
#include "utilities/comparisons.h"


#include <vector>

/*! @file
 * AttackMain tries to emulate the behavior of a striker/forward attacker in real soccer.
 *
 * A robot assigned this behavior will prioritize the following objectives:
 * - Kicking to the goal if a clear shot is available.
 * - Passing to a teammate if a clear pass is available.
 * - Dribbling the ball to a position where 1) or 2) is possible.
 *
 * Objectives 1) and 2) are straight-forward in terms of implementation.
 * We simply check if the path between the ball and the opponent goal post is
 * sufficiently clear of obstacles.
 *
 * Objective 3) requires the calculation of a point in the field from which
 * it is possible to score. This calculation is faciliated by a
 * probability field which stores the probability of scoring from a each point
 * in a sparse discrete representation of the field. A sparse represenation is
 * used, as opposed to processing every point possible within the field boundaries,
 * for performance reasons.
 *
 * The points in the probability field (discrete represenation of actual field)
 * are assigned values as follows:
 * - Decreasing probabilities as the points get farther from the goal post
 * - Decreasing probabilities as the points get farther from the centerline stretching from one goal post to the other
 * - Zero probability in the areas covered by opponent robots
 */

// Distance between nodes in the probability field
// (Probability Node Distance)
#define PND_MAIN 30

// Probability Field Variables
#define PF_LENGTH_MAIN (Field::FIELD_LENGTH+1)/PND_MAIN
#define PF_WIDTH_MAIN  (Field::FIELD_WIDTH +1)/PND_MAIN
#define PF_SIZE_MAIN  PF_LENGTH_MAIN * PF_WIDTH_MAIN




class AttackMain : public Behavior//:public GenericMovementBehavior
{
public:
    AttackMain(Robot*);
   ~AttackMain();
    void perform() override;
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

    int prob_field_rows;
    int prob_field_cols;
    ProbNode** prob_field;

    Skill::KickToPointOmni* score_skill;
    Skill::KickToPointOmni* pass_skill;
    Skill::DribbleToPoint* dribble_skill;
    Point kick_point;
    bool has_kicked_to_goal = false;
    bool has_passed = false;

    enum {scoring, passing, dribbling} state;

    // Counters to reduce the impact of noise on the state machine
    int clear_shot_count = 0;
    int clear_pass_count = 0;

    /* Angle tolerances for kicking in degrees (then converted to radians).
     * Passing is lower because it needs to be more precise */
    static float SCORE_ANGLE_TOLERANCE;
    static float PASS_ANGLE_TOLERANCE;

    /*! Calculates the probility of scoring from each point in the field based on field geometry (fixed factors).
     *  Called only once in the constructor.
     */
    void calcStaticProb();

    /*! Calculates the probility of scoring from each point in the field based on opponents, teammates, and the ball (variable factors).
     *  Called for every update to the game model.
     */
    void calcDynamicProb();

    /*! Returns information about clusters of robots.
     * @return A 2D vector is returned with each row representing a cluster.
     * Each column in the returned 2D vector points to a robot in the cluster.
     * A cluster is defined as a group of robots that are within a certain distance of each other.
     * The clustering distance is selected so as to cluster robots that are close such that
     * a ball could not pass between them.
     */
    std::vector<std::vector<Point>> genClusters();

    /*! Calculates the best point at which to aim in order to score a goal.
     *
     * If it is impossible to score a goal from the current ball position,
     * the boolean in the returned pair will be false. The Point in the
     * pair should be disregarded.
     *
     * Otherwise, if it is possible to score, the boolean will be true and
     * the returned Point will be the best place to aim.
     *
     * @param Robot* The robot that is taking the kick.
     * @return A boolean and a point, contained in a C++ pair.
     */
    std::pair<bool, Point> calcBestGoalPoint();

    /*! Calculates the best point to pass to a teammate.
     *
     * If it is impossible to pass from the current ball position,
     * the boolean in the returned pair will be false. The Point in the
     * pair should be disregarded.
     *
     * Otherwise, if a pass is possible, the boolean will be true and
     * the returned Point will be the best place to aim.
     *
     * @param Robot* The robot that is sending the pass.
     * @return A boolean and a point, contained in a C++ pair.
     */
    std::pair<bool, Point> calcBestPassPoint();

    /*! Given a point, returns the probability of scoring a goal from there.
     * This function assumes that probability is already calculated and stored in
     * the probability array by calcDynamicProb() and calcStaticProb().
     * @see calcDynamicProb() @see calcStaticProb().
     * @param Point The point at which the probability is queried.
     * @return Probability of scoring.
     */
    float getScoreProb(const Point&);
};

#endif // ATTACK_MAIN_H
