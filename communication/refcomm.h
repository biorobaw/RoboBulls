#ifndef REFCOMM_H
#define REFCOMM_H

#include <QtCore/QThread>
#include "include/netraw.h"
#include "model/gamemodel.h"

using namespace std;

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

class RefComm : public QThread
{
public:
    /*! @brief Constructor
     * @param gm The GameModel to fill with information
     * @param net_ref_address Address Refbox is broadcasting to
     * @param port The port Refbox is broadcasting to */
    RefComm(GameModel *gm);

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
    * @details [RefBox legacy format](http://robocupssl.cpe.ku.ac.th/referee:legacy-protocol)
    * This is the formati of packet recieved by the RefBox. */
    struct Packet {
        char command;               //!<ASCII chatacter sent as the current comment
        unsigned char counter;      //!<Incremented when a new command is sent
        unsigned char goals_blue;   //!<Number of Blue goals
        unsigned char goals_yellow; //!<Number of Yellow goals
        unsigned short time_left;   //!<Remaining time (Seconds)?
    } lastPacket;
};

#endif // REFCOMM_H
