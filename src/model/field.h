#ifndef FIELD_H
#define FIELD_H

#include "yaml-cpp/yaml.h"
#include "utilities/point.h"


#define FIELD_SIDE_NEGATIVE 0
#define FIELD_SIDE_POSITIVE 1

class Field {

public:


    static void load(YAML::Node field_node);

    static Point  getPenaltyPosition(int on_field);
    static Point  getGoalPosition(int on_field);


    /*! @brief Approximate ball radius in millimeters.*/
    static float BALL_RADIUS;

    //! @brief The center-to-goal-line length (X-Axis) of the field
    static int HALF_FIELD_LENGTH;

    //! @brief The full side-to-side length (X-Axis)
    static int FIELD_LENGTH;

    //! @brief The center-to-side length (Y-Axis) of the field
    static int HALF_FIELD_WIDTH;

    //! @brief The full goal-to-goal length (Y-Axis)
    static int FIELD_WIDTH;

    //! @brief Radius of circular corners of defence area
    //! Look at SSL rule book for clarification
    static int DEF_AREA_RADIUS;

    //! @brief Distance from Y=0 to the centre of circular areas
    //! //! Look at SSL rule book for clarification
    static int DEF_AREA_OFFSET;

    //! @brief Width of Goal Post
    static int GOAL_WIDTH;

    //! @brief Conversion factor from meters to field Point coordinates
    static int POINTS_PER_METER;

    //! @brief Conversion factor from points to meters
    static float METERS_PER_POINT;

    static int CENTER_RADIUS;




};






#endif // FIELD_H
