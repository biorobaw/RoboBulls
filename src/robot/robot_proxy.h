#ifndef ROBCOMM_H
#define ROBCOMM_H
#include <set>
#include <QList>
class Robot;
class Pilot;

namespace YAML {
    class Node;
}


 /*! @brief RobComm is a base class used to send signals to robots.
 * @details RobComm requires one virtual function be imeplemented which
 * actually sends out packets given a vector of robots.
 * NxtRobcomm is used for field robots, SimRobcomm for simulator.
 * @author Narges Ghaedi
 */

class RobotProxy
{
public:

    // === CONSTRUCTORS AND DESTRUCTORS =============================

    static RobotProxy* load(QString robot_type,YAML::Node* comm_node);
    virtual ~RobotProxy();

    // === COMMUNICATION WITH ROBOTS ================================

    void close_communication(const QSet<Robot*>& robots);
    virtual void sendVels(const QSet<Robot*>& robots) = 0;

    // === FUNCTIONS THAT DEPEND ON THE CAPABILITIES OF THE ROBOT ===

    virtual bool hasKicker() = 0;
    virtual bool isHolonomic() = 0;
    virtual Pilot* createPilot(Robot* robot) = 0;


private:
    virtual void close() = 0;

};

#endif // ROBCOMM_H
