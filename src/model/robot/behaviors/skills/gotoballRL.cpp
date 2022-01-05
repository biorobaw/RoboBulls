#include "model/game_state.h"
#include "utilities/comparisons.h"
#include "utilities/debug.h"
#include "gotoballRL.h"
#include "model/ball.h"
#include "model/field.h"
#include "model/robot/robot.h"
#include "model/robot/navigation/robot_pilot.h"
#include "model/game_state.h"
#include "model/team/team.h"
#include <iostream>
#include "utilities/measurements.h"

#include "ddpg.h"

#include <QDebug>
/*
 *Utilizing a similar state action space as prupose in: https://zhuyifengzju.github.io/files/2018Robocup.pdf
 *By Carnegie Mellon University
 *
 */

//float BEHIND_RAD_AVOID = ROBOT_RADIUS+Field::BALL_RADIUS + DIST_TOLERANCE; //was 50
//float BEHIND_RAD = ROBOT_RADIUS+Field::BALL_RADIUS;
//float FORWARD_WAIT_COUNT = 5;
//float RECREATE_DIST_TOL = 25;
//float STRICTEST_ANG_TOL = ROT_TOLERANCE;  //40 * (M_PI/180);
//float KICK_LOCK_ANGLE =  ROT_TOLERANCE;  //12 * (M_PI/180);
//float KICKLOCK_COUNT = 15;
//#endif

int steps_per_epoch = 4000;
int num_epochs = 5;
/************************************************************************/

GoToBallRL::GoToBallRL(Robot* robot, const Point& target,
                                 float targetTolerance, float kickDistance, bool useFullPower)
    : GoToBallRL(robot, &m_targetPoint, targetTolerance, kickDistance, useFullPower)
{
    m_targetPoint = target;
    train = true;
    episode_complete = false;
}
//DDPG_Agent(int o_size, int a_size, int update_after_numsteps = 1000, int update_every_numsteps = 50,
//int batchsize = 32, int steps_per_epoch = 4000);
GoToBallRL::GoToBallRL(Robot* robot, Point* targetPtr,
                                 float targetTolerance, float kickDistance, bool useFullPower)
    : Behavior(robot)
    , agent(/*num observations*/9,/*num actions*/ 3, steps_per_epoch, num_epochs)
    , m_targetPointer(targetPtr)
    , m_moveCompletionCount(0)
//    , m_targetTolerance((targetTolerance < 0) ? STRICTEST_ANG_TOL : targetTolerance)
//    , m_kickDistance(kickDistance)
//    , m_kickLockCount(0)
//    , m_hasRecoveredKickLock(true)
//    , m_useFullPower(useFullPower)
//    , m_lastBallAwayDist(0)
//    , m_ballMovingAwayCount(0)
//    , m_kickCommandCount(0)
//    , m_hasKicked(false)

{
    train = true;
    episode_complete = false;
    done = false;
    //qInfo() <<"New kick to point omni robot "<<robot->getId();
    //debug::registerVariable("ktpo_rc", &RECREATE_DIST_TOL);

}
std::vector<float> GoToBallRL::getState(){
//State input
Point ball_pos = (*robot - *ball).rotate(-(robot->angle())); //relative to robot
Point velocity = robot->getVelocity().rotate(-(robot->angle())); //translational
float dist_to_ball = Measurements::distance(*ball, *robot);
float dist_to_top_field = Field::FIELD_LENGTH/2 - robot->y;
float dist_to_right_field = Field::FIELD_WIDTH/2 - robot->x;
float dist_to_bot_field =  robot->y - (-Field::FIELD_LENGTH/2);
float dist_to_left_field = robot->x - (-Field::FIELD_WIDTH/2);
return {ball_pos.x, ball_pos.y, velocity.x, velocity.y, dist_to_ball, dist_to_top_field, dist_to_right_field, dist_to_bot_field, dist_to_left_field};
}



