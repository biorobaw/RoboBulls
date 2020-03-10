#ifndef NORMALGAMESTRATEGY_H
#define NORMALGAMESTRATEGY_H
#include "robot/robot.h"
#include "../strategy.h"
#include "utilities/region/defencearea.h"
#include "../behaviors/genericmovementbehavior.h"

/*! @file
 * This strategy is meant for the ' ' gamestate (space) which means
 * normal play.
 */
class NormalGameStrategy : public Strategy
{
public:
    NormalGameStrategy(RobotTeam* _team);
    void assignBehaviors() override;
    void update() override;

public:
    /*! Assigns goalie behavior to ID GOALIE_ID if it is on the team
     * Meant to be used outside the class, if the robot with GOALIE_ID
     * isn't null, assigns Goalie to it */
    static void assignGoalieIfOk(RobotTeam* team);

private:
    Point initialBallPos;
    DefenceArea our_def_area;
    DefenceArea opp_def_area;
    bool clearing_ball = false;

    // Used to switch when ball is closer to the other
    Robot* supp = nullptr;
    Robot* main = nullptr;

    enum {opp_kickoff, our_kickoff_1, our_kickoff_2, our_kickoff_3,
          shoot_penalty, defend_penalty,
          evaluate, attack, defend, goalkick, clearball} state, prev_state;
};

#endif // NORMALGAMESTRATEGY_H
