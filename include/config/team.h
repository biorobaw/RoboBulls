#ifndef CONFIG_TEAM_H
#define CONFIG_TEAM_H
//! @file

//! @brief Option specifying team BLUE
#define TEAM_BLUE   0

//! @brief Option specifying team YELLOW
#define TEAM_YELLOW 1

//! @addtogroup config
//! @{

/*! @brief The Program's Team (TEAM_BLUE or TEAM_YELLOW)
 * @details The player's team constant. This sets which team the program
 * will transmit to. XBee Channel D is for Blue, E is for Yellow */

#define TEAM TEAM_BLUE

//! @}

#endif