float angularVel;
bool GoToBallRL::perform()
{
    std::vector<float> observation = getState();

    if(episode_complete){
        if(train){
            resetEnvironment();
            if(agent.endTraining()){
                qInfo() <<"Max number episodes trained";
                return false;
            }
        }
        episode_complete = false;
        step_num = 0;
        total_reward = 0;
        done = false;

    }
    else if (step_num != 0){
        //Algorithm updates after each step, in other examples they are
        //able to step the simulation, but here we must perform the update
        //steps on the next call to perform.. cant do on step 0 since timestep needs to pass.
        //calculate reward and determine if done
       float reward = getReward(dist_to_ball);
       total_reward += reward;
       done = robot->hasBall(); // add comparison

       //update stores replay buffer and performs updating to the networks.
       //returns whether episode is complete
       if(train)
            episode_complete = agent.step(prev_observation, action, reward,observation, done);
       else
           episode_complete = done || agent.endEpisode();

    }
    if(episode_complete)
            return done;

    //Get action from ddpg_agent actor_critic network.
    action = agent.Act(observation, train /*adds noise if training*/);

    //Action space
    Point vel(action[0], action[1]);
    float angularVel = action[2];

    robot->setTargetVelocityLocal(vel, angularVel);
    prev_observation = observation; step_num++;

    //Normally we would update here(simulator examples), but we must wait for the next cycle

    //DDPG
    return done; // was false
}

//The following are utility functions to help switch state.
//- canKick: All conditions okay to actuate the kicker
//- isWithinKickDistnace: "Within kick distance" means we are close to the target to kick to it
//- isCloseToBall: We are close to the ball if we are within kicking distance
//- isVeryFarFromBall: True if we are pretty far away from the ball
//- isFacingBall: True if facing the ball. The tolerance angle is the user-given m_targetTolerance
/*- isInKickLock: "Kick-lock" is a side-effect of robot physical geometry.
    * Sometimes the robot gets too close to the ball while not facing it, and keeps
    * pushing the ball along in which it cannot get behind it. This helps to detect that */

//bool GoToBallRL::canKick(Robot* robot) {
//    bool closeToBall = isCloseToBall(robot);
//    bool facingBall = isFacingBall(robot);
//    bool facingTarget = isFacingTarget(robot);
//    bool withinKickDist =  isWithinKickDistance(robot);
//    return m_hasRecoveredKickLock &&
//           closeToBall && facingBall && facingTarget
//           && withinKickDist;
//}

//bool GoToBallRL::isWithinKickDistance(Robot *robot) {
//    return m_kickDistance == -1 || Measurements::distance(robot, *m_targetPointer) < m_kickDistance;
//}

//bool GoToBallRL::isCloseToBall(Robot *robot) {
//    // We cannot simply check the distance to the ball because it is unreliable
//    // A misplacement of the markings on top of the robot can render the check tolerance
//    // too much or too little. Instead we check how far the robot has travelled from
//    // the behindball point.
//    //std::cout << measurements::distance(robot, behindBall) << std::endl;
//    return Measurements::distance(robot, behindBall) >= DIST_TOLERANCE; //was 30
//}

//bool GoToBallRL::isVeryFarFromBall(Robot *robot) {
//    return Measurements::distance(robot, *ball) > ROBOT_RADIUS*6;
//}

//bool GoToBallRL::isFacingBall(Robot* robot) {
//    return Comparisons::isFacingPoint(robot, *ball, M_PI/3.0);
//}

//bool GoToBallRL::isFacingTarget(Robot* robot) {
    //return Comparisons::isFacingPoint(robot, *m_targetPointer, m_targetTolerance);
//}

//bool GoToBallRL::isInKickLock(Robot* robot)
//{
//    bool close = isCloseToBall(robot);
//    bool facingBall = Comparisons::isFacingPoint(robot, *ball, KICK_LOCK_ANGLE);
//    if(close && !facingBall) {
//        if(++m_kickLockCount > KICKLOCK_COUNT) {
//            m_kickLockCount = 0;
//            m_hasRecoveredKickLock = false;
//            return true;
//        }
//    }
//    return false;
//}

void GoToBallRL::resetEnvironment(){
    qInfo() << "episode complete, reset training environment(hit enter to continue):";
    std::cin >>;
}
float GoToBallRL::getReward(float d){
   float reward = 0;

   //rtotal = rcontact + rdistance + rorientation

   if(robot->hasBall()) //contact...perhaps change to robot has ball on dribbler like in paper?
       reward+=100;

   //distance
   reward +=( (5/sqrt(2*M_PI))*exp(-(d*d)/2) )- 2;
   //orientation
   float theta_r_b  = fabs(Measurements::angleDiff(Measurements::angleBetween(*robot, *ball), robot->getOrientation()));
   reward += (1/sqrt(2*M_PI))*exp(-2*(theta_r_b/(M_PI*M_PI)));


   return reward;
    // return m_hasKicked; // TODO: Actually this will always return false, needs to be fixed
}
bool GoToBallRL::isFinished(){
   return true;
    // return m_hasKicked; // TODO: Actually this will always return false, needs to be fixed
}
string GoToBallRL::getName(){
    return "Kick 2 point omni";
}


