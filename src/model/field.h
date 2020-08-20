#ifndef FIELD_H
#define FIELD_H

#include "utilities/point.h"
#include "configuration/constants.h"

namespace YAML {
    class Node;
}


//#    NOTE: For more details see https://robocup-ssl.github.io/ssl-rules/sslrules.html#_field_setup
//#
//#    NOTE: Coordinate (0,0) is at the center of the field
//#     y axis
//#     ^                                            HALF_FIELD_LENGTH
//#     |                                         <---------------------->
//#     --->              -----------------------------------------------
//#    x axis            |                       |  ^                    |
//#                 ^    |-----                  |  | HALF          -----|
//#                 |    |     |                 |  | FIELD        |     |
//#                 |   -|     |                 |  | WIDTH        |     |-
//#            DEF  |  | |     |                o|o v              |     | |  ^  DEF
//#          LENGTH |  | |-----|--------------o--|--o--------------|-----| |  |  AREA
//#           AREA  |  | |     |                o|o                |     | |  V LENGTH
//#                 |   -|     |                 |                 |     |-
//#                 |    |     |                 |                 |     |
//#                 V    |-----                  |                  -----|
//#                      |                       |                       |
//#                       -----------------------------------------------
//#                       <--->                  |->
//#                   DEF_AREA_WIDTH       CENTER_RADIUS
//#





class Field {

public:


    static void load(YAML::Node* field_node);

    static Point  getPenaltyPosition(int on_field);
    static Point  getGoalPosition(int on_field);


    //! @brief Conversion factor from meters to field Point coordinates
    static int POINTS_PER_METER;

    /*! @brief Approximate ball radius in millimeters.*/
    static float BALL_RADIUS;



    //! @brief The goal-to-goal-line length
    static int FIELD_LENGTH;

    //! @brief The center-to-goal-line length (X-Axis) of the field
    static int HALF_FIELD_LENGTH;

    //! @brief The full side-to-side length (Y-Axis)
    static int FIELD_WIDTH;

    //! @brief The center-to-side length (Y-Axis) of the field
    static int HALF_FIELD_WIDTH;

    //! @brief Radius of the circle in the center of the field
    static int CENTER_RADIUS;



    //! @brief Distance from Y=0 to the centre of circular areas
    static int DEF_AREA_LENGTH;

    //! @brief Width  of the defence area (distance x direction)
    static int DEF_AREA_WIDTH;

    //! @brief Length of the goal post (distance y direction)
    static int GOAL_LENGTH;

    //! @brief  distance from goal at which penalties are taken
    static int PENALTY_MARK;

    //! @brief the positive side penalty mark x coodinate
    static int PENALTY_MARK_X;




    static int NEGATIVE_SIDE_TEAM;


};






#endif // FIELD_H
