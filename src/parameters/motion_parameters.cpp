#define _USE_MATH_DEFINES
#include <cmath>
#include "parameters/motion_parameters.h"
#include <iostream>
using namespace std;





float ROBOT_RADIUS      = 100;
float DEF_AREA_TOL      = ROBOT_RADIUS + 10;
float DIST_TOLERANCE    = 40;
float ROT_TOLERANCE     = 5*M_PI/180;
float DRIBBLE_FRWD_SPD  = 70;
float DRIBBLE_BACK_SPD  = 7;
float DRIBBLE_TURN_RATE = 0.1;


void load_motion_parameters(YAML::Node motion_node){

    cout << "--Motion Parameters: " << endl;
    cout << "        ROBOT_RADIUS      - " << motion_node["ROBOT_RADIUS"]     << endl;
    cout << "        DIST_TOLERANCE    - " << motion_node["DIST_TOLERANCE"]   << endl;
    cout << "        ROT_TOLERANCE     - " << motion_node["ROT_TOLERANCE"]    << endl;
    cout << "        DRIBBLE_FRWD_SPD  - " << motion_node["DRIBBLE_FRWD_SPD"] << endl;
    cout << "        DRIBBLE_BACK_SPD  - " << motion_node["DRIBBLE_BACK_SPD"] << endl;
    cout << "        DRIBBLE_TURN_RATE - " << motion_node["DRIBBLE_TURN_RATE"]<< endl;

    ROBOT_RADIUS      = motion_node["ROBOT_RADIUS"].as<float>();
    DEF_AREA_TOL      = ROBOT_RADIUS + 10;
    DIST_TOLERANCE    = motion_node["DIST_TOLERANCE"].as<float>()   ;
    ROT_TOLERANCE     = motion_node["ROT_TOLERANCE"].as<float>()*M_PI/180;
    DRIBBLE_FRWD_SPD  = motion_node["DRIBBLE_FRWD_SPD"].as<float>() ;
    DRIBBLE_BACK_SPD  = motion_node["DRIBBLE_BACK_SPD"].as<float>() ;
    DRIBBLE_TURN_RATE = motion_node["DRIBBLE_TURN_RATE"].as<float>();

    cout << "--Motion Parameters DONE" << endl;


}
