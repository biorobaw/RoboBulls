#include "gotoballandshootRL.h"
#include "model/game_state.h"
#include "utilities/comparisons.h"
#include "utilities/debug.h"
#include "model/ball.h"
#include "model/field.h"
#include "model/robot/robot.h"
#include "model/robot/navigation/robot_pilot.h"
#include "model/game_state.h"
#include "model/team/team.h"
#include "utilities/measurements.h"
#include <iostream>

#include <QDebug>
/*
 *Utilizing a similar state action space as pruposed in: https://zhuyifengzju.github.io/files/2018Robocup.pdf
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

#include <chrono>

//std::chrono::time_point<std::chrono::high_resolution_clock> time_start, time_stop, time_last;
/************************************************************************/


GoToBallAndShootRL::GoToBallAndShootRL(Robot* robot, float targetTolerance)
    : Behavior(robot)
{

    start_flag = true;
    end_flag = false;

    go_to_ball_skill = new GoToBallRL(robot);
    shoot_to_goal_skill = new ShootToGoalRL(robot);

    state = GOTOBALL;

    write_file = "C:\\Users\\justi\\Downloads\\GTBDDPGTime.csv";
    file_out.open(write_file, std::ios_base::app);

}



bool GoToBallAndShootRL::perform(){
//    auto cycle_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()-time_start);
//    qInfo()<<"cycle time: " <<cycle_time.count()/1000.0;
    if(start_flag){
         time_start =std::chrono::high_resolution_clock::now();
         start_flag = false;
         qInfo() <<"Start:";
    }
    else if(end_flag){
        qInfo() <<"End:";
        auto dur_to_get_ball = std::chrono::duration_cast<std::chrono::milliseconds>(time_got_ball-time_start).count()/1000.f;
        auto dur_to_shoot = std::chrono::duration_cast<std::chrono::milliseconds>(time_shot-time_got_ball).count()/1000.f;
        auto dur_total = std::chrono::duration_cast<std::chrono::milliseconds>(time_shot-time_start).count()/1000.f;
        qInfo() <<"Time taken to get ball: "<<dur_to_get_ball <<"\nTime taken to shoot: "<<dur_to_shoot<<"Total time: "<<dur_total;

        string write_str =  std::to_string(dur_to_get_ball)+','+std::to_string(dur_to_shoot)+','+std::to_string(dur_total);
        end_flag=false; start_flag=true;

        qInfo() <<"Hit enter, then hit y or n if the shot was sucessful:";std::string input;      std::cin >> input;
        if(input[0] == 'y')
                file_out<<write_str<<",YES"<<std::endl;
        else    file_out<<write_str<<",NO" <<std::endl;
        return false;
    }

    switch(state){
        case GOTOBALL:
        {
            qInfo() << "GoToBall";
            go_to_ball_skill->perform();

            if(Measurements::distance(*robot, *ball) < 200)
                state = PICKUPBALL;
            break;
        }
        case PICKUPBALL:
        {
        qInfo() << "PickupBall";
            robot->setDribble(true);

            float angle_to_ball = Measurements::angleDiff(Measurements::angleBetween(*robot, *ball), robot->getOrientation());

            robot->setTargetVelocityLocal(Point(500, 0), angle_to_ball*.25);

            if(robot->hasBall()){
                state=SHOOTTOGOAL;
                time_got_ball = std::chrono::high_resolution_clock::now();
            }
            else if((Measurements::distance(*robot, *ball) >= 200))
                state = GOTOBALL;
            break;


        }
        case SHOOTTOGOAL:
        {
            qInfo() << "ShootToGoal";
            shoot_to_goal_skill->perform();

            if(!robot->hasBall()){
                state=GOTOBALL;
                end_flag = true;
                //start_flag = true;
                time_shot = std::chrono::high_resolution_clock::now();
            //end
            }
            break;
        }
    }

//    time_start = std::chrono::high_resolution_clock::now();

    //Set done
    return false;
}






bool GoToBallAndShootRL::isFinished(){
   return done;
}

string GoToBallAndShootRL::getName(){
    return "Go to ball RL";
}


