#ifndef CONFIG_TEAM_H
#define CONFIG_TEAM_H
//! @file

//! @brief Option specifying team BLUE
#define TEAM_BLUE   0

//! @brief Option specifying team YELLOW
#define TEAM_YELLOW 1

//! @addtogroup config
//! @{

/*! @brief The Program's Team constant (TEAM_BLUE or TEAM_YELLOW)
 * @details The player's team. In the case of having two teams,
 * compile one program with TEAM set to TEAM_BLUE, and one
 * program with TEAM set to TEAM_YELLOW. */

#define TEAM TEAM_BLUE

//! @}

#endif
