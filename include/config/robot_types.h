#ifndef CONFIG_ROBOTTYPES_H
#define CONFIG_ROBOTTYPES_H

/* The purpose of this file is to (1) declare an enum for robot types,
 * and (2) give a constant ID for the goalie robot
 *
 * (1)
 * The types of robots are differential, three wheel omni, or four
 * wheel omni. Since there is no way for the vision to detect whether
 * a robot is one or another, they must be manually specified. 
 * In this file, there is a map of robot IDs to their type.
 * In order to change the type, change the type next to the commented ID.
 * These values are then returned by robot->type() to get the robot type.
 */
enum RobotType
{
	differential,
	threeWheelOmni,
	fourWheelOmni
};

extern const RobotType robotIDTypes[];

/* (2)
 * The goalie robot's ID is given as an integer.
 * This ID can be changed with the `goalie_id` integer on
 * the utility command line. It is 5 by default (see .cpp)
 */
extern int GOALIE_ID;

#endif
