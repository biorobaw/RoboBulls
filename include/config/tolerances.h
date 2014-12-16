#ifndef CONFIG_TOLERANCES_H
#define CONFIG_TOLERANCES_H

/* The robot's radius in program points.
 * ROBOT_SIZE is the diameter plus a small overshoot
 */
extern const int ROBOT_RADIUS;
extern const int ROBOT_SIZE;

/* DIST_TOLERANCE: How close the robot needs to be to the target
 * before it is considered at that position
 */
extern const int DIST_TOLERANCE;

/* ROT_TOLERANCE: The max difference in angle for two angles to be 
 * considered equal. Test this with Measurements::angleDiff
 * This is also the default tolerance for the angle version
 * of Measurements::isClose
 */
extern const int ROT_TOLERANCE;

#endif
