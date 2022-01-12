#ifndef GOTOBALL_RL_H
#define GOTOBALL_RL_H
#include "utilities/point.h"
#include "model/robot/behavior.h"
#include "model/robot/navigation/commands/CmdGoToPose.h"
#include "gui/interface/gui_interface.h"
#include "actorcritic.h"
#include "ddpg.h"
/*
 * GoToBall behavior learned from Deep Deterministic Gradiant Policy
 *
 * Referenced: https://zhuyifengzju.github.io/files/2018Robocup.pdf
 */
class GoToBallRL : public Behavior
{
public:
    /*! @brief <b>Static-point</b> constructor, for kicking to a static point
    * @param target Target point to kick toarwards
    * @param targetTolerance: Tolerance for the robot angle facing the target point before
    * a kick is made, for accuracy or non-accuracy reasons. Leave blank to use default.
    * @param kickDistance: How close should the robot be to the target before kicking.
    * leave blank to kick as soon as possible
    * @param useFullPower: Use a full-power kick instead of a variable power one to the target */
    GoToBallRL(Robot* robot,
                    float targetTolerance = -1,
                    float kickDistance = -1,
                    bool  useFullPower = false);

    bool perform() override;
    bool isFinished() override;
    string getName() override;

private:
    CmdGoToPose cmd = CmdGoToPose(Point(0,0),0,true,false);
    Point  m_targetPoint;         //Local (static-point) target stored only by first ctor
    Point* m_targetPointer;       //Pointer to point we are kicking to (m_targetPoint for static)
      int  m_moveCompletionCount; //Number of times move_skill says we are behind ball
    Point behindBall;             //Point behind the ball to navigate to for alighnment with target


     Point Original_bp;

     //State input
     Point ball_pos; //relative to robot
     Point velocity; //translational
     float dist_to_ball;
     float dist_to_top_field;
     float dist_to_right_field;
     float dist_to_bot_field;
     float dist_to_left_field;

     //Action space
     Point vel;
     float angularVel;
     int step_num = 0;
     int cur_ep = 0;
     float total_reward = 0;
     bool train, /*episode_complete, train_complete,*/ done;
     EpisodeStatus episode_status;
     void resetEnvironment();
     void endTraining();
     float getReward(float d);
     std::vector<float> getState();
     void printState(std::vector<float> observation);
     void printAction();
     /*!
      * The steps after taking an action for Training and Testing are slightly different
      *  Could be condensed, but might reduce understandability
     */
     bool Train();
     bool Test();
     void Step();
     void BurnIn();
     std::vector<float> getBurnInAction();




    // Querying information to help switch states
    bool isFacingBall(Robot* robot);
    bool isFacingTarget(Robot* robot);
    bool isCloseToBall(Robot* robot);
    bool isVeryFarFromBall(Robot* robot);
    bool isWithinKickDistance(Robot* robot);
    bool isInKickLock(Robot* robot);
    bool canKick(Robot *robot);
    DDPG_Agent agent;
    std::vector<float> prev_observation;
    std::vector<float> action;
};



#endif //GOTOBALL_RL_H
