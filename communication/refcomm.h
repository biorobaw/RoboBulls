#ifndef REFCOMM_H
#define REFCOMM_H

#include <QtCore/QThread>
#include "include/netraw.h"
#include "model/gamemodel.h"

using namespace std;

/*! @brief Communication module to receive information from the RefBox
 * @author Origin. Narges Ghaedi, JamesW
 *@details
 * Retrieves and parses a legacy packet from the SSL Referee Box,
 * and fills out the GameModel.
 * <b>Referee Box Command Quick Reference:</b>
 * Command type               Command
 * ==========================================================
 * <<Control commands>>
 *     Halt                         H
 *     Stop                         S
 *     Ready                        ' ' (space)
 *     Start                        s
 * <<Game Notifications>>
 *     Begin first half             1
 *     Begin half time              h
 *     Begin second half            2
 *     Begin overtime half 1        o
 *     Begin overtime half 2        O
 *     Begin penalty shootout       a
 *
 * Command type      Yellow Team Command     Blue Team Command
 * ==========================================================
 * <<Game restarts>>
 *     Kick off                    k                     K
 *     Penalty                     p                     P
 *     Direct Free kick            f                     F
 *     Indirect Free kick          i                     I
 * <<Extras>>
 *     Timeout                     t                     T
 *     Timeout end                 z                     z
 *     Goal scored                 g                     G
 *     decrease Goal score         d                     D
 *     Yellow Card                 y                     Y
 *     Red Card                    r                     R
 *     Cancel                      c
 */

class RefComm : public QThread
{
public:
    /*! @brief Constructor
     * @param gm The GameModel to fill with information
     * @param net_ref_address Address Refbox is broadcasting to
     * @param port The port Refbox is broadcasting to */
    RefComm(GameModel *gm,
            int port = 10001,
            string net_ref_address="224.5.23.1",
            string net_ref_interface="");

   ~RefComm();

    //communicating with reciver
    bool open(bool blocking=false);
    void close();
    bool receive();
    void run();

protected:
    Net::UDP mc; // multicast client
    int    _port;
    string _net_address;
    string _net_interface;
    GameModel *gamemodel;

private:
   /*! @brief The legacy Refbox packet
    * @details
    * The packet format is as follows:
    * +---------+---------+------------+--------------+-------------------+
    * | Command | Counter | Goals blue | Goals yellow | time left (16 Bit)|
    * +---------+---------+------------+--------------+-------------------+ */
    struct Packet {
        char command;
        unsigned char counter;
        unsigned char goals_blue;
        unsigned char goals_yellow;
        unsigned short time_left;
    } lastPacket;
};

#endif // REFCOMM_H
