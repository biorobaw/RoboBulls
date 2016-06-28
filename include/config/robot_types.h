#ifndef CONFIG_ROBOTTYPES_H
#define CONFIG_ROBOTTYPES_H

/*! @file The purpose of this file is to manage robot types.
 * @details The types of robots are differential, three wheel omni, or four
 * wheel omni. Since there is no way for the vision to detect whether
 * a robot is one or another, they must be manually specified. 
 * In this file, there is a map of robot IDs to their type.
 * In order to change the type, change the type next to the commented ID.
 * These values are then returned by robot->type() to get the robot type. */

//! @brief Possible types for a robot in terms of number of wheels
enum RobotType
{
	differential,
	threeWheelOmni,
	fourWheelOmni
};

//! @addtogroup config
//! @{

//! @brief An array indexed by ID to RobotType
extern const RobotType robotIDTypes[];

//! @brief Array indexed by ID as to if the robot has kicking capabilities
extern const bool robotHasKicker[];

/*! @brief The goalie robot's ID is given as an integer.
 * @details This ID can be changed with the `goalie_id` entry on
 * the utility command line. It is 5 by default (see .cpp)
 * It is a float to be changed dynamically, but only use ints */
extern float GOALIE_ID;
extern float DEFEND_1;
extern float DEFEND_2;
extern float ATTACK_1;
extern float ATTACK_2;

//! @}

#endif
