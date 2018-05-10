#ifndef CONFIG_TEAM_H
#define CONFIG_TEAM_H
//! @file

//! Option specifying team BLUE
#define TEAM_BLUE 0

//! Option specifying team YELLOW
#define TEAM_YELLOW 1

//! Specifies our goal is on the negative side of the global camera coordinate frame
#define SIDE_NEGATIVE 0

//! Specifies our goal is on the positive side of the global camera coordinate frame
#define SIDE_POSITIVE 1

//! @addtogroup config
//! @{

/*! The Program's Team (TEAM_BLUE or TEAM_YELLOW)
 * The player's team constant. This sets which team the program
 * will transmit to. XBee Channel D is for Blue, E is for Yellow */
#define OUR_TEAM TEAM_YELLOW


/*! The currnet Side of the program's team
 * This specifies if we are playing from the negative X axis
 * or the positive X axis. It applies transformations to the camera's
 * readings always making our goal on the negative X axis. The enemy's
 * goal is always at positive X. It should not be used anywhere else. */
#define SIDE SIDE_POSITIVE
//! @}

#endif
