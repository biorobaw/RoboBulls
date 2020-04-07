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
 * Command Type                 |   Command
 * -----------------------------|----------
 *     Halt                     |   H
 *     Stop                     |   S
 *     Ready                    |   ' ' (space)
 *     Start                    |   s
 *     Begin first half         |   1
 *     Begin half time          |   h
 *     Begin second half        |   2
 *     Begin overtime half 1    |   o
 *     Begin overtime half 2    |   O
 *     Begin penalty shootout   |   a
 *
 * Command Type  |   Yellow Team Command     |  Blue Team Command
 * --------------|---------------------------|-------------------
 *     Kick off             |       k        |           K
 *     Penalty              |       p        |           P
 *     Direct Free kick     |       f        |           F
 *     Indirect Free kick   |       i        |           I
 *     Timeout              |       t        |           T
 *     Timeout end          |       z        |           z
 *     Goal scored          |       g        |           G
 *     decrease Goal score  |       d        |           D
 *     Yellow Card          |       y        |           Y
 *     Red Card             |       r        |           R
 *     Cancel               |       c        |           c
 */

class SSLRefBoxListener : public QThread
{
public:
    /*! @brief Constructor
     * @param gm The GameModel to fill with information
     * @param net_ref_address Address Refbox is broadcasting to
     * @param port The port Refbox is broadcasting to */
    SSLRefBoxListener(YAML::Node* comm_node);

    void run() override;
    void stop();

private:
    std::atomic_bool done;
    string net_address;
    int    port;

};

#endif // REFCOMM_H
