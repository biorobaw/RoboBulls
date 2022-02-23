#ifndef GOTOBALLANDSHOOT_RL_H
#define GOTOBALLANDSHOOT_RL_H
#include "utilities/point.h"
#include "model/robot/behavior.h"
#include "model/robot/navigation/commands/CmdGoToPose.h"
#include "gui/interface/gui_interface.h"

#include "gotoballRL.h"
#include "shoottogoalRL.h"




class GoToBallAndShootRL : public Behavior
{
public:
    /*!
     * \brief GoToBallAndShootRL
     * \param robot
     * \param targetTolerance
     */
    GoToBallAndShootRL(Robot* robot, float targetTolerance = -1);

    bool perform() override;
    bool isFinished() override;
    string getName() override;

private:
    bool done;

    bool start_flag, end_flag;

    std::chrono::time_point<std::chrono::high_resolution_clock> time_start, time_got_ball, time_shot;

    string write_file;
    std::ofstream file_out;



    GoToBallRL *go_to_ball_skill;
    ShootToGoalRL *shoot_to_goal_skill;




//    CmdGoToPose cmd = CmdGoToPose(Point(0,0),0,true,false);
//    Point  m_targetPoint;         //Local (static-point) target stored only by first ctor
//    Point* m_targetPointer;       //Pointer to point we are kicking to (m_targetPoint for static)
//    int  m_moveCompletionCount; //Number of times move_skill says we are behind ball
//    Point behindBall;             //Point behind the ball to navigate to for alighnment with target

    Point Original_bp;

    float max_v;
    float max_w;


    void printState(std::vector<float> observation);
    void printAction();


    // Querying information to help switch states
    bool isFacingBall(Robot* robot);
    bool isFacingTarget(Robot* robot);
    bool isCloseToBall(Robot* robot);
    bool isVeryFarFromBall(Robot* robot);
    bool isWithinKickDistance(Robot* robot);
    bool isInKickLock(Robot* robot);
    bool canKick(Robot *robot);
//    std::vector<float> prev_observation;
//    std::vector<float> action;

    enum { GOTOBALL,  //We are far from the ball and are moving behind it to face target
           PICKUPBALL,
           SHOOTTOGOAL
         } state;


};



#endif //GOTOBALLANDSHOOT_RL_H
