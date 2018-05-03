#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#include "include/config/simulated.h"

/*! @file
 * @brief Address and port configurations for the communication modules
 * @addtogroup config
 * @{
 */

/*! @brief Are we processing packets from four cameras? */
#define FOUR_CAMERA 0

//! @brief Vision listen address options
//! @{
#define VISION_ADDRESS_ABDUL "224.5.23.2"
#define VISION_ADDRESS_SHAMSI "224.5.23.4"
#define VISION_ADDRESS_JAMES  "224.5.23.3"
#define VISION_ADDRESS_ADRIANO "224.5.23.1"

//Should not be the same as any of the simulators
#define VISION_ADDRESS_BIOROB "224.5.23.2"
#define VISION_ADDRESS_ROBOCUP "224.5.23.2"
//! @}

//! @brief Vision listen address options
//! @{
#define SIMULATOR_ADDRESS_JAMES  "131.247.14.106"
#define SIMULATOR_ADDRESS_SHAMSI "131.247.14.101"
#define SIMULATOR_ADDRESS_LOCAL  "127.0.0.1"
//! @}

//! @brief Comunication configurations
//! @{

#define SIMULATOR_ADDRESS   SIMULATOR_ADDRESS_LOCAL
#define SIMULATOR_PORT      20011

#if SIMULATED
 #define VISION_ADDRESS     VISION_ADDRESS_ABDUL
 #define VISION_PORT        10020
#else
 #define VISION_ADDRESS     VISION_ADDRESS_BIOROB
 #define VISION_PORT        10002
#endif

#define REFBOX_ADDRESS      "224.5.23.1"
#define REFBOX_PORT         10001

//!@}

//! @brief Referee Box--do we listen to commands? See StrategyController
//! @details Chooses between listening to RefComm, and always choosing TestStrategy.
#define REFBOX_LISTEN_ENABLED 0

//!@}

#endif // COMMUNICATION_H
