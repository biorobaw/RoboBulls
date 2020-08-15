#include "model/field.h"
#include "utilities/my_yaml.h"

// for details see: https://robocup-ssl.github.io/ssl-rules/sslrules.html#_field_setup

//! @brief Conversion factor from points to meters
int Field::POINTS_PER_METER  = 1000;

float Field::BALL_RADIUS = 21.5;


//! @brief The full goal-to-goal length (Y-Axis)
int Field::FIELD_LENGTH  = 9000;

//! @brief The center-to-goal-line length (X-Axis) of the field
int Field::HALF_FIELD_LENGTH = FIELD_LENGTH/2;

//! @brief The full side-to-side length (X-Axis)
int Field::FIELD_WIDTH = 6000;

//! @brief The center-to-side length (Y-Axis) of the field
int Field::HALF_FIELD_WIDTH = FIELD_WIDTH/2;

//! @brief Radius of the circle in the center of the field
int Field::CENTER_RADIUS = 500;



//! @brief Length of the defence area (distance y direction) # todo is this half or full length?
int Field::DEF_AREA_LENGTH = 2000;

//! @brief Width  of the defence area (distance x direction)
int Field::DEF_AREA_WIDTH = 1000;


//! @brief Width of Goal Post
int Field::GOAL_LENGTH = 1000;

//! @brief  distance from goal at which penalties are taken
int Field::PENALTY_MARK = 6000;

//! @brief the positive side penalty mark x coodinate
int Field::PENALTY_MARK_X = PENALTY_MARK_X    = HALF_FIELD_LENGTH - PENALTY_MARK;;



int Field::NEGATIVE_SIDE_TEAM = ROBOT_TEAM_BLUE;

void Field::load(YAML::Node* field_node){

    qInfo() << "--FIELD PARAMS" <<endl;
    qInfo() << "        POINTS_PER_METER -" << (*field_node)["POINTS_PER_METER"]  ;
    qInfo() << "        BALL_RADIUS      -" << (*field_node)["BALL_RADIUS"]       ;
    qInfo() << "        FIELD_LENGTH     -" << (*field_node)["FIELD_LENGTH"]      ;
    qInfo() << "        FIELD_WIDTH      -" << (*field_node)["FIELD_WIDTH"]       ;
    qInfo() << "        CENTER_RADIUS    -" << (*field_node)["CENTER_RADIUS"]     ;
    qInfo() << "        DEF_AREA_LENGTH  -" << (*field_node)["DEF_AREA_LENGTH"]   ;
    qInfo() << "        DEF_AREA_WIDTH   -" << (*field_node)["DEF_AREA_WIDTH"]    ;
    qInfo() << "        GOAL_LENGTH      -" << (*field_node)["GOAL_LENGTH"]       ;
    qInfo() << "        PENALTY_MARK     -" << (*field_node)["PENALTY_MARK"]      ;


    POINTS_PER_METER  = (*field_node)["POINTS_PER_METER"].as<int>();
    BALL_RADIUS       = (*field_node)["BALL_RADIUS"].as<float>();
    FIELD_LENGTH      = (*field_node)["FIELD_LENGTH"].as<int>();
    HALF_FIELD_LENGTH = FIELD_LENGTH/2;
    FIELD_WIDTH       = (*field_node)["FIELD_WIDTH"].as<int>();
    HALF_FIELD_WIDTH  = FIELD_WIDTH/2;
    CENTER_RADIUS     = (*field_node)["CENTER_RADIUS"].as<int>();
    DEF_AREA_WIDTH    = (*field_node)["DEF_AREA_WIDTH"].as<int>();
    DEF_AREA_LENGTH   = (*field_node)["DEF_AREA_LENGTH"].as<int>();
    GOAL_LENGTH       = (*field_node)["GOAL_LENGTH"].as<int>();
    PENALTY_MARK      = (*field_node)["PENALTY_MARK"].as<int>();
    PENALTY_MARK_X    = HALF_FIELD_LENGTH - PENALTY_MARK;

    qInfo() << "--Field Parameters DONE" ;
}

Point  Field::getPenaltyPosition(int on_side){
    return on_side==FIELD_SIDE_POSITIVE ?
                Point( PENALTY_MARK_X, 22) :
                Point(-PENALTY_MARK_X, 22);
}

Point  Field::getGoalPosition(int on_side){
    return on_side==FIELD_SIDE_POSITIVE ?
                Point( HALF_FIELD_LENGTH, 0) :
                Point(-HALF_FIELD_LENGTH, 0);
}

