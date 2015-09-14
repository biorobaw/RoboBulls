#ifndef COMMUNICATION_H
#define COMMUNICATION_H

//! @file Address and port configurations for the communication moudles
//! @addtogroup config
//! @{

//! @brief Vision listen address options
//! @{
#define VISION_ADDRESS_SHAMSI "224.5.23.5"
#define VISION_ADDRESS_JAMES  "224.5.23.3"
//! @}

//! @brief Vision listen address options
//! @{
#define SIM_ADDRESS_JAMES  "131.247.14.106"
#define SIM_ADDRESS_SHAMSI "131.247.14.101"
#define SIM_ADDRESS_LOCAL  "127.0.0.1"
//! @}

//! @brief Simulator address
#define SIMULATOR_ADDRESS SIM_ADDRESS_LOCAL
//! @brief Simulator port
#define SIMULATOR_PORT 20011

//! @brief Vision listen address
#define VISION_ADDRESS VISION_ADDRESS_JAMES
//! @brief Vision listen address port
#define VISION_PORT 10020

//! @brief Referee Box broadcast address
#define REFBOX_ADDRESS "224.5.23.1"
//! @brief Referee Box broadcast address port
#define REFBOX_PORT 10001
//! @brief Referee Box--do we listen to commands? See StrategyController
//! @defails Chooses between listening to RefComm, and always choosing TestStrategy.
#define REFBOX_LISTEN_ENABLED 1

//@}

#endif // COMMUNICATION_H
