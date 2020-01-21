#ifndef FIELD
#define FIELD

#include "yaml-cpp/yaml.h"


/*! @brief Approximate ball radius in millimeters.*/
extern float BALL_RADIUS;

//! @brief The center-to-goal-line length (X-Axis) of the field
extern int HALF_FIELD_LENGTH;

//! @brief The full side-to-side length (X-Axis)
extern int FIELD_LENGTH;

//! @brief The center-to-side length (Y-Axis) of the field
extern int HALF_FIELD_WIDTH;

//! @brief The full goal-to-goal length (Y-Axis)
extern int FIELD_WIDTH;

//! @brief Radius of circular corners of defence area
//! Look at SSL rule book for clarification
extern int DEF_AREA_RADIUS;

//! @brief Distance from Y=0 to the centre of circular areas
//! //! Look at SSL rule book for clarification
extern int DEF_AREA_OFFSET;

//! @brief Width of Goal Post
extern int GOAL_WIDTH;

//! @brief Conversion factor from meters to field Point coordinates
extern int POINTS_PER_METER;

//! @brief Conversion factor from points to meters
extern float METERS_PER_POINT;

extern int CENTER_RADIUS;


void load_field_parameters(YAML::Node field_node);




#endif // FIELD

