#ifndef ROBCOMM_H
#define ROBCOMM_H
#include <set>
#include "yaml-cpp/yaml.h"
class Robot;

 /*! @brief RobComm is a base class used to send signals to robots.
 * @details RobComm requires one virtual function be imeplemented which
 * actually sends out packets given a vector of robots.
 * NxtRobcomm is used for field robots, SimRobcomm for simulator.
 * @author Narges Ghaedi
 */

class RobComm
{
public:

    /*! @brief Required; send velocities to the entire team at once
     * @details Given a vector of Robot, RobComms implementing this funciton
     * are to send out packets to these robots, sending their information such
     * as their wheel velocities and kick/dribble status */


    static void open_communication(YAML::Node team_node);
    static void close_communication(std::set<Robot*>& robots);
    static void sendVels(std::set<Robot*>& robots);

private:
    static RobComm* robcomm;     //!<Singleton instance pointer
    virtual void sendVelsLarge(std::set<Robot*>&) = 0;
    virtual void close() = 0;
};

#endif // ROBCOMM_H
