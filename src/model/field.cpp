#include "model/field.h"
#include <iostream>
#include "yaml-cpp/yaml.h"
using namespace std;

float Field::BALL_RADIUS = 21.5;

//! @brief The center-to-goal-line length (X-Axis) of the field
int Field::HALF_FIELD_LENGTH = 4500;

//! @brief The full side-to-side length (X-Axis)
int Field::FIELD_LENGTH  = (HALF_FIELD_LENGTH*2);

//! @brief The center-to-side length (Y-Axis) of the field
int Field::HALF_FIELD_WIDTH = 3000;

//! @brief The full goal-to-goal length (Y-Axis)
int Field::FIELD_WIDTH = (HALF_FIELD_WIDTH*2);

//! @brief Radius of circular corners of defence area
//! Look at SSL rule book for clarification
int Field::DEF_AREA_RADIUS = 1000;

//! @brief Distance from Y=0 to the centre of circular areas
//! //! Look at SSL rule book for clarification
int Field::DEF_AREA_OFFSET = 250;

//! @brief Width of Goal Post
int Field::GOAL_WIDTH = 1000;

//! @brief Conversion factor from meters to field Point coordinates
int Field::POINTS_PER_METER  = 1000;

//! @brief Conversion factor from points to meters
float Field::METERS_PER_POINT = (1.0 / POINTS_PER_METER);

int Field::CENTER_RADIUS = 500;


int Field::NEGATIVE_SIDE_TEAM = ROBOT_TEAM_BLUE;

void Field::load(YAML::Node* field_node){

    cout<< "--FIELD PARAMS" <<endl;
    cout<< "        BALL_RADIUS       - " << (*field_node)["BALL_RADIUS"]       <<endl;
    cout<< "        HALF_FIELD_LENGTH - " << (*field_node)["HALF_FIELD_LENGTH"] <<endl;
    cout<< "        HALF_FIELD_WIDTH  - " << (*field_node)["HALF_FIELD_WIDTH"]  <<endl;
    cout<< "        DEF_AREA_RADIUS   - " << (*field_node)["DEF_AREA_RADIUS"]   <<endl;
    cout<< "        DEF_AREA_OFFSET   - " << (*field_node)["DEF_AREA_OFFSET"]   <<endl;
    cout<< "        GOAL_WIDTH        - " << (*field_node)["GOAL_WIDTH"]        <<endl;
    cout<< "        CENTER_RADIUS     - " << (*field_node)["CENTER_RADIUS"]     <<endl;
    cout<< "        POINTS_PER_METER  - " << (*field_node)["POINTS_PER_METER"]  <<endl;


    BALL_RADIUS       = (*field_node)["BALL_RADIUS"].as<float>();
    HALF_FIELD_LENGTH = (*field_node)["HALF_FIELD_LENGTH"].as<int>();
    FIELD_LENGTH      = 2*HALF_FIELD_LENGTH;
    HALF_FIELD_WIDTH  = (*field_node)["HALF_FIELD_WIDTH"].as<int>();
    FIELD_WIDTH       = 2*HALF_FIELD_WIDTH;
    DEF_AREA_RADIUS   = (*field_node)["DEF_AREA_RADIUS"].as<int>();
    DEF_AREA_OFFSET   = (*field_node)["DEF_AREA_OFFSET"].as<int>();
    GOAL_WIDTH        = (*field_node)["GOAL_WIDTH"].as<int>();
    CENTER_RADIUS     = (*field_node)["CENTER_RADIUS"].as<int>();
    POINTS_PER_METER  = (*field_node)["POINTS_PER_METER"].as<int>();
    METERS_PER_POINT  = 1.0 / POINTS_PER_METER;

    cout << "--Field Parameters DONE" << endl;
}

Point  Field::getPenaltyPosition(int on_side){
    return on_side==FIELD_SIDE_POSITIVE ?
                Point(2045, 22) :
                Point(-2045, 22);
}

Point  Field::getGoalPosition(int on_side){
    return on_side==FIELD_SIDE_POSITIVE ?
                Point(HALF_FIELD_LENGTH, 0) :
                Point(-HALF_FIELD_LENGTH, 0);
}

