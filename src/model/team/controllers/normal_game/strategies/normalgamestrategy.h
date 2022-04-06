#ifndef NORMALGAMESTRATEGY_H
#define NORMALGAMESTRATEGY_H
#include "model/team/team_strategy.h"
#include "utilities/region/defencearea.h"
class Robot;

//delete
#include <fstream>      // std::ofstream


/*! @file
 * This strategy is meant for the ' ' gamestate (space) which means
 * normal play.
 */
class NormalGameStrategy : public TeamStrategy
{
public:
    NormalGameStrategy(RobotTeam* _team);
    void assignBehaviors() override;
    void runControlCycle() override;
    virtual QString getName() override;
    int getStatus() override;

public:
    /*! Assigns goalie behavior to ID GOALIE_ID if it is on the team
     * Meant to be used outside the class, if the robot with GOALIE_ID
     * isn't null, assigns Goalie to it */
    static void assignGoalieIfOk(RobotTeam* team);

private:
    //ToDelete
    bool start_flag, end_flag;
    std::chrono::time_point<std::chrono::high_resolution_clock> time_start, time_got_ball, time_shot;
    std::string write_file;
    std::ofstream file_out;
    bool picked_up_ball_flag;
    //***********


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
