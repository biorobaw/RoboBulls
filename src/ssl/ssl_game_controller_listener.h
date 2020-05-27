#ifndef REFCOMM_H
#define REFCOMM_H

#include <QtCore/QThread>
#include <QUdpSocket>
#include <atomic>

using namespace std;

namespace YAML {
    class Node;
}

/*! @brief Communication module to receive information from the RefBox
 * @author Origin. Narges Ghaedi, JamesW
 * @details
 * Retrieves and parses a legacy packet from the
 * [SSL Referee Box](http://robocupssl.cpe.ku.ac.th/referee:start),
 * and fills out the GameModel.<br><br>
 * <b>Referee Box Command Quick Reference:</b>
 */

class SSLGameControllerListener : public QThread
{
public:
    /*! @brief Constructor
     * @param gm The GameModel to fill with information
     * @param net_ref_address Address Refbox is broadcasting to
     * @param port The port Refbox is broadcasting to */
    SSLGameControllerListener(YAML::Node* comm_node);

    void run() override;
    void stop();

private:
    std::atomic_bool done;
    string net_address;
    int    port;

};

#endif // REFCOMM_H
