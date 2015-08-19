#ifndef ROBCOMM_H
#define ROBCOMM_H
#include <vector>
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
    //! @brief Get singleton instance of the RobComm
    static RobComm * getRobComm();

    //! @brief Required; send velocities to the entire team at once
    virtual void sendVelsLarge(std::vector<Robot*>&) = 0;

private:
    static RobComm* robcomm;     //!<Singleton instance pointer
};

#endif // ROBCOMM_H
