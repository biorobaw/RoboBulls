#ifndef SHOOTTOGOAL_RL_H
#define SHOOTTOGOAL_RL_H
#include "utilities/point.h"
#include "model/robot/behavior.h"
#include "model/robot/navigation/commands/CmdGoToPose.h"
#include "gui/interface/gui_interface.h"

//you need the undef around it because of Qt
#undef slots
#include<torch/torch.h>
#define slots Q_SLOTS
#include<torch/script.h>


/*
 * GoToBall behavior learned from Deep Deterministic Gradiant Policy
 *
 * Referenced: https://zhuyifengzju.github.io/files/2018Robocup.pdf
 *
 * This now uses a Torch Script module for predictions. This means we train in rSoccer in python and export the model to be used here
 * Training is exponentially faster, and it may not even work well using grsim at all. openAIgym(rSoccer runs gym environments) used to allow C++
 * interface through html, but this no longer works. Be careful that the State space and Action space is defined the same(using same units of measurement
 * and same reference frame). see https://github.com/justinandrewrodney/SSL_DDPG_rSoccer for more details.
 */
class ShootToGoalRL : public Behavior
{
public:
    std::chrono::time_point<std::chrono::high_resolution_clock> time_start, time_stop, time_last;

    /*!
     * \brief ShootToGoalRL
     * \param robot
     * \param targetTolerance
     */
    ShootToGoalRL(Robot* robot, float targetTolerance = -1);

    bool perform() override;
    bool isFinished() override;
    string getName() override;

private:
    torch::jit::script::Module actor;
    bool done;
    void takeAction(std::vector<float> actions);

    int max_episode_length= 58/*350*/;

    float max_v;
    float max_w;
    float model_field_length;
    float model_goal_width;

    Point gp_top;
    Point gp_bottom;



    CmdGoToPose cmd = CmdGoToPose(Point(0,0),0,true,false);
    Point  m_targetPoint;         //Local (static-point) target stored only by first ctor
    Point* m_targetPointer;       //Pointer to point we are kicking to (m_targetPoint for static)
    int  m_moveCompletionCount; //Number of times move_skill says we are behind ball
    Point behindBall;             //Point behind the ball to navigate to for alighnment with target

    Point Original_bp;

    int step_num = 0;
    int cur_ep = 0;
    float total_reward = 0;
    std::pair<float, bool> getRewardAndDone(float d, float theta_r_b);
    std::vector<float> getState();

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

};



#endif //SHOOTTOGOAL_RL_H
