#ifndef MOTION_PARAMETERS
#define MOTION_PARAMETERS

// TODO: review this file and place parameters where they correspond, not here


namespace YAML {
    class Node;
}

//! The robot's radius in program points.
extern float ROBOT_RADIUS;

/*! How close the robot needs to be to the target in program points
 * before it is considered at that position */
extern float DIST_TOLERANCE;

/*! The max difference in angle for two angles in degrees to be
 * considered equal. Test this with Measurements::angleDiff. This
 * is also the default tolerance for the angle version of
 * Measurements::isClose */
extern float ROT_TOLERANCE;

/*! An extension beyond the defence area in which we consider a robot
 * to be inside the defence area. Used mostly for path planning */
extern float DEF_AREA_TOL;

/*! Variables used to control the speed of a robot dribbling the ball */
extern float DRIBBLE_FRWD_SPD;
extern float DRIBBLE_BACK_SPD;
extern float DRIBBLE_TURN_RATE;


void load_motion_parameters(YAML::Node* motion_node);

#endif // MOTION_PARAMETERS
