#ifndef ROBCOMM_H
#define ROBCOMM_H
#include <vector>

class Robot;

/*
 * RobComm
 * It is used to send signals to robots.
 * Base class. NxtRobcomm is used for field robots, SimRobcomm for simulator.
 * Narges Ghaedi ``et al"
 */
class RobComm
{
public:
    static RobComm * getRobComm();

    //Required; send velocities to the entire team at once
    virtual void sendVelsLarge(std::vector<Robot*>&) = 0;

private:
    static RobComm * robcomm;
};

#endif // ROBCOMM_H
