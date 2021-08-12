#define _USE_MATH_DEFINES
#include <cmath>
#include "configuration/motion_parameters.h"
#include <QDebug>
#include "utilities/my_yaml.h"


float ROBOT_RADIUS      = 100;
float DEF_AREA_TOL      = ROBOT_RADIUS + 10;
float DIST_TOLERANCE    = 5;
float ROT_TOLERANCE     = 5*M_PI/180;
float DRIBBLE_FRWD_SPD  = 70;
float DRIBBLE_BACK_SPD  = 7;
float DRIBBLE_TURN_RATE = 0.1;


void load_motion_parameters(YAML::Node* motion_node){

    qInfo() << "--Motion Parameters: ";
    qInfo() << "        ROBOT_RADIUS      -" << (*motion_node)["ROBOT_RADIUS"]     ;
    qInfo() << "        DIST_TOLERANCE    -" << (*motion_node)["DIST_TOLERANCE"]   ;
    qInfo() << "        ROT_TOLERANCE     -" << (*motion_node)["ROT_TOLERANCE"]    ;
    qInfo() << "        DRIBBLE_FRWD_SPD  -" << (*motion_node)["DRIBBLE_FRWD_SPD"] ;
    qInfo() << "        DRIBBLE_BACK_SPD  -" << (*motion_node)["DRIBBLE_BACK_SPD"] ;
    qInfo() << "        DRIBBLE_TURN_RATE -" << (*motion_node)["DRIBBLE_TURN_RATE"];

    ROBOT_RADIUS      = (*motion_node)["ROBOT_RADIUS"].as<float>();
    DEF_AREA_TOL      = ROBOT_RADIUS + 10;
    DIST_TOLERANCE    = (*motion_node)["DIST_TOLERANCE"].as<float>()   ;
    ROT_TOLERANCE     = (*motion_node)["ROT_TOLERANCE"].as<float>()*M_PI/180;
    DRIBBLE_FRWD_SPD  = (*motion_node)["DRIBBLE_FRWD_SPD"].as<float>() ;
    DRIBBLE_BACK_SPD  = (*motion_node)["DRIBBLE_BACK_SPD"].as<float>() ;
    DRIBBLE_TURN_RATE = (*motion_node)["DRIBBLE_TURN_RATE"].as<float>();

    qInfo() << "--Motion Parameters DONE";


}
