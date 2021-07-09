#ifndef ROBCOMM_H
#define ROBCOMM_H

#include <QObject>

class Robot;
class RobotPilot;

namespace YAML {
    class Node;
}


 /*! @brief RobComm is a base class used to send signals to robots.
 * @details RobComm requires one virtual function be imeplemented which
 * actually sends out packets given a vector of robots.
 * NxtRobcomm is used for field robots, SimRobcomm for simulator.
 * @author Narges Ghaedi
 */

class RobotImplementation : public QObject
{
    Q_OBJECT
public:

    // === CONSTRUCTORS AND DESTRUCTORS =============================

    static RobotImplementation* load(YAML::Node* comm_node);
    virtual ~RobotImplementation();

    // === COMMUNICATION WITH ROBOTS ================================

    void stopAndClose(const QSet<Robot*>& robots);
    virtual void sendCommands(const QSet<Robot*>& robots) = 0;

    // === FUNCTIONS THAT DEPEND ON THE CAPABILITIES OF THE ROBOT ===

    virtual bool hasKicker() = 0;
    virtual bool isHolonomic() = 0;
    virtual RobotPilot* createPilot(Robot* robot) = 0;

    virtual QString getName() =0;

protected:
    virtual void close() = 0;

};

#endif // ROBCOMM_H
