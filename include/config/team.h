#ifndef CONFIG_TEAM_H
#define CONFIG_TEAM_H

/*! @file The player's team.
 * @details 0 is BLUE, 1 is Yellow.
 * In the case of having two teams, compile one program with TEAM
 * set to TEAM_BLUE, and one program with TEAM set to TEAM_YELLOW. */

//! @brief Option specifying team BLUE
#define TEAM_BLUE   0

//! @brief Option specifying team YELLOW
#define TEAM_YELLOW 1

//! @brief The Program's Team constant (make change here)
#define TEAM TEAM_BLUE

#endif
